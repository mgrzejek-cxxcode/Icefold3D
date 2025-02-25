
#include "RenderTargetCommon.h"
#include <Ic3/Graphics/GCI/Resources/Texture.h>
#include <Ic3/Graphics/GCI/Resources/RenderTargetTexture.h>
#include <cppx/memory.h>

namespace Ic3::Graphics::GCI
{

	RenderTargetLayout TRenderTargetArrayConfiguration<RenderTargetAttachmentBinding>::GetRenderTargetLayout() const noexcept
	{
		return GCU::RTOGetRenderTargetLayoutForBinding( *this );
	}

	cppx::bitmask<ERTAttachmentFlags> TRenderTargetArrayConfiguration<RenderTargetAttachmentBinding>::GetResolveAttachmentsMask() const noexcept
	{
		cppx::bitmask<ERTAttachmentFlags> resolveAttachmentsMask = 0;

		GCU::ForEachRTAttachmentIndex(
			activeAttachmentsMask,
			[&]( native_uint pAttachmentIndex, ERTAttachmentFlags pAttachmentBit ) -> bool {
				if( attachments[pAttachmentIndex].resolveTexture )
				{
					resolveAttachmentsMask.set( pAttachmentBit );
				}
				return true;
			});

		return resolveAttachmentsMask;
	}

	namespace GCU
	{

		EGPUResourceUsageFlags RTOGetAttachmentRequiredUsageMask(
				native_uint pAttachmentIndex,
				cppx::bitmask<ERenderTargetBufferFlags> pOptionalRTAMask )
		{
			if( pAttachmentIndex >= GCM::kRTOMaxCombinedAttachmentsNum )
			{
				// Returned mask is used for validation - we check whether the specified resource has valid usage
				// configured, so that it can be attached to the pipeline as an RT attachment. Thus, in case of an
				// invalid index we return the complete mask with all usages - then, even in naive, unchecked code
				// like: if( someResource.HasUsage( RTOGetAttachmentRequiredUsageMask( eRTIndexDepthStencil ) ) )
				// the behaviour will be correct (in the case above the check will fail - but returning 0, for example,
				// would make this check successful).
				return eGPUResourceUsageMaskAll;
			}
			else
			{
				if( pAttachmentIndex < GCM::kRTOMaxColorAttachmentsNum )
				{
					// If the query is for one of the color attachments, we need eGPUResourceUsageFlagRenderTargetColorBit.
					return eGPUResourceUsageFlagRenderTargetColorBit;
				}
				else
				{
					if( pOptionalRTAMask.is_set( eRenderTargetBufferMaskDepthStencil ) )
					{
						// Depth-stencil usage requires eGPUResourceUsageMaskRenderTargetDepthStencil.
						return eGPUResourceUsageMaskRenderTargetDepthStencil;
					}
					else if( pOptionalRTAMask.is_set( eRenderTargetBufferFlagDepthBit ) )
					{
						// Depth-only usage requires eGPUResourceUsageFlagRenderTargetDepthBit.
						return eGPUResourceUsageFlagRenderTargetDepthBit;
					}
					else if( pOptionalRTAMask.is_set( eRenderTargetBufferFlagStencilBit ) )
					{
						// Stencil-only usage requires eGPUResourceUsageFlagRenderTargetStencilBit.
						return eGPUResourceUsageFlagRenderTargetStencilBit;
					}
				}
			}
		}

		cppx::bitmask<ERenderTargetBufferFlags> RTOGetBufferMaskForRenderTargetTextureType( ERenderTargetTextureType pRenderTargetTextureType )
		{
			return static_cast<uint32>( pRenderTargetTextureType ) & eRenderTargetBufferMaskAll;
		}

		RenderTargetLayout RTOGetRenderTargetLayoutForBinding( const RenderTargetBinding & pRenderTargetBinding )
		{
			RenderTargetLayout renderTargetLayout{};

			const auto bindingValid = ForEachRTAttachmentIndex( pRenderTargetBinding.activeAttachmentsMask,
				[&]( native_uint pIndex, ERTAttachmentFlags pAttachmentBit )
				{
					if( const auto & attachmentBinding = pRenderTargetBinding.attachments[pIndex] )
					{
						if( renderTargetLayout.sharedImageSize == cxTextureSize2DUndefined )
						{
							renderTargetLayout.sharedImageSize = attachmentBinding.baseTexture->mRTTextureLayout.imageRect;
							renderTargetLayout.sharedMultiSamplingSettings.sampleCount = attachmentBinding.baseTexture->mRTTextureLayout.msaaLevel;
							renderTargetLayout.sharedMultiSamplingSettings.sampleQuality = 1;
						}
						else if( renderTargetLayout.sharedImageSize != attachmentBinding.baseTexture->mRTTextureLayout.imageRect )
						{
							renderTargetLayout.sharedImageSize = cxTextureSize2DUndefined;
							return false;
						}
						renderTargetLayout.attachments[pIndex].format = attachmentBinding.baseTexture->mRTTextureLayout.internalFormat;
					}
					return true;
				});

			if( bindingValid )
			{
				renderTargetLayout.activeAttachmentsMask = pRenderTargetBinding.activeAttachmentsMask;
				renderTargetLayout.activeAttachmentsNum = pRenderTargetBinding.activeAttachmentsNum;
			}

			return renderTargetLayout;
		}

		RenderTargetLayout TranslateSystemVisualConfigToRenderTargetLayout( const System::VisualConfig & pSysVisualConfig )
		{
			ETextureFormat colorAttachmentFormat = ETextureFormat::Undefined;
			switch( pSysVisualConfig.colorFormat )
			{
				case System::EColorFormat::B8G8R8:       colorAttachmentFormat = ETextureFormat::BGRX8UN;   break;
				case System::EColorFormat::B8G8R8A8:     colorAttachmentFormat = ETextureFormat::BGRA8UN;   break;
				case System::EColorFormat::B8G8R8A8SRGB: colorAttachmentFormat = ETextureFormat::BGRA8SRGB; break;
				case System::EColorFormat::B8G8R8X8:     colorAttachmentFormat = ETextureFormat::BGRX8UN;   break;
				case System::EColorFormat::R8G8B8A8:     colorAttachmentFormat = ETextureFormat::RGBA8UN;   break;
				case System::EColorFormat::R8G8B8A8SRGB: colorAttachmentFormat = ETextureFormat::RGBA8SRGB; break;
				case System::EColorFormat::R8G8B8X8:     colorAttachmentFormat = ETextureFormat::RGBX8UN;   break;
				case System::EColorFormat::R10G10B10A2:  colorAttachmentFormat = ETextureFormat::BGRX8UN;   break;

				default:
					break;
			}

			ETextureFormat depthStencilAttachmentFormat = ETextureFormat::Undefined;
			switch( pSysVisualConfig.depthStencilFormat )
			{
				case System::EDepthStencilFormat::D16:    depthStencilAttachmentFormat = ETextureFormat::D16UN;    break;
				case System::EDepthStencilFormat::D24S8:  depthStencilAttachmentFormat = ETextureFormat::D24UNS8U; break;
				case System::EDepthStencilFormat::D24X8:  depthStencilAttachmentFormat = ETextureFormat::D24UNX8;  break;
				case System::EDepthStencilFormat::D32F:   depthStencilAttachmentFormat = ETextureFormat::D32F;     break;
				default:
					break;
			}

			RenderTargetLayout renderTargetLayout;
			renderTargetLayout.activeAttachmentsMask = eRTAttachmentMaskDefaultC0DS;
			renderTargetLayout.activeAttachmentsNum = 2;
			renderTargetLayout.colorAttachments[0].format = colorAttachmentFormat;
			renderTargetLayout.depthStencilAttachment.format = depthStencilAttachmentFormat;

			return renderTargetLayout;
		}

		bool RTOValidateRenderTargetBinding( const RenderTargetBinding & pRenderTargetBinding )
		{
			if( const auto * firstActiveAttachmentRef = pRenderTargetBinding.FindFirstActiveAttachment() )
			{
				// Image Layout of the first attachment. This will serve as a reference point - all RT attachments
				// are required to have the same layout (dimensions), so we can compare to any of the active ones.
				const auto & commonImageLayout = firstActiveAttachmentRef->baseTexture->mRTTextureLayout;

				auto attachmentsValid = ForEachRTAttachmentIndex( pRenderTargetBinding.activeAttachmentsMask,
					[&]( native_uint pAttachmentIndex, ERTAttachmentFlags pAttachmentBit )
					{
						const auto & attachmentRef = pRenderTargetBinding.attachments[pAttachmentIndex];
						if( !attachmentRef )
						{
							return false;
						}
						const auto requiredUsageFlags = GCU::RTOGetAttachmentRequiredUsageMask( pAttachmentIndex );
						if( !attachmentRef.baseTexture->mResourceFlags.is_set( requiredUsageFlags ) )
						{
							return false;
						}
						const auto & textureLayout = attachmentRef.baseTexture->mRTTextureLayout;
						if( textureLayout.imageRect != commonImageLayout.imageRect )
						{
							return false;
						}
						if( textureLayout.msaaLevel != commonImageLayout.msaaLevel )
						{
							return false;
						}
						return true;
					} );

				if( attachmentsValid )
				{
					return true;
				}
			}

			return false;
		}

	}
/*
	void RenderTargetBindingDefinition::ResetAttachmentsFlags() noexcept
	{
		attachmentsActionResolveMask = 0;

		ForEachRTAttachmentIndex( activeAttachmentsMask,
			[&]( native_uint pIndex, ERTAttachmentFlags pAttachmentBit )
			{
				const auto & attachmentBinding = attachments[pIndex];
				if( !attachmentBinding )
				{
					activeAttachmentsMask.unset( pAttachmentBit );
				}
				else
				{
					if( attachmentBinding.resolveTexture )
					{
						attachmentsActionResolveMask.set( pAttachmentBit );
					}
				}
				return true;
			} );
	}


	namespace GCU
	{

		const RenderTargetAttachmentBinding * GetRenderTargetBindingDefinitionFirstTarget(
				const RenderTargetBindingDefinition & pBindingDefinition )
		{
			const RenderTargetAttachmentBinding * firstBinding = nullptr;

			for( const auto & attachmentBinding : pBindingDefinition.attachments )
			{
				if( attachmentBinding )
				{
					firstBinding = &attachmentBinding;
					break;
				}
			}

			return firstBinding;
		}

		bool ValidateRenderTargetBindingDefinition( const RenderTargetBindingDefinition & pBindingDefinition )
		{
			if( const auto * firstBinding = GetRenderTargetBindingDefinitionFirstTarget( pBindingDefinition ) )
			{
				const auto & commonImageLayout = firstBinding->attachmentTexture->mRTTextureLayout;

				const auto bindingValid = ForEachRTAttachmentIndex( pBindingDefinition.activeAttachmentsMask,
					[&]( native_uint pIndex, ERTAttachmentFlags pAttachmentBit )
					{
						const auto & attachmentBinding = pBindingDefinition.attachments[pIndex];
						if( !attachmentBinding )
						{
							return false;
						}
						const auto requiredUsageFlags = CXU::GetRTAttachmentRequiredUsageMask( pIndex );
						if( !attachmentBinding.attachmentTexture->mResourceFlags.is_set_any_of( requiredUsageFlags ) )
						{
							return false;
						}
						const auto & textureLayout = attachmentBinding.attachmentTexture->mRTTextureLayout;
						if( textureLayout.imageRect != commonImageLayout.imageRect )
						{
							return false;
						}
						if( textureLayout.msaaLevel != commonImageLayout.msaaLevel )
						{
							return false;
						}
						if( pBindingDefinition.attachmentsActionResolveMask.is_set( pAttachmentBit ) && !attachmentBinding.resolveTexture )
						{
							return false;
						}
						return true;
					} );

				if( bindingValid )
				{
					return true;
				}
			}

			return false;
		}

		RenderTargetLayout GetRenderTargetLayoutForBindingDefinition(
				const RenderTargetBindingDefinition & pBindingDefinition )
		{
			if( const auto * firstBinding = GetRenderTargetBindingDefinitionFirstTarget( pBindingDefinition ) )
			{
				const auto & commonLayout = firstBinding->attachmentTexture->mRTTextureLayout;

				RenderTargetLayout renderTargetLayout{};
				renderTargetLayout.activeAttachmentsMask = 0;
				renderTargetLayout.sharedImageRect = commonLayout.imageRect;
				renderTargetLayout.sharedMSAALevel = commonLayout.msaaLevel;

				const auto layoutValid = ForEachRTAttachmentIndex( pBindingDefinition.activeAttachmentsMask,
					[&]( native_uint pIndex, ERTAttachmentFlags pAttachmentBit )
					{
						const auto & attachmentBinding = pBindingDefinition.attachments[pIndex];
						if( attachmentBinding.IsEmpty() )
						{
							return false;
						}

						const auto & caTexture = attachmentBinding.attachmentTexture;
						const auto & caLayout = caTexture->mRTTextureLayout;

						if((caLayout.imageRect != commonLayout.imageRect ) || (caLayout.msaaLevel != commonLayout.msaaLevel ) )
						{
							return false;
						}

						renderTargetLayout.attachments[pIndex].format = caLayout.internalFormat;
						renderTargetLayout.activeAttachmentsMask.set( pAttachmentBit );

						return true;
					} );

				if( layoutValid )
				{
					return renderTargetLayout;
				}
			}

			return RenderTargetLayout{};
		}
	}

	namespace defaults
	{

		RenderTargetLayout GetRenderTargetLayoutDefaultBGRA8()
		{
			RenderTargetLayout rtLayout{};
			rtLayout.activeAttachmentsMask = eRTAttachmentFlagColor0Bit;
			rtLayout.sharedImageRect = cxTextureSize2DUndefined;
			rtLayout.sharedMSAALevel = cxTextureMSAALevelUndefined;
			rtLayout.colorAttachments[0].format = ETextureFormat::BGRA8UN;
			return rtLayout;
		}

		RenderTargetLayout GetRenderTargetLayoutDefaultBGRA8D24S8()
		{
			RenderTargetLayout rtLayout{};
			rtLayout.activeAttachmentsMask = eRTAttachmentFlagColor0Bit | eRTAttachmentFlagDepthStencilBit;
			rtLayout.sharedImageRect = cxTextureSize2DUndefined;
			rtLayout.sharedMSAALevel = cxTextureMSAALevelUndefined;
			rtLayout.colorAttachments[0].format = ETextureFormat::BGRA8UN;
			rtLayout.depthStencilAttachment.format = ETextureFormat::D24UNS8U;
			return rtLayout;
		}

		RenderTargetLayout GetRenderTargetLayoutDefaultRGBA8()
		{
			RenderTargetLayout rtLayout{};
			rtLayout.activeAttachmentsMask = eRTAttachmentFlagColor0Bit;
			rtLayout.sharedImageRect = cxTextureSize2DUndefined;
			rtLayout.sharedMSAALevel = cxTextureMSAALevelUndefined;
			rtLayout.colorAttachments[0].format = ETextureFormat::RGBA8UN;
			return rtLayout;
		}

		RenderTargetLayout GetRenderTargetLayoutDefaultRGBA8D24S8()
		{
			RenderTargetLayout rtLayout{};
			rtLayout.activeAttachmentsMask = eRTAttachmentFlagColor0Bit | eRTAttachmentFlagDepthStencilBit;
			rtLayout.sharedImageRect = cxTextureSize2DUndefined;
			rtLayout.sharedMSAALevel = cxTextureMSAALevelUndefined;
			rtLayout.colorAttachments[0].format = ETextureFormat::RGBA8UN;
			rtLayout.depthStencilAttachment.format = ETextureFormat::D24UNS8U;
			return rtLayout;
		}

		const RenderTargetLayout cvRenderTargetLayoutDefaultBGRA8 = GetRenderTargetLayoutDefaultBGRA8();

		const RenderTargetLayout cvRenderTargetLayoutDefaultBGRA8D24S8 = GetRenderTargetLayoutDefaultBGRA8D24S8();

		const RenderTargetLayout cvRenderTargetLayoutDefaultRGBA8 = GetRenderTargetLayoutDefaultRGBA8();

		const RenderTargetLayout cvRenderTargetLayoutDefaultRGBA8D24S8 = GetRenderTargetLayoutDefaultRGBA8D24S8();
	
	}
*/

//	bool CreateRenderTargetLayout( const RenderTargetLayoutDesc & pRTLayoutDesc,
//	                               RenderTargetLayout & pOutRTLayout )
//	{
//		RenderTargetLayout renderTargetLayout;
//
//		for( const auto & attachmentLayoutDesc : pRTLayoutDesc.attachmentLayoutDescArray )
//		{
//			if( attachmentLayoutDesc )
//			{
//				auto attachmentIndex = static_cast<uint32>( attachmentLayoutDesc.attachmentID );
//				if( attachmentIndex < GCM::RT_MAX_COLOR_ATTACHMENTS_NUM )
//				{
//					renderTargetLayout.colorAttachmentArray[attachmentIndex].format = attachmentLayoutDesc.format;
//					renderTargetLayout.colorAttachmentActiveCount += 1;
//					renderTargetLayout.attachmentMask.set( eRTAttachmentFlagColor0Bit << attachmentIndex );
//				}
//				else
//				{
//					renderTargetLayout.depthStencilAttachment.format = attachmentLayoutDesc.format;
//					renderTargetLayout.attachmentMask.set( E_RENDER_TARGET_ATTACHMENT_FLAGS_DEPTH_STENCIL );
//				}
//			}
//		}
//
//		if( renderTargetLayout.attachmentMask == 0 )
//		{
//			return false;
//		}
//
//		mem_copy( pOutRTLayout, renderTargetLayout );
//
//		return true;
//	}
//
//	RenderTargetLayout CreateRenderTargetLayout( const RenderTargetLayoutDesc & pRTLayoutDesc )
//	{
//		RenderTargetLayout renderTargetLayout;
//		CreateRenderTargetLayout( pRTLayoutDesc, renderTargetLayout );
//		return renderTargetLayout;
//	}
//
//	static bool validateRenderBufferRef( RenderTargetResourceBinding & pRTResourceBinding, const RTARenderBufferRef & pRenderBufferRef );
//	static bool validateTextureRef( RenderTargetResourceBinding & pRTResourceBinding, const RTATextureRef & pTextureRef );
//
//	bool CreateRenderTargetLayoutAndResourceBinding( const RenderTargetResourceBindingDesc & pRTResourceBindingDesc,
//	                                                 RenderTargetLayout & pOutRTLayout,
//	                                                 RenderTargetResourceBinding & pOutRTResourceBinding )
//	{
//		RenderTargetLayout renderTargetLayout;
//		RenderTargetResourceBinding rtResourceBinding;
//
//		for( const auto & attachmentResourceBindingDesc : pRTResourceBindingDesc.attachmentResourceBindingDescArray )
//		{
//			if( renderTargetLayout.colorAttachmentActiveCount + renderTargetLayout.depthStencilAttachmentState == pRTResourceBindingDesc.activeBindingsNum )
//			{
//				break;
//			}
//
//			if( attachmentResourceBindingDesc )
//			{
//				auto attachmentIndex = static_cast<uint32>( attachmentResourceBindingDesc.attachmentID );
//				if( attachmentIndex >= GCM::RT_MAX_COMBINED_ATTACHMENTS_NUM )
//				{
//					Ic3DebugInterrupt();
//					return false;
//				}
//
//				RenderTargetAttachmentLayout * attachmentLayoutPtr = nullptr;
//				RenderTargetAttachmentResourceBinding * attachmentResourceBindingPtr = nullptr;
//
//				if( attachmentIndex < GCM::RT_MAX_COLOR_ATTACHMENTS_NUM )
//				{
//					attachmentLayoutPtr = &( renderTargetLayout.colorAttachmentArray[attachmentIndex] );
//					attachmentResourceBindingPtr = &( rtResourceBinding.colorAttachmentArray[attachmentIndex] );
//					renderTargetLayout.colorAttachmentActiveCount += 1;
//					renderTargetLayout.attachmentMask.set( eRTAttachmentFlagColor0Bit << attachmentIndex );
//				}
//				else
//				{
//					attachmentLayoutPtr = &( renderTargetLayout.depthStencilAttachment );
//					attachmentResourceBindingPtr = &( rtResourceBinding.depthStencilAttachment );
//					renderTargetLayout.attachmentMask.set( E_RENDER_TARGET_ATTACHMENT_FLAGS_DEPTH_STENCIL );
//				}
//
//				if( attachmentResourceBindingDesc.attachmentResourceType == ERenderTargetResourceType::RenderBuffer )
//				{
//					if( !validateRenderBufferRef( rtResourceBinding, attachmentResourceBindingDesc.renderBufferRef ) )
//					{
//						return false;
//					}
//
//					auto * renderBuffer = attachmentResourceBindingDesc.renderBufferRef.renderBuffer;
//					attachmentLayoutPtr->format = renderBuffer->mRenderBufferLayout.internalFormat;
//					attachmentResourceBindingPtr->attachmentResourceType = ERenderTargetResourceType::RenderBuffer;
//					attachmentResourceBindingPtr->format = attachmentLayoutPtr->format;
//					attachmentResourceBindingPtr->uRenderBufferRef.renderBuffer = renderBuffer;
//				}
//				else
//				{
//					if( !validateTextureRef( rtResourceBinding, attachmentResourceBindingDesc.textureRef ) )
//					{
//						return false;
//					}
//
//					auto * texture = attachmentResourceBindingDesc.textureRef.texture;
//					attachmentLayoutPtr->format = texture->mTextureLayout.pixelFormat;
//					attachmentResourceBindingPtr->attachmentResourceType = ERenderTargetResourceType::Texture;
//					attachmentResourceBindingPtr->format = attachmentLayoutPtr->format;
//					attachmentResourceBindingPtr->uTextureRef.texture = texture;
//					attachmentResourceBindingPtr->uTextureRef.textureSubResource = attachmentResourceBindingDesc.textureRef.textureSubResource;
//				}
//			}
//		}
//
//		if( renderTargetLayout.attachmentMask == 0 )
//		{
//			return false;
//		}
//
//		mem_copy( pOutRTLayout, renderTargetLayout );
//		mem_copy( pOutRTResourceBinding, rtResourceBinding );
//
//		return true;
//	}
//
//	bool checkRenderTargetLayoutCompatibility( const RenderTargetResourceBinding & pRTResourceBinding,
//	                                           const RenderTargetLayout & pRTLayout )
//	{
//
//		for( uint32 caIndex = 0; caIndex < GCM::RT_MAX_COLOR_ATTACHMENTS_NUM; ++caIndex )
//		{
//			auto & caResourceBinding = pRTResourceBinding.colorAttachmentArray[caIndex];
//			auto & caLayout = pRTLayout.colorAttachmentArray[caIndex];
//
//			if( ( caResourceBinding && !caLayout ) || ( !caResourceBinding && caLayout ) )
//			{
//				return false;
//			}
//
//			if( caResourceBinding && caLayout )
//			{
//				if( caResourceBinding.format != caLayout.format )
//				{
//					return false;
//				}
//			}
//		}
//
//		auto & dsaResourceBinding = pRTResourceBinding.depthStencilAttachment;
//		auto & dsaLayout = pRTLayout.depthStencilAttachment;
//
//		if( ( dsaResourceBinding && !dsaLayout ) || ( !dsaResourceBinding && dsaLayout ) )
//		{
//			return false;
//		}
//
//		if( dsaResourceBinding && dsaLayout )
//		{
//			if( dsaResourceBinding.format != dsaLayout.format )
//			{
//				return false;
//			}
//		}
//
//		return true;
//	}
//
//
//	bool validateRenderBufferRef( RenderTargetResourceBinding & pRTResourceBinding, const RTARenderBufferRef & pRenderBufferRef )
//	{
//		if( !pRenderBufferRef.renderBuffer )
//		{
//			return false;
//		}
//
//		if( pRTResourceBinding.commonMSAALevel == CX_RT_BUFFER_MSAA_LEVEL_INVALID )
//		{
//			pRTResourceBinding.commonBufferSize = pRenderBufferRef.renderBuffer->mRenderBufferLayout.bufferSize;
//			pRTResourceBinding.commonMSAALevel = pRenderBufferRef.renderBuffer->mRenderBufferLayout.msaaLevel;
//
//			return true;
//		}
//
//		if( pRenderBufferRef.renderBuffer->mRenderBufferLayout.msaaLevel != pRTResourceBinding.commonMSAALevel )
//		{
//			return false;
//		}
//
//		if( ( pRenderBufferRef.renderBuffer->mRenderBufferLayout.bufferSize.width != pRTResourceBinding.commonBufferSize.width ) ||
//		    ( pRenderBufferRef.renderBuffer->mRenderBufferLayout.bufferSize.height != pRTResourceBinding.commonBufferSize.height ) )
//		{
//			return false;
//		}
//
//		return true;
//	}
//
//	bool validateTextureRef( RenderTargetResourceBinding & pRTResourceBinding, const RTATextureRef & pTextureRef )
//	{
//		if( !pTextureRef.texture )
//		{
//			return false;
//		}
//
//		const auto & textureLayout = pTextureRef.texture->mTextureLayout;
//
//		if( pRTResourceBinding.commonMSAALevel == CX_RT_BUFFER_MSAA_LEVEL_INVALID )
//		{
//			pRTResourceBinding.commonBufferSize.width = textureLayout.dimensions.width;
//			pRTResourceBinding.commonBufferSize.height = textureLayout.dimensions.height;
//			pRTResourceBinding.commonMSAALevel = textureLayout.msaaLevel;
//
//			return true;
//		}
//
//		if( textureLayout.msaaLevel != pRTResourceBinding.commonMSAALevel )
//		{
//			return false;
//		}
//
//		if( ( textureLayout.dimensions.width != pRTResourceBinding.commonBufferSize.width ) ||
//		    ( textureLayout.dimensions.height != pRTResourceBinding.commonBufferSize.height ) )
//		{
//			return false;
//		}
//
//		if( !TextureUtils::validateTextureSubResource( textureLayout.texClass, textureLayout.dimensions, pTextureRef.textureSubResource ) )
//		{
//			return false;
//		}
//
//		return true;
//	}

} // namespace Ic3::Graphics::GCI
