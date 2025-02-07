
#pragma once

#ifndef __IC3_GRAPHICS_GCI_RENDER_TARGET_COMMON_H__
#define __IC3_GRAPHICS_GCI_RENDER_TARGET_COMMON_H__

#include "RenderTargetArrayUtils.h"
#include "../Resources/TextureCommon.h"

namespace Ic3::Graphics::GCI
{

	Ic3DeclareClassHandle( RenderTargetTexture );

	enum ERenderTargetBufferFlags : uint32
	{
		ERenderTargetBufferFlagColorBit = 0x01,
		ERenderTargetBufferFlagDepthBit = 0x02,
		ERenderTargetBufferFlagStencilBit = 0x04,
		ERenderTargetBufferMaskDepthStencil = ERenderTargetBufferFlagDepthBit | ERenderTargetBufferFlagStencilBit,
		ERenderTargetBufferMaskAll = ERenderTargetBufferFlagColorBit | ERenderTargetBufferMaskDepthStencil,
	};

	/// @brief
	enum class ERenderTargetTextureType : uint32
	{
		Unknown = 0,
		RTColor = ERenderTargetBufferFlagColorBit,
		RTDepthOnly = ERenderTargetBufferFlagDepthBit,
		RTDepthStencil = ERenderTargetBufferMaskDepthStencil,
		RTStencilOnly = ERenderTargetBufferFlagStencilBit,
	};

	/**
	 *
	 */
	struct RenderTargetAttachmentLayout
	{
		ETextureFormat format = ETextureFormat::Undefined;

		explicit operator bool() const noexcept
		{
			return IsValid();
		}

		bool IsValid() const noexcept
		{
			return format != ETextureFormat::Undefined;
		}

		void Reset()
		{
			format = ETextureFormat::Undefined;
		}
	};

	/**
	 *
	 */
	struct RenderTargetArrayLayout : public TRenderTargetArrayConfiguration<RenderTargetAttachmentLayout>
	{
		TextureSize2D sharedImageSize = cxTextureSize2DUndefined;

		uint32 sharedMSAALevel = 0;
	};

	namespace defaults
	{

		IC3_GRAPHICS_GCI_OBJ const RenderTargetArrayLayout cvRenderTargetLayoutDefaultBGRA8;
		IC3_GRAPHICS_GCI_OBJ const RenderTargetArrayLayout cvRenderTargetLayoutDefaultBGRA8D24S8;
		IC3_GRAPHICS_GCI_OBJ const RenderTargetArrayLayout cvRenderTargetLayoutDefaultRGBA8;
		IC3_GRAPHICS_GCI_OBJ const RenderTargetArrayLayout cvRenderTargetLayoutDefaultRGBA8D24S8;

	}

	namespace GCU
	{

		CPPX_ATTR_NO_DISCARD EGPUResourceUsageFlags RTOGetAttachmentRequiredUsageMask(
				native_uint pAttachmentIndex,
				cppx::bitmask<ERenderTargetBufferFlags> pOptionalRTAMask = 0 );

		CPPX_ATTR_NO_DISCARD cppx::bitmask<ERenderTargetBufferFlags> RTOGetBufferMaskForRenderTargetTextureType(
				ERenderTargetTextureType pRenderTargetTextureType );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_RENDER_TARGET_COMMON_H__
