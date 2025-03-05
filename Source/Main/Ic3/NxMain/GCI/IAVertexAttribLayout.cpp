
#include "IAVertexAttribLayout.h"
#include <regex>

namespace Ic3
{

	bool VertexInputAttributeArrayConfig::CheckAttributeArraySpace(
			uint32 pBaseAttributeSlot,
			uint32 pSemanticGroupSize ) const noexcept
	{
		// First, check if the specified attribute range is valid at all.
		if( !GCIUtils::IsAttributeLocationAndSizeValid( pBaseAttributeSlot, pSemanticGroupSize ) )
		{
			return false;
		}

		// Quick check: if the specified attribute range is completely outside the range of active attributes,
		// we can immediately return with success (no attribute outside this range is an active attribute).
		if( !_activeAttributesRange.overlaps_with( cppx::make_range<uint32>( pBaseAttributeSlot, pBaseAttributeSlot + pSemanticGroupSize ) ) )
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
			const auto attributeIndex = pBaseAttributeSlot + iSubAttribute;
			const auto & attributeRef = _attributeArray[attributeIndex];

			if( attributeRef.IsActive() )
			{
				return false;
			}
		}

		return true;
	}

	bool VertexInputAttributeArrayConfig::CheckAttributeDefinitionCompatibility(
			const VertexInputAttributeDefinition & pAttributeDefinition ) const noexcept
	{
		if( !pAttributeDefinition.IsValid() )
		{
			return false;
		}

		if( !CheckAttributeArraySpace( pAttributeDefinition.attributeSlot, pAttributeDefinition.semanticGroupSize ) )
		{
			return false;
		}

		return true;
	}

	GenericVertexInputAttribute * VertexInputAttributeArrayConfig::AddAttribute(
			const VertexInputAttributeDefinition & pAttributeDefinition )
	{
		if( !CheckAttributeDefinitionCompatibility( pAttributeDefinition ) )
		{
			return nullptr;
		}

		return _AddAttributeImpl( pAttributeDefinition );
	}

	void VertexInputAttributeArrayConfig::Reset()
	{
		for( auto & attribute : _attributeArray )
		{
			attribute.Reset();
		}

		_activeBaseAttributesNum = 0;
		_activeAttributeSlotsNum = 0;
		_activeAttributesMask.clear();
		_activeAttributeSemanticsMask.clear();
		_activeAttributesRange = InputAssemblerSlotRange::empty_range();
		_activeAttributesSlots.clear();
		_semanticNameMap.clear();
	}

	GenericVertexInputAttribute * VertexInputAttributeArrayConfig::_AddAttributeImpl(
			const VertexInputAttributeDefinition & pAttributeDefinition )
	{
		if( !CheckAttributeDefinitionCompatibility( pAttributeDefinition ) )
		{
			return nullptr;
		}

		_activeAttributesSlots.reserve( _activeAttributesSlots.size() + pAttributeDefinition.semanticGroupSize );

		auto & baseAttribute = _attributeArray[pAttributeDefinition.attributeSlot];

		baseAttribute.InitBaseAttributeFromDefinition( pAttributeDefinition );

		_activeBaseAttributesNum += 1;
		_activeAttributeSlotsNum += 1;
		_activeAttributesMask.set( GCI::CXU::IAMakeVertexAttributeFlag( pAttributeDefinition.attributeSlot ) );
		_activeAttributeSemanticsMask.set( baseAttribute.semanticFlags );
		_activeAttributesSlots.insert( pAttributeDefinition.attributeSlot );
		_semanticNameMap[baseAttribute.semanticName.str_view()] = pAttributeDefinition.attributeSlot;

		for( uint32 nSemanticComponent = 1; nSemanticComponent < pAttributeDefinition.semanticGroupSize; ++nSemanticComponent )
		{
			const auto subAttributeSlot = pAttributeDefinition.attributeSlot + nSemanticComponent;

			auto & subAttribute = _attributeArray[subAttributeSlot];
			subAttribute.InitSemanticSubAttributeFromBaseAttribute( baseAttribute, nSemanticComponent );

			_activeAttributeSlotsNum += 1;
			_activeAttributesMask.set( GCI::CXU::IAMakeVertexAttributeFlag( subAttributeSlot ) );
			_activeAttributesSlots.insert( cppx::numeric_cast<gci_input_assembler_slot_t>( subAttributeSlot ) );
		}

		const auto lastAttributeIndex = pAttributeDefinition.attributeSlot + pAttributeDefinition.semanticGroupSize - 1;
		_activeAttributesRange.add( InputAssemblerSlotRange{ pAttributeDefinition.attributeSlot, lastAttributeIndex } );

		return &baseAttribute;
	}


	namespace GCIUtils
	{

		GCI::IAVertexAttributeDesc MakeGCIAttributeDesc(
				VertexAttributeKey pAttributeKey,
				uint32 pVertexStreamSlot,
				uint32 pVertexStreamRelativeOffset )
		{
			GCI::IAVertexAttributeDesc gciAttributeDesc{};
			gciAttributeDesc.attribInfo.attributeSlot = pAttributeKey.uBaseSlot;
			gciAttributeDesc.attribInfo.dataFormat = pAttributeKey.uBaseDataFormat;
			gciAttributeDesc.attribInfo.dataRate = pAttributeKey.GetDataRate();
			gciAttributeDesc.semantics = static_cast<GCI::IAVertexAttributeSemantics>( GetShaderSemanticsForAttributeKey( pAttributeKey ) );
			gciAttributeDesc.streamBinding.streamSlot = cppx::numeric_cast<decltype( gciAttributeDesc.streamBinding.streamSlot )>( pVertexStreamSlot );
			gciAttributeDesc.streamBinding.streamRelativeOffset = pVertexStreamRelativeOffset;
			return gciAttributeDesc;
		}

		GCI::EVertexAttribFormat GetAttributeFormatFromStringIdentifier( const std::string & pAttribFormatStringID )
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

				const auto formatComponentsNum = cppx::from_string_or_default<uint32>( formatComponentsNumStr );
				const auto formatComponentBitSize = cppx::from_string_or_default<uint32>( formatComponentBitSizeStr );
				const auto isNormalized = !formatNormalizedStr.empty();
				const auto isFloat = ( formatBaseTypeStr == "F" );
				const auto isSignedInt = !isFloat && ( formatBaseTypeStr == "I" );
				const auto isUnsignedInt = !isFloat && !isSignedInt && ( formatBaseTypeStr == "U" );

				GCI::EBaseDataType baseDataType = GCI::EBaseDataType::Undefined;
				cppx::bitmask<GCI::EGPUDataFormatFlags> dataFormatFlags = 0;

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
					else if( formatComponentBitSize == 32 )
					{
						baseDataType = GCI::EBaseDataType::Float32;
					}

					dataFormatFlags.set( GCI::eGPUDataFormatFlagTypeFloatingPointBit );
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
					else if( formatComponentBitSize == 32 )
					{
						baseDataType = GCI::EBaseDataType::Int32;
					}

					dataFormatFlags.set( GCI::eGPUDataFormatMaskSInt );
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
					else if( formatComponentBitSize == 32 )
					{
						baseDataType = GCI::EBaseDataType::Uint32;
					}

					dataFormatFlags.set( GCI::eGPUDataFormatMaskUInt );
				}

				if( isNormalized )
				{
					dataFormatFlags.set( GCI::eGPUDataFormatFlagNormalizedBit );
				}

				if( baseDataType != GCI::EBaseDataType::Undefined )
				{
					const auto vertexAttributeFormatValue = MakeVertexAttribFormatEnumValue(
						cppx::numeric_cast<uint8>( formatComponentsNum ),
						baseDataType,
						dataFormatFlags );

					vertexAttributeFormat = static_cast<GCI::EVertexAttribFormat>( vertexAttributeFormatValue );
				}
			}

			return vertexAttributeFormat;
		}

		cppx::string_view GetAttributeFormatStringIdentifier( GCI::EVertexAttribFormat pGCIAttributeFormat )
		{
			switch( pGCIAttributeFormat )
			{
				Ic3CaseReturn( GCI::EVertexAttribFormat::F16      , "1F16"  );
				Ic3CaseReturn( GCI::EVertexAttribFormat::F32      , "1F32"  );
				Ic3CaseReturn( GCI::EVertexAttribFormat::I8       , "1I8"   );
				Ic3CaseReturn( GCI::EVertexAttribFormat::I16      , "1I16"  );
				Ic3CaseReturn( GCI::EVertexAttribFormat::I32      , "1I32"  );
				Ic3CaseReturn( GCI::EVertexAttribFormat::U8       , "1U8"   );
				Ic3CaseReturn( GCI::EVertexAttribFormat::U16      , "1U16"  );
				Ic3CaseReturn( GCI::EVertexAttribFormat::U32      , "1U32"  );
				Ic3CaseReturn( GCI::EVertexAttribFormat::I8N      , "1I8N"  );
				Ic3CaseReturn( GCI::EVertexAttribFormat::I16N     , "1I16N" );
				Ic3CaseReturn( GCI::EVertexAttribFormat::U8N      , "1U8N"  );
				Ic3CaseReturn( GCI::EVertexAttribFormat::U16N     , "1U16N" );
				Ic3CaseReturn( GCI::EVertexAttribFormat::Vec2F16  , "2F16"  );
				Ic3CaseReturn( GCI::EVertexAttribFormat::Vec2F32  , "2F32"  );
				Ic3CaseReturn( GCI::EVertexAttribFormat::Vec2I8   , "2I8"   );
				Ic3CaseReturn( GCI::EVertexAttribFormat::Vec2I16  , "2I16"  );
				Ic3CaseReturn( GCI::EVertexAttribFormat::Vec2I32  , "2I32"  );
				Ic3CaseReturn( GCI::EVertexAttribFormat::Vec2U8   , "2U8"   );
				Ic3CaseReturn( GCI::EVertexAttribFormat::Vec2U16  , "2U16"  );
				Ic3CaseReturn( GCI::EVertexAttribFormat::Vec2U32  , "2U32"  );
				Ic3CaseReturn( GCI::EVertexAttribFormat::Vec2I8N  , "2I8N"  );
				Ic3CaseReturn( GCI::EVertexAttribFormat::Vec2I16N , "2I16N" );
				Ic3CaseReturn( GCI::EVertexAttribFormat::Vec2U8N  , "2U8N"  );
				Ic3CaseReturn( GCI::EVertexAttribFormat::Vec2U16N , "2U16N" );
				Ic3CaseReturn( GCI::EVertexAttribFormat::Vec3F32  , "3F32"  );
				Ic3CaseReturn( GCI::EVertexAttribFormat::Vec3I32  , "3I32"  );
				Ic3CaseReturn( GCI::EVertexAttribFormat::Vec3U32  , "3U32"  );
				Ic3CaseReturn( GCI::EVertexAttribFormat::Vec4F16  , "4F16"  );
				Ic3CaseReturn( GCI::EVertexAttribFormat::Vec4F32  , "4F32"  );
				Ic3CaseReturn( GCI::EVertexAttribFormat::Vec4I8   , "4I8"   );
				Ic3CaseReturn( GCI::EVertexAttribFormat::Vec4I16  , "4I16"  );
				Ic3CaseReturn( GCI::EVertexAttribFormat::Vec4I32  , "4I32"  );
				Ic3CaseReturn( GCI::EVertexAttribFormat::Vec4U8   , "4U8"   );
				Ic3CaseReturn( GCI::EVertexAttribFormat::Vec4U16  , "4U16"  );
				Ic3CaseReturn( GCI::EVertexAttribFormat::Vec4U32  , "4U32"  );
				Ic3CaseReturn( GCI::EVertexAttribFormat::Vec4I8N  , "4I8N"  );
				Ic3CaseReturn( GCI::EVertexAttribFormat::Vec4I16N , "4I16N" );
				Ic3CaseReturn( GCI::EVertexAttribFormat::Vec4U8N  , "4U8N"  );
				Ic3CaseReturn( GCI::EVertexAttribFormat::Vec4U16N , "4U16N" );
				Ic3CaseDefaultBreak();
			}

			return "#";
		}

		cppx::string_view GetShaderSemanticShortName( const cppx::string_view & pSemanticName )
		{
			if( pSemanticName == kVertexAttributeSemanticNameSysPosition         ) return "Pos"  ;
			if( pSemanticName == kVertexAttributeSemanticNameSysNormal           ) return "Nor"  ;
			if( pSemanticName == kVertexAttributeSemanticNameSysTangent          ) return "Tng"  ;
			if( pSemanticName == kVertexAttributeSemanticNameSysBiTangent        ) return "Btg"  ;
			if( pSemanticName == kVertexAttributeSemanticNameSysFixedColor       ) return "Col"  ;
			if( pSemanticName == kVertexAttributeSemanticNameSysTexCoord0        ) return "Uv0"  ;
			if( pSemanticName == kVertexAttributeSemanticNameSysTexCoord1        ) return "Uv1"  ;
			if( pSemanticName == kVertexAttributeSemanticNameSysTexCoord2        ) return "Uv2"  ;
			if( pSemanticName == kVertexAttributeSemanticNameSysTexCoord3        ) return "Uv3"  ;
			if( pSemanticName == kVertexAttributeSemanticNameSysTexCoord4        ) return "Uv4"  ;
			if( pSemanticName == kVertexAttributeSemanticNameSysTexCoord5        ) return "Uv5"  ;
			if( pSemanticName == kVertexAttributeSemanticNameSysTexCoordP01      ) return "UvP01";
			if( pSemanticName == kVertexAttributeSemanticNameSysTexCoordP23      ) return "UvP23";
			if( pSemanticName == kVertexAttributeSemanticNameSysBlendIndices     ) return "Bli"  ;
			if( pSemanticName == kVertexAttributeSemanticNameSysBlendWeights     ) return "Blw"  ;
			if( pSemanticName == kVertexAttributeSemanticNameSysInstanceMatrix   ) return "Imt"  ;
			if( pSemanticName == kVertexAttributeSemanticNameSysInstanceUserData ) return "Iud"  ;

			return nullptr;
		}

		cppx::string_view ResolveShaderSemanticShortName( const cppx::string_view & pSemanticName )
		{
			if( pSemanticName == "Pos"   ) return kVertexAttributeSemanticNameSysPosition;
			if( pSemanticName == "Nor"   ) return kVertexAttributeSemanticNameSysNormal;
			if( pSemanticName == "Tng"   ) return kVertexAttributeSemanticNameSysTangent;
			if( pSemanticName == "Btg"   ) return kVertexAttributeSemanticNameSysBiTangent;
			if( pSemanticName == "Col"   ) return kVertexAttributeSemanticNameSysFixedColor;
			if( pSemanticName == "Uv0"   ) return kVertexAttributeSemanticNameSysTexCoord0;
			if( pSemanticName == "Uv1"   ) return kVertexAttributeSemanticNameSysTexCoord1;
			if( pSemanticName == "Uv2"   ) return kVertexAttributeSemanticNameSysTexCoord2;
			if( pSemanticName == "Uv3"   ) return kVertexAttributeSemanticNameSysTexCoord3;
			if( pSemanticName == "Uv4"   ) return kVertexAttributeSemanticNameSysTexCoord4;
			if( pSemanticName == "Uv5"   ) return kVertexAttributeSemanticNameSysTexCoord5;
			if( pSemanticName == "Uvp01" ) return kVertexAttributeSemanticNameSysTexCoordP01;
			if( pSemanticName == "Uvp23" ) return kVertexAttributeSemanticNameSysTexCoordP23;
			if( pSemanticName == "Bli"   ) return kVertexAttributeSemanticNameSysBlendIndices;
			if( pSemanticName == "Blw"   ) return kVertexAttributeSemanticNameSysBlendWeights;
			if( pSemanticName == "Imt"   ) return kVertexAttributeSemanticNameSysInstanceMatrix;
			if( pSemanticName == "Iud"   ) return kVertexAttributeSemanticNameSysInstanceUserData;

			return nullptr;
		}

		std::string GenerateVertexAttributeFormatString( const GenericVertexInputAttribute & pAttribute )
		{
			std::string formatStr;
			formatStr.reserve( 16 );
			formatStr.append( 1, 'A' );
			formatStr.append( std::to_string( pAttribute.attributeSlot ) );

			if( const auto semanticsIDCStr = GetShaderSemanticShortName( pAttribute.semanticName.str_view() ) )
			{
				formatStr.append( semanticsIDCStr );
			}
			else
			{
				formatStr.append( pAttribute.semanticName.str_view() );
			}

			formatStr.append( 1, ':' );
			formatStr.append( std::to_string( pAttribute.vertexStreamRelativeOffset ) );

			const auto baseFormatStr = GetAttributeFormatStringIdentifier( pAttribute.dataFormat );
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

} // namespace Ic3
