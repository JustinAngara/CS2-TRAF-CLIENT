#include <windows.h>
#include "../../../sdk/entity/EntityManager.h"
#include "../../../sdk/utils/Globals.h"
#include "Bhop.h"

// TODO: Use the CUserCmd structures
// find correct implementation
// CUserCmd is going to a pain to figure out someone please help me
namespace Bhop
{
	static bool wasOnGround = false;
	static bool isOnGround  = false;

	void Run()
	{
		if (!Globals::bhop_enabled) return;

		if (!wasOnGround && isOnGround) return; // this should do the method to jump
	}
	
}
