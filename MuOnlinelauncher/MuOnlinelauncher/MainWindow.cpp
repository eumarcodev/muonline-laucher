#include "pch.h"
#include "MainWindow.h"
#include "defines.h"


#define IDC_COMBOBOX 101

const RECT BUTTON_MINIMIZE_AREA = { 807, 61, 822, 75 };
const RECT BUTTON_CLOSE_AREA = { 830, 58, 849, 79 };
const RECT BUTTON_SETTINGS_AREA = { 40, 61, 72, 92 };
const RECT BUTTON_PLAY_AREA = { 177, 386, 260, 470 };

MainWindow::MainWindow(HINSTANCE hInstance, CImages* cImages)
  : m_hInstance(hInstance), m_hWnd(nullptr), m_images(cImages), m_backgroundImage(nullptr), m_settingsWindow(hInstance, cImages) {
}

bool MainWindow::Create() {
  WNDCLASSEX wc = {
      sizeof(WNDCLASSEX),
      CS_HREDRAW | CS_VREDRAW,
      MainWindow::WndProc,
      0,
      0,
      m_hInstance,
      LoadIcon(nullptr, IDI_APPLICATION),
      LoadCursor(nullptr, IDC_ARROW),
      nullptr,
      nullptr,
      _T("MainWindowClass"),
      LoadIcon(nullptr, IDI_APPLICATION)
  };

  if (!RegisterClassEx(&wc)) {
    MessageBox(nullptr, _T("Failed to register window class"), _T("Error"), MB_OK);
    return false;
  }

  m_backgroundImage = m_images->getImage(LAUNCHER_IMAGE_BACKGROUND);
  int width = 0;
  int height = 0;
  if (m_backgroundImage) {
    width = m_backgroundImage->GetWidth();
    height = m_backgroundImage->GetHeight();
  }

  int xPos = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
  int yPos = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

  m_hWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOPMOST, _T("MainWindowClass"), nullptr, WS_POPUP,
    xPos, yPos, width, height, nullptr, nullptr, m_hInstance, this);

  if (!m_hWnd) {
    MessageBox(nullptr, _T("Failed to create window"), _T("Error"), MB_OK);
    return false;
  }

  // Configurar a janela como transparente
  UpdateLayeredWindow();

  ShowWindow(m_hWnd, SW_SHOW);
  UpdateWindow(m_hWnd);
  return true;
}

HWND MainWindow::GetHWnd() const {
  return m_hWnd;
}

void MainWindow::DrawBackground(Graphics& graphics) {
  if (m_backgroundImage) {
    graphics.DrawImage(m_backgroundImage, 0, 0, m_backgroundImage->GetWidth(), m_backgroundImage->GetHeight());
  }
}

void MainWindow::UpdateLayeredWindow() {
  if (!m_backgroundImage) return;

  HDC hdcScreen = GetDC(NULL);
  HDC hdcMem = CreateCompatibleDC(hdcScreen);

  int width = m_backgroundImage->GetWidth();
  int height = m_backgroundImage->GetHeight();

  HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
  SelectObject(hdcMem, hBitmap);

  Graphics graphics(hdcMem);
  DrawBackground(graphics);

  POINT ptWinPos = { 0, 0 };
  SIZE sizeWin = { width, height };
  POINT ptSrc = { 0, 0 };
  BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

  ::UpdateLayeredWindow(m_hWnd, hdcScreen, NULL, &sizeWin, hdcMem, &ptSrc, 0, &blend, ULW_ALPHA);

  DeleteObject(hBitmap);
  DeleteDC(hdcMem);
  ReleaseDC(NULL, hdcScreen);
}

LRESULT CALLBACK MainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  MainWindow* pThis = nullptr;

  if (message == WM_NCCREATE) {
    CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
    pThis = (MainWindow*)pCreate->lpCreateParams;
    SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);
    pThis->m_hWnd = hWnd;
  }
  else {
    pThis = (MainWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
  }

  if (pThis) {
    return pThis->HandleMessage(message, wParam, lParam);
  }
  else {
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
}

LRESULT MainWindow::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam) {
  static POINTS ptsBegin;

  int x, y;
  switch (message) {
  case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(m_hWnd, &ps);
    Graphics graphics(hdc);
    DrawBackground(graphics);
    EndPaint(m_hWnd, &ps);
    break;
  }
  case WM_LBUTTONDOWN:
    ptsBegin = MAKEPOINTS(lParam);
    SetCapture(m_hWnd);

    x = GET_X_LPARAM(lParam);
    y = GET_Y_LPARAM(lParam);

    std::cout << "X: " << x << " Y: " << y << std::endl;

    if (x >= BUTTON_MINIMIZE_AREA.left && x <= BUTTON_MINIMIZE_AREA.right &&
      y >= BUTTON_MINIMIZE_AREA.top && y <= BUTTON_MINIMIZE_AREA.bottom) {
      ShowWindow(m_hWnd, SW_MINIMIZE); // Minimize a janela
    }

    if (x >= BUTTON_CLOSE_AREA.left && x <= BUTTON_CLOSE_AREA.right &&
      y >= BUTTON_CLOSE_AREA.top && y <= BUTTON_CLOSE_AREA.bottom) {
      PostQuitMessage(0);
    }

    if(x >= BUTTON_SETTINGS_AREA.left && x <= BUTTON_SETTINGS_AREA.right &&
      y >= BUTTON_SETTINGS_AREA.top && y <= BUTTON_SETTINGS_AREA.bottom) {
      m_settingsWindow.Create();
    }

    if (x >= BUTTON_PLAY_AREA.left && x <= BUTTON_PLAY_AREA.right &&
      y >= BUTTON_PLAY_AREA.top && y <= BUTTON_PLAY_AREA.bottom) {
      
      wchar_t buffer[MAX_PATH];
      GetModuleFileName(NULL, buffer, MAX_PATH);
      std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
      std::wstring exePath = std::wstring(buffer).substr(0, pos) + L"\\main.exe";

      //Estruturas necessárias para CreateProcess
      STARTUPINFO si = { sizeof(si) };
      PROCESS_INFORMATION pi;

      // Inicialzar as estruturas
      ZeroMemory(&si, sizeof(si));

      // Criar o processo
      if (!CreateProcess(
        exePath.c_str(), // Caminho do executável
        NULL, // Argumentos
        NULL, // Atributos de segurança do processo
        NULL, // Atributos de segurança do thread
        FALSE, // Não herdar handle
        0, // Nenhuma flag
        NULL, // Variaveis de ambiente
        NULL, // Diretório de trabalho padrão
        &si, // Informações de inicialização
        &pi // Informações do processo
      )) {

        DWORD error = GetLastError();

        MessageBox(NULL, L"Falha ao iniciar o Main.exe", L"Error", MB_OK | MB_ICONERROR);
      }

      PostQuitMessage(0);
    }

    std::wcout << L"WM_LBUTTONDOWN called" << std::endl;
    break;

  case WM_MOUSEMOVE:
    if (wParam & MK_LBUTTON) {
      POINTS ptsEnd = MAKEPOINTS(lParam);

      int dx = ptsEnd.x - ptsBegin.x;
      int dy = ptsEnd.y - ptsBegin.y;

      RECT rect;
      GetWindowRect(m_hWnd, &rect);

      MoveWindow(m_hWnd, rect.left + dx, rect.top + dy, rect.right - rect.left, rect.bottom - rect.top, TRUE);
      InvalidateRect(m_hWnd, NULL, TRUE);
    }
    break;

  case WM_SYSCOMMAND:
    if ((wParam & 0xFFF0) == SC_MINIMIZE || (wParam & 0xFFF0) == SC_RESTORE) {
      UpdateLayeredWindow();
    }
    return DefWindowProc(m_hWnd, message, wParam, lParam);

  case WM_WINDOWPOSCHANGED:
    UpdateLayeredWindow();
    return DefWindowProc(m_hWnd, message, wParam, lParam);

  case WM_LBUTTONUP:
    ReleaseCapture();
    std::wcout << L"WM_LBUTTONUP called" << std::endl;
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    break;

  default:
    return DefWindowProc(m_hWnd, message, wParam, lParam);
  }

  return 0;
}

