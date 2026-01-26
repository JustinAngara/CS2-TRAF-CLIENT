#pragma once
#include <windows.h>


// macros
#if defined(_MSC_VER) && (_MSC_VER >= 1900)
#define NOEXCEPT noexcept
#else
#define NOEXCEPT
#endif


// type aliases 
using f_LoadLibraryA		= HINSTANCE(WINAPI*)(LPCSTR);
using f_GetProcAddress		= FARPROC(WINAPI*)(HMODULE, LPCSTR);
using f_DLL_ENTRY_POINT		= BOOL(WINAPI*)(void*, DWORD, void*);
using f_RtlAddFunctionTable = BOOLEAN(WINAPI*)(PRUNTIME_FUNCTION, DWORD, DWORD64);

struct MANUAL_MAPPING_DATA
{
	f_LoadLibraryA		  pLoadLibraryA		   = nullptr;
	f_GetProcAddress	  pGetProcAddress	   = nullptr;
	f_RtlAddFunctionTable pRtlAddFunctionTable = nullptr;
	BYTE*				  pBase				   = nullptr;
	HINSTANCE			  hMod				   = nullptr;
	DWORD				  dwReason			   = 0;
	LPVOID				  lpReserved		   = nullptr;
	BOOL				  bSEHSupport		   = FALSE;
};
