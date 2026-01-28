#include "HackManager.h"
#include "../../src/feature/visuals/Visuals.h"
#include "../feature/misc/Misc.h"
#include "../feature/combat/Combat.h"
#include "../feature/misc/bhop/Bhop.h"
#include "../feature/visuals/esp/Esp.h"
#include <cstdint>
#include <iostream>

uintptr_t HackManager::g_client{  };

void HackManager::Loop()
{

	// iterator
	const auto& entities = EntityManager::Get().GetEntities();
	C_CSPlayerPawn* local = EntityManager::Get().GetLocalPawn();
	g_client = Memory::GetModuleBase("client.dll");

	std::cout << "this is g client 0x" << g_client << "\n";

	ESP::setup();

	int iterator = 0, size = entities.size();
	// entity list is not getting hit
	std::cout << "this is entity list size: " << size << "\n";

	for (auto ent : entities)
	{
		// directly mention esp from here and pass in render
		ESP::renderEntity(ent, local);
		
		iterator++;
	} // now that we have all state ready we can do some fuck ass shit

	// assume taht these are rendered from core.cpp (alreadyt is)

	HackManager::RunFeatures();
}

void HackManager::RunFeatures()
{
	Combat::Render();
	Visuals::Render();
	//Misc::Render();


}


