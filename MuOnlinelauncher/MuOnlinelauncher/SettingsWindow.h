#pragma once

#include <windows.h>
#include <gdiplus.h>
#include "Images.h"

class SettingsWindow {
public:
  SettingsWindow(HINSTANCE hInstance, CImages* cImages);
  bool Create();
  HWND GetHWnd() const;
  void Hide();
  void Show();

private:
  static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
  LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam);
  void DrawBackground(Gdiplus::Graphics& graphics);
  void UpdateLayeredWindow();

  HINSTANCE m_hInstance;
  HWND m_hWnd;
  CImages* m_images;
  Gdiplus::Image* m_backgroundImage;
};
