#include "DirectX.h"

#include "Window.h"

namespace ttt
{
DirectX::Device const& DirectX::device() const
{
    return mDevice;
}

DirectX::Context const& DirectX::context() const
{
    return mContext;
}

DirectX::SwapChain const& DirectX::swapChain() const
{
    return mSwapChain;
}

DirectX::RenderTargetView const& DirectX::renderTarget() const
{
    return mMainRenderTargetView;
}

bool DirectX::setup(std::shared_ptr<Window> const& window, bool debug)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = window->handler();
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = debug ? D3D11_CREATE_DEVICE_DEBUG : 0;
    D3D_FEATURE_LEVEL featureLevel;
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;
    IDXGISwapChain* swapChain = nullptr;

    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd,
        &swapChain, &device, &featureLevel, &context) != S_OK)
    {
        cleanup();
        return false;
    }

    mDevice = Device(device);
    mContext = Context(context);
    mSwapChain = SwapChain(swapChain);

    createRenderTarget();
    return true;
}

void DirectX::cleanup()
{
    cleanupRenderTarget();
    mSwapChain.release();
    mContext.release();
    mDevice.release();
}

void DirectX::createRenderTarget()
{
    ID3D11Texture2D* pBackBuffer = nullptr;
    swapChain()->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    if (pBackBuffer)
    {
        ID3D11RenderTargetView* renderTarget = nullptr;
        device()->CreateRenderTargetView(pBackBuffer, NULL, &renderTarget);
        mMainRenderTargetView = RenderTargetView(renderTarget);
        pBackBuffer->Release();
    }
}

void DirectX::cleanupRenderTarget()
{
    mMainRenderTargetView.release();
}
} // namespace ttt
