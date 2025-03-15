
#include "VertexFormatCombinedLayout.h"
#include <cppx/stdHelperAlgo.h>

namespace Ic3
{

	namespace VertexFormat
	{

		VertexInputStream * AppendVertexInputStreamInternal(
				VertexFormatCombinedLayoutBase & pVertexFormatCombinedLayout,
				native_uint pVertexStreamSlot,
				GCI::EIAVertexAttributeDataRate pStreamDataRate ) noexcept;

		GenericVertexInputAttribute * AppendVertexFormatAttributeInternal(
				VertexFormatCombinedLayoutBase & pVertexFormatCombinedLayout,
				const VertexInputAttributeDefinition & pAttributeDefinition ) noexcept;

		void UpdateVertexStreamWithAttributeData(
				VertexInputStream & pVertexInputStream,
				const GenericVertexInputAttribute & pVertexInputAttribute );
		

		VertexFormatCombinedLayoutSize ComputeCombinedLayoutSize(
				const cppx::array_view<VertexInputAttributeDefinition> & pVertexAttributeDefinitions )
		{
			VertexFormatCombinedLayoutSize layoutSize{};

			bool activeAttributesFlagArray[GCM::kIAMaxVertexAttributesNum]{};
			bool activeStreamsFlagArray[GCM::kIAMaxDataStreamVertexBuffersNum]{};

			for( const auto & attributeDefinition : pVertexAttributeDefinitions )
			{
				if( attributeDefinition.IsValid() )
				{
					if( !activeAttributesFlagArray[attributeDefinition.attributeSlot] )
					{
						activeAttributesFlagArray[attributeDefinition.attributeSlot] = true;
						layoutSize.activeAttributesNum += attributeDefinition.semanticGroupSize;

						if( !activeStreamsFlagArray[attributeDefinition.vertexStreamSlot] )
						{
							activeStreamsFlagArray[attributeDefinition.vertexStreamSlot] = true;
							layoutSize.activeStreamsNum += 1;
						}
					}
				}
			}

			return layoutSize;
		}

		bool CreateVertexFormatCombinedLayout(
				const cppx::array_view<VertexInputAttributeDefinition> & pAttributeDefinitions,
				VertexFormatCombinedLayoutBase & pOutVertexFormatCombinedLayoutBase ) noexcept
		{
			if( !ValidateVertexInputAttributeDefinitions( pAttributeDefinitions ) )
			{
				return false;
			}

			pOutVertexFormatCombinedLayoutBase.activeAttributesMask.clear();
			pOutVertexFormatCombinedLayoutBase.activeStreamsMask.clear();

			for( auto & attributeDefinition : pAttributeDefinitions )
			{
				auto * vertexInputStream = AppendVertexInputStreamInternal( pOutVertexFormatCombinedLayoutBase, attributeDefinition.vertexStreamSlot, attributeDefinition.dataRate );
				if( !vertexInputStream )
				{
					return false;
				}

				if( !AdjustAttributeDefinition( pOutVertexFormatCombinedLayoutBase, attributeDefinition ) )
				{
					return false;
				}

				auto * genericVertexAttribute = AppendVertexFormatAttributeInternal( pOutVertexFormatCombinedLayoutBase, attributeDefinition );
				if( !genericVertexAttribute )
				{
					return false;
				}

				UpdateVertexStreamWithAttributeData( *vertexInputStream, *genericVertexAttribute );
			}

			return true;
		}

		bool ValidateAttributeDefinition( const VertexInputAttributeDefinition & pAttributeDefinition ) noexcept
		{
			// Check if this attribute definition is valid. IsValid() checks
			// the attribute index, Stream index, semantics, format, etc.
			if( !pAttributeDefinition.IsValid() )
			{
				Ic3DebugOutputNxError(
					"Invalid attribute definition (attribute: %u, sgs: %u, Stream: %u, dataFormat: %s, semantics: %s). Validation failed.",
					pAttributeDefinition.attributeSlot,
					pAttributeDefinition.semanticGroupSize,
					pAttributeDefinition.vertexStreamSlot,
					GCI::QueryEnumTypeInfo<GCI::EVertexAttribFormat>().GetConstantName( pAttributeDefinition.baseDataFormat ).c_str(),
					pAttributeDefinition.semantics.semanticName.data() );
				return false;
			}

			return true;
		}

		bool ValidateVertexInputAttributeDefinitions(
			const cppx::array_view<VertexInputAttributeDefinition> & pAttributeDefinitions ) noexcept
		{
			// Array of bools indicating whether an attribute at index i is active or not.
			bool activeAttributesFlagArray[GCM::kIAMaxVertexAttributesNum]{};
			// Array of bools indicating whether a Stream at index i is active or not.
			bool activeStreamsFlagArray[GCM::kIAMaxDataStreamVertexBuffersNum]{};

			uint32 activeAttributesNum = 0;
			uint32 activeStreamsNum = 0;

			for( const auto & attributeDefinition : pAttributeDefinitions )
			{
				// Validate if this attribute definition is valid. IsValid() checks
				// validity of the attribute index, Stream index, semantics, format, etc.
				if( !ValidateAttributeDefinition( attributeDefinition ) )
				{
					return false;
				}

				// If the boolean for this index is set, it means that the current definition
				// is for an attribute index which has been already defined previously.
				if( activeAttributesFlagArray[attributeDefinition.attributeSlot] )
				{
					Ic3DebugOutputNxError(
						"Multiple attributes defined for IA slot %u. Validation failed",
						attributeDefinition.attributeSlot );
					return false;
				}

				activeAttributesFlagArray[attributeDefinition.attributeSlot] = true;
				activeAttributesNum += attributeDefinition.semanticGroupSize;

				if( !activeStreamsFlagArray[attributeDefinition.vertexStreamSlot] )
				{
					activeStreamsFlagArray[attributeDefinition.vertexStreamSlot] = true;
					activeStreamsNum += 1;
				}
			}

			return ( activeAttributesNum > 0 ) && ( activeStreamsNum > 0 );
		}

		bool CheckAttributeDefinitionCompatibility(
				const VertexFormatCombinedLayoutBase & pVertexFormatCombinedLayout,
				const VertexInputAttributeDefinition & pAttributeDefinition ) noexcept
		{
			if( !pAttributeDefinition.IsValid() )
			{
				return false;
			}

			if( !GCU::IAIsAttributeSemanticGroupValid( pAttributeDefinition.attributeSlot, pAttributeDefinition.semanticGroupSize ) )
			{
				return false;
			}

			if( !CheckAttributeArraySpace( pVertexFormatCombinedLayout, pAttributeDefinition.attributeSlot, pAttributeDefinition.semanticGroupSize ) )
			{
				Ic3DebugOutputNxError( "" );
				return false;
			}

			const auto * attributeStream = pVertexFormatCombinedLayout.GetStreamAtSlot( pAttributeDefinition.vertexStreamSlot );
			if( attributeStream && !attributeStream->CheckAttributeCompatibility( pAttributeDefinition ) )
			{
				return false;
			}

			return true;
		}

		bool CheckAttributeArraySpace(
				const VertexFormatCombinedLayoutBase & pVertexFormatCombinedLayout,
				native_uint pFirstAttributeSlot,
				native_uint pAttributesNum ) noexcept
		{
			// Quick check: if the specified attribute range is completely outside of the range of active attributes in the
			// vertex format data, we can immediately return with success (no attribute outside this range is an active attribute).
			if( !pVertexFormatCombinedLayout.activeAttributesRange.overlaps_with( cppx::make_range_from_size<uint32>( pFirstAttributeSlot, pAttributesNum ) ) )
			{
				return true;
			}

			// Iterate over the specified range and check the attributes. If at least one is already an active one, return false (attributes
			// cannot be redefined after they are specified). Note: The range check above doesn't imply there is no space because the range
			// may not be continuous. Consider: If the layout has two active attributes: 0 and 5 the activeAttributesRange would be <0;5>.
			// However, there are 4 unused attributes in that range that can be used.
			for( uint32 iAttribute = 0; iAttribute < pAttributesNum; ++iAttribute )
			{
				if( pVertexFormatCombinedLayout.IsAttributeActive( pFirstAttributeSlot + iAttribute ) )
				{
					return false;
				}
			}

			return true;
		}

		bool AdjustAttributeDefinition(
				const VertexFormatCombinedLayoutBase & pVertexFormatCombinedLayout,
				VertexInputAttributeDefinition & pAttributeDefinition ) noexcept
		{
			// This method updates some of the "auto" values that can be specified for attributes with correct ones, based
			// on the current state of the vertex format configuration. Currently, the following values are supported:
			// - vertexStreamRelativeOffset: possible values are:
			//   > kIAVertexAttributeOffsetAppend, meaning that such attribute should follow immediately after the previous one in the vertex Stream.
			//   > kIAVertexAttributeOffsetAppend16, just like kIAVertexAttributeOffsetAppend, but the data should be also 16B-aligned..

			if( pAttributeDefinition.HasAppendAsRelativeOffset() )
			{
				// Get the vertex Stream this attribute is defined for.
				if( const auto * vertexInputStream = pVertexFormatCombinedLayout.GetStreamAtSlot( pAttributeDefinition.vertexStreamSlot ) )
				{
					// If the Stream is already defined in the Stream array config, replace the "append" offsets with
					// the current data size defined for this Stream (potentially aligned, if requested).
					if( pAttributeDefinition.vertexStreamRelativeOffset == kVertexAttributeOffsetAppend )
					{
						pAttributeDefinition.vertexStreamRelativeOffset = vertexInputStream->dataStrideInBytes;
					}
					else if( pAttributeDefinition.vertexStreamRelativeOffset == kVertexAttributeOffsetAppendAligned16 )
					{
						pAttributeDefinition.vertexStreamRelativeOffset = vertexInputStream->dataStrideInBytes;

						// In this case, overwrite the specified data padding and set it 16B alignment.
						// Padding is handled later in this function (see below).
						pAttributeDefinition.dataPadding = kVertexAttributePaddingAlign16;
					}
				}
				else
				{
					// If the Stream for the specified attribute does not exist yet, it means there will be no data
					// in this Stream before that attribute. In this case, offset is replaced with 0.
					pAttributeDefinition.vertexStreamRelativeOffset = 0u;
				}
			}

			// This handles also kVertexAttributePaddingAlign16
			if( pAttributeDefinition.dataPadding > 0 )
			{
				const auto attributeSizeInBytes = pAttributeDefinition.GetDataSizeInBytes();
				Ic3DebugAssert( attributeSizeInBytes <= 16 );

				const auto maxAllowedPaddingToFit = 16 - attributeSizeInBytes;
				const auto correctPaddingValue = cppx::get_min_of( pAttributeDefinition.dataPadding, maxAllowedPaddingToFit );

				pAttributeDefinition.dataPadding = cppx::numeric_cast<vertex_attribute_padding_t>( correctPaddingValue );
			}

			return true;
		}

		GenericVertexInputAttribute * AppendVertexFormatAttribute(
				VertexFormatCombinedLayoutBase & pVertexFormatCombinedLayout,
				const VertexInputAttributeDefinition & pAttributeDefinition ) noexcept
		{
			if( !CheckAttributeDefinitionCompatibility( pVertexFormatCombinedLayout, pAttributeDefinition ) )
			{
				return nullptr;
			}

			auto * vertexInputStream = AppendVertexInputStreamInternal( pVertexFormatCombinedLayout, pAttributeDefinition.vertexStreamSlot, pAttributeDefinition.dataRate );
			if( !vertexInputStream )
			{
				return false;
			}

			auto adjustedAttributeDefinition = pAttributeDefinition;
			if( !AdjustAttributeDefinition( pVertexFormatCombinedLayout, adjustedAttributeDefinition ) )
			{
				return false;
			}

			auto * genericVertexAttribute = AppendVertexFormatAttributeInternal( pVertexFormatCombinedLayout, adjustedAttributeDefinition );
			if( !genericVertexAttribute )
			{
				return false;
			}

			UpdateVertexStreamWithAttributeData( *vertexInputStream, *genericVertexAttribute );

			return genericVertexAttribute;
		}

		VertexInputStream * AppendVertexInputStreamInternal(
				VertexFormatCombinedLayoutBase & pVertexFormatCombinedLayout,
				native_uint pVertexStreamSlot,
				GCI::EIAVertexAttributeDataRate pStreamDataRate ) noexcept
		{
			if( !pVertexFormatCombinedLayout.IsStreamActive( pVertexStreamSlot ) )
			{
				auto * vertexInputStream = pVertexFormatCombinedLayout.AppendActiveStream( pVertexStreamSlot );
				Ic3DebugAssert( vertexInputStream );
				vertexInputStream->Init( pVertexStreamSlot, pStreamDataRate );
			}

			return pVertexFormatCombinedLayout.GetStreamAtSlot( pVertexStreamSlot );
		}

		GenericVertexInputAttribute * AppendVertexFormatAttributeInternal(
				VertexFormatCombinedLayoutBase & pVertexFormatCombinedLayout,
				const VertexInputAttributeDefinition & pAttributeDefinition ) noexcept
		{
			Ic3DebugAssert( !pVertexFormatCombinedLayout.IsAttributeActive( pAttributeDefinition.attributeSlot ) );

			auto * genericVertexAttributeBase = pVertexFormatCombinedLayout.AppendActiveAttribute( pAttributeDefinition.attributeSlot );
			Ic3DebugAssert( genericVertexAttributeBase );
			genericVertexAttributeBase->InitBaseAttributeFromDefinition( pAttributeDefinition );

			for( uint32 nSemanticComponent = 1; nSemanticComponent < pAttributeDefinition.semanticGroupSize; ++nSemanticComponent )
			{
				auto * genericSubAttribute = pVertexFormatCombinedLayout.AppendActiveAttribute( pAttributeDefinition.attributeSlot + nSemanticComponent );
				Ic3DebugAssert( genericSubAttribute );
				genericSubAttribute->InitSemanticSubAttributeFromBaseAttribute( *genericVertexAttributeBase, nSemanticComponent );
			}

			pVertexFormatCombinedLayout.activeAttributesRange.add(
					cppx::make_range_from_size<input_assembler_slot_t>(
							pAttributeDefinition.attributeSlot,
							pAttributeDefinition.semanticGroupSize ) );

			return genericVertexAttributeBase;
		}

		void UpdateVertexStreamWithAttributeData(
				VertexInputStream & pVertexInputStream,
				const GenericVertexInputAttribute & pVertexInputAttribute )
		{
			for( uint32 nSubAttribute = 0; nSubAttribute < pVertexInputAttribute.semanticGroupSize; ++nSubAttribute )
			{
				const auto genericAttributeSlot = pVertexInputAttribute.attributeSlot + nSubAttribute;
				const auto genericAttributeDataStride = pVertexInputAttribute.GetDataStride();

				pVertexInputStream.activeAttributesMask.set( GCI::CXU::IAMakeVertexAttributeFlag( genericAttributeSlot ) );
				pVertexInputStream.dataStrideInBytes += cppx::numeric_cast<uint16>( genericAttributeDataStride );
			}
		}

	}

} // namespace Ic3
