
#include "VertexFormatParsing.h"
#include "VertexFormatCombinedLayout.h"
#include <Ic3/Graphics/GCI/State/InputAssemblerCommon.h>
#include <regex>

namespace Ic3
{

	namespace VertexFormat
	{

		GCI::EVertexAttribFormat ParseAttributeFormatSerialString( const std::string & pAttributeFormatSerialString )
		{
			static const std::string cvReStrBaseFormat{ R"(([1-4])(F|I|U)(8|16|32)(N)?)" };
			static const std::regex cvRegexBaseFormat{ cvReStrBaseFormat };

			auto vertexAttributeFormat = GCI::EVertexAttribFormat::Undefined;

			std::smatch regexMatch;
			if( std::regex_search( pAttributeFormatSerialString, regexMatch, cvRegexBaseFormat ) )
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
					const auto vertexAttributeFormatValue = GCI::CXU::MakeVertexAttribFormatEnumValue(
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

		std::string GenerateAttributeSerialString( const GenericVertexInputAttribute & pAttribute )
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

		// Example VFS (second stream S1 set intentionally as empty):
		// "#S0(V)<A0POSITION:0:3F32|A1NORMAL:12:3F32|A2TANGENT:24:3F32|A3BITANGENT:36:3F32|A5TEXCOORD0:48:2F32>#S1(I)<>#S2(I)<A14INSTANCE_UDATA:0:2F32[2]>"

		// Attribute format: A{1}{2}:{3}:{4}[+{5}][[{6}]]
		// {1} -> Input Assembler slot
		// {2} -> Semantic name
		// {3} -> Relative offset (from start of the Vertex Stream)
		// {4} -> Base format (format of a single component)
		// {5} -> Additional per-component padding (OPTIONAL)
		// {6} -> Array size (i.e. number of components, i.e. number of occupied slots) (OPTIONAL)
		// Example: A14INSTANCE_DATA:0:2F32[2]
		// Meaning: Attribute "INSTANCE_DATA" starts at slot 14 and has 2 components (occupies 2 IA slots).
		//          Each component is a 2-component vector of 32-bit floats. Attribute's data is located at the
		//          beginning of its vertex stream (relative offset = 0).
		#define RE_STR_VFS_ATTRIBUTE R"(A([0-9]+)([A-Z][A-Za-z0-9_]+)\:([0-9]+)\:([A-Z0-9]+)(\+([0-9]+))?(\[([0-9])\])?)"

		// Stream format: S{1}({2})<{3}>
		// {1} -> Input Assembler slot
		// {2} -> Data rate: "V" means per-vertex, "I" means per-instance.
		// {3} -> List of attributes separated with "|" (OPTIONAL).
		#define RE_STR_VFS_STREAM R"(S([0-9]+)\((V|I)\)\=([0-9]+)<([a-zA-Z0-9_:|\+\[\]]+)?>)"

		static const std::regex cvReVfsAttribute{ RE_STR_VFS_ATTRIBUTE };
		static const std::regex cvReVfsStream{ RE_STR_VFS_STREAM };

		std::string GenerateVertexFormatSignatureSerialString(
				const VertexFormatCombinedLayoutBase & pVertexFormatCombinedLayout )
		{
			std::string vertexStreamStrings[GCM::kIAMaxDataStreamVertexBuffersNum];

			const auto & activeAttributes = pVertexFormatCombinedLayout.GetActiveAttributesView();
			for( const auto & genericAttribute : activeAttributes )
			{
				Ic3DebugAssert( genericAttribute.IsActive() )
				if( genericAttribute.IsBaseAttribute() )
				{
					const auto attributeFormatStr = GenerateAttributeSerialString( genericAttribute );
					if( !vertexStreamStrings[genericAttribute.vertexStreamSlot].empty() )
					{
						vertexStreamStrings[genericAttribute.vertexStreamSlot].append( 1, '|' );
					}
					vertexStreamStrings[genericAttribute.vertexStreamSlot].append( attributeFormatStr );
				}
			}

			std::string resultStringID;

			const auto & activeStreams = pVertexFormatCombinedLayout.GetActiveStreamsView();
			for( const auto & vertexStream : activeStreams )
			{
				if( vertexStream.IsActive() )
				{
					if( !vertexStreamStrings[vertexStream.streamSlot].empty() )
					{
						resultStringID.append( 1, '#' );
						resultStringID.append( 1, 'S' );
						resultStringID.append( std::to_string( vertexStream.streamSlot ) );
						resultStringID.append( 1, '(' );
						resultStringID.append( 1, ( vertexStream.streamDataRate == GCI::EIAVertexAttributeDataRate::PerInstance ) ? 'I' : 'V' );
						resultStringID.append( 1, ')' );
						resultStringID.append( 1, '=' );
						resultStringID.append( std::to_string( vertexStream.dataStrideInBytes ) );
						resultStringID.append( 1, '<' );
						resultStringID.append( vertexStreamStrings[vertexStream.streamSlot] );
						resultStringID.append( 1, '>' );
					}
				}
			}

			return resultStringID;
		}

		std::vector<VertexInputAttributeDefinition> ParseVertexFormatSignatureSerialString(
			const std::string_view & pVertexFormatString )
		{
			std::vector<VertexInputAttributeDefinition> vertexAttribsDefinitions{};

			using StrArray = std::vector<std::string>;
			auto vertexStreamStrings = cppx::strutil::split_string_ex<StrArray>(
					pVertexFormatString, '#', []( auto & pResult, auto * pStr, auto pLength ) {
						pResult.push_back( std::string( pStr, pLength ) );
					} );

			StrArray vsAttributesStrings{};
			for( const auto & vertexStreamStr : vertexStreamStrings )
			{
				std::smatch regexMatch;
				if( std::regex_search( vertexStreamStr, regexMatch, cvReVfsStream ) )
				{
					if( regexMatch.size() < 3 )
					{
						continue;
					}

					// [0]: The whole matched expression.
					const auto & debugView = regexMatch[0].str();
					// [1]: Stream slot.
					const auto & streamIndexStr = regexMatch[1].str();
					// [2]: Stream data rate (V - per-vertex, I - per-instance).
					const auto & streamDataRateStr = regexMatch[2].str();
					// [3]: Stream data stride (in bytes).
					const auto & streamDataStrideStr = regexMatch[3].str();
					// [4]: String with attributes contained within this stream.
					const auto & streamAttributesCombinedStr = regexMatch[4].str();

					if( streamIndexStr.empty() || streamDataRateStr.empty() || streamAttributesCombinedStr.empty() )
					{
						continue;
					}

					const auto streamIndex = cppx::from_string_or_default<uint8>( streamIndexStr, GCI::kIAVertexStreamSlotUndefined );
					if( !GCI::CXU::IAIsDataStreamVertexBufferSlotValid( streamIndex ) )
					{
						continue;
					}

					const auto streamDataRate = ( streamDataRateStr == "I" ) ? GCI::EIAVertexAttributeDataRate::PerInstance : GCI::EIAVertexAttributeDataRate::PerVertex;

					cppx::strutil::split_string(
							streamAttributesCombinedStr, '|',
							[&vsAttributesStrings]( auto * pStr, auto pLength ) {
								vsAttributesStrings.push_back( std::string( pStr, pLength ) );
							} );

					for( const auto & attributeStr : vsAttributesStrings )
					{
						if( std::regex_search( attributeStr, regexMatch, cvReVfsAttribute ) )
						{
							const auto regexSize= regexMatch.size();

							// Let's assume the following attribute string: A12INSTANCE_MATRIX:64:3F32+4[4].
							// (Per-instance matrix, located at slot 12 (12 is the base slot, i.e. slot of the first component),
							// consisted of four components of type Vec3f (3F32), each with 4 bytes of added padding)

							// [0]: The whole matched expression.
							const auto & attribDebugView = regexMatch[0].str();
							// [1]: Attribute slot (example: "12").
							const auto & attributeSlotStr = regexMatch[1].str();
							// [2]: The semantic name of the attribute (example: "INSTANCE_MATRIX").
							const auto & attribSemanticsStr = regexMatch[2].str();
							// [3]: Attribute offset relative to the beginning of its vertex stream (example: "64").
							const auto & attribRelativeOffsetStr = regexMatch[3].str();
							// [4]: The type of single component (example: "3F32").
							const auto & attribBaseFormatStr = regexMatch[4].str();

							// [5/Optional]: Per-component padding specification (example: "+4"). Not used in parsing.
							// [6/Optional]: Value of the per-component padding (example: "4").
							const auto & dataPaddingValueStr = regexMatch[6].str();

							// [7/Optional]: Array suffix for multi-slot/-component attribs (example: "[4]"). Not used in parsing.
							// [8/Optional]: Size of the array for multi-slot/-component attributes (example: "4").
							const auto & semanticGroupSizeValueStr = regexMatch[8].str();

							if( attributeSlotStr.empty() || attribSemanticsStr.empty() || attribBaseFormatStr.empty() )
							{
								continue;
							}

							const auto attributeSlot =
								cppx::from_string_or_default<uint8>( attributeSlotStr, GCI::kIAVertexAttributeSlotUndefined );
							
							const auto attribRelativeOffset =
								cppx::from_string_or_default<uint32>( attribRelativeOffsetStr, GCI::kIAVertexAttributeOffsetInvalid );
							
							const auto semanticGroupSize =
								cppx::from_string_or_default<uint8>( semanticGroupSizeValueStr, 1u );
							
							const auto dataPadding =
								cppx::from_string_or_default<uint8>( dataPaddingValueStr, 0u );

							if( !GCU::IAIsAttributeSemanticGroupValid( attributeSlot, semanticGroupSize ) || ( attribRelativeOffset == GCI::kIAVertexAttributeOffsetInvalid ) )
							{
								continue;
							}

							auto & attributeDefinition = vertexAttribsDefinitions.emplace_back();
							attributeDefinition.attributeSlot = attributeSlot;
							attributeDefinition.vertexStreamSlot = streamIndex;
							attributeDefinition.baseDataFormat = ParseAttributeFormatSerialString( attribBaseFormatStr );
							attributeDefinition.semanticGroupSize = semanticGroupSize;
							attributeDefinition.dataPadding = dataPadding;
							attributeDefinition.vertexStreamRelativeOffset = attribRelativeOffset;
							attributeDefinition.dataRate = streamDataRate;

							if( const auto resolvedSemanticName = ResolveShaderSemanticShortName( attribSemanticsStr ) )
							{
								attributeDefinition.semantics.semanticName = resolvedSemanticName;
							}
							else
							{
								attributeDefinition.semantics.semanticName = attribSemanticsStr;
							}

							attributeDefinition.semantics.semanticFlags = GetSemanticFlagsFromAttributeName( attributeDefinition.semantics.semanticName );
						}
					}

					vsAttributesStrings.clear();
				}
			}

			return vertexAttribsDefinitions;
		}

	}

} // namespace Ic3
