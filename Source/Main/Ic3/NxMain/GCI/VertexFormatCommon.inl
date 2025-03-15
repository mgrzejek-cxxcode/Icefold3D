
#if !defined( __IC3_NXMAIN_VERTEX_FORMAT_COMMON_H__ )
#  error ".inl files must be included only via their related headers!"
#endif

namespace Ic3
{

	inline uint32 VertexInputAttributeDefinition::GetDataSizeInBytes() const noexcept
	{
		return GCI::CXU::GetVertexAttribFormatByteSize( baseDataFormat );
	}

	inline uint32 VertexInputAttributeDefinition::GetDataStride() const noexcept
	{
		return GetDataSizeInBytes() + dataPadding;
	}

	inline bool VertexInputAttributeDefinition::IsValid() const noexcept
	{
		return GCI::Utilities::IAIsAttributeSemanticGroupValid( attributeSlot, semanticGroupSize ) &&
		       GCI::CXU::IAIsDataStreamVertexBufferSlotValid( vertexStreamSlot ) &&
		       ( baseDataFormat != GCI::EVertexAttribFormat::Undefined ) &&
		       !semantics.semanticName.empty();
	}

	inline bool VertexInputAttributeDefinition::HasAppendAsRelativeOffset() const noexcept
	{
		return ( vertexStreamRelativeOffset == kVertexAttributeOffsetAppend ) ||
		       ( vertexStreamRelativeOffset == kVertexAttributeOffsetAppendAligned16 );
	}



	inline GenericVertexInputAttribute::operator bool() const noexcept
	{
		return IsActive();
	}

	inline bool GenericVertexInputAttribute::IsActive() const noexcept
	{
		return GCI::CXU::IAIsVertexAttributeSlotValid( attributeSlot ) && ( dataFormat != GCI::EVertexAttribFormat::Undefined );
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
		semanticName = pDefinition.semantics.semanticName;
		semanticFlags = pDefinition.semantics.semanticFlags;
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



	inline VertexInputStream::operator bool() const noexcept
	{
		return IsActive();
	}
	
	inline bool VertexInputStream::IsActive() const noexcept
	{
		return IsInitialized() && !activeAttributesMask.empty();
	}

	inline bool VertexInputStream::IsInitialized() const noexcept
	{
		return GCI::CXU::IAIsDataStreamVertexBufferSlotValid( streamSlot ) && ( streamDataRate != GCI::EIAVertexAttributeDataRate::Undefined );
	}

	inline bool VertexInputStream::IsSameAs( const VertexInputStream & pOther ) const noexcept
	{
		return ( activeAttributesMask == pOther.activeAttributesMask ) &&
		       ( streamSlot == pOther.streamSlot ) &&
		       ( streamDataRate == pOther.streamDataRate ) &&
		       ( dataStrideInBytes == pOther.dataStrideInBytes );
	}

	inline bool VertexInputStream::CheckAttributeCompatibility( const VertexInputAttributeDefinition & pAttributeDefinition ) const noexcept
	{
		return ( streamDataRate == GCI::EIAVertexAttributeDataRate::Undefined ) || ( pAttributeDefinition.dataRate == streamDataRate );
	}

	inline void VertexInputStream::Init( uint16 pStreamSlot, GCI::EIAVertexAttributeDataRate pDataRate )
	{
		streamSlot = pStreamSlot;
		streamDataRate = pDataRate;
	}

	inline void VertexInputStream::Reset()
	{
		streamSlot = GCI::kIAVertexAttributeSlotUndefined;
		streamDataRate = GCI::EIAVertexAttributeDataRate::Undefined;
	}

}
