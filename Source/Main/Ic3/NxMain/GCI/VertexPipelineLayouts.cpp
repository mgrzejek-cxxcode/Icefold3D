
#include "VertexPipelineLayouts.h"

namespace Ic3
{

	CVertexAttributeArrayLayout::CVertexAttributeArrayLayout()
	: mLayoutData( _layoutData )
	{}

	CVertexAttributeArrayLayout::CVertexAttributeArrayLayout( CVertexAttributeArrayLayout && pSource ) noexcept
	: mLayoutData( _layoutData )
	, _layoutData( std::move( pSource._layoutData ) )
	{}

	CVertexAttributeArrayLayout & CVertexAttributeArrayLayout::operator=( CVertexAttributeArrayLayout && pRhs ) noexcept
	{
		_layoutData = std::move( pRhs._layoutData );
		return *this;
	}

	CVertexAttributeArrayLayout::CVertexAttributeArrayLayout( const CVertexAttributeArrayLayout & pSource )
	: mLayoutData( _layoutData )
	, _layoutData( pSource._layoutData )
	{}

	CVertexAttributeArrayLayout & CVertexAttributeArrayLayout::operator=( const CVertexAttributeArrayLayout & pRhs )
	{
		_layoutData = pRhs._layoutData;
		return *this;
	}

	CVertexAttributeArrayLayout::~CVertexAttributeArrayLayout() = default;

	SGenericVertexAttributeInfo & CVertexAttributeArrayLayout::operator[]( size_t pAttributeIndex ) noexcept
	{
		return _layoutData[pAttributeIndex];
	}

	const SGenericVertexAttributeInfo & CVertexAttributeArrayLayout::operator[]( size_t pAttributeIndex ) const noexcept
	{
		return _layoutData[pAttributeIndex];
	}

	CVertexAttributeArrayLayout::operator bool() const noexcept
	{
		return !empty();
	}

	bool CVertexAttributeArrayLayout::empty() const noexcept
	{
		return _layoutData.empty();
	}

	uint32 CVertexAttributeArrayLayout::resolveAttributeRef( std::string_view pSemanticName, uint32 pSubIndex ) const noexcept
	{
		const auto attribNameMapIter = _layoutData.semanticNameMap.find( pSemanticName );
		if( attribNameMapIter != _layoutData.semanticNameMap.end() )
		{
			const auto & baseAttributeInfo = _layoutData.attributesArray[attribNameMapIter->second];
			if( pSubIndex < baseAttributeInfo.subComponentsNum )
			{
				return attribNameMapIter->second + pSubIndex;
			}
		}
		return cxGCIVertexAttributeIndexUndefined;
	}

	uint32 CVertexAttributeArrayLayout::resolveAttributeRef( EShaderInputSemanticID pSemanticID ) const noexcept
	{
		const auto attribIDMapIter = _layoutData.semanticIDMap.find( pSemanticID );
		if( attribIDMapIter != _layoutData.semanticIDMap.end() )
		{
			return attribIDMapIter->second;
		}
		return cxGCIVertexAttributeIndexUndefined;
	}

	uint32 CVertexAttributeArrayLayout::resolveAttributeRef( EStandardVertexAttributeID pAttributeID ) const noexcept
	{
		const auto semanticID = CxDef::getStandardVertexAttributeSemanticID( pAttributeID );
		return resolveAttributeRef( semanticID );
	}

	uint32 CVertexAttributeArrayLayout::resolveAttributeRef( const SShaderSemantics & pSemantics ) const noexcept
	{
		const auto semanticName = pSemantics.empty() ? pSemantics.smtName : std::string_view{};
		return resolveAttributeRef( semanticName );
	}

	const SGenericVertexAttributeInfo * CVertexAttributeArrayLayout::getAttribute( uint32 pAttributeIndex ) const noexcept
	{
		if( cxGCIValidVertexAttributeIndexRange.contains( pAttributeIndex ) )
		{
			if( const auto & attributeInfo = _layoutData[pAttributeIndex] )
			{
				return &attributeInfo;
			}
		}

		return nullptr;
	}

	const SGenericVertexAttributeInfo * CVertexAttributeArrayLayout::getAttribute( std::string_view pSemanticName, uint32 pSubIndex ) const noexcept
	{
		const auto resolvedIndex = resolveAttributeRef( pSemanticName, pSubIndex );
		return getAttribute( resolvedIndex );
	}

	const SGenericVertexAttributeInfo * CVertexAttributeArrayLayout::getAttribute( EShaderInputSemanticID pSemanticID ) const noexcept
	{
		const auto resolvedIndex = resolveAttributeRef( pSemanticID );
		return getAttribute( resolvedIndex );
	}

	const SGenericVertexAttributeInfo * CVertexAttributeArrayLayout::getAttribute( EStandardVertexAttributeID pAttributeID ) const noexcept
	{
		const auto resolvedIndex = resolveAttributeRef( pAttributeID );
		return getAttribute( resolvedIndex );
	}

	const SGenericVertexAttributeInfo * CVertexAttributeArrayLayout::getAttribute( const SShaderSemantics & pSemantics ) const noexcept
	{
		const auto resolvedIndex = resolveAttributeRef( pSemantics );
		return getAttribute( resolvedIndex );
	}

	ArrayView<const SGenericVertexAttributeInfo> CVertexAttributeArrayLayout::getActiveAttributesRange() const noexcept
	{
		return bindArrayView( _layoutData.attributesArray.data() + _layoutData.activeAttributesRange.begin,
		                      _layoutData.activeAttributesRange.length() );
	}

	std::string CVertexAttributeArrayLayout::generateVertexFormatStringID() const noexcept
	{
		return GCIUtils::generateVertexAttributeLayoutString( _layoutData );
	}

	std::vector<SVertexAttributeDefinition> CVertexAttributeArrayLayout::generateAttributeDefinitionArray() const noexcept
	{
		return {};
	}

	bool CVertexAttributeArrayLayout::checkAttributeSlotRangeFree(
			uint32 pAttributeBaseIndex,
			uint32 pAttributeComponentsNum ) const noexcept
	{
		for( uint32 nComponent = 0; nComponent < pAttributeComponentsNum; ++nComponent )
		{
			const auto componentIndex = pAttributeBaseIndex + nComponent;
			const auto & attributeInfo = _layoutData.attributesArray[componentIndex];

			if( attributeInfo.active() )
			{
				return false;
			}
		}

		return true;
	}

	bool CVertexAttributeArrayLayout::validateAttributeDefinition(
			SVertexAttributeDefinition & pAttributeDefinition,
			uint32 pComponentStrideInBytes,
			uint32 pCombinedAttributeDataStride ) const noexcept
	{
		if( !pAttributeDefinition.valid() )
		{
			return false;
		}

		if( !checkAttributeSlotRangeFree( pAttributeDefinition.baseAttributeIndex, pAttributeDefinition.componentsNum ) )
		{
			return false;
		}

		if( pCombinedAttributeDataStride == 0 )
		{
			if( pComponentStrideInBytes == 0 )
			{
				pComponentStrideInBytes = pAttributeDefinition.baseSize() + pAttributeDefinition.subComponentPadding;
			}

			pCombinedAttributeDataStride = pComponentStrideInBytes * pAttributeDefinition.componentsNum;
		}

		if( pAttributeDefinition.vertexStreamRelativeOffset >= GCI::CxDef::IA_VERTEX_ATTRIBUTE_OFFSET_APPEND16 )
		{
			return false;
		}

		if( ( pAttributeDefinition.dataStride > 0 ) && ( pAttributeDefinition.dataStride < pCombinedAttributeDataStride ) )
		{
			return false;
		}

		if( pAttributeDefinition.dataStride == 0 )
		{
			pAttributeDefinition.dataStride = pCombinedAttributeDataStride;
		}

		return true;
	}

	SGenericVertexAttributeInfo * CVertexAttributeArrayLayout::setAttribute( SVertexAttributeDefinition pAttributeDefinition )
	{
		const auto componentStrideInBytes = pAttributeDefinition.baseSize() + pAttributeDefinition.subComponentPadding;
		const auto combinedAttributeDataStride = componentStrideInBytes * pAttributeDefinition.componentsNum;

		if( !validateAttributeDefinition( pAttributeDefinition, componentStrideInBytes, combinedAttributeDataStride ) )
		{
			return nullptr;
		}

		auto & baseAttributeInfo = _updateBaseAttribute( pAttributeDefinition, componentStrideInBytes );

		return &baseAttributeInfo;
	}

	SGenericVertexAttributeInfo * CVertexAttributeArrayLayout::setAttributeUnchecked( SVertexAttributeDefinition pAttributeDefinition )
	{
		const auto componentStrideInBytes = pAttributeDefinition.baseSize() + pAttributeDefinition.subComponentPadding;
		const auto combinedAttributeDataStride = componentStrideInBytes * pAttributeDefinition.componentsNum;

		auto & baseAttributeInfo = _updateBaseAttribute( pAttributeDefinition, componentStrideInBytes );

		return &baseAttributeInfo;
	}

	SGenericVertexAttributeInfo * CVertexAttributeArrayLayout::setStandardAttribute(
			EStandardVertexAttributeID pAttributeID,
			uint32 pStreamIndex,
			uint32 pStreamRelativeOffset,
			uint32 pDataStride )
	{
		SVertexAttributeDefinition attributeDefinition{};
		attributeDefinition.baseAttributeIndex = CxDef::getStandardVertexAttributeBaseIndex( pAttributeID );
		attributeDefinition.baseFormat = CxDef::getStandardVertexAttributeBaseFormat( pAttributeID );
		attributeDefinition.semantics.smtID = CxDef::getStandardVertexAttributeSemanticID( pAttributeID );
		attributeDefinition.componentsNum = CxDef::getStandardVertexAttributeComponentsNum( pAttributeID );
		attributeDefinition.subComponentPadding = 0u;
		const auto standardAttributeFlags = CxDef::getStandardShaderInputSemanticFlags( pAttributeID );
		attributeDefinition.instanceRate = standardAttributeFlags.isSetAnyOf( E_SHADER_INPUT_SEMANTIC_MASK_INSTANCE_ALL ) ? 1 : 0;
		attributeDefinition.vertexStreamIndex = pStreamIndex;
		attributeDefinition.vertexStreamRelativeOffset = numeric_cast<uint16>( pStreamRelativeOffset );

		return setAttribute( std::move( attributeDefinition ) );
	}

	void CVertexAttributeArrayLayout::reset()
	{
		for( auto & attributeInfo : _layoutData.attributesArray )
		{
			attributeInfo.reset();
		}

		_layoutData.activeBaseAttributesNum = 0;
		_layoutData.activeGenericAttributesNum = 0;
		_layoutData.activeAttributesRange.setInvalid();
		_layoutData.activeAttributesMask.clear();
		_layoutData.activeAttributeSemanticsMask.clear();
		_layoutData.semanticIDMap.clear();
		_layoutData.semanticNameMap.clear();
	}

	SGenericVertexAttributeInfo & CVertexAttributeArrayLayout::_updateBaseAttribute(
			SVertexAttributeDefinition & pAttributeDefinition,
			uint32 pComponentStrideInBytes )
	{
		const auto baseAttributeIndex = pAttributeDefinition.baseAttributeIndex;
		const auto lastComponentIndex = baseAttributeIndex + pAttributeDefinition.componentsNum - 1;

		auto & baseAttributeInfo = _layoutData[baseAttributeIndex];
		baseAttributeInfo.baseFormat = pAttributeDefinition.baseFormat;
		baseAttributeInfo.baseAttributeIndex = baseAttributeIndex;
		baseAttributeInfo.instanceRate = pAttributeDefinition.instanceRate;
		baseAttributeInfo.dataStride = pAttributeDefinition.dataStride;
		baseAttributeInfo.vertexStreamIndex = pAttributeDefinition.vertexStreamIndex;
		baseAttributeInfo.vertexStreamRelativeOffset = pAttributeDefinition.vertexStreamRelativeOffset;
		baseAttributeInfo.semantics = std::move( pAttributeDefinition.semantics );
		baseAttributeInfo.attributeClass = EVertexAttributeClass::BaseAttribute;
		baseAttributeInfo.subComponentIndex = 0;

		_layoutData.activeBaseAttributesNum += 1;
		_layoutData.activeGenericAttributesNum += 1;
		_layoutData.activeAttributesMask.set( GCI::CxDef::makeIAVertexAttributeFlag( baseAttributeIndex ) );
		_layoutData.activeAttributeSemanticsMask.set( static_cast<uint32>( baseAttributeInfo.semantics.smtID ) );

		_layoutData.semanticIDMap[baseAttributeInfo.semantics.smtID] = baseAttributeIndex;
		_layoutData.semanticNameMap[baseAttributeInfo.semantics.smtName] = baseAttributeIndex;

		if( pAttributeDefinition.componentsNum == 1 )
		{
			baseAttributeInfo.baseAttribute = &baseAttributeInfo;
			baseAttributeInfo.nextComponent = nullptr;
		}
		else
		{
			_updateSubComponents( baseAttributeInfo, pAttributeDefinition.componentsNum, pComponentStrideInBytes );
		}

		if( baseAttributeIndex < _layoutData.activeAttributesRange.begin )
		{
			_layoutData.activeAttributesRange.begin = baseAttributeIndex;
		}

		if( lastComponentIndex > _layoutData.activeAttributesRange.end )
		{
			_layoutData.activeAttributesRange.end = lastComponentIndex;
		}

		return baseAttributeInfo;
	}

	void CVertexAttributeArrayLayout::_updateSubComponents(
			SGenericVertexAttributeInfo & pBaseAttribute,
			uint32 pAttributeComponentsNum,
			uint32 pComponentStrideInBytes )
	{
		ic3DebugAssert( pAttributeComponentsNum > 1 );

		auto * baseAttributePtr = &pBaseAttribute;
		auto * previousAttributePtr = baseAttributePtr;

		for( uint32 nComponent = 1; nComponent < pAttributeComponentsNum; ++nComponent )
		{
			const auto subComponentIndex = pBaseAttribute.baseAttributeIndex + nComponent;

			auto & subComponentInfo = _layoutData[subComponentIndex];
			subComponentInfo.baseFormat = pBaseAttribute.baseFormat;
			subComponentInfo.baseAttributeIndex = pBaseAttribute.baseAttributeIndex;
			subComponentInfo.instanceRate = pBaseAttribute.instanceRate;
			subComponentInfo.dataStride = pBaseAttribute.dataStride;
			subComponentInfo.vertexStreamIndex = pBaseAttribute.vertexStreamIndex;
			subComponentInfo.vertexStreamRelativeOffset = pBaseAttribute.vertexStreamRelativeOffset + ( nComponent * pComponentStrideInBytes );
			subComponentInfo.attributeClass = EVertexAttributeClass::SubComponent;
			subComponentInfo.subComponentIndex = nComponent;

			_layoutData.activeGenericAttributesNum += 1;
			_layoutData.activeAttributesMask.set( GCI::CxDef::makeIAVertexAttributeFlag( subComponentIndex ) );

			previousAttributePtr->baseAttribute = baseAttributePtr;
			previousAttributePtr->nextComponent = &subComponentInfo;
			previousAttributePtr = previousAttributePtr->nextComponent;
		}

		previousAttributePtr->baseAttribute = baseAttributePtr;
		previousAttributePtr->nextComponent = nullptr;
	}


	CVertexStreamArrayLayout::CVertexStreamArrayLayout()
	: mLayoutData( _layoutData )
	{}

	CVertexStreamArrayLayout::CVertexStreamArrayLayout( CVertexStreamArrayLayout && pSource ) noexcept
	: mLayoutData( _layoutData )
	, _layoutData( std::move( pSource._layoutData ) )
	{}

	CVertexStreamArrayLayout & CVertexStreamArrayLayout::operator=( CVertexStreamArrayLayout && pRhs ) noexcept
	{
		_layoutData = std::move( pRhs._layoutData );
		return *this;
	}

	CVertexStreamArrayLayout::CVertexStreamArrayLayout( const CVertexStreamArrayLayout & pSource )
	: mLayoutData( _layoutData )
	, _layoutData( pSource._layoutData )
	{}

	CVertexStreamArrayLayout & CVertexStreamArrayLayout::operator=( const CVertexStreamArrayLayout & pRhs )
	{
		_layoutData = pRhs._layoutData;
		return *this;
	}

	CVertexStreamArrayLayout::~CVertexStreamArrayLayout() = default;

	SVertexStreamInfo & CVertexStreamArrayLayout::operator[]( size_t pStreamIndex ) noexcept
	{
		return _layoutData[pStreamIndex];
	}

	const SVertexStreamInfo & CVertexStreamArrayLayout::operator[]( size_t pStreamIndex ) const noexcept
	{
		return _layoutData[pStreamIndex];
	}

	CVertexStreamArrayLayout::operator bool() const noexcept
	{
		return !empty();
	}

	bool CVertexStreamArrayLayout::empty() const noexcept
	{
		return _layoutData.empty();
	}

	ArrayView<const SVertexStreamInfo> CVertexStreamArrayLayout::getActiveStreamsRange() const noexcept
	{
		return bindArrayView( _layoutData.streamArray.data() + _layoutData.activeStreamsRange.begin,
		                      _layoutData.activeStreamsRange.length() );
	}

}
