#include "AutoFire.h"
#include "../../../sdk/utils/Globals.h"
#include "../../../sdk/entity/EntityManager.h"
#include "../../../sdk/utils/Utils.h"
#include "../../../sdk/memory/PatternScan.h"
#include "../../../sdk/memory/Offsets.h"
#include "../../../feature/combat/Combat.h"
void AutoFire::run()
{
	if (!Globals::autofire_enabled) return;

	uintptr_t client = Memory::GetModuleBase("client.dll");
	if (!client) return;

	// auto fire will trigger if we can figure out if the FOV delta is 0 from the nearest enemy
	C_CSPlayerPawn* local = EntityManager::Get().GetLocalPawn();
	if (!local || !local->IsAlive()) return;

	// check for nearest player
	C_CSPlayerPawn* target = Combat::getBestTarget(local);
	if (!target || !target->IsAlive()) return;

	// see if delta matches to close to 0 (meaning my crosshair is on an enemy)
	Vector* currentAngles = reinterpret_cast<Vector*>(client + Offsets::dwViewAngles);


	// shoot type shit

}


C_CSPlayerPawn* getBestTarget(C_CSPlayerPawn* local)
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
