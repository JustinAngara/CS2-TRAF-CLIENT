#pragma once
#include <windows.h>

#include <string>
#include <vector>
#include <random>


class InjectorContext
{
public:
	HWND hwndMain		   = nullptr;
	HWND hwndStatus		   = nullptr;
	HWND hwndBrowseButton  = nullptr;
	HWND hwndInjectButton  = nullptr;
	HWND hwndProgressBar   = nullptr;
	HWND hwndProcessCombo  = nullptr;
	HWND hwndRefreshButton = nullptr;
	HWND hwndProcessLabel  = nullptr;
	HWND hwndDllLabel	   = nullptr;

	std::wstring			  dllPath;
	std::vector<std::wstring> logBuffer;
	bool					  enableLogging = false;

	std::wstring processName;
	const char*	 ntdllName;
	const char	 kXorKey;

	InjectorContext() :
		processName(L""),
		ntdllName("ntdll.dll"),
		kXorKey(GenerateXorKey())
	{
		InitializeLogging();
	}

	bool ValidateDLLPath(const std::wstring& dllPath);

private:
	static char GenerateXorKey()
	{
		std::random_device				   rd;
		std::mt19937					   gen(rd());
		std::uniform_int_distribution<int> dis(1, 255);
		return static_cast<char>(dis(gen));
	}

	void InitializeLogging() { }
};
