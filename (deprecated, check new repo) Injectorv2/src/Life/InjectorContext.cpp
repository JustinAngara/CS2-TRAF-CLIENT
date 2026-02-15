
#include "../Life/InjectorContext.h"
#include "../Global/Conditionals.h"
#include <string>
#include <Shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

bool InjectorContext::ValidateDLLPath(const std::wstring& dllPath)
{
	if (dllPath.length() > 260)
	{
		Conditionals::LogErrorAndStatus(*this, L"[-] DLL path too long", RGB(255, 0, 0), true);
		return false;
	}
	if (dllPath.find(L"..\\") != std::wstring::npos || dllPath.find(L"/") != std::wstring::npos || dllPath.find(L"\\") == 0)
	{
		Conditionals::LogErrorAndStatus(*this, L"[-] Invalid characters in DLL path", RGB(255, 0, 0), true);
		return false;
	}
	if (PathFileExistsW(dllPath.c_str()) == FALSE)
	{
		Conditionals::LogErrorAndStatus(*this, L"[-] DLL file does not exist", RGB(255, 0, 0), true);
		return false;
	}
	return true;
}
