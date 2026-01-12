#include "Combat.h"
#include "../combat/legitbot/LegitAimbot.h"
#include "../combat/Attack/Aimbot.h"
#include <iostream>
void Combat::Render()
{
	
	static Aimbot aimbot{}; // this will be initalized once, also has variants of rage and legit
	static LegitAimbot aimbotLegit{}; // this will be initalized once, also has variants of rage and legit
	aimbot.run();
	
}
