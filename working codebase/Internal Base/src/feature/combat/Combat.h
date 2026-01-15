#pragma once

#include "../../sdk/entity/EntityManager.h"
#include <Windows.h>
#include "../../sdk/utils/Utils.h"

namespace Combat{
	void Render();

	// helpers 
	// mouse stuff
	enum class FireInput
	{
		KeyboardL,
		MouseLeft
	};
	void clickFire(FireInput input   = FireInput::KeyboardL);
	void holdFire(FireInput input    = FireInput::KeyboardL);
	void releaseFire(FireInput input = FireInput::KeyboardL);
	bool isMB1Held();

	// player stuff
	C_CSPlayerPawn* getBestTarget(C_CSPlayerPawn* local);
	BoneID findNearestBoneId(C_CSPlayerPawn* local, C_CSPlayerPawn* target, bool validBaim);
	bool IsVisible(C_CSPlayerPawn* target, int localIndex = 1);
	// angle stuff
	Vector getDeltaAngle(C_CSPlayerPawn* local, C_CSPlayerPawn* target, uintptr_t client, BoneID targetBone);
	void   lockAtTarget(uintptr_t client, C_CSPlayerPawn* local, C_CSPlayerPawn* target, BoneID targetBone);
	
	// instance variables to handle gracefully we maybe dont need
	//static DWORD lastAimTime;
	//static DWORD lastShootTime;
	//static bool isAiming;
	


}
