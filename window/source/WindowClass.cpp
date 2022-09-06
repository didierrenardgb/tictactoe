#include "WindowClass.h"

#include <WinUser.h>
#include <tchar.h>

namespace ttt
{
WindowClass::WindowClass(const char* windowClassName)
{
    mWindowClass = { sizeof(WNDCLASSEX), CS_CLASSDC, onWindowEvent, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T(windowClassName), NULL };
    ::RegisterClassEx(&mWindowClass);
}

WindowClass::~WindowClass()
{
    ::UnregisterClass(mWindowClass.lpszClassName, mWindowClass.hInstance);
}

std::shared_ptr<Window> WindowClass::createWindow(const char* windowTitle, Window::OnEventCallback const& onEventCallback, WindowGeometry const& geometry)
{
    auto w = std::make_shared<Window>(::CreateWindow(mWindowClass.lpszClassName, _T(windowTitle), WS_OVERLAPPEDWINDOW, geometry.x, geometry.y, geometry.width, geometry.height, NULL, NULL, mWindowClass.hInstance, this), this, onEventCallback);
    mWindows[w->handler()] = w;
    return w;
}

LRESULT WindowClass::onWindowEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams));
        break;
    }

    auto _this = reinterpret_cast<WindowClass*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (_this != nullptr)
    {
        auto window = _this->findWindow(hWnd);
        if (window)
        {
            return window->onEvent(msg, wParam, lParam);
        }
    }

    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

std::shared_ptr<Window> WindowClass::findWindow(HWND byHandle) const
{
    auto iter = mWindows.find(byHandle);
    if (iter != mWindows.end())
    {
        return iter->second;
    }
    return nullptr;
}
} // namespace ttt
