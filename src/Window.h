#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>

class OSDWindow
{
public:
	~OSDWindow();
    bool Create(HINSTANCE instance);
    void Show(int percent);

private:
    static LRESULT CALLBACK WindowProc(
        HWND,
        UINT,
        WPARAM,
        LPARAM);
    void Paint(HDC hdc);
    HWND hwnd = nullptr;
    int volume = 50;
	HBRUSH blackBrush = nullptr;
    HBRUSH whiteBrush = nullptr;
};