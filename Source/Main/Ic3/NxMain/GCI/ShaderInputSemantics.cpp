
#include "ShaderInputSemantics.h"
#include "VertexAttributeCommonDefs.h"
#include <cppx/stdHelperAlgo.h>

namespace Ic3
{

	bool ShaderSemantics::Resolve() noexcept
	{
		if( systemSemanticFlags.is_set( eSystemAttributeSemanticFlagCustomAttributeBit ) )
		{
			// Custom semantics, so a non-empty semantic name is required.
			if( semanticName.empty() )
			{
				return false;
			}
		}
		else if( GCU::IsStandardShaderInputAttribute( systemSemanticFlags )  )
		{
			// One of the standard semantic IDs, semantic name resolved from the ID.
			semanticName = GCU::GetStandardSemanticNameFromSystemFlags( systemSemanticFlags );
		}
		else
		{
			if( semanticName.empty() )
			{
				return false;
			}

			systemSemanticFlags = GCU::GetAttributeSystemSemanticFlagsFromName( semanticName );
		}

		return !semanticName.empty() && !systemSemanticFlags.empty();
	}

	ShaderSemantics ShaderSemantics::ResolveSemantics( const ShaderSemantics & pSemantics )
	{
		ShaderSemantics resolvedSemantics{ pSemantics };
		if( !resolvedSemantics.Resolve() )
		{
			resolvedSemantics.Clear();
		}
		return resolvedSemantics;
	}

	ShaderSemantics ShaderSemantics::FromVertexAttributeKey(
			vertex_attribute_key_value_t pAttributeKeyValue,
			uint32 pSemanticIndex )
	{
		const auto attributeKey = VertexAttributeKey( pAttributeKeyValue );
		auto semanticName = GCU::GetStandardSemanticNameFromSystemFlags( attributeKey.GetFlags() );
		return ShaderSemantics{ std::move( semanticName ), pSemanticIndex, attributeKey.GetFlags() };
	}

	namespace GCU
	{

		using AttribSemanticNameHash = cppx::hash_object<cppx::hash_algo::sdbm>;
		using AttribSemanticNameToFlagsMap = std::unordered_map<AttribSemanticNameHash, uint32>;
		using AttribSemanticFlagsToNameMap = std::unordered_map<uint32, std::string_view>;

		AttribSemanticNameHash ComputeAttribSemanticNameHash( const std::string_view & pSemanticName )
		{
			return cppx::hash_compute<cppx::hash_algo::sdbm>( pSemanticName );
		}

		AttribSemanticNameToFlagsMap BuildAttribSemanticNameToFlagsMap()
		{
			AttribSemanticNameToFlagsMap resultMap{};

			resultMap.insert( { ComputeAttribSemanticNameHash( "POSITION"   )      , eSystemAttributeSemanticFlagPositionBit         } );
			resultMap.insert( { ComputeAttribSemanticNameHash( "NORMAL"     )      , eSystemAttributeSemanticFlagNormalBit           } );
			resultMap.insert( { ComputeAttribSemanticNameHash( "TANGENT"    )      , eSystemAttributeSemanticFlagTangentBit          } );
			resultMap.insert( { ComputeAttribSemanticNameHash( "BITANGENT"  )      , eSystemAttributeSemanticFlagBiTangentBit        } );
			resultMap.insert( { ComputeAttribSemanticNameHash( "COLOR"      )      , eSystemAttributeSemanticFlagFixedColorBit       } );
			resultMap.insert( { ComputeAttribSemanticNameHash( "TEXCOORD0"  )      , eSystemAttributeSemanticFlagTexCoord0Bit        } );
			resultMap.insert( { ComputeAttribSemanticNameHash( "TEXCOORD1"  )      , eSystemAttributeSemanticFlagTexCoord1Bit        } );
			resultMap.insert( { ComputeAttribSemanticNameHash( "TEXCOORD2"  )      , eSystemAttributeSemanticFlagTexCoord2Bit        } );
			resultMap.insert( { ComputeAttribSemanticNameHash( "TEXCOORD3"  )      , eSystemAttributeSemanticFlagTexCoord3Bit        } );
			resultMap.insert( { ComputeAttribSemanticNameHash( "TEXCOORD4"  )      , eSystemAttributeSemanticFlagTexCoord4Bit        } );
			resultMap.insert( { ComputeAttribSemanticNameHash( "TEXCOORD5"  )      , eSystemAttributeSemanticFlagTexCoord5Bit        } );
			resultMap.insert( { ComputeAttribSemanticNameHash( "TEXCOORD6"  )      , eSystemAttributeSemanticFlagTexCoord6Bit        } );
			resultMap.insert( { ComputeAttribSemanticNameHash( "TEXCOORD7"  )      , eSystemAttributeSemanticFlagTexCoord7Bit        } );
			resultMap.insert( { ComputeAttribSemanticNameHash( "TEXCOORD01" )      , eSystemAttributeSemanticMaskTexCoord01Packed    } );
			resultMap.insert( { ComputeAttribSemanticNameHash( "TEXCOORD23" )      , eSystemAttributeSemanticMaskTexCoord23Packed    } );
			resultMap.insert( { ComputeAttribSemanticNameHash( "BLEND_INDICES"   ) , eSystemAttributeSemanticFlagBlendIndicesBit     } );
			resultMap.insert( { ComputeAttribSemanticNameHash( "BLEND_WEIGHTS"   ) , eSystemAttributeSemanticFlagBlendWeightsBit     } );
			resultMap.insert( { ComputeAttribSemanticNameHash( "INSTANCE_MATRIX" ) , eSystemAttributeSemanticFlagInstanceMatrixBit   } );
			resultMap.insert( { ComputeAttribSemanticNameHash( "INSTANCE_UDATA"  ) , eSystemAttributeSemanticFlagInstanceUserDataBit } );

			return resultMap;
		}

		AttribSemanticFlagsToNameMap BuildAttribSemanticFlagsToNameMap()
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

		cppx::bitmask<ESystemAttributeSemanticFlags> GetAttributeSystemSemanticFlagsFromName( const cppx::string_view & pSemanticName )
		{
			static const auto attribSemanticNameToFlagsMap = BuildAttribSemanticNameToFlagsMap();

			const auto semanticNameHash = ComputeAttribSemanticNameHash( pSemanticName );

			return cppx::get_map_value_ref_or_default(
					attribSemanticNameToFlagsMap,
					semanticNameHash,
					eSystemAttributeSemanticFlagCustomAttributeBit );
		}

		cppx::string_view GetStandardSemanticNameFromSystemFlags( cppx::bitmask<ESystemAttributeSemanticFlags> pSystemSemanticFlags )
		{
			static const auto attribSemanticFlagsToNameMap = BuildAttribSemanticFlagsToNameMap();

			return cppx::get_map_value_ref_or_default(
					attribSemanticFlagsToNameMap,
					pSystemSemanticFlags,
					cppx::string_view{} );
		}

	}

}
