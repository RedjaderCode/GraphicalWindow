#include <cstdio>
#include <windows.h>
#include <cstdint>

#ifndef UNICODE
#error PLEASE FLAG -DUNICODE
#endif

//https://learn.microsoft.com/en-us/windows/win32/learnwin32/initializing-the-com-library

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wp, LPARAM lp)
{
    switch(uMsg)
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
        }
        
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

            EndPaint(hwnd, &ps);
        }
    }

    return DefWindowProc(hwnd, uMsg, wp, lp);
}

interface IDrawable
{
    void Draw();
};

struct keys
{BOOL pressed = false;
}m_key[0xFF];

struct window;

namespace COM
{
    void CreateCOMContext()
    {
        CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    }
};

void UpdateEvents()
{

}

void* _UpdateInputLoop(BOOL running = false)
{
    short NewKey[0xFF] = { NULL };
    short OldKey[0xFF] = { NULL };

    void* _DataInputLastRecord = nullptr;

    MSG msg = { NULL };

    while(running)
    {
        for(uint16_t i=0; i<=0xFF; ++i)
        {
            OldKey[ i ] = GetAsyncKeyState( i );
            if(OldKey[ i ]!=NewKey[ i ])
            {
                m_key[ i ].pressed = true;
            }
            else
            {
                m_key[ i ].pressed = false;
            }
            NewKey[ i ]          = OldKey[ i ];
            _DataInputLastRecord = (void*)OldKey[ i ];
        }

        // windows message qeue interpretation
        while(GetMessage(&msg, NULL, 0, 0) != 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        (void)UpdateEvents();

        if(msg.message = WM_CLOSE)
        {
            running = false;
        }
    }

    return _DataInputLastRecord;
}

namespace Physics
{
    void jump(uint16_t i);
};

struct window
{
public:
    void Initialize(const wchar_t* CLASS_NAME = nullptr, HINSTANCE hInstance = NULL)
    {
        WNDCLASS WinClass = { NULL };

        WinClass.lpfnWndProc = WindowProc;
        WinClass.hInstance = hInstance;
        WinClass.lpszClassName = CLASS_NAME;

        ::RegisterClass(&WinClass);

        _handler = CreateWindowEx(
            0,
            CLASS_NAME,
            L"DummyWindow",
            WS_OVERLAPPEDWINDOW,

            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            NULL, NULL, hInstance, NULL
        );

        if(_handler==nullptr)
        {
            //error handling
        }

        ::ShowWindow((HWND)_handler, SW_SHOW);
        ::UpdateWindow((HWND)_handler);

    }
    ~window()
    {
        _handler = nullptr;
    }
protected:
    void* _handler = nullptr;
};

INT WINAPI WinMain(HINSTANCE CurrInst, HINSTANCE PrevInst, LPSTR msg, INT sMSG)
{
    window win;
    (void)win.Initialize(L"DUMMY_CLASS", CurrInst);

    (void*)_UpdateInputLoop(true);

    return MessageBoxW(NULL, L"WINDOW CLOSING", L"ERROR!", 0x00000000L);
}