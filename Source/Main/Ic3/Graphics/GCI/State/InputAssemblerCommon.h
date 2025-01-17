
#pragma once

#ifndef __IC3_GRAPHICS_GCI_INPUT_ASSEMBLER_COMMON_H__
#define __IC3_GRAPHICS_GCI_INPUT_ASSEMBLER_COMMON_H__

#include "CommonGpuStateDefs.h"
#include "../Resources/ShaderCommon.h"
#include "../Resources/GpuBufferReference.h"

namespace Ic3::Graphics::GCI
{

	Ic3GCIDeclareClassHandle( GpuBuffer );

	/// @brief
	using input_assembler_index_t = uint8;

	/// @brief
	using vertex_attribute_offset_t = uint16;

	///
	constexpr auto cxIAVertexAttributeIndexUndefined = cppx::meta::limits<input_assembler_index_t>::max_value;

	///
	constexpr auto cxIAVertexAttributeOffsetAppend = cppx::meta::limits<vertex_attribute_offset_t>::max_value;

	///
	constexpr auto cxIAVertexAttributeOffsetAppend16 = cppx::static_numeric_cast<vertex_attribute_offset_t>( cxIAVertexAttributeOffsetAppend - 1 );

	///
	constexpr auto cxIAVertexStreamIndexUndefined = cppx::meta::limits<input_assembler_index_t>::max_value;

	///
	constexpr auto cxIAVertexAttributeMaskAll = cppx::make_lsfb_bitmask<input_assembler_index_t>( GCM::cxIAMaxVertexAttributesNum );

	///
	constexpr auto cxIAVertexBufferBindingMaskAll = cppx::make_lsfb_bitmask<input_assembler_index_t>( GCM::cxIAMaxVertexBufferBindingsNum );

	///
	constexpr auto cxIAInputStreamBindingMaskAll = cxIAVertexBufferBindingMaskAll | 0x10000u;

	namespace CxDef
	{

		/// @brief
		inline constexpr bool IsIAVertexAttributeIndexValid( native_uint pIndex )
		{
			return pIndex < GCM::cxIAMaxVertexAttributesNum;
		}

		/// @brief
		inline constexpr uint32 makeIAVertexAttributeFlag( native_uint pAttribIndex )
		{
			return ( pAttribIndex < GCM::cxIAMaxVertexAttributesNum ) ? ( 1 << static_cast<input_assembler_index_t>( pAttribIndex ) ) : 0u;
		}

		/// @brief
		inline constexpr bool IsIAVertexBufferIndexValid( native_uint pStreamIndex )
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
	enum EIAVertexAttributeFlags : uint16
	{
		eIAVertexAttributeFlagAttr0Bit  = CxDef::makeIAVertexAttributeFlag( 0 ),
		eIAVertexAttributeFlagAttr1Bit  = CxDef::makeIAVertexAttributeFlag( 1 ),
		eIAVertexAttributeFlagAttr2Bit  = CxDef::makeIAVertexAttributeFlag( 2 ),
		eIAVertexAttributeFlagAttr3Bit  = CxDef::makeIAVertexAttributeFlag( 3 ),
		eIAVertexAttributeFlagAttr4Bit  = CxDef::makeIAVertexAttributeFlag( 4 ),
		eIAVertexAttributeFlagAttr5Bit  = CxDef::makeIAVertexAttributeFlag( 5 ),
		eIAVertexAttributeFlagAttr6Bit  = CxDef::makeIAVertexAttributeFlag( 6 ),
		eIAVertexAttributeFlagAttr7Bit  = CxDef::makeIAVertexAttributeFlag( 7 ),
		eIAVertexAttributeFlagAttr8Bit  = CxDef::makeIAVertexAttributeFlag( 8 ),
		eIAVertexAttributeFlagAttr9Bit  = CxDef::makeIAVertexAttributeFlag( 9 ),
		eIAVertexAttributeFlagAttr10Bit = CxDef::makeIAVertexAttributeFlag( 10 ),
		eIAVertexAttributeFlagAttr11Bit = CxDef::makeIAVertexAttributeFlag( 11 ),
		eIAVertexAttributeFlagAttr12Bit = CxDef::makeIAVertexAttributeFlag( 12 ),
		eIAVertexAttributeFlagAttr13Bit = CxDef::makeIAVertexAttributeFlag( 13 ),
		eIAVertexAttributeFlagAttr14Bit = CxDef::makeIAVertexAttributeFlag( 14 ),
		eIAVertexAttributeFlagAttr15Bit = CxDef::makeIAVertexAttributeFlag( 15 ),
		eIAVertexAttributeMaskAll       = cppx::make_lsfb_bitmask<uint32>( GCM::cxIAMaxVertexAttributesNum ),
	};

	/// @brief
	enum EIAVertexStreamBindingFlags : uint32
	{
		eIAVertexStreamBindingFlagVertexBuffer0Bit  = CxDef::makeIAVertexBufferFlag( 0 ),
		eIAVertexStreamBindingFlagVertexBuffer1Bit  = CxDef::makeIAVertexBufferFlag( 1 ),
		eIAVertexStreamBindingFlagVertexBuffer2Bit  = CxDef::makeIAVertexBufferFlag( 2 ),
		eIAVertexStreamBindingFlagVertexBuffer3Bit  = CxDef::makeIAVertexBufferFlag( 3 ),
		eIAVertexStreamBindingFlagVertexBuffer4Bit  = CxDef::makeIAVertexBufferFlag( 4 ),
		eIAVertexStreamBindingFlagVertexBuffer5Bit  = CxDef::makeIAVertexBufferFlag( 5 ),
		eIAVertexStreamBindingFlagVertexBuffer6Bit  = CxDef::makeIAVertexBufferFlag( 6 ),
		eIAVertexStreamBindingFlagVertexBuffer7Bit  = CxDef::makeIAVertexBufferFlag( 7 ),
		eIAVertexStreamBindingFlagVertexBuffer8Bit  = CxDef::makeIAVertexBufferFlag( 8 ),
		eIAVertexStreamBindingFlagVertexBuffer9Bit  = CxDef::makeIAVertexBufferFlag( 9 ),
		eIAVertexStreamBindingFlagVertexBuffer10Bit = CxDef::makeIAVertexBufferFlag( 10 ),
		eIAVertexStreamBindingFlagVertexBuffer11Bit = CxDef::makeIAVertexBufferFlag( 11 ),
		eIAVertexStreamBindingFlagVertexBuffer12Bit = CxDef::makeIAVertexBufferFlag( 12 ),
		eIAVertexStreamBindingFlagVertexBuffer13Bit = CxDef::makeIAVertexBufferFlag( 13 ),
		eIAVertexStreamBindingFlagVertexBuffer14Bit = CxDef::makeIAVertexBufferFlag( 14 ),
		eIAVertexStreamBindingFlagVertexBuffer15Bit = CxDef::makeIAVertexBufferFlag( 15 ),

		eIAVertexStreamBindingFlagIndexBufferBit = CxDef::makeIAIndexBufferFlag(),

		eIAVertexStreamBindingMaskVertexBufferAllBits =
			cppx::make_lsfb_bitmask<uint32>( GCM::cxIAMaxVertexBufferBindingsNum ),

		eIAVertexStreamBindingMaskAll =
			eIAVertexStreamBindingMaskVertexBufferAllBits | eIAVertexStreamBindingFlagIndexBufferBit
	};

	struct IAVertexAttributeSemantics
	{
		std::string_view SemanticName;
		input_assembler_index_t SemanticIndex = 0;

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept
		{
			return SemanticName.empty();
		}

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return !IsEmpty();
		}
	};

	/// @brief Definition of a vertex input attribute.
	struct IAVertexAttributeInfo
	{
		IAVertexAttributeSemantics inputSemantics;

		EVertexAttribFormat format = EVertexAttribFormat::Undefined;

		input_assembler_index_t streamIndex = cxIAVertexStreamIndexUndefined;

		/// An offset of the attribute from the start of the vertex data.
		/// VERTEX_ATTRIBUTE_OFFSET_APPEND can be specified if the attribute is placed directly after previous one.
		uint16 relativeOffset;

		uint16 instanceRate;

		/// @brief Returns true if this instance represents a valid vertex attribute.
		CPPX_ATTR_NO_DISCARD bool IsActive() const noexcept
		{
			return inputSemantics && CxDef::IsIAVertexBufferIndexValid( streamIndex ) && ( format != EVertexAttribFormat::Undefined );
		}
	};

	/// @brief Typedef for ordered, fixed-size array of vertex attribute definitions.
	using IAVertexAttributeInfoArray = std::array<IAVertexAttributeInfo, GCM::cxIAMaxVertexAttributesNum>;

	/// @brief
	struct IAInputLayoutDefinition
	{
		EPrimitiveTopology primitiveTopology;
		cppx::bitmask<EIAVertexAttributeFlags> activeAttributesMask;
		IAVertexAttributeInfoArray attributeArray;
	};

	struct IAVertexStreamBufferReference
	{
		GpuBufferReference sourceBuffer;
		gpu_memory_size_t relativeOffset;

		bool IsEmpty() const noexcept
		{
			return sourceBuffer.IsEmpty();
		}

		gpu_memory_size_t DataOffset() const noexcept
		{
			return sourceBuffer.GetRefSubRegion().offset + relativeOffset;
		}

		explicit operator bool() const noexcept
		{
			return !IsEmpty();
		}

		void Reset()
		{
			sourceBuffer.Reset();
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
		cppx::bitmask<EIAVertexStreamBindingFlags> activeBindingsMask;
		IAVertexBufferReferenceArray vertexBufferReferences;
		IAIndexBufferReference indexBufferReference;
	};

	/// @brief Describes a single, continuous range of vertex buffer binding points in the vertex stream.
	/// Used by drivers with support for range-based binding (D3D11, GL4) to reduce the number of API calls.
	struct IAVertexBufferRange
	{
		/// First index of the vertex buffer binding this range defines.
		input_assembler_index_t firstIndex = cxIAVertexStreamIndexUndefined;

		/// Length of this range, i.e. number of vertex buffers within the range.
		uint16 length = 0;
	};

	using IAVertexBufferBindingIndexList = std::vector<input_assembler_index_t>;

	using IAVertexBufferRangeList = std::vector<IAVertexBufferRange>;

	// State Management Utility API
	namespace SMU
	{

		/// @brief Returns
		IC3_GRAPHICS_GCI_API_NO_DISCARD cppx::bitmask<EIAVertexAttributeFlags> GetIAVertexInputActiveAttributesMask(
				const IAVertexAttributeInfoArray & pVertexAttributes ) noexcept;

		/// @brief
		IC3_GRAPHICS_GCI_API_NO_DISCARD uint32 GetIAVertexInputActiveAttributesNum(
				const IAVertexAttributeInfoArray & pVertexAttributes ) noexcept;

		/// @brief
		IC3_GRAPHICS_GCI_API_NO_DISCARD IAVertexBufferRangeList GenerateActiveVertexBufferRanges(
				const IAVertexBufferReferenceArray & pVBReferences ) noexcept;

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_INPUT_ASSEMBLER_COMMON_H__
