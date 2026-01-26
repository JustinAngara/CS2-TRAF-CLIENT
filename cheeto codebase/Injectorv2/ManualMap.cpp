#include "ManualMap.h"

void InjectionHelpers::InitializeProgressBar(HWND hwndProgressBar)
{
	SendMessage(hwndProgressBar, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
	SendMessage(hwndProgressBar, PBM_SETSTEP, (WPARAM)10, 0);
	SendMessage(hwndProgressBar, PBM_SETSTATE, PBST_NORMAL, 0);	SendMessage(hwndProgressBar, PBM_SETPOS, 0, 0);
}

void InjectionHelpers::StepProgressBarWithDelay(HWND hwndProgressBar, std::mt19937& gen, std::uniform_int_distribution<>& dis)
{
	SendMessage(hwndProgressBar, PBM_STEPIT, 0, 0);
	std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
}

void InjectionHelpers::FinalizeProgressBar(HWND hwndProgressBar)
{
	SendMessage(hwndProgressBar, PBM_SETPOS, 100, 0);
	SendMessage(hwndProgressBar, PBM_SETSTATE, PBST_NORMAL, 0);
}

void InjectionHelpers::SetProgressBarError(HWND hwndProgressBar)
{
	SendMessage(hwndProgressBar, PBM_SETSTATE, PBST_ERROR, 0);
}


void InjectionHelpers::LogCompletionTime(InjectorContext& ctx, std::chrono::high_resolution_clock::time_point startTime)
{
	auto			   endTime	   = std::chrono::high_resolution_clock::now();
	auto			   durationMs  = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
	double			   durationSec = durationMs / 1000.0;
	std::wstringstream durationStream;
	durationStream << std::fixed << std::setprecision(3) << durationSec;
	Conditionals::LogErrorAndStatus(ctx, L"[+] Injection completed in " + durationStream.str() + L" seconds", RGB(0, 255, 0), false);
}


void InjectionHelpers::CleanupOnFailure(HANDLE hProcess, BYTE* pTargetBase, BYTE* pMappingDataAlloc, void* pShellcode)
{
	if (pTargetBase)
		VirtualFreeEx(hProcess, pTargetBase, 0, MEM_RELEASE);
	if (pMappingDataAlloc)
		VirtualFreeEx(hProcess, pMappingDataAlloc, 0, MEM_RELEASE);
	if (pShellcode)
		VirtualFreeEx(hProcess, pShellcode, 0, MEM_RELEASE);
}


bool InjectionHelpers::PrepareMappingData(InjectorContext& ctx, HANDLE hProcess, BYTE* pTargetBase, bool sehSupport, DWORD reason, LPVOID reserved, BYTE*& pMappingDataAlloc)
{
	std::wstring		errorMsg;
	MANUAL_MAPPING_DATA mappingData = { 0 };
	mappingData.pLoadLibraryA		= LoadLibraryA;
	mappingData.pGetProcAddress		= GetProcAddress;
	HMODULE hNtdll					= GetModuleHandleA(ctx.ntdllName);
	if (!hNtdll)
	{
		Conditionals::LogErrorAndStatus(ctx, L"[-] Error getting handle to module, code: 0x" + std::to_wstring(GetLastError()), RGB(255, 0, 0), true);
		return false;
	}
	mappingData.pRtlAddFunctionTable = reinterpret_cast<f_RtlAddFunctionTable>(GetProcAddress(hNtdll, "RtlAddFunctionTable"));
	mappingData.pBase				 = pTargetBase;
	mappingData.dwReason			 = reason;
	mappingData.lpReserved			 = reserved;
	mappingData.bSEHSupport			 = sehSupport;
	pMappingDataAlloc				 = Memory::AllocateMappingData(ctx, hProcess, mappingData, errorMsg);
	if (!pMappingDataAlloc)
	{
		Conditionals::LogErrorAndStatus(ctx, errorMsg, RGB(255, 0, 0), true);
		return false;
	}
	Conditionals::LogErrorAndStatus(ctx, errorMsg, RGB(0, 255, 0), false);
	return true;
}
