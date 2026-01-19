#include "HackManager.h"
#include "../../src/feature/visuals/Visuals.h"
#include "../feature/misc/Misc.h"
#include "../feature/combat/Combat.h"
#include "../feature/misc/bhop/Bhop.h"
#include "../feature/visuals/esp/Esp.h"

void HackManager::Loop()
{



	// become setup
	Combat::Render();
	Visuals::Render();
	Misc::Render();


	// iterator
	const auto& entities = EntityManager::Get().GetEntities();
	C_CSPlayerPawn* local = EntityManager::Get().GetLocalPawn();
	for (auto ent : entities)
	{
		// directly mention esp from here and pass in render
		ESP::renderEntity(ent, local);
	}

}



