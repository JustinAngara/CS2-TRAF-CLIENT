#pragma once
#include "../../../sdk/entity/EntityManager.h"
#include "../../../sdk/utils/Utils.h"

class Aimbot
{
public:
	void run();

private:
	virtual void aimAtTarget(C_CSPlayerPawn* local, C_CSPlayerPawn* target);

};
