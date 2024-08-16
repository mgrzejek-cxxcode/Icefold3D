
#pragma once

#ifndef __IC3_GRAPHICS_GCI_INPUT_ASSEMBLER_COMMON_H__
#define __IC3_GRAPHICS_GCI_INPUT_ASSEMBLER_COMMON_H__

#include "CommonGPUStateDefs.h"
#include "../Resources/ShaderCommon.h"
#include "../Resources/GPUBufferReference.h"

namespace Ic3::Graphics::GCI
{

	ic3GpaDeclareClassHandle( GPUBuffer );

	/// @brief
	using input_assembler_index_t = uint16;

	/// @brief
	using vertex_attribute_offset_t = uint16;

	namespace CxDef
	{

		///
		constexpr auto IA_VERTEX_ATTRIBUTE_INDEX_UNDEFINED = QLimits<input_assembler_index_t>::maxValue;

		///
		constexpr auto IA_VERTEX_ATTRIBUTE_OFFSET_APPEND = QLimits<vertex_attribute_offset_t>::maxValue;

		///
		constexpr auto IA_VERTEX_ATTRIBUTE_OFFSET_APPEND16 = static_numeric_cast<vertex_attribute_offset_t>( IA_VERTEX_ATTRIBUTE_OFFSET_APPEND - 1 );

		///
		constexpr auto IA_VERTEX_BUFFER_BINDING_INDEX_UNDEFINED = QLimits<input_assembler_index_t>::maxValue;

		///
		constexpr auto IA_VERTEX_STREAM_INSTANCE_RATE_UNDEFINED = QLimits<uint16>::maxValue;

		///
		constexpr auto IA_VERTEX_STREAM_INDEX_UNDEFINED = IA_VERTEX_BUFFER_BINDING_INDEX_UNDEFINED;

		///
		constexpr auto IA_VERTEX_ATTRIBUTE_MASK_ALL = makeLSFBitmask<input_assembler_index_t>( GCM::IA_MAX_VERTEX_ATTRIBUTES_NUM );

		///
		constexpr auto IA_VERTEX_BUFFER_BINDING_MASK_ALL = makeLSFBitmask<input_assembler_index_t>( GCM::IA_MAX_VERTEX_BUFFER_BINDINGS_NUM );

		///
		constexpr auto IA_INPUT_STREAM_BINDING_MASK_ALL = IA_VERTEX_BUFFER_BINDING_MASK_ALL | 0x10000u;

		/// @brief
		inline constexpr bool isIAVertexAttributeIndexValid( native_uint pIndex )
		{
			return pIndex < GCM::IA_MAX_VERTEX_ATTRIBUTES_NUM;
		}

		/// @brief
		inline constexpr uint32 makeIAVertexAttributeFlag( native_uint pAttribIndex )
		{
			return ( pAttribIndex < GCM::IA_MAX_VERTEX_ATTRIBUTES_NUM ) ? ( 1 << static_cast<input_assembler_index_t>( pAttribIndex ) ) : 0u;
		}

		/// @brief
		inline constexpr bool isIAVertexBufferIndexValid( native_uint pStreamIndex )
		{
			return pStreamIndex < GCM::IA_MAX_VERTEX_BUFFER_BINDINGS_NUM;
		}

		/// @brief Returns
		inline constexpr uint32 makeIAVertexBufferFlag( native_uint pStreamIndex )
		{
			return ( pStreamIndex < GCM::IA_MAX_VERTEX_BUFFER_BINDINGS_NUM ) ? ( 1 << static_cast<input_assembler_index_t> ( pStreamIndex ) ) : 0u;
		}

		/// @brief Returns
		inline constexpr uint32 makeIAIndexBufferFlag()
		{
			return ( 1 << static_cast<input_assembler_index_t>( GCM::IA_MAX_VERTEX_BUFFER_BINDINGS_NUM ) );
		}

	}

	/// @brief
	enum EIAVertexAttributeFlags : uint32
	{
		E_IA_VERTEX_ATTRIBUTE_FLAG_ATTR_0_BIT = CxDef::makeIAVertexAttributeFlag( 0 ),
		E_IA_VERTEX_ATTRIBUTE_FLAG_ATTR_1_BIT = CxDef::makeIAVertexAttributeFlag( 1 ),
		E_IA_VERTEX_ATTRIBUTE_FLAG_ATTR_2_BIT = CxDef::makeIAVertexAttributeFlag( 2 ),
		E_IA_VERTEX_ATTRIBUTE_FLAG_ATTR_3_BIT = CxDef::makeIAVertexAttributeFlag( 3 ),
		E_IA_VERTEX_ATTRIBUTE_FLAG_ATTR_4_BIT = CxDef::makeIAVertexAttributeFlag( 4 ),
		E_IA_VERTEX_ATTRIBUTE_FLAG_ATTR_5_BIT = CxDef::makeIAVertexAttributeFlag( 5 ),
		E_IA_VERTEX_ATTRIBUTE_FLAG_ATTR_6_BIT = CxDef::makeIAVertexAttributeFlag( 6 ),
		E_IA_VERTEX_ATTRIBUTE_FLAG_ATTR_7_BIT = CxDef::makeIAVertexAttributeFlag( 7 ),
		E_IA_VERTEX_ATTRIBUTE_MASK_ALL = makeLSFBitmask<uint32>( GCM::IA_MAX_VERTEX_ATTRIBUTES_NUM ),
	};

	/// @brief
	enum EIAVertexStreamBindingFlags : uint32
	{
		E_IA_VERTEX_STREAM_BINDING_FLAG_VERTEX_BUFFER_0_BIT = CxDef::makeIAVertexBufferFlag( 0 ),
		E_IA_VERTEX_STREAM_BINDING_FLAG_VERTEX_BUFFER_1_BIT = CxDef::makeIAVertexBufferFlag( 1 ),
		E_IA_VERTEX_STREAM_BINDING_FLAG_VERTEX_BUFFER_2_BIT = CxDef::makeIAVertexBufferFlag( 2 ),
		E_IA_VERTEX_STREAM_BINDING_FLAG_VERTEX_BUFFER_3_BIT = CxDef::makeIAVertexBufferFlag( 3 ),
		E_IA_VERTEX_STREAM_BINDING_FLAG_VERTEX_BUFFER_4_BIT = CxDef::makeIAVertexBufferFlag( 4 ),
		E_IA_VERTEX_STREAM_BINDING_FLAG_VERTEX_BUFFER_5_BIT = CxDef::makeIAVertexBufferFlag( 5 ),
		E_IA_VERTEX_STREAM_BINDING_FLAG_VERTEX_BUFFER_6_BIT = CxDef::makeIAVertexBufferFlag( 6 ),
		E_IA_VERTEX_STREAM_BINDING_FLAG_VERTEX_BUFFER_7_BIT = CxDef::makeIAVertexBufferFlag( 7 ),

		E_IA_VERTEX_STREAM_BINDING_FLAG_INDEX_BUFFER_BIT = CxDef::makeIAIndexBufferFlag(),

		E_IA_VERTEX_STREAM_BINDING_MASK_VERTEX_BUFFER_ALL_BITS =
				makeLSFBitmask<uint32>( GCM::IA_MAX_VERTEX_BUFFER_BINDINGS_NUM ),

		E_IA_VERTEX_STREAM_BINDING_MASK_ALL =
				E_IA_VERTEX_STREAM_BINDING_MASK_VERTEX_BUFFER_ALL_BITS | E_IA_VERTEX_STREAM_BINDING_FLAG_INDEX_BUFFER_BIT
	};

	/// @brief Definition of a vertex input attribute.
	struct IAVertexAttributeInfo
	{
		input_assembler_index_t streamIndex{ CxDef::IA_VERTEX_BUFFER_BINDING_INDEX_UNDEFINED };

		std::string semanticName;

		input_assembler_index_t semanticIndex;

		EVertexAttribFormat format{ EVertexAttribFormat::Undefined };

		/// An offset of the attribute from the start of the vertex data.
		/// VERTEX_ATTRIBUTE_OFFSET_APPEND can be specified if the attribute is placed directly after previous one.
		uint16 relativeOffset;

		uint16 instanceRate;

		/// @brief Returns true if this instance represents a valid vertex attribute.
		IC3_ATTR_NO_DISCARD bool active() const noexcept
		{
			return !semanticName.empty() && CxDef::isIAVertexBufferIndexValid( streamIndex ) && ( format != EVertexAttribFormat::Undefined );
		}
	};

	/// @brief Typedef for ordered, fixed-size array of vertex attribute definitions.
	using IAVertexAttributeInfoArray = std::array<IAVertexAttributeInfo, GCM::IA_MAX_VERTEX_ATTRIBUTES_NUM>;

	/// @brief
	struct IAInputLayoutDefinition
	{
		EPrimitiveTopology primitiveTopology;
		Bitmask<EIAVertexAttributeFlags> activeAttributesMask;
		IAVertexAttributeInfoArray attributeArray;
	};

	struct IAVertexStreamBufferReference
	{
		GPUBufferReference sourceBuffer;
		gpu_memory_size_t relativeOffset;

		bool empty() const noexcept
		{
			return sourceBuffer.empty();
		}

		gpu_memory_size_t dataOffset() const noexcept
		{
			return sourceBuffer.getRefSubRegion().offset + relativeOffset;
		}

		explicit operator bool() const noexcept
		{
			return !empty();
		}

		void reset()
		{
			sourceBuffer.reset();
			relativeOffset = 0;
		}
	};

	/// @brief
	struct IAIndexBufferReference : public IAVertexStreamBufferReference
	{
		EIndexDataFormat indexFormat;
	};

	/// @brief
	struct IAVertexBufferReference : public IAVertexStreamBufferReference
	{
		gpu_memory_size_t vertexStride;
	};

	/// @brief
	using IAVertexBufferReferenceArray = std::array<IAVertexBufferReference, GCM::IA_MAX_VERTEX_BUFFER_BINDINGS_NUM>;

	/// @brief
	struct IAVertexStreamDefinition
	{
		Bitmask<EIAVertexStreamBindingFlags> activeBindingsMask;
		IAVertexBufferReferenceArray vertexBufferReferences;
		IAIndexBufferReference indexBufferReference;
	};

	/// @brief Describes a single, continuous range of vertex buffer binding points in the vertex stream.
	/// Used by drivers with support for range-based binding (D3D11, GL4) to reduce the number of API calls.
	struct IAVertexBufferRange
	{
		/// First index of the vertex buffer binding this range defines.
		input_assembler_index_t firstIndex = CxDef::IA_VERTEX_BUFFER_BINDING_INDEX_UNDEFINED;

		/// Length of this range, i.e. number of vertex buffers within the range.
		uint16 length = 0;
	};

	using IAVertexBufferBindingIndexList = std::vector<input_assembler_index_t>;

	using IAVertexBufferRangeList = std::vector<IAVertexBufferRange>;

	// State Management Utility API
	namespace smutil
	{

		/// @brief Returns
		IC3_GRAPHICS_GCI_API_NO_DISCARD Bitmask<EIAVertexAttributeFlags> getIAVertexInputActiveAttributesMask(
				const IAVertexAttributeInfoArray & pVertexAttributes ) noexcept;

		/// @brief
		IC3_GRAPHICS_GCI_API_NO_DISCARD uint32 getIAVertexInputActiveAttributesNum(
				const IAVertexAttributeInfoArray & pVertexAttributes ) noexcept;

		/// @brief
		IC3_GRAPHICS_GCI_API_NO_DISCARD IAVertexBufferRangeList generateActiveVertexBufferRanges(
				const IAVertexBufferReferenceArray & pVBReferences ) noexcept;

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_INPUT_ASSEMBLER_COMMON_H__
