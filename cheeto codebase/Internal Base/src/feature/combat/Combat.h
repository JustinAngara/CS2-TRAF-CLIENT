#pragma once

#include "../../sdk/entity/EntityManager.h"
#include "../../sdk/entity/Classes.h"
#include <Windows.h>
#include "../../sdk/utils/Utils.h"

class Combat{
public:
	static void Render();

	// helpers 
	// mouse stuff
	enum class FireInput
	{
		KeyboardL,
		MouseLeft
	};
	static void clickFire(FireInput input   = FireInput::KeyboardL);
	static void holdFire(FireInput input    = FireInput::KeyboardL);
	static void releaseFire(FireInput input = FireInput::KeyboardL);
	static bool isMB1Held();

	// player stuff
	static C_CSPlayerPawn* GetBestTarget(C_CSPlayerPawn* local); // Turned into getter method, don't plan on removing
	static void DetermineBestPlayer(Entity_t& ent, int i, int size); // this will be latched onto original game entities hook
	static BoneID FindNearestBoneId(C_CSPlayerPawn* local, C_CSPlayerPawn* target, bool validBaim);
	static bool IsVisible(C_CSPlayerPawn* target, int localIndex = 1);
	static void DetermineClosest(C_CSPlayerPawn* local, C_CSPlayerPawn* enemy);

	// angle stuff
	static Vector GetDeltaAngle(C_CSPlayerPawn* local, C_CSPlayerPawn* target, BoneID targetBone);
	static void LockAtTarget(C_CSPlayerPawn* local, C_CSPlayerPawn* target, BoneID targetBone);



private:
	static C_CSPlayerPawn* g_bestTarget;
	static float		   m_bestDistance;
	// instance variables to handle gracefully we maybe dont need
	//static DWORD lastAimTime;
	//static DWORD lastShootTime;
	//static bool isAiming;

	


};
