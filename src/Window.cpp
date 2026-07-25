#include "Window.h"

OSDWindow::~OSDWindow()
{
    if(hwnd)
        DestroyWindow(hwnd);

    if(blackBrush)
        DeleteObject(blackBrush);

    if(whiteBrush)
        DeleteObject(whiteBrush);
}

constexpr wchar_t CLASS_NAME[] = L"MicroRVXWindow";
constexpr UINT OSD_TIMEOUT = 1000;
constexpr int width = 300;
constexpr int height = 24;

static OSDWindow* self = nullptr;

bool OSDWindow::Create(HINSTANCE instance)
{
    self = this;

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = instance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = nullptr;

    RegisterClass(&wc);

    int x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
    int y = GetSystemMetrics(SM_CYSCREEN) - 100;

    hwnd = CreateWindowEx(

        WS_EX_TOPMOST |
        WS_EX_TOOLWINDOW |
        WS_EX_NOACTIVATE,

        CLASS_NAME,

        L"",

        WS_POPUP,

        x,
        y,
        width,
        height,

        nullptr,
        nullptr,
        instance,
        nullptr);

		blackBrush = CreateSolidBrush(
		RGB(0,0,0));

		whiteBrush = CreateSolidBrush(
		RGB(255,255,255));

    return hwnd != nullptr;
}

void OSDWindow::Show(int percent)
{
    volume = percent;

    InvalidateRect(
        hwnd,
        nullptr,
        FALSE);


    SetWindowPos(
        hwnd,
        HWND_TOPMOST,
        0,
        0,
        0,
        0,
        SWP_NOMOVE |
        SWP_NOSIZE |
        SWP_NOACTIVATE |
        SWP_SHOWWINDOW);


    KillTimer(
        hwnd,
        1);


    SetTimer(
        hwnd,
        1,
        OSD_TIMEOUT,
        nullptr);
}

void OSDWindow::Paint(HDC hdc)
{
    RECT rc = {0,0,300,24};

    FillRect(
    hdc,
    &rc,
    blackBrush);

    RECT bar = rc;

    bar.right =
        (rc.right * volume) / 100;

    FillRect(
    hdc,
    &bar,
    whiteBrush);
	
	wchar_t text[8];

wsprintf(
    text,
    L"%d%%",
    volume);


SetBkMode(
    hdc,
    TRANSPARENT);


SetTextColor(
    hdc,
    RGB(255,255,255));


TextOut(
    hdc,
    135,
    3,
    text,
    lstrlen(text));
	
}

LRESULT CALLBACK OSDWindow::WindowProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch(msg)
    {
    case WM_TIMER:

    KillTimer(hwnd,1);

    ShowWindow(hwnd, SW_HIDE);

    return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;

        HDC hdc = BeginPaint(hwnd,&ps);

        self->Paint(hdc);

        EndPaint(hwnd,&ps);

        return 0;
    }

    }

    return DefWindowProc(
        hwnd,
        msg,
        wParam,
        lParam);
}