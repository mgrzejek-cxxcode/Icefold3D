
#include "ShaderInputSemantics.h"
#include "VertexAttributeCommonDefs.h"
#include <cppx/stdHelperAlgo.h>

namespace Ic3
{

	bool ShaderSemantics::resolve() noexcept
	{
		Ic3DebugInterrupt();

		/*
		if( smtID == EShaderInputSemanticID::Custom )
		{
			// Custom semantics, so a non-empty semantic name is required.
			if( smtName.empty() )
			{
				return false;
			}
		}
		else if( CxDef::isStandardShaderInputSemanticID( smtID )  )
		{
			// One of the standard semantic IDs, semantic name resolved from the ID.
			smtName = GCU::getShaderInputSemanticNameFromID( smtID );
		}
		else
		{
			if( smtName.empty() )
			{
				return false;
			}

			smtID = GCU::getShaderInputSemanticIDFromName( smtName );
		}
		 */

		return true;
	}

	ShaderSemantics ShaderSemantics::resolveSemantics( const ShaderSemantics & pSemantics )
	{
		ShaderSemantics resolvedSemantics{ pSemantics };
		if( !resolvedSemantics.resolve() )
		{
			resolvedSemantics.clear();
		}
		return resolvedSemantics;
	}

	//ShaderSemantics ShaderSemantics::fromVertexAttributeKey( VertexAttributeKey pAttributeKey, uint32 pSmtIndex )
	//{
	//	const auto smtID = CxDef::getVertexAttributeKeySystemSemanticFlags( pAttributeKey );
	//	auto smtName = GCU::getShaderInputSemanticNameFromID( smtID );
	//	return ShaderSemantics{ smtID, std::move( smtName ), pSmtIndex };
	//}

	namespace GCU
	{

		using AttribSemanticNameHash = cppx::hash_object<cppx::hash_algo::sdbm>;
		using AttribSemanticNameToFlagsMap = std::unordered_map<AttribSemanticNameHash, uint32>;
		using AttribSemanticFlagsToNameMap = std::unordered_map<uint32, std::string_view>;

		AttribSemanticNameHash computeAttribSemanticNameHash( const std::string_view & pSemanticName )
		{
			return cppx::hash_compute<cppx::hash_algo::sdbm>( pSemanticName );
		}

		AttribSemanticNameToFlagsMap buildAttribSemanticNameToFlagsMap()
		{
			AttribSemanticNameToFlagsMap resultMap{};

			resultMap.insert( { computeAttribSemanticNameHash( "POSITION"   )      , eSystemAttributeSemanticFlagPositionBit         } );
			resultMap.insert( { computeAttribSemanticNameHash( "NORMAL"     )      , eSystemAttributeSemanticFlagNormalBit           } );
			resultMap.insert( { computeAttribSemanticNameHash( "TANGENT"    )      , eSystemAttributeSemanticFlagTangentBit          } );
			resultMap.insert( { computeAttribSemanticNameHash( "BITANGENT"  )      , eSystemAttributeSemanticFlagBiTangentBit        } );
			resultMap.insert( { computeAttribSemanticNameHash( "COLOR"      )      , eSystemAttributeSemanticFlagFixedColorBit       } );
			resultMap.insert( { computeAttribSemanticNameHash( "TEXCOORD0"  )      , eSystemAttributeSemanticFlagTexCoord0Bit        } );
			resultMap.insert( { computeAttribSemanticNameHash( "TEXCOORD1"  )      , eSystemAttributeSemanticFlagTexCoord1Bit        } );
			resultMap.insert( { computeAttribSemanticNameHash( "TEXCOORD2"  )      , eSystemAttributeSemanticFlagTexCoord2Bit        } );
			resultMap.insert( { computeAttribSemanticNameHash( "TEXCOORD3"  )      , eSystemAttributeSemanticFlagTexCoord3Bit        } );
			resultMap.insert( { computeAttribSemanticNameHash( "TEXCOORD4"  )      , eSystemAttributeSemanticFlagTexCoord4Bit        } );
			resultMap.insert( { computeAttribSemanticNameHash( "TEXCOORD5"  )      , eSystemAttributeSemanticFlagTexCoord5Bit        } );
			resultMap.insert( { computeAttribSemanticNameHash( "TEXCOORD6"  )      , eSystemAttributeSemanticFlagTexCoord6Bit        } );
			resultMap.insert( { computeAttribSemanticNameHash( "TEXCOORD7"  )      , eSystemAttributeSemanticFlagTexCoord7Bit        } );
			resultMap.insert( { computeAttribSemanticNameHash( "TEXCOORD01" )      , eSystemAttributeSemanticMaskTexCoord01Packed    } );
			resultMap.insert( { computeAttribSemanticNameHash( "TEXCOORD23" )      , eSystemAttributeSemanticMaskTexCoord23Packed    } );
			resultMap.insert( { computeAttribSemanticNameHash( "BLEND_INDICES"   ) , eSystemAttributeSemanticFlagBlendIndicesBit     } );
			resultMap.insert( { computeAttribSemanticNameHash( "BLEND_WEIGHTS"   ) , eSystemAttributeSemanticFlagBlendWeightsBit     } );
			resultMap.insert( { computeAttribSemanticNameHash( "INSTANCE_MATRIX" ) , eSystemAttributeSemanticFlagInstanceMatrixBit   } );
			resultMap.insert( { computeAttribSemanticNameHash( "INSTANCE_UDATA"  ) , eSystemAttributeSemanticFlagInstanceUserDataBit } );

			return resultMap;
		}

		AttribSemanticFlagsToNameMap buildAttribSemanticFlagsToNameMap()
		{
			AttribSemanticFlagsToNameMap resultMap{};

			resultMap.insert( { eSystemAttributeSemanticFlagPositionBit         , "POSITION"        } );
			resultMap.insert( { eSystemAttributeSemanticFlagNormalBit           , "NORMAL"          } );
			resultMap.insert( { eSystemAttributeSemanticFlagTangentBit          , "TANGENT"         } );
			resultMap.insert( { eSystemAttributeSemanticFlagBiTangentBit        , "BITANGENT"       } );
			resultMap.insert( { eSystemAttributeSemanticFlagFixedColorBit       , "COLOR"           } );
			resultMap.insert( { eSystemAttributeSemanticFlagTexCoord0Bit        , "TEXCOORD0"       } );
			resultMap.insert( { eSystemAttributeSemanticFlagTexCoord1Bit        , "TEXCOORD1"       } );
			resultMap.insert( { eSystemAttributeSemanticFlagTexCoord2Bit        , "TEXCOORD2"       } );
			resultMap.insert( { eSystemAttributeSemanticFlagTexCoord3Bit        , "TEXCOORD3"       } );
			resultMap.insert( { eSystemAttributeSemanticFlagTexCoord4Bit        , "TEXCOORD4"       } );
			resultMap.insert( { eSystemAttributeSemanticFlagTexCoord5Bit        , "TEXCOORD5"       } );
			resultMap.insert( { eSystemAttributeSemanticFlagTexCoord6Bit        , "TEXCOORD6"       } );
			resultMap.insert( { eSystemAttributeSemanticFlagTexCoord7Bit        , "TEXCOORD7"       } );
			resultMap.insert( { eSystemAttributeSemanticMaskTexCoord01Packed    , "TEXCOORD01"      } );
			resultMap.insert( { eSystemAttributeSemanticMaskTexCoord23Packed    , "TEXCOORD23"      } );
			resultMap.insert( { eSystemAttributeSemanticFlagBlendIndicesBit     , "BLEND_INDICES"   } );
			resultMap.insert( { eSystemAttributeSemanticFlagBlendWeightsBit     , "BLEND_WEIGHTS"   } );
			resultMap.insert( { eSystemAttributeSemanticFlagInstanceMatrixBit   , "INSTANCE_MATRIX" } );
			resultMap.insert( { eSystemAttributeSemanticFlagInstanceUserDataBit , "INSTANCE_UDATA"  } );

			return resultMap;
		}

		cppx::bitmask<ESystemAttributeSemanticFlags> getAttributeSystemSemanticFlagsFromName( const cppx::string_view & pSemanticName )
		{
			static const auto attribSemanticNameToFlagsMap = buildAttribSemanticNameToFlagsMap();
			const auto semanticNameHash = computeAttribSemanticNameHash( pSemanticName );
			return cppx::get_map_value_ref_or_default( attribSemanticNameToFlagsMap, semanticNameHash, eSystemAttributeSemanticFlagCustomAttributeBit );
		}

		cppx::string_view getStandardSemanticNameFromSystemFlags( cppx::bitmask<ESystemAttributeSemanticFlags> pSystemSemanticFlags )
		{
			static const auto attribSemanticFlagsToNameMap = buildAttribSemanticFlagsToNameMap();
			return cppx::get_map_value_ref_or_default( attribSemanticFlagsToNameMap, pSystemSemanticFlags, "" );
		}

	}

}
