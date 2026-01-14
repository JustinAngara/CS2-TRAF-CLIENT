#include "Combat.h"
#include "../combat/legitbot/LegitAimbot.h"
#include "../combat/Attack/Aimbot.h"
#include "../combat/AutoFire/AutoFire.h"
#include "../../sdk/utils/Globals.h"
#include <iostream>
void Combat::Render()
{

	// classes/objects calls
	static Aimbot aimbot{}; // this will be initalized once, also has variants of rage and legit
	aimbot.run();

	// namespace functions calls
	AutoFire::run();

}

////////////// helpers
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
		}
	}

	return bestTarget;
	
}




// this ist stil targeting
// proposal is to create a start and end
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

Vector Combat::getDeltaAngle(C_CSPlayerPawn* local, C_CSPlayerPawn* target, uintptr_t client, BoneID targetBone)
{
	Vector targetPos = Utils::GetBonePos(target, targetBone);
	Vector* currentAngles = reinterpret_cast<Vector*>(client + Offsets::dwViewAngles);
	Vector localPos = local->m_vOldOrigin() + local->m_vecViewOffset();
	Vector aimAngles = Utils::CalcAngle(localPos, targetPos);
	Vector delta = aimAngles - *currentAngles;

	return delta;
}

////////////////////////////// MOUSE STUFF 
void Combat::clickFire()
{
	holdFire();
	releaseFire();
}
void Combat::holdFire()
{
	if (!(GetAsyncKeyState('L') & 0x8000)) keybd_event('L', 0, 0, 0);
}

void Combat::releaseFire()
{
	if (GetAsyncKeyState('L') & 0x8000) keybd_event('L', 0, KEYEVENTF_KEYUP, 0);
}

bool Combat::isMB1Held()
{
	return (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
}
