
#pragma once

#ifndef __IC3_GRAPHICS_GCI_DST_RENDER_TARGET_BINDING_H__
#define __IC3_GRAPHICS_GCI_DST_RENDER_TARGET_BINDING_H__

#include <Ic3/Graphics/GCI/State/RenderTargetCommon.h>
#include <Ic3/Graphics/Common/Utilities/DynamicallySizedTemplate.h>

namespace Ic3::Graphics::GCI
{

	/**
	 * @brief 
	 * @tparam TPColorOutputAttachmentBinding 
	 * @tparam TPColorResolveAttachmentBinding 
	 * @tparam TPDepthStencilOutputAttachmentBinding 
	 * @tparam TPDepthStencilResolveAttachmentBinding 
	 */
	template <typename TPColorOutputAttachmentBinding,
	          typename TPColorResolveAttachmentBinding,
	          typename TPDepthStencilOutputAttachmentBinding,
	          typename TPDepthStencilResolveAttachmentBinding>
	struct DSTRenderTargetBindingBase : public RenderTargetArrayCommonConfig
	{
		using ColorOutputAttachmentBindingType = TPColorOutputAttachmentBinding;
		using ColorResolveAttachmentBindingType = TPColorResolveAttachmentBinding;
		using DepthStencilOutputAttachmentBindingType = TPDepthStencilOutputAttachmentBinding;
		using DepthStencilResolveAttachmentBindingType = TPDepthStencilResolveAttachmentBinding;

		std::array<uint8, GCM::kIAMaxDataStreamVertexBuffersNum> colorOutputAttachmentIndexMapping;
		std::array<uint8, GCM::kIAMaxDataStreamVertexBuffersNum> colorResolveAttachmentIndexMapping;

		cppx::array_view<ETextureFormat> colorOutputAttachmentFormats;
		cppx::array_view<TPColorOutputAttachmentBinding> colorOutputAttachmentBindings;
		cppx::array_view<TPColorResolveAttachmentBinding> colorResolveAttachmentBindings;

		ETextureFormat depthStencilOutputAttachmentFormat;
		TPDepthStencilOutputAttachmentBinding depthStencilOutputAttachmentBinding;
		TPDepthStencilResolveAttachmentBinding depthStencilResolveAttachmentBinding;

		DSTRenderTargetBindingBase(
			cppx::array_view<ETextureFormat> pColorOutputAttachmentFormats,
			cppx::array_view<TPColorOutputAttachmentBinding> pColorOutputAttachmentBindings,
			cppx::array_view<TPColorResolveAttachmentBinding> pColorResolveAttachmentBindings )
		: colorOutputAttachmentFormats( pColorOutputAttachmentFormats )
		, colorOutputAttachmentBindings( pColorOutputAttachmentBindings )
		, colorResolveAttachmentBindings( pColorResolveAttachmentBindings )
		{
			ResetColorAttachmentIndexMappings();
		}

		void ResetColorAttachmentIndexMappings()
		{
			for( auto & arrayMappedIndex : colorOutputAttachmentIndexMapping )
			{
				arrayMappedIndex = kRTOAttachmentIndexUndefined;
			}
			for( auto & arrayMappedIndex : colorResolveAttachmentIndexMapping )
			{
				arrayMappedIndex = kRTOAttachmentIndexUndefined;
			}
		}

		void SetColorOutputAttachmentIndexMapping( native_uint pColorAttachmentIndex, native_uint pArrayIndex )
		{
			colorOutputAttachmentIndexMapping[pColorAttachmentIndex] = cppx::numeric_cast<uint8>( pArrayIndex );
		}

		void SetColorOutputAttachmentIndexInactive( native_uint pColorAttachmentIndex )
		{
			colorOutputAttachmentIndexMapping[pColorAttachmentIndex] = kRTOAttachmentIndexUndefined;
		}

		void SetColorResolveAttachmentIndexMapping( native_uint pColorAttachmentIndex, native_uint pArrayIndex )
		{
			colorResolveAttachmentIndexMapping[pColorAttachmentIndex] = cppx::numeric_cast<uint8>( pArrayIndex );
		}

		void SetColorResolveAttachmentIndexInactive( native_uint pColorAttachmentIndex )
		{
			colorResolveAttachmentIndexMapping[pColorAttachmentIndex] = kRTOAttachmentIndexUndefined;
		}

		TPColorOutputAttachmentBinding * GetBindingForColorOutputAttachment( native_uint pColorAttachmentIndex ) const noexcept
		{
			Ic3DebugAssert( CXU::RTOIsColorAttachmentIndexValid( pColorAttachmentIndex ) );
			const auto bindingIndexInArray = colorOutputAttachmentIndexMapping[pColorAttachmentIndex];

			if( bindingIndexInArray != kRTOAttachmentIndexUndefined )
			{
				return &( colorOutputAttachmentBindings[bindingIndexInArray] );
			}
			else
			{
				return nullptr;
			}
		}

		TPColorResolveAttachmentBinding * GetBindingForColorResolveAttachment( native_uint pColorAttachmentIndex ) const noexcept
		{
			Ic3DebugAssert( CXU::RTOIsColorAttachmentIndexValid( pColorAttachmentIndex ) );
			const auto bindingIndexInArray = colorResolveAttachmentIndexMapping[pColorAttachmentIndex];

			if( bindingIndexInArray != kRTOAttachmentIndexUndefined )
			{
				return &( colorResolveAttachmentBindings[bindingIndexInArray] );
			}
			else
			{
				return nullptr;
			}
		}
	};

	/**
	 * @brief 
	 */
	template <typename TPRenderTargetBindingBase, native_uint tpActiveColorOutputAttachmentsNum, native_uint tpActiveColorResolveAttachmentsNum>
	struct DSTRenderTargetBinding : public TPRenderTargetBindingBase
	{
		using BaseType = TPRenderTargetBindingBase;

		using ColorOutputAttachmentBindingType = typename TPRenderTargetBindingBase::ColorOutputAttachmentBindingType;
		using ColorResolveAttachmentBindingType = typename TPRenderTargetBindingBase::ColorResolveAttachmentBindingType;

		std::array<ETextureFormat, tpActiveColorOutputAttachmentsNum> colorOutputAttachmentFormatArray;
		std::array<ColorOutputAttachmentBindingType, tpActiveColorOutputAttachmentsNum> colorOutputAttachmentBindingArray;
		std::array<ColorResolveAttachmentBindingType, tpActiveColorResolveAttachmentsNum> colorResolveAttachmentBindingArray;

		DSTRenderTargetBinding()
		: BaseType(
			cppx::bind_array_view( colorOutputAttachmentFormatArray ),
			cppx::bind_array_view( colorOutputAttachmentBindingArray ),
			cppx::bind_array_view( colorResolveAttachmentBindingArray ) )
		{}
	};

	/**
	 * @brief 
	 */
	template <typename TPRenderTargetBindingBase, native_uint tpActiveColorOutputAttachmentsNum>
	struct DSTRenderTargetBinding<TPRenderTargetBindingBase, tpActiveColorOutputAttachmentsNum, 0> : public TPRenderTargetBindingBase
	{
		using BaseType = TPRenderTargetBindingBase;

		using ColorOutputAttachmentBindingType = typename TPRenderTargetBindingBase::ColorOutputAttachmentBindingType;

		std::array<ETextureFormat, tpActiveColorOutputAttachmentsNum> colorOutputAttachmentFormatArray;
		std::array<ColorOutputAttachmentBindingType, tpActiveColorOutputAttachmentsNum> colorOutputAttachmentBindingArray;

		DSTRenderTargetBinding()
		: BaseType( cppx::bind_array_view( colorOutputAttachmentFormatArray ), cppx::bind_array_view( colorOutputAttachmentBindingArray ), nullptr )
		{}
	};

	/**
	 * @brief 
	 * @tparam TPColorOutputAttachmentBinding 
	 * @tparam TPColorResolveAttachmentBinding 
	 * @tparam TPDepthStencilOutputAttachmentBinding 
	 * @tparam TPDepthStencilResolveAttachmentBinding 
	 */
	template <typename TPRenderTargetBindingBase>
	struct DSTRenderTargetBinding<TPRenderTargetBindingBase, 0, 0> : public TPRenderTargetBindingBase
	{
		using BaseType = TPRenderTargetBindingBase;

		DSTRenderTargetBinding()
		: BaseType( nullptr, nullptr, nullptr )
		{}
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_DST_RENDER_TARGET_BINDING_H__
