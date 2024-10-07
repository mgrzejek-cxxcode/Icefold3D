
#include "IAVertexAttribLayout.h"
#include <regex>

namespace Ic3
{

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
		auto attribIter = _activeAttributes.find( pAttribIASlot, VertexAttributeIndexCmpLess{}, VertexAttributeIndexCmpEqual{} );
		return ( attribIter != _activeAttributes.end() ) ? ( attribIter - _activeAttributes.begin() ) : cxInvalidPosition;
	}

	bool VertexAttributeArrayLayout::checkAttributeArraySpace(
			gci_input_assembler_slot_t pAttribBaseIASlot,
			size_t pComponentsNum ) const noexcept
	{
		for( uint32 nComponent = 0; nComponent < pComponentsNum; ++nComponent )
		{
			const auto attributeComponentSlot = pAttribBaseIASlot + nComponent;
			const auto attributeArrayIndex = findAttributeAtSlot( attributeComponentSlot );

			if( attributeArrayIndex != cxInvalidPosition )
			{
				ic3DebugAssert( _activeAttributesMask.isSet( GCI::CxDef::makeIAVertexAttributeFlag( attributeComponentSlot ) ) );
				ic3DebugAssert( _activeAttributesRange.contains( attributeComponentSlot ) );
				ic3DebugAssert( _activeAttributesSlots.hasValue( attributeComponentSlot ) );
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

		_activeAttributes.reserve( _activeAttributes.size() + pAttributeDefinition.semanticComponentsNum );
		_activeAttributesSlots.reserve( _activeAttributesSlots.size() + pAttributeDefinition.semanticComponentsNum );

		auto baseAttribIter = _activeAttributes.insert( VertexAttributeComponent{ pAttributeDefinition.attributeIASlot } );
		baseAttribIter->initBaseAttributeFromDefinition( std::move( pAttributeDefinition ) );

		const auto baseAttributeSlot = pAttributeDefinition.attributeIASlot;
		const auto attribComponentsNum = pAttributeDefinition.semanticComponentsNum;
		const auto baseAttributeArrayIndex = baseAttribIter - _activeAttributes.begin();

		_activeAttributesNum += 1;
		_activeAttributesMask.set( GCI::CxDef::makeIAVertexAttributeFlag( baseAttributeSlot) );
		_activeAttributeSemanticsMask.set( CxDef::getShaderInputSemanticIDFlags( baseAttribIter->shaderSemantics.smtID ) );
		_activeAttributesSlots.insert( baseAttributeSlot );
		_semanticNameMap[baseAttribIter->shaderSemantics.smtName] = baseAttributeSlot;

		for( uint32 nSemanticComponent = 1; nSemanticComponent < attribComponentsNum; ++nSemanticComponent )
		{
			const auto attribComponentSlot = baseAttributeSlot + nSemanticComponent;

			auto attribComponentIter = _activeAttributes.insert( VertexAttributeComponent{ attribComponentSlot } );
			attribComponentIter->initSubComponentFromBaseAttribute( *baseAttribIter, nSemanticComponent );

			_activeAttributesNum += 1;
			_activeAttributesMask.set( GCI::CxDef::makeIAVertexAttributeFlag( attribComponentSlot ) );
			_activeAttributesSlots.insert( attribComponentSlot );
		}

		const auto lastComponentSlot = baseAttributeSlot + attribComponentsNum - 1;
		_activeAttributesRange.add( InputAssemblerSlotRange{ baseAttributeSlot, lastComponentSlot } );

		return &( _activeAttributes[baseAttributeArrayIndex] );
	}

	void VertexAttributeArrayLayout::reserveAttributeArraySpace( size_t pActiveAttributesNum )
	{
		_activeAttributes.reserve( pActiveAttributesNum );
	}

	void VertexAttributeArrayLayout::reset()
	{
		for( auto & attributeInfo : _activeAttributes )
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
					return GCI::EVertexAttribFormat::Unknown;
				}

				if( ( isSignedInt || isUnsignedInt ) && ( formatComponentBitSize == 32 ) && isNormalized )
				{
					return GCI::EVertexAttribFormat::Unknown;
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
							GCI::CxDef::declareVertexAttribFormat( baseDataType, formatComponentsNum, dataFormatFlags );

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

} // namespace Ic3
