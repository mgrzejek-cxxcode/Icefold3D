
#if !defined( __IC3_NXMAIN_IA_COMMON_DEFS_H__ )
#  error ".inl files must be included only via their related headers!"
#endif

namespace Ic3
{

	// @CommonVertexAttributeData

	inline uint32 CommonVertexAttributeData::sizeInBytes() const noexcept
	{
		return GCI::CxDef::getVertexAttribFormatByteSize( baseFormat );
	}

	inline uint32 CommonVertexAttributeData::dataStride() const noexcept
	{
		return sizeInBytes() + componentPadding;
	}


	// @VertexAttributeDefinition

	inline bool VertexAttributeDefinition::hasAppendAsRelativeOffset() const noexcept
	{
		return ( vertexStreamRelativeOffset == GCI::cxIAVertexAttributeOffsetAppend ) ||
		       ( vertexStreamRelativeOffset == GCI::cxIAVertexAttributeOffsetAppend16 );
	}

	inline bool VertexAttributeDefinition::operator==( const VertexAttributeDefinition & pRhs ) const noexcept
	{
		return attributeIASlot == pRhs.attributeIASlot;
	}

	inline bool VertexAttributeDefinition::operator<( const VertexAttributeDefinition & pRhs ) const noexcept
	{
		return attributeIASlot < pRhs.attributeIASlot;
	}

	inline bool VertexAttributeDefinition::valid() const noexcept
	{
		return ( baseFormat != GCI::EVertexAttribFormat::Undefined ) &&
		       ( vertexStreamIASlot != GCI::cxIAVertexStreamIndexUndefined ) &&
		       // Vertex stream index should be in the valid range of supported values.
		       cxGCIValidInputAssemblerSlotRange.contains( vertexStreamIASlot ) &&
			   //
			   GCU::isAttributeLocationAndSizeValid( attributeIASlot, semanticComponentsNum );

	}


	namespace GCU
	{

		inline bool isAttributeLocationAndSizeValid( uint32 pBaseAttributeIASlot, uint32 pComponentsNum )
		{
			return ( pBaseAttributeIASlot != cxGCIVertexAttributeSlotUndefined ) &&
			       // Vertex attribute index should be in the valid range of supported values.
			       cxGCIValidVertexAttributeSlotRange.contains( pBaseAttributeIASlot ) &&
			       // Each attribute has to have at least one component and no more than the GCI-level limit.
			       cxGCIValidVertexAttributeComponentsNumberRange.contains( pComponentsNum ) &&
			       // Attributes can have multiple components (e.g. a 4x4 matrix is a 4-component attribute, with each component
			       // being a 4-element vector). Even though the base index is valid, we need to check all potential sub-attributes.
			       cxGCIValidVertexAttributeSlotRange.contains( pBaseAttributeIASlot + pComponentsNum - 1 );
		}

	}

}
