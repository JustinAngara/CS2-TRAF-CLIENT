#pragma once

#include "../../sdk/entity/EntityManager.h"
#include "../../sdk/entity/Classes.h"
#include <Windows.h>
#include "../../sdk/utils/Utils.h"

// todo: decide what is actually needs to be static, though its fine as is rn
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
	static C_CSPlayerPawn* getBestTarget(C_CSPlayerPawn* local);
	static void DetermineBestPlayer(Entity_t& ent, int i, int size); // we want local
	static BoneID findNearestBoneId(C_CSPlayerPawn* local, C_CSPlayerPawn* target, bool validBaim);
	static bool isVisible(C_CSPlayerPawn* target, int localIndex = 1);
	// angle stuff
	static Vector getDeltaAngle(C_CSPlayerPawn* local, C_CSPlayerPawn* target, uintptr_t client, BoneID targetBone);
	static void lockAtTarget(uintptr_t client, C_CSPlayerPawn* local, C_CSPlayerPawn* target, BoneID targetBone);

	// instance variables to handle gracefully we maybe dont need
	//static DWORD lastAimTime;
	//static DWORD lastShootTime;
	//static bool isAiming;

private:
	static C_CSPlayerPawn* g_bestTarget;
	


};
