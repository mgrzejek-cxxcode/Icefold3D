
#pragma once

#ifndef __IC3_NXMAIN_IA_VERTEX_STREAM_LAYOUT_H__
#define __IC3_NXMAIN_IA_VERTEX_STREAM_LAYOUT_H__

#include "IACommonDefs.h"
#include <cppx/staticVector.h>
#include <cppx/sortedArray.h>
#include <cppx/stdHelperAlgo.h>

namespace Ic3
{

	/**
	 *
	 */
	#pragma pack( push, 1 )
	struct __attribute__(( packed )) VertexInputStream
	{
		/**
		 *
		 */
		cppx::bitmask<GCI::EIAVertexAttributeFlags> activeAttributesMask {};

		/**
		 * Index of the slot within the Input Assembler Array (IAA) this stream is bound to.
		 */
		GCI::input_assembler_index_t streamSlot = GCI::kIAVertexStreamSlotUndefined;

		/**
		 *
		 */
		uint8 activeAttributesNum = 0;

		/**
		 *
		 */
		uint8 dataStrideInBytes = 0;

		/**
		 *
		 */
		GCI::EIAVertexAttributeDataRate streamDataRate = GCI::EIAVertexAttributeDataRate::Undefined;

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsActive() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsInitialized() const noexcept;

		CPPX_ATTR_NO_DISCARD bool CheckAttributeCompatibility( const VertexInputAttributeDefinition & pAttributeDefinition ) const noexcept;

		void Init( uint16 pStreamSlot, GCI::EIAVertexAttributeDataRate pDataRate );

		void Reset();
	};
	#pragma pack( pop )

	/**
	 *
	 */
	class VertexInputStreamArrayConfig
	{
		friend class VertexFormatSignature;

	public:
		VertexInputStreamArrayConfig() = default;
		~VertexInputStreamArrayConfig() = default;

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept;

		CPPX_ATTR_NO_DISCARD const VertexInputStream & operator[]( native_uint pStreamSlot ) const noexcept;

		CPPX_ATTR_NO_DISCARD const VertexInputStream & StreamAt( native_uint pStreamSlot ) const;

		CPPX_ATTR_NO_DISCARD const VertexInputStream * StreamPtr( native_uint pStreamSlot ) const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsStreamActive( native_uint pStreamSlot ) const noexcept;

		CPPX_ATTR_NO_DISCARD const VertexInputStreamArray & GetStreamArray() const noexcept;

		CPPX_ATTR_NO_DISCARD uint32 GetActiveStreamsNum() const noexcept;

		CPPX_ATTR_NO_DISCARD cppx::bitmask<GCI::EVertexSourceBindingFlags> GetActiveStreamsMask() const noexcept;

		CPPX_ATTR_NO_DISCARD const InputAssemblerSlotRange & GetActiveStreamsRange() const noexcept;

		CPPX_ATTR_NO_DISCARD const InputAssemblerSlotArray & GetActiveStreamsSlots() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsActiveStreamsRangeContinuous() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept;

		IC3_NXMAIN_API_NO_DISCARD bool CheckAttributeDefinitionCompatibility(
				const VertexInputAttributeDefinition & pAttributeDefinition ) const noexcept;

		IC3_NXMAIN_API bool AddActiveStream(
				gci_input_assembler_slot_t pStreamSlot,
				GCI::EIAVertexAttributeDataRate pStreamDataRate );

		IC3_NXMAIN_API bool AppendAttribute(
				gci_input_assembler_slot_t pStreamSlot,
				const GenericVertexInputAttribute & pAttribute );

		IC3_NXMAIN_API bool AppendAttributeAuto(
				gci_input_assembler_slot_t pStreamSlot,
				GCI::EIAVertexAttributeDataRate pStreamDataRate,
				const GenericVertexInputAttribute & pAttribute );

		IC3_NXMAIN_API void Reset();

	private:
		void _AddStreamImpl( gci_input_assembler_slot_t pStreamSlot, GCI::EIAVertexAttributeDataRate pStreamDataRate );

		void _AppendAttributeImpl( VertexInputStream & pStream, const GenericVertexInputAttribute & pAttribute );

	private:
		VertexInputStreamArray _streamArray;

		cppx::bitmask<GCI::EVertexSourceBindingFlags> _activeStreamsMask;

		uint32 _activeStreamsNum = 0;

		InputAssemblerSlotRange _activeStreamsRange = InputAssemblerSlotRange::empty_range();

		InputAssemblerSlotArray _activeStreamsSlots;
	};

} // namespace Ic3

#include "IAVertexStreamLayout.inl"

#endif // __IC3_NXMAIN_IA_VERTEX_STREAM_LAYOUT_H__
