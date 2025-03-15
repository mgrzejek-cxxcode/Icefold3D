
#include "VertexAttributeShaderSemantics.h"
#include <cppx/stdHelperAlgo.h>
#include <regex>

namespace Ic3
{

	VertexAttributeShaderSemantics::VertexAttributeShaderSemantics(
			cppx::bitmask<EVertexAttributeSemanticFlags> pSemanticFlags,
			uint32 pSemanticIndex )
	: IAVertexAttributeSemantics{
		VertexFormat::GetSemanticNameFromAttributeFlags( pSemanticFlags ),
		cppx::numeric_cast< uint8 >( pSemanticIndex ) }
	, semanticFlags( pSemanticFlags )
	{}

	VertexAttributeShaderSemantics::VertexAttributeShaderSemantics(
			cppx::immutable_string pSemanticName,
			uint32 pSemanticIndex,
			cppx::bitmask<EVertexAttributeSemanticFlags> pSemanticFlags )
	: IAVertexAttributeSemantics{
		pSemanticName,
		cppx::numeric_cast<uint8>( pSemanticIndex ) }
	, semanticFlags( pSemanticFlags ? pSemanticFlags : VertexFormat::GetSemanticFlagsFromAttributeName( semanticName.str_view() ) )
	{}


	namespace VertexFormat
	{

		using AttribSemanticNameToFlagsMap = std::unordered_map<cppx::string_view, uint32, cppx::hash_std_proxy<cppx::immutable_string>>;
		using AttribSemanticFlagsToNameMap = std::unordered_map<uint32, cppx::string_view>;

		static const AttribSemanticNameToFlagsMap kAttributeSemanticNameToFlagsMap{
			{ "POSITION"        , eVertexAttributeSemanticFlagPositionBit         },
			{ "NORMAL"          , eVertexAttributeSemanticFlagNormalBit           },
			{ "TANGENT"         , eVertexAttributeSemanticFlagTangentBit          },
			{ "BITANGENT"       , eVertexAttributeSemanticFlagBiTangentBit        },
			{ "COLOR"           , eVertexAttributeSemanticFlagFixedColorBit       },
			{ "TEXCOORD0"       , eVertexAttributeSemanticFlagTexCoord0Bit        },
			{ "TEXCOORD1"       , eVertexAttributeSemanticFlagTexCoord1Bit        },
			{ "TEXCOORD2"       , eVertexAttributeSemanticFlagTexCoord2Bit        },
			{ "TEXCOORD3"       , eVertexAttributeSemanticFlagTexCoord3Bit        },
			{ "TEXCOORD4"       , eVertexAttributeSemanticFlagTexCoord4Bit        },
			{ "TEXCOORD5"       , eVertexAttributeSemanticFlagTexCoord5Bit        },
			{ "TEXCOORDP01"     , eVertexAttributeSemanticMaskTexCoordP01         },
			{ "TEXCOORDP23"     , eVertexAttributeSemanticMaskTexCoordP23         },
			{ "BLEND_INDICES"   , eVertexAttributeSemanticFlagBlendIndicesBit     },
			{ "BLEND_WEIGHTS"   , eVertexAttributeSemanticFlagBlendWeightsBit     },
			{ "INSTANCE_MATRIX" , eVertexAttributeSemanticFlagInstanceMatrixBit   },
			{ "INSTANCE_UDATA"  , eVertexAttributeSemanticFlagInstanceUserDataBit },
		};

		static const AttribSemanticFlagsToNameMap kAttributeSemanticFlagsToNameMap{
			{ eVertexAttributeSemanticFlagPositionBit         , "POSITION"        },
			{ eVertexAttributeSemanticFlagNormalBit           , "NORMAL"          },
			{ eVertexAttributeSemanticFlagTangentBit          , "TANGENT"         },
			{ eVertexAttributeSemanticFlagBiTangentBit        , "BITANGENT"       },
			{ eVertexAttributeSemanticFlagFixedColorBit       , "COLOR"           },
			{ eVertexAttributeSemanticFlagTexCoord0Bit        , "TEXCOORD0"       },
			{ eVertexAttributeSemanticFlagTexCoord1Bit        , "TEXCOORD1"       },
			{ eVertexAttributeSemanticFlagTexCoord2Bit        , "TEXCOORD2"       },
			{ eVertexAttributeSemanticFlagTexCoord3Bit        , "TEXCOORD3"       },
			{ eVertexAttributeSemanticFlagTexCoord4Bit        , "TEXCOORD4"       },
			{ eVertexAttributeSemanticFlagTexCoord5Bit        , "TEXCOORD5"       },
			{ eVertexAttributeSemanticMaskTexCoordP01         , "TEXCOORDP01"     },
			{ eVertexAttributeSemanticMaskTexCoordP23         , "TEXCOORDP23"     },
			{ eVertexAttributeSemanticFlagBlendIndicesBit     , "BLEND_INDICES"   },
			{ eVertexAttributeSemanticFlagBlendWeightsBit     , "BLEND_WEIGHTS"   },
			{ eVertexAttributeSemanticFlagInstanceMatrixBit   , "INSTANCE_MATRIX" },
			{ eVertexAttributeSemanticFlagInstanceUserDataBit , "INSTANCE_UDATA"  },
		};

		cppx::string_view GetSemanticNameFromAttributeID( EVertexAttributeSemanticID pAttributeSemanticID )
		{
			return GetSemanticNameFromAttributeFlags( static_cast<EVertexAttributeSemanticFlags>( pAttributeSemanticID ) );
		}

		cppx::string_view GetSemanticNameFromAttributeFlags( cppx::bitmask<EVertexAttributeSemanticFlags> pAttributeSemanticFlags )
		{
			return cppx::get_map_value_ref_or_default( kAttributeSemanticFlagsToNameMap, pAttributeSemanticFlags, cppx::string_view{} );
		}

		EVertexAttributeSemanticID GetSemanticIDFromAttributeName( const cppx::string_view & pAttributeSemanticName )
		{
			const auto semanticFlags = GetSemanticFlagsFromAttributeName( pAttributeSemanticName );
			return semanticFlags ? static_cast<EVertexAttributeSemanticID>( semanticFlags.get() ) : EVertexAttributeSemanticID::Unknown;
		}

		cppx::bitmask<EVertexAttributeSemanticFlags> GetSemanticFlagsFromAttributeName( const cppx::string_view & pAttributeSemanticName )
		{
			// Get flags from the map or the default (eVertexAttributeSemanticFlagNone) if the name is not found there.
			const auto & semanticFlags = cppx::get_map_value_ref_or_default(
					kAttributeSemanticNameToFlagsMap,
					pAttributeSemanticName,
					eVertexAttributeSemanticFlagNone );

			if( semanticFlags != eVertexAttributeSemanticFlagNone )
			{
				return semanticFlags;
			}
			else
			{
				// If the mask yields eVertexAttributeSemanticFlagNone, this means the name was not found. This means that either:
				// - name was empty: return eVertexAttributeSemanticFlagNone
				// - name is a name of a custom, non-system attribute: return eVertexAttributeSemanticFlagCustomAttributeBit;
				return pAttributeSemanticName.empty() ? eVertexAttributeSemanticFlagNone : eVertexAttributeSemanticFlagCustomAttributeBit;
			}
		}

		VertexAttributeShaderSemantics GetSemanticsFromAttributeID(
				EVertexAttributeSemanticID pAttributeSemanticID,
				uint32 pSemanticIndex )
		{
			return VertexAttributeShaderSemantics(
				VertexFormat::GetSemanticNameFromAttributeID( pAttributeSemanticID ),
				pSemanticIndex,
				static_cast<EVertexAttributeSemanticFlags>( pAttributeSemanticID ) & eVertexAttributeSemanticMaskAll );
		}

		VertexAttributeShaderSemantics GetSemanticsFromAttributeFlags(
				cppx::bitmask<EVertexAttributeSemanticFlags> pAttributeSemanticFlags,
				uint32 pSemanticIndex )
		{
			return VertexAttributeShaderSemantics(
				VertexFormat::GetSemanticNameFromAttributeFlags( pAttributeSemanticFlags ),
					pSemanticIndex,
					pAttributeSemanticFlags & eVertexAttributeSemanticMaskAll );
		}

		GCI::IAVertexAttributeSemantics ProcessIndexedAttributeSemantics( const cppx::immutable_string & pSemanticName, native_uint pSemanticIndex )
		{
			static const auto reStrDX11AttrNameSemanticsCombined = R"(^[A-Za-z]+([0-9]*)$)";
			static const std::regex reDX11AttrNameSemanticsCombined{ reStrDX11AttrNameSemanticsCombined };

			static const auto reStrDX11AttrNameSemanticIndexOnly = R"(([0-9]*)$)";
			static const std::regex reDX11AttrNameSemanticIndexOnly{ reStrDX11AttrNameSemanticIndexOnly };

			GCI::IAVertexAttributeSemantics resultSemantics{};
			resultSemantics.semanticName = pSemanticName;
			resultSemantics.semanticIndex = pSemanticIndex;

			std::smatch regexMatch;
			if( std::regex_search( resultSemantics.semanticName.str(), regexMatch, reDX11AttrNameSemanticsCombined ) )
			{
				const auto attributeSemanticIndexStr = regexMatch[1].str();
				if( !attributeSemanticIndexStr.empty() )
				{
					const auto attributeSemanticIndex = cppx::from_string<uint32>( attributeSemanticIndexStr );
					Ic3DebugAssert( ( resultSemantics.semanticIndex == 0 ) || ( attributeSemanticIndex == resultSemantics.semanticIndex ) );

					if( resultSemantics.semanticIndex == 0 )
					{
						resultSemantics.semanticIndex = cppx::numeric_cast< uint8 >( attributeSemanticIndex );
					}

					resultSemantics.semanticName = std::regex_replace( resultSemantics.semanticName.str(), reDX11AttrNameSemanticIndexOnly, "" );
				}
			}

			return resultSemantics;
		}

		GCI::IAVertexAttributeSemantics ProcessIndexedAttributeSemantics( const GCI::IAVertexAttributeSemantics & pInputSemantics )
		{
			return ProcessIndexedAttributeSemantics( pInputSemantics.semanticName, pInputSemantics.semanticIndex );
		}

	}

}
