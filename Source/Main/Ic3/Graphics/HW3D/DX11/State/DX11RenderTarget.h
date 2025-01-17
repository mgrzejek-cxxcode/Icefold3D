
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX11_RENDER_TARGET_H__
#define __IC3_GRAPHICS_HW3D_DX11_RENDER_TARGET_H__

#include "../Resources/DX11Texture.h"
#include <Ic3/Graphics/GCI/Resources/TextureReference.h>
#include <Ic3/Graphics/GCI/State/RenderTargetImmutableStates.h>

namespace Ic3::Graphics::GCI
{

	class DX11Texture;

	struct DX11RenderTargetColorAttachment
	{
		ID3D11Resource * d3d11Resource = nullptr;
		UINT d3d11SubResourceIndex;
		ComPtr<ID3D11RenderTargetView> d3d11RTView;

		explicit operator bool() const noexcept
		{
			return d3d11Resource != nullptr;
		}
	};

	struct DX11RenderTargetDepthStencilAttachment
	{
		ID3D11Resource * d3d11Resource = nullptr;
		UINT d3d11SubResourceIndex;
		ComPtr<ID3D11DepthStencilView> d3d11DSView;

		explicit operator bool() const noexcept
		{
			return d3d11Resource != nullptr;
		}
	};

	struct DX11RenderTargetResolveAttachment
	{
		ID3D11Resource * d3d11Resource = nullptr;
		UINT d3d11SubResourceIndex;
		DXGI_FORMAT targetDXGIFormat;

		explicit operator bool() const noexcept
		{
			return d3d11Resource != nullptr;
		}
	};

	struct DX11RenderTargetBindingData
	{
		cppx::bitmask<ERTAttachmentFlags> activeAttachmentsMask = 0;

		DX11RenderTargetColorAttachment colorAttachments[GCM::RT_MAX_COLOR_ATTACHMENTS_NUM];
		DX11RenderTargetDepthStencilAttachment depthStencilAttachment;
		DX11RenderTargetResolveAttachment resolveAttachments[GCM::RT_MAX_COMBINED_ATTACHMENTS_NUM];

		ID3D11RenderTargetView * d3d11ColorAttachmentRTViewArray[GCM::RT_MAX_COLOR_ATTACHMENTS_NUM];
		ID3D11DepthStencilView * d3d11DepthStencilAttachmentDSView;
	};

	class DX11RenderTargetBindingImmutableState : public RenderTargetBindingImmutableState
	{
	public:
		DX11RenderTargetBindingData const mDX11RTBindingData;

	public:
		DX11RenderTargetBindingImmutableState(
				DX11GpuDevice & pGpuDevice,
				const RenderTargetLayout & pRenderTargetLayout,
				DX11RenderTargetBindingData pDX11RTBindingData );

		virtual ~DX11RenderTargetBindingImmutableState();

		static GpaHandle<DX11RenderTargetBindingImmutableState> CreateInstance(
				DX11GpuDevice & pGpuDevice,
				const RenderTargetBindingDefinition & pBindingDefinition );

		static GpaHandle<DX11RenderTargetBindingImmutableState> CreateForScreen(
				DX11GpuDevice & pGpuDevice,
				ComPtr<ID3D11Texture2D> pColorBuffer,
				ComPtr<ID3D11Texture2D> pDepthStencilBuffer );
	};


	namespace SMU
	{

		CPPX_ATTR_NO_DISCARD RenderTargetLayout GetRenderTargetLayoutForScreenDX11(
				ID3D11Texture2D * pColorBuffer,
				ID3D11Texture2D * pDepthStencilBuffer );

		CPPX_ATTR_NO_DISCARD DX11RenderTargetColorAttachment CreateRenderTargetColorAttachmentDX11(
				DX11GpuDevice & pGpuDevice,
				const TextureReference & pAttachmentTextureRef );

		CPPX_ATTR_NO_DISCARD DX11RenderTargetDepthStencilAttachment CreateRenderTargetDepthStencilAttachmentDX11(
				DX11GpuDevice & pGpuDevice,
				const TextureReference & pAttachmentTextureRef );

		CPPX_ATTR_NO_DISCARD DX11RenderTargetResolveAttachment CreateRenderTargetResolveAttachmentDX11(
				const TextureReference & pAttachmentTextureRef );

		CPPX_ATTR_NO_DISCARD DX11RenderTargetBindingData CreateRenderTargetBindingDataDX11(
				DX11GpuDevice & pGpuDevice,
				const RenderTargetBindingDefinition & pBindingDefinition );

		void RenderPassClearRenderTargetDX11(
				ID3D11DeviceContext1 * pD3D1DeviceContext,
				const DX11RenderTargetBindingData & pRenderTargetBinding,
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicState & pDynamicState );

		void RenderPassResolveRenderTargetDX11(
				ID3D11DeviceContext1 * pD3D1DeviceContext,
				const DX11RenderTargetBindingData & pRenderTargetBinding,
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicState & pDynamicState );

	}
	
} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_RENDER_TARGET_H__
