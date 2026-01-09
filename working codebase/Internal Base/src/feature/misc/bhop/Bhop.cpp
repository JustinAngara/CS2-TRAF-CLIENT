#include <windows.h>
#include "../../../sdk/entity/EntityManager.h"
#include "../../../sdk/memory/PatternScan.h"
#include <iostream>
#include "Bhop.h"

namespace Bhop
{
	void Run()
	{
		auto& em = EntityManager::Get();
		C_CSPlayerPawn* local = em.GetLocalPawn();
		if (!local || !local->IsAlive())
			return;

		if (!(GetAsyncKeyState(VK_SPACE) & 0x8000))
			return;

		static bool wasOnGround = false;
		bool onGround = local->m_fFlags() & (1 << 0); 

		if (onGround && !wasOnGround)
		{
			INPUT ip = { 0 };
			ip.type = INPUT_KEYBOARD;
			ip.ki.wVk = 'K';
			SendInput(1, &ip, sizeof(INPUT));

			Sleep(10);

			ip.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &ip, sizeof(INPUT));
		}

		wasOnGround = onGround;
	}
}
