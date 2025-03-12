
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX11_GRAPHICS_PIPELINE_STATE_RTO_H__
#define __IC3_GRAPHICS_HW3D_DX11_GRAPHICS_PIPELINE_STATE_RTO_H__

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
		RenderTargetLayout renderTargetLayout;

		DX11RenderTargetColorAttachment colorAttachments[GCM::kRTOMaxColorAttachmentsNum];
		DX11RenderTargetDepthStencilAttachment depthStencilAttachment;
		DX11RenderTargetResolveAttachment resolveAttachments[GCM::kRTOMaxCombinedAttachmentsNum];

		ID3D11RenderTargetView * d3d11ColorAttachmentRTViewArray[GCM::kRTOMaxColorAttachmentsNum]{};
		ID3D11DepthStencilView * d3d11DepthStencilAttachmentDSView = nullptr;
	};

	struct DX11RenderTargetBindingStatic : public DX11RenderTargetBinding
	{
		// Dynamic RT descriptors store GCI::RenderTargetBinding, so they contain active
		// texture references on their own - on the DX11 level, we only need D3D11 resources.
	};

	struct DX11RenderTargetBindingDynamic : public DX11RenderTargetBinding
	{
		// An array of RenderTargetAttachmentBinding objects, holding
		// active references to the textures/buffers used by a descriptor.
		TRenderTargetAttachmentPropertyArray<RenderTargetAttachmentBinding> gciBindings;
	};

	class IC3_GX_DX11_CLASS DX11RenderTargetDescriptor : public HW3DPipelineStateDescriptor<RenderTargetDescriptor>
	{
	public:
		DX11RenderTargetBinding const mDX11RenderTargetBinding;

	public:
		DX11RenderTargetDescriptor(
				DX11GPUDevice & pGPUDevice,
				DX11RenderTargetBinding pDX11RenderTargetBinding );

		virtual ~DX11RenderTargetDescriptor();

		virtual bool IsAttachmentConfigured( native_uint pAttachmentIndex ) const noexcept override final;

		static TGfxHandle<DX11RenderTargetDescriptor> CreateInstance(
				DX11GPUDevice & pGPUDevice,
				const RenderTargetBinding & pRenderTargetBinding );

		static TGfxHandle<DX11RenderTargetDescriptor> CreateForScreen(
				DX11GPUDevice & pGPUDevice,
				ComPtr<ID3D11Texture2D> pColorBuffer,
				ComPtr<ID3D11Texture2D> pDepthStencilBuffer );
	};


	namespace GCU
	{

		CPPX_ATTR_NO_DISCARD RenderTargetLayout RTOGetRenderTargetLayoutForScreenDX11(
				ID3D11Texture2D * pD3D11ColorBufferTexture,
				ID3D11Texture2D * pD3D11DepthStencilBufferTexture );

		CPPX_ATTR_NO_DISCARD DX11RenderTargetColorAttachment RTOCreateColorAttachmentDX11(
				DX11GPUDevice & pGPUDevice,
				const TextureReference & pAttachmentTextureRef );

		CPPX_ATTR_NO_DISCARD DX11RenderTargetDepthStencilAttachment RTOCreateDepthStencilAttachmentDX11(
				DX11GPUDevice & pGPUDevice,
				const TextureReference & pAttachmentTextureRef );

		CPPX_ATTR_NO_DISCARD DX11RenderTargetResolveAttachment RTOCreateResolveAttachmentDX11(
				const TextureReference & pAttachmentTextureRef );

		CPPX_ATTR_NO_DISCARD DX11RenderTargetBinding RTOCreateRenderTargetBindingDX11(
				DX11GPUDevice & pGPUDevice,
				const RenderTargetBinding & pRenderTargetBinding );

		CPPX_ATTR_NO_DISCARD bool RTOUpdateRenderTargetBindingDX11(
				DX11GPUDevice & pGPUDevice,
				const RenderTargetBinding & pRenderTargetBinding,
				DX11RenderTargetBinding & pOutDX11RenderTargetBinding );

		void RTORenderPassExecuteOpLoadClearDX11(
				ID3D11DeviceContext1 * pD3D11DeviceContext,
				const DX11RenderTargetBinding & pDX11RenderTargetBinding,
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicConfig & pDynamicConfig );

		void RTORenderPassExecuteOpStoreResolveDX11(
				ID3D11DeviceContext1 * pD3D11DeviceContext,
				const DX11RenderTargetBinding & pRenderTargetBinding,
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicConfig & pDynamicConfig );

	}
	
} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_GRAPHICS_PIPELINE_STATE_RTO_H__
