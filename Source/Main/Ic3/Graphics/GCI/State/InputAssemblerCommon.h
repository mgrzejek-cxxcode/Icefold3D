
#pragma once

#ifndef __IC3_GRAPHICS_GCI_INPUT_ASSEMBLER_COMMON_H__
#define __IC3_GRAPHICS_GCI_INPUT_ASSEMBLER_COMMON_H__

#include "CommonGPUStateDefs.h"
#include "../Resources/ShaderCommon.h"
#include "../Resources/GPUBufferReference.h"

namespace Ic3::Graphics::GCI
{

	Ic3GCIDeclareClassHandle( GPUBuffer );

	/// @brief
	using input_assembler_index_t = uint8;

	/// @brief
	using vertex_attribute_offset_t = uint16;

	///
	constexpr auto kIAVertexAttributeIndexUndefined = cppx::meta::limits<input_assembler_index_t>::max_value;

	///
	constexpr auto kIAVertexAttributeOffsetAppend = cppx::meta::limits<vertex_attribute_offset_t>::max_value;

	///
	constexpr auto kIAVertexStreamIndexUndefined = cppx::meta::limits<input_assembler_index_t>::max_value;


	struct IAVertexAttributeSemantics
	{
		std::string_view semanticName;
		input_assembler_index_t semanticIndex = 0;

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return !IsEmpty();
		}

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept
		{
			return semanticName.empty();
		}

		void Clear()
		{
			semanticName = {};
			semanticIndex = 0;
		}
	};

	/**
	 * Definition of a vertex input attribute.
	 */
	struct IAVertexAttributeInfo
	{
		IAVertexAttributeSemantics inputSemantics;

		EVertexAttribFormat format = EVertexAttribFormat::Undefined;

		input_assembler_index_t streamIndex = kIAVertexStreamIndexUndefined;

		/// An offset of the attribute from the start of the vertex data.
		/// VERTEX_ATTRIBUTE_OFFSET_APPEND can be specified if the attribute is placed directly after previous one.
		uint16 relativeOffset;

		uint16 instanceRate;

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return IsActive();
		}

		/// @brief Returns true if this instance represents a valid vertex attribute.
		CPPX_ATTR_NO_DISCARD bool IsActive() const noexcept
		{
			return inputSemantics && ( format != EVertexAttribFormat::Undefined ) && CXU::IAIsDataStreamVertexBufferIndexValid( streamIndex );
		}

		void Reset()
		{
			inputSemantics.Clear();
			format = EVertexAttribFormat::Undefined;
			streamIndex = kIAVertexStreamIndexUndefined;
		}
	};

	/// @brief Typedef for ordered, fixed-size array of vertex attribute definitions.
	using IAVertexAttributeInfoArray = std::array<IAVertexAttributeInfo, GCM::kIAMaxVertexAttributesNum>;

	/**
	 *
	 */
	struct IAInputLayoutCommonConfig
	{
		/**
		 * Active attributes mask. It contains all bits corresponding to attributes active as part of this descriptor.
		 * @see EIAVertexAttributeFlags
		 */
		cppx::bitmask<EIAVertexAttributeFlags> activeAttributesMask;

		/**
		 * Number of active attributes enabled for the IA stage.
		 */
		native_uint activeAttributesNum;

		/**
		 * Primitive topology used to render vertices.
		 * @see EPrimitiveTopology
		 */
		EPrimitiveTopology primitiveTopology;

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return !IsEmpty();
		}

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept
		{
			return activeAttributesMask.empty() || ( activeAttributesNum == 0 );
		}

		void ResetActiveAttributesInfo() noexcept
		{
			activeAttributesMask.clear();
			activeAttributesNum = 0;
		}

		CPPX_ATTR_NO_DISCARD bool IsAttributeActive( native_uint pAttributeIndex ) const noexcept
		{
			const auto attributeBit = CXU::IAMakeVertexAttributeFlag( pAttributeIndex );
			return activeAttributesMask.is_set( attributeBit );
		}
	};

	/**
	 *
	 */
	struct IAInputLayoutDefinition : public IAInputLayoutCommonConfig
	{
		IAVertexAttributeInfoArray attributeArray;

		void Reset() noexcept
		{
			for( auto & attributeInfo : attributeArray )
			{
				attributeInfo.Reset();
			}

			ResetActiveAttributesInfo();
		}
	};

	struct IAVertexStreamBufferReference
	{
		GPUBufferReference sourceBuffer;
		gpu_memory_size_t relativeOffset;

		explicit operator bool() const noexcept
		{
			return !IsEmpty();
		}

		bool IsEmpty() const noexcept
		{
			return sourceBuffer.IsEmpty();
		}

		gpu_memory_size_t DataOffset() const noexcept
		{
			return sourceBuffer.GetRefSubRegion().offset + relativeOffset;
		}

		void Reset()
		{
			sourceBuffer.Reset();
			relativeOffset = 0;
		}
	};

	/**
	 *
	 */
	struct IAIndexBufferRefParams
	{
		EIndexDataFormat indexFormat;
	};

	/**
	 *
	 */
	struct IAIndexBufferReference : public IAVertexStreamBufferReference
	{
		IAIndexBufferRefParams refParams;
	};

	/**
	 *
	 */
	struct IAVertexBufferRefParams
	{
		gpu_memory_size_t vertexStride;
	};

	/**
	 *
	 */
	struct IAVertexBufferReference : public IAVertexStreamBufferReference
	{
		IAVertexBufferRefParams refParams;
	};

	/// @brief
	using IAVertexStreamVertexBufferReferenceArray = std::array<IAVertexBufferReference, GCM::kIAMaxDataStreamVertexBuffersNum>;

	/**
	 *
	 */
	struct IAVertexStreamCommonConfig
	{
		cppx::bitmask<EIAVertexStreamBindingFlags> activeStreamsMask;

		uint32 activeStreamsNum;

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return !IsEmpty();
		}

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept
		{
			return activeStreamsMask.empty() || ( activeStreamsNum == 0 );
		}

		void ResetActiveStreamsInfo() noexcept
		{
			activeStreamsMask.clear();
			activeStreamsNum = 0;
		}

		CPPX_ATTR_NO_DISCARD bool IsVertexBufferActive( native_uint pVertexBufferIndex ) const noexcept
		{
			const auto vertexBufferBit = CXU::IAMakeDataStreamVertexBufferBindingFlag( pVertexBufferIndex );
			return activeStreamsMask.is_set( vertexBufferBit );
		}
	};

	/**
	 *
	 */
	struct IAVertexStreamConfiguration : public IAVertexStreamCommonConfig
	{
		IAVertexStreamVertexBufferReferenceArray vertexBufferReferences;

		IAIndexBufferReference indexBufferReference;

		void Reset() noexcept
		{
			for( auto & vertexBufferRef : vertexBufferReferences )
			{
				vertexBufferRef.Reset();
			}
			indexBufferReference.Reset();

			ResetActiveStreamsInfo();
		}
	};

	/**
	 * Describes a single, continuous range of vertex buffer binding points in the vertex stream.
	 * Used by drivers with support for range-based binding (D3D11, GL4) to reduce the number of API calls.
	 */
	struct IAVertexStreamArrayRange
	{
		/// First index of the vertex buffer binding this range defines.
		input_assembler_index_t firstIndex = kIAVertexStreamIndexUndefined;

		/// Length of this range, i.e. number of vertex buffers within the range.
		uint16 length = 0;
	};

	/**
	 * An alias for a dynamic array with the bound vertex buffer indices.
	 */
	using IAVertexStreamBindingIndexList = std::vector<input_assembler_index_t>;

	/**
	 * An alias for a dynamic array with the ranges of bound vertex buffers.
	 */
	using IAVertexStreamBindingRageList = std::vector<IAVertexStreamArrayRange>;

	namespace GCU
	{

		/// @brief Returns
		IC3_GRAPHICS_GCI_API_NO_DISCARD cppx::bitmask<EIAVertexAttributeFlags> IAGetActiveVertexAttributesMask(
				const IAVertexAttributeInfoArray & pVertexAttributes ) noexcept;

		/// @brief
		IC3_GRAPHICS_GCI_API_NO_DISCARD uint32 IAGetActiveVertexAttributesNum(
				const IAVertexAttributeInfoArray & pVertexAttributes ) noexcept;

		/// @brief
		IC3_GRAPHICS_GCI_API_NO_DISCARD uint32 IAGetActiveVertexBuffersNum(
				const IAVertexStreamVertexBufferReferenceArray & pVertexBufferReferences ) noexcept;

		/// @brief
		IC3_GRAPHICS_GCI_API_NO_DISCARD IAVertexStreamBindingIndexList IAGenerateActiveVertexBuffersIndexList(
				const IAVertexStreamVertexBufferReferenceArray & pVertexBufferReferences ) noexcept;

		/// @brief
		IC3_GRAPHICS_GCI_API_NO_DISCARD IAVertexStreamBindingRageList IAGenerateActiveVertexBuffersRanges(
				const IAVertexStreamVertexBufferReferenceArray & pVertexBufferReferences ) noexcept;

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_INPUT_ASSEMBLER_COMMON_H__
