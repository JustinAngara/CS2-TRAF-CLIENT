#include "AutoFire.h"
#include "../../../sdk/utils/Globals.h"
#include "../../../sdk/entity/EntityManager.h"
#include "../../../sdk/utils/Utils.h"
#include "../../../sdk/memory/PatternScan.h"
#include "../../../sdk/memory/Offsets.h"
#include "../../../feature/combat/Combat.h"
#include "../../../core/HackManager.h"

void AutoFire::Run()
{
	C_CSPlayerPawn* local = EntityManager::Get().GetLocalPawn();
	
	// new base case, if MB1 is held we shouldn't want to interact with any MB1/'L' clicks
	if (!Globals::autofire_enabled || !HackManager::g_client) return;
	if (!local || !local->IsAlive()) return;
	

	// auto fire will trigger if we can figure out if the FOV delta is 0 from the nearest enemy
	// TODO: I want to add a queue, simply shoot if valid on eahc tick by releasing a queue. self implement one
	//       I want a small memory footprint just figure out a way to fucking do a queue

	// check for nearest player and their bone
	C_CSPlayerPawn* target = Combat::Enemy::GetBestTarget(local);
	BoneID targetBone = Combat::Enemy::FindNearestBoneId(local, target, false); 
	if (!target || !target->IsAlive()) return;

	

	// see if delta matches to close to 0 (meaning my crosshair is on an enemy)
	Vector delta = Combat::Angle::GetDeltaAngle(local, target, targetBone);
	Utils::NormalizeAngles(delta);

	// shoot type shit
	if (delta.Length() < Globals::autofire_fov)
	{
		// now check if we want to do this
		Combat::Angle::LockAtTarget(local, target, targetBone);
		
		// fire but we want ti to maybe be recursive if we want to make it like good type shit
		Combat::Action::clickFire(); 

		// TODO:: ADD a log here of the player wanting to get hit
		//        track data if the queue invovles view angles, maybe try and figure out how we can implement a 
	}

}

