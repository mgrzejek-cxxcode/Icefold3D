
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

	///
	constexpr auto cxIAVertexAttributeIndexUndefined = Cppx::QLimits<input_assembler_index_t>::sMaxValue;

	///
	constexpr auto cxIAVertexAttributeOffsetAppend = QLimits<vertex_attribute_offset_t>::sMaxValue;

	///
	constexpr auto cxIAVertexAttributeOffsetAppend16 = static_numeric_cast<vertex_attribute_offset_t>( cxIAVertexAttributeOffsetAppend - 1 );

	///
	constexpr auto cxIAVertexBufferBindingIndexUndefined = QLimits<input_assembler_index_t>::sMaxValue;

	///
	constexpr auto cxIAVertexStreamInstanceRateUndefined = QLimits<uint16>::sMaxValue;

	///
	constexpr auto cxIAVertexStreamIndexUndefined = cxIAVertexBufferBindingIndexUndefined;

	///
	constexpr auto cxIAVertexAttributeMaskAll = Cppx::makeLSFBitmask<input_assembler_index_t>( GCM::cxIAMaxVertexAttributesNum );

	///
	constexpr auto cxIAVertexBufferBindingMaskAll = Cppx::makeLSFBitmask<input_assembler_index_t>( GCM::cxIAMaxVertexBufferBindingsNum );

	///
	constexpr auto cxIAInputStreamBindingMaskAll = cxIAVertexBufferBindingMaskAll | 0x10000u;

	namespace CxDef
	{

		/// @brief
		inline constexpr bool isIAVertexAttributeIndexValid( native_uint pIndex )
		{
			return pIndex < GCM::cxIAMaxVertexAttributesNum;
		}

		/// @brief
		inline constexpr uint32 makeIAVertexAttributeFlag( native_uint pAttribIndex )
		{
			return ( pAttribIndex < GCM::cxIAMaxVertexAttributesNum ) ? ( 1 << static_cast<input_assembler_index_t>( pAttribIndex ) ) : 0u;
		}

		/// @brief
		inline constexpr bool isIAVertexBufferIndexValid( native_uint pStreamIndex )
		{
			return pStreamIndex < GCM::cxIAMaxVertexBufferBindingsNum;
		}

		/// @brief Returns
		inline constexpr uint32 makeIAVertexBufferFlag( native_uint pStreamIndex )
		{
			return ( pStreamIndex < GCM::cxIAMaxVertexBufferBindingsNum ) ? ( 1 << static_cast<input_assembler_index_t> ( pStreamIndex ) ) : 0u;
		}

		/// @brief Returns
		inline constexpr uint32 makeIAIndexBufferFlag()
		{
			return ( 1 << static_cast<input_assembler_index_t>( GCM::cxIAMaxVertexBufferBindingsNum ) );
		}

	}

	/// @brief
	enum EIAVertexAttributeFlags : uint32
	{
		eIAVertexAttributeFlagAttr0Bit = CxDef::makeIAVertexAttributeFlag( 0 ),
		eIAVertexAttributeFlagAttr1Bit = CxDef::makeIAVertexAttributeFlag( 1 ),
		eIAVertexAttributeFlagAttr2Bit = CxDef::makeIAVertexAttributeFlag( 2 ),
		eIAVertexAttributeFlagAttr3Bit = CxDef::makeIAVertexAttributeFlag( 3 ),
		eIAVertexAttributeFlagAttr4Bit = CxDef::makeIAVertexAttributeFlag( 4 ),
		eIAVertexAttributeFlagAttr5Bit = CxDef::makeIAVertexAttributeFlag( 5 ),
		eIAVertexAttributeFlagAttr6Bit = CxDef::makeIAVertexAttributeFlag( 6 ),
		eIAVertexAttributeFlagAttr7Bit = CxDef::makeIAVertexAttributeFlag( 7 ),
		eIAVertexAttributeMaskAll      = Cppx::makeLSFBitmask<uint32>( GCM::cxIAMaxVertexAttributesNum ),
	};

	/// @brief
	enum EIAVertexStreamBindingFlags : uint32
	{
		eIAVertexStreamBindingFlagVertexBuffer0Bit = CxDef::makeIAVertexBufferFlag( 0 ),
		eIAVertexStreamBindingFlagVertexBuffer1Bit = CxDef::makeIAVertexBufferFlag( 1 ),
		eIAVertexStreamBindingFlagVertexBuffer2Bit = CxDef::makeIAVertexBufferFlag( 2 ),
		eIAVertexStreamBindingFlagVertexBuffer3Bit = CxDef::makeIAVertexBufferFlag( 3 ),
		eIAVertexStreamBindingFlagVertexBuffer4Bit = CxDef::makeIAVertexBufferFlag( 4 ),
		eIAVertexStreamBindingFlagVertexBuffer5Bit = CxDef::makeIAVertexBufferFlag( 5 ),
		eIAVertexStreamBindingFlagVertexBuffer6Bit = CxDef::makeIAVertexBufferFlag( 6 ),
		eIAVertexStreamBindingFlagVertexBuffer7Bit = CxDef::makeIAVertexBufferFlag( 7 ),

		eIAVertexStreamBindingFlagIndexBufferBit = CxDef::makeIAIndexBufferFlag(),

		eIAVertexStreamBindingMaskVertexBufferAllBits =
			Cppx::makeLSFBitmask<uint32>( GCM::cxIAMaxVertexBufferBindingsNum ),

		eIAVertexStreamBindingMaskAll =
			eIAVertexStreamBindingMaskVertexBufferAllBits | eIAVertexStreamBindingFlagIndexBufferBit
	};

	/// @brief Definition of a vertex input attribute.
	struct IAVertexAttributeInfo
	{
		EVertexAttribFormat format{ EVertexAttribFormat::Undefined };

		std::string semanticName;

		input_assembler_index_t semanticIndex;

		input_assembler_index_t mStreamIndex{ cxIAVertexBufferBindingIndexUndefined };

		/// An offset of the attribute from the start of the vertex data.
		/// VERTEX_ATTRIBUTE_OFFSET_APPEND can be specified if the attribute is placed directly after previous one.
		uint16 relativeOffset;

		uint16 instanceRate;

		/// @brief Returns true if this instance represents a valid vertex attribute.
		IC3_ATTR_NO_DISCARD bool active() const noexcept
		{
			return !semanticName.empty() && CxDef::isIAVertexBufferIndexValid( mStreamIndex ) && (format != EVertexAttribFormat::Undefined );
		}
	};

	/// @brief Typedef for ordered, fixed-size array of vertex attribute definitions.
	using IAVertexAttributeInfoArray = std::array<IAVertexAttributeInfo, GCM::cxIAMaxVertexAttributesNum>;

	/// @brief
	struct IAInputLayoutDefinition
	{
		EPrimitiveTopology primitiveTopology;
		TBitmask<EIAVertexAttributeFlags> activeAttributesMask;
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
	using IAVertexBufferReferenceArray = std::array<IAVertexBufferReference, GCM::cxIAMaxVertexBufferBindingsNum>;

	/// @brief
	struct IAVertexStreamDefinition
	{
		TBitmask<EIAVertexStreamBindingFlags> activeBindingsMask;
		IAVertexBufferReferenceArray vertexBufferReferences;
		IAIndexBufferReference indexBufferReference;
	};

	/// @brief Describes a single, continuous range of vertex buffer binding points in the vertex stream.
	/// Used by drivers with support for range-based binding (D3D11, GL4) to reduce the number of API calls.
	struct IAVertexBufferRange
	{
		/// First index of the vertex buffer binding this range defines.
		input_assembler_index_t firstIndex = cxIAVertexBufferBindingIndexUndefined;

		/// Length of this range, i.e. number of vertex buffers within the range.
		uint16 length = 0;
	};

	using IAVertexBufferBindingIndexList = std::vector<input_assembler_index_t>;

	using IAVertexBufferRangeList = std::vector<IAVertexBufferRange>;

	// State Management Utility API
	namespace SMU
	{

		/// @brief Returns
		IC3_GRAPHICS_GCI_API_NO_DISCARD TBitmask<EIAVertexAttributeFlags> getIAVertexInputActiveAttributesMask(
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
