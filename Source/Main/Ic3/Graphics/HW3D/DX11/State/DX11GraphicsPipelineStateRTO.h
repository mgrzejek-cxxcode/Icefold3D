
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX11_GRAPHICS_PIPELINE_STATE_RTO_H__
#define __IC3_GRAPHICS_HW3D_DX11_GRAPHICS_PIPELINE_STATE_RTO_H__

#include "../Resources/DX11Texture.h"
#include <Ic3/Graphics/GCI/Resources/TextureReference.h>
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorRTO.h>
#include <Ic3/Graphics/GCI/Utilities/DSTRenderTargetBinding.h>

namespace Ic3::Graphics::GCI
{

	class DX11Texture;

	struct DX11RenderTargetColorAttachmentRef
	{
		ID3D11Resource * d3d11Resource = nullptr;
		UINT d3d11SubResourceIndex;
		ComPtr<ID3D11RenderTargetView> d3d11RTV;

		explicit operator bool() const noexcept
		{
			return d3d11Resource != nullptr;
		}
	};

	struct DX11RenderTargetDepthStencilAttachmentRef
	{
		ID3D11Resource * d3d11Resource = nullptr;
		UINT d3d11SubResourceIndex;
		ComPtr<ID3D11DepthStencilView> d3d11DSV;

		explicit operator bool() const noexcept
		{
			return d3d11Resource != nullptr;
		}
	};

	struct DX11RenderTargetResolveAttachmentRef
	{
		ID3D11Resource * d3d11Resource = nullptr;
		UINT d3d11SubResourceIndex;
		DXGI_FORMAT targetDXGIFormat;

		explicit operator bool() const noexcept
		{
			return d3d11Resource != nullptr;
		}
	};

	using DX11RenderTargetBindingBase = DSTRenderTargetBindingBase<
		DX11RenderTargetColorAttachmentRef,
		DX11RenderTargetResolveAttachmentRef,
		DX11RenderTargetDepthStencilAttachmentRef,
		DX11RenderTargetResolveAttachmentRef>;

	template <native_uint tpActiveColorOutputAttachmentsNum, native_uint tpActiveColorResolveAttachmentsNum>
	using DX11RenderTargetBinding = DSTRenderTargetBinding<DX11RenderTargetBindingBase, tpActiveColorOutputAttachmentsNum, tpActiveColorResolveAttachmentsNum>;

	using DX11RenderTargetBindingPtr = std::unique_ptr<DX11RenderTargetBindingBase>;

	/**
	 * @brief 
	 */
	struct DX11UnwrappedRenderTargetBinding
	{
		std::array<ID3D11RenderTargetView *, GCM::kRTOMaxColorAttachmentsNum> colorAttachmentRTVArray;

		ID3D11DepthStencilView * depthStencilAttachmentDSV;

		void SetBindings( const DX11RenderTargetBindingBase & pDX11RenderTargetBinding )
		{
			for( native_uint colorAttachmentIndex = 0; colorAttachmentIndex < GCM::kRTOMaxColorAttachmentsNum; ++colorAttachmentIndex )
			{
				if( const auto * dx11ColorAttachmentBinding = pDX11RenderTargetBinding.GetBindingForColorOutputAttachment( colorAttachmentIndex ) )
				{
					colorAttachmentRTVArray[colorAttachmentIndex] = dx11ColorAttachmentBinding->d3d11RTV.Get();
				}
				else
				{
					colorAttachmentRTVArray[colorAttachmentIndex] = nullptr;
				}
			}

			if( pDX11RenderTargetBinding.depthStencilOutputAttachmentBinding )
			{
				depthStencilAttachmentDSV = pDX11RenderTargetBinding.depthStencilOutputAttachmentBinding.d3d11DSV.Get();
			}
			else
			{
				depthStencilAttachmentDSV = nullptr;
			}
		}
	};

	class IC3_GX_DX11_CLASS DX11RenderTargetDescriptor : public HW3DPipelineStateDescriptor<RenderTargetDescriptor>
	{
	public:
		DX11RenderTargetBindingPtr const mDX11RenderTargetBinding;

	public:
		DX11RenderTargetDescriptor(
				DX11GPUDevice & pGPUDevice,
				DX11RenderTargetBindingPtr pDX11RenderTargetBinding );

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

		CPPX_ATTR_NO_DISCARD DX11RenderTargetBindingPtr RTOCreateRenderTargetBindingDX11(
				DX11GPUDevice & pGPUDevice,
				const RenderTargetBinding & pRenderTargetBinding );

		CPPX_ATTR_NO_DISCARD DX11RenderTargetBindingPtr RTOCreateRenderTargetBindingForScreenDX11(
				DX11GPUDevice & pGPUDevice,
				ComPtr<ID3D11Texture2D> pColorBuffer,
				ComPtr<ID3D11Texture2D> pDepthStencilBuffer );

		CPPX_ATTR_NO_DISCARD bool RTOUpdateRenderTargetBindingDX11(
				DX11GPUDevice & pGPUDevice,
				const RenderTargetBinding & pRenderTargetBinding,
				DX11RenderTargetBindingBase & pOutDX11RenderTargetBinding );

		CPPX_ATTR_NO_DISCARD DX11RenderTargetColorAttachmentRef RTOCreateColorAttachmentDX11(
				DX11GPUDevice & pGPUDevice,
				const TextureReference & pAttachmentTextureRef );

		CPPX_ATTR_NO_DISCARD DX11RenderTargetDepthStencilAttachmentRef RTOCreateDepthStencilAttachmentDX11(
				DX11GPUDevice & pGPUDevice,
				const TextureReference & pAttachmentTextureRef );

		CPPX_ATTR_NO_DISCARD DX11RenderTargetResolveAttachmentRef RTOCreateResolveAttachmentDX11(
				const TextureReference & pAttachmentTextureRef );

		cppx::bitmask<ERTAttachmentFlags> RTORenderPassExecuteOpLoadClearDX11(
				ID3D11DeviceContext1 * pD3D11DeviceContext,
				const DX11RenderTargetBindingBase & pDX11RenderTargetBinding,
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicConfig & pDynamicConfig );

		cppx::bitmask<ERTAttachmentFlags> RTORenderPassExecuteOpStoreResolveDX11(
				ID3D11DeviceContext1 * pD3D11DeviceContext,
				const DX11RenderTargetBindingBase & pRenderTargetBinding,
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicConfig & pDynamicConfig );

	}
	
} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_GRAPHICS_PIPELINE_STATE_RTO_H__
