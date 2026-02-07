#include "Aimbot.h"
#include "../../../sdk/entity/EntityManager.h"
#include "../../../sdk/utils/Utils.h"
#include "../../../sdk/memory/PatternScan.h"
#include "../../../sdk/memory/Offsets.h"
#include "../../../sdk/utils/Globals.h"
#include "../../../feature/combat/Combat.h"
#include "../../../feature/combat/Recoil/Recoil.h"
#include <Windows.h>
#include <iostream>
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

	HackManager::g_client = Memory::GetModuleBase("client.dll");
	if (!HackManager::g_client)
	{
		std::cout << "shit gClient is wrong\n";
		std::cout << HackManager::g_client << ", please work\n";
		return;
	}

	Vector* currentAngles = reinterpret_cast<Vector*>(HackManager::g_client + Offsets::dwViewAngles);
	if (!currentAngles) 
	{
		std::cout << "shit current angles is wrong\n";
		return;
	}

	bool validBaim = Globals::aimbot_force_baim && target->m_iHealth() <= Globals::aimbot_baim_min;
	BoneID targetBone = Combat::FindNearestBoneId(local, target, validBaim);
	Vector targetPos = Utils::GetBonePos(target, targetBone);

	Vector delta = Combat::GetDeltaAngle(local, target, targetBone);


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

	// apply recoil from here
	finalDelta.x -= recoilDelta.x;
	finalDelta.y -= recoilDelta.y;

	*currentAngles = *currentAngles + finalDelta;

	float deltaX = fabsf(delta.x);
	float deltaY = fabsf(delta.y);
	float totalDelta = sqrtf(deltaX * deltaX + deltaY * deltaY);

	auto currentTime = GetTickCount();
	bool isOnTarget = totalDelta <= Globals::aimbot_shoot_threshold;

	// check between if it is between a first bullet fired vs continueing the spray pattern
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
