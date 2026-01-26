#pragma once
#include "../../../sdk/entity/Classes.h"
namespace Recoil
{
	void Run(C_CSPlayerPawn* local);
	// change to delta soon?
	void VectorAngleRun(C_CSPlayerPawn* local, Vector& oldPunch, Vector& recoilData); // helper method for pre-determined vectors we want to adjust
	
}
