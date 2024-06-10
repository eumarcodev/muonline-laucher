#pragma once

#include "defines.h" 

class LauncherWindow {
public:
    LauncherWindow(HINSTANCE hInstance, int nCmdShow);
    ~LauncherWindow();
    
    BOOL InitInstance();
    ATOM MyRegisterClass();
    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    HINSTANCE hInst;                                // current instance
    WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
    WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK StaticAbout(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
    INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};
