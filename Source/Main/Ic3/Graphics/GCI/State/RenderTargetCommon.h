
#pragma once

#ifndef __IC3_GRAPHICS_GCI_RENDER_TARGET_COMMON_H__
#define __IC3_GRAPHICS_GCI_RENDER_TARGET_COMMON_H__

#include "CommonGraphicsConfig.h"
#include "RenderTargetArrayUtils.h"
#include "../Resources/TextureCommon.h"
#include <Ic3/System/Core/Visual.h>

namespace Ic3::Graphics::GCI
{

	struct RenderPassAttachmentConfig;
	struct RenderTargetAttachmentBinding;
	struct RenderTargetAttachmentLayout;

	Ic3GCIDeclareClassHandle( RenderTargetTexture );

	using RenderPassConfiguration = TRenderTargetArrayConfiguration<RenderPassAttachmentConfig>;
	using RenderTargetBinding = TRenderTargetArrayConfiguration<RenderTargetAttachmentBinding>;
	using RenderTargetLayout = TRenderTargetArrayConfiguration<RenderTargetAttachmentLayout>;

	enum ERenderTargetBufferFlags : uint32
	{
		eRenderTargetBufferFlagColorBit = 0x01,
		eRenderTargetBufferFlagDepthBit = 0x02,
		eRenderTargetBufferFlagStencilBit = 0x04,
		eRenderTargetBufferMaskDepthStencil = eRenderTargetBufferFlagDepthBit | eRenderTargetBufferFlagStencilBit,
		eRenderTargetBufferMaskAll = eRenderTargetBufferFlagColorBit | eRenderTargetBufferMaskDepthStencil,
	};

	/// @brief
	enum class ERenderTargetTextureType : uint32
	{
		Unknown = 0,
		RTColor = eRenderTargetBufferFlagColorBit,
		RTDepthOnly = eRenderTargetBufferFlagDepthBit,
		RTDepthStencil = eRenderTargetBufferMaskDepthStencil,
		RTStencilOnly = eRenderTargetBufferFlagStencilBit,
	};

	/**
	 *
	 */
	struct RenderTargetAttachmentBinding
	{
		RenderTargetTextureHandle baseTexture;

		RenderTargetTextureHandle resolveTexture;

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return IsActive();
		}

		CPPX_ATTR_NO_DISCARD bool Equals( const RenderTargetAttachmentBinding & pOther ) const noexcept
		{
			return ( baseTexture == pOther.baseTexture ) && ( resolveTexture == pOther.resolveTexture );
		}

		CPPX_ATTR_NO_DISCARD bool IsActive() const noexcept
		{
			return baseTexture ? true : false;
		}

		void Reset()
		{
			baseTexture.reset();
			resolveTexture.reset();
		}
	};

	/**
	 *
	 */
	struct RenderTargetAttachmentLayout
	{
		ETextureFormat format = ETextureFormat::Undefined;

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return IsValid();
		}

		CPPX_ATTR_NO_DISCARD bool Equals( const RenderTargetAttachmentLayout & pOther ) const noexcept
		{
			return format == pOther.format;
		}

		CPPX_ATTR_NO_DISCARD bool IsValid() const noexcept
		{
			return format != ETextureFormat::Undefined;
		}

		void Reset()
		{
			format = ETextureFormat::Undefined;
		}
	};

	template <>
	struct TRenderTargetArrayConfiguration<RenderTargetAttachmentBinding> : public TRenderTargetArrayConfigurationBase<RenderTargetAttachmentBinding>
	{
		IC3_GRAPHICS_GCI_API_NO_DISCARD RenderTargetLayout GetRenderTargetLayout() const noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD cppx::bitmask<ERTAttachmentFlags> GetResolveAttachmentsMask(
				cppx::bitmask<ERTAttachmentFlags> pFilterMask = eRTAttachmentMaskAll ) const noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD uint32 GetResolveAttachmentsNum(
				cppx::bitmask<ERTAttachmentFlags> pFilterMask = eRTAttachmentMaskAll ) const noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD cppx::bitmask<ERTAttachmentFlags> GetResolveColorAttachmentsMask() const noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD uint32 GetResolveColorAttachmentsNum() const noexcept;
	};

	template <>
	struct TRenderTargetArrayConfiguration<RenderTargetAttachmentLayout> : public TRenderTargetArrayConfigurationBase<RenderTargetAttachmentLayout>
	{
		TextureSize2D sharedImageSize = cxTextureSize2DUndefined;

		MultiSamplingSettings sharedMultiSamplingSettings;
	};

	struct RenderTargetDescriptorCreateInfo : public PipelineStateDescriptorCreateInfoBase
	{
		RenderTargetBinding rtArrayBinding;
	};

	namespace Utilities
	{

		IC3_GRAPHICS_GCI_API_NO_DISCARD EGPUResourceUsageFlags RTOGetAttachmentRequiredUsageMask(
				native_uint pAttachmentIndex,
				cppx::bitmask<ERenderTargetBufferFlags> pOptionalRTAMask = 0 );

		IC3_GRAPHICS_GCI_API_NO_DISCARD cppx::bitmask<ERenderTargetBufferFlags> RTOGetBufferMaskForRenderTargetTextureType(
				ERenderTargetTextureType pRenderTargetTextureType );

		IC3_GRAPHICS_GCI_API_NO_DISCARD RenderTargetLayout RTOGetRenderTargetLayoutForBinding(
			const RenderTargetBinding & pRenderTargetBinding );

		IC3_GRAPHICS_GCI_API_NO_DISCARD RenderTargetLayout TranslateSystemVisualConfigToRenderTargetLayout(
			const System::VisualConfig & pSysVisualConfig );

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool RTOValidateRenderTargetBinding( const RenderTargetBinding & pRenderTargetBinding );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_RENDER_TARGET_COMMON_H__
