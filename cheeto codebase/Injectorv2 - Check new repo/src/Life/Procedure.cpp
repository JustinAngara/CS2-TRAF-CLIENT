#include "Procedure.h"
#include "../Global/Conditionals.h"
#include "../Global/Process.h"
#include "../Global/Resource.h"
#include "../Global/HandleGuard.h"
#include "../Memory/Memory.h"
#include <TlHelp32.h>
#include <CommCtrl.h>
#include <WinUser.h>
#include <Richedit.h>
#include <winnt.h>

void UIHelpers::CreateFonts(HFONT& hFontTitle, HFONT& hFontButton, HFONT& hFontStatus, HFONT& hFontFooter)
{
	hFontTitle	= CreateFontW(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
	 CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
	hFontButton = CreateFontW(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
	CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
	hFontStatus = CreateFontW(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
	CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
	hFontFooter = CreateFontW(14, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
	CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
}


HBITMAP UIHelpers::LoadBitmapResource(InjectorContext& ctx)
{
	HBITMAP hBitmap = (HBITMAP)LoadImageW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_TRAF_BMP), IMAGE_BITMAP, 0, 0, 0);
	if (!hBitmap)
	{
		DWORD error = GetLastError();
		Conditionals::LogErrorAndStatus(ctx, L"[-] Error loading bitmap, code: 0x" + std::to_wstring(error), RGB(255, 0, 0), true);
	}
	return hBitmap;
}

void UIHelpers::CreateImageControl(HWND hwnd, HBITMAP hBitmap)
{
	HWND hImage = CreateWindowW(L"STATIC", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP | SS_CENTERIMAGE,
	(800 - 362) / 2, 20, 362, 55, hwnd, NULL, GetModuleHandle(NULL), NULL);
	if (hBitmap)
	{
		SendMessage(hImage, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
	}
}

void UIHelpers::CreateTitleControl(HWND hwnd, HFONT hFontTitle)
{
	HWND hTitle = CreateWindowW(L"STATIC", L"DLL Injector",
	WS_VISIBLE | WS_CHILD | SS_CENTER, 50, 95, 700, 50, hwnd, NULL, GetModuleHandle(NULL), NULL);
	SendMessage(hTitle, WM_SETFONT, (WPARAM)hFontTitle, TRUE);
}

void UIHelpers::CreateProcessControls(InjectorContext& ctx, HWND hwnd, HFONT hFontStatus, HFONT hFontButton)
{
	ctx.hwndProcessLabel = CreateWindowW(L"STATIC", L"Selected Process: None", WS_VISIBLE | WS_CHILD | SS_CENTER,
	50, 125, 330, 20, hwnd, (HMENU)5, GetModuleHandle(NULL), NULL);
	SendMessage(ctx.hwndProcessLabel, WM_SETFONT, (WPARAM)hFontStatus, TRUE);

	ctx.hwndProcessCombo = CreateWindowW(L"COMBOBOX", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_SORT | WS_VSCROLL,
	50, 155, 280, 200, hwnd, (HMENU)IDD_PROCESSSELECT, GetModuleHandle(NULL), NULL);
	SendMessage(ctx.hwndProcessCombo, WM_SETFONT, (WPARAM)hFontButton, TRUE);

	ctx.hwndRefreshButton = CreateWindowW(L"BUTTON", L"Refresh", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT,
	340, 155, 50, 30, hwnd, (HMENU)4, GetModuleHandle(NULL), NULL);
	SendMessage(ctx.hwndRefreshButton, WM_SETFONT, (WPARAM)hFontButton, TRUE);
}

void UIHelpers::CreateDLLControls(InjectorContext& ctx, HWND hwnd, HFONT hFontStatus, HFONT hFontButton)
{
	ctx.hwndDllLabel = CreateWindowW(L"STATIC", L"Selected DLL: None", WS_VISIBLE | WS_CHILD | SS_CENTER,
	400, 125, 300, 20, hwnd, (HMENU)6, GetModuleHandle(NULL), NULL);
	SendMessage(ctx.hwndDllLabel, WM_SETFONT, (WPARAM)hFontStatus, TRUE);

	ctx.hwndBrowseButton = CreateWindowW(L"BUTTON", L"Select .DLL...", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT,
	400, 155, 120, 30, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);
	SendMessage(ctx.hwndBrowseButton, WM_SETFONT, (WPARAM)hFontButton, TRUE);

	ctx.hwndInjectButton = CreateWindowW(L"BUTTON", L"INJECT !", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT,
	530, 155, 120, 30, hwnd, (HMENU)2, GetModuleHandle(NULL), NULL);
	SendMessage(ctx.hwndInjectButton, WM_SETFONT, (WPARAM)hFontButton, TRUE);
}

void UIHelpers::CreateProgressBar(InjectorContext& ctx, HWND hwnd)
{
	ctx.hwndProgressBar = CreateWindowW(PROGRESS_CLASSW, NULL, WS_VISIBLE | WS_CHILD | PBS_SMOOTH,
	50, 205, 700, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
}

void UIHelpers::CreateStatusControl(InjectorContext& ctx, HWND hwnd, HFONT hFontStatus)
{
	ctx.hwndStatus = CreateWindowExW(0, L"EDIT", L"Status: Ready to select process and DLL\r\n", WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE | ES_READONLY,
	50, 245, 700, 240, hwnd, NULL, GetModuleHandle(NULL), NULL);
	SendMessage(ctx.hwndStatus, WM_SETFONT, (WPARAM)hFontStatus, TRUE);

	CHARFORMATW cf = { sizeof(CHARFORMATW) };
	cf.dwMask	   = CFM_COLOR;
	cf.crTextColor = RGB(0, 255, 0);
	SendMessageW(ctx.hwndStatus, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);
}

void UIHelpers::CreateFooterControl(HWND hwnd, HFONT hFontFooter)
{
	HWND hFooter = CreateWindowW(L"STATIC", L"",
	WS_VISIBLE | WS_CHILD | SS_CENTER, 50, 495, 700, 40, hwnd, (HMENU)3, GetModuleHandle(NULL), NULL);
	SendMessage(hFooter, WM_SETFONT, (WPARAM)hFontFooter, TRUE);
}

void UIHelpers::PopulateProcessList(InjectorContext& ctx)
{
	HandleGuard snapshot(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));
	if (snapshot.get() == INVALID_HANDLE_VALUE)
	{
		Conditionals::LogErrorAndStatus(ctx, L"[-] Failed to load process list, please try refreshing", RGB(255, 0, 0), true);
		EnableWindow(ctx.hwndInjectButton, FALSE);
		return;
	}

	PROCESSENTRY32W entry = { sizeof(PROCESSENTRY32W) };
	if (Process32FirstW(snapshot, &entry))
	{
		do
		{
			std::wstring display = std::wstring(entry.szExeFile) + L" (PID: " + std::to_wstring(entry.th32ProcessID) + L")";
			SendMessageW(ctx.hwndProcessCombo, CB_ADDSTRING, 0, (LPARAM)display.c_str());
		} while (Process32NextW(snapshot, &entry));
	}
	SendMessageW(ctx.hwndProcessCombo, CB_SETCURSEL, 0, 0);
}

void UIHelpers::RestoreLastSelection(InjectorContext& ctx)
{
	if (!ctx.processName.empty())
	{
		std::wstring searchStr = ctx.processName + L" (PID: ";
		LRESULT		 index	   = SendMessageW(ctx.hwndProcessCombo, CB_FINDSTRING, (WPARAM)-1, (LPARAM)searchStr.c_str());
		if (index != CB_ERR)
		{
			SendMessageW(ctx.hwndProcessCombo, CB_SETCURSEL, index, 0);
			SetWindowTextW(ctx.hwndProcessLabel, (L"Selected Process: " + ctx.processName).c_str());
		}
	}
	if (!ctx.dllPath.empty())
	{
		SetWindowTextW(ctx.hwndDllLabel, (L"Selected DLL: " + ctx.dllPath).c_str());
		Conditionals::LogErrorAndStatus(ctx, L"[+] Loaded last DLL: " + ctx.dllPath, RGB(0, 255, 0), false);
	}
	EnableWindow(ctx.hwndInjectButton, !ctx.processName.empty() && !ctx.dllPath.empty());
}




///////////////// COMMAND HELPERS
 
void CommandHandlers::HandleRefresh(InjectorContext& ctx)
{
	SendMessageW(ctx.hwndProcessCombo, CB_RESETCONTENT, 0, 0);
	HandleGuard snapshot(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));
	if (snapshot.get() == INVALID_HANDLE_VALUE)
	{
		Conditionals::LogErrorAndStatus(ctx, L"[-] Failed to refresh process list", RGB(255, 0, 0), true);
		EnableWindow(ctx.hwndInjectButton, FALSE);
	}
	else
	{
		PROCESSENTRY32W entry = { sizeof(PROCESSENTRY32W) };
		if (Process32FirstW(snapshot, &entry))
		{
			do
			{
				std::wstring display = std::wstring(entry.szExeFile) + L" (PID: " + std::to_wstring(entry.th32ProcessID) + L")";
				SendMessageW(ctx.hwndProcessCombo, CB_ADDSTRING, 0, (LPARAM)display.c_str());
			} while (Process32NextW(snapshot, &entry));
		}
		SendMessageW(ctx.hwndProcessCombo, CB_SETCURSEL, 0, 0);
		Conditionals::LogErrorAndStatus(ctx, L"[+] Process list refreshed", RGB(0, 255, 0), false);
		EnableWindow(ctx.hwndInjectButton, !ctx.processName.empty() && !ctx.dllPath.empty());
	}
}

void CommandHandlers::HandleProcessSelection(InjectorContext& ctx)
{
	LRESULT index = SendMessageW(ctx.hwndProcessCombo, CB_GETCURSEL, 0, 0);
	if (index != CB_ERR)
	{
		wchar_t buffer[260];
		SendMessageW(ctx.hwndProcessCombo, CB_GETLBTEXT, index, (LPARAM)buffer);
		std::wstring selected = buffer;
		size_t		 pos	  = selected.find(L" (PID:");
		if (pos != std::wstring::npos)
		{
			ctx.processName = selected.substr(0, pos);
			SetWindowTextW(ctx.hwndProcessLabel, (L"Selected Process: " + ctx.processName).c_str());
			WritePrivateProfileStringW(L"Settings", L"LastProcess", ctx.processName.c_str(), L"Injector.ini");
			Conditionals::LogErrorAndStatus(ctx, L"[+] Selected process: " + ctx.processName, RGB(0, 255, 0), false);
			EnableWindow(ctx.hwndInjectButton, !ctx.processName.empty() && !ctx.dllPath.empty());
		}
	}
}

void CommandHandlers::HandleBrowseDLL(InjectorContext& ctx, HWND hwnd)
{
	OPENFILENAMEW ofn		  = { 0 };
	wchar_t		  szFile[260] = { 0 };
	ofn.lStructSize			  = sizeof(ofn);
	ofn.hwndOwner			  = hwnd;
	ofn.lpstrFile			  = szFile;
	ofn.nMaxFile			  = sizeof(szFile) / sizeof(*szFile);
	ofn.lpstrFilter			  = L"DLL Files\0*.dll\0All\0*.*\0";
	ofn.nFilterIndex		  = 1;
	ofn.Flags				  = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetOpenFileNameW(&ofn))
	{
		ctx.dllPath = szFile;
		if (!ctx.ValidateDLLPath(ctx.dllPath))
		{
			ctx.dllPath.clear();
			SetWindowTextW(ctx.hwndDllLabel, L"Selected DLL: None");
			EnableWindow(ctx.hwndInjectButton, FALSE);
			return;
		}
		SetWindowTextW(ctx.hwndDllLabel, (L"Selected DLL: " + ctx.dllPath).c_str());
		WritePrivateProfileStringW(L"Settings", L"LastDLL", ctx.dllPath.c_str(), L"Injector.ini");
		std::wstring status = L"[+] DLL selected: " + ctx.dllPath;
		Conditionals::LogErrorAndStatus(ctx, status, RGB(0, 255, 0), false);
		EnableWindow(ctx.hwndInjectButton, !ctx.processName.empty() && !ctx.dllPath.empty());
	}
}

bool CommandHandlers::ValidateInjectionPreconditions(InjectorContext& ctx, HWND hwnd)
{
	if (!Conditionals::IsRunAsAdmin())
	{
		MessageBoxW(hwnd, L"DLL Injector MUST be run as Administrator.", L"Error", MB_OK | MB_ICONERROR);
		Conditionals::LogErrorAndStatus(ctx, L"[-] Application MUST be run as administrator", RGB(255, 0, 0), true);
		return false;
	}
	if (ctx.dllPath.empty())
	{
		Conditionals::LogErrorAndStatus(ctx, L"[-] Please select a DLL file first", RGB(255, 0, 0), true);
		return false;
	}
	if (ctx.processName.empty())
	{
		Conditionals::LogErrorAndStatus(ctx, L"[-] Please select a process first", RGB(255, 0, 0), true);
		return false;
	}
	return true;
}

bool CommandHandlers::ConfirmInjection(InjectorContext& ctx, HWND hwnd)
{
	std::wstring confirmMsg = L"Are you sure you want to inject\n" + ctx.dllPath + L"\ninto process: " + ctx.processName + L"?";
	if (MessageBoxW(hwnd, confirmMsg.c_str(), L"Confirm Injection", MB_YESNO | MB_ICONQUESTION) != IDYES)
	{
		Conditionals::LogErrorAndStatus(ctx, L"[*] Injection cancelled by user", RGB(255, 255, 0), false);
		return false;
	}
	return true;
}

DWORD CommandHandlers::FindTargetProcess(InjectorContext& ctx)
{
	Conditionals::LogErrorAndStatus(ctx, L"[*] Searching for process: " + ctx.processName, RGB(255, 255, 0), false);
	DWORD pid = Process::GetPIDByName(ctx, ctx.processName);
	if (pid == 0)
	{
		Conditionals::LogErrorAndStatus(ctx, L"[-] Target process not found. Ensure the process is running!", RGB(255, 0, 0), true);
		return 0;
	}
	std::wstring pidStatus = L"[+] Injecting into target process (PID: " + std::to_wstring(pid) + L")";
	Conditionals::LogErrorAndStatus(ctx, pidStatus, RGB(0, 255, 0), false);
	return pid;
}

void CommandHandlers::EnableDebugPrivileges(InjectorContext& ctx)
{
	HandleGuard hToken;
	HANDLE		hTokenTemp = nullptr;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hTokenTemp))
	{
		hToken.reset(hTokenTemp);
		TOKEN_PRIVILEGES privileges			= { 0 };
		privileges.PrivilegeCount			= 1;
		privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if (LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &privileges.Privileges[0].Luid))
		{
			AdjustTokenPrivileges(hToken, FALSE, &privileges, 0, nullptr, nullptr);
			Conditionals::LogErrorAndStatus(ctx, L"[+] Debug privileges enabled", RGB(0, 255, 0), false);
		}
		else
		{
			Conditionals::LogErrorAndStatus(ctx, L"[!] Warning: Could not enable debug privileges, code: 0x" + std::to_wstring(GetLastError()), RGB(255, 255, 0), true);
		}
	}
	else
	{
		Conditionals::LogErrorAndStatus(ctx, L"[!] Warning: Could not open process token, code: 0x" + std::to_wstring(GetLastError()), RGB(255, 225, 0), true);
	}
}

HANDLE CommandHandlers::OpenTargetProcess(InjectorContext& ctx, DWORD pid)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!hProcess || hProcess == INVALID_HANDLE_VALUE)
	{
		const DWORD err = GetLastError();
		Conditionals::LogErrorAndStatus(
		ctx,
		L"[-] Error opening target process, code: 0x" + std::to_wstring(err),
		RGB(255, 0, 0),
		true);
		return nullptr;
	}

	Conditionals::LogErrorAndStatus(
	ctx,
	L"[+] Target process opened successfully",
	RGB(0, 255, 0),
	false);

	return hProcess; // caller must CloseHandle() or wrap in HandleGuard
}

bool CommandHandlers::VerifyArchitecture(InjectorContext& ctx, const HandleGuard& hProcess)
{
	if (!Conditionals::IsCorrectArchitecture(hProcess))
	{
		Conditionals::LogErrorAndStatus(ctx, L"[-] Target process architecture not compatible", RGB(255, 0, 0), true);
		return false;
	}
	Conditionals::LogErrorAndStatus(ctx, L"[+] Target process architecture verified", RGB(0, 255, 0), false);
	return true;
}

std::vector<BYTE> CommandHandlers::LoadDLLData(InjectorContext& ctx)
{
	std::vector<BYTE> dllData;
	try
	{
		dllData = Memory::LoadDLL(ctx, ctx.dllPath);
		Conditionals::LogErrorAndStatus(ctx, L"[+] DLL file loaded successfully", RGB(0, 255, 0), false);
	}
	catch (const std::exception& e)
	{
		std::wstring error = L"[-] Error loading DLL: " + std::wstring(e.what(), e.what() + strlen(e.what()));
		Conditionals::LogErrorAndStatus(ctx, error, RGB(255, 0, 0), true);
		return std::vector<BYTE>();
	}
	return dllData;
}
