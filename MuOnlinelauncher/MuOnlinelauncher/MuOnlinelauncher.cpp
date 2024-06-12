#include "pch.h"
#include "MuOnlinelauncher.h"
#include "LauncherWindow.h"
#include "defines.h"

ULONG_PTR gdiplusToken;

void InitializeConsole()
{
    AllocConsole();
    FILE* file;
    freopen_s(&file, "CONOUT$", "w", stdout);
    freopen_s(&file, "CONOUT$", "w", stderr);
    freopen_s(&file, "CONIN$", "r", stdin);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{


    if (SHOW_CONSOLE)
        InitializeConsole();


    InitializeGDIPlus();

    std::unique_ptr<CImages> cImages = std::make_unique<CImages>(hInstance);
    cImages->loadImages();

    LauncherWindow launcherWindow(hInstance, cImages.get());
    if (!launcherWindow.Create())
    {
        ShutdownGDIPlus();
        return FALSE;
    }

    // Mostre a janela e atualize
    ShowWindow(launcherWindow.GetHWnd(), nCmdShow);
    UpdateWindow(launcherWindow.GetHWnd());

    // Forçar a repintura da janela
    InvalidateRect(launcherWindow.GetHWnd(), NULL, TRUE);
    UpdateWindow(launcherWindow.GetHWnd());

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    ShutdownGDIPlus();
    return (int)msg.wParam;
}

void InitializeGDIPlus()
{
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
}

void ShutdownGDIPlus()
{
    GdiplusShutdown(gdiplusToken);
}
