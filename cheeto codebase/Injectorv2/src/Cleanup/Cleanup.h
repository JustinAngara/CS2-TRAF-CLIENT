#pragma once
#include "../Life/InjectorContext.h"
#include <windows.h>
namespace Cleanup
{
	bool WaitAndCleanUp(InjectorContext& ctx, HANDLE hProcess, BYTE* pTargetBase, IMAGE_NT_HEADERS* pNtHeaders, void* pShellcode, BYTE* pMappingDataAlloc, bool cleanHeader, bool cleanUnneededSections, bool adjustProtections, bool sehSupport);
	bool WaitForInjection(InjectorContext& ctx, HANDLE hProcess, BYTE* pMappingData, HINSTANCE& hModule, std::wstring& errorMsg);
	bool CleanAndProtectMemory(InjectorContext& ctx, HANDLE hProcess, BYTE* pTargetBase, IMAGE_NT_HEADERS* pNtHeaders, void* pShellcode, BYTE* pMappingData, bool cleanHeader, bool cleanUnneededSections, bool adjustProtections, bool sehSupport, std::wstring& errorMsg);

}
