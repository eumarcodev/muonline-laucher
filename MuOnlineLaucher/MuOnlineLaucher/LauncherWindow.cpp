#include "pch.h"
#include "LauncherWindow.h"
#include "Resource.h"

LauncherWindow::LauncherWindow(HINSTANCE hInstance, int nCmdShow) : hInst(hInstance) {
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MUONLINELAUCHER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass();

    InitInstance();
}

LauncherWindow::~LauncherWindow() {}

BOOL LauncherWindow::InitInstance() {
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInst, this);

    if (!hWnd) {
        return FALSE;
    }

    ShowWindow(hWnd, SW_SHOWNORMAL);
    UpdateWindow(hWnd);

    return TRUE;
}

ATOM LauncherWindow::MyRegisterClass() {
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = StaticWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInst;
    wcex.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_MUONLINELAUCHER));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MUONLINELAUCHER);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

LRESULT CALLBACK LauncherWindow::StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    LauncherWindow* pThis = nullptr;

    if (message == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<LauncherWindow*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);
    }
    else {
        pThis = reinterpret_cast<LauncherWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (pThis) {
        return pThis->WndProc(hWnd, message, wParam, lParam);
    }
    else {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

LRESULT CALLBACK LauncherWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId) {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, StaticAbout);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    } break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    } break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK LauncherWindow::StaticAbout(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    LauncherWindow* pThis = reinterpret_cast<LauncherWindow*>(GetWindowLongPtr(GetParent(hDlg), GWLP_USERDATA));
    if (pThis) {
        return pThis->About(hDlg, message, wParam, lParam);
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK LauncherWindow::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}