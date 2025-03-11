
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX11_RENDER_TARGET_H__
#define __IC3_GRAPHICS_HW3D_DX11_RENDER_TARGET_H__

#include "../Resources/DX11Texture.h"
#include <Ic3/Graphics/GCI/Resources/TextureReference.h>
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorRTO.h>

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

	struct DX11RenderTargetBinding : public RenderTargetArrayCommonConfig
	{
		DX11RenderTargetColorAttachment colorAttachments[GCM::kRTOMaxColorAttachmentsNum];
		DX11RenderTargetDepthStencilAttachment depthStencilAttachment;
		DX11RenderTargetResolveAttachment resolveAttachments[GCM::kRTOMaxCombinedAttachmentsNum];

		ID3D11RenderTargetView * d3d11ColorAttachmentRTViewArray[GCM::kRTOMaxColorAttachmentsNum];
		ID3D11DepthStencilView * d3d11DepthStencilAttachmentDSView;
	};

	class DX11RenderTargetDescriptor : public PIM::RenderTargetDescriptorNative
	{
	public:
		DX11RenderTargetBinding const mDX11RenderTargetBinding;

	public:
		DX11RenderTargetDescriptor(
				DX11GPUDevice & pGPUDevice,
				const RenderTargetLayout & pRenderTargetLayout,
				DX11RenderTargetBinding pDX11RTBindingData );

		virtual ~DX11RenderTargetDescriptor();

		static TGfxHandle<DX11RenderTargetDescriptor> CreateInstance(
				DX11GPUDevice & pGPUDevice,
				const RenderTargetDescriptorCreateInfo & pCreateInfo );

		static TGfxHandle<DX11RenderTargetDescriptor> CreateForScreen(
				DX11GPUDevice & pGPUDevice,
				ComPtr<ID3D11Texture2D> pColorBuffer,
				ComPtr<ID3D11Texture2D> pDepthStencilBuffer );
	};


	namespace GCU
	{

		CPPX_ATTR_NO_DISCARD RenderTargetLayout GetRenderTargetLayoutForScreenDX11(
				ID3D11Texture2D * pColorBuffer,
				ID3D11Texture2D * pDepthStencilBuffer );

		CPPX_ATTR_NO_DISCARD DX11RenderTargetColorAttachment CreateRenderTargetColorAttachmentDX11(
				DX11GPUDevice & pGPUDevice,
				const TextureReference & pAttachmentTextureRef );

		CPPX_ATTR_NO_DISCARD DX11RenderTargetDepthStencilAttachment CreateRenderTargetDepthStencilAttachmentDX11(
				DX11GPUDevice & pGPUDevice,
				const TextureReference & pAttachmentTextureRef );

		CPPX_ATTR_NO_DISCARD DX11RenderTargetResolveAttachment CreateRenderTargetResolveAttachmentDX11(
				const TextureReference & pAttachmentTextureRef );

		CPPX_ATTR_NO_DISCARD DX11RenderTargetBinding CreateRenderTargetBindingDX11(
				DX11GPUDevice & pGPUDevice,
				const RenderTargetBindingDefinition & pBindingDefinition );

		void RenderPassClearRenderTargetDX11(
				ID3D11DeviceContext1 * pD3D1DeviceContext,
				const DX11RenderTargetBinding & pRenderTargetBinding,
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicConfig & pDynamicConfig );

		void RenderPassResolveRenderTargetDX11(
				ID3D11DeviceContext1 * pD3D1DeviceContext,
				const DX11RenderTargetBinding & pRenderTargetBinding,
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicConfig & pDynamicConfig );

	}
	
} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_RENDER_TARGET_H__
