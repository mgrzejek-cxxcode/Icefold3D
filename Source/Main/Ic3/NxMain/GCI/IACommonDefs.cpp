
#include "IACommonDefs.h"
#include <cppx/stdHelperAlgo.h>

namespace Ic3
{

	namespace GCIUtils
	{

		using AttribSemanticNameToFlagsMap = std::unordered_map<cppx::string_view, uint32, cppx::hash_std_proxy<cppx::immutable_string>>;
		using AttribSemanticFlagsToNameMap = std::unordered_map<uint32, cppx::string_view>;

		static const AttribSemanticNameToFlagsMap kAttributeSemanticNameToFlagsMap {
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
			{ "TEXCOORDP01"      , eVertexAttributeSemanticMaskTexCoordP01        },
			{ "TEXCOORDP23"      , eVertexAttributeSemanticMaskTexCoordP23        },
			{ "BLEND_INDICES"   , eVertexAttributeSemanticFlagBlendIndicesBit     },
			{ "BLEND_WEIGHTS"   , eVertexAttributeSemanticFlagBlendWeightsBit     },
			{ "INSTANCE_MATRIX" , eVertexAttributeSemanticFlagInstanceMatrixBit   },
			{ "INSTANCE_UDATA"  , eVertexAttributeSemanticFlagInstanceUserDataBit },
		};

		AttribSemanticFlagsToNameMap kAttributeSemanticFlagsToNameMap {
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

		cppx::string_view GetSemanticNameFromAttributeID(
				EVertexAttributeSemanticID pAttributeSemanticID )
		{
			return GetSemanticNameFromAttributeFlags(
					static_cast<EVertexAttributeSemanticFlags>( pAttributeSemanticID ) );
		}

		cppx::string_view GetSemanticNameFromAttributeFlags(
				cppx::bitmask<EVertexAttributeSemanticFlags> pAttributeSemanticFlags )
		{
			return cppx::get_map_value_ref_or_default(
					kAttributeSemanticFlagsToNameMap,
					pAttributeSemanticFlags,
					cppx::string_view{} );
		}

		cppx::bitmask<EVertexAttributeSemanticFlags> GetSemanticFlagsFromAttributeName(
				const cppx::string_view & pAttributeSemanticName )
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

		EVertexAttributeSemanticID GetSemanticIDFromAttributeName(
				const cppx::string_view & pAttributeSemanticName )
		{
			const auto semanticFlags = GetSemanticFlagsFromAttributeName( pAttributeSemanticName );
			return semanticFlags ? static_cast<EVertexAttributeSemanticID>( semanticFlags.get() ) : EVertexAttributeSemanticID::Unknown;
		}

	}

} // namespace Ic3
