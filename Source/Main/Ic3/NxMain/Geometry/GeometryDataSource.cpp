
#include "GeometryDataSource.h"

namespace Ic3
{

	XXGeometryInputSourceBase::XXGeometryInputSourceBase() = default;

	XXGeometryInputSourceBase::~XXGeometryInputSourceBase() = default;

	bool XXGeometryInputSourceBase::addVertexAttribute(
			ShaderSemantics pAttributeSemantics,
			uint32 pBufferIndex,
			GCI::EVertexAttribFormat pBaseFormat,
			uint32 pComponentsNum,
			uint32 pBufferRelativeOffset )
	{
		if( auto * existingAttribute = getAttributeDesc( pAttributeSemantics ) )
		{
			return false;
		}

		if( pBufferIndex >= GCM::kIAMaxDataStreamVertexBuffersNum )
		{
			return false;
		}

		if( ( pComponentsNum > 4 ) || ( _activeAttributeSlotsNum + pComponentsNum >= GCM::kIAMaxVertexAttributesNum ) )
		{
			return false;
		}

		auto & targetBuffer = _localVertexBufferArray[pBufferIndex];

		if( pBufferRelativeOffset == GCI::kIAVertexAttributeOffsetAppend )
		{
			pBufferRelativeOffset = targetBuffer.elementSizeInBytes;
		}

		SGeometryInputAttributeInfo attribDesc;
		attribDesc.attributeBaseFormat = pBaseFormat;
		attribDesc.attributeComponentsNum = pComponentsNum;
		attribDesc.attributeSizeInBytes = GCI::CXU::GetVertexAttribFormatByteSize( pBaseFormat ) * pComponentsNum;
		attribDesc.bufferIndex = pBufferIndex;
		attribDesc.bufferRelativeOffset = pBufferRelativeOffset;
		attribDesc.mSemantics = std::move( pAttributeSemantics );
		std::string attributeSemanticName = attribDesc.mSemantics.smtName;

		targetBuffer.elementSizeInBytes += attribDesc.attributeSizeInBytes;

		_attributeMap.insert({ std::move( attributeSemanticName ), std::move( attribDesc ) });
		_activeAttributeSlotsNum += pComponentsNum;

		return true;
	}

	bool XXGeometryInputSourceBase::addVertexAttribute(
			EStandardVertexAttributeID pStandardAttribute,
			GCI::EVertexAttribFormat pBaseFormat,
			uint32 pBufferRelativeOffset )
	{
	}

	void XXGeometryInputSourceBase::setIndexed( GCI::EIndexDataFormat pIndexFormat )
	{
		if( !_allocState.empty() )
		{
			Ic3Throw( 0 );
		}

		if( pIndexFormat != GCI::EIndexDataFormat::Undefined )
		{
			_localIndexBuffer.indexElementFormat = pIndexFormat;
			_localIndexBuffer.elementSizeInBytes = GCI::CXU::GetIndexDataFormatByteSize( pIndexFormat );
		}
		else
		{
			_localIndexBuffer.indexElementFormat = GCI::EIndexDataFormat::Undefined;
			_localIndexBuffer.elementSizeInBytes = 0;
		}
	}

	void XXGeometryInputSourceBase::reserveStorage( uint32 pVertexElementsNum, uint32 pIndexElementsNum  )
	{
		if( _localIndexBuffer.active() )
		{
			if( pIndexElementsNum > _allocState.indexElementsCapacity )
			{
				_localIndexBuffer.bufferCapacityInBytes = _localIndexBuffer.elementSizeInBytes * pIndexElementsNum;
				_localIndexBuffer.buffer.resize( _localIndexBuffer.bufferCapacityInBytes );
			}
		}
		else if( _localIndexBuffer.bufferCapacityInBytes > 0 )
		{
			_localIndexBuffer.bufferCapacityInBytes = 0;
			_localIndexBuffer.buffer.release();
		}

		for( auto & localVB : _localVertexBufferArray )
		{
			if( localVB.active() )
			{
				if( pVertexElementsNum > _allocState.vertexElementsCapacity )
				{
					localVB.bufferCapacityInBytes = localVB.elementSizeInBytes * pVertexElementsNum;
					localVB.buffer.resize( localVB.bufferCapacityInBytes );
				}
			}
			else if( localVB.bufferCapacityInBytes > 0 )
			{
				localVB.bufferCapacityInBytes = 0;
				localVB.buffer.release();
			}
		}

		if( pIndexElementsNum > _allocState.indexElementsCapacity )
		{
			_allocState.indexElementsCapacity = pIndexElementsNum;
		}

		if( pVertexElementsNum > _allocState.vertexElementsCapacity )
		{
			_allocState.vertexElementsCapacity = pVertexElementsNum;
		}
	}

	const GeometryInputDataLocation * XXGeometryInputSourceBase::addGeometry( uint32 pVertexElementsNum, uint32 pIndexElementsNum )
	{
		if( pVertexElementsNum == 0 )
		{
			Ic3Throw( 0 );
		}

		if( ( pIndexElementsNum == 0 ) && isIndexedGeometry() )
		{
			Ic3Throw( 0 );
		}

		const auto currentCapacity = getCurrentCapacity();
		if( ( pVertexElementsNum > currentCapacity.vertexElementsNum ) || ( pIndexElementsNum > currentCapacity.indexElementsNum ) )
		{
			reserveStorage(
				_allocState.vertexElementsAllocOffset + pVertexElementsNum,
				_allocState.indexElementsAllocOffset + pIndexElementsNum );
		}

		auto & geometryReference = _geometryRefList.emplace_back();
		geometryReference.geometryIndex = _geometryRefList.size() - 1;
		geometryReference.indexBufferOffsetInElementsNum = _allocState.indexElementsAllocOffset;
		geometryReference.indexElementsNum = pIndexElementsNum;
		geometryReference.vertexBufferOffsetInElementsNum = _allocState.vertexElementsAllocOffset;
		geometryReference.vertexElementsNum = pVertexElementsNum;

		_allocState.indexElementsAllocOffset += pIndexElementsNum;
		_allocState.vertexElementsAllocOffset += pVertexElementsNum;

		return &geometryReference;
	}

}
