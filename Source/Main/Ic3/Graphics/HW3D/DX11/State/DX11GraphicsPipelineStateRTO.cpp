
#include "DX11GraphicsPipelineStateRTO.h"
#include <Ic3/Graphics/GCI/Resources/RenderTargetTexture.h>
#include <Ic3/Graphics/HW3D/DX11/DX11APITranslationLayer.h>
#include <Ic3/Graphics/HW3D/DX11/DX11gpuDevice.h>
#include <Ic3/Graphics/HW3D/DX11/Resources/DX11Texture.h>

namespace Ic3::Graphics::GCI
{

	DX11RenderTargetDescriptor::DX11RenderTargetDescriptor(
			DX11GPUDevice & pGPUDevice,
			DX11RenderTargetBinding pDX11RenderTargetBinding )
	: HW3DPipelineStateDescriptor( pGPUDevice )
	, mDX11RenderTargetBinding( std::move( pDX11RenderTargetBinding ) )
	{}

	DX11RenderTargetDescriptor::~DX11RenderTargetDescriptor() = default;

	bool DX11RenderTargetDescriptor::IsAttachmentConfigured( native_uint pAttachmentIndex ) const noexcept
	{
		return mDX11RenderTargetBinding.activeAttachmentsMask.is_set( CXU::RTOMakeAttachmentFlag( pAttachmentIndex ) );
	}

	TGfxHandle<DX11RenderTargetDescriptor> DX11RenderTargetDescriptor::CreateInstance(
			DX11GPUDevice & pGPUDevice,
			const RenderTargetBinding & pRenderTargetBinding )
	{
		auto dx11RenderTargetBinding = GCU::RTOCreateRenderTargetBindingDX11( pGPUDevice, pRenderTargetBinding );

		auto dx11RenderTargetDescriptor = CreateGfxObject<DX11RenderTargetDescriptor>( pGPUDevice, std::move( dx11RenderTargetBinding ) );

		return dx11RenderTargetDescriptor;
	}

	TGfxHandle<DX11RenderTargetDescriptor> DX11RenderTargetDescriptor::CreateForScreen(
			DX11GPUDevice & pGPUDevice,
			ComPtr<ID3D11Texture2D> pColorBuffer,
			ComPtr<ID3D11Texture2D> pDepthStencilBuffer )
	{
		ComPtr<ID3D11RenderTargetView> colorBufferRTView;
		auto hResult = pGPUDevice.mD3D11Device1->CreateRenderTargetView( pColorBuffer.Get(), nullptr, colorBufferRTView.GetAddressOf() );
		if( FAILED( hResult ) )
		{
			return nullptr;
		}

		ComPtr<ID3D11DepthStencilView> depthStencilBufferDSView;
		hResult = pGPUDevice.mD3D11Device1->CreateDepthStencilView( pDepthStencilBuffer.Get(), nullptr, depthStencilBufferDSView.GetAddressOf() );
		if( FAILED( hResult ) )
		{
			return nullptr;
		}

		DX11RenderTargetBinding dx11RTBindingData;
		dx11RTBindingData.activeAttachmentsMask = eRTAttachmentMaskDefaultC0DS;
		dx11RTBindingData.colorAttachments[0].d3d11RTView = colorBufferRTView;
		dx11RTBindingData.colorAttachments[0].d3d11Resource = pColorBuffer.Get();
		dx11RTBindingData.colorAttachments[0].d3d11SubResourceIndex = 0;
		dx11RTBindingData.d3d11ColorAttachmentRTViewArray[0] = colorBufferRTView.Get();
		dx11RTBindingData.depthStencilAttachment.d3d11DSView = depthStencilBufferDSView;
		dx11RTBindingData.depthStencilAttachment.d3d11Resource = pDepthStencilBuffer.Get();
		dx11RTBindingData.depthStencilAttachment.d3d11SubResourceIndex = 0;
		dx11RTBindingData.d3d11DepthStencilAttachmentDSView = depthStencilBufferDSView.Get();
		dx11RTBindingData.renderTargetLayout = GCU::RTOGetRenderTargetLayoutForScreenDX11( pColorBuffer.Get(), pDepthStencilBuffer.Get() );

		auto dx11RenderTargetDescriptor = CreateGfxObject<DX11RenderTargetDescriptor>( pGPUDevice, std::move( dx11RTBindingData ) );

		return dx11RenderTargetDescriptor;
	}


	namespace GCU
	{

		RenderTargetLayout RTOGetRenderTargetLayoutForScreenDX11(
				ID3D11Texture2D * pD3D11ColorBufferTexture,
				ID3D11Texture2D * pD3D11DepthStencilBufferTexture )
		{
			D3D11_TEXTURE2D_DESC colorBufferTextureDesc;
			pD3D11ColorBufferTexture->GetDesc( & colorBufferTextureDesc );

			D3D11_TEXTURE2D_DESC depthStencilBufferTextureDesc;
			pD3D11DepthStencilBufferTexture->GetDesc( & depthStencilBufferTextureDesc );

			RenderTargetLayout renderTargetLayout;
			renderTargetLayout.activeAttachmentsMask = eRTAttachmentMaskDefaultC0DS;
			renderTargetLayout.sharedImageSize.width = colorBufferTextureDesc.Width;
			renderTargetLayout.sharedImageSize.height = colorBufferTextureDesc.Height;
			renderTargetLayout.sharedMultiSamplingSettings.sampleCount = cppx::numeric_cast<uint16>( colorBufferTextureDesc.SampleDesc.Count );
			renderTargetLayout.sharedMultiSamplingSettings.sampleQuality = cppx::numeric_cast<uint16>( colorBufferTextureDesc.SampleDesc.Quality );
			renderTargetLayout.colorAttachments[0].format = ATL::DXTranslateTextureFormatInv( colorBufferTextureDesc.Format );
			renderTargetLayout.depthStencilAttachment.format = ATL::DXTranslateTextureFormatInv( depthStencilBufferTextureDesc.Format );

			return renderTargetLayout;
		}

		DX11RenderTargetColorAttachment RTOCreateColorAttachmentDX11(
				DX11GPUDevice & pGPUDevice,
				const TextureReference & pAttachmentTextureRef )
		{
			auto * dx11Texture = pAttachmentTextureRef->QueryInterface<DX11Texture>();
			const auto & textureSubResource = pAttachmentTextureRef.GetRefSubResource();

			D3D11_RENDER_TARGET_VIEW_DESC d3d11RTViewDesc;
			d3d11RTViewDesc.ViewDimension = D3D11_RTV_DIMENSION_UNKNOWN;

			ID3D11Resource * d3d11AttachmentResource = nullptr;
			UINT d3d11AttachmentSubResourceIndex = 0;

			if( dx11Texture->mTextureLayout.texClass == ETextureClass::T3D )
			{
				auto * d3d11Texture3D = dx11Texture->mD3D11Texture3D.Get();
				Ic3DebugAssert( d3d11Texture3D );

				D3D11_TEXTURE3D_DESC texture3DDesc;
				d3d11Texture3D->GetDesc( &texture3DDesc );

				d3d11RTViewDesc.Format = texture3DDesc.Format;
				d3d11RTViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE3D;
				d3d11RTViewDesc.Texture3D.MipSlice = textureSubResource.uSubRes3D.mipLevel;
				d3d11RTViewDesc.Texture3D.FirstWSlice = textureSubResource.uSubRes3D.depthLayerIndex;

				d3d11AttachmentResource = d3d11Texture3D;
				d3d11AttachmentSubResourceIndex = D3D11CalcSubresource(
						textureSubResource.uSubRes3D.mipLevel,
						0,
						dx11Texture->mTextureLayout.dimensions.mipLevelsNum );
			}
			else
			{
				auto * d3d11Texture2D = dx11Texture->mD3D11Texture2D.Get();
				Ic3DebugAssert( d3d11Texture2D );

				D3D11_TEXTURE2D_DESC texture2DDesc;
				d3d11Texture2D->GetDesc( &texture2DDesc );

				d3d11RTViewDesc.Format = texture2DDesc.Format;

				if( dx11Texture->mTextureLayout.texClass == ETextureClass::T2D )
				{
					d3d11RTViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
					d3d11RTViewDesc.Texture2D.MipSlice = textureSubResource.uSubRes2D.mipLevel;
					d3d11AttachmentSubResourceIndex = D3D11CalcSubresource(
							textureSubResource.uSubRes2D.mipLevel,
							0,
							dx11Texture->mTextureLayout.dimensions.mipLevelsNum );

				}
				else if( dx11Texture->mTextureLayout.texClass == ETextureClass::T2DArray )
				{
					d3d11RTViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
					d3d11RTViewDesc.Texture2DArray.MipSlice = textureSubResource.uSubRes2DArray.mipLevel;
					d3d11RTViewDesc.Texture2DArray.FirstArraySlice = textureSubResource.uSubRes2DArray.arrayIndex;
					d3d11RTViewDesc.Texture2DArray.ArraySize = 1;
					d3d11AttachmentSubResourceIndex = D3D11CalcSubresource(
							textureSubResource.uSubRes2DArray.mipLevel,
							textureSubResource.uSubRes2DArray.arrayIndex,
							dx11Texture->mTextureLayout.dimensions.mipLevelsNum );
				}
				else if( dx11Texture->mTextureLayout.texClass == ETextureClass::T2DMS )
				{
					d3d11RTViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
					d3d11AttachmentSubResourceIndex = D3D11CalcSubresource( 0, 0, 0 );
				}
				else if( dx11Texture->mTextureLayout.texClass == ETextureClass::T2DMSArray )
				{
					d3d11RTViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY;
					d3d11RTViewDesc.Texture2DMSArray.FirstArraySlice = textureSubResource.uSubRes2DArray.arrayIndex;
					d3d11RTViewDesc.Texture2DMSArray.ArraySize = 1;
					d3d11AttachmentSubResourceIndex = D3D11CalcSubresource( 0, textureSubResource.uSubRes2DArray.arrayIndex, 0 );
				}
				else if( dx11Texture->mTextureLayout.texClass == ETextureClass::TCubeMap )
				{
					d3d11RTViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
					d3d11RTViewDesc.Texture2DArray.MipSlice = textureSubResource.uSubResCubeMap.mipLevel;
					d3d11RTViewDesc.Texture2DArray.FirstArraySlice = textureSubResource.uSubResCubeMap.faceIndex;
					d3d11RTViewDesc.Texture2DArray.ArraySize = 1;
					d3d11AttachmentSubResourceIndex = D3D11CalcSubresource(
							textureSubResource.uSubResCubeMap.mipLevel,
							textureSubResource.uSubResCubeMap.faceIndex,
							dx11Texture->mTextureLayout.dimensions.mipLevelsNum );
				}
				else
				{
					Ic3DebugInterrupt();
					return {};
				}

				d3d11AttachmentResource = d3d11Texture2D;
			}

			ComPtr<ID3D11RenderTargetView> d3d11ResourceRTView;
			auto hResult = pGPUDevice.mD3D11Device1->CreateRenderTargetView( d3d11AttachmentResource, &d3d11RTViewDesc, d3d11ResourceRTView.GetAddressOf() );

			if( FAILED( hResult ) )
			{
				Ic3DebugInterrupt();
				return {};
			}

			DX11RenderTargetColorAttachment dx11ColorAttachment;
			dx11ColorAttachment.d3d11Resource = d3d11AttachmentResource;
			dx11ColorAttachment.d3d11SubResourceIndex = d3d11AttachmentSubResourceIndex;
			dx11ColorAttachment.d3d11RTView = d3d11ResourceRTView;

			return dx11ColorAttachment;
		}

		DX11RenderTargetDepthStencilAttachment RTOCreateDepthStencilAttachmentDX11(
				DX11GPUDevice & pGPUDevice,
				const TextureReference & pAttachmentTextureRef )
		{
			auto * dx11Texture = pAttachmentTextureRef->QueryInterface<DX11Texture>();
			const auto & textureSubResource = pAttachmentTextureRef.GetRefSubResource();

			auto * d3d11Texture2D = dx11Texture->mD3D11Texture2D.Get();
			Ic3DebugAssert( d3d11Texture2D );

			ID3D11Resource * d3d11AttachmentResource = d3d11Texture2D;
			UINT d3d11AttachmentSubResourceIndex = 0;

			D3D11_TEXTURE2D_DESC texture2DDesc;
			d3d11Texture2D->GetDesc( &texture2DDesc );

			D3D11_DEPTH_STENCIL_VIEW_DESC d3d11DSViewDesc;
			d3d11DSViewDesc.ViewDimension = D3D11_DSV_DIMENSION_UNKNOWN;
			d3d11DSViewDesc.Flags = 0;
			d3d11DSViewDesc.Format = texture2DDesc.Format;

			if( RCU::checkIsDXGIFormatTypelessDX11( d3d11DSViewDesc.Format ) )
			{
				d3d11DSViewDesc.Format = RCU::GetDSVFormatForTypelessFormatDX11( d3d11DSViewDesc.Format );
			}

			if( dx11Texture->mTextureLayout.texClass == ETextureClass::T2D )
			{
				d3d11DSViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				d3d11DSViewDesc.Texture2D.MipSlice = textureSubResource.uSubRes2D.mipLevel;
				d3d11AttachmentSubResourceIndex = D3D11CalcSubresource(
						textureSubResource.uSubRes2D.mipLevel,
						0,
						dx11Texture->mTextureLayout.dimensions.mipLevelsNum );

			}
			else if( dx11Texture->mTextureLayout.texClass == ETextureClass::T2DArray )
			{
				d3d11DSViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
				d3d11DSViewDesc.Texture2DArray.MipSlice = textureSubResource.uSubRes2DArray.mipLevel;
				d3d11DSViewDesc.Texture2DArray.FirstArraySlice = textureSubResource.uSubRes2DArray.arrayIndex;
				d3d11DSViewDesc.Texture2DArray.ArraySize = 1;
				d3d11AttachmentSubResourceIndex = D3D11CalcSubresource(
						textureSubResource.uSubRes2DArray.mipLevel,
						textureSubResource.uSubRes2DArray.arrayIndex,
						dx11Texture->mTextureLayout.dimensions.mipLevelsNum );
			}
			else if( dx11Texture->mTextureLayout.texClass == ETextureClass::T2DMS )
			{
				d3d11DSViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
				d3d11AttachmentSubResourceIndex = D3D11CalcSubresource( 0, 0, 0 );
			}
			else if( dx11Texture->mTextureLayout.texClass == ETextureClass::T2DMSArray )
			{
				d3d11DSViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
				d3d11DSViewDesc.Texture2DMSArray.FirstArraySlice = textureSubResource.uSubRes2DArray.arrayIndex;
				d3d11DSViewDesc.Texture2DMSArray.ArraySize = 1;
				d3d11AttachmentSubResourceIndex = D3D11CalcSubresource( 0, textureSubResource.uSubRes2DArray.arrayIndex, 0 );
			}
			else if( dx11Texture->mTextureLayout.texClass == ETextureClass::TCubeMap )
			{
				d3d11DSViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
				d3d11DSViewDesc.Texture2DArray.MipSlice = textureSubResource.uSubResCubeMap.mipLevel;
				d3d11DSViewDesc.Texture2DArray.FirstArraySlice = textureSubResource.uSubResCubeMap.faceIndex;
				d3d11DSViewDesc.Texture2DArray.ArraySize = 1;
				d3d11AttachmentSubResourceIndex = D3D11CalcSubresource(
						textureSubResource.uSubResCubeMap.mipLevel,
						textureSubResource.uSubResCubeMap.faceIndex,
						dx11Texture->mTextureLayout.dimensions.mipLevelsNum );
			}
			else
			{
				Ic3DebugInterrupt();
				return {};
			}

			ComPtr<ID3D11DepthStencilView> d3d11ResourceDSView;
			auto hResult = pGPUDevice.mD3D11Device1->CreateDepthStencilView(
					d3d11AttachmentResource,
					&d3d11DSViewDesc,
					d3d11ResourceDSView.GetAddressOf() );

			if( FAILED( hResult ) )
			{
				Ic3DebugInterrupt();
				return {};
			}

			DX11RenderTargetDepthStencilAttachment dx11DepthStencilAttachment;
			dx11DepthStencilAttachment.d3d11Resource = d3d11AttachmentResource;
			dx11DepthStencilAttachment.d3d11SubResourceIndex = d3d11AttachmentSubResourceIndex;
			dx11DepthStencilAttachment.d3d11DSView = d3d11ResourceDSView;

			return dx11DepthStencilAttachment;
		}

		DX11RenderTargetResolveAttachment RTOCreateResolveAttachmentDX11(
				const TextureReference & pAttachmentTextureRef )
		{
			auto * dx11Texture = pAttachmentTextureRef->QueryInterface<DX11Texture>();
			const auto & textureSubResource = pAttachmentTextureRef.GetRefSubResource();

			ID3D11Resource * d3d11AttachmentResource = nullptr;
			DXGI_FORMAT targetDXGIFormat = DXGI_FORMAT_UNKNOWN;
			UINT d3d11AttachmentSubResourceIndex = 0;

			if( dx11Texture->mTextureLayout.texClass == ETextureClass::T3D )
			{
				auto * d3d11Texture3D = dx11Texture->mD3D11Texture3D.Get();
				Ic3DebugAssert( d3d11Texture3D );

				D3D11_TEXTURE3D_DESC texture3DDesc;
				d3d11Texture3D->GetDesc( &texture3DDesc );

				d3d11AttachmentResource = d3d11Texture3D;
				targetDXGIFormat = texture3DDesc.Format;
				d3d11AttachmentSubResourceIndex = D3D11CalcSubresource(
						textureSubResource.uSubRes3D.mipLevel,
						0,
						dx11Texture->mTextureLayout.dimensions.mipLevelsNum );
			}
			else
			{
				auto * d3d11Texture2D = dx11Texture->mD3D11Texture2D.Get();
				Ic3DebugAssert( d3d11Texture2D );

				D3D11_TEXTURE2D_DESC texture2DDesc;
				d3d11Texture2D->GetDesc( &texture2DDesc );

				d3d11AttachmentResource = d3d11Texture2D;
				targetDXGIFormat = texture2DDesc.Format;

				if( dx11Texture->mTextureLayout.texClass == ETextureClass::T2D )
				{
					d3d11AttachmentSubResourceIndex = D3D11CalcSubresource(
							textureSubResource.uSubRes2D.mipLevel,
							0,
							dx11Texture->mTextureLayout.dimensions.mipLevelsNum );

				}
				else if( dx11Texture->mTextureLayout.texClass == ETextureClass::T2DArray )
				{
					d3d11AttachmentSubResourceIndex = D3D11CalcSubresource(
							textureSubResource.uSubRes2DArray.mipLevel,
							textureSubResource.uSubRes2DArray.arrayIndex,
							dx11Texture->mTextureLayout.dimensions.mipLevelsNum );
				}
				else if( dx11Texture->mTextureLayout.texClass == ETextureClass::T2DMS )
				{
					d3d11AttachmentSubResourceIndex = D3D11CalcSubresource( 0, 0, 0 );
				}
				else if( dx11Texture->mTextureLayout.texClass == ETextureClass::T2DMSArray )
				{
					d3d11AttachmentSubResourceIndex = D3D11CalcSubresource( 0, textureSubResource.uSubRes2DArray.arrayIndex, 0 );
				}
				else if( dx11Texture->mTextureLayout.texClass == ETextureClass::TCubeMap )
				{
					d3d11AttachmentSubResourceIndex = D3D11CalcSubresource(
							textureSubResource.uSubResCubeMap.mipLevel,
							textureSubResource.uSubResCubeMap.faceIndex,
							dx11Texture->mTextureLayout.dimensions.mipLevelsNum );
				}
				else
				{
					Ic3DebugInterrupt();
					return {};
				}
			}

			DX11RenderTargetResolveAttachment dx11ResolveAttachment;
			dx11ResolveAttachment.d3d11Resource = d3d11AttachmentResource;
			dx11ResolveAttachment.d3d11SubResourceIndex = d3d11AttachmentSubResourceIndex;
			dx11ResolveAttachment.targetDXGIFormat = targetDXGIFormat;

			return dx11ResolveAttachment;
		}

		DX11RenderTargetBinding RTOCreateRenderTargetBindingDX11(
				DX11GPUDevice & pGPUDevice,
				const RenderTargetBinding & pRenderTargetBinding )
		{
			DX11RenderTargetBinding dx11RenderTargetBinding;
			RTOUpdateRenderTargetBindingDX11( pGPUDevice, pRenderTargetBinding, dx11RenderTargetBinding );
			return dx11RenderTargetBinding;
		}

		bool RTOUpdateRenderTargetBindingDX11(
			DX11GPUDevice & pGPUDevice,
			const RenderTargetBinding & pRenderTargetBinding,
			DX11RenderTargetBinding & pOutDX11RenderTargetBinding )
		{
			ForEachRTColorAttachmentIndex(
					pRenderTargetBinding.activeAttachmentsMask,
					[&]( native_uint pColorAttachmentIndex, ERTAttachmentFlags pAttachmentBit ) {
						auto * colorAttachmentBinding = pRenderTargetBinding.GetAttachment( pColorAttachmentIndex );
						auto & baseTextureReference = colorAttachmentBinding->baseTexture->mTargetTexture;
						
						auto dx11ColorAttachment = RTOCreateColorAttachmentDX11( pGPUDevice, baseTextureReference );

						pOutDX11RenderTargetBinding.colorAttachments[pColorAttachmentIndex] = dx11ColorAttachment;
						pOutDX11RenderTargetBinding.d3d11ColorAttachmentRTViewArray[pColorAttachmentIndex] = dx11ColorAttachment.d3d11RTView.Get();
						pOutDX11RenderTargetBinding.activeAttachmentsMask.set( pAttachmentBit );

						if( colorAttachmentBinding->resolveTexture )
						{
							auto dx11ResolveAttachmentTexture = RTOCreateResolveAttachmentDX11( colorAttachmentBinding->resolveTexture->mTargetTexture );
							pOutDX11RenderTargetBinding.resolveAttachments[pColorAttachmentIndex] = dx11ResolveAttachmentTexture;
						}

						return true;
					} );

			if( pRenderTargetBinding.activeAttachmentsMask.is_set( eRTAttachmentFlagDepthStencilBit ) )
			{
				auto * depthStencilAttachmentBinding = pRenderTargetBinding.GetAttachment( kRTOAttachmentIndexDepthStencil );
				auto & baseTextureReference = depthStencilAttachmentBinding->baseTexture->mTargetTexture;

				auto dx11DepthStencilAttachment = RTOCreateDepthStencilAttachmentDX11( pGPUDevice, baseTextureReference );

				pOutDX11RenderTargetBinding.depthStencilAttachment = dx11DepthStencilAttachment;
				pOutDX11RenderTargetBinding.d3d11DepthStencilAttachmentDSView = dx11DepthStencilAttachment.d3d11DSView.Get();
				pOutDX11RenderTargetBinding.activeAttachmentsMask.set( eRTAttachmentFlagDepthStencilBit );

				if( depthStencilAttachmentBinding->resolveTexture )
				{
					auto dx11ResolveAttachmentTexture = RTOCreateResolveAttachmentDX11( depthStencilAttachmentBinding->resolveTexture->mTargetTexture );
					pOutDX11RenderTargetBinding.resolveAttachments[kRTOAttachmentIndexDepthStencil] = dx11ResolveAttachmentTexture;
				}
			}

			pOutDX11RenderTargetBinding.activeAttachmentsMask = pRenderTargetBinding.activeAttachmentsMask;
			pOutDX11RenderTargetBinding.renderTargetLayout = pRenderTargetBinding.GetRenderTargetLayout();

			return !pOutDX11RenderTargetBinding.IsEmpty();
		}

		void RTORenderPassExecuteOpLoadClearDX11(
				ID3D11DeviceContext1 * pD3D11DeviceContext,
				const DX11RenderTargetBinding & pDX11RenderTargetBinding,
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicConfig & pDynamicConfig )
		{
			// Dynamic configuration overwrites the values specified via descriptors.
			// The activeStateMask member tells us which state override is enabled.
			const bool clearConfigOverride = pDynamicConfig.activeStateMask.is_set(
				eGraphicsPipelineDynamicConfigFlagRenderTargetClearConfigBit );

			const auto attachmentsLoadClearMask =
				pRenderPassConfiguration.GetAttachmentsMaskWithLoadFlags( eRenderPassAttachmentActionFlagLoadClearBit );

			auto clearedAttachmentsMask = cppx::make_bitmask_tp<ERTAttachmentFlags>();

			if( !attachmentsLoadClearMask.empty() )
			{
				ForEachRTColorAttachmentIndex(
						attachmentsLoadClearMask,
						[&]( native_uint pColorAttachmentIndex, ERTAttachmentFlags pAttachmentBit ) {
							const auto * attachmentConfig = pRenderPassConfiguration.GetAttachment( pColorAttachmentIndex );
							const auto * clearConfig = &( attachmentConfig->loadParameters.opClear.clearConfig );

							if( clearConfigOverride )
							{
								clearConfig = &( pDynamicConfig.renderTargetClearConfig );
							}

							auto * d3d11AttachmentRTView = pDX11RenderTargetBinding.d3d11ColorAttachmentRTViewArray[pColorAttachmentIndex];

							pD3D11DeviceContext->ClearRenderTargetView(
								d3d11AttachmentRTView,
								clearConfig->colorValue.uv_rgba );
							return true;
						} );

				if( attachmentsLoadClearMask.is_set( eRTAttachmentFlagDepthStencilBit ) )
				{
					const auto * attachmentConfig = pRenderPassConfiguration.GetAttachment( kRTOAttachmentIndexDepthStencil );
					const auto * clearConfig = &( attachmentConfig->loadParameters.opClear.clearConfig );

					if( clearConfigOverride )
					{
						clearConfig = &( pDynamicConfig.renderTargetClearConfig );
					}

					auto * d3d11AttachmentDSView = pDX11RenderTargetBinding.d3d11DepthStencilAttachmentDSView;
					const auto d3d11ClearMask = ATL::TranslateDX11RTClearDepthStencilFlags( attachmentConfig->loadParameters.opClear.clearMask );

					pD3D11DeviceContext->ClearDepthStencilView(
							d3d11AttachmentDSView,
							d3d11ClearMask,
							static_cast<FLOAT>( clearConfig->depthValue ),
							static_cast<UINT8>( clearConfig->stencilValue ) );
				}
			}
		}

		void RTORenderPassExecuteOpStoreResolveDX11(
				ID3D11DeviceContext1 * pD3D11DeviceContext,
				const DX11RenderTargetBinding & pRenderTargetBinding,
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicConfig & pDynamicConfig )
		{
			const auto attachmentsStoreResolveMask =
				pRenderPassConfiguration.GetAttachmentsMaskWithStoreFlags( eRenderPassAttachmentActionFlagStoreResolveBit );

			auto resolvedAttachmentsMask = cppx::make_bitmask_tp<ERTAttachmentFlags>();

			if( !attachmentsStoreResolveMask.empty() )
			{
				ForEachRTAttachmentIndex(
						pRenderTargetBinding.activeAttachmentsMask,
						[&]( native_uint pColorAttachmentIndex, ERTAttachmentFlags pAttachmentBit ) {
							const auto & sourceAttachment = pRenderTargetBinding.colorAttachments[pColorAttachmentIndex];
							const auto & destAttachment = pRenderTargetBinding.resolveAttachments[pColorAttachmentIndex];

							pD3D11DeviceContext->ResolveSubresource(
									destAttachment.d3d11Resource,
									destAttachment.d3d11SubResourceIndex,
									sourceAttachment.d3d11Resource,
									sourceAttachment.d3d11SubResourceIndex,
									destAttachment.targetDXGIFormat );
							return true;
						} );

				if( attachmentsStoreResolveMask.is_set( eRTAttachmentFlagDepthStencilBit ) )
				{
					const auto & sourceAttachment = pRenderTargetBinding.depthStencilAttachment;
					const auto & destAttachment = pRenderTargetBinding.resolveAttachments[kRTOAttachmentIndexDepthStencil];

					pD3D11DeviceContext->ResolveSubresource(
							destAttachment.d3d11Resource,
							destAttachment.d3d11SubResourceIndex,
							sourceAttachment.d3d11Resource,
							sourceAttachment.d3d11SubResourceIndex,
							destAttachment.targetDXGIFormat );
				}
			}
		}

	}
	
}
