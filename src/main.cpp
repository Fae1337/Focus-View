#include <windows.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include "cfg.h"

void GetParamets(HDC hWndDC) {
    HWND hDesktop = GetDesktopWindow();
    HDC hDesktopDC = GetDC(hDesktop);

    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);

    float srcW = WIDTH / Zoom;
    float srcH = HEIGHT / Zoom;

    float x = (screenW - srcW) / 2;
    float y = (screenH - srcH) / 2;

    SetStretchBltMode(hWndDC, HALFTONE); // сглаживание но хуйня всё равно
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
    for (auto logo : std::string{ "Focus-View" }) {
        std::cout << logo;
        Sleep(100);
    }
    std::cout << std::endl;
    std::cout << std::endl;

    system("cls");
    std::cout << "Config parameters:" << std::endl << std::endl;

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
        xposition, yposition, WIDTH, HEIGHT, // 100, 100 - позиция
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


