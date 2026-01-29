#include "../Parse/GrabContent.h"
#include "../Parse/Parse.h"

#define UNICODE
#define NOMINMAX
#include <windows.h>

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
		return 0;

	case WM_COMMAND:
		return 0;

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
	Parse p{ R"(src\Data\client_dll.hpp)" };
	GrabContent::Run(p);
	

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
	L"JAPOO",
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
