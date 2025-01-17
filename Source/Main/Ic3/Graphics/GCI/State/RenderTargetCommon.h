
#pragma once

#ifndef __IC3_GRAPHICS_GCI_RENDER_TARGET_COMMON_H__
#define __IC3_GRAPHICS_GCI_RENDER_TARGET_COMMON_H__

#include "CommonGPUStateDefs.h"
#include "../Resources/TextureCommon.h"
#include <cppx/bitUtils.h>

namespace Ic3::Graphics::GCI
{

	Ic3DeclareClassHandle( RenderTargetTexture );

	enum ERenderTargetBufferFlags : uint32
	{
		E_RENDER_TARGET_BUFFER_FLAG_COLOR_BIT = 0x01,
		E_RENDER_TARGET_BUFFER_FLAG_DEPTH_BIT = 0x02,
		E_RENDER_TARGET_BUFFER_FLAG_STENCIL_BIT = 0x04,

		E_RENDER_TARGET_BUFFER_MASK_DEPTH_STENCIL =
			E_RENDER_TARGET_BUFFER_FLAG_DEPTH_BIT |
			E_RENDER_TARGET_BUFFER_FLAG_STENCIL_BIT,

		E_RENDER_TARGET_BUFFER_MASK_ALL =
			E_RENDER_TARGET_BUFFER_FLAG_COLOR_BIT |
			E_RENDER_TARGET_BUFFER_MASK_DEPTH_STENCIL,
	};

	/// @brief
	enum class ERenderTargetTextureType : uint32
	{
		Unknown = 0,
		RTColor = E_RENDER_TARGET_BUFFER_FLAG_COLOR_BIT,
		RTDepthOnly = E_RENDER_TARGET_BUFFER_FLAG_DEPTH_BIT,
		RTDepthStencil = E_RENDER_TARGET_BUFFER_MASK_DEPTH_STENCIL,
		RTStencilOnly = E_RENDER_TARGET_BUFFER_FLAG_STENCIL_BIT,
	};

	namespace CxDef
	{

		/// @brief
		CPPX_ATTR_NO_DISCARD inline constexpr cppx::bitmask<uint32> GetRTBufferMaskForRenderTargetTextureType(
				ERenderTargetTextureType pRenderTargetTextureType )
		{
			return static_cast<uint32>( pRenderTargetTextureType ) & E_RENDER_TARGET_BUFFER_MASK_ALL;
		}

		/// @brief
		CPPX_ATTR_NO_DISCARD inline constexpr uint32 GetRTAttachmentRequiredUsageMask( native_uint pAttachmentIndex )
		{
			return
					( pAttachmentIndex < GCM::cxRTMaxColorAttachmentsNum ) ?
					eGPUResourceUsageFlagRenderTargetColorBit :
					eGPUResourceUsageMaskRenderTargetDepthStencil;
		}

	}

	template <typename TAttachmentProperty>
	using RenderTargetColorAttachmentPropertyArray = std::array<TAttachmentProperty, GCM::cxRTMaxColorAttachmentsNum>;

	template <typename TAttachmentProperty>
	struct RenderTargetAttachmentPropertySet
	{
		using AttachmentPropertyArray = std::array<TAttachmentProperty, GCM::cxRTMaxCombinedAttachmentsNum>;

		AttachmentPropertyArray attachments;

		cppx::bitmask<ERTAttachmentFlags> activeAttachmentsMask = 0;

		cppx::array_view<TAttachmentProperty> const colorAttachments;

		TAttachmentProperty & depthStencilAttachment;

		RenderTargetAttachmentPropertySet()
		: colorAttachments( cppx::bind_array_view( attachments.data(), GCM::cxRTMaxColorAttachmentsNum ) )
		, depthStencilAttachment( attachments[eRTAttachmentIndexDepthStencil] )
		{}

		RenderTargetAttachmentPropertySet( const RenderTargetAttachmentPropertySet<TAttachmentProperty> & pSource )
		: attachments( pSource.attachments )
		, activeAttachmentsMask( pSource.activeAttachmentsMask )
		, colorAttachments( cppx::bind_array_view( attachments.data(), GCM::cxRTMaxColorAttachmentsNum ) )
		, depthStencilAttachment( attachments[eRTAttachmentIndexDepthStencil] )
		{}

		RenderTargetAttachmentPropertySet & operator=( const RenderTargetAttachmentPropertySet<TAttachmentProperty> & pRhs )
		{
			attachments = pRhs.attachments;
			activeAttachmentsMask = pRhs.activeAttachmentsMask;
			return *this;
		}

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept
		{
			return activeAttachmentsMask.empty();
		}

		CPPX_ATTR_NO_DISCARD uint32 CountActiveColorAttachments() const noexcept
		{
			return cppx::pop_count( static_cast<uint32>( activeAttachmentsMask & eRTAttachmentMaskColorAll ) );
		}

		CPPX_ATTR_NO_DISCARD bool IsColorAttachmentActive( uint32 pAttachmentIndex ) const noexcept
		{
			const auto attachmentBit = CxDef::makeRTAttachmentFlag( pAttachmentIndex );
			return ( attachmentBit != 0 ) && activeAttachmentsMask.is_set( attachmentBit );
		}

		CPPX_ATTR_NO_DISCARD bool IsDepthStencilAttachmentActive() const noexcept
		{
			return activeAttachmentsMask.is_set( eRtAttachmentFlagDepthStencilBit );
		}

		CPPX_ATTR_NO_DISCARD bool HasAnyColorAttachmentsActive() const noexcept
		{
			return (activeAttachmentsMask & eRTAttachmentMaskColorAll) != 0;
		}
	};

	template <typename TAttachmentProperty>
	struct RenderTargetAttachmentConfigurationSet : public RenderTargetAttachmentPropertySet<TAttachmentProperty>
	{
		cppx::bitmask<ERTAttachmentFlags> attachmentsActionResolveMask = 0;

		CPPX_ATTR_NO_DISCARD uint32 CountAttachmentsActionResolve() const noexcept
		{
			return cppx::pop_count( static_cast<uint32>( attachmentsActionResolveMask & eRTAttachmentMaskColorAll ) );
		}
	};

	/// @brief
	struct RenderTargetAttachmentBinding
	{
		RenderTargetTextureHandle attachmentTexture;
		RenderTargetTextureHandle resolveTexture;

		void Reset()
		{
			attachmentTexture.reset();
			resolveTexture.reset();
		}

		bool IsEmpty() const noexcept
		{
			return !attachmentTexture;
		}

		explicit operator bool() const noexcept
		{
			return !IsEmpty();
		}
	};

	using RenderTargetColorAttachmentBindingArray = RenderTargetColorAttachmentPropertyArray<RenderTargetAttachmentBinding>;

	struct RenderTargetBindingDefinition : public RenderTargetAttachmentConfigurationSet<RenderTargetAttachmentBinding>
	{
		CPPX_ATTR_NO_DISCARD RenderTargetBindingDefinition GetValidated() const noexcept
		{
			RenderTargetBindingDefinition validatedDefinition = *this;
			validatedDefinition.ResetAttachmentsFlags();
			return validatedDefinition;
		}

		IC3_GRAPHICS_GCI_API void ResetAttachmentsFlags() noexcept;
	};

	/// @brief
	struct RenderTargetAttachmentLayout
	{
		ETextureFormat format = ETextureFormat::Undefined;

		void Reset()
		{
			format = ETextureFormat::Undefined;
		}

		bool IsValid() const noexcept
		{
			return format != ETextureFormat::Undefined;
		}

		explicit operator bool() const noexcept
		{
			return IsValid();
		}
	};

	/// @brief A definition of a vertex layout used to Create a driver-specific RenderTargetLayout object.
	struct RenderTargetLayout : public RenderTargetAttachmentPropertySet<RenderTargetAttachmentLayout>
	{
		TextureSize2D sharedImageRect = cxTextureSize2DUndefined;

		uint32 sharedMSAALevel = 0;
	};

	namespace SMU
	{

		IC3_GRAPHICS_GCI_API_NO_DISCARD const RenderTargetAttachmentBinding * GetRenderTargetBindingDefinitionFirstTarget(
				const RenderTargetBindingDefinition & pBindingDefinition );

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool ValidateRenderTargetBindingDefinition(
				const RenderTargetBindingDefinition & pBindingDefinition );

		IC3_GRAPHICS_GCI_API_NO_DISCARD RenderTargetLayout GetRenderTargetLayoutForBindingDefinition(
				const RenderTargetBindingDefinition & pBindingDefinition );

	}

	namespace defaults
	{

		IC3_GRAPHICS_GCI_OBJ const RenderTargetLayout cvRenderTargetLayoutDefaultBGRA8;
		IC3_GRAPHICS_GCI_OBJ const RenderTargetLayout cvRenderTargetLayoutDefaultBGRA8D24S8;
		IC3_GRAPHICS_GCI_OBJ const RenderTargetLayout cvRenderTargetLayoutDefaultRGBA8;
		IC3_GRAPHICS_GCI_OBJ const RenderTargetLayout cvRenderTargetLayoutDefaultRGBA8D24S8;

	}

	template <typename TFunction>
	inline bool ForEachRTAttachmentIndex( cppx::bitmask<ERTAttachmentFlags> pActiveAttachmentsMask, TFunction pFunction )
	{
		// A local copy of the active attachments mask. Bits of already processed attachments
		// are removed, so when the value reaches 0, we can immediately stop further processing.
		auto activeAttachmentsMask = pActiveAttachmentsMask;

		for( // Iterate using RTA (Render Target Attachment) index value.
			native_uint attachmentIndex = 0;
			// Stop after reaching the limit or when there are no active attachments to process.
			CxDef::IsRTAttachmentIndexValid( attachmentIndex ) && !activeAttachmentsMask.empty();
			// This is rather self-descriptive, but it looked bad without this third comment here :)
			++attachmentIndex )
		{
			const auto attachmentBit = CxDef::makeRTAttachmentFlag( attachmentIndex );
			// Check if the attachments mask has this bit set.
			if( activeAttachmentsMask.is_set( attachmentBit ) )
			{
				// The function returns false if there was some internal error condition
				// and the processing should be aborted.
				if( !pFunction( attachmentIndex, cppx::make_bitmask_tp<ERTAttachmentFlags>( attachmentBit ) ) )
				{
					return false;
				}

				// Update the control mask.
				activeAttachmentsMask.unset( attachmentBit );
			}
		}

		return true;
	}

	template <typename TFunction>
	inline bool ForEachRTColorAttachmentIndex( cppx::bitmask<ERTAttachmentFlags> pActiveAttachmentsMask, TFunction pFunction )
	{
		// A local copy of the active attachments mask. Bits of already processed attachments
		// are removed, so when the value reaches 0, we can immediately stop further processing.
		auto activeAttachmentsMask = pActiveAttachmentsMask;

		for( // Iterate using RTA (Render Target Attachment) index value.
			native_uint attachmentIndex = 0;
			// Stop after reaching the limit or when there are no active attachments to process.
			CxDef::IsRTColorAttachmentIndexValid( attachmentIndex ) && !activeAttachmentsMask.empty();
			// This is rather self-descriptive, but it looked bad without this third comment here :)
			++attachmentIndex )
		{
			const auto attachmentBit = CxDef::makeRTAttachmentFlag( attachmentIndex );
			// Check if the attachments mask has this bit set.
			if( activeAttachmentsMask.is_set( attachmentBit ) )
			{
				// The function returns false if there was some internal error condition
				// and the processing should be aborted.
				if( !pFunction( attachmentIndex, cppx::make_bitmask_tp<ERTAttachmentFlags>( attachmentBit ) ) )
				{
					return false;
				}

				// Update the control mask.
				activeAttachmentsMask.unset( attachmentBit );
			}
		}

		return true;
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_RENDER_TARGET_COMMON_H__
