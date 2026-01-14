#include "AutoFire.h"
#include <iostream>
#include "../../../sdk/utils/Globals.h"
#include "../../../sdk/entity/EntityManager.h"
#include "../../../sdk/utils/Utils.h"
#include "../../../sdk/memory/PatternScan.h"
#include "../../../sdk/memory/Offsets.h"
#include "../../../feature/combat/Combat.h"


void AutoFire::run()
{
	
	if (!Globals::autofire_enabled) return;

	// new base case, if MB1 is held we shouldn't want to interact with any MB1/'L' clicks

	uintptr_t client = Memory::GetModuleBase("client.dll");
	if (!client) return;

	// auto fire will trigger if we can figure out if the FOV delta is 0 from the nearest enemy
	C_CSPlayerPawn* local = EntityManager::Get().GetLocalPawn();
	if (!local || !local->IsAlive()) return;

	// check for nearest player
	C_CSPlayerPawn* target = Combat::getBestTarget(local);
	BoneID targetBone = Combat::findNearestBoneId(local, target, false);
	

	if (!target || !target->IsAlive()) return;

	// see if delta matches to close to 0 (meaning my crosshair is on an enemy)
	Vector delta = Combat::getDeltaAngle(local, target, client, targetBone);
	Utils::NormalizeAngles(delta);

	// shoot type shit
	if (delta.Length() < 0.3)
	{
		std::cout << "i am in fire\n";
		Combat::clickFire();
	}

}

