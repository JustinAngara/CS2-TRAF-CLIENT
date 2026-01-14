#pragma once

#include "../../sdk/entity/EntityManager.h"
#include <Windows.h>

namespace Combat{
	void Render();

	// helpers 
	C_CSPlayerPawn* getBestTarget(C_CSPlayerPawn* local);
	void clickFire();
	void holdFire();
	void releaseFire();
	bool isMB1Held();
	findNearestBoneId();

	// instance variables to handle gracefully
	static DWORD lastAimTime;
	static DWORD lastShootTime;
	static bool isAiming;
	


}
