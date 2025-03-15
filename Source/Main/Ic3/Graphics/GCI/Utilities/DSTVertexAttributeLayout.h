
#pragma once

#ifndef __IC3_GRAPHICS_GCI_DST_VERTEX_ATTRIBUTE_LAYOUT_H__
#define __IC3_GRAPHICS_GCI_DST_VERTEX_ATTRIBUTE_LAYOUT_H__

#include <Ic3/Graphics/GCI/State/InputAssemblerCommon.h>
#include <Ic3/Graphics/Common/Utilities/DynamicallySizedTemplate.h>

namespace Ic3::Graphics::GCI
{

	template <typename TPVertexAttributeData>
	struct DSTVertexAttributeLayoutBase : public IAVertexSourceBindingCommonConfig
	{
		std::array<uint8, GCM::kIAMaxVertexAttributesNum> vertexAttributeIndexMapping;

		cppx::array_view<TPVertexBufferBinding> vertexBufferBindings;

		TPIndexBufferBinding indexBufferBinding;

		DSTVertexSourceBindingBase( cppx::array_view<TPVertexBufferBinding> pVertexBufferBindings )
		: vertexBufferBindings( pVertexBufferBindings )
		{
			ResetVertexStreamIndexMappings();
		}

		void ResetVertexStreamIndexMappings()
		{
			for( auto & arrayMappedIndex : vertexStreamIndexMapping )
			{
				arrayMappedIndex = kIAVertexStreamSlotUndefined;
			}
		}

		void SetVertexStreamIndexMapping( native_uint pVertexStreamIndex, native_uint pArrayIndex )
		{
			Ic3DebugAssert( CXU::IAIsDataStreamVertexBufferSlotValid( pVertexStreamIndex ) );
			vertexStreamIndexMapping[pVertexStreamIndex] = cppx::numeric_cast< uint8 >( pArrayIndex );
		}

		void SetVertexStreamIndexInactive( native_uint pVertexStreamIndex )
		{
			Ic3DebugAssert( CXU::IAIsDataStreamVertexBufferSlotValid( pVertexStreamIndex ) );
			vertexStreamIndexMapping[pVertexStreamIndex] = kIAVertexStreamSlotUndefined;
		}

		TPVertexBufferBinding * GetBindingForVertexStream( native_uint pVertexStreamIndex ) const noexcept
		{
			Ic3DebugAssert( CXU::IAIsDataStreamVertexBufferSlotValid( pVertexStreamIndex ) );
			const auto bindingIndexInArray = vertexStreamIndexMapping[pVertexStreamIndex];

			if( bindingIndexInArray == kIAVertexStreamSlotUndefined )
			{
				return nullptr;
			}
			else
			{
				return &( vertexBufferBindings[bindingIndexInArray] );
			}
		}
	};

	template <native_uint tpActiveVertexBuffersNum, typename TPVertexBufferBinding, typename TPIndexBufferBinding>
	struct DSTVertexSourceBinding : public DSTVertexSourceBindingBase<TPVertexBufferBinding, TPIndexBufferBinding>
	{
		using BaseType = DSTVertexSourceBindingBase<TPVertexBufferBinding, TPIndexBufferBinding>;

		TPVertexBufferBinding vertexBufferBindingArray[tpActiveVertexBuffersNum];

		DSTVertexSourceBinding()
		: BaseType( cppx::bind_array_view( vertexBufferBindingArray ) )
		{}
	};

	template <typename TPVertexBufferBinding, typename TPIndexBufferBinding>
	struct DSTVertexSourceBinding<0, TPVertexBufferBinding, TPIndexBufferBinding> : public DSTVertexSourceBindingBase<TPVertexBufferBinding, TPIndexBufferBinding>
	{
		using BaseType = DSTVertexSourceBindingBase<TPVertexBufferBinding, TPIndexBufferBinding>;

		DSTVertexSourceBinding()
		: BaseType( nullptr )
		{}
	};

	// Ic3DefineDynamicallySizedTemplateStruct0( GLIAVertexSourceBinding, GLIAVertexSourceBindingBase );

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_DST_VERTEX_ATTRIBUTE_LAYOUT_H__
