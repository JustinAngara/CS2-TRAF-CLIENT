#include "AutoFire.h"
#include <iostream>
#include "../../../sdk/utils/Globals.h"
#include "../../../sdk/entity/EntityManager.h"
#include "../../../sdk/utils/Utils.h"
#include "../../../sdk/memory/PatternScan.h"
#include "../../../sdk/memory/Offsets.h"
#include "../../../feature/combat/Combat.h"
#include "../../../core/HackManager.h"

void AutoFire::run()
{
	
	if (!Globals::autofire_enabled) return;

	// new base case, if MB1 is held we shouldn't want to interact with any MB1/'L' clicks

	
	if (!HackManager::g_client) return;


	// auto fire will trigger if we can figure out if the FOV delta is 0 from the nearest enemy
	C_CSPlayerPawn* local = EntityManager::Get().GetLocalPawn();
	if (!local || !local->IsAlive()) return;


	// check for nearest player
	C_CSPlayerPawn* target = Combat::GetBestTarget(local);
	BoneID targetBone = Combat::FindNearestBoneId(local, target, false);
	if (!target || !target->IsAlive()) return;

	

	// see if delta matches to close to 0 (meaning my crosshair is on an enemy)
	Vector delta = Combat::GetDeltaAngle(local, target, targetBone);
	Utils::NormalizeAngles(delta);

	// shoot type shit
	if (delta.Length() < Globals::autofire_fov)
	{
		// now check if we want to do this
		Combat::LockAtTarget(local, target, targetBone);
		
		// fire but we want ti to maybe be recursive if we want to make it like good type shit
		Combat::clickFire(); 
	}

}

