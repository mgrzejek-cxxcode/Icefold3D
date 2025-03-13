
#pragma once

#ifndef __IC3_GRAPHICS_GCI_DST_VERTEX_SOURCE_BINDING_H__
#define __IC3_GRAPHICS_GCI_DST_VERTEX_SOURCE_BINDING_H__

#include <Ic3/Graphics/GCI/State/InputAssemblerCommon.h>
#include <Ic3/Graphics/Common/Utilities/DynamicallySizedTemplate.h>

namespace Ic3::Graphics::GCI
{

	template <typename TPVertexBufferBinding, typename TPIndexBufferBinding>
	struct DSTVertexSourceBindingBase : public IAVertexSourceBindingCommonConfig
	{
		using VertexBufferBindingType = TPVertexBufferBinding;
		using IndexBufferBindingType = TPIndexBufferBinding;

		std::array<uint8, GCM::kIAMaxDataStreamVertexBuffersNum> vertexStreamIndexMapping;

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
			vertexStreamIndexMapping[pVertexStreamIndex] = cppx::numeric_cast<uint8>( pArrayIndex );
		}

		void SetVertexStreamIndexInactive( native_uint pVertexStreamIndex )
		{
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

	template <typename TPVertexSourceBindingBase, native_uint tpActiveVertexBuffersNum>
	struct DSTVertexSourceBinding : public TPVertexSourceBindingBase
	{
		using BaseType = TPVertexSourceBindingBase;

		using VertexBufferBindingType = typename BaseType::VertexBufferBindingType;

		std::array<VertexBufferBindingType, tpActiveVertexBuffersNum> vertexBufferBindingArray;

		DSTVertexSourceBinding()
		: BaseType( cppx::bind_array_view( vertexBufferBindingArray ) )
		{}
	};

	template <typename TPVertexSourceBindingBase>
	struct DSTVertexSourceBinding<TPVertexSourceBindingBase, 0> : public TPVertexSourceBindingBase
	{
		using BaseType = TPVertexSourceBindingBase;

		DSTVertexSourceBinding()
		: BaseType( nullptr )
		{}
	};

	// Ic3DefineDynamicallySizedTemplateStruct0( GLIAVertexSourceBinding, GLIAVertexSourceBindingBase );

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_DST_VERTEX_SOURCE_BINDING_H__
