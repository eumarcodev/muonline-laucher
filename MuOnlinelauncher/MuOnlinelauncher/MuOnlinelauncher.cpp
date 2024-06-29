#include "pch.h"
#include "MuOnlinelauncher.h"
#include "defines.h""
#include "MainWindow.h"
#include "RegEdit.h"

ULONG_PTR gdiplusToken;

void InitializeConsole() {
  AllocConsole();
  FILE* file;
  freopen_s(&file, "CONOUT$", "w", stdout);
  freopen_s(&file, "CONOUT$", "w", stderr);
  freopen_s(&file, "CONIN$", "r", stdin);

  // Adicionar suporte para std::wcout
  std::wcout.clear();
  std::wclog.clear();
  std::wcerr.clear();
  std::wcin.clear();

  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  HANDLE hErr = GetStdHandle(STD_ERROR_HANDLE);
  HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

  SetConsoleMode(hOut, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);
  SetConsoleMode(hIn, ENABLE_EXTENDED_FLAGS | ENABLE_INSERT_MODE | ENABLE_QUICK_EDIT_MODE);

  std::wcout << L"Console initialized" << std::endl;
}

std::unique_ptr<RegEdit> registryReader = std::make_unique<RegEdit>();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPWSTR    lpCmdLine,
  _In_ int       nCmdShow)
{

  HANDLE hMutex = CreateMutex(
    NULL, // Indica que o mutex não tem atributos de segurança inicial. 
    FALSE, // O Mutex não está inicialmente Sinalizado, ou seja, o mutex não está ocupado.
    _T("MuOnlineLauncherMutex") // o nome do Mutex, OBS IMPORTE: esse nome tem que ser unico para o aplicativo.
  );

  if (GetLastError() == ERROR_ALREADY_EXISTS) {
    MessageBox(nullptr, _T("MuOnlineLauncher is already running"), _T("Error"), MB_OK);
    return 0;
  }

  if (SHOW_CONSOLE)
    InitializeConsole();


  InitializeGDIPlus();

  //Leitura dos registros do windows chave: Software\\Webzen\\Mu\\Config
  registryReader->Load();

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
