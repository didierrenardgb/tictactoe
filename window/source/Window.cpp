#include "Window.h"

namespace ttt
{

Window::Window(HWND handler, WindowClass* type, OnEventCallback const& eventCallback):
    mHandler(handler)
    , mType(type)
    , mOnEventCallback(eventCallback)
{
}

Window::~Window()
{
    destroy();
}

HWND Window::handler() const
{
    return mHandler;
}

WindowClass* Window::type() const
{
    return mType;
}

void Window::show()
{
    ::ShowWindow(mHandler, SW_SHOWDEFAULT);
    ::UpdateWindow(mHandler);
}

void Window::destroy()
{
    if (mHandler)
    {
        ::DestroyWindow(mHandler);
        mHandler = nullptr;
    }
}

LRESULT Window::onEvent(UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (mOnEventCallback)
    {
        return mOnEventCallback(this, msg, wParam, lParam);
    }

    return ::DefWindowProc(mHandler, msg, wParam, lParam);
}

void Window::setEventCallback(OnEventCallback const& callback)
{
    mOnEventCallback = callback;
}

bool Window::peekMessage() const
{
    MSG msg;
    while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
        if (msg.message == WM_QUIT)
        {
            return true;
        }
    }
    return false;
}

} // namespace ttt
