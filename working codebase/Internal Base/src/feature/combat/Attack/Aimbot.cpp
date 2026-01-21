#include "Aimbot.h"
#include "../../../sdk/entity/EntityManager.h"
#include "../../../sdk/utils/Utils.h"
#include "../../../sdk/memory/PatternScan.h"
#include "../../../sdk/memory/Offsets.h"
#include "../../../sdk/utils/Globals.h"
#include "../../../feature/combat/Combat.h"
#include "../../../feature/combat/Recoil/Recoil.h"
#include <Windows.h>
#include "../../../core/HackManager.h"

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

	// apply standalone recoil control (when NOT aimbotting)
	if (Globals::norecoil_enabled)
	{
		Recoil::Run(local);
	}

	// if MB1 is not pressed, release fire and return
	if (!Combat::isMB1Held())
	{
		Combat::releaseFire();
		return;
	}

	C_CSPlayerPawn* bestTarget = Combat::GetBestTarget(local);

	if (!bestTarget)
	{
		// no target found
		Combat::holdFire();
		return;
	}

	aimAtTarget(local, bestTarget);
}

void Aimbot::aimAtTarget(C_CSPlayerPawn* local, C_CSPlayerPawn* target)
{
	static DWORD lastAimTime = 0;
	static DWORD lastShootTime = 0;
	static bool isAiming = false;
	static Vector oldPunch{};

	if (!local || !target) return;


	if (!HackManager::g_client) return;

	Vector* currentAngles = reinterpret_cast<Vector*>(HackManager::g_client + Offsets::dwViewAngles);
	if (!currentAngles) return;

	bool validBaim = Globals::aimbot_force_baim && target->m_iHealth() <= Globals::aimbot_baim_min;
	BoneID targetBone = Combat::FindNearestBoneId(local, target, validBaim);
	Vector targetPos = Utils::GetBonePos(target, targetBone);

	if (targetPos.IsZero()) return;
	Vector localPos	= local->m_vOldOrigin() + local->m_vecViewOffset();
	Vector aimAngles = Utils::CalcAngle(localPos, targetPos);
	Vector delta = aimAngles - *currentAngles;

	Utils::NormalizeAngles(delta);

	// get recoil compensation
	Vector recoilDelta{};
	if (Globals::norecoil_enabled)
	{
		// local, oldPunch ptr and set to -> currentPunch 
		Recoil::VectorAngleRun(local, oldPunch, recoilDelta);
		
	}

	// apply smoothing and update aim FIRST, accounting for recoil
	Vector smoothedDelta = delta * (1.f - Globals::aimbot_smoothness);
	Vector finalDelta = Globals::aimbot_smooth ? smoothedDelta : delta;

	// Subtract recoil from the aim adjustment (from the recoil)
	finalDelta.x -= recoilDelta.x;
	finalDelta.y -= recoilDelta.y;

	*currentAngles = *currentAngles + finalDelta;

	// calculate the REMAINING delta magnitude (how close we are to target) TODO CHANGE
	float deltaX = fabsf(delta.x);
	float deltaY = fabsf(delta.y);
	float totalDelta = sqrtf(deltaX * deltaX + deltaY * deltaY);

	auto currentTime = GetTickCount();
	bool isOnTarget = totalDelta <= Globals::aimbot_shoot_threshold;

	if (isOnTarget)
	{
		if (!isAiming)
		{
			isAiming = true;
			lastAimTime = currentTime;
		}

		auto timeOnTarget = currentTime - lastAimTime;
		auto timeSinceShot = currentTime - lastShootTime;

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
