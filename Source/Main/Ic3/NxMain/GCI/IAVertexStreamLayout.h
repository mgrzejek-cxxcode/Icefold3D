
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
	struct VertexInputStream
	{
		/**
		 * Index of the slot within the Input Assembler Array (IAA) this stream is bound to.
		 */
		GCI::input_assembler_index_t streamIASlot = cxGCIVertexStreamIndexUndefined;

		/**
		 *
		 */
		cppx::bitmask<GCI::EIAVertexAttributeFlags> activeAttributesMask {};

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

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsActive() const noexcept;

		CPPX_ATTR_NO_DISCARD bool CheckAttributeCompatibility( const VertexAttributeDefinition & pAttributeDefinition ) const noexcept;

		void Init( uint16 pStreamIASlot, EVertexDataRate pDataRate );

		void Reset();
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

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept;

		CPPX_ATTR_NO_DISCARD const VertexInputStream & operator[]( native_uint pStreamIASlot ) const noexcept;

		CPPX_ATTR_NO_DISCARD const VertexInputStream & StreamAt( native_uint pStreamIASlot ) const;

		CPPX_ATTR_NO_DISCARD const VertexInputStream * StreamPtr( native_uint pStreamIASlot ) const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsStreamActive( native_uint pStreamIASlot ) const noexcept;

		CPPX_ATTR_NO_DISCARD const VertexInputStreamArray & GetStreamArray() const noexcept;

		CPPX_ATTR_NO_DISCARD uint32 GetActiveStreamsNum() const noexcept;

		CPPX_ATTR_NO_DISCARD cppx::bitmask<GCI::EIAVertexStreamBindingFlags> GetActiveAttributesMask() const noexcept;

		CPPX_ATTR_NO_DISCARD const InputAssemblerSlotRange & GetActiveAttributesRange() const noexcept;

		CPPX_ATTR_NO_DISCARD const InputAssemblerSlotArray & GetActiveStreamsSlots() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsActiveStreamsRangeContinuous() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept;

		IC3_NXMAIN_API_NO_DISCARD bool CheckAttributeDefinitionCompatibility(
				const VertexAttributeDefinition & pAttributeDefinition ) const noexcept;

		IC3_NXMAIN_API bool AddActiveStream(
				gci_input_assembler_slot_t pStreamIASlot,
				EVertexDataRate pStreamDataRate );

		IC3_NXMAIN_API bool AppendAttribute(
				gci_input_assembler_slot_t pStreamIASlot,
				const GenericVertexAttribute & pAttribute );

		IC3_NXMAIN_API bool AppendAttributeAuto(
				gci_input_assembler_slot_t pStreamIASlot,
				EVertexDataRate pStreamDataRate,
				const GenericVertexAttribute & pAttribute );

		IC3_NXMAIN_API void Reset();

	private:
		void _AddStreamImpl( gci_input_assembler_slot_t pStreamIASlot, EVertexDataRate pStreamDataRate );

		void _AppendAttributeImpl( VertexInputStream & pStream, const GenericVertexAttribute & pAttribute );

	private:
		VertexInputStreamArray _streamArray;

		cppx::bitmask<GCI::EIAVertexStreamBindingFlags> _activeStreamsMask;

		uint32 _activeStreamsNum = 0;

		InputAssemblerSlotRange _activeStreamsRange = InputAssemblerSlotRange::empty_range();

		InputAssemblerSlotArray _activeStreamsSlots;
	};

} // namespace Ic3

#include "IAVertexStreamLayout.inl"

#endif // __IC3_NXMAIN_IA_VERTEX_STREAM_LAYOUT_H__
