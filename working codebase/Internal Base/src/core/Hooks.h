#pragma once
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>

class Hooks
{
public:
	static void Setup();
	static void Destroy();

	// Existing hooks
	static LRESULT __stdcall hkWndProc(HWND, UINT, WPARAM, LPARAM);
	static HRESULT __stdcall hkPresent(IDXGISwapChain*, UINT, UINT);

	// Add CreateMove hook
	static void __fastcall hkCreateMove(void* thisptr, int slot, float flInputSampleTime, bool bActive);
	inline static void(__fastcall* oCreateMove)(void*, int, float, bool) = nullptr;

	inline static WNDPROC oWndProc = nullptr;
	inline static HRESULT(__stdcall* oPresent)(IDXGISwapChain*, UINT, UINT) = nullptr;

};
