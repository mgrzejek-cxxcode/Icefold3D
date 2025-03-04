
#if !defined( __IC3_NXMAIN_IA_COMMON_DEFS_H__ )
#  error ".inl files must be included only via their related headers!"
#endif

namespace Ic3
{

	// @VertexAttributeDefinition

	inline uint32 VertexInputAttributeDefinition::GetDataSizeInBytes() const noexcept
	{
		return GCI::CXU::GetVertexAttribFormatByteSize( baseDataFormat );
	}

	inline bool VertexInputAttributeDefinition::IsValid() const noexcept
	{
		return GCIUtils::IsAttributeLocationAndSizeValid( attributeSlot, semanticGroupSize ) &&
		       GCIUtils::IAIsDataStreamVertexBufferSlotValid( vertexStreamSlot ) &&
		       ( baseDataFormat != GCI::EVertexAttribFormat::Undefined ) &&
		       !semanticName.empty();
	}

	inline bool VertexInputAttributeDefinition::HasAppendAsRelativeOffset() const noexcept
	{
		return ( vertexStreamRelativeOffset == GCI::kIAVertexAttributeOffsetAppend );
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
		return ( attributeSlot == pOther.attributeSlot ) &&
		       ( dataFormat == pOther.dataFormat ) &&
		       ( semanticIndex == pOther.semanticGroupSize ) &&
		       ( semanticGroupSize == pOther.semanticGroupSize ) &&
		       ( semanticName == pOther.semanticName ) &&
		       ( semanticFlags == pOther.semanticFlags ) &&
		       ( dataPadding == pOther.dataPadding ) &&
		       ( vertexStreamSlot == pOther.vertexStreamSlot ) &&
		       ( vertexStreamRelativeOffset == pOther.vertexStreamRelativeOffset );
	}

	inline bool GenericVertexInputAttribute::HasSameFormatAs( const GenericVertexInputAttribute & pOther ) const noexcept
	{
		return dataFormat == pOther.dataFormat;
	}

	inline bool GenericVertexInputAttribute::HasSameSemanticsAs( const GenericVertexInputAttribute & pOther ) const noexcept
	{
		return !semanticName.empty() && ( semanticName == pOther.semanticName ) && ( semanticIndex == pOther.semanticIndex );
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
		attributeSlot = cppx::numeric_cast<decltype( attributeSlot )>( pDefinition.attributeSlot );
		dataFormat = pDefinition.baseDataFormat;
		dataRate = pDefinition.dataRate;
		semanticIndex = 0;
		semanticGroupSize = cppx::numeric_cast<decltype( semanticGroupSize )>( pDefinition.semanticGroupSize );
		semanticName = pDefinition.semanticName;
		semanticFlags = pDefinition.semanticFlags;
		dataPadding = cppx::numeric_cast<decltype( dataPadding )>( pDefinition.dataPadding );
		vertexStreamSlot = cppx::numeric_cast<decltype( vertexStreamSlot )>( pDefinition.vertexStreamSlot );
		vertexStreamRelativeOffset = cppx::numeric_cast<decltype( vertexStreamRelativeOffset )>( pDefinition.vertexStreamRelativeOffset );
	}

	inline void GenericVertexInputAttribute::InitSemanticSubAttributeFromBaseAttribute(
			const GenericVertexInputAttribute & pBaseAttribute,
			uint32 pSemanticIndex )
	{
		attributeSlot = cppx::numeric_cast<decltype( attributeSlot )>( pBaseAttribute.attributeSlot + pSemanticIndex );
		dataFormat = pBaseAttribute.dataFormat;
		dataRate = pBaseAttribute.dataRate;
		semanticIndex = cppx::numeric_cast<decltype( semanticIndex )>( pSemanticIndex );
		semanticGroupSize = pBaseAttribute.semanticGroupSize;
		semanticName = pBaseAttribute.semanticName;
		semanticFlags = pBaseAttribute.semanticFlags;
		dataPadding = pBaseAttribute.dataPadding;
		vertexStreamSlot = pBaseAttribute.vertexStreamSlot;

		// To compute the offset for the next attributes in the same semantic group, we simply
		// add the total stride to the offset of the base (first) attribute in that group.
		const auto subAttributeRelativeOffset =
				pBaseAttribute.vertexStreamRelativeOffset + ( pBaseAttribute.GetDataStride() * pSemanticIndex );

		vertexStreamRelativeOffset = cppx::numeric_cast<decltype( vertexStreamRelativeOffset )>( subAttributeRelativeOffset );
	}

	inline void GenericVertexInputAttribute::Reset()
	{
		attributeSlot = GCI::kIAVertexAttributeSlotUndefined;
		dataFormat = GCI::EVertexAttribFormat::Undefined;
		semanticIndex = 0;
		semanticGroupSize = 0;
		semanticName.clear();
		semanticFlags = 0;
		dataPadding = 0;
		vertexStreamSlot = GCI::kIAVertexStreamSlotUndefined;
		vertexStreamRelativeOffset = 0;
	}


	namespace GCIUtils
	{

		inline bool IsAttributeLocationAndSizeValid( uint32 pAttributeBaseSlot, uint32 pSemanticGroupSize = 1 )
		{
			return ( pAttributeBaseSlot != GCI::kIAVertexAttributeSlotUndefined ) &&
			       // Vertex attribute index should be in the valid range of supported values.
			       GCI::CXU::IAIsVertexAttributeSlotValid( pAttributeBaseSlot ) &&
			       // Each attribute has to have at least one component and no more than the GCI-level limit.
			       GCI::CXU::IAIsVertexAttributeSemanticGroupSizeValid( pSemanticGroupSize ) &&
			       // Attributes can have multiple components (e.g. a 4x4 matrix is a 4-component attribute, with each component
			       // being a 4-element vector). Even though the base index is valid, we need to check all potential sub-attributes.
			       GCI::CXU::IAIsVertexAttributeSlotValid( pAttributeBaseSlot + pSemanticGroupSize - 1 );
		}

	} // namespace GCU

}
