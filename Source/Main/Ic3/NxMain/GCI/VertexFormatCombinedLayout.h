
#pragma once

#ifndef __IC3_NXMAIN_VERTEX_FORMAT_COMBINED_LAYOUT_H__
#define __IC3_NXMAIN_VERTEX_FORMAT_COMBINED_LAYOUT_H__

#include "VertexFormatCommon.h"

namespace Ic3
{

	/**
	 * @brief Contains data representing a complete vertex input configuration:
	 * 
	 * The complete input data for vertex format is consisted of:
	 * - an array of GenericVertexInputAttributes, describing usage of every element of the IA attribute array,
	 * - an array of VertexInputStream with configuration of all IA vertex streams.
	 * 
	 * This is an intermediate struct created from a list of attribute definitions (VertexInputAttributeDefinition).
	 */
	struct VertexFormatCombinedLayoutBase
	{
		std::array<uint8, GCM::kIAMaxVertexAttributesNum> attributeIndexMapping;
		std::array<uint8, GCM::kIAMaxDataStreamVertexBuffersNum> streamIndexMapping;

		InputAssemblerSlotRange activeAttributesRange = InputAssemblerSlotRange::empty_range();
		InputAssemblerSlotRange activeStreamsRange = InputAssemblerSlotRange::empty_range();

		const uint8 attributeArraySize;
		const uint8 streamArraySize;

		uint8 activeAttributesNum = 0;
		uint8 activeBaseAttributesNum = 0;
		uint8 activeStreamsNum = 0;

		cppx::bitmask<GCI::EIAVertexAttributeFlags> activeAttributesMask;
		cppx::bitmask<GCI::EIAVertexSourceBindingFlags> activeStreamsMask;

		GenericVertexInputAttribute * const attributeArray = nullptr;
		input_assembler_slot_t * const attributeSlotArray = nullptr;

		VertexInputStream * const streamArray = nullptr;
		input_assembler_slot_t * const streamSlotArray = nullptr;

		VertexFormatCombinedLayoutBase(
				native_uint pAttributeArraySize,
				native_uint pStreamArraySize,
				GenericVertexInputAttribute * pAttributeArray,
				input_assembler_slot_t * pAttributeSlotArray,
				VertexInputStream * pStreamArray,
				input_assembler_slot_t * pStreamSlotArray )
		: attributeArraySize( cppx::numeric_cast<uint8>( pAttributeArraySize ) )
		, streamArraySize( cppx::numeric_cast<uint8>( pStreamArraySize ) )
		, attributeArray( pAttributeArray )
		, attributeSlotArray( pAttributeSlotArray )
		, streamArray( pStreamArray )
		, streamSlotArray( pStreamSlotArray )
		{
			attributeIndexMapping.fill( GCI::kIAVertexAttributeSlotUndefined );
			streamIndexMapping.fill( GCI::kIAVertexAttributeSlotUndefined );
		}

		CPPX_ATTR_NO_DISCARD bool Equals( const VertexFormatCombinedLayoutBase & pOther ) const noexcept
		{
			if( this == &pOther )
			{
				return true;
			}

			if( ( attributeArraySize != pOther.attributeArraySize ) || ( streamArraySize != pOther.streamArraySize ) )
			{
				return false;
			}

			if( ( activeAttributesNum != pOther.activeAttributesNum ) || ( activeStreamsNum != pOther.activeStreamsNum ) )
			{
				return false;
			}
			
			if( ( activeAttributesMask != pOther.activeAttributesMask ) || ( activeStreamsMask != pOther.activeStreamsMask ) )
			{
				return false;
			}
			
			if( ( activeAttributesRange != pOther.activeAttributesRange ) || ( activeStreamsRange != pOther.activeStreamsRange ) )
			{
				return false;
			}

			for( native_uint iAttributeArrayIndex = 0; iAttributeArrayIndex < activeAttributesNum; ++iAttributeArrayIndex )
			{
				if( !attributeArray[iAttributeArrayIndex].IsSameAs( pOther.attributeArray[iAttributeArrayIndex] ) )
				{
					return false;
				}
			}

			return true;
		}

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept
		{
			return activeAttributesNum == 0;
		}

		CPPX_ATTR_NO_DISCARD cppx::array_view<GenericVertexInputAttribute> GetActiveAttributesView() const noexcept
		{
			return cppx::bind_array_view( attributeArray, activeAttributesNum );
		}
		
		CPPX_ATTR_NO_DISCARD cppx::array_view<input_assembler_slot_t> GetActiveAttributeSlotsView() const noexcept
		{
			return cppx::bind_array_view( attributeSlotArray, activeAttributesNum );
		}
		
		CPPX_ATTR_NO_DISCARD cppx::array_view<VertexInputStream> GetActiveStreamsView() const noexcept
		{
			return cppx::bind_array_view( streamArray, activeStreamsNum );
		}
		
		CPPX_ATTR_NO_DISCARD cppx::array_view<input_assembler_slot_t> GetActiveStreamSlotsView() const noexcept
		{
			return cppx::bind_array_view( streamSlotArray, activeStreamsNum );
		}

		CPPX_ATTR_NO_DISCARD GenericVertexInputAttribute * GetAttributeAtSlot( native_uint pAttributeSlot ) const noexcept
		{
			Ic3DebugAssert( GCI::CXU::IAIsVertexAttributeSlotValid( pAttributeSlot ) );
			const auto attributeIndexInArray = attributeIndexMapping[pAttributeSlot];

			if( attributeIndexInArray == GCI::kIAVertexAttributeSlotUndefined )
			{
				return nullptr;
			}
			else
			{
				return &( attributeArray[attributeIndexInArray] );
			}
		}

		CPPX_ATTR_NO_DISCARD VertexInputStream * GetStreamAtSlot( native_uint pStreamSlot ) const noexcept
		{
			Ic3DebugAssert( GCI::CXU::IAIsDataStreamVertexBufferSlotValid( pStreamSlot ) );
			const auto streamIndexInArray = streamIndexMapping[pStreamSlot];

			if( streamIndexInArray == GCI::kIAVertexStreamSlotUndefined )
			{
				return nullptr;
			}
			else
			{
				return &( streamArray[streamIndexInArray] );
			}
		}
		
		CPPX_ATTR_NO_DISCARD bool IsAttributeActive( native_uint pAttributeSlot ) const noexcept
		{
			Ic3DebugAssert( GCI::CXU::IAIsVertexAttributeSlotValid( pAttributeSlot ) );
			const auto attributeIndexInArray = attributeIndexMapping[pAttributeSlot];
			return attributeIndexInArray != GCI::kIAVertexAttributeSlotUndefined;
		}

		CPPX_ATTR_NO_DISCARD bool IsStreamActive( native_uint pStreamSlot ) const noexcept
		{
			Ic3DebugAssert( GCI::CXU::IAIsDataStreamVertexBufferSlotValid( pStreamSlot ) );
			const auto streamIndexInArray = streamIndexMapping[pStreamSlot];
			return streamIndexInArray != GCI::kIAVertexStreamSlotUndefined;
		}

		CPPX_ATTR_NO_DISCARD GenericVertexInputAttribute * AppendActiveAttribute( native_uint pAttributeSlot )
		{
			Ic3DebugAssert( activeAttributesNum < attributeArraySize );
			Ic3DebugAssert( GCI::CXU::IAIsVertexAttributeSlotValid( pAttributeSlot ) );

			const auto reservedArrayIndex = activeAttributesNum;
			attributeIndexMapping[pAttributeSlot] = reservedArrayIndex;

			activeAttributesNum += 1;
			activeAttributesMask.set( GCI::CXU::IAMakeVertexAttributeFlag( pAttributeSlot ) );
			activeStreamsRange.add( InputAssemblerSlotRange{ pAttributeSlot, pAttributeSlot } );

			return &( attributeArray[reservedArrayIndex] );
		}

		CPPX_ATTR_NO_DISCARD VertexInputStream * AppendActiveStream( native_uint pStreamSlot )
		{
			Ic3DebugAssert( activeStreamsNum < streamArraySize );
			Ic3DebugAssert( GCI::CXU::IAIsDataStreamVertexBufferSlotValid( pStreamSlot ) );

			const auto reservedArrayIndex = activeStreamsNum;
			streamIndexMapping[pStreamSlot] = reservedArrayIndex;

			activeStreamsNum += 1;
			activeStreamsMask.set( GCI::CXU::IAMakeVertexBufferBindingFlag( pStreamSlot ) );
			activeStreamsRange.add( InputAssemblerSlotRange{ pStreamSlot, pStreamSlot } );

			return &( streamArray[reservedArrayIndex] );
		}

		bool CopyFrom( const VertexFormatCombinedLayoutBase & pOther )
		{
			if( ( attributeArraySize != pOther.attributeArraySize ) || ( streamArraySize != pOther.streamArraySize ) )
			{
				return false;
			}

			cppx::mem_copy( attributeIndexMapping, pOther.attributeIndexMapping );
			cppx::mem_copy( streamIndexMapping, pOther.streamIndexMapping );

			activeAttributesRange = pOther.activeAttributesRange;
			activeStreamsRange = pOther.activeStreamsRange;

			activeAttributesNum = pOther.activeAttributesNum;
			activeBaseAttributesNum = pOther.activeBaseAttributesNum;
			activeStreamsNum = pOther.activeStreamsNum;

			activeAttributesMask = pOther.activeAttributesMask;
			activeStreamsMask = pOther.activeStreamsMask;

			for( uint32 iAttribute = 0; iAttribute < attributeArraySize; ++iAttribute )
			{
				attributeArray[iAttribute] = pOther.attributeArray[iAttribute];
				attributeSlotArray[iAttribute] = pOther.attributeSlotArray[iAttribute];
			}

			for( uint32 iStream = 0; iStream < streamArraySize; ++iStream )
			{
				streamArray[iStream] = pOther.streamArray[iStream];
				streamSlotArray[iStream] = pOther.streamSlotArray[iStream];
			}
		}

		void Reset()
		{
			attributeIndexMapping.fill( GCI::kIAVertexAttributeSlotUndefined );
			streamIndexMapping.fill( GCI::kIAVertexAttributeSlotUndefined );

			activeAttributesRange = InputAssemblerSlotRange::empty_range();
			activeStreamsRange = InputAssemblerSlotRange::empty_range();

			activeAttributesNum = 0;
			activeBaseAttributesNum = 0;
			activeStreamsNum = 0;

			activeAttributesMask.clear();
			activeStreamsMask.clear();
		}
	};

	template <native_uint tpAttributeArraySize, native_uint tpStreamArraySize>
	struct VertexFormatCombinedLayout : public VertexFormatCombinedLayoutBase
	{
		using ActiveAttributeArray = std::array<GenericVertexInputAttribute, tpAttributeArraySize>;
		using ActiveAttributeSlotArray = std::array<input_assembler_slot_t, tpAttributeArraySize>;
		using ActiveStreamArray = std::array<VertexInputStream, tpStreamArraySize>;
		using ActiveStreamSlotArray = std::array<input_assembler_slot_t, tpStreamArraySize>;

		static constexpr auto sAttributeArraySize = tpAttributeArraySize;
		static constexpr auto sStreamArraySize = tpStreamArraySize;

		ActiveAttributeArray attributeArrayStatic;
		ActiveAttributeSlotArray attributeSlotArrayStatic;
		ActiveStreamArray streamArrayStatic;
		ActiveStreamSlotArray streamSlotArrayStatic;

		VertexFormatCombinedLayout()
		: VertexFormatCombinedLayoutBase(
			sAttributeArraySize,
			sStreamArraySize,
			&( attributeArrayStatic[0] ),
			&( attributeSlotArrayStatic[0] ),
			&( streamArrayStatic[0] ),
			&( streamSlotArrayStatic[0] ) )
		{}
	};

	struct VertexFormatCombinedLayoutDynamic : VertexFormatCombinedLayoutBase
	{
		std::unique_ptr<GenericVertexInputAttribute[]> attributeArrayDynamic;
		std::unique_ptr<input_assembler_slot_t[]> attributeSlotArrayDynamic;
		std::unique_ptr<VertexInputStream[]> streamArrayDynamic;
		std::unique_ptr<input_assembler_slot_t[]> streamSlotArrayDynamic;

		VertexFormatCombinedLayoutDynamic( native_uint pAttributeArraySize, native_uint pStreamArraySize )
		: VertexFormatCombinedLayoutBase(
			pAttributeArraySize,
			pStreamArraySize,
			MakeAttributeArrayPtr( pAttributeArraySize ),
			MakeAttributeSlotArrayPtr( pAttributeArraySize ),
			MakeStreamArrayPtr( pStreamArraySize ),
			MakeStreamSlotArrayPtr( pStreamArraySize ) )
		{
			attributeArrayDynamic.reset( attributeArray );
			attributeSlotArrayDynamic.reset( attributeSlotArray );
			streamArrayDynamic.reset( streamArray );
			streamSlotArrayDynamic.reset( streamSlotArray );
		}

		static GenericVertexInputAttribute * MakeAttributeArrayPtr( native_uint pAttributeArraySize ) noexcept
		{
			return new ( std::nothrow ) GenericVertexInputAttribute[pAttributeArraySize];
		}

		static input_assembler_slot_t * MakeAttributeSlotArrayPtr( native_uint pAttributeArraySize ) noexcept
		{
			return new ( std::nothrow ) input_assembler_slot_t[pAttributeArraySize];
		}

		static VertexInputStream * MakeStreamArrayPtr( native_uint pStreamArraySize ) noexcept
		{
			return new ( std::nothrow ) VertexInputStream[pStreamArraySize];
		}

		static input_assembler_slot_t * MakeStreamSlotArrayPtr( native_uint pStreamArraySize ) noexcept
		{
			return new ( std::nothrow ) input_assembler_slot_t[pStreamArraySize];
		}
	};


	namespace VertexFormat
	{

		struct VertexFormatCombinedLayoutSize
		{
			uint16 activeAttributesNum = 0;
			uint16 activeStreamsNum = 0;

			explicit operator bool() const noexcept
			{
				return ( activeAttributesNum > 0 ) && ( activeStreamsNum > 0 );
			}
		};

		IC3_NXMAIN_API_NO_DISCARD VertexFormatCombinedLayoutSize ComputeCombinedLayoutSize(
				const cppx::array_view<VertexInputAttributeDefinition> & pVertexAttributeDefinitions );

		/**
		 * @brief 
		 * @param pAttributeDefinitions 
		 * @param pOutVertexFormatCombinedLayout 
		 * @return 
		 */
		IC3_NXMAIN_API_NO_DISCARD bool CreateVertexFormatCombinedLayout(
				const cppx::array_view<VertexInputAttributeDefinition> & pAttributeDefinitions,
				VertexFormatCombinedLayoutBase & pOutVertexFormatCombinedLayout ) noexcept;

		/**
		 * @brief 
		 * @param pVertexFormatCombinedLayout 
		 * @param pAttributeDefinition 
		 * @return 
		 */
		IC3_NXMAIN_API_NO_DISCARD bool ValidateAttributeDefinition( const VertexInputAttributeDefinition & pAttributeDefinition ) noexcept;

		/**
		 * @brief Validates an array of VertexInputAttributeDefinitions and checks if they can form together a vertex format.
		 * @param pAttributeDefinitions List of attribute definitions to validate.
		 * @return True if all definitions are valid and can form together a verrtex format, or false otherwise.
		 */
		IC3_NXMAIN_API_NO_DISCARD bool ValidateVertexInputAttributeDefinitions(
				const cppx::array_view<VertexInputAttributeDefinition> & pAttributeDefinitions ) noexcept;

		/**
		 * @brief 
		 * @param pVertexFormatCombinedLayout 
		 * @param pAttributeDefinition 
		 * @return 
		 */
		IC3_NXMAIN_API_NO_DISCARD bool CheckAttributeDefinitionCompatibility(
				const VertexFormatCombinedLayoutBase & pVertexFormatCombinedLayout,
				const VertexInputAttributeDefinition & pAttributeDefinition ) noexcept;

		/**
		 * @brief Returns true if the specified attributes range can be added to VertexFormatCombinedLayout object.
		 * @param pVertexFormatCombinedLayout The VertexFormatCombinedLayout object to validate.
		 * @param pBaseAttributeSlot First index of thge attribute range
		 * @param pSemanticGroupSize 
		 * @return 
		 */
		IC3_NXMAIN_API_NO_DISCARD bool CheckAttributeArraySpace(
				const VertexFormatCombinedLayoutBase & pVertexFormatCombinedLayout,
				native_uint pFirstAttributeSlot,
				native_uint pAttributesNum ) noexcept;

		/**
		 * @brief 
		 * @param pVertexFormatCombinedLayout 
		 * @param pAttributeDefinition 
		 * @return 
		 */
		bool AdjustAttributeDefinition(
				const VertexFormatCombinedLayoutBase & pVertexFormatCombinedLayout,
				VertexInputAttributeDefinition & pAttributeDefinition ) noexcept;

		/**
		 * @brief 
		 * @param pVertexFormatCombinedLayout 
		 * @param pAttributeDefinition 
		 * @return 
		 */
		IC3_NXMAIN_API GenericVertexInputAttribute * AppendVertexFormatAttribute(
				VertexFormatCombinedLayoutBase & pVertexFormatCombinedLayout,
				const VertexInputAttributeDefinition & pAttributeDefinition ) noexcept;

		/**
		 *
		 * @param pAttributeSemanticID
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD cppx::string_view GetSemanticNameFromAttributeID(
				EVertexAttributeSemanticID pAttributeSemanticID );

		/**
		 *
		 * @param pAttributeSemanticFlags
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD cppx::string_view GetSemanticNameFromAttributeFlags(
				cppx::bitmask<EVertexAttributeSemanticFlags> pAttributeSemanticFlags );

		/**
		 *
		 * @param pAttributeSemanticName
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD cppx::bitmask<EVertexAttributeSemanticFlags> GetSemanticFlagsFromAttributeName(
				const cppx::string_view & pAttributeSemanticName );

		/**
		 *
		 * @param pAttributeSemanticName
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD EVertexAttributeSemanticID GetSemanticIDFromAttributeName(
				const cppx::string_view & pAttributeSemanticName );

	}

} // namespace Ic3

#endif // __IC3_NXMAIN_VERTEX_FORMAT_COMBINED_LAYOUT_H__
