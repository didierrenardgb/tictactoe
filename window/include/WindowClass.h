#pragma once

#include <memory>
#include <windows.h>
#include <map>

#include "Window.h"
#include "WindowGeometry.h"

namespace ttt
{

class WindowClass
{
public:
    WindowClass(const char* windowClassName);
    ~WindowClass();

    std::shared_ptr<Window> createWindow(const char* windowTitle, Window::OnEventCallback const& onEventCallback = nullptr, WindowGeometry const& geometry = { 100, 100, 1280, 800 });

private:

    static LRESULT onWindowEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    std::shared_ptr<Window> findWindow(HWND byHandle) const;

    std::map<HWND, std::shared_ptr<Window>> mWindows;
    WNDCLASSEX mWindowClass;
};

} // namespace ttt
