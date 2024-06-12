#include "pch.h"
#include "MuOnlinelauncher.h"
#include "defines.h""
#include "MainWindow.h"

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

  MainWindow mainWindow(hInstance, cImages.get());
  if (!mainWindow.Create()) {
    return -1;
  }

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
