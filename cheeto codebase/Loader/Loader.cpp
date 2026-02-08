#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wrl.h>
#include <string>
#include <Shlwapi.h>
#include "WebView2.h"

#pragma comment(lib, "Shlwapi.lib")

using Microsoft::WRL::ComPtr;

static HWND							   g_hwnd{};
static ComPtr<ICoreWebView2Controller> g_controller;
static ComPtr<ICoreWebView2>		   g_webview;

static std::wstring ExeDir()
{
	wchar_t path[MAX_PATH]{};
	GetModuleFileNameW(nullptr, path, MAX_PATH);
	std::wstring p(path);
	auto		 pos = p.find_last_of(L"\\/");
	return (pos == std::wstring::npos) ? L"." : p.substr(0, pos);
}

static std::wstring PathToFileUri(const std::wstring& path)
{
	wchar_t uri[2048]{};
	DWORD	len = (DWORD)_countof(uri);
	HRESULT hr	= UrlCreateFromPathW(path.c_str(), uri, &len, 0);
	return FAILED(hr) ? L"" : std::wstring(uri);
}

static void OnJsMessage(const std::wstring& msg)
{
	if (msg == L"clicked")
	{
		MessageBoxW(g_hwnd, L"Injection Started", L"Status", MB_OK);
	}
	else if (msg == L"minimize")
	{
		ShowWindow(g_hwnd, SW_MINIMIZE);
	}
	else if (msg == L"close")
	{
		PostQuitMessage(0);
	}
	else if (msg == L"drag")
	{
		ReleaseCapture();
		PostMessage(g_hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
	}
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
		if (g_controller)
		{
			RECT r{};
			GetClientRect(hwnd, &r);
			g_controller->put_Bounds(r);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int nCmdShow)
{
	WNDCLASSW wc{};
	wc.lpfnWndProc	 = WndProc;
	wc.hInstance	 = hInstance;
	wc.lpszClassName = L"Loader";
	wc.hCursor		 = LoadCursor(nullptr, IDC_ARROW);
	RegisterClassW(&wc);

	g_hwnd = CreateWindowExW(0, wc.lpszClassName, L"Loader", WS_POPUP | WS_VISIBLE,
	CW_USEDEFAULT, CW_USEDEFAULT, 400, 250, nullptr, nullptr, hInstance, nullptr);

	if (!g_hwnd) return 0;

	ShowWindow(g_hwnd, nCmdShow);
	UpdateWindow(g_hwnd);

	CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
	Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
	[](HRESULT hr, ICoreWebView2Environment* env) -> HRESULT
	{
		if (FAILED(hr) || !env) return hr;
		return env->CreateCoreWebView2Controller(g_hwnd,
		Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
		[](HRESULT hr2, ICoreWebView2Controller* controller) -> HRESULT
		{
			if (FAILED(hr2) || !controller) return hr2;
			g_controller = controller;
			g_controller->get_CoreWebView2(&g_webview);

			RECT r{};
			GetClientRect(g_hwnd, &r);
			g_controller->put_Bounds(r);

			EventRegistrationToken tok{};
			g_webview->add_WebMessageReceived(
			Microsoft::WRL::Callback<ICoreWebView2WebMessageReceivedEventHandler>(
			[](ICoreWebView2*, ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT
			{
				PWSTR s = nullptr;
				if (SUCCEEDED(args->TryGetWebMessageAsString(&s)))
				{
					OnJsMessage(s ? s : L"");
					CoTaskMemFree(s);
				}
				return S_OK;
			})
			.Get(),
			&tok);

			g_webview->Navigate(PathToFileUri(ExeDir() + L"\\Loader.html").c_str());
			return S_OK;
		})
		.Get());
	})
	.Get());

	MSG m{};
	while (GetMessageW(&m, nullptr, 0, 0))
	{
		TranslateMessage(&m);
		DispatchMessageW(&m);
	}
	return 0;
}
