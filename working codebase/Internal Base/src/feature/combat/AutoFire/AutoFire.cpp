#include "AutoFire.h"
#include "../../../sdk/utils/Globals.h"
#include "../../../sdk/entity/EntityManager.h"
#include "../../../sdk/utils/Utils.h"
#include "../../../sdk/memory/PatternScan.h"
#include "../../../sdk/memory/Offsets.h"
void AutoFire::run()
{
	C_CSPlayerPawn* local = EntityManager::Get().GetLocalPawn();
	if (!Globals::autofire_enabled) return;

	// auto fire will trigger if we can figure out if the FOV delta is 0 from the nearest enemy
	if (!local || !local->IsAlive()) return;

}
