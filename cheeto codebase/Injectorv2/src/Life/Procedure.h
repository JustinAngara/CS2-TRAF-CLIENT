#pragma once
#include "InjectorContext.h"
#include "../Global/HandleGuard.h"
#include <windows.h>



namespace UIHelpers
{
	HBITMAP LoadBitmapResource(InjectorContext& ctx);

	void CreateFonts(HFONT& hFontTitle, HFONT& hFontButton, HFONT& hFontStatus, HFONT& hFontFooter);

	void CreateImageControl(HWND hwnd, HBITMAP hBitmap);
	void CreateTitleControl(HWND hwnd, HFONT hFontTitle);
	void CreateProcessControls(InjectorContext& ctx, HWND hwnd, HFONT hFontStatus, HFONT hFontButton);

	void CreateDLLControls(InjectorContext& ctx, HWND hwnd, HFONT hFontStatus, HFONT hFontButton);
	void CreateProgressBar(InjectorContext& ctx, HWND hwnd);
	void CreateStatusControl(InjectorContext& ctx, HWND hwnd, HFONT hFontStatus);

	void CreateFooterControl(HWND hwnd, HFONT hFontFooter);
	void PopulateProcessList(InjectorContext& ctx);
	void RestoreLastSelection(InjectorContext& ctx);

}

namespace CommandHandlers
{
	std::vector<BYTE> LoadDLLData(InjectorContext& ctx);
	DWORD FindTargetProcess(InjectorContext& ctx);
	HANDLE OpenTargetProcess(InjectorContext& ctx, DWORD pid);

	void HandleRefresh(InjectorContext& ctx);
	void HandleProcessSelection(InjectorContext& ctx);
	void HandleBrowseDLL(InjectorContext& ctx, HWND hwnd);

	bool ConfirmInjection(InjectorContext& ctx, HWND hwnd);
	void EnableDebugPrivileges(InjectorContext& ctx);

	bool ValidateInjectionPreconditions(InjectorContext& ctx, HWND hwnd);
	bool VerifyArchitecture(InjectorContext& ctx, const HandleGuard& hProcess);

}
