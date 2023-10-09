
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX11_PRESENTATION_LAYER_H__
#define __IC3_GRAPHICS_HW3D_DX11_PRESENTATION_LAYER_H__

#include "DX11Prerequisites.h"
#include <Ic3/Graphics/GCI/State/CommonGPUStateDefs.h>
#include <Ic3/Graphics/HW3D/DX/DXPresentationLayer.h>

namespace Ic3::Graphics::GCI
{

	ic3DeclareClassHandle( DX11ScreenPresentationLayer );

	struct DX11PresentationLayerCreateInfo : public PresentationLayerCreateInfo
	{
		Bitmask<UINT> dxgiFlags = 0;
	};

	class ICFGX_DX11_CLASS DX11ScreenPresentationLayer final : public DXScreenPresentationLayer
	{
	public:
		ComPtr<ID3D11Device1> const mD3D11Device1;

		RenderTargetBindingImmutableStateHandle const mScreenRenderTargetBindingState;

	public:
		DX11ScreenPresentationLayer(
				DX11GPUDevice & pDevice,
				System::WindowHandle pSysWindow,
				ComPtr<IDXGISwapChain1> pDXGISwapChain,
				RenderTargetBindingImmutableStateHandle pScreenRenderTargetBindingState );

		virtual ~DX11ScreenPresentationLayer();

		virtual void bindRenderTarget( CommandContext * pCmdContext ) override;

		virtual void invalidateRenderTarget( CommandContext * pCmdContext ) override;

		virtual void present() override;

		static DX11ScreenPresentationLayerHandle create( DX11GPUDevice & pDX11Device, const DX11PresentationLayerCreateInfo & pCreateInfo );

	private:
		ID3D11RenderTargetView * getBackBufferRTView();
		ID3D11DepthStencilView * getBackBufferDSView();
		bool _createBackBufferResources();

	private:
		ComPtr<ID3D11RenderTargetView> _backBufferRTView;
		ComPtr<ID3D11DepthStencilView> _backBufferDSView;
		ComPtr<ID3D11Texture2D> _backBufferDSTexture;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_PRESENTATION_LAYER_H__
