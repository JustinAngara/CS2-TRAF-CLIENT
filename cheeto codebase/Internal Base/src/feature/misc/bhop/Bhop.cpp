#include <windows.h>
#include "../../../sdk/entity/EntityManager.h"
#include "../../../sdk/utils/usermode.h"
#include "../../../sdk/utils/Globals.h"
#include "Bhop.h"

// TODO: Use the CUserCmd structures
// find correct implementation
namespace Bhop
{
	static bool wasOnGround = false;

	void Run(CUserCmd* cmd)
	{
		if (!Globals::bhop_enabled || !cmd)
			return;

		// Validate CUserCmd structure
		if (!cmd->csgoUserCmd.pBaseCmd || !cmd->csgoUserCmd.pBaseCmd->pInButtonState)
			return;

		auto& em = EntityManager::Get();
		C_CSPlayerPawn* local = em.GetLocalPawn();

		if (!local || !local->IsAlive())
		{
			wasOnGround = false;
			return;
		}

		bool keyPressed = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;
		bool onGround = local->IsOnGround();

		if (!keyPressed)
		{
			wasOnGround = false;
			return;
		}

		// CS2: Use the protobuf button state
		auto* buttonState = cmd->csgoUserCmd.pBaseCmd->pInButtonState;

		if (onGround && keyPressed)
		{
			// Set jump button
			buttonState->nValue |= IN_JUMP;
			wasOnGround = true;
		}
		else if (!onGround && wasOnGround)
		{
			// Remove jump button while in air
			buttonState->nValue &= ~IN_JUMP;
		}

		// Update ground state
		if (onGround)
			wasOnGround = true;
		else if (!keyPressed)
			wasOnGround = false;
	}
}
