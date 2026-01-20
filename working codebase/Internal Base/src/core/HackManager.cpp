#include "HackManager.h"
#include "../../src/feature/visuals/Visuals.h"
#include "../feature/misc/Misc.h"
#include "../feature/combat/Combat.h"
#include "../feature/misc/bhop/Bhop.h"
#include "../feature/visuals/esp/Esp.h"

void HackManager::Loop()
{


	// iterator
	const auto& entities = EntityManager::Get().GetEntities();
	C_CSPlayerPawn* local = EntityManager::Get().GetLocalPawn();
	for (auto ent : entities)
	{
		// directly mention esp from here and pass in render
		ESP::renderEntity(ent, local);
	} // now that we have all state ready we can do some fuck ass shit

	// assume taht these are rendered from core.cpp (alreadyt is)


}

void HackManager::RunFeatures()
{
	Combat::Render();
	Visuals::Render();
	Misc::Render();

}


