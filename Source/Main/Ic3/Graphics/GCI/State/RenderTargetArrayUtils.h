
#pragma once

#ifndef __IC3_GRAPHICS_GCI_RENDER_TARGET_ARRAY_UTILS_H__
#define __IC3_GRAPHICS_GCI_RENDER_TARGET_ARRAY_UTILS_H__

#include "CommonGPUStateDefs.h"

namespace Ic3::Graphics::GCI
{

	/**
	 *
	 */
	template <typename TPAttachmentConfig>
	struct TRenderTargetArrayConfiguration;

	/**
	 * An alias for a fixed-sized array that holds an arbitrary property for all RT attachments (including depth-stencil).
	 * @tparam TPAttachmentProperty Type of the property that describes a single RT attachment.
	 */
	template <typename TPAttachmentProperty>
	using TRenderTargetAttachmentPropertyArray = std::array<TPAttachmentProperty, GCM::kRTOMaxCombinedAttachmentsNum>;

	/**
	 * An alias for a fixed-sized array that holds an arbitrary property for all RT color attachments.
	 * @tparam TPAttachmentProperty Type of the property that describes a single RT color attachment.
	 */
	template <typename TPAttachmentProperty>
	using TRenderTargetColorAttachmentPropertyArray = std::array<TPAttachmentProperty, GCM::kRTOMaxColorAttachmentsNum>;

	namespace GCU
	{

		template <typename TPFunction>
		bool ForEachRTAttachmentIndex( cppx::bitmask<ERTAttachmentFlags> pActiveAttachmentsMask, TPFunction pFunction );

		template <typename TPFunction>
		bool ForEachRTColorAttachmentIndex( cppx::bitmask<ERTAttachmentFlags> pActiveAttachmentsMask, TPFunction pFunction );

	}

	/**
	 *
	 */
	struct RenderTargetArrayCommonConfig
	{
		cppx::bitmask<ERTAttachmentFlags> activeAttachmentsMask = 0;

		uint32 activeAttachmentsNum = 0;

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return !IsEmpty();
		}

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept
		{
			return activeAttachmentsMask.empty() || ( activeAttachmentsNum == 0 );
		}

		void ResetActiveAttachmentInfo() noexcept
		{
			activeAttachmentsMask.clear();
			activeAttachmentsNum = 0;
		}
	};

	/**
	 *
	 */
	template <typename TPAttachmentConfig>
	struct TRenderTargetArrayConfiguration : public RenderTargetArrayCommonConfig
	{
		TRenderTargetAttachmentPropertyArray<TPAttachmentConfig> attachmentConfigArray;

		cppx::array_view<TPAttachmentConfig> const colorAttachmentConfigArray;

		TPAttachmentConfig & depthStencilAttachmentConfig;

		TRenderTargetArrayConfiguration()
		: RenderTargetArrayCommonConfig()
		, colorAttachmentConfigArray( cppx::bind_array_view( attachmentConfigArray.data(), GCM::kRTOMaxColorAttachmentsNum ) )
		, depthStencilAttachmentConfig( attachmentConfigArray[kRTOAttachmentIndexDepthStencil] )
		{}

		TRenderTargetArrayConfiguration( const TRenderTargetArrayConfiguration & pSource )
		: RenderTargetArrayCommonConfig( pSource )
		, attachmentConfigArray( pSource.attachmentConfigArray )
		, colorAttachmentConfigArray( cppx::bind_array_view( attachmentConfigArray.data(), GCM::kRTOMaxColorAttachmentsNum ) )
		, depthStencilAttachmentConfig( attachmentConfigArray[kRTOAttachmentIndexDepthStencil] )
		{}

		TRenderTargetArrayConfiguration & operator=( const TRenderTargetArrayConfiguration & pRhs ) noexcept
		{
			if( &pRhs != this )
			{
				activeAttachmentsMask = pRhs.activeAttachmentsMask;
				activeAttachmentsNum = pRhs.activeAttachmentsNum;
				attachmentConfigArray = pRhs.attachmentConfigArray;
			}

			return *this;
		}

		CPPX_ATTR_NO_DISCARD TPAttachmentConfig & operator[]( native_uint pAttachmentIndex ) noexcept
		{
			Ic3DebugAssert( CXU::RTOIsAttachmentIndexValid( pAttachmentIndex ) );
			return attachmentConfigArray[pAttachmentIndex];
		}

		CPPX_ATTR_NO_DISCARD const TPAttachmentConfig & operator[]( native_uint pAttachmentIndex ) const noexcept
		{
			Ic3DebugAssert( CXU::RTOIsAttachmentIndexValid( pAttachmentIndex ) );
			return attachmentConfigArray[pAttachmentIndex];
		}

		CPPX_ATTR_NO_DISCARD TPAttachmentConfig * GetAttachment( native_uint pAttachmentIndex ) noexcept
		{
			Ic3DebugAssert( CXU::RTOIsAttachmentIndexValid( pAttachmentIndex ) );
			return &( attachmentConfigArray[pAttachmentIndex] );
		}

		CPPX_ATTR_NO_DISCARD const TPAttachmentConfig * GetAttachment( native_uint pAttachmentIndex ) const noexcept
		{
			Ic3DebugAssert( CXU::RTOIsAttachmentIndexValid( pAttachmentIndex ) );
			return &( attachmentConfigArray[pAttachmentIndex] );
		}

		CPPX_ATTR_NO_DISCARD const TPAttachmentConfig * FindFirstActiveAttachment( native_uint pFirstIndex = 0 ) const noexcept
		{
			const auto attachmentBaseIndex = cppx::get_min_of( pFirstIndex, GCM::kRTOMaxCombinedAttachmentsNum );
			const auto activeAttachmentIter = std::find_if(
					attachmentConfigArray.begin() + attachmentBaseIndex,
					attachmentConfigArray.end(),
					[]( const auto & pAttachmentConfig ) -> bool {
						return pAttachmentConfig ? true : false;
					});
			return ( activeAttachmentIter != attachmentConfigArray.end() ) ? &( *activeAttachmentIter ) : nullptr;
		}

		void UpdateActiveAttachmentInfo() noexcept
		{
			activeAttachmentsMask.clear();
			activeAttachmentsNum = 0;

			ForEachRTAttachmentIndex( eRTAttachmentMaskAll,
				[&]( native_uint pIndex, ERTAttachmentFlags pAttachmentBit )
				{
					if( const auto & attachmentConfig = attachmentConfigArray[pIndex] )
					{
						activeAttachmentsMask.set( pAttachmentBit );
						activeAttachmentsNum += 1;
					}
					return true;
				});
		}

		void Reset() noexcept
		{
			for( auto & attachmentConfig : attachmentConfigArray )
			{
				attachmentConfig.Reset();
			}
			activeAttachmentsMask.clear();
			activeAttachmentsNum = 0;
		}
	};

	namespace GCU
	{

		template <typename TPFunction>
		inline bool ForEachRTAttachmentIndex( cppx::bitmask<ERTAttachmentFlags> pActiveAttachmentsMask, TPFunction pFunction )
		{
			// A local copy of the active attachments mask. Bits of already processed attachments
			// are removed, so when the value reaches 0, we can immediately stop further processing.
			auto activeAttachmentsMask = pActiveAttachmentsMask;

			for( // Iterate using RTA (Render Target Attachment) index value.
			     native_uint attachmentIndex = 0;
			     // Stop after reaching the limit or when there are no active attachments to process.
			     CXU::RTOIsAttachmentIndexValid( attachmentIndex ) && !activeAttachmentsMask.empty();
			     // This is rather self-descriptive, but it looked bad without this third comment here :)
			     ++attachmentIndex )
			{
				const auto attachmentBit = CXU::RTOMakeAttachmentFlag( attachmentIndex );
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

		template <typename TPFunction>
		inline bool ForEachRTColorAttachmentIndex( cppx::bitmask<ERTAttachmentFlags> pActiveAttachmentsMask, TPFunction pFunction )
		{
			// A local copy of the active attachments mask. Bits of already processed attachments
			// are removed, so when the value reaches 0, we can immediately stop further processing.
			auto activeColorAttachmentsMask = pActiveAttachmentsMask & eRTAttachmentMaskColorAll;

			for( // Iterate using RTA (Render Target Attachment) index value.
			     native_uint colorAttachmentIndex = 0;
			     // Stop after reaching the limit or when there are no active attachments to process.
			     CXU::RTOIsColorAttachmentIndexValid( colorAttachmentIndex ) && !activeColorAttachmentsMask.empty();
			     // This is also self-descriptive, but - again - it looked bad without this third comment here, too :)
			     ++colorAttachmentIndex )
			{
				const auto colorAttachmentBit = CXU::RTOMakeAttachmentFlag( colorAttachmentIndex );
				// Check if the attachments mask has this bit set.
				if( activeColorAttachmentsMask.is_set( colorAttachmentBit ) )
				{
					// The function returns false if there was some internal error condition
					// and the processing should be aborted.
					if( !pFunction( colorAttachmentIndex, cppx::make_bitmask_tp<ERTAttachmentFlags>( colorAttachmentBit ) ) )
					{
						return false;
					}

					// Update the control mask.
					activeColorAttachmentsMask.unset( colorAttachmentBit );
				}
			}

			return true;
		}

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_RENDER_TARGET_ARRAY_UTILS_H__
