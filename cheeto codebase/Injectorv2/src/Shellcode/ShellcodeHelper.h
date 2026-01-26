#pragma once
#include "../Life/InjectorContext.h"
#include "../Memory/Memory.h"
#include <Windows.h>


void __stdcall Shellcode(MANUAL_MAPPING_DATA* pData);

namespace ShellcodeHelper
{
	void* AllocateAndWriteShellcode(InjectorContext& ctx, HANDLE hProcess, std::wstring& errorMsg);
	bool ExecuteShellcode(InjectorContext& ctx, HANDLE hProcess, void* pShellcode, BYTE* pMappingData, std::wstring& errorMsg);
	bool AllocateAndWriteShellcodeAndExecute(InjectorContext& ctx, HANDLE hProcess, BYTE* pMappingDataAlloc, void*& pShellcode);

}
