
#include "IAVertexAttribLayout.h"
#include <regex>

namespace Ic3
{

	bool VertexAttributeArrayLayout::checkAttributeArraySpace( native_uint pBaseIASlot, size_t pSemanticGroupSize ) const noexcept
	{
		// First, check if the specified attribute range is valid at all.
		if( !GCU::isAttributeLocationAndSizeValid( pBaseIASlot, pSemanticGroupSize ) )
		{
			return false;
		}

		// Quick check: if the specified attribute range is completely outside the range of active attributes,
		// we can immediately return with success (no attribute outside this range is an active attribute).
		if( !_activeAttributesRange.overlapsWith( Cppx::makeRange<uint32>( pBaseIASlot, pBaseIASlot + pSemanticGroupSize ) ) )
		{
			return true;
		}

		// Iterate over the specified range and check the attributes. If at least one is already an active one,
		// return false (attributes cannot be redefined after they are specified).
		// Note: The range check above doesn't imply there is no space because the range may not be continuous.
		// Consider: If the layout has two active attributes: 0 and 5 the _activeAttributesRange would be <0;5>.
		// However, there are 4 unused attributes in that range that can be used.
		for( uint32 iSubAttribute = 0; iSubAttribute < pSemanticGroupSize; ++iSubAttribute )
		{
			const auto attributeSlotIndex = pBaseIASlot + iSubAttribute;
			const auto & attributeRef = _attributeArray[attributeSlotIndex];

			if( attributeRef.isActive() )
			{
				return false;
			}
		}

		return true;
	}

	bool VertexAttributeArrayLayout::checkAttributeDefinitionCompatibility(
			const VertexAttributeDefinition & pAttributeDefinition ) const noexcept
	{
		if( !pAttributeDefinition.isValid() )
		{
			return false;
		}

		if( !checkAttributeArraySpace( pAttributeDefinition.attributeIASlot, pAttributeDefinition.semanticGroupSize ) )
		{
			return false;
		}

		return true;
	}

	GenericVertexAttribute * VertexAttributeArrayLayout::addActiveAttribute( VertexAttributeDefinition pAttributeDefinition )
	{
		if( !checkAttributeDefinitionCompatibility( pAttributeDefinition ) )
		{
			return nullptr;
		}
		
		_activeAttributesSlots.reserve( _activeAttributesSlots.size() + pAttributeDefinition.semanticGroupSize );

		const auto baseAttributeSlotIndex = pAttributeDefinition.attributeIASlot;
		const auto semanticGroupSize = pAttributeDefinition.semanticGroupSize;

		auto & baseAttribute = _attributeArray[baseAttributeSlotIndex];
		baseAttribute.initBaseAttributeFromDefinition( pAttributeDefinition );

		_activeBaseAttributesNum += 1;
		_activeAttributeSlotsNum += 1;
		_activeAttributesMask.set( GCI::CxDef::makeIAVertexAttributeFlag( baseAttributeSlotIndex ) );
		_activeAttributeSemanticsMask.set( baseAttribute.shaderSemantics.systemSemanticFlags );
		_activeAttributesSlots.insert( baseAttributeSlotIndex );
		_semanticNameMap[baseAttribute.shaderSemantics.semanticName.strView()] = baseAttributeSlotIndex;

		for( uint32 nSemanticComponent = 1; nSemanticComponent < semanticGroupSize; ++nSemanticComponent )
		{
			const auto subAttributeSlotIndex = baseAttributeSlotIndex + nSemanticComponent;

			auto & subAttribute = _attributeArray[subAttributeSlotIndex];
			subAttribute.initSemanticSubAttributeFromBaseAttribute( baseAttribute, nSemanticComponent );

			_activeAttributeSlotsNum += 1;
			_activeAttributesMask.set( GCI::CxDef::makeIAVertexAttributeFlag( subAttributeSlotIndex ) );
			_activeAttributesSlots.insert( subAttributeSlotIndex );
		}

		const auto lastAttributeSlotIndex = baseAttributeSlotIndex + semanticGroupSize - 1;
		_activeAttributesRange.add( InputAssemblerSlotRange{ baseAttributeSlotIndex, lastAttributeSlotIndex } );

		return &baseAttribute;
	}


	namespace GCU
	{

		GCI::EVertexAttribFormat getAttributeFormatFromStringIdentifier( const std::string & pAttribFormatStringID )
		{
			static const std::string cvReStrBaseFormat{ R"(([1-4])(F|I|U)(8|16|32)(N)?)" };
			static const std::regex cvRegexBaseFormat{ cvReStrBaseFormat };

			auto vertexAttributeFormat = GCI::EVertexAttribFormat::Undefined;

			std::smatch regexMatch;
			if( std::regex_search( pAttribFormatStringID, regexMatch, cvRegexBaseFormat ) )
			{
				const auto & formatComponentsNumStr = regexMatch[1].str();
				const auto & formatBaseTypeStr = regexMatch[2].str();
				const auto & formatComponentBitSizeStr = regexMatch[3].str();
				const auto & formatNormalizedStr = regexMatch[4].str();

				const auto formatComponentsNum = Cppx::fromStringOrDefault<uint32>( formatComponentsNumStr );
				const auto formatComponentBitSize = Cppx::fromStringOrDefault<uint32>( formatComponentBitSizeStr );
				const auto isNormalized = !formatNormalizedStr.empty();
				const auto isFloat = ( formatBaseTypeStr == "F" );
				const auto isSignedInt = !isFloat && ( formatBaseTypeStr == "I" );
				const auto isUnsignedInt = !isFloat && !isSignedInt && ( formatBaseTypeStr == "U" );

				GCI::EBaseDataType baseDataType = GCI::EBaseDataType::Undefined;
				TBitmask<GCI::EGpuDataFormatFlags> dataFormatFlags = 0;

				if( isFloat && isNormalized )
				{
					return GCI::EVertexAttribFormat::Undefined;
				}

				if( ( isSignedInt || isUnsignedInt ) && ( formatComponentBitSize == 32 ) && isNormalized )
				{
					return GCI::EVertexAttribFormat::Undefined;
				}

				if( isFloat )
				{
					if( formatComponentBitSize == 16 )
					{
						baseDataType = GCI::EBaseDataType::Float16;
					}
					else if( formatComponentsNum == 32 )
					{
						baseDataType = GCI::EBaseDataType::Float32;
					}

					dataFormatFlags.set( GCI::eGpuDataFormatFlagTypeSignedBit );
				}
				else if( isSignedInt )
				{
					if( formatComponentBitSize == 8 )
					{
						baseDataType = GCI::EBaseDataType::Byte;
					}
					else if( formatComponentBitSize == 16 )
					{
						baseDataType = GCI::EBaseDataType::Int16;
					}
					else if( formatComponentsNum == 32 )
					{
						baseDataType = GCI::EBaseDataType::Int32;
					}

					dataFormatFlags.set( GCI::eGpuDataFormatFlagTypeSignedBit );
				}
				else if( isUnsignedInt )
				{
					if( formatComponentBitSize == 8 )
					{
						baseDataType = GCI::EBaseDataType::Ubyte;
					}
					else if( formatComponentBitSize == 16 )
					{
						baseDataType = GCI::EBaseDataType::Uint16;
					}
					else if( formatComponentsNum == 32 )
					{
						baseDataType = GCI::EBaseDataType::Uint32;
					}

					dataFormatFlags.set( GCI::eGpuDataFormatFlagTypeUnsignedBit );
				}

				if( isNormalized )
				{
					dataFormatFlags.set( GCI::eGpuDataFormatFlagNormalizedBit );
				}

				if( baseDataType != GCI::EBaseDataType::Undefined )
				{
					const auto vertexAttributeFormatValue =
							GCI::CxDef::makeVertexAttribFormatEnumValue( formatComponentsNum, baseDataType, dataFormatFlags );

					vertexAttributeFormat = static_cast<GCI::EVertexAttribFormat>( vertexAttributeFormatValue );
				}
			}

			return vertexAttributeFormat;
		}

		StringView getAttributeFormatStringIdentifier( GCI::EVertexAttribFormat pGCIAttributeFormat )
		{
			switch( pGCIAttributeFormat )
			{
				ic3CaseReturn( GCI::EVertexAttribFormat::F16      , "1F16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::F32      , "1F32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::I8       , "1I8"   );
				ic3CaseReturn( GCI::EVertexAttribFormat::I16      , "1I16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::I32      , "1I32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::U8       , "1U8"   );
				ic3CaseReturn( GCI::EVertexAttribFormat::U16      , "1U16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::U32      , "1U32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::I8N      , "1I8N"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::I16N     , "1I16N" );
				ic3CaseReturn( GCI::EVertexAttribFormat::U8N      , "1U8N"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::U16N     , "1U16N" );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2F16  , "2F16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2F32  , "2F32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2I8   , "2I8"   );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2I16  , "2I16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2I32  , "2I32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2U8   , "2U8"   );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2U16  , "2U16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2U32  , "2U32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2I8N  , "2I8N"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2I16N , "2I16N" );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2U8N  , "2U8N"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2U16N , "2U16N" );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec3F32  , "3F32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec3I32  , "3I32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec3U32  , "3U32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4F16  , "4F16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4F32  , "4F32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4I8   , "4I8"   );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4I16  , "4I16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4I32  , "4I32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4U8   , "4U8"   );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4U16  , "4U16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4U32  , "4U32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4I8N  , "4I8N"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4I16N , "4I16N" );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4U8N  , "4U8N"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4U16N , "4U16N" );
				ic3CaseDefaultBreak();
			}

			return "#";
		}

		StringView getShaderSemanticShortName( const StringView & pSemanticName )
		{
			if( pSemanticName == "POSITION"        ) return "Pos" ;
			if( pSemanticName == "NORMAL"          ) return "Nor" ;
			if( pSemanticName == "TANGENT"         ) return "Tng" ;
			if( pSemanticName == "BITANGENT"       ) return "Btg" ;
			if( pSemanticName == "COLOR"           ) return "Col" ;
			if( pSemanticName == "TEXCOORD0"       ) return "Uv0" ;
			if( pSemanticName == "TEXCOORD1"       ) return "Uv1" ;
			if( pSemanticName == "TEXCOORD2"       ) return "Uv2" ;
			if( pSemanticName == "TEXCOORD3"       ) return "Uv3" ;
			if( pSemanticName == "TEXCOORD4"       ) return "Uv4" ;
			if( pSemanticName == "TEXCOORD5"       ) return "Uv5" ;
			if( pSemanticName == "TEXCOORD6"       ) return "Uv6" ;
			if( pSemanticName == "TEXCOORD7"       ) return "Uv7" ;
			if( pSemanticName == "TEXCOORD01"      ) return "Uv01";
			if( pSemanticName == "TEXCOORD23"      ) return "Uv23";
			if( pSemanticName == "TEXCOORD45"      ) return "Uv45";
			if( pSemanticName == "TEXCOORD67"      ) return "Uv67";
			if( pSemanticName == "BLEND_INDICES"   ) return "Bli" ;
			if( pSemanticName == "BLEND_WEIGHTS"   ) return "Blw" ;
			if( pSemanticName == "INSTANCE_MATRIX" ) return "Imt" ;
			if( pSemanticName == "INSTANCE_UDATA"  ) return "Iud" ;

			return nullptr;
		}

		StringView resolveShaderSemanticShortName( const StringView & pSemanticName )
		{
			if( pSemanticName == "Pos"  ) return "POSITION";
			if( pSemanticName == "Nor"  ) return "NORMAL";
			if( pSemanticName == "Tng"  ) return "TANGENT";
			if( pSemanticName == "Btg"  ) return "BITANGENT";
			if( pSemanticName == "Col"  ) return "COLOR";
			if( pSemanticName == "Uv0"  ) return "TEXCOORD0";
			if( pSemanticName == "Uv1"  ) return "TEXCOORD1";
			if( pSemanticName == "Uv2"  ) return "TEXCOORD2";
			if( pSemanticName == "Uv3"  ) return "TEXCOORD3";
			if( pSemanticName == "Uv4"  ) return "TEXCOORD4";
			if( pSemanticName == "Uv5"  ) return "TEXCOORD5";
			if( pSemanticName == "Uv6"  ) return "TEXCOORD6";
			if( pSemanticName == "Uv7"  ) return "TEXCOORD7";
			if( pSemanticName == "Uv01" ) return "TEXCOORD01";
			if( pSemanticName == "Uv23" ) return "TEXCOORD23";
			if( pSemanticName == "Uv45" ) return "TEXCOORD45";
			if( pSemanticName == "Uv67" ) return "TEXCOORD67";
			if( pSemanticName == "Bli"  ) return "BLEND_INDICES";
			if( pSemanticName == "Blw"  ) return "BLEND_WEIGHTS";
			if( pSemanticName == "Imt"  ) return "INSTANCE_MATRIX";
			if( pSemanticName == "Iud"  ) return "INSTANCE_UDATA";

			return nullptr;
		}

		std::string generateVertexAttributeFormatString( const GenericVertexAttribute & pAttribute )
		{
			std::string formatStr;
			formatStr.reserve( 16 );
			formatStr.append( 1, 'A' );
			formatStr.append( std::to_string( pAttribute.attributeIASlot ) );

			if( const auto semanticsIDCStr = getShaderSemanticShortName( pAttribute.shaderSemantics.semanticName.strView() ) )
			{
				formatStr.append( semanticsIDCStr );
			}
			else
			{
				formatStr.append( pAttribute.shaderSemantics.semanticName.strView() );
			}

			formatStr.append( 1, ':' );
			formatStr.append( std::to_string( pAttribute.vertexStreamRelativeOffset ) );

			const auto baseFormatStr = getAttributeFormatStringIdentifier( pAttribute.dataFormat );
			formatStr.append( 1, ':' );
			formatStr.append( baseFormatStr );

			if( pAttribute.dataPadding > 0 )
			{
				formatStr.append( 1, '+' );
				formatStr.append( std::to_string( pAttribute.dataPadding ) );
			}

			if( pAttribute.semanticGroupSize > 1 )
			{
				formatStr.append( 1, '[' );
				formatStr.append( std::to_string( pAttribute.semanticGroupSize ) );
				formatStr.append( 1, ']' );
			}

			return formatStr;
		}

	}

	/*
	namespace GCU
	{

		GCI::EVertexAttribFormat getCombinedVertexAttribFormat(
				GCI::EVertexAttribFormat pFormat1,
				GCI::EVertexAttribFormat pFormat2 )
		{
			if( ( pFormat1 == GCI::EVertexAttribFormat::Undefined ) || ( pFormat2 == GCI::EVertexAttribFormat::Undefined ) )
			{
				return GCI::EVertexAttribFormat::Undefined;
			}

			const auto format1BaseComponentsNum = GCI::CxDef::getVertexAttribFormatComponentsNum( pFormat1 );
			const auto format1BaseDataType = GCI::CxDef::getVertexAttribFormatBaseDataType( pFormat1 );
			const auto format1DataTypeFlags = GCI::CxDef::getVertexAttribFormatFlags( pFormat1 );

			const auto format2BaseComponentsNum = GCI::CxDef::getVertexAttribFormatComponentsNum( pFormat2 );
			const auto format2BaseDataType = GCI::CxDef::getVertexAttribFormatBaseDataType( pFormat2 );
			const auto format2DataTypeFlags = GCI::CxDef::getVertexAttribFormatFlags( pFormat2 );

			const auto combinedBaseComponentsNum = format1BaseComponentsNum + format2BaseComponentsNum;

			if( ( combinedBaseComponentsNum == 0 ) || ( combinedBaseComponentsNum > 4 ) )
			{
				return GCI::EVertexAttribFormat::Undefined;
			}

			if( ( format1BaseDataType != format2BaseDataType ) || ( format1DataTypeFlags != format2DataTypeFlags ) )
			{
				return GCI::EVertexAttribFormat::Undefined;
			}

			const auto combinedVertexAttribFormatValue = GCI::CxDef::makeVertexAttribFormatEnumValue(
					static_cast<uint8>( combinedBaseComponentsNum ),
					format1BaseDataType,
					static_cast<uint8>( format2DataTypeFlags ) );

			const auto combinedVertexAttribFormat = static_cast<GCI::EVertexAttribFormat>( combinedVertexAttribFormatValue );

			return combinedVertexAttribFormat;
		}

		GCI::EVertexAttribFormat getCombinedVertexAttribFormat(
				const GCI::EVertexAttribFormat * pFormatArray,
				size_t pFormatsNum )
		{
			if( !pFormatArray || !Cppx::makeRange<size_t>( 1, 4 ).contains( pFormatsNum ) )
			{
				return GCI::EVertexAttribFormat::Undefined;
			}

			auto combinedBaseDataType = GCI::EBaseDataType::Undefined;
			auto combinedBaseComponentsNum = 0u;
			auto combinedDataTypeFlags = Cppx::makeBitmask<GCI::EGpuDataFormatFlags>();

			for( size_t iFormat = 0; iFormat < pFormatsNum; ++iFormat )
			{
				const auto format = pFormatArray[iFormat];
				if( format == GCI::EVertexAttribFormat::Undefined )
				{
					continue;
				}

				const auto formatBaseComponentsNum = GCI::CxDef::getVertexAttribFormatComponentsNum( format );
				const auto formatBaseDataType = GCI::CxDef::getVertexAttribFormatBaseDataType( format );
				const auto formatDataTypeFlags = GCI::CxDef::getVertexAttribFormatFlags( format );

				if( combinedBaseDataType == GCI::EBaseDataType::Undefined )
				{
					combinedBaseDataType = formatBaseDataType;
					combinedBaseComponentsNum = formatBaseComponentsNum;
					combinedDataTypeFlags = formatDataTypeFlags;
				}
				else
				{
					if( formatBaseDataType != combinedBaseDataType )
					{
						combinedBaseDataType = GCI::EBaseDataType::Undefined;
						break;
					}

					if( formatBaseComponentsNum + combinedBaseComponentsNum > 4 )
					{
						combinedBaseDataType = GCI::EBaseDataType::Undefined;
						break;
					}

					if( formatDataTypeFlags != combinedDataTypeFlags )
					{
						combinedBaseDataType = GCI::EBaseDataType::Undefined;
						break;
					}

					combinedBaseComponentsNum += formatBaseComponentsNum;
				}
			}

			if( combinedBaseDataType == GCI::EBaseDataType::Undefined )
			{
				return GCI::EVertexAttribFormat::Undefined;
			}

			const auto combinedVertexAttribFormatValue = GCI::CxDef::makeVertexAttribFormatEnumValue(
					static_cast<uint8>( combinedBaseComponentsNum ),
					combinedBaseDataType,
					static_cast<uint8>( combinedDataTypeFlags ) );

			const auto combinedVertexAttribFormat = static_cast<GCI::EVertexAttribFormat>( combinedVertexAttribFormatValue );

			return combinedVertexAttribFormat;
		}

		GCI::EVertexAttribFormat getCombinedVertexAttribFormat(
				GCI::EVertexAttribFormat pFormat1,
				GCI::EVertexAttribFormat pFormat2,
				GCI::EVertexAttribFormat pFormat3,
				GCI::EVertexAttribFormat pFormat4 )
		{
			uint32 numOfBaseTypeComponents = 0;
			GCI::EBaseDataType baseDataType = GCI::EBaseDataType::Undefined;

			if( pFormat1 != GCI::EVertexAttribFormat::Undefined )
			{
				numOfBaseTypeComponents = GCI::CxDef::getVertexAttribFormatComponentsNum( pFormat1 );
				baseDataType = GCI::CxDef::getVertexAttribFormatBaseDataType( pFormat1 );
			}

			if( pFormat2 != GCI::EVertexAttribFormat::Undefined )
			{
				numOfBaseTypeComponents = GCI::CxDef::getVertexAttribFormatComponentsNum( pFormat1 );
				baseDataType = GCI::CxDef::getVertexAttribFormatBaseDataType( pFormat1 );
			}
		}

	} // namespace GCU
	*/

	/*
	struct VertexAttributeIndexCmpEqual
	{
		IC3_ATTR_NO_DISCARD bool operator()( const VertexAttributeComponent & pLhs, size_t pRhs ) const noexcept
		{
			return pLhs.attributeIASlot == pRhs;
		}
	};

	struct VertexAttributeIndexCmpLess
	{
		IC3_ATTR_NO_DISCARD bool operator()( const VertexAttributeComponent & pLhs, size_t pRhs ) const noexcept
		{
			return pLhs.attributeIASlot < pRhs;
		}
	};

	size_t VertexAttributeArrayLayout::findAttributeAtSlot( gci_input_assembler_slot_t pAttribIASlot ) const noexcept
	{
		auto attribIter = _attributeArray.find( pAttribIASlot, VertexAttributeIndexCmpLess{}, VertexAttributeIndexCmpEqual{} );
		return ( attribIter != _attributeArray.end() ) ? ( attribIter - _attributeArray.begin() ) : cxInvalidPosition;
	}

	bool VertexAttributeArrayLayout::checkAttributeArraySpace(
			gci_input_assembler_slot_t pAttribBaseIASlot,
			size_t pComponentsNum ) const noexcept
	{
		for( uint32 nComponent = 0; nComponent < pComponentsNum; ++nComponent )
		{
			const auto vertexPropertySlot = pAttribBaseIASlot + nComponent;
			const auto attributeArrayIndex = findAttributeAtSlot( vertexPropertySlot );

			if( attributeArrayIndex != cxInvalidPosition )
			{
				ic3DebugAssert( _activeAttributesMask.isSet( GCI::CxDef::makeIAVertexAttributeFlag( vertexPropertySlot ) ) );
				ic3DebugAssert( _activeAttributesRange.contains( vertexPropertySlot ) );
				ic3DebugAssert( _activeAttributesSlots.hasValue( vertexPropertySlot ) );
				return false;
			}
		}

		return true;
	}

	bool VertexAttributeArrayLayout::checkAttributeDefinitionCompatibility(
			const VertexAttributeDefinition & pAttributeDefinition ) const noexcept
	{
		if( !pAttributeDefinition.valid() )
		{
			return false;
		}

		if( !checkAttributeArraySpace( pAttributeDefinition.attributeIASlot, pAttributeDefinition.semanticComponentsNum ) )
		{
			return false;
		}

		return true;
	}

	VertexAttributeComponent * VertexAttributeArrayLayout::addActiveAttribute( VertexAttributeDefinition pAttributeDefinition )
	{
		auto attributeArrayIndex = findAttributeAtSlot( pAttributeDefinition.attributeIASlot );
		if( attributeArrayIndex != cxInvalidPosition )
		{
			ic3DebugInterrupt();
			return nullptr;
		}

		_attributeArray.reserve( _attributeArray.size() + pAttributeDefinition.semanticComponentsNum );
		_activeAttributesSlots.reserve( _activeAttributesSlots.size() + pAttributeDefinition.semanticComponentsNum );

		auto baseAttribIter = _attributeArray.insert( VertexAttributeComponent{ pAttributeDefinition.attributeIASlot } );
		baseAttribIter->initBaseAttributeFromDefinition( std::move( pAttributeDefinition ) );

		const auto baseAttribute = pAttributeDefinition.attributeIASlot;
		const auto attribComponentsNum = pAttributeDefinition.semanticComponentsNum;
		const auto baseAttributeArrayIndex = baseAttribIter - _attributeArray.begin();

		_activeAttributesNum += 1;
		_activeAttributesMask.set( GCI::CxDef::makeIAVertexAttributeFlag( baseAttribute) );
		_activeAttributeSemanticsMask.set( CxDef::getShaderInputSemanticIDFlags( baseAttribIter->shaderSemantics.smtID ) );
		_activeAttributesSlots.insert( baseAttribute );
		_semanticNameMap[baseAttribIter->shaderSemantics.smtName] = baseAttribute;

		for( uint32 nSemanticComponent = 1; nSemanticComponent < attribComponentsNum; ++nSemanticComponent )
		{
			const auto attribComponentSlot = baseAttribute + nSemanticComponent;

			auto attribComponentIter = _attributeArray.insert( VertexAttributeComponent{ attribComponentSlot } );
			attribComponentIter->initSubComponentFromBaseAttribute( *baseAttribIter, nSemanticComponent );

			_activeAttributesNum += 1;
			_activeAttributesMask.set( GCI::CxDef::makeIAVertexAttributeFlag( attribComponentSlot ) );
			_activeAttributesSlots.insert( attribComponentSlot );
		}

		const auto lastComponentSlot = baseAttribute + attribComponentsNum - 1;
		_activeAttributesRange.add( InputAssemblerSlotRange{ baseAttribute, lastComponentSlot } );

		return &( _attributeArray[baseAttributeArrayIndex] );
	}

	void VertexAttributeArrayLayout::reserveAttributeArraySpace( size_t pActiveAttributesNum )
	{
		_attributeArray.reserve( pActiveAttributesNum );
	}

	void VertexAttributeArrayLayout::reset()
	{
		for( auto & attributeInfo : _attributeArray )
		{
			attributeInfo.reset();
		}

		_activeAttributesRange = InputAssemblerSlotRange::emptyRange();
		_activeAttributesSlots.clear();
		_activeAttributesMask.clear();
		_activeAttributeSemanticsMask.clear();
		_semanticNameMap.clear();
	}


	namespace GCU
	{

		GCI::EVertexAttribFormat getAttributeFormatFromStringIdentifier( const std::string & pAttribFormatStringID )
		{
			static const std::string cvReStrBaseFormat{ R"(([1-4])(F|I|U)(8|16|32)(N)?)" };
			static const std::regex cvRegexBaseFormat{ cvReStrBaseFormat };

			auto vertexAttributeFormat = GCI::EVertexAttribFormat::Undefined;

			std::smatch regexMatch;
			if( std::regex_search( pAttribFormatStringID, regexMatch, cvRegexBaseFormat ) )
			{
				const auto & formatComponentsNumStr = regexMatch[1].str();
				const auto & formatBaseTypeStr = regexMatch[2].str();
				const auto & formatComponentBitSizeStr = regexMatch[3].str();
				const auto & formatNormalizedStr = regexMatch[4].str();

				const auto formatComponentsNum = Cppx::fromStringOrDefault<uint32>( formatComponentsNumStr );
				const auto formatComponentBitSize = Cppx::fromStringOrDefault<uint32>( formatComponentBitSizeStr );
				const auto isNormalized = !formatNormalizedStr.empty();
				const auto isFloat = ( formatBaseTypeStr == "F" );
				const auto isSignedInt = !isFloat && ( formatBaseTypeStr == "I" );
				const auto isUnsignedInt = !isFloat && !isSignedInt && ( formatBaseTypeStr == "U" );

				GCI::EBaseDataType baseDataType = GCI::EBaseDataType::Undefined;
				TBitmask<GCI::EGpuDataFormatFlags> dataFormatFlags = 0;

				if( isFloat && isNormalized )
				{
					return GCI::EVertexAttribFormat::Undefined;
				}

				if( ( isSignedInt || isUnsignedInt ) && ( formatComponentBitSize == 32 ) && isNormalized )
				{
					return GCI::EVertexAttribFormat::Undefined;
				}

				if( isFloat )
				{
					if( formatComponentBitSize == 16 )
					{
						baseDataType = GCI::EBaseDataType::Float16;
					}
					else if( formatComponentsNum == 32 )
					{
						baseDataType = GCI::EBaseDataType::Float32;
					}

					dataFormatFlags.set( GCI::eGpuDataFormatFlagTypeSignedBit );
				}
				else if( isSignedInt )
				{
					if( formatComponentBitSize == 8 )
					{
						baseDataType = GCI::EBaseDataType::Byte;
					}
					else if( formatComponentBitSize == 16 )
					{
						baseDataType = GCI::EBaseDataType::Int16;
					}
					else if( formatComponentsNum == 32 )
					{
						baseDataType = GCI::EBaseDataType::Int32;
					}

					dataFormatFlags.set( GCI::eGpuDataFormatFlagTypeSignedBit );
				}
				else if( isUnsignedInt )
				{
					if( formatComponentBitSize == 8 )
					{
						baseDataType = GCI::EBaseDataType::Ubyte;
					}
					else if( formatComponentBitSize == 16 )
					{
						baseDataType = GCI::EBaseDataType::Uint16;
					}
					else if( formatComponentsNum == 32 )
					{
						baseDataType = GCI::EBaseDataType::Uint32;
					}

					dataFormatFlags.set( GCI::eGpuDataFormatFlagTypeUnsignedBit );
				}

				if( isNormalized )
				{
					dataFormatFlags.set( GCI::eGpuDataFormatFlagNormalizedBit );
				}

				if( baseDataType != GCI::EBaseDataType::Undefined )
				{
					const auto vertexAttributeFormatValue =
							GCI::CxDef::makeVertexAttribFormatEnumValue( baseDataType, formatComponentsNum, dataFormatFlags );

					vertexAttributeFormat = static_cast<GCI::EVertexAttribFormat>( vertexAttributeFormatValue );
				}
			}

			return vertexAttributeFormat;
		}

		StringView getAttributeFormatStringIdentifier( GCI::EVertexAttribFormat pGCIAttributeFormat )
		{
			switch( pGCIAttributeFormat )
			{
				ic3CaseReturn( GCI::EVertexAttribFormat::F16      , "1F16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::F32      , "1F32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::I8       , "1I8"   );
				ic3CaseReturn( GCI::EVertexAttribFormat::I16      , "1I16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::I32      , "1I32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::U8       , "1U8"   );
				ic3CaseReturn( GCI::EVertexAttribFormat::U16      , "1U16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::U32      , "1U32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::I8N      , "1I8N"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::I16N     , "1I16N" );
				ic3CaseReturn( GCI::EVertexAttribFormat::U8N      , "1U8N"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::U16N     , "1U16N" );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2F16  , "2F16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2F32  , "2F32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2I8   , "2I8"   );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2I16  , "2I16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2I32  , "2I32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2U8   , "2U8"   );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2U16  , "2U16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2U32  , "2U32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2I8N  , "2I8N"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2I16N , "2I16N" );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2U8N  , "2U8N"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2U16N , "2U16N" );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec3F32  , "3F32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec3I32  , "3I32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec3U32  , "3U32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4F16  , "4F16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4F32  , "4F32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4I8   , "4I8"   );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4I16  , "4I16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4I32  , "4I32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4U8   , "4U8"   );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4U16  , "4U16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4U32  , "4U32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4I8N  , "4I8N"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4I16N , "4I16N" );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4U8N  , "4U8N"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4U16N , "4U16N" );
				ic3CaseDefaultBreak();
			}

			return "#";
		}

		StringView getShaderSemanticShortName( const std::string_view & pSemanticName )
		{
			if( pSemanticName == "POSITION"        ) return "Pos";
			if( pSemanticName == "NORMAL"          ) return "Nor";
			if( pSemanticName == "TANGENT"         ) return "Tng";
			if( pSemanticName == "BITANGENT"       ) return "Btg";
			if( pSemanticName == "COLOR"           ) return "Col";
			if( pSemanticName == "TEXCOORD0"       ) return "Uv0";
			if( pSemanticName == "TEXCOORD1"       ) return "Uv1";
			if( pSemanticName == "TEXCOORD2"       ) return "Uv2";
			if( pSemanticName == "TEXCOORD3"       ) return "Uv3";
			if( pSemanticName == "TEXCOORD4"       ) return "Uv4";
			if( pSemanticName == "TEXCOORD5"       ) return "Uv5";
			if( pSemanticName == "TEXCOORD6"       ) return "Uv6";
			if( pSemanticName == "TEXCOORD7"       ) return "Uv7";
			if( pSemanticName == "BLEND_INDICES"   ) return "Bli";
			if( pSemanticName == "BLEND_WEIGHTS"   ) return "Blw";
			if( pSemanticName == "INSTANCE_MATRIX" ) return "Imt";
			if( pSemanticName == "INSTANCE_UDATA"  ) return "Iud";

			return nullptr;
		}

		StringView resolveShaderSemanticShortName( const std::string_view & pSemanticName )
		{
			if( pSemanticName == "Pos" ) return "POSITION";
			if( pSemanticName == "Nor" ) return "NORMAL";
			if( pSemanticName == "Tng" ) return "TANGENT";
			if( pSemanticName == "Btg" ) return "BITANGENT";
			if( pSemanticName == "Col" ) return "COLOR";
			if( pSemanticName == "Uv0" ) return "TEXCOORD0";
			if( pSemanticName == "Uv1" ) return "TEXCOORD1";
			if( pSemanticName == "Uv2" ) return "TEXCOORD2";
			if( pSemanticName == "Uv3" ) return "TEXCOORD3";
			if( pSemanticName == "Uv4" ) return "TEXCOORD4";
			if( pSemanticName == "Uv5" ) return "TEXCOORD5";
			if( pSemanticName == "Uv6" ) return "TEXCOORD6";
			if( pSemanticName == "Uv7" ) return "TEXCOORD7";
			if( pSemanticName == "Bli" ) return "BLEND_INDICES";
			if( pSemanticName == "Blw" ) return "BLEND_WEIGHTS";
			if( pSemanticName == "Imt" ) return "INSTANCE_MATRIX";
			if( pSemanticName == "Iud" ) return "INSTANCE_UDATA";

			return nullptr;
		}

		std::string generateVertexAttributeFormatString( const VertexAttributeComponent & pAttribute )
		{
			std::string formatStr;
			formatStr.reserve( 16 );
			formatStr.append( 1, 'A' );
			formatStr.append( std::to_string( pAttribute.attributeIASlot ) );

			if( const auto semanticsIDCStr = getShaderSemanticShortName( pAttribute.shaderSemantics.smtName ) )
			{
				formatStr.append( semanticsIDCStr );
			}
			else
			{
				formatStr.append( pAttribute.shaderSemantics.smtName );
			}

			formatStr.append( 1, ':' );
			formatStr.append( std::to_string( pAttribute.vertexStreamRelativeOffset ) );

			const auto baseFormatStr = getAttributeFormatStringIdentifier( pAttribute.baseFormat );
			formatStr.append( 1, ':' );
			formatStr.append( baseFormatStr );

			if( pAttribute.componentPadding > 0 )
			{
				formatStr.append( 1, '+' );
				formatStr.append( std::to_string( pAttribute.componentPadding ) );
			}

			if( pAttribute.semanticComponentsNum > 1 )
			{
				formatStr.append( 1, '[' );
				formatStr.append( std::to_string( pAttribute.semanticComponentsNum ) );
				formatStr.append( 1, ']' );
			}

			return formatStr;
		}

	} // namespace GCU

	 */

} // namespace Ic3
