// iiiii.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <string>

// Get process ID by name
DWORD GetProcessIdByName(const std::wstring& processName)
{
    DWORD processId = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snapshot == INVALID_HANDLE_VALUE)
    {
        std::wcerr << L"Failed to create process snapshot" << std::endl;
        return 0;
    }

    PROCESSENTRY32W entry;
    entry.dwSize = sizeof(PROCESSENTRY32W);

    if (Process32FirstW(snapshot, &entry))
    {
        do
        {
            if (_wcsicmp(entry.szExeFile, processName.c_str()) == 0)
            {
                processId = entry.th32ProcessID;
                break;
            }
        } while (Process32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return processId;
}

// Inject DLL into process
bool InjectDLL(DWORD processId, const std::wstring& dllPath)
{
    // Verify DLL exists
    if (GetFileAttributesW(dllPath.c_str()) == INVALID_FILE_ATTRIBUTES)
    {
        std::wcerr << L"[ERROR] DLL file not found: " << dllPath << std::endl;
        return false;
    }

    std::wcout << L"[INFO] Target Process ID: " << processId << std::endl;
    std::wcout << L"[INFO] DLL Path: " << dllPath << std::endl;

    // Open target process
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (!hProcess)
    {
        std::wcerr << L"[ERROR] Failed to open process. Error: " << GetLastError() << std::endl;
        std::wcerr << L"[ERROR] Make sure you're running as Administrator!" << std::endl;
        return false;
    }
    std::wcout << L"[OK] Process opened successfully" << std::endl;

    // Calculate size needed for DLL path (Unicode = 2 bytes per char)
    SIZE_T dllPathSize = (dllPath.length() + 1) * sizeof(wchar_t);

    // Allocate memory in target process
    LPVOID pRemoteBuf = VirtualAllocEx(
        hProcess,
        NULL,
        dllPathSize,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );

    if (!pRemoteBuf)
    {
        std::wcerr << L"[ERROR] VirtualAllocEx failed. Error: " << GetLastError() << std::endl;
        CloseHandle(hProcess);
        return false;
    }
    std::wcout << L"[OK] Memory allocated at: 0x" << std::hex << pRemoteBuf << std::dec << std::endl;

    // Write DLL path to allocated memory
    SIZE_T bytesWritten = 0;
    if (!WriteProcessMemory(hProcess, pRemoteBuf, dllPath.c_str(), dllPathSize, &bytesWritten))
    {
        std::wcerr << L"[ERROR] WriteProcessMemory failed. Error: " << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }
    std::wcout << L"[OK] DLL path written (" << bytesWritten << L" bytes)" << std::endl;

    // Get LoadLibraryW address (use W for wide/unicode)
    HMODULE hKernel32 = GetModuleHandleW(L"kernel32.dll");
    if (!hKernel32)
    {
        std::wcerr << L"[ERROR] Failed to get kernel32.dll handle" << std::endl;
        VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    LPVOID pLoadLibraryW = (LPVOID)GetProcAddress(hKernel32, "LoadLibraryW");
    if (!pLoadLibraryW)
    {
        std::wcerr << L"[ERROR] Failed to get LoadLibraryW address" << std::endl;
        VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }
    std::wcout << L"[OK] LoadLibraryW address: 0x" << std::hex << pLoadLibraryW << std::dec << std::endl;

    // Create remote thread to call LoadLibraryW
    HANDLE hThread = CreateRemoteThread(
        hProcess,
        NULL,
        0,
        (LPTHREAD_START_ROUTINE)pLoadLibraryW,
        pRemoteBuf,
        0,
        NULL
    );

    if (!hThread)
    {
        std::wcerr << L"[ERROR] CreateRemoteThread failed. Error: " << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }
    std::wcout << L"[OK] Remote thread created" << std::endl;

    // Wait for thread to complete (max 5 seconds)
    std::wcout << L"[INFO] Waiting for LoadLibraryW to complete..." << std::endl;
    DWORD waitResult = WaitForSingleObject(hThread, 5000);

    if (waitResult == WAIT_TIMEOUT)
    {
        std::wcerr << L"[ERROR] Thread timed out!" << std::endl;
        CloseHandle(hThread);
        VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    // Get thread exit code (= DLL base address if successful, 0 if failed)
    DWORD exitCode = 0;
    GetExitCodeThread(hThread, &exitCode);

    std::wcout << L"[INFO] LoadLibraryW returned: 0x" << std::hex << exitCode << std::dec << std::endl;

    // Cleanup
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);
    CloseHandle(hProcess);

    if (exitCode == 0)
    {
        std::wcerr << L"[FAIL] LoadLibraryW returned NULL - DLL failed to load!" << std::endl;
        std::wcerr << L"[FAIL] Possible reasons:" << std::endl;
        std::wcerr << L"  - Wrong architecture (x86 vs x64)" << std::endl;
        std::wcerr << L"  - Missing dependencies" << std::endl;
        std::wcerr << L"  - DLL_PROCESS_ATTACH returned FALSE" << std::endl;
        std::wcerr << L"  - DLL crashed during initialization" << std::endl;
        return false;
    }

    std::wcout << L"[SUCCESS] DLL injected! Module base: 0x" << std::hex << exitCode << std::dec << std::endl;
    return true;
}

int main()
{
    std::wcout << L"========================================" << std::endl;
    std::wcout << L"         DLL Injector v1.0" << std::endl;
    std::wcout << L"========================================" << std::endl;
    std::wcout << std::endl;

    // Configuration
    const std::wstring targetProcess = L"cs2.exe";
    const std::wstring dllPath = LR"(C:\Users\justi\Downloads\new internal\CS2 Internal\x64\Release\InternalBase.dll)";

    std::wcout << L"[INFO] Target Process: " << targetProcess << std::endl;
    std::wcout << L"[INFO] DLL Path: " << dllPath << std::endl;
    std::wcout << std::endl;

    // Wait for target process
    std::wcout << L"[INFO] Waiting for " << targetProcess << L"..." << std::endl;

    DWORD processId = 0;
    while (processId == 0)
    {
        processId = GetProcessIdByName(targetProcess);
        if (processId == 0)
        {
            Sleep(1000);
        }
    }

    std::wcout << L"[OK] Process found! PID: " << processId << std::endl;
    std::wcout << std::endl;

    // Small delay to ensure process is fully initialized
    std::wcout << L"[INFO] Waiting 2 seconds for process initialization..." << std::endl;
    Sleep(2000);

    // Inject DLL
    std::wcout << L"[INFO] Starting injection..." << std::endl;
    std::wcout << std::endl;

    if (InjectDLL(processId, dllPath))
    {
        std::wcout << std::endl;
        std::wcout << L"========================================" << std::endl;
        std::wcout << L"    Injection completed successfully!" << std::endl;
        std::wcout << L"========================================" << std::endl;
    }
    else
    {
        std::wcout << std::endl;
        std::wcout << L"========================================" << std::endl;
        std::wcout << L"         Injection failed!" << std::endl;
        std::wcout << L"========================================" << std::endl;
    }

    std::wcout << std::endl;
    system("pause");
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

// C:\\Users\\justi\\Downloads\\new internal\\CS2 Internal\\x64\\Debug\\Internal Base.dll