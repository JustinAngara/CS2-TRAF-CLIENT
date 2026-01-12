#include "Misc.h"
#include "bhop/Bhop.h"
#include "../../sdk/utils/usermode.h"

void Misc::Render()
{
	Misc::Run();
}

void Misc::Run() {
	CUserCmd* pCmd{};
	Bhop::Run(pCmd); // this is where bhop is
}
