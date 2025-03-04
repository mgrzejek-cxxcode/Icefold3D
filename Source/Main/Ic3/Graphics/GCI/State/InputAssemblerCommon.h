
#pragma once

#ifndef __IC3_GRAPHICS_GCI_INPUT_ASSEMBLER_COMMON_H__
#define __IC3_GRAPHICS_GCI_INPUT_ASSEMBLER_COMMON_H__

#include "CommonGPUStateDefs.h"
#include "../Resources/ShaderCommon.h"
#include "../Resources/GPUBufferReference.h"

namespace Ic3::Graphics::GCI
{

	Ic3GCIDeclareClassHandle( GPUBuffer );

	/**
	 *
	 */
	using input_assembler_index_t = uint8;

	/**
	 *
	 */
	using vertex_attribute_offset_t = uint32;

	/**
	 *
	 */
	using vertex_attribute_padding_t = uint8;


	/**
	 * Represents an invalid vertex attribute slot index.
	 */
	constexpr auto kIAVertexAttributeSlotUndefined = cppx::meta::limits<input_assembler_index_t>::max_value;

	/**
	 *
	 */
	constexpr auto kIAVertexAttributeOffsetAppend = cppx::meta::limits<vertex_attribute_offset_t>::max_value;

	/**
	 *
	 */
	constexpr auto kIAVertexAttributeOffsetInvalid = cppx::meta::limits<vertex_attribute_offset_t>::max_value - 2;

	/**
	 *
	 */
	constexpr auto kIAVertexAttributePaddingAlign16 = cppx::meta::limits<vertex_attribute_padding_t>::max_value;

	/**
	 * Represents an invalid vertex stream slot index.
	 */
	constexpr auto kIAVertexStreamSlotUndefined = cppx::meta::limits<input_assembler_index_t>::max_value;

	/**
	 *
	 */
	enum EIAVertexAttributeDataRate : uint8
	{
		PerVertex,
		PerInstance,
		Undefined = cppx::meta::limits<uint8>::max_value
	};

	/**
	 * Definition of a vertex input attribute.
	 */
	#pragma pack( push, 1 )
	struct __attribute__(( packed )) IAVertexAttributeInfo
	{
		/**
		 * Base attribute index. Allowed values are from 0 to (GCM::kIAMaxVertexAttributesNum - 1).
		 * For multi-component attributes, this is the index of the first occupied attribute slot.
		 */
		input_assembler_index_t attributeSlot = kIAVertexAttributeSlotUndefined;

		/**
		 * Format of the attribute's data.
		 */
		EVertexAttribFormat dataFormat = EVertexAttribFormat::Undefined;

		/**
		 *
		 */
		EIAVertexAttributeDataRate dataRate = EIAVertexAttributeDataRate::Undefined;

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return IsActive();
		}

		/**
		 * @brief Returns true if this instance represents a valid vertex attribute.
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD bool IsActive() const noexcept
		{
			return CXU::IAIsVertexAttributeSlotValid( attributeSlot ) && ( dataFormat != EVertexAttribFormat::Undefined );
		}

		/**
		 * @brief
		 */
		void Reset()
		{
			attributeSlot = kIAVertexStreamSlotUndefined;
			dataFormat = EVertexAttribFormat::Undefined;
		}
	};
	#pragma pack( pop )

	#pragma pack( push, 1 )
	struct __attribute__(( packed )) IAVertexAttributeSemantics
	{
		/**
		 * Semantic name of the attribute. This is the name that identifies the attribute in the shader.
		 * Multiple attributes can share the same name (meaning they are part of the same semantic group),
		 * as long as their semanticIndex is different. Attribute with the same semantic must be placed
		 * in an adjacent slot range (no gaps allowed).
		 */
		cppx::immutable_string semanticName;

		/**
		 * Semantic index of the attribute. Semantic group is a group of 2, 3 or 4 attributes that
		 * share the same semantic name. Semantic group occupies continuous range of IA slots and
		 * each of the attributes must have a different semantic index.
		 * An example could be an 4x4 instance matrix, that would be stored as 4 vertex attributes,
		 * each of type Vec4 and with semantic indices 0, 1, 2 and 3 (and identical semantic name).
		 */
		input_assembler_index_t semanticIndex = 0;

		/**
		 *
		 */
		void Reset()
		{
			semanticName.clear();
			semanticIndex = 0;
		}
	};
	#pragma pack( pop )

	#pragma pack( push, 1 )
	struct __attribute__(( packed )) IAVertexAttributeStreamBinding
	{
		/**
		 *
		 */
		uint32 streamRelativeOffset = 0;

		/**
		 * An index of a vertex buffer slot this attribute is fetched from.
		 */
		input_assembler_index_t streamSlot = kIAVertexStreamSlotUndefined;

		void Reset()
		{
			streamRelativeOffset = 0;
			streamSlot = kIAVertexStreamSlotUndefined;
		}
	};
	#pragma pack( pop )

	struct IAVertexAttributeDesc
	{
		IAVertexAttributeInfo attribInfo;
		IAVertexAttributeSemantics semantics;
		IAVertexAttributeStreamBinding streamBinding;

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return IsActive();
		}

		CPPX_ATTR_NO_DISCARD bool IsActive() const noexcept
		{
			return attribInfo.IsActive();
		}

		void Reset()
		{
			attribInfo.Reset();
			semantics.Reset();
			streamBinding.Reset();
		}
	};

	/// @brief Typedef for ordered, fixed-size array of vertex attribute definitions.
	using IAVertexAttributeDescArray = std::array<IAVertexAttributeDesc, GCM::kIAMaxVertexAttributesNum>;

	/**
	 *
	 */
	struct IAVertexAttributeLayoutCommonConfig
	{
		/**
		 * Number of active attributes enabled for the IA stage.
		 */
		native_uint activeAttributesNum = 0;

		/**
		 * Active attributes mask. It contains all bits corresponding to attributes active as part of this descriptor.
		 * @see EIAVertexAttributeFlags
		 */
		cppx::bitmask<EIAVertexAttributeFlags> activeAttributesMask;

		/**
		 * Primitive topology used to render vertices.
		 * @see EPrimitiveTopology
		 */
		EPrimitiveTopology primitiveTopology = EPrimitiveTopology::Undefined;

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
	struct IAVertexAttributeLayoutDefinition : public IAVertexAttributeLayoutCommonConfig
	{
		IAVertexAttributeDescArray attributeArray;

		void Reset() noexcept
		{
			for( auto & attributeDesc : attributeArray )
			{
				attributeDesc.Reset();
			}

			ResetActiveAttributesInfo();
		}
	};

	struct IAVertexStreamBufferReference
	{
		GPUBufferReference sourceBuffer;
		gpu_memory_size_t relativeOffset = 0;

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
		EIndexDataFormat indexFormat = EIndexDataFormat::Undefined;
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
		gpu_memory_size_t vertexStride = 0;
	};

	/**
	 *
	 */
	struct IAVertexBufferReference : public IAVertexStreamBufferReference
	{
		IAVertexBufferRefParams refParams;
	};

	/// @brief
	using IAVertexBufferReferenceArray = std::array<IAVertexBufferReference, GCM::kIAMaxDataStreamVertexBuffersNum>;

	/**
	 *
	 */
	struct IAVertexSourceBindingCommonConfig
	{
		cppx::bitmask<EVertexSourceBindingFlags> activeStreamsMask;

		uint32 activeStreamsNum = 0;

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
			const auto vertexBufferBit = CXU::IAMakeVertexSourceVertexBufferBindingFlag( pVertexBufferIndex );
			return activeStreamsMask.is_set( vertexBufferBit );
		}
	};

	/**
	 *
	 */
	struct IAVertexSourceBindingDefinition : public IAVertexSourceBindingCommonConfig
	{
		IAVertexBufferReferenceArray vertexBufferReferences;

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

	struct VertexAttributeLayoutDescriptorCreateInfo : public PipelineStateDescriptorCreateInfoBase
	{
		IAVertexAttributeLayoutDefinition layoutDefinition;

		Shader * vertexShaderWithBinary = nullptr;

		CPPX_ATTR_NO_DISCARD virtual bool Validate() const noexcept override final;

		CPPX_ATTR_NO_DISCARD pipeline_config_hash_t GetConfigHash() const noexcept
		{
			return cppx::hash_compute<pipeline_config_hash_t::hash_algo>( layoutDefinition );
		}
	};

	struct VertexSourceBindingDescriptorCreateInfo : public PipelineStateDescriptorCreateInfoBase
	{
		IAVertexSourceBindingDefinition bindingDefinition;

		CPPX_ATTR_NO_DISCARD pipeline_config_hash_t GetConfigHash() const noexcept
		{
			return cppx::hash_compute<pipeline_config_hash_t::hash_algo>( bindingDefinition );
		}
	};

	/**
	 * Describes a single, continuous range of vertex buffer binding points in the vertex stream.
	 * Used by drivers with support for range-based binding (D3D11, GL4) to reduce the number of API calls.
	 */
	struct IAVertexStreamArrayRange
	{
		/// First index of the vertex buffer binding this range defines.
		input_assembler_index_t firstIndex = kIAVertexStreamSlotUndefined;

		/// Length of this range, i.e. number of vertex buffers within the range.
		uint16 length = 0;
	};

	/**
	 * An alias for a dynamic array with the bound vertex buffer indices.
	 */
	using VertexSourceBindingIndexList = std::vector<input_assembler_index_t>;

	/**
	 * An alias for a dynamic array with the ranges of bound vertex buffers.
	 */
	using VertexSourceBindingRageList = std::vector<IAVertexStreamArrayRange>;

	namespace GCU
	{

		/// @brief Returns
		IC3_GRAPHICS_GCI_API_NO_DISCARD cppx::bitmask<EIAVertexAttributeFlags> IAGetActiveVertexAttributesMask(
				const IAVertexAttributeDescArray & pVertexAttributes ) noexcept;

		/// @brief
		IC3_GRAPHICS_GCI_API_NO_DISCARD uint32 IAGetActiveVertexAttributesNum(
				const IAVertexAttributeDescArray & pVertexAttributes ) noexcept;

		/// @brief
		IC3_GRAPHICS_GCI_API_NO_DISCARD uint32 IAGetActiveVertexBuffersNum(
				const IAVertexBufferReferenceArray & pVertexBufferReferences ) noexcept;

		/// @brief
		IC3_GRAPHICS_GCI_API_NO_DISCARD VertexSourceBindingIndexList IAGenerateActiveVertexBuffersIndexList(
				const IAVertexBufferReferenceArray & pVertexBufferReferences ) noexcept;

		/// @brief
		IC3_GRAPHICS_GCI_API_NO_DISCARD VertexSourceBindingRageList IAGenerateActiveVertexBuffersRanges(
				const IAVertexBufferReferenceArray & pVertexBufferReferences ) noexcept;

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_INPUT_ASSEMBLER_COMMON_H__
