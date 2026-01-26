#include "Conditionals.h"
#include <chrono>

#include <Richedit.h>
std::wstring Conditionals::GetCurrentTimestamp()
{
	auto now  = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(now);
	tm	 local_time;
	localtime_s(&local_time, &time);
	std::wstringstream wss;
	wss << std::put_time(&local_time, L"%Y-%m-%d %H:%M:%S")
		<< L"." << std::setfill(L'0') << std::setw(3)
		<< (std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000).count();
	return wss.str();
}


void Conditionals::LogToMemory(InjectorContext& ctx, const std::wstring& message)
{
	if (!ctx.enableLogging) return;
	std::wstring timestampedMessage = L"[" + GetCurrentTimestamp() + L"] " + message;
	ctx.logBuffer.push_back(timestampedMessage);
}


void Conditionals::LogErrorAndStatus(InjectorContext& ctx, const std::wstring& message, COLORREF color, bool isError)
{
	std::wstring timestampedMessage = L"[" + GetCurrentTimestamp() + L"] " + message;
	LRESULT			len				   = SendMessageW(ctx.hwndStatus, WM_GETTEXTLENGTH, 0, 0) + 1;
	std::vector<wchar_t> buffer(len);
	SendMessageW(ctx.hwndStatus, WM_GETTEXT, len, (LPARAM)buffer.data());
	std::wstring currentText(buffer.data());
	currentText		= currentText.substr(0, currentText.find_last_not_of(L"\r\n") + 1);
	std::wstring newText = currentText.empty() ? timestampedMessage : currentText + L"\r\n" + timestampedMessage;
	SendMessageW(ctx.hwndStatus, WM_SETTEXT, 0, (LPARAM)newText.c_str());
	CHARFORMATW cf = { sizeof(CHARFORMATW) };
	cf.dwMask	   = CFM_COLOR;
	cf.crTextColor = color;
	SendMessageW(ctx.hwndStatus, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);
	SendMessageW(ctx.hwndStatus, EM_SETSEL, (WPARAM)-1, (LPARAM)-1);
	SendMessageW(ctx.hwndStatus, EM_SCROLLCARET, 0, 0);
	SendMessageW(ctx.hwndStatus, WM_VSCROLL, SB_BOTTOM, 0);
	InvalidateRect(ctx.hwndStatus, NULL, TRUE);
	UpdateWindow(ctx.hwndStatus);
	LogToMemory(ctx, timestampedMessage);
	if (isError)
	{
		// do something if error
	}
}



bool Conditionals::IsRunAsAdmin()
{
	BOOL					 isAdmin	 = FALSE;
	PSID					 adminGroup	 = nullptr;
	SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
	if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup))
	{
		CheckTokenMembership(NULL, adminGroup, &isAdmin);
		FreeSid(adminGroup);
	}
	return isAdmin;
}

BOOL Conditionals::Is64BitWindows()
{
	SYSTEM_INFO si;
	GetNativeSystemInfo(&si);
	return (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64);
}

BOOL Conditionals::Is64BitProcess(HANDLE hProcess, PBOOL isWow64)
{
	if (!Is64BitWindows())
	{
		*isWow64 = FALSE;
		return TRUE;
	}
	return IsWow64Process(hProcess, isWow64);
}


bool Conditionals::IsCorrectArchitecture(HANDLE hProcess)
{
	if (!Is64BitWindows()) return true;
	BOOL isTargetWow64 = FALSE;
	if (!Is64BitProcess(hProcess, &isTargetWow64))
	{
		return false;
	}
	BOOL isHostWow64 = FALSE;
	Is64BitProcess(GetCurrentProcess(), &isHostWow64);
	return isTargetWow64 == isHostWow64;
}

bool Conditionals::CheckDLLArchitecture(InjectorContext& ctx, const std::vector<BYTE>& dllData, HANDLE hProcess)
{
	try
	{
		if (dllData.size() < sizeof(IMAGE_DOS_HEADER))
		{
			LogErrorAndStatus(ctx, L"[-] Invalid DLL data size for architecture check", RGB(255, 0, 0), true);
			return false;
		}
		const BYTE*		  rawData	 = dllData.data();
		IMAGE_DOS_HEADER* pDosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(const_cast<BYTE*>(rawData));
		if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
		{
			LogErrorAndStatus(ctx, L"[-] Invalid DLL (no MZ signature)", RGB(255, 0, 0), true);
			return false;
		}
		if (static_cast<SIZE_T>(pDosHeader->e_lfanew) + sizeof(IMAGE_NT_HEADERS) > dllData.size() || pDosHeader->e_lfanew < 0)
		{
			LogErrorAndStatus(ctx, L"[-] Invalid NT headers offset in DLL", RGB(255, 0, 0), true);
			return false;
		}
		IMAGE_NT_HEADERS* pNtHeaders = reinterpret_cast<IMAGE_NT_HEADERS*>(const_cast<BYTE*>(rawData + pDosHeader->e_lfanew));
		if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE)
		{
			LogErrorAndStatus(ctx, L"[-] Invalid NT signature in DLL", RGB(255, 0, 0), true);
			return false;
		}
		BOOL isProcessWow64 = FALSE;
		if (!Is64BitProcess(hProcess, &isProcessWow64))
		{
			LogErrorAndStatus(ctx, L"[-] Error checking process architecture for DLL validation", RGB(255, 0, 0), true);
			return false;
		}
		bool isProcess64Bit = !isProcessWow64 && Is64BitWindows();
		bool isDLL64Bit = pNtHeaders->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64;
		if (isDLL64Bit != isProcess64Bit)
		{
			LogErrorAndStatus(ctx, L"[-] DLL architecture does not match process architecture", RGB(255, 0, 0), true);
			return false;
		}
		LogErrorAndStatus(ctx, L"[+] DLL architecture verified", RGB(0, 255, 0), false);
		return true;
	}
	catch (const std::exception& e)
	{
		std::wstring error = L"[-] Exception in CheckDLLArchitecture: " + std::wstring(e.what(), e.what() + strlen(e.what()));
		LogErrorAndStatus(ctx, error, RGB(255, 0, 0), true);
		return false;
	}
}


bool Conditionals::ValidatePEHeaders(InjectorContext& ctx, const BYTE* pSourceData, SIZE_T fileSize, std::wstring& errorMsg)
{
	try
	{
		if (!pSourceData || fileSize < sizeof(IMAGE_DOS_HEADER))
		{
			errorMsg = L"[-] Invalid source data size";
			return false;
		}
		IMAGE_DOS_HEADER* pDosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(const_cast<BYTE*>(pSourceData));
		if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
		{
			errorMsg = L"[-] Invalid file (no MZ signature)";
			return false;
		}
		if (pDosHeader->e_lfanew < 0 || static_cast<SIZE_T>(pDosHeader->e_lfanew) + sizeof(IMAGE_NT_HEADERS) > fileSize)
		{
			errorMsg = L"[-] Invalid NT headers offset";
			return false;
		}
		IMAGE_NT_HEADERS* pNtHeaders = reinterpret_cast<IMAGE_NT_HEADERS*>(const_cast<BYTE*>(pSourceData + pDosHeader->e_lfanew));
		if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE)
		{
			errorMsg = L"[-] Invalid NT signature";
			return false;
		}

		if (pNtHeaders->FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64)
		{
			errorMsg = L"[-] Invalid file architecture";
			return false;
		}
		errorMsg = L"[+] Valid PE file detected";
		return true;
	}
	catch (const std::exception& e)
	{
		errorMsg = L"[-] Exception in ValidatePEHeaders: " + std::wstring(e.what(), e.what() + strlen(e.what()));
		return false;
	}
}
