#include "HackManager.h"
#include "../../src/feature/visuals/Visuals.h"
#include "../feature/misc/Misc.h"
#include "../feature/combat/Combat.h"
#include "../feature/misc/bhop/Bhop.h"

void HackManager::Loop()
{
	const auto& entities = EntityManager::Get().GetEntities();
	for (const auto& ent : entities)
	{
		// directly mention esp from here and pass in render
	}


	Combat::Render();
	Visuals::Render();
	Misc::Render();

}



