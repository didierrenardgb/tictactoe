#include "ImGuiWindow.h"

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

#include <windows.h>

#include "WindowClass.h"
#include "Window.h"
#include "DirectX.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace ttt
{
ImGuiWindow::ImGuiWindow(std::string const& windowTitle, WindowGeometry const& geometry):
    mWindowTitle(windowTitle)
    , mInitialGeometry(geometry)
    , mWindowClassName(windowTitle + " Window Class")
{
}

ImGuiWindow::~ImGuiWindow() = default;

bool ImGuiWindow::start(DrawFunction const& drawFunction, UpdateFunction const& updateFunction)
{
    mDrawFunction = drawFunction;
    mUpdateFunction = updateFunction;
    mWindowClass = std::make_unique<ttt::WindowClass>(mWindowClassName.c_str());
    mDirectX = std::make_unique<DirectX>();
    mWindow = mWindowClass->createWindow(mWindowTitle.c_str(), [this](ttt::Window* w, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
        {
            return onWindowEvent(w, msg, wParam, lParam);
        }, mInitialGeometry);

    if (!mDirectX->setup(mWindow))
    {
        mWindow.reset();
        mWindowClass.release();
        return false;
    }

    mWindow->show();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(mWindow->handler());
    ImGui_ImplDX11_Init(mDirectX->device().get(), mDirectX->context().get());

    return true;
}

void ImGuiWindow::mainLoop(ImVec4 const& clearColor)
{
    ImVec4 myClearColor(clearColor);

    while (true)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        if (checkEventLoop())
        {
            break;
        }

        if (mUpdateFunction)
        {
            mUpdateFunction();
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        if (mDrawFunction)
        {
            mDrawFunction(myClearColor);
        }

        render(myClearColor);
    }
}

void ImGuiWindow::end()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    mDirectX->cleanup();
    mWindow.reset();
    mWindowClass.release();
}

bool ImGuiWindow::checkEventLoop()
{
    return mWindow->peekMessage();
}

std::unique_ptr<ttt::DirectX> const& ImGuiWindow::directX() const
{
    return mDirectX;
}

void ImGuiWindow::render(ImVec4 const& clearColor)
{
    ImGui::Render();
    const float clear_color_with_alpha[4] = { clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w };
    auto renderTarget = mDirectX->renderTarget().get();
    mDirectX->context()->OMSetRenderTargets(1, &renderTarget, NULL);
    mDirectX->context()->ClearRenderTargetView(renderTarget, clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    mDirectX->swapChain()->Present(1, 0); // Present with vsync
}

LRESULT ImGuiWindow::onWindowEvent(ttt::Window* w, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(w->handler(), msg, wParam, lParam))
    {
        return true;
    }

    switch (msg)
    {
        case WM_SIZE:
            if (mDirectX->device() != nullptr && wParam != SIZE_MINIMIZED)
            {
                mDirectX->cleanupRenderTarget();
                mDirectX->swapChain()->ResizeBuffers(0, static_cast<UINT>(LOWORD(lParam)), static_cast<UINT>(HIWORD(lParam)), DXGI_FORMAT_UNKNOWN, 0);
                mDirectX->createRenderTarget();
            }
        return 0;

        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            {
                return 0;
            }
        break;

        case WM_DESTROY:
            ::PostQuitMessage(0);
        return 0;
    }

    return ::DefWindowProc(w->handler(), msg, wParam, lParam);
}
} // namespace ttt
