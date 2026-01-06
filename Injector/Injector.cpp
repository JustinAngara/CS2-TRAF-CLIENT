
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>

DWORD GetProcessIdByName(const wchar_t* processName)
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE)
		return 0;

	PROCESSENTRY32W entry = { sizeof(PROCESSENTRY32W) };

	if (Process32FirstW(snapshot, &entry))
	{
		do
		{
			if (_wcsicmp(entry.szExeFile, processName) == 0)
			{
				CloseHandle(snapshot);
				return entry.th32ProcessID;
			}
		} while (Process32NextW(snapshot, &entry));
	}

	CloseHandle(snapshot);
	return 0;
}

bool InjectDLL(DWORD processId, const char* dllPath)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	if (!hProcess)
	{
		std::cout << "Failed to open process. Error: " << GetLastError() << std::endl;
		return false;
	}

	void* allocMem = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1,
	MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!allocMem)
	{
		std::cout << "Failed to allocate memory. Error: " << GetLastError() << std::endl;
		CloseHandle(hProcess);
		return false;
	}

	if (!WriteProcessMemory(hProcess, allocMem, dllPath, strlen(dllPath) + 1, NULL))
	{
		std::cout << "Failed to write memory. Error: " << GetLastError() << std::endl;
		VirtualFreeEx(hProcess, allocMem, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return false;
	}

	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
	(LPTHREAD_START_ROUTINE)LoadLibraryA,
	allocMem, 0, NULL);
	if (!hThread)
	{
		std::cout << "Failed to create remote thread. Error: " << GetLastError() << std::endl;
		VirtualFreeEx(hProcess, allocMem, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return false;
	}

	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hThread);
	VirtualFreeEx(hProcess, allocMem, 0, MEM_RELEASE);
	CloseHandle(hProcess);

	return true;
}

int main()
{
	const char* dllPath = "C:\\Users\\justi\\source\\repos\\asphyxia\\asphyxia-cs2\\build\\Debug\\cstrike.dll";
	


	std::cout << "Waiting for cs2.exe..." << std::endl;

	DWORD processId = 0;
	while (!processId)
	{
		processId = GetProcessIdByName(L"cs2.exe");
		Sleep(1000);
	}

	std::cout << "Found cs2.exe (PID: " << processId << ")" << std::endl;
	std::cout << "Injecting DLL..." << std::endl;

	if (InjectDLL(processId, dllPath))
	{
		std::cout << "Successfully injected!" << std::endl;
	}
	else
	{
		std::cout << "Injection failed!" << std::endl;
	}

	system("pause");
	return 0;
}

