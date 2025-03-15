
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
	 *s
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
	constexpr auto kIAVertexAttributeOffsetInvalid = cppx::meta::limits<vertex_attribute_offset_t>::max_value;

	/**
	 * Represents an invalid vertex stream slot index.
	 */
	constexpr auto kIAVertexStreamSlotUndefined = cppx::meta::limits<input_assembler_index_t>::max_value;

	/**
	 *
	 */
	enum class EIAVertexAttributeDataRate : uint8
	{
		PerVertex,
		PerInstance,
		Undefined = cppx::meta::limits<uint8>::max_value
	};

	/**
	 * Part of the definition of a vertex input attribute. Contains basic information about
	 * the attribute - its slot, data format, etc. Together with IAVertexAttributeSemantics
	 * and IAVertexAttributeStreamBinding form a complete attribute definition (IAVertexAttributeDesc)
	 */
	struct IAVertexAttributeInfo
	{
		/**
		 * @brief Index of the input attribute array slot in the IA.
		 *
		 * Allowed values are from 0 to ( GCM::kIAMaxVertexAttributesNum - 1 ).
		 * Maximum number of available attributes is guaranteed to be at least 16.
		 */
		input_assembler_index_t attributeSlot = kIAVertexAttributeSlotUndefined;

		/**
		 * @brief Attribute data rate, determines whether an attribute is fetched per-vertex or per-instance.
		 */
		EIAVertexAttributeDataRate dataRate = EIAVertexAttributeDataRate::Undefined;

		/**
		 * 
		 */
		uint16 instanceStepRate = 0;

		/**
		 * @brief Format of the attribute's data.
		 */
		EVertexAttribFormat dataFormat = EVertexAttribFormat::Undefined;

		/**
		 * @brief Returns true if this instance represents a valid vertex attribute, or false otherwise.
		 * @return True if this instance represents a valid vertex attribute, or false otherwise.
		 */
		CPPX_ATTR_NO_DISCARD bool IsActive() const noexcept
		{
			return CXU::IAIsVertexAttributeSlotValid( attributeSlot ) && ( dataFormat != EVertexAttribFormat::Undefined );
		}
	};

	/**
	 * Part of the definition of a vertex input attribute. Contains information about semantics
	 * of the attribute. Together with IAVertexAttributeSemantics and IAVertexAttributeStreamBinding
	 * form a complete attribute definition (IAVertexAttributeDesc).
	 */
	struct IAVertexAttributeSemantics
	{
		/**
		 * Semantic name of the attribute. This is the name that identifies the attribute in the shader.
		 * Multiple attributes can share the same name (meaning they are either part of the same semantic
		 * group or have an indexed semantic name), as long as their semanticIndex is different.
		 * 
		 * @note
		 * It is an ERROR to specify an indexed semantic name via this member - GCI works with
		 * physical (per-slot) vertex attributes, so GCI semantic specification must be decoupled.
		 * For example, to describe texture coordinate with shader semantic name "TEXCOORD3":
		 * - semanticName must be set to "TEXCOORD"
		 * - semanticIndex must be set to "3"
		 * Otherwise, an error will be reported upon the attempt to create a VertexAttributeLayoutDescriptor
		 * using a configuration that breaks this rule.
		 */
		cppx::immutable_string semanticName;

		/**
		 * Semantic index of the attribute. This index differtiates attributes in two scenarios:
		 * 1) Semantic group: semantic group is a group of 2, 3 or 4 attributes that share the same
		 *    semantic name. Semantic group occupies continuous range of IA slots and each of the
		 *    attributes must have a different semantic index.
		 *    An example could be an 4x4 instance matrix, that would be stored as 4 vertex attributes,
		 *    each of type Vec4 and with semantic indices 0, 1, 2 and 3 (and identical semantic name).
		 * 2) Indexed semantics: a semantic name that contains the index embedded within itself
		 *    A typical example are tex coords (semantic names: TEXXCORD0, TEXCOORD1, TEXCOORD2, etc.)
		 *    which can be decoupled into the shared semantic name (TEXCOORD) and a semantic index (0, 1, 2).
		 */
		input_assembler_index_t semanticIndex = 0;
	};

	/**
	 * Part of the definition of a vertex input attribute. Contains information about vertex
	 * stream which contains the data for this attribute. Together with IAVertexAttributeSemantics
	 * and IAVertexAttributeStreamBinding form a complete attribute definition (IAVertexAttributeDesc).
	 */
	struct IAVertexAttributeStreamBinding
	{
		/**
		 * @brief An index of a vertex stream (vertex buffer slot) this attribute is fetched from.
		 * Multiple attributes can be associated with and streamed from a single vertex stream.
		 * In this case the stride of the stream is the sum of sizes of all attribues.
		 */
		input_assembler_index_t streamSlot = kIAVertexStreamSlotUndefined;

		/**
		 * An offset, in bytes, from the start of the buffer range bound to the associated stream
		 * to the beginning of the data for this attribute. Hence 'relative' - this offset is NOT
		 * from the "physical" beginning of the buffer, but the range which has been bound to the
		 * stream. These two offsets are then automatically combined to compute the final, absolute
		 * offset (this is done automatically by the GCI runtime).
		 * 
		 * @note
		 * The offset is consumed without any modifications or processing, that is, there are no
		 * special values like "Append" or "AppendAligned" (higher-level NxMain component has this
		 * feature, though).
		 */
		uint32 streamRelativeOffset = 0;
	};

	/**
	 * @brief Contains complete set of properties describing a single vertex input attribute.
	 * 
	 * @note
	 * GCI does not provide any (automated) way to define multi-slot attributes (aka semantic groups).
	 * It operates on a low-level, close-to-the-GPU terms, hence this struct ALWAYS represents a SINGLE
	 * attribute which occupies a SINGLE input assembler slot. It is up to the user to properly fill
	 * all the properties for multi-slot attributes.
	 */
	struct IAVertexAttributeDesc
	{
		/**
		 * @brief Basic attribute info
		 */
		IAVertexAttributeInfo attribInfo;

		/**
		 * @brief Semantics of the attribute
		 */
		IAVertexAttributeSemantics semantics;

		/**
		 * @brief Vertex stream association
		 */
		IAVertexAttributeStreamBinding vertexStreamBinding;

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return IsActive();
		}

		CPPX_ATTR_NO_DISCARD bool IsActive() const noexcept
		{
			return attribInfo.IsActive();
		}
	};

	/**
	 * @brief Alias for a fixed array of vertex attribute definitions. Contains elements for all supported IA slots.
	 */
	using IAVertexAttributeDescArray = std::array<IAVertexAttributeDesc, GCM::kIAMaxVertexAttributesNum>;

	/**
	 *
	 */
	struct IAVertexAttributeLayoutMetaData
	{
		/**
		 * Active attributes mask. It contains all bits corresponding to attributes active as part of this definition.
		 * @see EIAVertexAttributeFlags
		 */
		cppx::bitmask<EIAVertexAttributeFlags> activeAttributesMask;

		/**
		 * @brief Primitive topology - controls how multiple vertices are assembled into primitives during rendering.
		 */
		EPrimitiveTopology primitiveTopology = EPrimitiveTopology::Undefined;

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return !IsEmpty();
		}

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept
		{
			return activeAttributesMask.empty();
		}

		CPPX_ATTR_NO_DISCARD uint32 GetActiveAttributesNum() const noexcept
		{
			return ( activeAttributesMask & eIAVertexAttributeMaskAll ).count_bits();
		}

		void ResetActiveAttributesMask() noexcept
		{
			activeAttributesMask.clear();
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
	struct IAVertexAttributeLayoutDefinition : public IAVertexAttributeLayoutMetaData
	{
		IAVertexAttributeDescArray attributeArray;
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
		cppx::bitmask<EIAVertexSourceBindingFlags> activeStreamsMask;

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return !IsEmpty();
		}

		CPPX_ATTR_NO_DISCARD uint32 GetActiveBuffersNum() const noexcept
		{
			return ( activeStreamsMask & eIAVertexSourceBindingMaskAll ).count_bits();
		}

		CPPX_ATTR_NO_DISCARD uint32 GetActiveVertexBuffersNum() const noexcept
		{
			return ( activeStreamsMask & eIAVertexSourceBindingMaskVertexBufferAllBits ).count_bits();
		}

		CPPX_ATTR_NO_DISCARD bool IsIndexBufferActive() const noexcept
		{
			return activeStreamsMask.is_set( eIAVertexSourceBindingFlagIndexBufferBit  );
		}

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept
		{
			return activeStreamsMask.empty();
		}

		void ResetActiveStreamsMask() noexcept
		{
			activeStreamsMask.clear();
		}

		CPPX_ATTR_NO_DISCARD bool IsVertexBufferActive( native_uint pVertexBufferIndex ) const noexcept
		{
			const auto vertexBufferBit = CXU::IAMakeVertexBufferBindingFlag( pVertexBufferIndex );
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

			ResetActiveStreamsMask();
		}
	};

	struct VertexAttributeLayoutDescriptorCreateInfo : public PipelineStateDescriptorCreateInfoBase
	{
		IAVertexAttributeLayoutDefinition layoutDefinition;

		Shader * vertexShaderWithBinary = nullptr;

		IC3_GRAPHICS_GCI_API_NO_DISCARD virtual bool Validate() const noexcept override final;

		CPPX_ATTR_NO_DISCARD pipeline_config_hash_t GetConfigHash() const noexcept
		{
			return cppx::hash_compute<pipeline_config_hash_t::hash_algo>( layoutDefinition );
		}
	};

	struct VertexSourceBindingDescriptorCreateInfo : public PipelineStateDescriptorCreateInfoBase
	{
		IAVertexSourceBindingDefinition bindingDefinition;

		IC3_GRAPHICS_GCI_API_NO_DISCARD virtual bool Validate() const noexcept override final;

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
	using VertexSourceBindingRangeList = std::vector<IAVertexStreamArrayRange>;


	namespace Utilities
	{

		/**
		 * @brief 
		 * @param pVertexAttributes 
		 * @return 
		 */
		IC3_GRAPHICS_GCI_API_NO_DISCARD cppx::bitmask<EIAVertexAttributeFlags> IAGetActiveVertexAttributesMask(
				const IAVertexAttributeDescArray & pVertexAttributes ) noexcept;

		/**
		 * @brief 
		 * @param pVertexAttributes 
		 * @return 
		 */
		IC3_GRAPHICS_GCI_API_NO_DISCARD uint32 IAGetActiveVertexAttributesNum(
				const IAVertexAttributeDescArray & pVertexAttributes ) noexcept;

		/**
		 * @brief 
		 * @param pVertexBufferReferences 
		 * @return 
		 */
		IC3_GRAPHICS_GCI_API_NO_DISCARD uint32 IAGetActiveVertexBuffersNum(
				const IAVertexBufferReferenceArray & pVertexBufferReferences ) noexcept;

		/**
		 * @brief 
		 * @param pVertexBufferReferences 
		 * @return 
		 */
		IC3_GRAPHICS_GCI_API_NO_DISCARD VertexSourceBindingIndexList IAGenerateActiveVertexBuffersIndexList(
				const IAVertexBufferReferenceArray & pVertexBufferReferences ) noexcept;

		/**
		 * @brief 
		 * @param pVertexBufferReferences 
		 * @return 
		 */
		IC3_GRAPHICS_GCI_API_NO_DISCARD VertexSourceBindingRangeList IAGenerateActiveVertexBuffersRanges(
				const IAVertexBufferReferenceArray & pVertexBufferReferences ) noexcept;

		/**
		 * @brief 
		 * @param pAttributeBaseSlot 
		 * @param pAttributesNum 
		 * @return 
		 */
		inline bool IAIsAttributeSlotRangeValid( uint32 pAttributeBaseSlot, uint32 pAttributesNum = 1 )
		{
			return ( pAttributeBaseSlot != GCI::kIAVertexAttributeSlotUndefined ) &&
			       // Vertex attribute index should be in the valid range of supported values.
			       CXU::IAIsVertexAttributeSlotValid( pAttributeBaseSlot ) &&
			       //
			       ( pAttributesNum <= GCM::kIAMaxVertexAttributesNum ) &&
			       // Attributes can have multiple components (e.g. a 4x4 matrix is a 4-component attribute, with each component
			       // being a 4-element vector). Even though the base index is valid, we need to check all potential sub-attributes.
			       CXU::IAIsVertexAttributeSlotValid( pAttributeBaseSlot + pAttributesNum - 1 );
		}

		/**
		 * @brief 
		 * @param pAttributeBaseSlot 
		 * @param pSemanticGroupSize 
		 * @return 
		 */
		inline bool IAIsAttributeSemanticGroupValid( uint32 pAttributeBaseSlot, uint32 pSemanticGroupSize )
		{
			return // Each attribute has to have at least one component and no more than the GCI-level limit.
			       CXU::IAIsVertexAttributeSemanticGroupSizeValid( pSemanticGroupSize ) &&
			       //
			       IAIsAttributeSlotRangeValid( pAttributeBaseSlot, pSemanticGroupSize );
		}

		/**
		 * @brief 
		 * @tparam TPFunction 
		 * @param pActiveVertexBuffersMask 
		 * @param pFunction 
		 * @return 
		 */
		template <typename TPFunction>
		inline bool ForEachVertexBufferIndex( cppx::bitmask<EIAVertexSourceBindingFlags> pActiveVertexBuffersMask, TPFunction pFunction )
		{
			// A local copy of the active attachments mask. Bits of already processed attachments
			// are removed, so when the value reaches 0, we can immediately stop further processing.
			auto activeVertexBuffersMask = pActiveVertexBuffersMask & eIAVertexSourceBindingMaskVertexBufferAllBits;

			for( // Iterate over the valid vertex buffer index range.
			     native_uint vertexBufferIndex = 0;
			     CXU::IAIsDataStreamVertexBufferSlotValid( vertexBufferIndex ) && !activeVertexBuffersMask.empty();
			     ++vertexBufferIndex )
			{
				const auto vertexBufferBit = CXU::IAMakeVertexBufferBindingFlag( vertexBufferIndex );
				// Check if the attachments mask has this bit set.
				if( activeVertexBuffersMask.is_set( vertexBufferBit ) )
				{
					// The function returns false if there was some internal error condition
					// and the processing should be aborted.
					if( !pFunction( vertexBufferIndex, static_cast<EIAVertexSourceBindingFlags>( vertexBufferBit ) ) )
					{
						return false;
					}

					// Update the control mask.
					activeVertexBuffersMask.unset( vertexBufferBit );
				}
			}

			return true;
		}

		/**
		 * @brief 
		 * @tparam TPFunction 
		 * @param pFirstVertexBufferIndex 
		 * @param pVertexBufferCount 
		 * @param pActiveVertexBuffersMask 
		 * @param pFunction 
		 * @return 
		 */
		template <typename TPFunction>
		inline bool ForEachVertexBufferIndexInRange(
				native_uint pFirstVertexBufferIndex,
				native_uint pVertexBufferCount,
				cppx::bitmask<EIAVertexSourceBindingFlags> pActiveVertexBuffersMask,
				TPFunction pFunction )
		{
			// A local copy of the active attachments mask. Bits of already processed attachments
			// are removed, so when the value reaches 0, we can immediately stop further processing.
			auto activeVertexBuffersMask = pActiveVertexBuffersMask & eIAVertexSourceBindingMaskVertexBufferAllBits;

			const auto lastVertexBufferIndex = pFirstVertexBufferIndex + pVertexBufferCount - 1;

			for( // Iterate over the valid vertex buffer index range.
			     native_uint vertexBufferIndex = pFirstVertexBufferIndex;
			     CXU::IAIsDataStreamVertexBufferSlotValid( vertexBufferIndex ) && ( vertexBufferIndex < lastVertexBufferIndex ) && !activeVertexBuffersMask.empty();
			     ++vertexBufferIndex )
			{
				const auto vertexBufferBit = CXU::IAMakeVertexBufferBindingFlag( vertexBufferIndex );
				// Check if the attachments mask has this bit set.
				if( activeVertexBuffersMask.is_set( vertexBufferBit ) )
				{
					// The function returns false if there was some internal error condition
					// and the processing should be aborted.
					if( !pFunction( vertexBufferIndex, vertexBufferBit ) )
					{
						return false;
					}

					// Update the control mask.
					activeVertexBuffersMask.unset( vertexBufferBit );
				}
			}

			return true;
		}

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_INPUT_ASSEMBLER_COMMON_H__
