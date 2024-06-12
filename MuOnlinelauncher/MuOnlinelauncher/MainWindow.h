#pragma once

#include "Images.h"
#include "SettingsWindow.h"

class MainWindow {
public:
    MainWindow(HINSTANCE hInstance, CImages* cImages);
    bool Create();
    HWND GetHWnd() const;

    void DrawBackground(Graphics& graphics);

private:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam);

    void UpdateLayeredWindow();

    HINSTANCE m_hInstance;
    HWND m_hWnd;
    CImages* m_images;
    Image* m_backgroundImage;
    SettingsWindow m_settingsWindow;
};
