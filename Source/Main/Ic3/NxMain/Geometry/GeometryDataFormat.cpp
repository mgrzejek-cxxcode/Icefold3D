
#include "VertexPipelineConfig.h"

namespace Ic3
{

	CVertexPipelineConfig::CVertexPipelineConfig()
	: mPrimitiveTopology( _primitiveTopology )
	, mIndexDataFormat( _indexDataFormat )
	, mAttributeLayout( _attribLayout )
	, mStreamLayout( _streamLayout )
	{}

	CVertexPipelineConfig::~CVertexPipelineConfig() = default;

	const SVertexStreamInfo * CVertexPipelineConfig::getStreamForAttribute( uint32 pAttributeIndex ) const noexcept
	{
		const auto * attributeInfo = getAttribute( pAttributeIndex );
		return _getVertexStreamForAttribute( attributeInfo );
	}

	const SVertexStreamInfo * CVertexPipelineConfig::getStreamForAttribute( std::string_view pSemanticName, uint32 pSubIndex ) const noexcept
	{
		const auto * attributeInfo = getAttribute( pSemanticName, pSubIndex );
		return _getVertexStreamForAttribute( attributeInfo );
	}

	const SVertexStreamInfo * CVertexPipelineConfig::getStreamForAttribute( EShaderInputSemanticID pSemanticID ) const noexcept
	{
		const auto * attributeInfo = getAttribute( pSemanticID );
		return _getVertexStreamForAttribute( attributeInfo );
	}

	const SVertexStreamInfo * CVertexPipelineConfig::getStreamForAttribute( EStandardVertexAttributeID pAttributeID ) const noexcept
	{
		const auto * attributeInfo = getAttribute( pAttributeID );
		return _getVertexStreamForAttribute( attributeInfo );
	}

	const SVertexStreamInfo * CVertexPipelineConfig::getStreamForAttribute( const SShaderSemantics & pSemantics ) const noexcept
	{
		const auto * attributeInfo = getAttribute( pSemantics );
		return _getVertexStreamForAttribute( attributeInfo );
	}

	const SVertexStreamInfo * CVertexPipelineConfig::_getVertexStreamForAttribute( const SGenericVertexAttributeInfo * pAttributeInfo ) const noexcept
	{
		if( pAttributeInfo && pAttributeInfo->active() )
		{
			const auto streamIndex = pAttributeInfo->vertexStreamIndex;
			return getStream( streamIndex );
		}

		return nullptr;
	}



	bool CVertexPipelineConfigBuilder::checkAttributeSlotRangeFree( uint32 pAttributeBaseIndex, uint32 pAttributeComponentsNum ) const noexcept
	{
		for( uint32 nComponent = 0; nComponent < pAttributeComponentsNum; ++nComponent )
		{
			const auto componentIndex = pAttributeBaseIndex + nComponent;
			const auto & attributeInfo = _pipelineConfig->_attribLayout[componentIndex];
			if( attributeInfo.active() )
			{
				return false;
			}
		}

		return true;
	}

	SharedHandle<CVertexPipelineConfig> CVertexPipelineConfigBuilder::getPipelineConfig() const noexcept
	{
		return _pipelineConfig;
	}

	void CVertexPipelineConfigBuilder::setPrimitiveTopology( GCI::EPrimitiveTopology pTopology )
	{
		_pipelineConfig->_primitiveTopology = pTopology;
	}

	void CVertexPipelineConfigBuilder::setIndexDataFormat( GCI::EIndexDataFormat pIndexDataFormat )
	{
		_pipelineConfig->_indexDataFormat = pIndexDataFormat;
	}

	bool CVertexPipelineConfigBuilder::addAttribute( SVertexAttributeDefinition pAttributeDefinition )
	{
		const auto componentSizeInBytes = GCI::CxDef::getVertexAttribFormatByteSize( pAttributeDefinition.baseFormat );
		const auto componentStrideInBytes = componentSizeInBytes + pAttributeDefinition.subComponentPadding;

		if( !validateAttributeDefinition( pAttributeDefinition, componentStrideInBytes ) )
		{
			return false;
		}

		auto & baseAttributeInfo = _pipelineConfig->_attribLayout.setAttribute( pAttributeDefinition );
		updateVertexStreamState( baseAttributeInfo );
	}

	SGenericVertexAttributeInfo & CVertexPipelineConfigBuilder::updateAttribute(
			const SVertexAttributeDefinition & pAttributeDefinition,
			uint32 pComponentStrideInBytes )
	{
		const auto baseAttributeIndex = pAttributeDefinition.baseAttributeIndex;
		const auto lastComponentIndex = baseAttributeIndex + pAttributeDefinition.componentsNum - 1;

		auto & attribLayout = _pipelineConfig->_attribLayout;
		auto & baseAttributeInfo = attribLayout[baseAttributeIndex];
		baseAttributeInfo.baseFormat = pAttributeDefinition.baseFormat;
		baseAttributeInfo.baseAttributeIndex = baseAttributeIndex;
		baseAttributeInfo.instanceRate = pAttributeDefinition.instanceRate;
		baseAttributeInfo.dataStride = pAttributeDefinition.dataStride;
		baseAttributeInfo.vertexStreamIndex = pAttributeDefinition.vertexStreamIndex;
		baseAttributeInfo.vertexStreamRelativeOffset = pAttributeDefinition.vertexStreamRelativeOffset;
		baseAttributeInfo.semantics = std::move( pAttributeDefinition.semantics );
		baseAttributeInfo.attributeClass = EVertexAttributeClass::BaseAttribute;
		baseAttributeInfo.subComponentIndex = 0;

		attribLayout.activeBaseAttributesNum += 1;
		attribLayout.activeGenericAttributesNum += 1;
		attribLayout.activeAttributesMask.set( GCI::CxDef::makeIAVertexAttributeFlag( baseAttributeIndex ) );
		attribLayout.activeAttributeSemanticsMask.set( static_cast<uint32>( baseAttributeInfo.semantics.smtID ) );

		attribLayout.semanticIDMap[baseAttributeInfo.semantics.smtID] = baseAttributeIndex;
		attribLayout.semanticNameMap[baseAttributeInfo.semantics.smtName] = baseAttributeIndex;

		updateAttributeSubComponents( baseAttributeInfo, pAttributeDefinition.componentsNum, pComponentStrideInBytes );

		if( baseAttributeIndex < attribLayout.activeAttributesRange.begin )
		{
			attribLayout.activeAttributesRange.begin = baseAttributeIndex;
		}

		if( lastComponentIndex > attribLayout.activeAttributesRange.end )
		{
			attribLayout.activeAttributesRange.end = lastComponentIndex;
		}

		return baseAttributeInfo;
	}

	void CVertexPipelineConfigBuilder::updateAttributeSubComponents(
			SGenericVertexAttributeInfo & pBaseAttribute,
			uint32 pAttributeComponentsNum,
			uint32 pComponentStrideInBytes )
	{
		if( pAttributeComponentsNum > 1 )
		{
			auto & attribLayout = _pipelineConfig->_attribLayout;

			auto * baseAttributePtr = &pBaseAttribute;
			auto * currentAttributePtr = baseAttributePtr;

			for( uint32 nComponent = 1; nComponent < pAttributeComponentsNum; ++nComponent )
			{
				const auto subComponentIndex = pBaseAttribute.baseAttributeIndex + nComponent;

				auto & subComponentInfo = attribLayout[subComponentIndex];
				subComponentInfo.baseFormat = pBaseAttribute.baseFormat;
				subComponentInfo.baseAttributeIndex = pBaseAttribute.baseAttributeIndex;
				subComponentInfo.instanceRate = pBaseAttribute.instanceRate;
				subComponentInfo.dataStride = pBaseAttribute.dataStride;
				subComponentInfo.vertexStreamIndex = pBaseAttribute.vertexStreamIndex;
				subComponentInfo.vertexStreamRelativeOffset = pBaseAttribute.vertexStreamRelativeOffset + ( nComponent * pComponentStrideInBytes );
				subComponentInfo.attributeClass = EVertexAttributeClass::SubComponent;
				subComponentInfo.subComponentIndex = nComponent;

				attribLayout.activeGenericAttributesNum += 1;
				attribLayout.activeAttributesMask.set( GCI::CxDef::makeIAVertexAttributeFlag( subComponentIndex ) );

				currentAttributePtr->nextComponent = &subComponentInfo;
				currentAttributePtr = currentAttributePtr->nextComponent;
			}
		}
	}

	void CVertexPipelineConfigBuilder::updateVertexStreamState(
			SGenericVertexAttributeInfo & pBaseAttribute )
	{
		ic3DebugAssert( pBaseAttribute.isBaseAttribute() );

		auto & streamLayout = _pipelineConfig->_streamLayout;
		auto & vertexStreamInfo = streamLayout[pBaseAttribute.vertexStreamIndex];

		if( vertexStreamInfo.activeAttributesMask.empty() )
		{
			streamLayout.activeStreamsNum += 1;
			streamLayout.activeStreamsMask.set( GCI::CxDef::makeIAVertexBufferFlag( pBaseAttribute.vertexStreamIndex ) );
		}

		vertexStreamInfo.activeAttributesNum += 1;
		vertexStreamInfo.activeAttributesMask.set( GCI::CxDef::makeIAVertexAttributeFlag( pBaseAttribute.dataStride ) );
		vertexStreamInfo.elementStrideInBytes += pBaseAttribute.dataStride;

		if( pBaseAttribute.vertexStreamIndex < streamLayout.activeStreamsRange.begin )
		{
			streamLayout.activeStreamsRange.begin = pBaseAttribute.vertexStreamIndex;
		}

		if( pBaseAttribute.vertexStreamIndex > streamLayout.activeStreamsRange.end  )
		{
			streamLayout.activeStreamsRange.end = pBaseAttribute.vertexStreamIndex;
		}
	}

	bool CVertexPipelineConfigBuilder::addStandardAttribute(
			EStandardVertexAttributeID pAttributeID,
			uint32 pStreamIndex,
			uint32 pStreamRelativeOffset )
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

		return addAttribute( std::move( attributeDefinition ) );
	}

	SharedHandle<CVertexPipelineConfig> CVertexPipelineConfigBuilder::reset( SharedHandle<CVertexPipelineConfig> pPipelineConfig )
	{
		_pipelineConfig = pPipelineConfig ? std::move( pPipelineConfig ) : createDynamicInterfaceObject<CVertexPipelineConfig>();
		return _pipelineConfig;
	}

/*
	GCI::IAInputLayoutDefinition CVertexPipelineConfigBuilder::generateGpaInputLayoutDefinition() const noexcept
	{
		GCI::IAInputLayoutDefinition gpaInputLayoutDefinition;
		gpaInputLayoutDefinition.primitiveTopology = _pipelineConfig->_primitiveTopology;
		gpaInputLayoutDefinition.activeAttributesMask = _pipelineConfig->_attribLayout.activeAttributesMask;

		for( uint32 iAttribute = 0; iAttribute < _attributeFormatArray.attributeArraySize; ) // No increment intentional
		{
			const auto & baseAttributeFormat = _attributeFormatArray.attributeFormat( iAttribute );
			if( baseAttributeFormat.isBaseAttribute() )
			{
				auto & gpaAttributeInfo = gpaInputLayoutDefinition.attributeArray[iAttribute];
				gpaAttributeInfo.streamIndex = baseAttributeFormat.streamIndex;
				gpaAttributeInfo.semanticName = baseAttributeFormat.semantics.smtName;
				gpaAttributeInfo.semanticIndex = 0;
				gpaAttributeInfo.format = baseAttributeFormat.componentFormat;
				gpaAttributeInfo.relativeOffset = baseAttributeFormat.streamElementRelativeOffset;
				gpaAttributeInfo.instanceRate = baseAttributeFormat.instanceRate;

				if( baseAttributeFormat.componentsNum > 1 )
				{
					auto * attributeSubComponent = baseAttributeFormat.nextComponent;
					auto attributeSubComponentIndex = 1;

					while( attributeSubComponent )
					{
						auto & gpaSubAttributeInfo = gpaInputLayoutDefinition.attributeArray[iAttribute + attributeSubComponentIndex];
						gpaAttributeInfo.streamIndex = baseAttributeFormat.streamIndex;
						gpaAttributeInfo.semanticName = baseAttributeFormat.semantics.smtName;
						gpaAttributeInfo.semanticIndex = numeric_cast<GCI::input_assembler_index_t>( attributeSubComponentIndex );
						gpaAttributeInfo.format = baseAttributeFormat.componentFormat;
						gpaAttributeInfo.instanceRate = baseAttributeFormat.instanceRate;
						gpaAttributeInfo.relativeOffset =
								baseAttributeFormat.streamElementRelativeOffset + attributeSubComponentIndex * baseAttributeFormat.componentSizeInBytes;

						attributeSubComponent = attributeSubComponent->nextComponent;
						attributeSubComponentIndex += 1;
					}
				}

				iAttribute += baseAttributeFormat.componentsNum;
			}
		}

		return gpaInputLayoutDefinition;
	}
*/
	bool CVertexPipelineConfigBuilder::validateAttributeDefinition(
			SVertexAttributeDefinition & pAttributeDefinition,
			uint32 pComponentStrideInBytes )
	{
		//
		if( !pAttributeDefinition.valid() )
		{
			return false;
		}

		if( !checkAttributeSlotRangeFree( pAttributeDefinition.baseAttributeIndex, pAttributeDefinition.componentsNum ) )
		{
			return false;
		}

		// // Each vertex stream is either supplied at vertex rate or an instance rate. Therefore, all attributes within
		// // the same stream must share that property. Stream data rate is defined when its first attribute is specified.
		// if( !checkAttributeVertexStreamCompatibility( pAttributeDefinition.streamIndex, pAttributeDefinition.instanceRate ) )
		// {
		// 	return false;
		// }

		const auto combinedAttributeDataStride = pComponentStrideInBytes * pAttributeDefinition.componentsNum;
		if( ( pAttributeDefinition.dataStride > 0 ) && ( pAttributeDefinition.dataStride < combinedAttributeDataStride ) )
		{
			false;
		}

		if( !resolveAttributeSemanticsDefinition( pAttributeDefinition.semantics ) )
		{
			return false;
		}

		if( pAttributeDefinition.vertexStreamRelativeOffset >= GCI::CxDef::IA_VERTEX_ATTRIBUTE_OFFSET_APPEND16 )
		{
			const auto & sourceVertexStream = _pipelineConfig->_streamLayout[pAttributeDefinition.vertexStreamIndex];
			if( pAttributeDefinition.vertexStreamRelativeOffset == GCI::CxDef::IA_VERTEX_ATTRIBUTE_OFFSET_APPEND )
			{
				pAttributeDefinition.vertexStreamRelativeOffset = sourceVertexStream.elementStrideInBytes;
			}
			else if( pAttributeDefinition.vertexStreamRelativeOffset == GCI::CxDef::IA_VERTEX_ATTRIBUTE_OFFSET_APPEND16 )
			{
				pAttributeDefinition.vertexStreamRelativeOffset = memGetAlignedValue( sourceVertexStream.elementStrideInBytes, 16 );
			}
		}

		if( pAttributeDefinition.dataStride == 0 )
		{
			pAttributeDefinition.dataStride = combinedAttributeDataStride;
		}

		return true;
	}

	bool CVertexPipelineConfigBuilder::resolveAttributeSemanticsDefinition(
			SShaderSemantics & pAttributeSemantics )
	{
		if( !pAttributeSemantics.resolve() )
		{
			return false;
		}

		if( _pipelineConfig->_attribLayout.isSemanticsActive( pAttributeSemantics ) )
		{
			return false;
		}

		return true;
	}

//	bool CVertexPipelineConfigBuilder::checkAttributeVertexStreamCompatibility(
//			uint32 pStreamIndex,
//			uint32 pInstanceDataRate ) const noexcept
//	{
//		auto & vertexStreamFormat = _pipelineConfig->_streamLayout[pStreamIndex];
//
//		// Compatible if it's either undefined or has the same data rate as the attribute specified.
//		return ( vertexStreamFormat.instanceDataRate == GCI::CxDef::IA_VERTEX_STREAM_INSTANCE_RATE_UNDEFINED ) ||
//		       ( vertexStreamFormat.instanceDataRate == pInstanceDataRate );
//	}
//
//	void CVertexPipelineConfigBuilder::updateVertexStreamState( uint32 pBaseAttributeIndex )
//	{
//	}

}
