#include "Hooks.h"

#include <iostream>
#include "../../ext/imgui/imgui.h"
#include "../../ext/imgui/imgui_impl_win32.h"
#include "../../ext/imgui/imgui_impl_dx11.h"
#include "../../ext/minhook/MinHook.h"
#include "../../src/menu/Menu.h"
#include "../../src/sdk/entity/EntityManager.h"
#include "../../src/sdk/utils/Globals.h"
#include "../../src/sdk/memory/Offsets.h"
#include "../../src/sdk/memory/PatternScan.h"
#include "HackManager.h"
#include "../../src/sdk/utils/usermode.h"
#include "../../src/feature/visuals/Visuals.h"
#include "../feature/misc/Misc.h"
#include "../feature/combat/Combat.h"
#include "../feature/misc/bhop/Bhop.h"
#include "../feature/visuals/esp/Esp.h"

#pragma comment(lib, "d3d11.lib")

// global stuff
static ID3D11Device* g_Device = nullptr;
static ID3D11DeviceContext* g_Context = nullptr;
static ID3D11RenderTargetView* g_RTV = nullptr;
static HWND g_Window = nullptr;
static bool g_Init = false;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);


// present
LRESULT __stdcall Hooks::hkWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (Menu::IsOpen)
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
		return true;
	}

	return CallWindowProc(oWndProc, hWnd, msg, wParam, lParam);
}

HRESULT __stdcall Hooks::hkPresent(IDXGISwapChain* swapChain, UINT sync, UINT flags)
{
	if (!g_Init)
	{
		if (FAILED(swapChain->GetDevice(__uuidof(ID3D11Device), (void**)&g_Device)))
			return oPresent(swapChain, sync, flags);

		g_Device->GetImmediateContext(&g_Context);

		DXGI_SWAP_CHAIN_DESC sd{};
		swapChain->GetDesc(&sd);
		g_Window = sd.OutputWindow;

		ID3D11Texture2D* backBuffer = nullptr;
		swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
		g_Device->CreateRenderTargetView(backBuffer, nullptr, &g_RTV);
		backBuffer->Release();

		oWndProc = (WNDPROC)SetWindowLongPtr(
		g_Window, GWLP_WNDPROC, (LONG_PTR)hkWndProc);

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = nullptr;

		ImGui_ImplWin32_Init(g_Window);
		ImGui_ImplDX11_Init(g_Device, g_Context);

		g_Init = true;
	}

	EntityManager::Get().Update();

	uintptr_t client = Memory::GetModuleBase("client.dll");
	memcpy(
		&Globals::ViewMatrix,
		(void*)(client + Offsets::dwViewMatrix),
		sizeof(Globals::ViewMatrix)
	);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (GetAsyncKeyState(VK_INSERT) & 1) Menu::IsOpen = !Menu::IsOpen;

	if (Menu::IsOpen) Menu::Render();

	///////////////// THIS IS WHERE GAME TICK IS LATCHED ONTO ADD STUFF HERE
	// from here i want to create a entity hook that iterates through every entity and performs updates to features
	// encapsulate a general hack manager to pass in vars/update state

	// setup area
	{
		HackManager::RunFeatures(); // this will fill in any static variables needed
	}

	HackManager::Loop();
	ImGui::Render();



	g_Context->OMSetRenderTargets(1, &g_RTV, nullptr);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return oPresent(swapChain, sync, flags);
}


void Hooks::Setup()
{
	static bool initialized = false;
	if (initialized) return;
	initialized = true;

	std::cout << "[INFO] Initializing hooks...\n";

	if (MH_Initialize() != MH_OK)
	{
		std::cout << "[ERROR] MH_Initialize failed\n";
		return;
	}

	WNDCLASSEXW wc{};
	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = DefWindowProcW;
	wc.hInstance = GetModuleHandleW(nullptr);
	wc.lpszClassName = L"DummyDX";

	RegisterClassExW(&wc);
	HWND hwnd = CreateWindowW(wc.lpszClassName, L"", WS_OVERLAPPEDWINDOW,
	0, 0, 100, 100, nullptr, nullptr, wc.hInstance, nullptr);

	DXGI_SWAP_CHAIN_DESC sd{};
	sd.BufferCount = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	IDXGISwapChain* sc = nullptr;
	ID3D11Device* dev = nullptr;
	ID3D11DeviceContext* ctx = nullptr;
	D3D_FEATURE_LEVEL fl;

	if (SUCCEEDED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
		nullptr, 0, D3D11_SDK_VERSION, &sd, &sc, &dev, &fl, &ctx)))
	{
		void** vtable = *reinterpret_cast<void***>(sc);
		void* present = vtable[8];
		MH_CreateHook(present, &hkPresent, reinterpret_cast<void**>(&oPresent));

		// Hook CreateMove via vtable
		uintptr_t inputSystemAddr = Memory::PatternScan("client.dll", "48 8B 0D ?? ?? 48 8B 01 FF 90 ?? ?? 84");
		if (inputSystemAddr)
		{
			std::cout << "[INFO] Found input system at: 0x" << std::hex << inputSystemAddr << std::dec << "\n";

			int32_t offset = *reinterpret_cast<int32_t*>(inputSystemAddr + 3);
			uintptr_t inputPtr = inputSystemAddr + 7 + offset;
			void** input = *reinterpret_cast<void***>(inputPtr);
			// in Setup(), replace the vtable hooking loop with:
			/*
			if (input && *input)
			{
				void** inputVtable = *reinterpret_cast<void***>(input);

				// Hook ONLY index 5 (the actual CreateMove)
				if (MH_CreateHook(inputVtable[5], &hkCreateMove, reinterpret_cast<void**>(&oCreateMove)) == MH_OK)
				{
					std::cout << "[SUCCESS] Hooked CreateMove at vtable[5]\n";
				}
			}
			*/
		}

		MH_EnableHook(MH_ALL_HOOKS);
		sc->Release();
		dev->Release();
		ctx->Release();
	}

	DestroyWindow(hwnd);
	UnregisterClassW(wc.lpszClassName, wc.hInstance);
	std::cout << "[INFO] Hook setup complete\n";
}


void __fastcall Hooks::hkCreateMove(void* thisptr, int slot, bool active)
{
	oCreateMove(thisptr, slot, active);
	/*
	CCSGOInput* input = (CCSGOInput*)thisptr;

	// Access buttons directly from CCSGOInput structure
	std::cout << "[DEBUG] Buttons before: " << input->m_buttons << "\n";

	auto& em = EntityManager::Get();
	C_CSPlayerPawn* local = em.GetLocalPawn();

	if (!local || !local->IsAlive())
		return;

	bool keyPressed = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;
	bool onGround = local->IsOnGround();

	if (keyPressed && onGround && Globals::bhop_enabled)
	{
		input->m_buttons |= IN_JUMP;
		std::cout << "[DEBUG] Added jump! Buttons: " << input->m_buttons << "\n";
	}
	else if (!onGround)
	{
		input->m_buttons &= ~IN_JUMP;
	}
	*/
}

//DEBUG  START ------------------
/*
void* __fastcall Hooks::hkCreateMove(void* thisptr, void* edx, int slot, float flInputSampleTime, bool bActive)
{
	std::cout << "[DEBUG] hkCreateMove called! thisptr=" << thisptr << "\n";

	// Call original first
	void* result = oCreateMove(thisptr, edx, slot, flInputSampleTime, bActive);

	// Try multiple possible offsets for CUserCmd
	CUserCmd* pCmd = nullptr;

	// Common offsets to try (these vary by CS2 version)
	uintptr_t offsets[] = { 0x5540, 0x5548, 0x5550, 0x5538, 0x5530 };

	for (auto offset : offsets)
	{
		pCmd = *reinterpret_cast<CUserCmd**>((uintptr_t)thisptr + offset);
		if (pCmd && pCmd->csgoUserCmd.pBaseCmd)
		{
			std::cout << "[DEBUG] Found valid CUserCmd at offset: 0x" << std::hex << offset << std::dec << "\n";
			break;
		}
	}

	if (!pCmd)
	{
		static bool once = false;
		if (!once)
		{
			std::cout << "[DEBUG] pCmd is NULL - all offsets failed\n";
			once = true;
		}
		return result;
	}

	if (!pCmd->csgoUserCmd.pBaseCmd)
	{
		static bool once2 = false;
		if (!once2)
		{
			std::cout << "[DEBUG] pBaseCmd is NULL\n";
			once2 = true;
		}
		return result;
	}

	Bhop::Run(pCmd);

	return result;
}
*/

/*
*
* THIS CONTAINS THE VTABLE DON'T DELETE 
void Hooks::Setup()
{
	static bool initialized = false;
	if (initialized)
		return;
	initialized = true;

	std::cout << "[INFO] Initializing hooks...\n";

	if (MH_Initialize() != MH_OK)
	{
		std::cout << "[ERROR] MH_Initialize failed\n";
		return;
	}

	WNDCLASSEXW wc{};
	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = DefWindowProcW;
	wc.hInstance = GetModuleHandleW(nullptr);
	wc.lpszClassName = L"DummyDX";

	RegisterClassExW(&wc);
	HWND hwnd = CreateWindowW(wc.lpszClassName, L"", WS_OVERLAPPEDWINDOW,
	0, 0, 100, 100, nullptr, nullptr, wc.hInstance, nullptr);

	DXGI_SWAP_CHAIN_DESC sd{};
	sd.BufferCount = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	IDXGISwapChain* sc = nullptr;
	ID3D11Device* dev = nullptr;
	ID3D11DeviceContext* ctx = nullptr;
	D3D_FEATURE_LEVEL fl;

	if (SUCCEEDED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
		nullptr, 0, D3D11_SDK_VERSION, &sd, &sc, &dev, &fl, &ctx)))
	{
		void** vtable = *reinterpret_cast<void***>(sc);
		void* present = vtable[8];
		// MH_CreateHook(present, &hkPresent, reinterpret_cast<void**>(&oPresent));

		// Hook CreateMove via vtable
		uintptr_t inputSystemAddr = Memory::PatternScan("client.dll", "48 8B 0D ? ? ? ? 48 8B 01 FF 90 ? ? ? ? 84");
		if (inputSystemAddr)
		{
			////////// TODO DEBUG HERE ////////////////////
			/*
			std::cout << "[INFO] Found input system at: 0x" << std::hex << inputSystemAddr << std::dec << "\n";

			int32_t offset = *reinterpret_cast<int32_t*>(inputSystemAddr + 3);
			uintptr_t inputPtr = inputSystemAddr + 7 + offset;
			void** input = *reinterpret_cast<void***>(inputPtr);

			// In Setup(), replace the vtable hooking loop with:
			if (input && *input)
			{
				void** inputVtable = *reinterpret_cast<void***>(input);

				// Hook ONLY index 5 (the actual CreateMove)
				if (MH_CreateHook(inputVtable[5], &hkCreateMove, reinterpret_cast<void**>(&oCreateMove)) == MH_OK)
				{
					std::cout << "[SUCCESS] Hooked CreateMove at vtable[5]\n";
				}
			}
			
		}

		MH_EnableHook(MH_ALL_HOOKS);
		sc->Release();
		dev->Release();
		ctx->Release();
	}

	DestroyWindow(hwnd);
	UnregisterClassW(wc.lpszClassName, wc.hInstance);
	std::cout << "[INFO] Hook setup complete\n";
}
*/


/*
*
* THIS IS THE SUPPOSEDLY UPDATED hkCreateMove function, dont you lie to me unknown cheats
double __fastcall Hooks::hkCreateMove(void* thisptr, unsigned int a2, CUserCmd* pCmd)
{
	double result = oCreateMove(thisptr, a2, pCmd);

	if (!pCmd || !Globals::bhop_enabled)
		return result;

	std::cout << "[DEBUG] pCmd: " << pCmd << "\n";

	if (!pCmd->csgoUserCmd.pBaseCmd)
	{
		std::cout << "[DEBUG] pBaseCmd null\n";
		return result;
	}

	std::cout << "[DEBUG] pBaseCmd: " << pCmd->csgoUserCmd.pBaseCmd << "\n";

	if (!pCmd->csgoUserCmd.pBaseCmd->pInButtonState)
	{
		std::cout << "[DEBUG] pInButtonState null\n";
		return result;
	}

	std::cout << "[DEBUG] Valid! Calling Bhop\n";
	Bhop::Run(pCmd);

	return result;
}
*/

// this will be the outdate hkcreatemove but isn't crashing so pick your poison

//DEBUG END ------------------



void Hooks::Destroy()
{

	MH_DisableHook(MH_ALL_HOOKS);
	MH_Uninitialize();

	if (g_Window && oWndProc)
		SetWindowLongPtr(g_Window, GWLP_WNDPROC, (LONG_PTR)oWndProc);

	if (!g_Init)
		return;

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	if (g_RTV)
		g_RTV->Release();
	if (g_Context)
		g_Context->Release();
	if (g_Device)
		g_Device->Release();

	g_Init = false;
}
