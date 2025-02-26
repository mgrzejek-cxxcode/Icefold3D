
#if !defined( __IC3_NXMAIN_IA_COMMON_DEFS_H__ )
#  error ".inl files must be included only via their related headers!"
#endif

namespace Ic3
{

	// @VertexAttributeDefinition

	inline bool VertexInputAttributeDefinition::IsValid() const noexcept
	{
		return ( attributeSlot != kGCIVertexInputAttributeSlotUndefined ) &&
		       ( streamSlot != kGCIVertexInputStreamSlotUndefined ) &&
		       ( dataFormat != GCI::EVertexAttribFormat::Undefined ) &&
		       GCI::CXU::IAIsVertexAttributeSemanticGroupSizeValid( semanticGroupSize ) &&
		       shaderSemantics.IsValid();
	}

	inline bool VertexInputAttributeDefinition::HasAppendAsRelativeOffset() const noexcept
	{
		return vertexStreamRelativeOffset == kGCIVertexAttributeOffsetAppend;
	}

	// @GenericVertexAttribute

	inline GenericVertexInputAttribute::operator bool() const noexcept
	{
		return IsActive();
	}

	inline bool GenericVertexInputAttribute::IsActive() const noexcept
	{
		return dataFormat != GCI::EVertexAttribFormat::Undefined;
	}

	inline bool GenericVertexInputAttribute::IsBaseAttribute() const noexcept
	{
		return semanticIndex == 0;
	}

	inline bool GenericVertexInputAttribute::IsSemanticGroupAttribute() const noexcept
	{
		return GCI::CXU::IAIsVertexAttributeSemanticGroupSizeValid( semanticGroupSize ) && ( semanticGroupSize > 1 );
	}

	inline bool GenericVertexInputAttribute::IsSameAs( const GenericVertexInputAttribute & pOther ) const noexcept
	{
		return ( dataFormat == pOther.dataFormat ) &&
		       ( dataPadding == pOther.dataPadding ) &&
		       ( semanticGroupSize == pOther.semanticGroupSize ) &&
		       ( semanticIndex == pOther.semanticIndex ) &&
		       ( attributeSlot == pOther.attributeSlot ) &&
		       ( streamSlot == pOther.streamSlot ) &&
		       ( vertexStreamRelativeOffset == pOther.vertexStreamRelativeOffset ) &&
		       ( shaderSemantics == pOther.shaderSemantics );
	}

	inline bool GenericVertexInputAttribute::HasSameFormatAs( const GenericVertexInputAttribute & pOther ) const noexcept
	{
		return dataFormat == pOther.dataFormat;
	}

	inline bool GenericVertexInputAttribute::HasSameSemanticsAs( const GenericVertexInputAttribute & pOther ) const noexcept
	{
		return shaderSemantics == pOther.shaderSemantics;
	}

	inline GCI::EBaseDataType GenericVertexInputAttribute::GetBaseDataType() const noexcept
	{
		return GCI::CXU::GetVertexAttribFormatBaseDataType( dataFormat );
	}

	inline uint32 GenericVertexInputAttribute::GetDataSizeInBytes() const noexcept
	{
		return GCI::CXU::GetVertexAttribFormatByteSize( dataFormat );
	}

	inline uint32 GenericVertexInputAttribute::GetDataStride() const noexcept
	{
		return GetDataSizeInBytes() + dataPadding;
	}

	inline void GenericVertexInputAttribute::InitBaseAttributeFromDefinition(
			const VertexInputAttributeDefinition & pDefinition )
	{
		dataFormat = pDefinition.dataFormat;
		attributeSlot = pDefinition.attributeSlot;
		streamSlot = pDefinition.streamSlot;
		dataPadding = pDefinition.dataPadding;
		semanticGroupSize = pDefinition.semanticGroupSize;
		vertexStreamRelativeOffset = pDefinition.vertexStreamRelativeOffset;
		semanticIndex = 0;
		shaderSemantics = pDefinition.shaderSemantics;
	}

	inline void GenericVertexInputAttribute::InitSemanticSubAttributeFromBaseAttribute(
			const GenericVertexInputAttribute & pBaseAttribute,
			uint32 pSemanticIndex )
	{
		dataFormat = pBaseAttribute.dataFormat;
		attributeSlot = pBaseAttribute.attributeSlot + pSemanticIndex;
		streamSlot = pBaseAttribute.streamSlot;
		dataPadding = pBaseAttribute.dataPadding;
		semanticGroupSize = pBaseAttribute.semanticGroupSize;
		semanticIndex = pSemanticIndex;
		shaderSemantics = pBaseAttribute.shaderSemantics;

		// To compute the offset for the next attributes in the same semantic group, we simply
		// add the total stride to the offset of the base (first) attribute in that group.
		vertexStreamRelativeOffset = pBaseAttribute.vertexStreamRelativeOffset + ( pBaseAttribute.GetDataStride() * pSemanticIndex );
	}

	inline void GenericVertexInputAttribute::Reset()
	{
		dataFormat = GCI::EVertexAttribFormat::Undefined;
		attributeSlot = kGCIVertexInputAttributeSlotUndefined;
		streamSlot = kGCIVertexInputStreamSlotUndefined;
		dataPadding = 0;
		semanticGroupSize = 0;
		semanticIndex = 0;
		shaderSemantics = {};
		vertexStreamRelativeOffset = 0;
	}


	namespace GCU
	{

		inline bool IsAttributeLocationAndSizeValid( uint32 pAttributeBaseSlot, uint32 pSemanticGroupSize = 1 )
		{
			return ( pAttributeBaseSlot != kGCIVertexInputAttributeSlotUndefined ) &&
			       // Vertex attribute index should be in the valid range of supported values.
			       GCI::CXU::IAIsVertexAttributeIndexValid( pAttributeBaseSlot ) &&
			       // Each attribute has to have at least one component and no more than the GCI-level limit.
			       GCI::CXU::IAIsVertexAttributeSemanticGroupSizeValid( pSemanticGroupSize ) &&
			       // Attributes can have multiple components (e.g. a 4x4 matrix is a 4-component attribute, with each component
			       // being a 4-element vector). Even though the base index is valid, we need to check all potential sub-attributes.
			       GCI::CXU::IAIsVertexAttributeIndexValid( pAttributeBaseSlot + pSemanticGroupSize - 1 );
		}

	} // namespace GCU

}
