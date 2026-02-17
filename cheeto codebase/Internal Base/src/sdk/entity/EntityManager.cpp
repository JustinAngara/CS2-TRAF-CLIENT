#include "EntityManager.h"
#include "../memory/PatternScan.h"
#include "../memory/Offsets.h"
#include <chrono>
#include <iostream>

EntityManager::EntityManager()
{
    uintptr_t client = Memory::GetModuleBase("client.dll");
    entityListAddress = client ? client + Offsets::dwEntityList : 0;

}

EntityManager& EntityManager::Get()
{
    static EntityManager instance;
    return instance;
}

void EntityManager::Update()
{
	uintptr_t client = Memory::GetModuleBase("client.dll");
	if (!client)
	{
		
		return;
	}

	uintptr_t listPtr = *reinterpret_cast<uintptr_t*>(client + Offsets::dwEntityList);


	C_CSPlayerPawn* currentLocalPawn = nullptr;

	uintptr_t localPawnAddr = *reinterpret_cast<uintptr_t*>(client + Offsets::dwLocalPlayerPawn);
	if (localPawnAddr)
	{
		currentLocalPawn = reinterpret_cast<C_CSPlayerPawn*>(localPawnAddr);
	}

	std::vector<Entity_t> temp;
	temp.reserve(64);

	for (int i = 1; i < 64; ++i)
	{
		uintptr_t listEntry = *reinterpret_cast<uintptr_t*>(
		listPtr + (8 * ((i & 0x7FFF) >> 9)) + 16);

		if (!listEntry) continue;

		uintptr_t controllerPtr = *reinterpret_cast<uintptr_t*>(
		listEntry + 112 * (i & 0x1FF));

		if (!controllerPtr) continue;

		auto	 controller = reinterpret_cast<C_CSPlayerController*>(controllerPtr);
		uint32_t pawnHandle = controller->m_hPlayerPawn();

		if (!pawnHandle || pawnHandle == static_cast<uint32_t>(-1)) continue;

		uintptr_t pawnListEntry = *reinterpret_cast<uintptr_t*>(
		listPtr + 8 * ((pawnHandle & 0x7FFF) >> 9) + 16);

		if (!pawnListEntry) continue;

		uintptr_t pawnPtr = *reinterpret_cast<uintptr_t*>(
		pawnListEntry + 112 * (pawnHandle & 0x1FF));

		if (!pawnPtr) continue;

		auto pawn = reinterpret_cast<C_CSPlayerPawn*>(pawnPtr);

		if (!pawn || !pawn->IsAlive()) continue;

		if (pawn == currentLocalPawn)
		{
			localPlayerIndex = i;
			continue;
		}

		Entity_t ent{};
		ent.controller = controller;
		ent.pawn	   = pawn;
		ent.index	   = i;
		ent.isEnemy	   = currentLocalPawn && pawn->m_iTeamNum() != currentLocalPawn->m_iTeamNum();

		temp.push_back(ent);
	}

	
	{
		std::unique_lock lock(mutex);
		entities.swap(temp);
		localPawn = currentLocalPawn;
	}
}

C_CSPlayerPawn* EntityManager::GetPawnFromHandle(uint32_t handle)
{
    if (!handle || !entityListAddress)
        return nullptr;

    uintptr_t listPtr = *reinterpret_cast<uintptr_t*>(entityListAddress);
    if (!listPtr)
        return nullptr;

    uintptr_t entry = *reinterpret_cast<uintptr_t*>(
        listPtr + 8 * ((handle & 0x7FFF) >> 9) + 16
        );
    if (!entry)
        return nullptr;

	uintptr_t pawnPtr = *reinterpret_cast<uintptr_t*>(
		entry + 112 * (handle & 0x1FF) // Changed from 112
	);
    if (!pawnPtr) return nullptr;

    return reinterpret_cast<C_CSPlayerPawn*>(pawnPtr);
}

C_CSPlayerPawn* EntityManager::GetLocalPawn()
{
    std::shared_lock lock(mutex);
    return localPawn;
}

const std::vector<Entity_t>& EntityManager::GetEntities()
{
    std::shared_lock lock(mutex);
    return entities;
}

C_CSPlayerController* EntityManager::GetLocalPlayer()
{
	std::shared_lock lock(mutex);

	for (const auto& entity : entities)
	{
		if (entity.pawn == localPawn && entity.controller)
		{
			return entity.controller;
		}
	}

	return nullptr;
}
