#include <windows.h>
#include <iostream>
#include "cfg.h"

void GetParamets(HDC hWndDC) {
    HWND hDesktop = GetDesktopWindow();
    HDC hDesktopDC = GetDC(hDesktop);

    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);

    //std::cout << "\n1111111111111\n";

    int srcW = WIDTH / Zoom;
    int srcH = HEIGHT / Zoom;

    //std::cout << screenW << " | " << screenH << std::endl;
    //std::cout << srcW << " | " << srcH << std::endl;

    int x = (screenW - srcW) / 2;
    int y = (screenH - srcH) / 2;

    //std::cout << x << " | " << y;
    
    SetStretchBltMode(hWndDC, HALFTONE); // сглаживание
    StretchBlt(hWndDC, 0, 0, WIDTH, HEIGHT, hDesktopDC, x, y, srcW, srcH, SRCCOPY);

    ReleaseDC(hDesktop, hDesktopDC);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        GetParamets(hdc);
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_TIMER:
        InvalidateRect(hwnd, NULL, FALSE);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int main() {
    std::cout << "Monitor: " << GetSystemMetrics(SM_CXSCREEN) << " / " << GetSystemMetrics(SM_CYSCREEN) << std::endl;
    std::cout << "Area size: " << WIDTH << " | " << HEIGHT << std::endl;
    std::cout << "FPS: " << FPS << std::endl;
    std::cout << "Zoom area: " << Zoom << "x" << std::endl;

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"MyCenterCaptureClass";
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
        L"MyCenterCaptureClass",
        L"Center Capture",
        WS_POPUP,
        100, 100, WIDTH, HEIGHT,
        NULL, NULL, wc.hInstance, NULL
    );
    ShowWindow(hwnd, SW_SHOW);
    SetTimer(hwnd, 1, FPS, NULL); // SetTimer(hwnd, 1, fps, NULL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
