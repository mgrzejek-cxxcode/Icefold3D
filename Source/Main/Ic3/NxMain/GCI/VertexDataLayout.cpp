
#include "VertexPipelineLayouts.h"

namespace Ic3
{

	CVertexDataLayout::CVertexDataLayout()
	: mIndexDataFormat( _indexDataFormat )
	, mPrimitiveTopology( _primitiveTopology )
	, mAttribLayoutData( _attribLayoutData )
	, mStreamLayoutData( _streamLayoutData )
	{}

	CVertexDataLayout::CVertexDataLayout( CVertexDataLayout && pSource ) noexcept
	: mIndexDataFormat( _indexDataFormat )
	, mPrimitiveTopology( _primitiveTopology )
	, mAttribLayoutData( _attribLayoutData )
	, mStreamLayoutData( _streamLayoutData )
	{}

	CVertexDataLayout & CVertexDataLayout::operator=( CVertexDataLayout && pRhs ) noexcept
	{
		_indexDataFormat = pRhs._indexDataFormat;
		_primitiveTopology = pRhs._primitiveTopology;
		_attribLayoutData = std::move( pRhs._attribLayoutData );
		_streamLayoutData = std::move( pRhs._streamLayoutData );
		return *this;
	}

	CVertexDataLayout::CVertexDataLayout( const CVertexDataLayout & pSource )
	: mIndexDataFormat( _indexDataFormat )
	, mPrimitiveTopology( _primitiveTopology )
	, mAttribLayoutData( _attribLayoutData )
	, mStreamLayoutData( _streamLayoutData )
	, _indexDataFormat( pSource._indexDataFormat )
	, _primitiveTopology( pSource._primitiveTopology )
	, _attribLayoutData( pSource._attribLayoutData )
	, _streamLayoutData( pSource._streamLayoutData )
	{}

	CVertexDataLayout & CVertexDataLayout::operator=( const CVertexDataLayout & pRhs )
	{
		_indexDataFormat = pRhs._indexDataFormat;
		_primitiveTopology = pRhs._primitiveTopology;
		_attribLayoutData = pRhs._attribLayoutData;
		_streamLayoutData = pRhs._streamLayoutData;
		return *this;
	}

	CVertexDataLayout::~CVertexDataLayout() = default;

	bool CVertexDataLayout::empty() const noexcept
	{
		return _attribLayoutData.empty();
	}

	bool CVertexDataLayout::equals( const CVertexDataLayout & pOther ) const noexcept
	{
		for( uint32 nAttribute = 0; nAttribute < GCM::IA_MAX_VERTEX_ATTRIBUTES_NUM; ++nAttribute )
		{
			const auto & attribute = _attribLayoutData[nAttribute];
			const auto & otherAttribute = pOther._attribLayoutData[nAttribute];

			if( !attribute.equals( otherAttribute ) )
			{
				return false;
			}
		}

		return true;
	}

	bool CVertexDataLayout::isIndexedGeometry() const noexcept
	{
		return _indexDataFormat != GCI::EIndexDataFormat::Undefined;
	}

	bool CVertexDataLayout::isAttributeActive( uint32 pAttributeIndex ) const
	{
		const auto * attributeInfo = _attribLayoutData.attributePtr( pAttributeIndex );
		return attributeInfo && attributeInfo->active();
	}

	bool CVertexDataLayout::isVertexStreamActive( uint32 pStreamIndex ) const
	{
		const auto * streamUsage = _streamLayoutData.streamPtr( pStreamIndex );
		return streamUsage && streamUsage->active();
	}

	const SortedArray<uint32> & CVertexDataLayout::getActiveAttributeIndexList() const noexcept
	{
		return _attribLayoutData.activeAttributes;
	}

	const SortedArray<uint32> & CVertexDataLayout::getActiveStreamIndexList() const noexcept
	{
		return _streamLayoutData.activeStreams;
	}

	uint32 CVertexDataLayout::resolveAttributeRef( EShaderInputSemanticID pSemanticID, uint32 pSubIndex ) const noexcept
	{
		const auto attribIDMapIter = _attribLayoutData.semanticIDMap.find( pSemanticID );
		if( attribIDMapIter != _attribLayoutData.semanticIDMap.end() )
		{
			const auto & baseAttributeInfo = _attribLayoutData.attributesArray[attribIDMapIter->second];
			if( pSubIndex < baseAttributeInfo.componentsNum )
			{
				return attribIDMapIter->second + pSubIndex;
			}
		}
		return cxGCIVertexAttributeIndexUndefined;
	}

	uint32 CVertexDataLayout::resolveAttributeRef( EStandardVertexAttributeID pAttributeID, uint32 pSubIndex ) const noexcept
	{
		const auto semanticID = CxDef::getStandardVertexAttributeSemanticID( pAttributeID );
		return resolveAttributeRef( semanticID );
	}

	uint32 CVertexDataLayout::resolveAttributeRef( std::string_view pSemanticName, uint32 pSubIndex ) const noexcept
	{
		const auto attribNameMapIter = _attribLayoutData.semanticNameMap.find( pSemanticName );
		if( attribNameMapIter != _attribLayoutData.semanticNameMap.end() )
		{
			const auto & baseAttributeInfo = _attribLayoutData.attributesArray[attribNameMapIter->second];
			if( pSubIndex < baseAttributeInfo.componentsNum )
			{
				return attribNameMapIter->second + pSubIndex;
			}
		}
		return cxGCIVertexAttributeIndexUndefined;
	}

	uint32 CVertexDataLayout::resolveAttributeRef( const SShaderSemantics & pSemantics, uint32 pSubIndex ) const noexcept
	{
		const auto semanticName = pSemantics.empty() ? pSemantics.smtName : std::string_view{};
		return resolveAttributeRef( semanticName, pSubIndex );
	}

	std::string CVertexDataLayout::generateVertexFormatStringID() const noexcept
	{
		return GCIUtils::generateVertexAttributeLayoutString( _attribLayoutData );
	}

	std::vector<SVertexAttributeDefinition> CVertexDataLayout::generateAttributeDefinitionArray() const noexcept
	{
		std::vector<SVertexAttributeDefinition> resultDefinitionArray;
		if( !_attribLayoutData.empty() )
		{
			resultDefinitionArray.reserve( _attribLayoutData.activeBaseAttributesNum );
			for( const auto & baseAttributeInfo : _attribLayoutData.attributesArray )
			{
				if( baseAttributeInfo.isBaseAttribute() )
				{
					auto & attributeDefinition = resultDefinitionArray.emplace_back();
					attributeDefinition.baseFormat = baseAttributeInfo.baseFormat;
					attributeDefinition.baseAttributeIndex = baseAttributeInfo.baseAttributeIndex;
					attributeDefinition.componentsNum = baseAttributeInfo.componentsNum;
					attributeDefinition.instanceRate = baseAttributeInfo.instanceRate;
					attributeDefinition.vertexStreamIndex = baseAttributeInfo.vertexStreamIndex;
					attributeDefinition.vertexStreamRelativeOffset = baseAttributeInfo.vertexStreamRelativeOffset;
					attributeDefinition.semantics = baseAttributeInfo.semantics;
					attributeDefinition.componentPadding = 0;
					attributeDefinition.componentStride = baseAttributeInfo.elementStride;
				}
			}
		}
		return resultDefinitionArray;
	}

	const SGenericVertexAttributeInfo * CVertexDataLayout::addAttribute( SVertexAttributeDefinition pAttributeDefinition )
	{
		return GCIUtils::appendVertexLayoutAttribute(  std::move( pAttributeDefinition ), _attribLayoutData, &_streamLayoutData );
	}

	const SGenericVertexAttributeInfo * CVertexDataLayout::addStandardAttribute(
			EStandardVertexAttributeID pAttributeID,
			uint32 pStreamIndex,
			uint32 pStreamRelativeOffset,
			uint32 pDataStride )
	{
		if( pDataStride == 0 )
		{
			pDataStride = CxDef::getStandardVertexAttributeComponentByteSize( pAttributeID );
		}

		SVertexAttributeDefinition attributeDefinition;
		attributeDefinition.baseAttributeIndex = CxDef::getStandardVertexAttributeBaseIndex( pAttributeID );
		attributeDefinition.baseFormat = CxDef::getStandardVertexAttributeBaseFormat( pAttributeID );
		attributeDefinition.componentsNum = CxDef::getStandardVertexAttributeComponentsNum( pAttributeID );
		attributeDefinition.componentStride = pDataStride;
		attributeDefinition.instanceRate = 0;
		attributeDefinition.componentPadding = 0;
		attributeDefinition.vertexStreamIndex = pStreamIndex;
		attributeDefinition.vertexStreamRelativeOffset = pStreamRelativeOffset;
		attributeDefinition.semantics = SShaderSemantics{ pAttributeID };

		return addAttribute( std::move( attributeDefinition ) );
	}

	SortedArray<uint32> CVertexDataLayout::addAttributes( ArrayView<SVertexAttributeDefinition> pAttributeDefinitions )
	{
		SortedArray<uint32> resultIndexArray{};

		for( auto & attributeDefinition : pAttributeDefinitions )
		{
			if( const auto * baseAttributeInfo = addAttribute( std::move( attributeDefinition ) ) )
			{
				resultIndexArray.insert( baseAttributeInfo->baseAttributeIndex );
			}
		}

		return resultIndexArray;
	}

	SortedArray<uint32> CVertexDataLayout::setAttributes( ArrayView<SVertexAttributeDefinition> pAttributeDefinitions )
	{
		resetLayouts();
		return addAttributes( pAttributeDefinitions );
	}

	void CVertexDataLayout::setIndexDataFormat( GCI::EIndexDataFormat pIndexDataFormat )
	{
		_indexDataFormat = pIndexDataFormat;
	}

	void CVertexDataLayout::setPrimitiveTopology( GCI::EPrimitiveTopology pPrimitiveTopology )
	{
		_primitiveTopology = pPrimitiveTopology;
	}

	void CVertexDataLayout::resetIndexDataFormat()
	{
		_indexDataFormat = GCI::EIndexDataFormat::Undefined;
	}

	void CVertexDataLayout::resetPrimitiveTopology()
	{
		_primitiveTopology = GCI::EPrimitiveTopology::Undefined;
	}

	void CVertexDataLayout::resetLayouts()
	{
		_attribLayoutData.reset();
		_streamLayoutData.reset();
	}

	void CVertexDataLayout::resetAll()
	{
		resetIndexDataFormat();
		resetPrimitiveTopology();
		resetLayouts();
	}

}
