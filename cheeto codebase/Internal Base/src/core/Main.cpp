#include <Windows.h>
#include <thread>
#include <chrono>
#include "Hooks.h"
#include <iostream>


static HANDLE g_MainThread = nullptr;

DWORD WINAPI MainThread(LPVOID module)
{
#ifdef _DEBUG
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
	//std::cout << "[INFO] DLL injected, waiting for CS2 to initialize...\n";
#endif

	// Wait for game window to exist
	HWND gameWindow = nullptr;
	for (int i = 0; i < 30; i++) // Try for 30 seconds
	{
		gameWindow = FindWindowA("SDL_app", nullptr); // CS2 uses SDL
		if (gameWindow)
		{
			// std::cout << "[INFO] Found CS2 window\n";
			break;
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	if (!gameWindow)
	{
		//std::cout << "[ERROR] Could not find CS2 window!\n";
		std::this_thread::sleep_for(std::chrono::seconds(5));
		FreeLibraryAndExitThread(static_cast<HMODULE>(module), 0);
		return 0;
	}

	//std::cout << "[INFO] Setting up hooks...\n";
	Hooks::Setup();

	//std::cout << "[INFO] Hooks initialized. Press END to unload.\n";

	while (!(GetAsyncKeyState(VK_END) & 1))
		std::this_thread::sleep_for(std::chrono::milliseconds(150));

	Hooks::Destroy();

#ifdef _DEBUG
	if (f)
		fclose(f);
	FreeConsole();
#endif

	FreeLibraryAndExitThread(static_cast<HMODULE>(module), 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        g_MainThread = CreateThread(
            nullptr,
            0,
            MainThread,
            hModule,
            0,
            nullptr
        );
    }

    return TRUE;
}
