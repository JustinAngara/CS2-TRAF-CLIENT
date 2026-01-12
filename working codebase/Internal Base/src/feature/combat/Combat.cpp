#include "Combat.h"
#include "../combat/legitbot/LegitAimbot.h"
#include <iostream>
void Combat::Render()
{
	
	static LegitAimbot aimbot{}; // this will be initalized once, also has variants of rage and legit
	aimbot.run();
	
}
