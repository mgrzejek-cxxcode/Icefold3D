
#include "VertexFormatDescriptor.h"

namespace Ic3
{

	VertexFormatDescriptor::VertexFormatDescriptor()
	: mIndexDataFormat( _indexDataFormat )
	, mPrimitiveTopology( _primitiveTopology )
	, mAttribArrayLayout( _attribArrayLayout )
	, mStreamArrayConfig( _streamArrayConfig )
	{}

	VertexFormatDescriptor::VertexFormatDescriptor( VertexFormatDescriptor && pSource ) noexcept
	: mIndexDataFormat( _indexDataFormat )
	, mPrimitiveTopology( _primitiveTopology )
	, mAttribArrayLayout( _attribArrayLayout )
	, mStreamArrayConfig( _streamArrayConfig )
	{}

	VertexFormatDescriptor & VertexFormatDescriptor::operator=( VertexFormatDescriptor && pRhs ) noexcept
	{
		_indexDataFormat = pRhs._indexDataFormat;
		_primitiveTopology = pRhs._primitiveTopology;
		_attribArrayLayout = std::move( pRhs._attribArrayLayout );
		_streamArrayConfig = std::move( pRhs._streamArrayConfig );
		return *this;
	}

	VertexFormatDescriptor::VertexFormatDescriptor( const VertexFormatDescriptor & pSource )
	: mIndexDataFormat( _indexDataFormat )
	, mPrimitiveTopology( _primitiveTopology )
	, mAttribArrayLayout( _attribArrayLayout )
	, mStreamArrayConfig( _streamArrayConfig )
	, _indexDataFormat( pSource._indexDataFormat )
	, _primitiveTopology( pSource._primitiveTopology )
	, _attribArrayLayout( pSource._attribArrayLayout )
	, _streamArrayConfig( pSource._streamArrayConfig )
	{}

	VertexFormatDescriptor & VertexFormatDescriptor::operator=( const VertexFormatDescriptor & pRhs )
	{
		_indexDataFormat = pRhs._indexDataFormat;
		_primitiveTopology = pRhs._primitiveTopology;
		_attribArrayLayout = pRhs._attribArrayLayout;
		_streamArrayConfig = pRhs._streamArrayConfig;
		return *this;
	}

	VertexFormatDescriptor::~VertexFormatDescriptor() = default;

	bool VertexFormatDescriptor::equals( const VertexFormatDescriptor & pOther ) const noexcept
	{
		for( uint32 nAttribute = 0; nAttribute < GCM::cxIAMaxVertexAttributesNum; ++nAttribute )
		{
			const auto & attribute = _attribArrayLayout[nAttribute];
			const auto & otherAttribute = pOther._attribArrayLayout[nAttribute];

			if( !attribute.isSameAs( otherAttribute ) )
			{
				return false;
			}
		}

		return true;
	}

	uint32 VertexFormatDescriptor::getElementStrideForAttribute( native_uint pAttribIASlot ) const noexcept
	{
		const auto * genericAttribute = getAttribute( pAttribIASlot );
		if( genericAttribute && genericAttribute->isActive() )
		{
			const auto * attributeStream = getStream( genericAttribute->streamIASlot );
			if( attributeStream && attributeStream->isActive() )
			{
				return attributeStream->dataStrideInBytes;
			}
		}

		return 0;
	}

	uint32 VertexFormatDescriptor::getElementStrideForAttribute( StringView pSemanticName ) const noexcept
	{
		const auto attributeSlotIndex = _resolveAttributeRefImpl( pSemanticName, 0u );
		return getElementStrideForAttribute( attributeSlotIndex );
	}

	uint32 VertexFormatDescriptor::getElementStrideForAttribute( VertexAttributeKey pAttributeKey ) const noexcept
	{
		const auto attributeSlotIndex = _resolveAttributeRefImpl( pAttributeKey.getSystemSemanticFlags(), 0u );
		return getElementStrideForAttribute( attributeSlotIndex );
	}

	uint32 VertexFormatDescriptor::getElementStrideForAttribute( TBitmask<ESystemAttributeSemanticFlags> pSysSmtFlags ) const noexcept
	{
		const auto attributeSlotIndex = _resolveAttributeRefImpl( pSysSmtFlags, 0u );
		return getElementStrideForAttribute( attributeSlotIndex );
	}

	uint32 VertexFormatDescriptor::getElementStrideForAttributeUnchecked( native_uint pAttribIASlot ) const noexcept
	{
		const auto & genericAttribute = _attribArrayLayout[pAttribIASlot];
		const auto & attributeStream = _streamArrayConfig[genericAttribute.streamIASlot];
		return attributeStream.dataStrideInBytes;
	}

	uint32 VertexFormatDescriptor::getElementStrideForAttributeUnchecked( VertexAttributeKey pAttributeKey ) const noexcept
	{
		const auto & genericAttribute = _attribArrayLayout[pAttributeKey.uBaseSlot];
		const auto & attributeStream = _streamArrayConfig[genericAttribute.streamIASlot];
		return attributeStream.dataStrideInBytes;
	}

	uint32 VertexFormatDescriptor::resolveAttributeRef( const ShaderSemantics & pSemantics ) const noexcept
	{
		const auto semanticName = pSemantics.empty() ? pSemantics.semanticName.strView() : StringView{};
		return _resolveAttributeRefImpl( semanticName, pSemantics.semanticIndex );
	}

	uint32 VertexFormatDescriptor::resolveAttributeRef( StringView pSemanticName, uint32 pSemanticIndex ) const noexcept
	{
		return _resolveAttributeRefImpl( pSemanticName, pSemanticIndex );
	}

	uint32 VertexFormatDescriptor::resolveAttributeRef( VertexAttributeKey pAttributeKey, uint32 pSemanticIndex ) const noexcept
	{
		const auto sysSmtFlags = pAttributeKey.getSystemSemanticFlags();
		return _resolveAttributeRefImpl( sysSmtFlags, pSemanticIndex );
	}

	uint32 VertexFormatDescriptor::resolveAttributeRef( TBitmask<ESystemAttributeSemanticFlags> pSysSmtFlags, uint32 pSemanticIndex ) const noexcept
	{
		return _resolveAttributeRefImpl( pSysSmtFlags, pSemanticIndex );
	}

	std::string VertexFormatDescriptor::generateVertexFormatStringID() const noexcept
	{
		return GCU::generateVertexFormatLayoutString( _attribArrayLayout, _streamArrayConfig );
	}

	std::vector<VertexAttributeDefinition> VertexFormatDescriptor::generateAttributeDefinitionArray() const noexcept
	{
		std::vector<VertexAttributeDefinition> resultDefinitionArray;

		if( !_attribArrayLayout.empty() )
		{
			const auto & attributeArray = _attribArrayLayout.getAttributeArray();
			resultDefinitionArray.reserve( _attribArrayLayout.getActiveBaseAttributesNum() );

			for( const auto & attribute : attributeArray )
			{
				if( attribute.isBaseAttribute() )
				{
					auto & attributeDefinition = resultDefinitionArray.emplace_back();
					attributeDefinition.attributeIASlot = attribute.attributeIASlot;
					attributeDefinition.streamIASlot = attribute.streamIASlot;
					attributeDefinition.dataFormat = attribute.dataFormat;
					attributeDefinition.dataPadding = attribute.dataPadding;
					attributeDefinition.semanticGroupSize = attribute.semanticGroupSize;
					attributeDefinition.vertexStreamRelativeOffset = attribute.vertexStreamRelativeOffset;
					attributeDefinition.shaderSemantics = attribute.shaderSemantics;

					const auto & attributeStream = _streamArrayConfig[attribute.streamIASlot];
					attributeDefinition.dataRate = attributeStream.streamDataRate;
				}
			}
		}

		return resultDefinitionArray;
	}

	uint32 VertexFormatDescriptor::_resolveAttributeRefImpl( StringView pSemanticName, uint32 pSemanticIndex ) const noexcept
	{
		if( !pSemanticName.empty() && ( pSemanticIndex < GCM::cxIAMaxVertexAttributeSemanticGroupSize ) )
		{
			// Query the internal AttributeLayout object for the index of a base attribute with the specified semantics.
			// If such attribute does not exist, an invalid index is returned instead (cxGCIVertexAttributeIndexUndefined).
			const auto baseAttributeSlotIndex = _attribArrayLayout.queryBaseAttributeBySemantics( pSemanticName );

			if( baseAttributeSlotIndex != cxGCIVertexAttributeIndexUndefined )
			{
				const auto & baseAttribute = _attribArrayLayout[baseAttributeSlotIndex];

				// Attributes can be part of semantic groups, i.e. related attributes with the same semantic name,
				// occupying adjacent range of IA slots. Calculate the slot index of the requested attribute.
				const auto requestedAttributeIASlotIndex = baseAttributeSlotIndex + pSemanticIndex;

				// Fetch the requested attribute. In case it is not a valid attribute, a nullptr is returned here.
				if( const auto * requestedAttributeComponent = _attribArrayLayout.attributePtr( requestedAttributeIASlotIndex ) )
				{
					if( requestedAttributeComponent->hasSameSemanticsAs( baseAttribute ) )
					{
						ic3DebugAssert( requestedAttributeComponent->shaderSemantics.semanticIndex == pSemanticIndex );
						return requestedAttributeIASlotIndex;
					}
				}
			}
		}

		return cxGCIVertexAttributeIndexUndefined;
	}

	uint32 VertexFormatDescriptor::_resolveAttributeRefImpl( TBitmask<ESystemAttributeSemanticFlags> pSysSmtFlags, uint32 pSemanticIndex ) const noexcept
	{
		if( GCU::isStandardShaderInputAttribute( pSysSmtFlags ) )
		{
			const auto semanticName = GCU::getStandardSemanticNameFromSystemFlags( pSysSmtFlags );
			return _resolveAttributeRefImpl( semanticName, pSemanticIndex );
		}
		return cxGCIVertexAttributeIndexUndefined;
	}


	namespace GCU
	{

		std::string generateVertexFormatLayoutString(
				const VertexAttributeArrayLayout & pAttributeLayout,
				const VertexStreamArrayConfig & pStreamConfig )
		{
			std::string vertexStreamStrings[GCM::cxIAMaxVertexStreamsNum];

			const auto & attributeArray = pAttributeLayout.getAttributeArray();
			for( const auto & genericAttribute : attributeArray )
			{
				if( genericAttribute.isActive() )
				{
					if( genericAttribute.isBaseAttribute() )
					{
						const auto attributeFormatStr = generateVertexAttributeFormatString( genericAttribute );
						if( !vertexStreamStrings[genericAttribute.streamIASlot].empty() )
						{
							vertexStreamStrings[genericAttribute.streamIASlot].append( 1, '|' );
						}
						vertexStreamStrings[genericAttribute.streamIASlot].append( attributeFormatStr );
					}
				}
			}

			std::string resultStringID;

			const auto & streamArray = pStreamConfig.getStreamArray();
			for( const auto & vertexStream : streamArray )
			{
				if( vertexStream.isActive() )
				{
					if( !vertexStreamStrings[vertexStream.streamIASlot].empty() )
					{
						resultStringID.append( 1, '#' );
						resultStringID.append( 1, 'S' );
						resultStringID.append( std::to_string( vertexStream.streamIASlot ) );
						resultStringID.append( 1, '(' );
						resultStringID.append( 1, ( vertexStream.streamDataRate == EVertexDataRate::PerInstance ) ? 'I' : 'V' );
						resultStringID.append( 1, ')' );
						resultStringID.append( 1, '<' );
						resultStringID.append( vertexStreamStrings[vertexStream.streamIASlot] );
						resultStringID.append( 1, '>' );
					}
				}
			}

			return resultStringID;
		}

	}

}
