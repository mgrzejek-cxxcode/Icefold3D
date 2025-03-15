
#include "DX11GraphicsPipelineStateRTO.h"
#include <Ic3/Graphics/GCI/Resources/RenderTargetTexture.h>
#include <Ic3/Graphics/HW3D/DX11/DX11APITranslationLayer.h>
#include <Ic3/Graphics/HW3D/DX11/DX11GPUDevice.h>
#include <Ic3/Graphics/HW3D/DX11/Resources/DX11Texture.h>

namespace Ic3::Graphics::GCI
{

	DX11RenderTargetDescriptor::DX11RenderTargetDescriptor(
			DX11GPUDevice & pGPUDevice,
			DX11RenderTargetBindingPtr pDX11RenderTargetBinding )
	: HW3DPipelineStateDescriptor( pGPUDevice )
	, mDX11RenderTargetBinding( std::move( pDX11RenderTargetBinding ) )
	{}

	DX11RenderTargetDescriptor::~DX11RenderTargetDescriptor() = default;

	bool DX11RenderTargetDescriptor::IsAttachmentConfigured( native_uint pAttachmentIndex ) const noexcept
	{
		return mDX11RenderTargetBinding->activeAttachmentsMask.is_set( CXU::RTOMakeAttachmentFlag( pAttachmentIndex ) );
	}

	TGfxHandle<DX11RenderTargetDescriptor> DX11RenderTargetDescriptor::CreateInstance(
			DX11GPUDevice & pGPUDevice,
			const RenderTargetBinding & pRenderTargetBinding )
	{
		auto dx11RenderTargetBinding = Utilities::RTOCreateRenderTargetBindingDX11( pGPUDevice, pRenderTargetBinding );
		auto dx11RenderTargetDescriptor = CreateGfxObject<DX11RenderTargetDescriptor>( pGPUDevice, std::move( dx11RenderTargetBinding ) );

		return dx11RenderTargetDescriptor;
	}

	TGfxHandle<DX11RenderTargetDescriptor> DX11RenderTargetDescriptor::CreateForScreen(
			DX11GPUDevice & pGPUDevice,
			ComPtr<ID3D11Texture2D> pColorBuffer,
			ComPtr<ID3D11Texture2D> pDepthStencilBuffer )
	{

		auto dx11RenderTargetBinding = Utilities::RTOCreateRenderTargetBindingForScreenDX11( pGPUDevice, pColorBuffer, pDepthStencilBuffer );
		auto dx11RenderTargetDescriptor = CreateGfxObject<DX11RenderTargetDescriptor>( pGPUDevice, std::move( dx11RenderTargetBinding ) );

		return dx11RenderTargetDescriptor;
	}


	namespace Utilities
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

		DX11RenderTargetBindingPtr RTOCreateRenderTargetBindingDX11(
				DX11GPUDevice & pGPUDevice,
				const RenderTargetBinding & pRenderTargetBinding )
		{
			const auto activeColorOutputAttachmentsNum = pRenderTargetBinding.GetActiveColorAttachmentsNum();
			const auto activeColorResolveAttachmentsNum = pRenderTargetBinding.GetResolveColorAttachmentsNum();

			auto dx11RenderTargetBinding = MakeUniqueDynamicallySizedTemplate<DX11RenderTargetBinding>( activeColorOutputAttachmentsNum, activeColorResolveAttachmentsNum );

			RTOUpdateRenderTargetBindingDX11( pGPUDevice, pRenderTargetBinding, *dx11RenderTargetBinding );

			return dx11RenderTargetBinding;
		}

		DX11RenderTargetBindingPtr RTOCreateRenderTargetBindingForScreenDX11(
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

			auto dx11RenderTargetBinding = MakeUniqueDynamicallySizedTemplate<DX11RenderTargetBinding>( 1, 0 );

			dx11RenderTargetBinding->activeAttachmentsMask = eRTAttachmentMaskDefaultC0DS;

			dx11RenderTargetBinding->SetColorOutputAttachmentIndexMapping( 0, 0 );
			dx11RenderTargetBinding->colorOutputAttachmentBindings[0].d3d11RTV = colorBufferRTView;
			dx11RenderTargetBinding->colorOutputAttachmentBindings[0].d3d11Resource = pColorBuffer.Get();
			dx11RenderTargetBinding->colorOutputAttachmentBindings[0].d3d11SubResourceIndex = 0;

			dx11RenderTargetBinding->depthStencilOutputAttachmentBinding.d3d11DSV = depthStencilBufferDSView;
			dx11RenderTargetBinding->depthStencilOutputAttachmentBinding.d3d11Resource = pDepthStencilBuffer.Get();
			dx11RenderTargetBinding->depthStencilOutputAttachmentBinding.d3d11SubResourceIndex = 0;

			return dx11RenderTargetBinding;
		}

		bool RTOUpdateRenderTargetBindingDX11(
				DX11GPUDevice & pGPUDevice,
				const RenderTargetBinding & pRenderTargetBinding,
				DX11RenderTargetBindingBase & pOutDX11RenderTargetBinding )
		{
			uint32 activeColorOutputAttachmentsNum = 0;
			uint32 activeColorResolveAttachmentsNum = 0;

			for( uint32 colorAttachmentIndex = 0; CXU::RTOIsColorAttachmentIndexValid( colorAttachmentIndex ); ++colorAttachmentIndex )
			{
				const auto * colorAttachmentBinding = pRenderTargetBinding.GetAttachment( colorAttachmentIndex );
				if( colorAttachmentBinding && pRenderTargetBinding.IsColorAttachmentActive( colorAttachmentIndex ) )
				{
					const auto colorAttachmentBit = CXU::RTOMakeColorAttachmentFlag( colorAttachmentIndex );
					const auto & baseTextureReference = colorAttachmentBinding->baseTexture->mTargetTexture;
					Ic3DebugAssert( baseTextureReference );

					auto dx11ColorOutputAttachment = RTOCreateColorAttachmentDX11( pGPUDevice, baseTextureReference );
					pOutDX11RenderTargetBinding.colorOutputAttachmentBindings[activeColorOutputAttachmentsNum] = dx11ColorOutputAttachment;
					pOutDX11RenderTargetBinding.colorOutputAttachmentFormats[activeColorOutputAttachmentsNum] = baseTextureReference->mTextureLayout.internalFormat;
					pOutDX11RenderTargetBinding.SetColorOutputAttachmentIndexMapping( colorAttachmentIndex, activeColorOutputAttachmentsNum );
					pOutDX11RenderTargetBinding.activeAttachmentsMask.set( colorAttachmentBit );

					++activeColorOutputAttachmentsNum;

					if( auto & resolveTextureReference = colorAttachmentBinding->resolveTexture->mTargetTexture )
					{
						auto dx11ColorResolveAttachment = RTOCreateResolveAttachmentDX11( resolveTextureReference );
						pOutDX11RenderTargetBinding.colorResolveAttachmentBindings[activeColorResolveAttachmentsNum] = dx11ColorResolveAttachment;
						pOutDX11RenderTargetBinding.SetColorResolveAttachmentIndexMapping( colorAttachmentIndex, activeColorResolveAttachmentsNum );

						++activeColorResolveAttachmentsNum;
					}
					else
					{
						pOutDX11RenderTargetBinding.SetColorResolveAttachmentIndexInactive( colorAttachmentIndex );
					}
				}
				else
				{
					pOutDX11RenderTargetBinding.SetColorOutputAttachmentIndexInactive( colorAttachmentIndex );
				}
			}

			const auto & depthStencilAttachmentBinding = pRenderTargetBinding.depthStencilAttachment;
			if( depthStencilAttachmentBinding && pRenderTargetBinding.IsDepthStencilAttachmentActive() )
			{
				const auto & baseTextureReference = depthStencilAttachmentBinding.baseTexture->mTargetTexture;
				Ic3DebugAssert( baseTextureReference );

				auto dx11DepthStencilOutputAttachment = RTOCreateDepthStencilAttachmentDX11( pGPUDevice, baseTextureReference );
				pOutDX11RenderTargetBinding.depthStencilOutputAttachmentBinding = dx11DepthStencilOutputAttachment;
				pOutDX11RenderTargetBinding.depthStencilOutputAttachmentFormat = baseTextureReference->mTextureLayout.internalFormat;
				pOutDX11RenderTargetBinding.activeAttachmentsMask.set( eRTAttachmentFlagDepthStencilBit );

				++activeColorOutputAttachmentsNum;

				if( auto & resolveTextureReference = depthStencilAttachmentBinding.resolveTexture->mTargetTexture )
				{
					auto dx11DepthStencilResolveAttachment = RTOCreateResolveAttachmentDX11( resolveTextureReference );
					pOutDX11RenderTargetBinding.depthStencilResolveAttachmentBinding = dx11DepthStencilResolveAttachment;
				}
			}

			return !pOutDX11RenderTargetBinding.IsEmpty();
		}

		DX11RenderTargetColorAttachmentRef RTOCreateColorAttachmentDX11(
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

			ComPtr<ID3D11RenderTargetView> d3d11ResourceRTV;
			auto hResult = pGPUDevice.mD3D11Device1->CreateRenderTargetView(
					d3d11AttachmentResource,
					&d3d11RTViewDesc,
					d3d11ResourceRTV.GetAddressOf() );

			if( FAILED( hResult ) )
			{
				Ic3DebugInterrupt();
				return {};
			}

			DX11RenderTargetColorAttachmentRef dx11ColorAttachmentRef;
			dx11ColorAttachmentRef.d3d11Resource = d3d11AttachmentResource;
			dx11ColorAttachmentRef.d3d11SubResourceIndex = d3d11AttachmentSubResourceIndex;
			dx11ColorAttachmentRef.d3d11RTV = d3d11ResourceRTV;

			return dx11ColorAttachmentRef;
		}

		DX11RenderTargetDepthStencilAttachmentRef RTOCreateDepthStencilAttachmentDX11(
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

			ComPtr<ID3D11DepthStencilView> d3d11ResourceDSV;
			auto hResult = pGPUDevice.mD3D11Device1->CreateDepthStencilView(
					d3d11AttachmentResource,
					&d3d11DSViewDesc,
					d3d11ResourceDSV.GetAddressOf() );

			if( FAILED( hResult ) )
			{
				Ic3DebugInterrupt();
				return {};
			}

			DX11RenderTargetDepthStencilAttachmentRef dx11DepthStencilAttachmentRef;
			dx11DepthStencilAttachmentRef.d3d11Resource = d3d11AttachmentResource;
			dx11DepthStencilAttachmentRef.d3d11SubResourceIndex = d3d11AttachmentSubResourceIndex;
			dx11DepthStencilAttachmentRef.d3d11DSV = d3d11ResourceDSV;

			return dx11DepthStencilAttachmentRef;
		}

		DX11RenderTargetResolveAttachmentRef RTOCreateResolveAttachmentDX11(
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

			DX11RenderTargetResolveAttachmentRef dx11ResolveAttachmentRef;
			dx11ResolveAttachmentRef.d3d11Resource = d3d11AttachmentResource;
			dx11ResolveAttachmentRef.d3d11SubResourceIndex = d3d11AttachmentSubResourceIndex;
			dx11ResolveAttachmentRef.targetDXGIFormat = targetDXGIFormat;

			return dx11ResolveAttachmentRef;
		}

		cppx::bitmask<ERTAttachmentFlags> RTORenderPassExecuteOpLoadClearDX11(
				ID3D11DeviceContext1 * pD3D11DeviceContext,
				const DX11RenderTargetBindingBase & pDX11RenderTargetBinding,
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
						[&]( native_uint pColorAttachmentIndex, ERTAttachmentFlags pColorAttachmentBit ) {
							const auto * colorAttachmentRenderPassConfig = pRenderPassConfiguration.GetAttachment( pColorAttachmentIndex );
							const auto * clearConfig = &( colorAttachmentRenderPassConfig->loadParameters.opClear.clearConfig );

							if( clearConfigOverride )
							{
								clearConfig = &( pDynamicConfig.renderTargetClearConfig );
							}

							auto * dx11ColorOutputAttachmentRef = pDX11RenderTargetBinding.GetBindingForColorOutputAttachment( pColorAttachmentIndex );

							pD3D11DeviceContext->ClearRenderTargetView(
									dx11ColorOutputAttachmentRef->d3d11RTV.Get(),
									clearConfig->colorValue.uv_rgba );

							clearedAttachmentsMask.set( pColorAttachmentBit );

							return true;
						} );

				if( attachmentsLoadClearMask.is_set( eRTAttachmentFlagDepthStencilBit ) )
				{
					const auto * depthStencilAttachmentRenderPassConfig = pRenderPassConfiguration.GetAttachment( kRTOAttachmentIndexDepthStencil );
					const auto * clearConfig = &( depthStencilAttachmentRenderPassConfig->loadParameters.opClear.clearConfig );

					if( clearConfigOverride )
					{
						clearConfig = &( pDynamicConfig.renderTargetClearConfig );
					}


					auto & dx11DepthStencilOutputAttachmentRef = pDX11RenderTargetBinding.depthStencilOutputAttachmentBinding;
					const auto d3d11ClearMask = ATL::TranslateDX11RTClearDepthStencilFlags( depthStencilAttachmentRenderPassConfig->loadParameters.opClear.clearMask );

					pD3D11DeviceContext->ClearDepthStencilView(
							dx11DepthStencilOutputAttachmentRef.d3d11DSV.Get(),
							d3d11ClearMask,
							static_cast<FLOAT>( clearConfig->depthValue ),
							static_cast<UINT8>( clearConfig->stencilValue ) );

					clearedAttachmentsMask.set( eRTAttachmentFlagDepthStencilBit );
				}
			}

			return clearedAttachmentsMask;
		}

		cppx::bitmask<ERTAttachmentFlags> RTORenderPassExecuteOpStoreResolveDX11(
				ID3D11DeviceContext1 * pD3D11DeviceContext,
				const DX11RenderTargetBindingBase & pDX11RenderTargetBinding,
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicConfig & pDynamicConfig )
		{
			const auto attachmentsStoreResolveMask =
				pRenderPassConfiguration.GetAttachmentsMaskWithStoreFlags( eRenderPassAttachmentActionFlagStoreResolveBit );

			auto resolvedAttachmentsMask = cppx::make_bitmask_tp<ERTAttachmentFlags>();

			if( !attachmentsStoreResolveMask.empty() )
			{
				ForEachRTAttachmentIndex(
						attachmentsStoreResolveMask,
						[&]( native_uint pColorAttachmentIndex, ERTAttachmentFlags pColorAttachmentBit ) {
							const auto * dx11ColorOutputAttachmentRef = pDX11RenderTargetBinding.GetBindingForColorOutputAttachment( pColorAttachmentIndex );
							const auto * dx11ColorResolveAttachmentRef = pDX11RenderTargetBinding.GetBindingForColorResolveAttachment( pColorAttachmentIndex );

							pD3D11DeviceContext->ResolveSubresource(
									dx11ColorResolveAttachmentRef->d3d11Resource,
									dx11ColorResolveAttachmentRef->d3d11SubResourceIndex,
									dx11ColorOutputAttachmentRef->d3d11Resource,
									dx11ColorOutputAttachmentRef->d3d11SubResourceIndex,
									dx11ColorResolveAttachmentRef->targetDXGIFormat );

							resolvedAttachmentsMask.set( pColorAttachmentBit );

							return true;
						} );

				if( attachmentsStoreResolveMask.is_set( eRTAttachmentFlagDepthStencilBit ) )
				{
					const auto & dx11DepthStencilOutputAttachmentRef = pDX11RenderTargetBinding.depthStencilOutputAttachmentBinding;
					const auto & dx11DepthStencilResolveAttachmentRef = pDX11RenderTargetBinding.depthStencilResolveAttachmentBinding;

					pD3D11DeviceContext->ResolveSubresource(
							dx11DepthStencilResolveAttachmentRef.d3d11Resource,
							dx11DepthStencilResolveAttachmentRef.d3d11SubResourceIndex,
							dx11DepthStencilOutputAttachmentRef.d3d11Resource,
							dx11DepthStencilOutputAttachmentRef.d3d11SubResourceIndex,
							dx11DepthStencilResolveAttachmentRef.targetDXGIFormat );

					resolvedAttachmentsMask.set( eRTAttachmentFlagDepthStencilBit );
				}
			}

			return resolvedAttachmentsMask;
		}

	}
	
}
