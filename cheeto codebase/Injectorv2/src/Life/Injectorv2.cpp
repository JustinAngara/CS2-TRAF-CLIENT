#include "../Global/HandleGuard.h"
#include "../Global/Resource.h"
#include "InjectorContext.h"
#include "../Memory/ManualMap.h"
#include "../Global/Conditionals.h"
#include "../Shellcode/ShellcodeHelper.h"
#include "Memory.h"
#include "Procedure.h"
#include "../Cleanup/Cleanup.h"
#include <Windows.h>
#include <winnt.h>
#include <chrono>
#include <string>
#include <vector>
#include <CommCtrl.h>
#include <random>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shlwapi.lib")



bool ManualMapDLL(InjectorContext& ctx, HANDLE hProcess, const BYTE* pSourceData, SIZE_T fileSize, bool cleanHeader, bool cleanUnneededSections, bool adjustProtections, bool sehSupport, DWORD reason, LPVOID reserved)
{
	try
	{
		auto startTime = std::chrono::high_resolution_clock::now();
		Conditionals::LogErrorAndStatus(ctx, L"[+] Initializing injection process...", RGB(0, 255, 0), false);
		InjectionHelpers::InitializeProgressBar(ctx.hwndProgressBar);

		std::random_device				rd;
		std::mt19937					gen(rd());
		std::uniform_int_distribution<> dis(5, 15);

		BYTE*			  pTargetBase = nullptr;
		IMAGE_NT_HEADERS* pNtHeaders  = nullptr;
		DWORD			  oldProtect  = 0;
		if (!Memory::AllocateAndWriteHeaders(ctx, hProcess, pSourceData, fileSize, pTargetBase, pNtHeaders, oldProtect))
		{
			return false;
		}
		InjectionHelpers::StepProgressBarWithDelay(ctx.hwndProgressBar, gen, dis);

		if (!Memory::WriteSectionsToMemory(ctx, hProcess, pTargetBase, pSourceData, pNtHeaders))
		{
			VirtualFreeEx(hProcess, pTargetBase, 0, MEM_RELEASE);
			return false;
		}
		InjectionHelpers::StepProgressBarWithDelay(ctx.hwndProgressBar, gen, dis);

		BYTE* pMappingDataAlloc = nullptr;
		if (!InjectionHelpers::PrepareMappingData(ctx, hProcess, pTargetBase, sehSupport, reason, reserved, pMappingDataAlloc))
		{
			VirtualFreeEx(hProcess, pTargetBase, 0, MEM_RELEASE);
			return false;
		}
		InjectionHelpers::StepProgressBarWithDelay(ctx.hwndProgressBar, gen, dis);

		void* pShellcode = nullptr;
		if (!ShellcodeHelper::AllocateAndWriteShellcodeAndExecute(ctx, hProcess, pMappingDataAlloc, pShellcode))
		{
			InjectionHelpers::CleanupOnFailure(hProcess, pTargetBase, pMappingDataAlloc, nullptr);
			return false;
		}
		InjectionHelpers::StepProgressBarWithDelay(ctx.hwndProgressBar, gen, dis);

		if (!Cleanup::WaitAndCleanUp(ctx, hProcess, pTargetBase, pNtHeaders, pShellcode, pMappingDataAlloc, cleanHeader, cleanUnneededSections, adjustProtections, sehSupport))
		{
			InjectionHelpers::CleanupOnFailure(hProcess, pTargetBase, pMappingDataAlloc, pShellcode);
			return false;
		}
		InjectionHelpers::StepProgressBarWithDelay(ctx.hwndProgressBar, gen, dis);

		InjectionHelpers::LogCompletionTime(ctx, startTime);
		InjectionHelpers::FinalizeProgressBar(ctx.hwndProgressBar);
		return true;
	}
	catch (const std::exception& e)
	{
		std::wstring error = L"[-] Exception in ManualMapDLL: " + std::wstring(e.what(), e.what() + strlen(e.what()));
		Conditionals::LogErrorAndStatus(ctx, error, RGB(255, 0, 0), true);
		InjectionHelpers::SetProgressBarError(ctx.hwndProgressBar);
		return false;
	}
}

static bool PerformInjection(InjectorContext& ctx, const HandleGuard& hProcess, const std::vector<BYTE>& dllData)
{
	Conditionals::LogErrorAndStatus(ctx, L"[+] Starting DLL injection process...", RGB(0, 255, 0), false);
	if (!ManualMapDLL(ctx, hProcess, dllData.data(), dllData.size(), true, true, true, true, DLL_PROCESS_ATTACH, nullptr))
	{
		Conditionals::LogErrorAndStatus(ctx, L"[-] Error during injection", RGB(255, 0, 0), true);
		return false;
	}
	Conditionals::LogErrorAndStatus(ctx, L"[+] INJECTION COMPLETED SUCCESSFULLY!", RGB(0, 255, 0), false);
	return true;
}

static void HandleInject(InjectorContext& ctx, HWND hwnd)
{
	if (!CommandHandlers::ValidateInjectionPreconditions(ctx, hwnd))
		return;

	if (!CommandHandlers::ConfirmInjection(ctx, hwnd))
		return;

	DWORD pid = CommandHandlers::FindTargetProcess(ctx);
	if (pid == 0)
		return;

	CommandHandlers::EnableDebugPrivileges(ctx);

	HandleGuard hProcess(CommandHandlers::OpenTargetProcess(ctx, pid));
	if (!hProcess.get()) return;

	if (!CommandHandlers::VerifyArchitecture(ctx, hProcess))
		return;

	std::vector<BYTE> dllData = CommandHandlers::LoadDLLData(ctx);
	if (dllData.empty())
		return;

	if (!Conditionals::CheckDLLArchitecture(ctx, dllData, hProcess))
		return;

	if (PerformInjection(ctx, hProcess, dllData))
	{
		SetTimer(hwnd, 1, 5000, NULL);
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static InjectorContext ctx;
	static HFONT		   hFontTitle, hFontButton, hFontStatus, hFontFooter;
	static HBITMAP		   hBitmap			= nullptr;
	static HBRUSH		   hBackgroundBrush = CreateSolidBrush(RGB(12, 16, 25));
	static HBRUSH		   hStatusBrush		= CreateSolidBrush(RGB(0, 0, 0));

	switch (msg)
	{
	case WM_CREATE:
	{
		INITCOMMONCONTROLSEX icex = { sizeof(INITCOMMONCONTROLSEX), ICC_STANDARD_CLASSES | ICC_PROGRESS_CLASS };
		InitCommonControlsEx(&icex);
		SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)hBackgroundBrush);

		UIHelpers::CreateFonts(hFontTitle, hFontButton, hFontStatus, hFontFooter);
		hBitmap = UIHelpers::LoadBitmapResource(ctx);

		ctx.hwndMain = hwnd;
		UIHelpers::CreateImageControl(hwnd, hBitmap);
		UIHelpers::CreateTitleControl(hwnd, hFontTitle);
		UIHelpers::CreateProcessControls(ctx, hwnd, hFontStatus, hFontButton);
		UIHelpers::CreateDLLControls(ctx, hwnd, hFontStatus, hFontButton);
		UIHelpers::CreateProgressBar(ctx, hwnd);
		UIHelpers::CreateStatusControl(ctx, hwnd, hFontStatus);
		UIHelpers::CreateFooterControl(hwnd, hFontFooter);
		UIHelpers::PopulateProcessList(ctx);
		UIHelpers::RestoreLastSelection(ctx);
		break;
	}
	case WM_CTLCOLORSTATIC:
	{
		HDC	 hdc   = (HDC)wParam;
		HWND hCtrl = (HWND)lParam;
		if (hCtrl == ctx.hwndProcessLabel || hCtrl == ctx.hwndDllLabel || hCtrl == GetDlgItem(hwnd, 3))
		{
			SetTextColor(hdc, RGB(255, 255, 255));
			SetBkColor(hdc, RGB(12, 16, 25));
			return (LRESULT)hBackgroundBrush;
		}
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkColor(hdc, RGB(12, 16, 25));
		return (LRESULT)hBackgroundBrush;
	}
	case WM_CTLCOLOREDIT:
	{
		HDC hdc = (HDC)wParam;
		SetTextColor(hdc, RGB(0, 255, 0));
		SetBkColor(hdc, RGB(0, 0, 0));
		return (LRESULT)hStatusBrush;
	}
	case WM_CTLCOLORBTN:
	{
		HDC hdc = (HDC)wParam;
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkColor(hdc, RGB(100, 149, 237));
		return (LRESULT)CreateSolidBrush(RGB(100, 149, 237));
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC			hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, hBackgroundBrush);
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_TIMER:
	{
		if (wParam == 1)
		{
			KillTimer(hwnd, 1);
			PostMessage(hwnd, WM_CLOSE, 0, 0);
		}
		break;
	}
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == 4)
		{
			CommandHandlers::HandleRefresh(ctx);
		}
		else if (HIWORD(wParam) == CBN_SELCHANGE && LOWORD(wParam) == IDD_PROCESSSELECT)
		{
			CommandHandlers::HandleProcessSelection(ctx);
		}
		else if (LOWORD(wParam) == 1)
		{
			CommandHandlers::HandleBrowseDLL(ctx, hwnd);
		}
		else if (LOWORD(wParam) == 2)
		{
			// cannot persist at commandhandlers
			HandleInject(ctx, hwnd);
		}
		break;
	}
	case WM_DESTROY:
		if (hBitmap) DeleteObject(hBitmap);
		DeleteObject(hFontTitle);
		DeleteObject(hFontButton);
		DeleteObject(hFontStatus);
		DeleteObject(hFontFooter);
		DeleteObject(hBackgroundBrush);
		DeleteObject(hStatusBrush);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	// --- 1. Randomize EXE Name (Self-Obfuscation) ---
	std::random_device			  rd;
	std::mt19937				  gen(rd());
	std::string					  chars = "$%&/=$abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	std::uniform_int_distribution<int> dis(0, static_cast<int>(chars.size() - 1));
	std::wstring					   randomStr;
	for (int i = 0; i < 4; ++i)
	{
		randomStr += static_cast<wchar_t>(chars[dis(gen)]);
	}

	std::wstring exeName = L"Traf" + randomStr + L".exe";
	wchar_t currentExePath[MAX_PATH];
	GetModuleFileNameW(NULL, currentExePath, MAX_PATH);
	std::wstring newExePath = std::wstring(currentExePath);
	size_t	lastSlash  = newExePath.find_last_of(L"\\");

	if (lastSlash != std::wstring::npos)
	{
		newExePath = newExePath.substr(0, lastSlash + 1) + exeName;
	}
	else
	{
		newExePath = exeName;
	}

	// Note: This renames the file on disk even while it's running.
	MoveFileW(currentExePath, newExePath.c_str());

	// --- 2. Admin Check ---
	if (!Conditionals::IsRunAsAdmin())
	{
		MessageBoxW(NULL, L"Injector MUST be run as Administrator.", L"Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	// --- 3. Window Registration ---
	WNDCLASSW wc	 = { 0 };
	wc.lpfnWndProc	 = WndProc; // Make sure WndProc is defined!
	wc.hInstance	 = hInstance;
	wc.lpszClassName = L"InjectorWindowClass";
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	RegisterClassW(&wc);

	// Calculate window position to center it
	RECT rc = { 0, 0, 800, 565 };
	AdjustWindowRect(&rc, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);
	int windowWidth	 = rc.right - rc.left;
	int windowHeight = rc.bottom - rc.top;
	int screenWidth	 = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int posX		 = (screenWidth - windowWidth) / 2;
	int posY		 = (screenHeight - windowHeight) / 2;

	// --- 4. Create Window ---
	HWND hwndMain = CreateWindowW(L"InjectorWindowClass", L"Traf Injector",
	WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
	posX, posY, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);

	if (!hwndMain)
	{
		MessageBoxW(NULL, L"Error creating window.", L"Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	ShowWindow(hwndMain, nCmdShow);
	UpdateWindow(hwndMain);

	// --- 5. Message Loop ---
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}
