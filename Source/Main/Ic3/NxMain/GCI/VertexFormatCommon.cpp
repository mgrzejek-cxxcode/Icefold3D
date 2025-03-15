
#include "VertexFormatCombinedLayout.h"
#include <cppx/stdHelperAlgo.h>

namespace Ic3
{

	namespace VertexFormat
	{

		GCI::IAVertexAttributeLayoutDefinition GCICreateVertexAttributeLayoutDefinition(
				const cppx::array_view<VertexInputAttributeDefinition> & pVertexAttributeDefinitions )
		{
			GCI::IAVertexAttributeLayoutDefinition gciVertexAttributeLayoutDefinition{};
			if( !GCICreateVertexAttributeLayoutDefinition( pVertexAttributeDefinitions, gciVertexAttributeLayoutDefinition ) )
			{
				return {};
			}

			return gciVertexAttributeLayoutDefinition;
		}
		
		GCI::IAVertexAttributeLayoutDefinition GCICreateVertexAttributeLayoutDefinition(
				const cppx::array_view<GenericVertexInputAttribute> & pVertexAttributes )
		{
			GCI::IAVertexAttributeLayoutDefinition gciAttributeLayoutDefinition{};
			if( !GCICreateVertexAttributeLayoutDefinition( pVertexAttributes, gciAttributeLayoutDefinition ) )
			{
				return {};
			}

			return gciAttributeLayoutDefinition;
		}

		bool GCICreateVertexAttributeLayoutDefinition(
				const cppx::array_view<VertexInputAttributeDefinition> & pVertexAttributeDefinitions,
				GCI::IAVertexAttributeLayoutDefinition & pGCIVertexAttributeLayoutDefinition )
		{
			if( pVertexAttributeDefinitions.size() == 0 )
			{
				return {};
			}

			const auto combinedLayoutSize = ComputeCombinedLayoutSize( pVertexAttributeDefinitions );
			if( !combinedLayoutSize )
			{
				Ic3DebugOutputNxError( "DetermineCombinedLayoutSize has failed" );
				return {};
			}

			VertexFormatCombinedLayoutDynamic tmpVertexFormatCombinedLayout{
					combinedLayoutSize.activeAttributesNum,
					combinedLayoutSize.activeStreamsNum
			};

			if( !CreateVertexFormatCombinedLayout( pVertexAttributeDefinitions, tmpVertexFormatCombinedLayout ) )
			{
				Ic3DebugOutputNxError( "Failed to generate intermediate VertexFormatCombinedLayout" );
				return {};
			}

			return GCICreateVertexAttributeLayoutDefinition(
				tmpVertexFormatCombinedLayout.GetActiveAttributesView(),
				pGCIVertexAttributeLayoutDefinition );
		}
		
		bool GCICreateVertexAttributeLayoutDefinition(
				const cppx::array_view<GenericVertexInputAttribute> & pVertexAttributes,
				GCI::IAVertexAttributeLayoutDefinition & pOutGCIAttributeLayoutDefinition )
		{
			for( auto & genericVertexAttribute : pVertexAttributes )
			{
				auto & gciAttributeDesc = pOutGCIAttributeLayoutDefinition.attributeArray[genericVertexAttribute.attributeSlot];
				gciAttributeDesc.attribInfo.attributeSlot = genericVertexAttribute.attributeSlot;
				gciAttributeDesc.attribInfo.dataFormat = genericVertexAttribute.dataFormat;
				gciAttributeDesc.attribInfo.dataRate = genericVertexAttribute.dataRate;
				gciAttributeDesc.semantics = ProcessIndexedAttributeSemantics( genericVertexAttribute.semanticName, genericVertexAttribute.semanticIndex );
				gciAttributeDesc.vertexStreamBinding.streamSlot = genericVertexAttribute.vertexStreamSlot;
				gciAttributeDesc.vertexStreamBinding.streamRelativeOffset = genericVertexAttribute.vertexStreamRelativeOffset;

				pOutGCIAttributeLayoutDefinition.activeAttributesMask.set( GCI::CXU::IAMakeVertexAttributeFlag( gciAttributeDesc.attribInfo.attributeSlot ) );
			}

			return true;
		}

	}

} // namespace Ic3
