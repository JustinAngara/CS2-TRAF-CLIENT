#pragma once

#include "../Life/InjectorContext.h"
#include "../Cleanup/Cleanup.h"
#include "../Global/Conditionals.h"
#include "Memory.h"
#include <windows.h>
#include <thread>
#include <chrono>
#include <random>
#include <CommCtrl.h>
namespace InjectionHelpers
{
	void InitializeProgressBar(HWND hwndProgressBar);
	void StepProgressBarWithDelay(HWND hwndProgressBar, std::mt19937& gen, std::uniform_int_distribution<>& dis);
	void FinalizeProgressBar(HWND hwndProgressBar);

	void SetProgressBarError(HWND hwndProgressBar);

	void LogCompletionTime(InjectorContext& ctx, std::chrono::high_resolution_clock::time_point startTime);

	void CleanupOnFailure(HANDLE hProcess, BYTE* pTargetBase, BYTE* pMappingDataAlloc, void* pShellcode);
	bool PrepareMappingData(InjectorContext& ctx, HANDLE hProcess, BYTE* pTargetBase, bool sehSupport, DWORD reason, LPVOID reserved, BYTE*& pMappingDataAlloc);

}

