
#include "DX11RenderTarget.h"
#include "../DX11ApiTranslationLayer.h"
#include "../DX11gpuDevice.h"
#include "../Resources/DX11Texture.h"
#include <Ic3/Graphics/GCI/Resources/RenderTargetTexture.h>

namespace Ic3::Graphics::GCI
{

	DX11RenderTargetBindingImmutableState::DX11RenderTargetBindingImmutableState(
			DX11GPUDevice & pGPUDevice,
			const RenderTargetLayout & pRenderTargetLayout,
			DX11RenderTargetBindingData pDX11RTBindingData )
	: RenderTargetBindingImmutableState( pGPUDevice, pRenderTargetLayout )
	, mDX11RTBindingData( std::move( pDX11RTBindingData ) )
	{}

	DX11RenderTargetBindingImmutableState::~DX11RenderTargetBindingImmutableState() = default;

	GpaHandle<DX11RenderTargetBindingImmutableState> DX11RenderTargetBindingImmutableState::CreateInstance(
			DX11GPUDevice & pGPUDevice,
			const RenderTargetBindingDefinition & pBindingDefinition )
	{
		const auto renderTargetLayout = SMU::GetRenderTargetLayoutForBindingDefinition( pBindingDefinition );

		auto dx11RTBindingData = SMU::CreateRenderTargetBindingDataDX11( pGPUDevice, pBindingDefinition );

		auto immutableState = CreateGfxObject<DX11RenderTargetBindingImmutableState>(
				pGPUDevice,
				renderTargetLayout,
				std::move( dx11RTBindingData ) );

		return immutableState;
	}

	GpaHandle<DX11RenderTargetBindingImmutableState> DX11RenderTargetBindingImmutableState::CreateForScreen(
			DX11GPUDevice & pGPUDevice,
			ComPtr<ID3D11Texture2D> pColorBuffer,
			ComPtr<ID3D11Texture2D> pDepthStencilBuffer )
	{
		const auto renderTargetLayout = SMU::GetRenderTargetLayoutForScreenDX11( pColorBuffer.Get(), pDepthStencilBuffer.Get() );

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

		DX11RenderTargetBindingData dx11RTBindingData;
		mem_set_zero( dx11RTBindingData.d3d11ColorAttachmentRTViewArray );
		dx11RTBindingData.activeAttachmentsMask = E_RT_ATTACHMENT_MASK_DEFAULT_C0_DS;
		dx11RTBindingData.colorAttachments[0].d3d11RTView = colorBufferRTView;
		dx11RTBindingData.colorAttachments[0].d3d11Resource = pColorBuffer.Get();
		dx11RTBindingData.colorAttachments[0].d3d11SubResourceIndex = 0;
		dx11RTBindingData.d3d11ColorAttachmentRTViewArray[0] = colorBufferRTView.Get();
		dx11RTBindingData.depthStencilAttachment.d3d11DSView = depthStencilBufferDSView;
		dx11RTBindingData.depthStencilAttachment.d3d11Resource = pDepthStencilBuffer.Get();
		dx11RTBindingData.depthStencilAttachment.d3d11SubResourceIndex = 0;
		dx11RTBindingData.d3d11DepthStencilAttachmentDSView = depthStencilBufferDSView.Get();

		auto immutableState = CreateGfxObject<DX11RenderTargetBindingImmutableState>(
				pGPUDevice,
				renderTargetLayout,
				std::move( dx11RTBindingData ) );

		return immutableState;
	}


	namespace SMU
	{

		RenderTargetLayout GetRenderTargetLayoutForScreenDX11(
				ID3D11Texture2D * pColorBuffer,
				ID3D11Texture2D * pDepthStencilBuffer )
		{
			D3D11_TEXTURE2D_DESC colorBufferTextureDesc;
			pColorBuffer->GetDesc( & colorBufferTextureDesc );

			D3D11_TEXTURE2D_DESC depthStencilBufferTextureDesc;
			pDepthStencilBuffer->GetDesc( & depthStencilBufferTextureDesc );

			RenderTargetLayout renderTargetLayout;
			renderTargetLayout.activeAttachmentsMask = E_RT_ATTACHMENT_MASK_DEFAULT_C0_DS;
			renderTargetLayout.sharedImageRect.width = colorBufferTextureDesc.Width;
			renderTargetLayout.sharedImageRect.height = colorBufferTextureDesc.Height;
			renderTargetLayout.sharedMSAALevel = colorBufferTextureDesc.SampleDesc.Count;
			renderTargetLayout.colorAttachments[0].format = ATL::TranslateDXTextureFormatInv( colorBufferTextureDesc.Format );
			renderTargetLayout.depthStencilAttachment.format = ATL::TranslateDXTextureFormatInv( depthStencilBufferTextureDesc.Format );

			return renderTargetLayout;
		}

		DX11RenderTargetColorAttachment CreateRenderTargetColorAttachmentDX11(
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

		DX11RenderTargetDepthStencilAttachment CreateRenderTargetDepthStencilAttachmentDX11(
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

		DX11RenderTargetResolveAttachment CreateRenderTargetResolveAttachmentDX11(
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

			return dx11ResolveAttachment;
		}

		DX11RenderTargetBindingData CreateRenderTargetBindingDataDX11(
				DX11GPUDevice & pGPUDevice,
				const RenderTargetBindingDefinition & pBindingDefinition )
		{
			DX11RenderTargetBindingData dx11RenderTargetBindingData;

			for( native_uint caIndex = 0; caIndex < GCM::RT_MAX_COLOR_ATTACHMENTS_NUM; ++caIndex )
			{
				const auto attachmentBit = CxDef::makeRTAttachmentFlag( caIndex );
				if( pBindingDefinition.activeAttachmentsMask.is_set( attachmentBit ) )
				{
					if( const auto & attachmentBinding = pBindingDefinition.colorAttachments[caIndex] )
					{
						auto & colorAttachmentTextureRef = attachmentBinding.attachmentTexture->mTargetTexture;
						auto dx11ColorAttachment = SMU::CreateRenderTargetColorAttachmentDX11( pGPUDevice, colorAttachmentTextureRef );

						dx11RenderTargetBindingData.colorAttachments[caIndex] = dx11ColorAttachment;
						dx11RenderTargetBindingData.d3d11ColorAttachmentRTViewArray[caIndex] = dx11ColorAttachment.d3d11RTView.Get();
						dx11RenderTargetBindingData.activeAttachmentsMask.set( attachmentBit );

						if( pBindingDefinition.attachmentsActionResolveMask.is_set( attachmentBit ) )
						{
							Ic3DebugAssert( attachmentBinding.resolveTexture );

							auto dx11ResolveAttachmentTexture = CreateRenderTargetResolveAttachmentDX11( attachmentBinding.resolveTexture->mTargetTexture );
							dx11RenderTargetBindingData.resolveAttachments[caIndex] = dx11ResolveAttachmentTexture;
						}
					}
				}
				else
				{
					dx11RenderTargetBindingData.d3d11ColorAttachmentRTViewArray[caIndex] = nullptr;
				}
			}

			if( pBindingDefinition.activeAttachmentsMask.is_set( E_RT_ATTACHMENT_FLAG_DEPTH_STENCIL_BIT ) )
			{
				if( const auto & attachmentBinding = pBindingDefinition.depthStencilAttachment )
				{
					auto & dsAttachmentTextureRef = attachmentBinding.attachmentTexture->mTargetTexture;
					auto dx11DepthStencilAttachment = SMU::CreateRenderTargetDepthStencilAttachmentDX11( pGPUDevice, dsAttachmentTextureRef );

					dx11RenderTargetBindingData.depthStencilAttachment = dx11DepthStencilAttachment;
					dx11RenderTargetBindingData.d3d11DepthStencilAttachmentDSView = dx11DepthStencilAttachment.d3d11DSView.Get();
					dx11RenderTargetBindingData.activeAttachmentsMask.set( E_RT_ATTACHMENT_FLAG_DEPTH_STENCIL_BIT );

					if( pBindingDefinition.attachmentsActionResolveMask.is_set( E_RT_ATTACHMENT_FLAG_DEPTH_STENCIL_BIT ) )
					{
						Ic3DebugAssert( attachmentBinding.resolveTexture );

						auto dx11ResolveAttachmentTexture = CreateRenderTargetResolveAttachmentDX11( attachmentBinding.resolveTexture->mTargetTexture );
						dx11RenderTargetBindingData.resolveAttachments[eRTAttachmentIndexDepthStencil] = dx11ResolveAttachmentTexture;
					}
				}
			}
			else
			{
				dx11RenderTargetBindingData.d3d11DepthStencilAttachmentDSView = nullptr;
			}

			return dx11RenderTargetBindingData;
		}

		void RenderPassClearRenderTargetDX11(
				ID3D11DeviceContext1 * pD3D1DeviceContext,
				const DX11RenderTargetBindingData & pRenderTargetBinding,
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicState & pDynamicState )
		{
			ForEachRTColorAttachmentIndex(
					pRenderTargetBinding.activeAttachmentsMask,
					[&]( native_uint pIndex, ERTAttachmentFlags pAttachmentBit )
					{
						if( pRenderPassConfiguration.attachmentsActionClearMask.is_set( pAttachmentBit ) )
						{
							auto * d3d11AttachmentRTView = pRenderTargetBinding.d3d11ColorAttachmentRTViewArray[pIndex];
							const auto & clearConfig = pRenderPassConfiguration.colorAttachments[pIndex].clearConfig;

							pD3D1DeviceContext->ClearRenderTargetView(
									d3d11AttachmentRTView,
									clearConfig.colorValue.mRGBA );
						}
						return true;
					} );

			if( pRenderPassConfiguration.attachmentsActionClearMask.is_set( E_RT_ATTACHMENT_FLAG_DEPTH_STENCIL_BIT ) )
			{
				auto * d3d11AttachmentDSView = pRenderTargetBinding.d3d11DepthStencilAttachmentDSView;
				const auto & clearConfig = pRenderPassConfiguration.depthStencilAttachment.clearConfig;
				const auto d3d11ClearMask = ATL::TranslateDX11RTClearDepthStencilFlags( pRenderPassConfiguration.depthStencilAttachment.clearMask );

				pD3D1DeviceContext->ClearDepthStencilView(
						d3d11AttachmentDSView,
						d3d11ClearMask,
						static_cast<FLOAT>( clearConfig.depthValue ),
						static_cast<UINT8>( clearConfig.stencilValue ) );
			}
		}

		void RenderPassResolveRenderTargetDX11(
				ID3D11DeviceContext1 * pD3D1DeviceContext,
				const DX11RenderTargetBindingData & pRenderTargetBinding,
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicState & pDynamicState )
		{
			ForEachRTAttachmentIndex(
					pRenderTargetBinding.activeAttachmentsMask,
					[&]( native_uint pIndex, ERTAttachmentFlags pAttachmentBit )
					{
						if( pRenderPassConfiguration.attachmentsActionResolveMask.is_set( pAttachmentBit ) )
						{
							const auto & sourceAttachment = pRenderTargetBinding.colorAttachments[pIndex];
							const auto & destAttachment = pRenderTargetBinding.resolveAttachments[pIndex];

							pD3D1DeviceContext->ResolveSubresource(
									destAttachment.d3d11Resource,
									destAttachment.d3d11SubResourceIndex,
									sourceAttachment.d3d11Resource,
									sourceAttachment.d3d11SubResourceIndex,
									destAttachment.targetDXGIFormat );
						}
						return true;
					} );

			if( pRenderPassConfiguration.attachmentsActionResolveMask.is_set( E_RT_ATTACHMENT_FLAG_DEPTH_STENCIL_BIT ) )
			{
				auto * d3d11AttachmentDSView = pRenderTargetBinding.d3d11DepthStencilAttachmentDSView;
				const auto & clearConfig = pRenderPassConfiguration.depthStencilAttachment.clearConfig;
				const auto d3d11ClearMask = ATL::TranslateDX11RTClearDepthStencilFlags( pRenderPassConfiguration.depthStencilAttachment.clearMask );

				pD3D1DeviceContext->ClearDepthStencilView(
						d3d11AttachmentDSView,
						d3d11ClearMask,
						static_cast<FLOAT>( clearConfig.depthValue ),
						static_cast<UINT8>( clearConfig.stencilValue ) );
			}
		}

	}
	
}
