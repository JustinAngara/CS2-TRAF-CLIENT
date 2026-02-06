#include "../Parse/GrabContent.h"
#include "../Parse/Parse.h"
#include "../JsonCovnersion/ConvertToJson.h"
#include "Populate.h"
#include <windows.h>


#define UNICODE
#define NOMINMAX
#define ID_BTN_RUN 1001

static const wchar_t* kClassName = L"CTOJS";

static void AttachDebugConsole()
{
	AllocConsole();

	FILE* f = nullptr;
	freopen_s(&f, "CONOUT$", "w", stdout);
	freopen_s(&f, "CONOUT$", "w", stderr);
	freopen_s(&f, "CONIN$", "r", stdin);

	SetConsoleOutputCP(CP_UTF8);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		RECT rc{};
		GetClientRect(hwnd, &rc);

		const int btnW = 140;
		const int btnH = 44;

		const int x = (rc.right - btnW) / 2; // middle
		const int y = (rc.bottom * 2) / 3 - btnH / 2; // 3rd down (2/3 height)

		CreateWindowExW(
		0,
		L"BUTTON",
		L"Run",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		x, y,
		btnW, btnH,
		hwnd,
		(HMENU)(INT_PTR)ID_BTN_RUN,
		(HINSTANCE)GetWindowLongPtrW(hwnd, GWLP_HINSTANCE),
		nullptr);

		return 0;
	}


	case WM_COMMAND:
	{
		if (LOWORD(wParam) == ID_BTN_RUN && HIWORD(wParam) == BN_CLICKED)
		{
			Populate::Run(R"(src\Data\client_dll.hpp)");
			Populate::Run(R"(src\Data\offsets.hpp)");
			Populate::Run(R"(src\Data\server_dll.hpp)");
			MessageBoxW(hwnd, L"Produced files", L"ctojs", MB_OK);
			return 0;
		}
		return 0;
	}


	case WM_PAINT:
	{
		PAINTSTRUCT ps{};
		HDC			hdc = BeginPaint(hwnd, &ps);

		const wchar_t* text = L"TEST";
		TextOutW(hdc, 20, 20, text, (int)lstrlenW(text));

		EndPaint(hwnd, &ps);
		return 0;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
#ifdef _DEBUG
	AttachDebugConsole();
#endif
	


	WNDCLASSEXW wc{};
	wc.cbSize		 = sizeof(wc);
	wc.style		 = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc	 = WndProc;
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursorW(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = kClassName;

	if (!RegisterClassExW(&wc))
		return 0;

	HWND hwnd = CreateWindowExW(
	0,
	kClassName,
	L"ctojs",
	WS_OVERLAPPEDWINDOW,
	CW_USEDEFAULT, CW_USEDEFAULT,
	900, 600,
	nullptr,
	nullptr,
	hInstance,
	nullptr);

	if (!hwnd)
		return 0;

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg{};
	while (GetMessageW(&msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return (int)msg.wParam;
}
