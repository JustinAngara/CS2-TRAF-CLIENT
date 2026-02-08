#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wrl.h>
#include <string>
#include "WebView2.h"

namespace Loader
{
    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    int WINAPI Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR cmdLine, int nCmdShow);

    void OnJsMessage(const std::wstring& msg);
    std::wstring PathToFileUri(const std::wstring& path);
    std::wstring ExeDir();

    inline HWND g_hwnd = nullptr;
    inline Microsoft::WRL::ComPtr<ICoreWebView2Controller> g_controller{};
    inline Microsoft::WRL::ComPtr<ICoreWebView2> g_webview{};
}
