#pragma once
#include "InjectorContext.h"
#include "Globals.h"
#include <Richedit.h>
#include <vector>
#include <CommCtrl.h>

#pragma comment(lib, "comctl32.lib")
typedef LONG NTSTATUS;
extern "C" NTSTATUS NtWriteVirtualMemory_Syscall(
	HANDLE ProcessHandle,
	PVOID BaseAddress,
	PVOID Buffer,
	SIZE_T NumberOfBytesToWrite,
	PSIZE_T NumberOfBytesWritten
);
namespace Memory
{
	std::vector<BYTE> LoadDLL(InjectorContext& ctx, const std::wstring& dllPath);
	BYTE* AllocateProcessMemory(InjectorContext& ctx, HANDLE hProcess, SIZE_T size, DWORD& oldProtect, std::wstring& errorMsg);
	bool WritePEHeaders(InjectorContext& ctx, HANDLE hProcess, BYTE* pTargetBase, const BYTE* pSourceData, std::wstring& errorMsg);
	bool WriteSections(InjectorContext& ctx, HANDLE hProcess, BYTE* pTargetBase, const BYTE* pSourceData, IMAGE_NT_HEADERS* pNtHeaders, std::wstring& errorMsg);
	bool WriteSectionsToMemory(InjectorContext& ctx, HANDLE hProcess, BYTE* pTargetBase, const BYTE* pSourceData, IMAGE_NT_HEADERS* pNtHeaders);
	BYTE* AllocateMappingData(InjectorContext& ctx, HANDLE hProcess, const MANUAL_MAPPING_DATA& mappingData, std::wstring& errorMsg);
	bool AllocateAndWriteHeaders(InjectorContext& ctx, HANDLE hProcess, const BYTE* pSourceData, SIZE_T fileSize, BYTE*& pTargetBase, IMAGE_NT_HEADERS*& pNtHeaders, DWORD& oldProtect);

}
