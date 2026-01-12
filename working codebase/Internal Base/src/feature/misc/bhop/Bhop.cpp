#include <windows.h>
#include "../../../sdk/entity/EntityManager.h"
#include "../../../sdk/utils/usermode.h"
#include "../../../sdk/utils/Globals.h"
#include "Bhop.h"

namespace Bhop
{
	static bool wasOnGround = false;

	void Run(CUserCmd* cmd)
	{
		
		if (!Globals::bhop_enabled || !cmd)
			return;

		auto& em = EntityManager::Get();
		C_CSPlayerPawn* local = em.GetLocalPawn();

		if (!local || !local->IsAlive())
		{
			wasOnGround = false;
			return;
		}

		// Check if space key is pressed
		bool keyPressed = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;
		bool onGround = local->IsOnGround();

		if (!keyPressed)
		{
			wasOnGround = false;
			return;
		}

		// Get player flags to check ground state
		

		// Auto-bhop logic: only jump when on ground
		if (onGround && keyPressed)
		{
			// Set jump button in the command
			cmd->nButtons.nValue |= IN_JUMP;
			wasOnGround = true;
		}
		else if (!onGround && wasOnGround)
		{
			// Remove jump button while in air to prepare for next jump
			cmd->nButtons.nValue &= ~IN_JUMP;
		}

		// Update ground state
		if (onGround)
			wasOnGround = true;
		else if (!keyPressed)
			wasOnGround = false;
	}
}
