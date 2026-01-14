#include "Aimbot.h"
#include "../../../sdk/entity/EntityManager.h"
#include "../../../sdk/utils/Utils.h"
#include "../../../sdk/memory/PatternScan.h"
#include "../../../sdk/memory/Offsets.h"
#include "../../../sdk/utils/Globals.h"
#include "../../../feature/combat/Combat.h"

#include <Windows.h>


void Aimbot::run()
{
	// this allows standard functionality
	if (!Globals::aimbot_enabled)
	{
		if (Combat::isMB1Held())
		{
			Combat::holdFire();
		}
		else
		{
			Combat::releaseFire();
		}
		return;
	}

	C_CSPlayerPawn* local = EntityManager::Get().GetLocalPawn();
	if (!local || !local->IsAlive())
	{
		// if player is dead but MB1 is pressed, pass through to 'L'
		if (Combat::isMB1Held())
		{
			Combat::holdFire();
		}
		else
		{
			Combat::releaseFire();
		}
		return;
	}


	// If MB1 is not pressed, release fire and return
	if (!Combat::isMB1Held())
	{
		Combat::releaseFire();
		return;
	}

	
	C_CSPlayerPawn* bestTarget = Combat::getBestTarget(local);

	if (!bestTarget)
	{
		// no target found 
		Combat::holdFire();
		return;
	}

	aimAtTarget(local, bestTarget);
}

// aimbot exclusive thing
/*
TODO: There is a lot of encapsulations created by COmbat fix this shit
*/
void Aimbot::aimAtTarget(C_CSPlayerPawn* local, C_CSPlayerPawn* target)
{
	static DWORD lastAimTime = 0;
	static DWORD lastShootTime = 0;
	static bool isAiming = false;

	if (!local || !target) return;

	uintptr_t client = Memory::GetModuleBase("client.dll");
	if (!client) return;

	Vector* currentAngles = reinterpret_cast<Vector*>(client + Offsets::dwViewAngles);
	if (!currentAngles) return;

	bool validBaim = Globals::aimbot_force_baim && target->m_iHealth() <= Globals::aimbot_baim_min;
	BoneID targetBone = Combat::findNearestBoneId(local, target, validBaim);
	Vector targetPos = Utils::GetBonePos(target, targetBone);
	if (targetPos.IsZero()) return;
	Vector localPos = local->m_vOldOrigin() + local->m_vecViewOffset();
	Vector aimAngles = Utils::CalcAngle(localPos, targetPos);
	Vector delta = aimAngles - *currentAngles;
	Utils::NormalizeAngles(delta);




	// apply smoothing and update aim FIRST
	Vector smoothedDelta = delta * (1.f - Globals::aimbot_smoothness);
	*currentAngles = Globals::aimbot_smooth ? *currentAngles + smoothedDelta : aimAngles; // here is where the magic happens

	// calculate the REMAINING delta magnitude (how close we are to target)
	float deltaX = fabsf(delta.x);
	float deltaY = fabsf(delta.y);
	float totalDelta = sqrtf(deltaX * deltaX + deltaY * deltaY);



	DWORD currentTime = GetTickCount();
	bool isOnTarget = totalDelta <= Globals::aimbot_shoot_threshold;

	if (isOnTarget)
	{
		if (!isAiming)
		{
			isAiming = true;
			lastAimTime = currentTime;
		}

		DWORD timeOnTarget = currentTime - lastAimTime;
		DWORD timeSinceShot = currentTime - lastShootTime;

		if (timeOnTarget >= Globals::aimbot_shoot_delay && timeSinceShot >= Globals::aimbot_fire_rate)
		{
			Combat::clickFire();
			lastShootTime = currentTime;
		}
	}
	else
	{
		isAiming = false;
	}
}




