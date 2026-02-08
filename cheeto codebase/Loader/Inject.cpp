#include "Inject.h"


#include "ext/HandleGuard.h"
#include "ext/Resource.h"
#include "ext/InjectorContext.h"
#include "ext/ManualMap.h"
#include "ext/Conditionals.h"
#include "ext/ShellcodeHelper.h"
#include "ext/Memory.h" 
#include "ext/Procedure.h"
#include "ext/Cleanup.h"

#include <Windows.h>
#include <tlhelp32.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

const std::wstring DLL_PATH		  = LR"(C:\Users\justi\source\repos\asphyxia\asphyxia-cs2\cheeto codebase\x64\Release\Internal Base.dll)";
const std::wstring TARGET_PROCESS = L"cs2.exe";

DWORD GetProcessIdByName(const std::wstring& processName)
{
	DWORD  pid		 = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32W pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32W);
		if (Process32FirstW(hSnapshot, &pe32))
		{
			do
			{
				if (processName == pe32.szExeFile)
				{
					pid = pe32.th32ProcessID;
					break;
				}
			} while (Process32NextW(hSnapshot, &pe32));
		}
		CloseHandle(hSnapshot);
	}
	return pid;
}

std::vector<BYTE> LoadFileToBuffer(const std::wstring& path)
{
	std::ifstream file(path, std::ios::binary | std::ios::ate);
	if (!file) return {};

	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<BYTE> buffer(size);
	if (file.read((char*)buffer.data(), size))
		return buffer;

	return {};
}

bool ManualMapHeadless(HANDLE hProcess, const std::vector<BYTE>& dllData)
{
	InjectorContext dummyCtx; 

	BYTE*			  pTargetBase = nullptr;
	IMAGE_NT_HEADERS* pNtHeaders  = nullptr;
	DWORD			  oldProtect  = 0;

	if (!Memory::AllocateAndWriteHeaders(dummyCtx, hProcess, dllData.data(), dllData.size(), pTargetBase, pNtHeaders, oldProtect))
		return false;

	if (!Memory::WriteSectionsToMemory(dummyCtx, hProcess, pTargetBase, dllData.data(), pNtHeaders))
	{
		VirtualFreeEx(hProcess, pTargetBase, 0, MEM_RELEASE);
		return false;
	}

	BYTE* pMappingDataAlloc = nullptr;

	if (!InjectionHelpers::PrepareMappingData(dummyCtx, hProcess, pTargetBase, true, DLL_PROCESS_ATTACH, nullptr, pMappingDataAlloc))
	{
		VirtualFreeEx(hProcess, pTargetBase, 0, MEM_RELEASE);
		return false;
	}

	void* pShellcode = nullptr;
	if (!ShellcodeHelper::AllocateAndWriteShellcodeAndExecute(dummyCtx, hProcess, pMappingDataAlloc, pShellcode))
	{
		return false;
	}

	Cleanup::WaitAndCleanUp(dummyCtx, hProcess, pTargetBase, pNtHeaders, pShellcode, pMappingDataAlloc, true, true, true, true);

	return true;
}

void Inject::Run()
{
	DWORD pid = GetProcessIdByName(TARGET_PROCESS);
	if (pid == 0) return;

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!hProcess) return;

	std::vector<BYTE> dllData = LoadFileToBuffer(DLL_PATH);
	if (!dllData.empty())
	{
		ManualMapHeadless(hProcess, dllData);
	}

	CloseHandle(hProcess);
}
