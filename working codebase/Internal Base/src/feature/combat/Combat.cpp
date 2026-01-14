#include "Combat.h"
#include "../combat/legitbot/LegitAimbot.h"
#include "../combat/Attack/Aimbot.h"
#include "../combat/AutoFire/AutoFire.h"
#include "../../sdk/utils/Globals.h"
#include <iostream>
void Combat::Render()
{

	// classes/objects
	static Aimbot aimbot{}; // this will be initalized once, also has variants of rage and legit
	aimbot.run();

	// namespace functions
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
