
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

	uint32 VertexFormatDescriptor::resolveAttributeRef( EShaderInputSemanticID pSemanticID, uint32 pSemanticIndex ) const noexcept
	{
		const auto semanticName = GCU::getShaderInputSemanticNameFromID( pSemanticID );
		return _resolveAttributeRefImpl( semanticName, pSemanticIndex );
	}

	uint32 VertexFormatDescriptor::resolveAttributeRef( VertexAttributeKey pAttributeKey, uint32 pSemanticIndex ) const noexcept
	{
		const auto semanticID = CxDef::getVertexAttributeKeySemanticID( pAttributeKey );
		return resolveAttributeRef( semanticID, pSemanticIndex );
	}

	uint32 VertexFormatDescriptor::resolveAttributeRef( std::string_view pSemanticName, uint32 pSemanticIndex ) const noexcept
	{
		return _resolveAttributeRefImpl( pSemanticName, pSemanticIndex );
	}

	uint32 VertexFormatDescriptor::resolveAttributeRef( const ShaderSemantics & pSemantics, uint32 pSemanticIndex ) const noexcept
	{
		const auto semanticName = pSemantics.empty() ? pSemantics.smtName : std::string_view{};
		return _resolveAttributeRefImpl( semanticName, pSemanticIndex );
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
			const auto & activeAttributes = _attribArrayLayout.getActiveAttributes();
			resultDefinitionArray.reserve( activeAttributes.size() );

			for( const auto & baseAttributeInfo : activeAttributes )
			{
				if( baseAttributeInfo.isBaseAttribute() )
				{
					auto & attributeDefinition = resultDefinitionArray.emplace_back();
					attributeDefinition.baseFormat = baseAttributeInfo.baseFormat;
					attributeDefinition.attributeIASlot = baseAttributeInfo.attributeIASlot;
					attributeDefinition.semanticComponentsNum = baseAttributeInfo.semanticComponentsNum;
					attributeDefinition.semanticIndex = 0u;
					attributeDefinition.componentPadding = baseAttributeInfo.componentPadding;
					attributeDefinition.vertexStreamIASlot = baseAttributeInfo.vertexStreamIASlot;
					attributeDefinition.vertexStreamRelativeOffset = baseAttributeInfo.vertexStreamRelativeOffset;
					attributeDefinition.shaderSemantics = baseAttributeInfo.shaderSemantics;

					const auto & attributeStream = _streamArrayConfig[baseAttributeInfo.vertexStreamIASlot];
					attributeDefinition.dataRate = attributeStream.streamDataRate;
				}
			}
		}

		return resultDefinitionArray;
	}

	uint32 VertexFormatDescriptor::_resolveAttributeRefImpl( std::string_view pSemanticName, uint32 pSemanticIndex ) const noexcept
	{
		const auto baseAttributeSlot = _attribArrayLayout.resolveSemanticName( pSemanticName );
		if( ( baseAttributeSlot != cxGCIVertexAttributeSlotUndefined ) && ( pSemanticIndex < GCM::cxIAMaxVertexAttributeComponentsNum ) )
		{
			const auto & baseAttributeComponent = _attribArrayLayout[baseAttributeSlot];
			const auto requestedAttributeComponentSlot = baseAttributeSlot + pSemanticIndex;
			if( const auto * requestedAttributeComponent = _attribArrayLayout.attributePtr( requestedAttributeComponentSlot ) )
			{
				if( requestedAttributeComponent->hasSameSemanticsAs( baseAttributeComponent ) )
				{
					ic3DebugAssert( requestedAttributeComponent->shaderSemantics.smtIndex == pSemanticIndex );
					return
				}
			}
		}
		return cxGCIVertexAttributeSlotUndefined;
	}

	uint32 VertexFormatDescriptor::_resolveAttributeRefImpl( EShaderInputSemanticID pSemanticID, uint32 pSemanticIndex ) const noexcept
	{
		if( CxDef::isStandardShaderInputSemanticID( pSemanticID ) )
		{
			const auto semanticName = GCU::getShaderInputSemanticNameFromID( pSemanticID );
			return _resolveAttributeRefImpl( semanticName, pSemanticIndex );
		}
		return cxGCIVertexAttributeSlotUndefined;
	}


	namespace GCU
	{

		std::string generateVertexFormatLayoutString(
				const VertexAttributeArrayLayout & pAttributeLayout,
				const VertexStreamArrayConfig & pStreamConfig )
		{
			std::string vertexStreamStrings[GCM::cxIAMaxVertexStreamsNum];

			const auto & attributes = pAttributeLayout.getActiveAttributes();
			for( const auto & attributeComponent : attributes )
			{
				if( attributeComponent.isBaseAttribute() )
				{
					const auto attributeFormatStr = generateVertexAttributeFormatString( attributeComponent );
					if( !vertexStreamStrings[attributeComponent.vertexStreamIASlot].empty() )
					{
						vertexStreamStrings[attributeComponent.vertexStreamIASlot].append( 1, '|' );
					}
					vertexStreamStrings[attributeComponent.vertexStreamIASlot].append( attributeFormatStr );
				}
			}

			std::string resultStringID;

			const auto & streams = pStreamConfig.getActiveStreams();
			for( const auto & streamComponent : streams )
			{
				if( !vertexStreamStrings[streamComponent.streamIASlot].empty() )
				{
					resultStringID.append( 1, '#' );
					resultStringID.append( 1, 'S' );
					resultStringID.append( std::to_string( streamComponent.streamIASlot ) );
					resultStringID.append( 1, '(' );
					resultStringID.append( 1, ( streamComponent.streamDataRate == EVertexDataRate::PerInstance ) ? 'I' : 'V' );
					resultStringID.append( 1, ')' );
					resultStringID.append( 1, '<' );
					resultStringID.append( vertexStreamStrings[streamComponent.streamIASlot] );
					resultStringID.append( 1, '>' );
				}
			}

			return resultStringID;
		}

	}

}
