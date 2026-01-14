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
	void clickFire(FireInput input = FireInput::KeyboardL);
	void holdFire(FireInput input = FireInput::KeyboardL);
	void releaseFire(FireInput input = FireInput::KeyboardL);
	bool isMB1Held();

	// player stuff
	C_CSPlayerPawn* getBestTarget(C_CSPlayerPawn* local);
	BoneID findNearestBoneId(C_CSPlayerPawn* local, C_CSPlayerPawn* target, bool validBaim);

	// angle stuff
	Vector getDeltaAngle(C_CSPlayerPawn* local, C_CSPlayerPawn* target, uintptr_t client, BoneID targetBone);
	void   lockAtPoint(Vector* currentAngles, Vector localPos, Vector targetPos);
	
	// instance variables to handle gracefully
	static DWORD lastAimTime;
	static DWORD lastShootTime;
	static bool isAiming;
	


}
