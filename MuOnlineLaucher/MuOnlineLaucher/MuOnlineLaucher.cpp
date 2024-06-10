// MuOnlinelaucher.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "MuOnlinelaucher.h"
#include "LauncherWindow.h"

#define MAX_LOADSTRING 100

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LauncherWindow launcherWindow(hInstance, nCmdShow);

    MSG msg;
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MUONLINELAUCHER));

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}
