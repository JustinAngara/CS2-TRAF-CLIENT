#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include "../Life/InjectorContext.h"

namespace Conditionals
{
	std::wstring GetCurrentTimestamp();
	void LogToMemory(InjectorContext& ctx, const std::wstring& message);
	void LogErrorAndStatus(InjectorContext& ctx, const std::wstring& message, COLORREF color, bool isError);
	bool IsRunAsAdmin();
	BOOL Is64BitWindows();
	BOOL Is64BitProcess(HANDLE hProcess, PBOOL isWow64);
	bool IsCorrectArchitecture(HANDLE hProcess);
	bool CheckDLLArchitecture(InjectorContext& ctx, const std::vector<BYTE>& dllData, HANDLE hProcess);
	bool ValidatePEHeaders(InjectorContext& ctx, const BYTE* pSourceData, SIZE_T fileSize, std::wstring& errorMsg);
}
