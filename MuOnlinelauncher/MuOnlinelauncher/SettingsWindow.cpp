#include "pch.h"
#include "SettingsWindow.h"
#include <tchar.h>

SettingsWindow::SettingsWindow(HINSTANCE hInstance, CImages* cImages)
  : m_hInstance(hInstance), m_hWnd(nullptr), m_images(cImages), m_backgroundImage(nullptr) {
}

bool SettingsWindow::Create() {

  if (m_hWnd) {
    Show();
    return true;
  }

  WNDCLASSEX wc = {
      sizeof(WNDCLASSEX),
      CS_HREDRAW | CS_VREDRAW,
      SettingsWindow::WndProc,
      0,
      0,
      m_hInstance,
      LoadIcon(nullptr, IDI_APPLICATION),
      LoadCursor(nullptr, IDC_ARROW),
      nullptr,
      nullptr,
      _T("SettingsWindowClass"),
      LoadIcon(nullptr, IDI_APPLICATION)
  };

  if (!RegisterClassEx(&wc)) {
    MessageBox(nullptr, _T("Failed to register window class"), _T("Error"), MB_OK);
    return false;
  }

  m_backgroundImage = m_images->getImage(SETTINGS_IMAGE_BACKGROUND); // Defina uma imagem de fundo apropriada
  int width = 0;
  int height = 0;
  if (m_backgroundImage) {
    width = m_backgroundImage->GetWidth();
    height = m_backgroundImage->GetHeight();
  }

  int xPos = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
  int yPos = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

  m_hWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOOLWINDOW, _T("SettingsWindowClass"), nullptr, WS_POPUP,
    xPos, yPos, width, height, nullptr, nullptr, m_hInstance, this);

  if (!m_hWnd) {
    MessageBox(nullptr, _T("Failed to create window"), _T("Error"), MB_OK);
    return false;
  }

  // Configurar a janela como transparente
  UpdateLayeredWindow();

  ShowWindow(m_hWnd, SW_SHOW);
  SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
  UpdateWindow(m_hWnd);
  return true;
}

HWND SettingsWindow::GetHWnd() const {
  return m_hWnd;
}

void SettingsWindow::Hide() {
  ShowWindow(m_hWnd, SW_HIDE);
}

void SettingsWindow::Show() {
  ShowWindow(m_hWnd, SW_SHOW);
  SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void SettingsWindow::DrawBackground(Gdiplus::Graphics& graphics) {
  if (m_backgroundImage) {
    graphics.DrawImage(m_backgroundImage, 0, 0, m_backgroundImage->GetWidth(), m_backgroundImage->GetHeight());
  }
}

void SettingsWindow::UpdateLayeredWindow() {
  if (!m_backgroundImage) return;

  HDC hdcScreen = GetDC(NULL);
  HDC hdcMem = CreateCompatibleDC(hdcScreen);

  int width = m_backgroundImage->GetWidth();
  int height = m_backgroundImage->GetHeight();

  HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
  SelectObject(hdcMem, hBitmap);

  Gdiplus::Graphics graphics(hdcMem);
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

LRESULT CALLBACK SettingsWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  SettingsWindow* pThis = nullptr;

  if (message == WM_NCCREATE) {
    CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
    pThis = (SettingsWindow*)pCreate->lpCreateParams;
    SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);
    pThis->m_hWnd = hWnd;
  }
  else {
    pThis = (SettingsWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
  }

  if (pThis) {
    return pThis->HandleMessage(message, wParam, lParam);
  }
  else {
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
}

LRESULT SettingsWindow::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam) {
  switch (message) {
  case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(m_hWnd, &ps);
    Gdiplus::Graphics graphics(hdc);
    DrawBackground(graphics);
    EndPaint(m_hWnd, &ps);
    break;
  }
  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE) {
      Hide();
      return 0;
    }
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;

  default:
    return DefWindowProc(m_hWnd, message, wParam, lParam);
  }

  return 0;
}
