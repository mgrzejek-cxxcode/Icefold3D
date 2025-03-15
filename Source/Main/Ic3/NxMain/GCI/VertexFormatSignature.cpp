
#include "VertexFormatSignature.h"
#include "VertexFormatParsing.h"
#include <regex>

namespace Ic3
{

	VertexFormatSignatureBase::VertexFormatSignatureBase( const VertexFormatCombinedLayoutBase & pInternalLayout )
	: mIndexDataFormat( _indexDataFormat )
	, mPrimitiveTopology( _primitiveTopology )
	, mInternalLayout( pInternalLayout )
	{}

	VertexFormatSignatureBase::~VertexFormatSignatureBase() = default;

	uint32 VertexFormatSignatureBase::GetElementStrideForAttribute( native_uint pAttributeSlot ) const noexcept
	{
		const auto * genericAttribute = GetAttribute( pAttributeSlot );
		if( genericAttribute && genericAttribute->IsActive() )
		{
			const auto * attributeStream = GetStream( genericAttribute->vertexStreamSlot );
			if( attributeStream && attributeStream->IsActive() )
			{
				return attributeStream->dataStrideInBytes;
			}
		}

		return 0;
	}

	uint32 VertexFormatSignatureBase::GetElementStrideForAttribute( EVertexAttributeSemanticID pAttributeSemanticID ) const noexcept
	{
		return 0;
	}

	uint32 VertexFormatSignatureBase::GetElementStrideForAttribute( cppx::string_view pSemanticName ) const noexcept
	{
		return 0;
	}

	uint32 VertexFormatSignatureBase::GetElementStrideForAttributeUnchecked( native_uint pAttributeSlot ) const noexcept
	{
		return 0;
		// const auto & genericAttribute = _inputAttributeArrayLayout[pAttributeSlot];
		// const auto & attributeStream = _inputStreamArrayConfig[genericAttribute.vertexStreamSlot];
		// return attributeStream.dataStrideInBytes;
	}

	uint32 VertexFormatSignatureBase::GetElementStrideForAttributeUnchecked( EVertexAttributeSemanticID pAttributeSemanticID ) const noexcept
	{
		return 0;
		// const auto & genericAttribute = _inputAttributeArrayLayout[pAttributeKey.GetBaseSlot()];
		// const auto & attributeStream = _inputStreamArrayConfig[genericAttribute.vertexStreamSlot];
		// return attributeStream.dataStrideInBytes;
	}

	uint32 VertexFormatSignatureBase::GetElementStrideForAttributeUnchecked( cppx::string_view pSemanticName ) const noexcept
	{
		return 0;
	}

	bool VertexFormatSignatureBase::Equals( const VertexFormatSignatureBase & pOther ) const noexcept
	{
		// Signatures are also different if either index formats or primitive topologies are different.
		if( ( mIndexDataFormat != pOther.mIndexDataFormat ) || ( mPrimitiveTopology != pOther.mPrimitiveTopology ) )
		{
			return false;
		}

		return mInternalLayout.Equals( pOther.mInternalLayout );
	}

	std::string VertexFormatSignatureBase::GenerateSerialString() const noexcept
	{
		return VertexFormat::GenerateVertexFormatSignatureSerialString( mInternalLayout );
	}

	std::vector<VertexInputAttributeDefinition> VertexFormatSignatureBase::GenerateAttributeDefinitionArray() const noexcept
	{
		std::vector<VertexInputAttributeDefinition> resultDefinitionArray;

		if( !mInternalLayout.IsEmpty() )
		{
			const auto & attributeArray = mInternalLayout.GetActiveAttributesView();
			resultDefinitionArray.reserve( mInternalLayout.activeAttributesNum );

			for( const auto & attribute : attributeArray )
			{
				if( attribute.IsBaseAttribute() )
				{
					auto & attributeDefinition = resultDefinitionArray.emplace_back();
					attributeDefinition.attributeSlot = attribute.attributeSlot;
					attributeDefinition.semanticGroupSize = attribute.semanticGroupSize;
					attributeDefinition.baseDataFormat = attribute.dataFormat;
					attributeDefinition.dataPadding = attribute.dataPadding;
					attributeDefinition.semantics.semanticName = attribute.semanticName;
					attributeDefinition.semantics.semanticFlags = attribute.semanticFlags;
					attributeDefinition.vertexStreamSlot = attribute.vertexStreamSlot;
					attributeDefinition.vertexStreamRelativeOffset = attribute.vertexStreamRelativeOffset;

					const auto & streamArray = mInternalLayout.GetActiveStreamsView();
					const auto & attributeStream = streamArray[attribute.vertexStreamSlot];
					attributeDefinition.dataRate = attributeStream.streamDataRate;
				}
			}
		}

		return resultDefinitionArray;
	}

	TSharedHandle<VertexFormatSignatureBase> VertexFormatSignatureBase::MakeCopy() const
	{
		return nullptr;
	}

	void VertexFormatSignatureBase::SetIndexDataFormat( GCI::EIndexDataFormat pIndexDataFormat )
	{
	}

	void VertexFormatSignatureBase::SetPrimitiveTopology( GCI::EPrimitiveTopology pPrimitiveTopology )
	{
	}

	//uint32 VertexFormatSignatureBase::_ResolveAttributeRefImpl( cppx::string_view pSemanticName, uint32 pSemanticIndex ) const noexcept
	//{
	//	if( !pSemanticName.empty() && ( pSemanticIndex < GCM::kIAMaxVertexAttributeSemanticGroupSize ) )
	//	{
	//		// Query the internal AttributeLayout object for the index of a base attribute with the specified semantics.
	//		// If such attribute does not exist, an invalid index is returned instead (cxGCIVertexAttributeSlotUndefined).
	//		const auto baseAttributeSlot = _inputAttributeArrayLayout.QueryBaseAttributeBySemantics( pSemanticName );

	//		if( baseAttributeSlot != GCI::kIAVertexAttributeSlotUndefined )
	//		{
	//			const auto & baseAttribute = _inputAttributeArrayLayout[baseAttributeSlot];

	//			// Attributes can be part of semantic groups, i.e. related attributes with the same semantic name,
	//			// occupying adjacent range of IA slots. Calculate the slot index of the requested attribute.
	//			const auto requestedAttributeIASlot = baseAttributeSlot + pSemanticIndex;

	//			// Fetch the requested attribute. In case it is not a valid attribute, a nullptr is returned here.
	//			if( const auto * requestedAttributeComponent = _inputAttributeArrayLayout.AttributePtr( requestedAttributeIASlot ) )
	//			{
	//				if( requestedAttributeComponent->HasSameSemanticsAs( baseAttribute ) )
	//				{
	//					Ic3DebugAssert( requestedAttributeComponent->semanticIndex == pSemanticIndex );
	//					return requestedAttributeIASlot;
	//				}
	//			}
	//		}
	//	}

	//	return GCI::kIAVertexAttributeSlotUndefined;
	//}

}
