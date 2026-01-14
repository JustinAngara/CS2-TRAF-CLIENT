#pragma once

#include "../../sdk/entity/EntityManager.h"
#include <Windows.h>
#include "../../sdk/utils/Utils.h"

namespace Combat{
	void Render();

	// helpers 
	C_CSPlayerPawn* getBestTarget(C_CSPlayerPawn* local);
	void clickFire();
	void holdFire();
	void releaseFire();
	bool isMB1Held();
	BoneID findNearestBoneId(C_CSPlayerPawn* local, C_CSPlayerPawn* target, bool validBaim);
	Vector getDeltaAngle(C_CSPlayerPawn* local, C_CSPlayerPawn* target, uintptr_t client, BoneID targetBone);
	
	// instance variables to handle gracefully
	static DWORD lastAimTime;
	static DWORD lastShootTime;
	static bool isAiming;
	


}
