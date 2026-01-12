#pragma once
#include "../../../sdk/entity/EntityManager.h"
#include "../../../sdk/utils/Utils.h"

class Aimbot
{
public:
	void run();

private:
	virtual void aimAtTarget(C_CSPlayerPawn* local, C_CSPlayerPawn* target);
	C_CSPlayerPawn* getBestTarget(C_CSPlayerPawn* local);
	BoneID findNearestBoneId(C_CSPlayerPawn* local, C_CSPlayerPawn* target);
	// instance variables
	static DWORD lastAimTime;
	static DWORD lastShootTime;
	static bool isAiming;

};
