#include "Recoil.h"
void Recoil::Run(C_CSPlayerPawn* local)
{
	if (!local) return;

	uintptr_t client = Memory::GetModuleBase("client.dll");
	if (!client) return;

	Vector* viewAngles = reinterpret_cast<Vector*>(client + Offsets::dwViewAngles);
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

	//////// Normalize
	if (viewAngles->x > 89.0f) viewAngles->x = 89.0f;
	if (viewAngles->x < -89.0f) viewAngles->x = -89.0f;

	while (viewAngles->y > 180.0f)
		viewAngles->y -= 360.0f;
	while (viewAngles->y < -180.0f)
		viewAngles->y += 360.0f;

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
