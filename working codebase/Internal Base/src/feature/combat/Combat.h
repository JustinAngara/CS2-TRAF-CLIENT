#pragma once

#include "../../sdk/entity/EntityManager.h"
#include <Windows.h>

namespace Combat{
	void Render();

	// helpers 
	C_CSPlayerPawn* getBestTarget(C_CSPlayerPawn* local);

	// instance variables
	static DWORD lastAimTime;
	static DWORD lastShootTime;
	static bool isAiming;

	void clickFire();
	void holdFire();
	void releaseFire();

}
