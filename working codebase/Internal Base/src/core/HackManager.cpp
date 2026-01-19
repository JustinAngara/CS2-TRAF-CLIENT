#include "HackManager.h"
#include "../../src/feature/visuals/Visuals.h"
#include "../feature/misc/Misc.h"
#include "../feature/combat/Combat.h"
#include "../feature/misc/bhop/Bhop.h"

void HackManager::Loop()
{
	
	Combat::Render();
	Visuals::Render();
	Misc::Render();

}



