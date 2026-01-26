#include "Cleanup.h"
#include "../Global/Conditionals.h"
#include "../Life/InjectorContext.h"
#include "../Global/Globals.h"
#include <windows.h>
#include <thread>
#include <chrono>

bool Cleanup::WaitAndCleanUp(InjectorContext& ctx, HANDLE hProcess, BYTE* pTargetBase, IMAGE_NT_HEADERS* pNtHeaders, void* pShellcode, BYTE* pMappingDataAlloc, bool cleanHeader, bool cleanUnneededSections, bool adjustProtections, bool sehSupport)
{
	std::wstring errorMsg;
	HINSTANCE	 hModule = nullptr;
	if (!WaitForInjection(ctx, hProcess, pMappingDataAlloc, hModule, errorMsg))
	{
		Conditionals::LogErrorAndStatus(ctx, errorMsg, RGB(255, 0, 0), true);
		return false;
	}
	Conditionals::LogErrorAndStatus(ctx, errorMsg, RGB(0, 255, 0), false);
	if (!CleanAndProtectMemory(ctx, hProcess, pTargetBase, pNtHeaders, pShellcode, pMappingDataAlloc, cleanHeader, cleanUnneededSections, adjustProtections, sehSupport, errorMsg))
	{
		Conditionals::LogErrorAndStatus(ctx, errorMsg, RGB(255, 0, 0), true);
		return false;
	}
	Conditionals::LogErrorAndStatus(ctx, errorMsg, RGB(0, 255, 0), false);
	return true;
}

bool Cleanup::CleanAndProtectMemory(InjectorContext& ctx, HANDLE hProcess, BYTE* pTargetBase,
	IMAGE_NT_HEADERS* pNtHeaders, void* pShellcode, BYTE* pMappingData,
	bool cleanHeader, bool cleanUnneededSections, bool adjustProtections, bool sehSupport, std::wstring& errorMsg)
{
	std::random_device				rd;
	std::mt19937					gen(rd());
	std::uniform_int_distribution<> dis(5, 15);
	if (cleanHeader)
	{
		BYTE   cleanBuffer[0x1000] = { 0 };
		SIZE_T bytesWritten		   = 0;
		if (!WriteProcessMemory(hProcess, pTargetBase, cleanBuffer, pNtHeaders->OptionalHeader.SizeOfHeaders, &bytesWritten))
		{
			errorMsg = L"[-] Error cleaning headers, code: 0x" + std::to_wstring(GetLastError());
			return false;
		}
	}
	if (cleanUnneededSections)
	{
		BYTE				  cleanBuffer[0x1000] = { 0 };
		IMAGE_SECTION_HEADER* pSectionHeader	  = IMAGE_FIRST_SECTION(pNtHeaders);
		SIZE_T				  bytesWritten		  = 0;
		for (WORD i = 0; i < pNtHeaders->FileHeader.NumberOfSections; ++i)
		{
			bool isExecutable = (pSectionHeader[i].Characteristics & IMAGE_SCN_MEM_EXECUTE) != 0;
			bool isReadable	  = (pSectionHeader[i].Characteristics & IMAGE_SCN_MEM_READ) != 0;
			bool isWritable	  = (pSectionHeader[i].Characteristics & IMAGE_SCN_MEM_WRITE) != 0;
			if (!isExecutable && !isReadable && !isWritable)
			{
				if (pSectionHeader[i].SizeOfRawData)
				{
					if (!WriteProcessMemory(hProcess, pTargetBase + pSectionHeader[i].VirtualAddress,
						cleanBuffer, pSectionHeader[i].SizeOfRawData, &bytesWritten))
					{
						errorMsg = L"[-] Error cleaning section " + std::to_wstring(i) + L", code: 0x" + std::to_wstring(GetLastError());
						return false;
					}
				}
			}
		}
	}
	if (adjustProtections)
	{
		IMAGE_SECTION_HEADER* pSectionHeader = IMAGE_FIRST_SECTION(pNtHeaders);
		for (WORD i = 0; i < pNtHeaders->FileHeader.NumberOfSections; ++i)
		{
			DWORD oldProtect = 0;
			DWORD newProtect = 0;
			if (pSectionHeader[i].Characteristics & IMAGE_SCN_MEM_EXECUTE)
			{
				newProtect = (pSectionHeader[i].Characteristics & IMAGE_SCN_MEM_WRITE) ?
				PAGE_EXECUTE_READWRITE :
				PAGE_EXECUTE_READ;
			}
			else
			{
				newProtect = (pSectionHeader[i].Characteristics & IMAGE_SCN_MEM_WRITE) ?
				PAGE_READWRITE :
				PAGE_READONLY;
			}
			if (pSectionHeader[i].SizeOfRawData)
			{
				if (!VirtualProtectEx(hProcess, pTargetBase + pSectionHeader[i].VirtualAddress,
					pSectionHeader[i].SizeOfRawData, newProtect, &oldProtect))
				{
					errorMsg = L"[-] Error adjusting section protection " + std::to_wstring(i) + L", code: 0x" + std::to_wstring(GetLastError());
					return false;
				}
			}
		}
	}
	if (pShellcode)
	{
		VirtualFreeEx(hProcess, pShellcode, 0, MEM_RELEASE);
	}
	if (pMappingData)
	{
		VirtualFreeEx(hProcess, pMappingData, 0, MEM_RELEASE);
	}
	errorMsg = L"[+] Memory cleaned and protections adjusted";
	return true;
}


bool Cleanup::WaitForInjection(InjectorContext& ctx, HANDLE hProcess, BYTE* pMappingData, HINSTANCE& hModule, std::wstring& errorMsg)
{
	std::random_device				rd;
	std::mt19937					gen(rd());
	std::uniform_int_distribution<> dis(5, 15);
	for (int i = 0; i < 100; ++i)
	{
		SIZE_T	  bytesRead = 0;
		HINSTANCE tempModule;
		if (!ReadProcessMemory(hProcess, pMappingData + offsetof(MANUAL_MAPPING_DATA, hMod),
			&tempModule, sizeof(HINSTANCE), &bytesRead) ||
		bytesRead != sizeof(HINSTANCE))
		{
			errorMsg = L"[-] Error reading module handle, code: 0x" + std::to_wstring(GetLastError());
			return false;
		}
		if (tempModule != nullptr)
		{
			hModule = tempModule;
			if (hModule == reinterpret_cast<HINSTANCE>(0x404040))
			{
				errorMsg = L"[-] Injection failed (shellcode returned error)";
				return false;
			}
			if (hModule == reinterpret_cast<HINSTANCE>(0x505050))
			{
				errorMsg = L"[!] Injection completed but SEH support failed";
				hModule	 = reinterpret_cast<HINSTANCE>(pMappingData);
				return true;
			}
			errorMsg = L"[+] Injection successful, module handle retrieved";
			return true;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
	}
	errorMsg = L"[-] Injection timed out";
	return false;
}
