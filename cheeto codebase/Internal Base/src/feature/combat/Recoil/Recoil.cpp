#include "Recoil.h"
#include "../../../sdk/utils/Utils.h"
#include "../../../core/HackManager.h"

void Recoil::Run(C_CSPlayerPawn* local)
{
	if (!local) return;

	
	if (!HackManager::g_client) return;

	Vector* viewAngles = reinterpret_cast<Vector*>(HackManager::g_client + Offsets::dwViewAngles);
	if (!viewAngles) return;

	static Vector oldPunch{};

	int shotsFired = local->m_iShotsFired();

	if (shotsFired < 1)
	{
		oldPunch = Vector(0, 0, 0);
		return;
	}

	Vector currentPunch = local->m_aimPunchAngle();
	Vector punchDelta	= (currentPunch * 2.0f) - (oldPunch * 2.0f);

	viewAngles->x -= punchDelta.x;
	viewAngles->y -= punchDelta.y;
	
	Utils::NormalizeAngles(*viewAngles);

	oldPunch = currentPunch;
}


void Recoil::VectorAngleRun(C_CSPlayerPawn* local, Vector& oldPunch, Vector& recoilData)
{
	int shotsFired = local->m_iShotsFired();
	if (shotsFired >= 1)
	{
		Vector currentPunch = local->m_aimPunchAngle();
		recoilData = (currentPunch * 2.0f) - (oldPunch * 2.0f);
		oldPunch = currentPunch;
	}
	else
	{
		oldPunch = Vector(0, 0, 0);
	}
}
