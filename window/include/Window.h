#pragma once

#include <windows.h>

#include <functional>

namespace ttt
{
class WindowClass;

class Window
{
public:
    using OnEventCallback = std::function<LRESULT(Window* window, UINT msg, WPARAM wParam, LPARAM lParam)>;

    Window(HWND handler, WindowClass* type, OnEventCallback const& eventCallback);
    ~Window();

    HWND handler() const;
    WindowClass* type() const;

    void show();
    void destroy();

    LRESULT onEvent(UINT msg, WPARAM wParam, LPARAM lParam);
    void setEventCallback(OnEventCallback const& callback);

    bool peekMessage() const;

private:

    HWND mHandler;
    WindowClass* mType;
    OnEventCallback mOnEventCallback;
};

} // namespace ttt
