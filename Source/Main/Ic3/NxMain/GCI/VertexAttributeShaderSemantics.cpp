
#include "VertexAttributeShaderSemantics.h"
#include "VertexAttributeKey.h"
#include <cppx/stdHelperAlgo.h>

namespace Ic3
{

	namespace GCIUtils
	{

		VertexAttributeShaderSemantics GetSemanticsFromAttributeID(
				EVertexAttributeSemanticID pAttributeSemanticID,
				uint32 pSemanticIndex )
		{
			return VertexAttributeShaderSemantics(
				GCIUtils::GetSemanticNameFromAttributeID( pAttributeSemanticID ),
				pSemanticIndex,
				static_cast<EVertexAttributeSemanticFlags>( pAttributeSemanticID ) & eVertexAttributeSemanticMaskAll );
		}

		VertexAttributeShaderSemantics GetSemanticsFromAttributeFlags(
				cppx::bitmask<EVertexAttributeSemanticFlags> pAttributeSemanticFlags,
				uint32 pSemanticIndex )
		{
			return VertexAttributeShaderSemantics(
					GCIUtils::GetSemanticNameFromAttributeFlags( pAttributeSemanticFlags ),
					pSemanticIndex,
					pAttributeSemanticFlags & eVertexAttributeSemanticMaskAll );
		}

	}

}
