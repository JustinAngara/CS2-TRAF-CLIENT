#include "Combat.h"
#include "../combat/legitbot/LegitAimbot.h"
#include "../combat/Attack/Aimbot.h"
#include "../combat/AutoFire/AutoFire.h"
#include <iostream>
void Combat::Render()
{

	// classes/objects
	static Aimbot aimbot{}; // this will be initalized once, also has variants of rage and legit
	aimbot.run();

	// namespace functions
	AutoFire::run();

}
