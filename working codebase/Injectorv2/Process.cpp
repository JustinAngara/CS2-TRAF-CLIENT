#include "Process.h"
#include "HandleGuard.h"
#include "Conditionals.h"
#include <TlHelp32.h>

DWORD Process::GetPIDByName(InjectorContext& ctx, const std::wstring& name)
{
	HandleGuard snapshot(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));
	if (snapshot.get() == INVALID_HANDLE_VALUE)
	{
		DWORD error = GetLastError();
		Conditionals::LogErrorAndStatus(ctx, L"[-] Failed to create process snapshot, error code: 0x" + std::to_wstring(error), RGB(255, 0, 0), true);
		return 0;
	}
	PROCESSENTRY32W entry = { sizeof(PROCESSENTRY32W) };
	if (!Process32FirstW(snapshot, &entry))
	{
		DWORD error = GetLastError();
		Conditionals::LogErrorAndStatus(ctx, L"[-] Failed to enumerate first process, error code: 0x" + std::to_wstring(error), RGB(255, 0, 0), true);
		return 0;
	}
	std::wstring lowerName = name;
	std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::towlower);
	std::wstring foundProcesses;
	do
	{
		std::wstring exeName(entry.szExeFile);
		std::transform(exeName.begin(), exeName.end(), exeName.begin(), ::towlower);
		if (exeName == lowerName)
		{
			Conditionals::LogErrorAndStatus(ctx, L"[+] Found process: " + std::wstring(entry.szExeFile) + L" (PID: " + std::to_wstring(entry.th32ProcessID) + L")", RGB(0, 255, 0), false);
			return entry.th32ProcessID;
		}
		foundProcesses += std::wstring(entry.szExeFile) + L", ";
	} while (Process32NextW(snapshot, &entry));
	Conditionals::LogErrorAndStatus(ctx, L"[-] Process not found: " + name + L". Processes scanned: " + foundProcesses, RGB(255, 0, 0), true);
	return 0;
}
