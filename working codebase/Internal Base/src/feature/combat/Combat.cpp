#include "Combat.h"
#include "Attack/Aimbot.h"

#include <iostream>
void Combat::Render() {
	
	static Aimbot aimbot{}; // this will be initalized once, also has variants of rage and legit
	aimbot.run();
	


	//std::cout << "now in combat;\n";
}
