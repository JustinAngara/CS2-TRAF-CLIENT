#pragma once
#include "../../../sdk/entity/EntityManager.h"
#include "../../../sdk/utils/Utils.h"

class Aimbot
{
public:
	virtual void run();

private:
	// best case scenario we will make more helpers to make final
	virtual void aimAtTarget(C_CSPlayerPawn* local, C_CSPlayerPawn* target);
	virtual C_CSPlayerPawn* getBestTarget(C_CSPlayerPawn* local);
	virtual BoneID findNearestBoneId(C_CSPlayerPawn* local, C_CSPlayerPawn* target); 
};
