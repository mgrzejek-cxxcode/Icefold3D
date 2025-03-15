
#include "VertexAttributeKey.h"

namespace Ic3
{

	namespace VertexFormat
	{

		VertexInputAttributeDefinition MakeAttributeDefinition(
				uint32 pVertexStreamSlot,
				VertexAttributeKey pAttributeKey,
				uint32 pDataPadding,
				uint32 pVertexStreamRelativeOffset )
		{
			VertexInputAttributeDefinition attributeDefinition{};
			attributeDefinition.attributeSlot = pAttributeKey.GetBaseSlot();
			attributeDefinition.semanticGroupSize = pAttributeKey.GetSemanticGroupSize();
			attributeDefinition.baseDataFormat = pAttributeKey.GetBaseDataFormat();
			attributeDefinition.dataPadding = cppx::numeric_cast< decltype( attributeDefinition.dataPadding ) >( pDataPadding );
			attributeDefinition.semantics.semanticName = GetSemanticNameFromAttributeFlags( pAttributeKey.GetSemanticFlags() );
			attributeDefinition.semantics.semanticFlags = pAttributeKey.GetSemanticFlags();
			attributeDefinition.dataRate = pAttributeKey.GetDataRate();
			attributeDefinition.vertexStreamSlot = cppx::numeric_cast< decltype( attributeDefinition.vertexStreamSlot ) >( pVertexStreamSlot );
			attributeDefinition.vertexStreamRelativeOffset = pVertexStreamRelativeOffset;

			return attributeDefinition;
		}

		GCI::IAVertexAttributeDesc GCIMakeVertexAttributeDesc(
				VertexAttributeKey pAttributeKey,
				uint32 pVertexStreamSlot,
				uint32 pVertexStreamRelativeOffset )
		{
			GCI::IAVertexAttributeDesc gciAttributeDesc{};
			gciAttributeDesc.attribInfo.attributeSlot = pAttributeKey.GetBaseSlot();
			gciAttributeDesc.attribInfo.dataFormat = pAttributeKey.GetBaseDataFormat();
			gciAttributeDesc.attribInfo.dataRate = pAttributeKey.GetDataRate();
			gciAttributeDesc.semantics = static_cast< GCI::IAVertexAttributeSemantics >( GetShaderSemanticsForAttributeKey( pAttributeKey ) );
			gciAttributeDesc.vertexStreamBinding.streamSlot = cppx::numeric_cast< decltype( gciAttributeDesc.vertexStreamBinding.streamSlot ) >( pVertexStreamSlot );
			gciAttributeDesc.vertexStreamBinding.streamRelativeOffset = pVertexStreamRelativeOffset;

			return gciAttributeDesc;
		}

	}

} // namespace Ic3
