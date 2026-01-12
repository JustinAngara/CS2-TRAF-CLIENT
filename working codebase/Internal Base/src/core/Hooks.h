#pragma once
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>

class Hooks
{
public:
	static void Setup();
	static void Destroy();

	// Hooks
	static LRESULT __stdcall hkWndProc(HWND, UINT, WPARAM, LPARAM);
	static HRESULT __stdcall hkPresent(IDXGISwapChain*, UINT, UINT);
	static void __fastcall hkCreateMove(void* thisptr, int slot, bool active);

	// Originals
	using CreateMove_t = void(__fastcall*)(void*, int, bool);
	inline static CreateMove_t oCreateMove = nullptr;
	inline static WNDPROC oWndProc = nullptr;
	inline static HRESULT(__stdcall* oPresent)(IDXGISwapChain*, UINT, UINT) = nullptr;
};
