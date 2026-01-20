#include "Combat.h"
#include "../combat/legitbot/LegitAimbot.h"
#include "../combat/Recoil/Recoil.h"
#include "../combat/Attack/Aimbot.h"
#include "../combat/AutoFire/AutoFire.h"
#include "../../sdk/utils/Globals.h"
#include "../../sdk/utils/Utils.h"
#include "../../sdk/entity/EntityManager.h"
#include "../../sdk/entity/Classes.h"


//annoying static member variables
C_CSPlayerPawn* Combat::g_bestTarget = nullptr;


// this will be hatched to game tick
void Combat::Render()
{
	static Aimbot aimbot{}; // this will be initalized once, also has variants of rage and legit

	auto* localPlayer = EntityManager::Get().GetLocalPawn();
	Recoil::Run(localPlayer);

	// namespace functions calls
	AutoFire::run();

	// classes/objects calls
	aimbot.run();

}


////////////////////////////// HELPERS /////////////////////////////////
////////////////////////////// TARGET PLAYER STUFF

// potential fix
// to utilize get best target optimized
// pass in size of the entities array and the index it is on,
// double check if the entity is the best target (call setup)
// and then update best target as entity, but utilize the HackManager.cpp iterator
C_CSPlayerPawn* Combat::getBestTarget(C_CSPlayerPawn* local)
{
	// grab all the entities
	const auto& entities = EntityManager::Get().GetEntities();
	C_CSPlayerPawn* bestTarget = nullptr;
	float bestDistance = Globals::aimbot_fov;

	uintptr_t client = Memory::GetModuleBase("client.dll");
	if (!client)
		return nullptr;

	// get current angles
	Vector* currentAngles = reinterpret_cast<Vector*>(client + Offsets::dwViewAngles);
	if (!currentAngles)
		return nullptr;

	for (const auto& ent : entities)
	{
		// base case
		bool isTeammate = !ent.isEnemy;
		if (isTeammate && !Globals::aimbot_friendly_fire)
			continue;

		// find the pos
		Vector headPos = Utils::GetBonePos(ent.pawn, BoneID::Head);
		if (headPos.IsZero())
			continue;

		Vector localPos = local->m_vOldOrigin() + local->m_vecViewOffset();

		Vector aimAngles = Utils::CalcAngle(localPos, headPos);
		float fov = Utils::GetFoV(*currentAngles, aimAngles);

		if (fov < bestDistance)
		{
			bestDistance = fov;
			bestTarget = ent.pawn;
			// g_bestTarget = bestTarget;
			//return bestTarget; internally this is the exact same so why not do this?
			// we need a better algorithm that will accept this and prevent another annoying edge case
			// this will need to pick the difference between the enemies and figure out a answer quick
		}
	}

	return bestTarget;
}

void Combat::DetermineBestPlayer(Entity_t& ent, int i, int size)
{
	if (!g_bestTarget) g_bestTarget = ent.pawn; // basically we dont want a nullptr if possible

	//// we reached the end of the iterator
	//if (i >= size - 1) return;

	//uintptr_t client = Memory::GetModuleBase("client.dll");
	//if (!client) return;

	//return; // arbritrary 
}



BoneID Combat::findNearestBoneId(C_CSPlayerPawn* local, C_CSPlayerPawn* target, bool validBaim = false)
{
	if (!local || !target) return BoneID::Head;

	constexpr BoneID iterateBones[] = {
		BoneID::Head,
		BoneID::Neck,
		BoneID::Spine,
		BoneID::Stomach,
		BoneID::LeftShoulder,
		BoneID::RightShoulder,
	};

	int start = validBaim ? 2 : 0; // 0 is head, 1 is neck, 2 is spine ...

	uintptr_t client = Memory::GetModuleBase("client.dll");
	if (!client) return iterateBones[start];

	Vector* currentAngles = reinterpret_cast<Vector*>(client + Offsets::dwViewAngles);
	if (!currentAngles) return iterateBones[start];

	Vector localPos = local->m_vOldOrigin() + local->m_vecViewOffset();

	BoneID bestBone = iterateBones[start];
	float bestFov = FLT_MAX;

	for (int i = start; i < sizeof(iterateBones) / sizeof(BoneID); i++)
	{
		Vector bonePos = Utils::GetBonePos(target, iterateBones[i]);
		if (bonePos.IsZero()) continue;

		Vector aimAngles = Utils::CalcAngle(localPos, bonePos);
		float fov = Utils::GetFoV(*currentAngles, aimAngles);

		// penalize lower bone structs
		float penalty = i * 0.15f;
		float adjustedFov = fov + penalty;

		if (adjustedFov < bestFov)
		{
			bestFov = adjustedFov;
			bestBone = iterateBones[i];
		}
	}

	return bestBone;
}

bool Combat::isVisible(C_CSPlayerPawn* target, int localIndex)
{

	if (!target || localIndex <= 0) return false;

	uintptr_t spottedStatePtr = *(uintptr_t*)((uintptr_t)target + Offsets::m_entitySpottedState);
	if (!spottedStatePtr) return false;

	uint32_t mask = *(uint32_t*)(spottedStatePtr + 0xC); // m_bSpottedByMask at +0xC

	return mask & (1 << (localIndex - 1));
	
}

////////////////////////////// ANGLE STUFF
Vector Combat::getDeltaAngle(C_CSPlayerPawn* local, C_CSPlayerPawn* target, uintptr_t client, BoneID targetBone)
{
	Vector targetPos = Utils::GetBonePos(target, targetBone);
	Vector* currentAngles = reinterpret_cast<Vector*>(client + Offsets::dwViewAngles);
	Vector localPos = local->m_vOldOrigin() + local->m_vecViewOffset();
	Vector aimAngles = Utils::CalcAngle(localPos, targetPos);
	Vector delta = aimAngles - *currentAngles;

	return delta;
}


void Combat::lockAtTarget(uintptr_t client, C_CSPlayerPawn* local, C_CSPlayerPawn* target, BoneID targetBone)
{

	Vector* currentAngles = reinterpret_cast<Vector*>(client + Offsets::dwViewAngles);
	Vector targetPos = Utils::GetBonePos(target, targetBone);
	if (targetPos.IsZero()) return;
	Vector localPos	 = local->m_vOldOrigin() + local->m_vecViewOffset();
	Vector aimAngles = Utils::CalcAngle(localPos, targetPos);
	

	*currentAngles = aimAngles;
}



////////////////////////////// MOUSE TO FIRE STUFF 
void Combat::clickFire(FireInput input)
{
	holdFire(input);
	releaseFire(input);
}

void Combat::holdFire(FireInput input)
{
	if (input == FireInput::KeyboardL)
	{
		if (!(GetAsyncKeyState('L') & 0x8000))
			keybd_event('L', 0, 0, 0);
	}
	else
	{
		if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	}
}

void Combat::releaseFire(FireInput input)
{
	if (input == FireInput::KeyboardL)
	{
		if (GetAsyncKeyState('L') & 0x8000)
			keybd_event('L', 0, KEYEVENTF_KEYUP, 0);
	}
	else
	{
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	}
}


bool Combat::isMB1Held()
{
	return (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
}
