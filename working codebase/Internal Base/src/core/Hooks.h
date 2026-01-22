#pragma once
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include "../../src/sdk/utils/usermode.h"

/*
Everything done is through a tick-based style
everything needs re-rendering/processing
is called on a (1000/60)hz basis
*/

class Hooks
{
public:
	static void Setup();
	static void Destroy();

	// hooks
	static LRESULT __stdcall hkWndProc(HWND, UINT, WPARAM, LPARAM);
	static HRESULT __stdcall hkPresent(IDXGISwapChain*, UINT, UINT);
	static void __fastcall hkCreateMove(void* thisptr, int slot, bool active);
	// static double __fastcall hkCreateMove(void* thisptr, unsigned int a2, CUserCmd* pCmd);

	// debug
	//using CreateMove_t = double(__fastcall*)(void*, unsigned int, CUserCmd*);

	// originals
	using CreateMove_t = void(__fastcall*)(void*, int, bool);
	inline static CreateMove_t oCreateMove = nullptr;
	inline static WNDPROC oWndProc = nullptr;
	inline static HRESULT(__stdcall* oPresent)(IDXGISwapChain*, UINT, UINT) = nullptr;

	


};
