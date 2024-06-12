#include "pch.h"
#include "LauncherWindow.h"


const RECT BUTTON_MINIMIZE_AREA = { 807, 61, 822, 75 };
const RECT BUTTON_CLOSE_AREA = { 830, 58, 849, 79 };

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#endif

#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#endif

LauncherWindow::LauncherWindow(HINSTANCE hInstance, CImages* cImages)
    : m_hInstance(hInstance), m_hWnd(nullptr), m_images(cImages)
{
}

bool LauncherWindow::Create()
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = LauncherWindow::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInstance;
    wcex.hIcon = LoadIcon(m_hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"TransparentWindowClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassExW(&wcex))
    {
        std::wcerr << L"Failed to register window class" << std::endl;
        return false;
    }

    int xPos = 0;
    int yPos = 0;
    int width = 0;
    int height = 0;
    if (this->m_images) {
        Image* pImage = this->m_images->getImage(LAUNCHER_IMAGE_BACKGROUND);

        if (pImage)
        {
            int screenWidth = GetSystemMetrics(SM_CXSCREEN);
            int screenHeight = GetSystemMetrics(SM_CYSCREEN);

            RECT rect;
            GetWindowRect(m_hWnd, &rect);
            width = pImage->GetWidth();
            height = pImage->GetHeight();


            xPos = (screenWidth - width) / 2;
            yPos = (screenHeight - height) / 2;
        }
    }

    m_hWnd = CreateWindowExW(WS_EX_LAYERED | WS_EX_TOPMOST, L"TransparentWindowClass", nullptr, WS_POPUP,
        xPos, yPos, width, height, nullptr, nullptr, m_hInstance, this);

    if (!m_hWnd)
    {
        std::wcerr << L"Failed to create window" << std::endl;
        return false;
    }

    std::wcerr << L"Window created successfully" << std::endl;

    return m_hWnd != nullptr;
}

HWND LauncherWindow::GetHWnd() const
{
    return m_hWnd;
}

LRESULT CALLBACK LauncherWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LauncherWindow* pThis = nullptr;

    if (message == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        pThis = (LauncherWindow*)pCreate->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);

        pThis->m_hWnd = hWnd;
        std::wcout << L"WM_NCCREATE called" << std::endl;
    }
    else
    {
        pThis = (LauncherWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    }

    if (pThis)
    {
        return pThis->HandleMessage(message, wParam, lParam);
    }
    else
    {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

LRESULT LauncherWindow::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    static POINTS ptsBegin;
    
    int x, y;

    switch (message)
    {
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

        if(x >= BUTTON_CLOSE_AREA.left && x <= BUTTON_CLOSE_AREA.right &&
			y >= BUTTON_CLOSE_AREA.top && y <= BUTTON_CLOSE_AREA.bottom) {
			PostQuitMessage(0);
		}


        std::wcout << L"WM_LBUTTONDOWN called" << std::endl;
        break;

    case WM_MOUSEMOVE:
        if (wParam & MK_LBUTTON)
        {
            POINTS ptsEnd = MAKEPOINTS(lParam);

            int dx = ptsEnd.x - ptsBegin.x;
            int dy = ptsEnd.y - ptsBegin.y;

            RECT rect;
            GetWindowRect(m_hWnd, &rect);

            MoveWindow(m_hWnd, rect.left + dx, rect.top + dy, rect.right - rect.left, rect.bottom - rect.top, TRUE);
        }
        break;

    case WM_LBUTTONUP:
        ReleaseCapture();
        std::wcout << L"WM_LBUTTONUP called" << std::endl;
        break;

    case WM_PAINT:
    {
        std::wcout << L"WM_PAINT called" << std::endl;
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(m_hWnd, &ps);

        Graphics graphics(hdc);

        if (this->m_images) {
            Image* pImage = this->m_images->getImage(LAUNCHER_IMAGE_BACKGROUND);

            if (pImage)
            {
                int screenWidth = GetSystemMetrics(SM_CXSCREEN);
                int screenHeight = GetSystemMetrics(SM_CYSCREEN);

                RECT rect;
                GetWindowRect(m_hWnd, &rect);
                int width = pImage->GetWidth();
                int height = pImage->GetHeight();


                int xPos = (screenWidth - width) / 2;
                int yPos = (screenHeight - height) / 2;

                MoveWindow(m_hWnd, xPos, yPos, width, height, TRUE);

                HDC hdcMem = CreateCompatibleDC(hdc);
                HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);
                SelectObject(hdcMem, hBitmap);

                // Configurar o GDI+ para desenhar com transparência
                Graphics graphicsMem(hdcMem);
                graphicsMem.DrawImage(pImage, 0, 0, width, height);

                // Transferir o conteúdo do DC da memória para o DC da janela usando UpdateLayeredWindow
                POINT ptSrc = { 0, 0 };
                SIZE sizeWnd = { width, height };
                BLENDFUNCTION blend = { 0 };
                blend.BlendOp = AC_SRC_OVER;
                blend.BlendFlags = 0;
                blend.SourceConstantAlpha = 255;
                blend.AlphaFormat = AC_SRC_ALPHA;

                POINT ptPos = { rect.left, rect.top };
                UpdateLayeredWindow(m_hWnd, hdc, &ptPos, &sizeWnd, hdcMem, &ptSrc, 0, &blend, ULW_ALPHA);

                DeleteObject(hBitmap);
                DeleteDC(hdcMem);
            }
        }

        EndPaint(m_hWnd, &ps);
    }
    break;

    case WM_ERASEBKGND:
        //InvalidateRect(m_hWnd, &BUTTON_MINIMIZE_AREA, TRUE);
        return 1;

    case WM_DESTROY:
        PostQuitMessage(0);
        std::wcerr << L"WM_DESTROY called" << std::endl;
        break;

    default:
        return DefWindowProc(m_hWnd, message, wParam, lParam);
    }

    return 0;
}

