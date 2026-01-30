#include "AutoStop.h"
#include "../../../sdk/utils/Globals.h"
namespace
{
	void ReleaseKey(int vKey)
	{
		INPUT input		 = { 0 };
		input.type		 = INPUT_KEYBOARD;
		input.ki.wVk	 = vKey;
		input.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &input, sizeof(INPUT));
	}

	void StopMovement()
	{
		ReleaseKey('W');
		ReleaseKey('A');
		ReleaseKey('S');
		ReleaseKey('D');
	}
}

void AutoStop::Run()
{
	if (Globals::autostop_enabled)
	{
		StopMovement();
	}
};
