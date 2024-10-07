
#pragma once

#ifndef __IC3_NXMAIN_IA_VERTEX_STREAM_LAYOUT_H__
#define __IC3_NXMAIN_IA_VERTEX_STREAM_LAYOUT_H__

#include "IACommonDefs.h"
#include <Ic3/Cppx/StaticVector.h>
#include <Ic3/Cppx/SortedArray.h>
#include <Ic3/Cppx/STLHelperAlgo.h>

namespace Ic3
{

	/// @brief
	struct VertexStreamComponent
	{
		TBitmask<GCI::EIAVertexAttributeFlags> activeAttributesMask {};

		GCI::input_assembler_index_t streamIASlot = cxGCIVertexStreamSlotUndefined;

		uint16 activeAttributesNum = 0;

		uint16 elementStrideInBytes = 0;

		EVertexDataRate streamDataRate = EVertexDataRate::Undefined;

		explicit VertexStreamComponent( gci_input_assembler_slot_t pStreamIASlot = cxGCIVertexAttributeSlotUndefined );

		/// Returns true if this instance describes an active vertex data stream.
		IC3_ATTR_NO_DISCARD explicit operator bool() const noexcept;

		IC3_ATTR_NO_DISCARD bool operator==( const VertexStreamComponent & pRhs ) const noexcept;

		IC3_ATTR_NO_DISCARD bool operator<( const VertexStreamComponent & pRhs ) const noexcept;

		IC3_ATTR_NO_DISCARD bool active() const noexcept;

		IC3_ATTR_NO_DISCARD bool checkAttributeCompatibility( const VertexAttributeDefinition & pAttributeDefinition ) const noexcept;

		void init( uint16 pStreamIASlot, EVertexDataRate pDataRate );

		void reset();
	};

	/// @brief
	class VertexStreamArrayConfig
	{
		friend class VertexFormatDescriptor;

	public:
		VertexStreamArrayConfig() = default;
		~VertexStreamArrayConfig() = default;

		IC3_ATTR_NO_DISCARD explicit operator bool() const noexcept;

		IC3_ATTR_NO_DISCARD const VertexStreamComponent & operator[]( gci_input_assembler_slot_t pStreamIASlot ) const noexcept;

		IC3_ATTR_NO_DISCARD const VertexStreamComponent & streamAt( gci_input_assembler_slot_t pStreamIASlot ) const;

		IC3_ATTR_NO_DISCARD const VertexStreamComponent * streamPtr( gci_input_assembler_slot_t pStreamIASlot ) const noexcept;

		IC3_ATTR_NO_DISCARD bool isStreamActive( gci_input_assembler_slot_t pStreamIASlot ) const noexcept;

		IC3_ATTR_NO_DISCARD const VertexStreamArray & getActiveStreams() const noexcept;

		IC3_ATTR_NO_DISCARD TBitmask<GCI::EIAVertexStreamBindingFlags> getActiveAttributesMask() const noexcept;

		IC3_ATTR_NO_DISCARD uint32 getActiveStreamsNum() const noexcept;

		IC3_ATTR_NO_DISCARD const InputAssemblerSlotRange & getActiveAttributesRange() const noexcept;

		IC3_ATTR_NO_DISCARD const InputAssemblerSlotArray & getActiveStreamsSlots() const noexcept;

		IC3_ATTR_NO_DISCARD bool isActiveStreamsRangeContinuous() const noexcept;

		IC3_ATTR_NO_DISCARD bool empty() const noexcept;

		IC3_NXMAIN_API_NO_DISCARD size_t findStreamAtSlot( gci_input_assembler_slot_t pStreamIASlot ) const noexcept;

		IC3_NXMAIN_API_NO_DISCARD bool checkAttributeDefinitionCompatibility(
				const VertexAttributeDefinition & pAttributeDefinition ) const noexcept;

		IC3_NXMAIN_API bool addActiveStream(
				gci_input_assembler_slot_t pStreamIASlot,
				EVertexDataRate pStreamDataRate );

		IC3_NXMAIN_API bool appendAttribute(
				gci_input_assembler_slot_t pStreamIASlot,
				const VertexAttributeComponent & pAttribute );

		IC3_NXMAIN_API bool appendAttributeAuto(
				gci_input_assembler_slot_t pStreamIASlot,
				EVertexDataRate pStreamDataRate,
				const VertexAttributeComponent & pAttribute );

		IC3_NXMAIN_API void reserveAttributeArraySpace( size_t pActiveStreamsNum );

		IC3_NXMAIN_API void reset();

	private:
		size_t _addStreamImpl( gci_input_assembler_slot_t pStreamIASlot, EVertexDataRate pStreamDataRate );

		void _appendAttributeImpl( VertexStreamComponent & pStream, const VertexAttributeComponent & pAttribute );

	private:
		VertexStreamArray _activeStreams;

		TBitmask<GCI::EIAVertexStreamBindingFlags> _activeStreamsMask;

		uint32 _activeStreamsNum = 0;

		InputAssemblerSlotRange _activeStreamsRange = InputAssemblerSlotRange::emptyRange();

		InputAssemblerSlotArray _activeStreamsSlots;
	};

} // namespace Ic3

#include "IAVertexStreamLayout.inl"

#endif // __IC3_NXMAIN_IA_VERTEX_STREAM_LAYOUT_H__
