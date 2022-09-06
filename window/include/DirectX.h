#pragma once

#include <memory>

#include <d3d11.h>

#include "release_deleter.h"

namespace ttt
{
class Window;

class DirectX
{
public:
    using Device = std::unique_ptr<ID3D11Device, release_deleter<ID3D11Device>>;
    using Context = std::unique_ptr<ID3D11DeviceContext, release_deleter<ID3D11DeviceContext>>;
    using SwapChain = std::unique_ptr<IDXGISwapChain, release_deleter<IDXGISwapChain>>;
    using RenderTargetView = std::unique_ptr<ID3D11RenderTargetView, release_deleter<ID3D11RenderTargetView>>;

    DirectX() = default;

    Device const& device() const;
    Context const& context() const;
    SwapChain const& swapChain() const;
    RenderTargetView const& renderTarget() const;

    bool setup(std::shared_ptr<Window> const& window, bool debug = false);
    void cleanup();

    void createRenderTarget();
    void cleanupRenderTarget();

private:
    Device mDevice;
    Context mContext;
    SwapChain mSwapChain;
    RenderTargetView mMainRenderTargetView;
};

} // namespace ttt
