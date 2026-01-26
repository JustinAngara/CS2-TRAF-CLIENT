#include "ShellcodeHelper.h"
#include "../Global/Conditionals.h"
#include "../Memory/Memory.h"
#include <winnt.h>


#define RELOC_FLAG(RelInfo) ((RelInfo >> 0x0C) == IMAGE_REL_BASED_DIR64)

#pragma runtime_checks("", off)
#pragma optimize("", off)

void __stdcall Shellcode(MANUAL_MAPPING_DATA* pData)
{
	if (!pData) return;
	BYTE* pBase = pData->pBase;
	if (!pBase)
	{
		pData->hMod = reinterpret_cast<HINSTANCE>(0x404040);
		return;
	}
	IMAGE_DOS_HEADER* pDosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(pBase);
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		pData->hMod = reinterpret_cast<HINSTANCE>(0x404040);
		return;
	}
	IMAGE_NT_HEADERS* pNtHeaders = reinterpret_cast<IMAGE_NT_HEADERS*>(pBase + pDosHeader->e_lfanew);
	if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE)
	{
		pData->hMod = reinterpret_cast<HINSTANCE>(0x404040);
		return;
	}
	IMAGE_OPTIONAL_HEADER* pOptionalHeader		= &pNtHeaders->OptionalHeader;
	auto				   pLoadLibrary			= pData->pLoadLibraryA;
	auto				   pGetProcAddress		= pData->pGetProcAddress;
	auto				   pRtlAddFunctionTable = pData->pRtlAddFunctionTable;
	auto				   DllEntry				= reinterpret_cast<f_DLL_ENTRY_POINT>(pBase + pOptionalHeader->AddressOfEntryPoint);
	uintptr_t			   delta				= reinterpret_cast<uintptr_t>(pBase) - pOptionalHeader->ImageBase;
	if (delta != 0 && pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size)
	{
		auto* pRelocData = reinterpret_cast<IMAGE_BASE_RELOCATION*>(pBase + pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
		auto* pRelocEnd	 = reinterpret_cast<IMAGE_BASE_RELOCATION*>(reinterpret_cast<BYTE*>(pRelocData) + pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size);
		while (pRelocData < pRelocEnd && pRelocData->SizeOfBlock)
		{
			UINT  count			= (pRelocData->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
			WORD* pRelativeInfo = reinterpret_cast<WORD*>(pRelocData + 1);
			for (UINT i = 0; i < count; ++i, ++pRelativeInfo)
			{
				if (RELOC_FLAG(*pRelativeInfo))
				{
					uintptr_t* pPatch = reinterpret_cast<uintptr_t*>(pBase + pRelocData->VirtualAddress + ((*pRelativeInfo) & 0xFFF));
					*pPatch += static_cast<uintptr_t>(delta);
				}
			}
			pRelocData = reinterpret_cast<IMAGE_BASE_RELOCATION*>(reinterpret_cast<BYTE*>(pRelocData) + pRelocData->SizeOfBlock);
		}
	}
	if (pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size)
	{
		auto* pImportDesc = reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(pBase + pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
		while (pImportDesc->Name)
		{
			char*	  szMod = reinterpret_cast<char*>(pBase + pImportDesc->Name);
			HINSTANCE hDll	= pLoadLibrary(szMod);
			if (!hDll)
			{
				pData->hMod = reinterpret_cast<HINSTANCE>(0x404040);
				return;
			}
			ULONG_PTR* pThunkRef = reinterpret_cast<ULONG_PTR*>(pBase + pImportDesc->OriginalFirstThunk);
			ULONG_PTR* pFuncRef	 = reinterpret_cast<ULONG_PTR*>(pBase + pImportDesc->FirstThunk);
			if (!pThunkRef) pThunkRef = pFuncRef;
			for (; *pThunkRef; ++pThunkRef, ++pFuncRef)
			{
				if (IMAGE_SNAP_BY_ORDINAL(*pThunkRef))
				{
					*pFuncRef = reinterpret_cast<ULONG_PTR>(pGetProcAddress(hDll, reinterpret_cast<char*>(*pThunkRef & 0xFFFF)));
				}
				else
				{
					auto* pImport = reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(pBase + *pThunkRef);
					*pFuncRef	  = reinterpret_cast<ULONG_PTR>(pGetProcAddress(hDll, pImport->Name));
				}
				if (!*pFuncRef)
				{
					pData->hMod = reinterpret_cast<HINSTANCE>(0x404040);
					return;
				}
			}
			++pImportDesc;
		}
	}
	if (pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size)
	{
		auto* pTls		= reinterpret_cast<IMAGE_TLS_DIRECTORY*>(pBase + pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);
		auto* pCallback = reinterpret_cast<PIMAGE_TLS_CALLBACK*>(pTls->AddressOfCallBacks);
		while (pCallback && *pCallback)
		{
			(*pCallback)(pBase, DLL_PROCESS_ATTACH, nullptr);
			++pCallback;
		}
	}
	bool bExceptionSupportFailed = false;
	if (pData->bSEHSupport && pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].Size)
	{
		auto* pExceptionTable = reinterpret_cast<PRUNTIME_FUNCTION>(pBase + pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].VirtualAddress);
		DWORD entryCount	  = pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].Size / sizeof(RUNTIME_FUNCTION);
		if (!pRtlAddFunctionTable(pExceptionTable, entryCount, reinterpret_cast<DWORD64>(pBase)))
		{
			bExceptionSupportFailed = true;
		}
	}
	if (!DllEntry(pBase, pData->dwReason, pData->lpReserved))
	{
		pData->hMod = reinterpret_cast<HINSTANCE>(0x404040);
		return;
	}
	pData->hMod = bExceptionSupportFailed ? reinterpret_cast<HINSTANCE>(0x505050) : reinterpret_cast<HINSTANCE>(pBase);
}

#pragma optimize("", on)
#pragma runtime_checks("", restore)

///////////////////////////////////////////// HELPERS


void* ShellcodeHelper::AllocateAndWriteShellcode(InjectorContext& ctx, HANDLE hProcess, std::wstring& errorMsg)
{
	void* pShellcode = VirtualAllocEx(hProcess, nullptr, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!pShellcode)
	{
		errorMsg = L"[-] Error allocating shellcode memory, code: 0x" + std::to_wstring(GetLastError());
		return nullptr;
	}
	SIZE_T bytesWritten = 0;
	if (!WriteProcessMemory(hProcess, pShellcode, Shellcode, 4096, &bytesWritten) || bytesWritten != 4096)
	{
		errorMsg = L"[-] Error writing shellcode, code: 0x" + std::to_wstring(GetLastError());
		VirtualFreeEx(hProcess, pShellcode, 0, MEM_RELEASE);
		return nullptr;
	}
	errorMsg = L"[+] Shellcode allocated and written";
	return pShellcode;
}

bool ShellcodeHelper::ExecuteShellcode(InjectorContext& ctx, HANDLE hProcess, void* pShellcode, BYTE* pMappingData, std::wstring& errorMsg)
{
	HANDLE hThread = CreateRemoteThread(hProcess, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(pShellcode),
	pMappingData, 0, nullptr);
	if (!hThread)
	{
		errorMsg = L"[-] Error creating remote thread, code: 0x" + std::to_wstring(GetLastError());
		return false;
	}
	CloseHandle(hThread);
	errorMsg = L"[+] Remote thread created for shellcode execution";
	return true;
}

bool ShellcodeHelper::AllocateAndWriteShellcodeAndExecute(InjectorContext& ctx, HANDLE hProcess, BYTE* pMappingDataAlloc, void*& pShellcode)
{
	std::wstring errorMsg;
	pShellcode = AllocateAndWriteShellcode(ctx, hProcess, errorMsg);
	if (!pShellcode)
	{
		Conditionals::LogErrorAndStatus(ctx, errorMsg, RGB(255, 0, 0), true);
		return false;
	}
	Conditionals::LogErrorAndStatus(ctx, errorMsg, RGB(0, 255, 0), false);
	if (!ExecuteShellcode(ctx, hProcess, pShellcode, pMappingDataAlloc, errorMsg))
	{
		Conditionals::LogErrorAndStatus(ctx, errorMsg, RGB(255, 0, 0), true);
		return false;
	}
	Conditionals::LogErrorAndStatus(ctx, errorMsg, RGB(0, 255, 0), false);
	return true;
}
