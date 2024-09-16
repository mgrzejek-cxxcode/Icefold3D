
#pragma once

#ifndef __IC3_GRAPHICS_GCI_RENDER_TARGET_COMMON_H__
#define __IC3_GRAPHICS_GCI_RENDER_TARGET_COMMON_H__

#include "CommonGPUStateDefs.h"
#include "../Resources/TextureCommon.h"
#include <Ic3/Cppx/BitUtils.h>

namespace Ic3::Graphics::GCI
{

	ic3DeclareClassHandle( RenderTargetTexture );

	enum ERenderTargetBufferFlags : uint32
	{
		E_RENDER_TARGET_BUFFER_FLAG_COLORBit = 0x01,
		E_RENDER_TARGET_BUFFER_FLAG_DEPTHBit = 0x02,
		E_RENDER_TARGET_BUFFER_FLAG_STENCILBit = 0x04,

		E_RENDER_TARGET_BUFFER_MASK_DEPTH_STENCIL =
			E_RENDER_TARGET_BUFFER_FLAG_DEPTHBit |
			E_RENDER_TARGET_BUFFER_FLAG_STENCILBit,

		E_RENDER_TARGET_BUFFER_MASK_ALL =
			E_RENDER_TARGET_BUFFER_FLAG_COLORBit |
			E_RENDER_TARGET_BUFFER_MASK_DEPTH_STENCIL,
	};

	/// @brief
	enum class ERenderTargetTextureType : uint32
	{
		Unknown = 0,
		RTColor = E_RENDER_TARGET_BUFFER_FLAG_COLORBit,
		RTDepthOnly = E_RENDER_TARGET_BUFFER_FLAG_DEPTHBit,
		RTDepthStencil = E_RENDER_TARGET_BUFFER_MASK_DEPTH_STENCIL,
		RTStencilOnly = E_RENDER_TARGET_BUFFER_FLAG_STENCILBit,
	};

	namespace CxDef
	{

		/// @brief
		IC3_ATTR_NO_DISCARD inline constexpr TBitmask<uint32> getRTBufferMaskForRenderTargetTextureType(
				ERenderTargetTextureType pRenderTargetTextureType )
		{
			return static_cast<uint32>( pRenderTargetTextureType ) & E_RENDER_TARGET_BUFFER_MASK_ALL;
		}

		/// @brief
		IC3_ATTR_NO_DISCARD inline constexpr uint32 getRTAttachmentRequiredUsageMask( native_uint pAttachmentIndex )
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

		TBitmask<ERTAttachmentFlags> activeAttachmentsMask = 0;

		TArrayView<TAttachmentProperty> const colorAttachments;

		TAttachmentProperty & depthStencilAttachment;

		RenderTargetAttachmentPropertySet()
		: colorAttachments( bindArrayView( attachments.data(), GCM::cxRTMaxColorAttachmentsNum ) )
		, depthStencilAttachment( attachments[eRTAttachmentIndexDepthStencil] )
		{}

		RenderTargetAttachmentPropertySet( const RenderTargetAttachmentPropertySet<TAttachmentProperty> & pSource )
		: attachments( pSource.attachments )
		, activeAttachmentsMask( pSource.activeAttachmentsMask )
		, colorAttachments( Cppx::bindArrayView( attachments.data(), GCM::cxRTMaxColorAttachmentsNum ) )
		, depthStencilAttachment( attachments[eRTAttachmentIndexDepthStencil] )
		{}

		RenderTargetAttachmentPropertySet & operator=( const RenderTargetAttachmentPropertySet<TAttachmentProperty> & pRhs )
		{
			attachments = pRhs.attachments;
			activeAttachmentsMask = pRhs.activeAttachmentsMask;
			return *this;
		}

		IC3_ATTR_NO_DISCARD bool empty() const noexcept
		{
			return activeAttachmentsMask.empty();
		}

		IC3_ATTR_NO_DISCARD uint32 countActiveColorAttachments() const noexcept
		{
			return Cppx::popCount( static_cast<uint32>( activeAttachmentsMask & eRTAttachmentMaskColorAll ) );
		}

		IC3_ATTR_NO_DISCARD bool isColorAttachmentActive( uint32 pAttachmentIndex ) const noexcept
		{
			const auto attachmentBit = CxDef::makeRTAttachmentFlag( pAttachmentIndex );
			return ( attachmentBit != 0 ) && activeAttachmentsMask.isSet( attachmentBit );
		}

		IC3_ATTR_NO_DISCARD bool isDepthStencilAttachmentActive() const noexcept
		{
			return activeAttachmentsMask.isSet( eRtAttachmentFlagDepthStencilBit );
		}

		IC3_ATTR_NO_DISCARD bool hasAnyColorAttachmentsActive() const noexcept
		{
			return (activeAttachmentsMask & eRTAttachmentMaskColorAll) != 0;
		}
	};

	template <typename TAttachmentProperty>
	struct RenderTargetAttachmentConfigurationSet : public RenderTargetAttachmentPropertySet<TAttachmentProperty>
	{
		TBitmask<ERTAttachmentFlags> attachmentsActionResolveMask = 0;

		IC3_ATTR_NO_DISCARD uint32 countAttachmentsActionResolve() const noexcept
		{
			return Cppx::popCount( static_cast<uint32>( attachmentsActionResolveMask & eRTAttachmentMaskColorAll ) );
		}
	};

	/// @brief
	struct RenderTargetAttachmentBinding
	{
		RenderTargetTextureHandle attachmentTexture;
		RenderTargetTextureHandle resolveTexture;

		void reset()
		{
			attachmentTexture.reset();
			resolveTexture.reset();
		}

		bool empty() const noexcept
		{
			return !attachmentTexture;
		}

		explicit operator bool() const noexcept
		{
			return !empty();
		}
	};

	using RenderTargetColorAttachmentBindingArray = RenderTargetColorAttachmentPropertyArray<RenderTargetAttachmentBinding>;

	struct RenderTargetBindingDefinition : public RenderTargetAttachmentConfigurationSet<RenderTargetAttachmentBinding>
	{
		IC3_ATTR_NO_DISCARD RenderTargetBindingDefinition getValidated() const noexcept
		{
			RenderTargetBindingDefinition validatedDefinition = *this;
			validatedDefinition.resetAttachmentsFlags();
			return validatedDefinition;
		}

		IC3_GRAPHICS_GCI_API void resetAttachmentsFlags() noexcept;
	};

	/// @brief
	struct RenderTargetAttachmentLayout
	{
		ETextureFormat format = ETextureFormat::Unknown;

		void reset()
		{
			format = ETextureFormat::Unknown;
		}

		bool valid() const noexcept
		{
			return format != ETextureFormat::Unknown;
		}

		explicit operator bool() const noexcept
		{
			return valid();
		}
	};

	/// @brief A definition of a vertex layout used to create a driver-specific RenderTargetLayout object.
	struct RenderTargetLayout : public RenderTargetAttachmentPropertySet<RenderTargetAttachmentLayout>
	{
		TextureSize2D sharedImageRect = cxTextureSize2DUndefined;

		uint32 sharedMSAALevel = 0;
	};

	namespace SMU
	{

		IC3_GRAPHICS_GCI_API_NO_DISCARD const RenderTargetAttachmentBinding * getRenderTargetBindingDefinitionFirstTarget(
				const RenderTargetBindingDefinition & pBindingDefinition );

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool validateRenderTargetBindingDefinition(
				const RenderTargetBindingDefinition & pBindingDefinition );

		IC3_GRAPHICS_GCI_API_NO_DISCARD RenderTargetLayout getRenderTargetLayoutForBindingDefinition(
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
	inline bool foreachRTAttachmentIndex( TBitmask<ERTAttachmentFlags> pActiveAttachmentsMask, TFunction pFunction )
	{
		// A local copy of the active attachments mask. Bits of already processed attachments
		// are removed, so when the value reaches 0, we can immediately stop further processing.
		auto activeAttachmentsMask = pActiveAttachmentsMask;

		for( // Iterate using RTA (Render Target Attachment) index value.
			native_uint attachmentIndex = 0;
			// Stop after reaching the limit or when there are no active attachments to process.
			CxDef::isRTAttachmentIndexValid( attachmentIndex ) && !activeAttachmentsMask.empty();
			// This is rather self-descriptive, but it looked bad without this third comment here :)
			++attachmentIndex )
		{
			const auto attachmentBit = CxDef::makeRTAttachmentFlag( attachmentIndex );
			// Check if the attachments mask has this bit set.
			if( activeAttachmentsMask.isSet( attachmentBit ) )
			{
				// The function returns false if there was some internal error condition
				// and the processing should be aborted.
				if( !pFunction( attachmentIndex, Cppx::makeBitmaskEx<ERTAttachmentFlags>( attachmentBit ) ) )
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
	inline bool foreachRTColorAttachmentIndex( TBitmask<ERTAttachmentFlags> pActiveAttachmentsMask, TFunction pFunction )
	{
		// A local copy of the active attachments mask. Bits of already processed attachments
		// are removed, so when the value reaches 0, we can immediately stop further processing.
		auto activeAttachmentsMask = pActiveAttachmentsMask;

		for( // Iterate using RTA (Render Target Attachment) index value.
			native_uint attachmentIndex = 0;
			// Stop after reaching the limit or when there are no active attachments to process.
			CxDef::isRTColorAttachmentIndexValid( attachmentIndex ) && !activeAttachmentsMask.empty();
			// This is rather self-descriptive, but it looked bad without this third comment here :)
			++attachmentIndex )
		{
			const auto attachmentBit = CxDef::makeRTAttachmentFlag( attachmentIndex );
			// Check if the attachments mask has this bit set.
			if( activeAttachmentsMask.isSet( attachmentBit ) )
			{
				// The function returns false if there was some internal error condition
				// and the processing should be aborted.
				if( !pFunction( attachmentIndex, Cppx::makeBitmaskEx<ERTAttachmentFlags>( attachmentBit ) ) )
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
