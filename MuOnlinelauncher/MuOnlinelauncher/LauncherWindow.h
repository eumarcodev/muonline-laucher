#ifndef LAUNCHERWINDOW_H
#define LAUNCHERWINDOW_H

#include "framework.h"
#include "Images.h"

class LauncherWindow
{
public:
    LauncherWindow(HINSTANCE hInstance, CImages* cImages);
    bool Create();
    HWND GetHWnd() const;

private:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam);

    HINSTANCE m_hInstance;
    HWND m_hWnd;
    CImages* m_images;
};

#endif // LAUNCHERWINDOW_H