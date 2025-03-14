
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX11_PRESENTATION_LAYER_H__
#define __IC3_GRAPHICS_HW3D_DX11_PRESENTATION_LAYER_H__

#include "DX11Prerequisites.h"
#include <Ic3/Graphics/GCI/State/CommonGPUStateDefs.h>
#include <Ic3/Graphics/HW3D/DX/DXPresentationLayer.h>

namespace Ic3::Graphics::GCI
{

	Ic3DeclareClassHandle( DX11ScreenPresentationLayer );

	struct DX11PresentationLayerCreateInfo : public PresentationLayerCreateInfo
	{
		cppx::bitmask<UINT> dxgiFlags = 0;
	};

	class IC3_GX_DX11_CLASS DX11ScreenPresentationLayer final : public DXScreenPresentationLayer
	{
	public:
		ComPtr<ID3D11Device1> const mD3D11Device1;

		DX11RenderTargetDescriptorHandle const mDX11ScreenRenderTargetDescriptor;

	public:
		DX11ScreenPresentationLayer(
				DX11GPUDevice & pDevice,
				System::WindowHandle pSysWindow,
				ComPtr<IDXGISwapChain1> pDXGISwapChain,
				DX11RenderTargetDescriptorHandle pDX11ScreenRenderTargetDescriptor );

		virtual ~DX11ScreenPresentationLayer();

		virtual void BindRenderTarget( CommandContext & pCommandContext ) override;

		virtual void InvalidateRenderTarget( CommandContext & pCommandContext ) override;

		virtual void Present() override;

		static DX11ScreenPresentationLayerHandle Create( DX11GPUDevice & pDX11Device, const DX11PresentationLayerCreateInfo & pCreateInfo );

	private:
		ID3D11RenderTargetView * GetBackBufferRTView();
		ID3D11DepthStencilView * GetBackBufferDSView();
		bool _CreateBackBufferResources();

	private:
		ComPtr<ID3D11RenderTargetView> _backBufferRTView;
		ComPtr<ID3D11DepthStencilView> _backBufferDSView;
		ComPtr<ID3D11Texture2D> _backBufferDSTexture;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_PRESENTATION_LAYER_H__
