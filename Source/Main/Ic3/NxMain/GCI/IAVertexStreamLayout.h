
#pragma once

#ifndef __IC3_NXMAIN_IA_VERTEX_STREAM_LAYOUT_H__
#define __IC3_NXMAIN_IA_VERTEX_STREAM_LAYOUT_H__

#include "IACommonDefs.h"
#include <Ic3/Cppx/StaticVector.h>
#include <Ic3/Cppx/SortedArray.h>
#include <Ic3/Cppx/STLHelperAlgo.h>

namespace Ic3
{

	/**
	 *
	 */
	struct VertexInputStream
	{
		/**
		 * Index of the slot within the Input Assembler Array (IAA) this stream is bound to.
		 */
		GCI::input_assembler_index_t streamIASlot = cxGCIVertexStreamIndexUndefined;

		/**
		 *
		 */
		TBitmask<GCI::EIAVertexAttributeFlags> activeAttributesMask {};

		/**
		 *
		 */
		uint16 activeAttributesNum = 0;

		/**
		 *
		 */
		uint16 dataStrideInBytes = 0;

		/**
		 *
		 */
		EVertexDataRate streamDataRate = EVertexDataRate::Undefined;

		IC3_ATTR_NO_DISCARD explicit operator bool() const noexcept;

		IC3_ATTR_NO_DISCARD bool isActive() const noexcept;

		IC3_ATTR_NO_DISCARD bool checkAttributeCompatibility( const VertexAttributeDefinition & pAttributeDefinition ) const noexcept;

		void init( uint16 pStreamIASlot, EVertexDataRate pDataRate );

		void reset();
	};

	/**
	 *
	 */
	class VertexStreamArrayConfig
	{
		friend class VertexFormatDescriptor;

	public:
		VertexStreamArrayConfig() = default;
		~VertexStreamArrayConfig() = default;

		IC3_ATTR_NO_DISCARD explicit operator bool() const noexcept;

		IC3_ATTR_NO_DISCARD const VertexInputStream & operator[]( native_uint pStreamIASlot ) const noexcept;

		IC3_ATTR_NO_DISCARD const VertexInputStream & streamAt( native_uint pStreamIASlot ) const;

		IC3_ATTR_NO_DISCARD const VertexInputStream * streamPtr( native_uint pStreamIASlot ) const noexcept;

		IC3_ATTR_NO_DISCARD bool isStreamActive( native_uint pStreamIASlot ) const noexcept;

		IC3_ATTR_NO_DISCARD const VertexInputStreamArray & getStreamArray() const noexcept;

		IC3_ATTR_NO_DISCARD uint32 getActiveStreamsNum() const noexcept;

		IC3_ATTR_NO_DISCARD TBitmask<GCI::EIAVertexStreamBindingFlags> getActiveAttributesMask() const noexcept;

		IC3_ATTR_NO_DISCARD const InputAssemblerSlotRange & getActiveAttributesRange() const noexcept;

		IC3_ATTR_NO_DISCARD const InputAssemblerSlotArray & getActiveStreamsSlots() const noexcept;

		IC3_ATTR_NO_DISCARD bool isActiveStreamsRangeContinuous() const noexcept;

		IC3_ATTR_NO_DISCARD bool empty() const noexcept;

		IC3_NXMAIN_API_NO_DISCARD bool checkAttributeDefinitionCompatibility(
				const VertexAttributeDefinition & pAttributeDefinition ) const noexcept;

		IC3_NXMAIN_API bool addActiveStream(
				gci_input_assembler_slot_t pStreamIASlot,
				EVertexDataRate pStreamDataRate );

		IC3_NXMAIN_API bool appendAttribute(
				gci_input_assembler_slot_t pStreamIASlot,
				const GenericVertexAttribute & pAttribute );

		IC3_NXMAIN_API bool appendAttributeAuto(
				gci_input_assembler_slot_t pStreamIASlot,
				EVertexDataRate pStreamDataRate,
				const GenericVertexAttribute & pAttribute );

		IC3_NXMAIN_API void reset();

	private:
		void _addStreamImpl( gci_input_assembler_slot_t pStreamIASlot, EVertexDataRate pStreamDataRate );

		void _appendAttributeImpl( VertexInputStream & pStream, const GenericVertexAttribute & pAttribute );

	private:
		VertexInputStreamArray _streamArray;

		TBitmask<GCI::EIAVertexStreamBindingFlags> _activeStreamsMask;

		uint32 _activeStreamsNum = 0;

		InputAssemblerSlotRange _activeStreamsRange = InputAssemblerSlotRange::emptyRange();

		InputAssemblerSlotArray _activeStreamsSlots;
	};

} // namespace Ic3

#include "IAVertexStreamLayout.inl"

#endif // __IC3_NXMAIN_IA_VERTEX_STREAM_LAYOUT_H__
