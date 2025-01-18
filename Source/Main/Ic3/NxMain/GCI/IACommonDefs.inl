
#if !defined( __IC3_NXMAIN_IA_COMMON_DEFS_H__ )
#  error ".inl files must be included only via their related headers!"
#endif

namespace Ic3
{

	// @VertexAttributeDefinition

	inline bool VertexAttributeDefinition::isValid() const noexcept
	{
		return ( attributeIASlot != cxGCIVertexAttributeIndexUndefined ) &&
		       ( streamIASlot != cxGCIVertexStreamIndexUndefined ) &&
		       ( dataFormat != GCI::EVertexAttribFormat::Undefined ) &&
		       cxGCIValidVertexAttributeSemanticGroupSizeRange.contains( semanticGroupSize ) &&
		       shaderSemantics.valid();
	}

	inline bool VertexAttributeDefinition::hasAppendAsRelativeOffset() const noexcept
	{
		return vertexStreamRelativeOffset == cxGCIVertexAttributeOffsetAppend;
	}

	// @GenericVertexAttribute

	inline GenericVertexAttribute::operator bool() const noexcept
	{
		return isActive();
	}

	inline bool GenericVertexAttribute::isActive() const noexcept
	{
		return dataFormat != GCI::EVertexAttribFormat::Undefined;
	}

	inline bool GenericVertexAttribute::isBaseAttribute() const noexcept
	{
		return semanticIndex == 0;
	}

	inline bool GenericVertexAttribute::isSemanticGroupAttribute() const noexcept
	{
		return cxGCIValidVertexAttributeSemanticGroupSizeRange.contains( semanticGroupSize ) && ( semanticGroupSize > 1 );
	}

	inline bool GenericVertexAttribute::isSameAs( const GenericVertexAttribute & pOther ) const noexcept
	{
		/// TODO: Implement
		return false;
	}

	inline bool GenericVertexAttribute::hasSameFormatAs( const GenericVertexAttribute & pOther ) const noexcept
	{
		return dataFormat == pOther.dataFormat;
	}

	inline bool GenericVertexAttribute::hasSameSemanticsAs( const GenericVertexAttribute & pOther ) const noexcept
	{
		return shaderSemantics == pOther.shaderSemantics;
	}

	inline GCI::EBaseDataType GenericVertexAttribute::getBaseDataType() const noexcept
	{
		return GCI::CxDef::getVertexAttribFormatBaseDataType( dataFormat );
	}

	inline uint32 GenericVertexAttribute::getDataSizeInBytes() const noexcept
	{
		return GCI::CxDef::getVertexAttribFormatByteSize( dataFormat );
	}

	inline uint32 GenericVertexAttribute::getDataStride() const noexcept
	{
		return getDataSizeInBytes() + dataPadding;
	}

	inline void GenericVertexAttribute::initBaseAttributeFromDefinition( const VertexAttributeDefinition & pDefinition )
	{
		attributeIASlot = pDefinition.attributeIASlot;
		streamIASlot = pDefinition.streamIASlot;
		dataFormat = pDefinition.dataFormat;
		dataPadding = pDefinition.dataPadding;
		semanticGroupSize = pDefinition.semanticGroupSize;
		semanticIndex = 0;
		shaderSemantics = pDefinition.shaderSemantics;
	}

	inline void GenericVertexAttribute::initSemanticSubAttributeFromBaseAttribute(
			const GenericVertexAttribute & pBaseAttribute,
			uint32 pSemanticIndex )
	{
		attributeIASlot = pBaseAttribute.attributeIASlot + pSemanticIndex;
		streamIASlot = pBaseAttribute.streamIASlot;
		dataFormat = pBaseAttribute.dataFormat;
		dataPadding = pBaseAttribute.dataPadding;
		semanticGroupSize = pBaseAttribute.semanticGroupSize;
		semanticIndex = pSemanticIndex;
		shaderSemantics = pBaseAttribute.shaderSemantics;

		// To compute the offset for the next attributes in the same semantic group, we simply
		// add the total stride to the offset of the base (first) attribute in that group.
		vertexStreamRelativeOffset = pBaseAttribute.vertexStreamRelativeOffset + ( pBaseAttribute.getDataStride() * pSemanticIndex );
	}

	inline void GenericVertexAttribute::reset()
	{
		attributeIASlot = cxGCIVertexAttributeIndexUndefined;
		streamIASlot = cxGCIVertexStreamIndexUndefined;
		dataFormat = GCI::EVertexAttribFormat::Undefined;
		dataPadding = 0;
		semanticGroupSize = 0;
		semanticIndex = 0;
		shaderSemantics = {};
		vertexStreamRelativeOffset = 0;
	}


	namespace GCU
	{

		inline bool isAttributeLocationAndSizeValid( uint32 pAttributeBaseSlotIndex, uint32 pSemanticGroupSize = 1 )
		{
			return ( pAttributeBaseSlotIndex != cxGCIVertexAttributeIndexUndefined ) &&
			       // Vertex attribute index should be in the valid range of supported values.
			       cxGCIValidInputAssemblerSlotIndexRange.contains( pAttributeBaseSlotIndex ) &&
			       // Each attribute has to have at least one component and no more than the GCI-level limit.
			       cxGCIValidVertexAttributeSemanticGroupSizeRange.contains( pSemanticGroupSize ) &&
			       // Attributes can have multiple components (e.g. a 4x4 matrix is a 4-component attribute, with each component
			       // being a 4-element vector). Even though the base index is valid, we need to check all potential sub-attributes.
			       cxGCIValidInputAssemblerSlotIndexRange.contains( pAttributeBaseSlotIndex + pSemanticGroupSize - 1 );
		}

		/*
		inline PackedAttributeInfo checkPackedAttributeCompatibility( GCI::EVertexAttribFormat pFormat1, GCI::EVertexAttribFormat pFormat2 )
		{
			PackedAttributeInfo packedInfo{};

			if( GCI::CxDef::isVertexAttribFormatValid( pFormat1 ) )
			{
				const auto baseComponentsNum = GCI::CxDef::getVertexAttribFormatComponentsNum( pFormat1 );
				const auto baseDataType = GCI::CxDef::getVertexAttribFormatBaseDataType( pFormat1 );
				const auto dataTypeFlags = GCI::CxDef::getVertexAttribFormatFlags( pFormat1 );

				auto packedBaseComponentsNum = baseComponentsNum;

				if( GCI::CxDef::isVertexAttribFormatValid( pFormat2 ) )
				{
					const auto format2BaseComponentsNum = GCI::CxDef::getVertexAttribFormatComponentsNum( pFormat2 );
					const auto format2BaseDataType = GCI::CxDef::getVertexAttribFormatBaseDataType( pFormat2 );
					const auto format2DataTypeFlags = GCI::CxDef::getVertexAttribFormatFlags( pFormat2 );

					if( cxGCIValidVertexAttributeComponentsNumRange.contains( packedBaseComponentsNum + format2BaseComponentsNum ) )
					{
						if( ( format2BaseDataType == baseDataType ) && ( format2DataTypeFlags == dataTypeFlags ) )
						{
							packedBaseComponentsNum += format2BaseComponentsNum;
						}
					}
				}

				const auto baseFormatValue = GCI::CxDef::makeVertexAttribFormatEnumValue(
						1,
						baseDataType,
						static_cast<uint8>( dataTypeFlags ) );

				const auto packedFormatValue = GCI::CxDef::makeVertexAttribFormatEnumValue(
						static_cast<uint8>( packedBaseComponentsNum ),
						baseDataType,
						static_cast<uint8>( dataTypeFlags ) );

				packedInfo.baseDataFormat = static_cast<GCI::EVertexAttribFormat>( baseFormatValue );
				packedInfo.packedDataFormat = static_cast<GCI::EVertexAttribFormat>( packedFormatValue );
				packedInfo.packedComponentsNum = packedBaseComponentsNum;
			}

			return packedInfo;
		}
		 */

	} // namespace GCU

}
