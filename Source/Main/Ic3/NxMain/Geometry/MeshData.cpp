
#include "MeshData.h"
#include "CVertexPipelineConfig.h"
#include <Ic3/NxMain/Res/Image/pngCommon.h>


namespace Ic3
{

	InterleavedBufferElementRefReadWrite MeshData::getIndexDataSubRegionReadWrite(
			const CPUGeometryDataReferenceBase & pMeshDataRef ) noexcept
	{
		return InterleavedBufferElementRefReadWrite {
				_indexDataBuffer.data() + ( pMeshDataRef.indexDataRegion.offsetInElementsNum * pMeshDataRef.indexDataRegion.elementSize ),
				mDataFormat.indexElementSizeInBytes(),
				mDataFormat.indexElementSizeInBytes()
		};
	}

	InterleavedBufferElementRefReadWrite MeshData::getVertexAttributeDataSubRegionReadWrite(
			const CPUGeometryDataReferenceBase & pMeshDataRef,
			uint32 pAttributeIndex ) noexcept
	{
		const auto & attributeFormat = mDataFormat.attribute( pAttributeIndex );
		const auto & vertexStreamDataRef = pMeshDataRef.vertexStreamDataRegions[attributeFormat.mStreamIndex];

		auto * bufferBasePtr = _vertexDataBuffers[attributeFormat.mStreamIndex].data();

		return InterleavedBufferElementRefReadWrite {
				bufferBasePtr + ( vertexStreamDataRef.offsetInElementsNum * vertexStreamDataRef.elementSize ) + attributeFormat.streamElementRelativeOffset,
				attributeFormat.attributeTotalSizeInBytes,
				vertexStreamDataRef.elementSize
		};
	}

	void MeshData::initializeStorage( uint32 pVertexElementsNum, uint32 pIndexElementsNum )
	{
		setContainerCapacity( pVertexElementsNum, pIndexElementsNum );

		for( auto iVertexStream : mDataFormat.activeVertexStreams() )
		{
			const auto vertexStreamElementSize = mDataFormat.vertexStreamElementSizeInBytes( iVertexStream );
			const auto vertexStreamDataBufferSize = pVertexElementsNum * vertexStreamElementSize;
			_vertexDataBuffers[iVertexStream].resize( vertexStreamDataBufferSize );
			_allGeometryDataRef.vertexStreamDataRegions[iVertexStream].mDataPtr = _vertexDataBuffers[iVertexStream].data();
		}

		if( pIndexElementsNum > 0 )
		{
			const auto _indexDataBufferSize = pIndexElementsNum * mDataFormat.indexElementSizeInBytes();
			_indexDataBuffer.resize( _indexDataBufferSize );
			_allGeometryDataRef.indexDataRegion.mDataPtr = _indexDataBuffer.data();
		}
	}

	void MeshData::setMeshName( std::string pName )
	{
		_meshName = std::move( pName );
	}

	MeshSubComponentData * MeshData::addMeshComponent( uint32 pVertexElementsNum, uint32 pIndexElementsNum )
	{
		if( !checkStorageAvailableSpace( pVertexElementsNum, pIndexElementsNum ) )
		{
			return nullptr;
		}

		auto & subComponentData = _meshSubComponents.emplace_back( mDataFormat );
		subComponentData.componentIndex = cppx::numeric_cast<uint32>( _meshSubComponents.size() - 1 );

		if( subComponentData.componentIndex == 0 )
		{
			subComponentData.geometryDataRef = gmutil::getGeometryDataReferenceBaseSubRegion(
					_allGeometryDataRef,
					0, pVertexElementsNum,
					0, pIndexElementsNum );
		}
		else
		{
			const auto & previousSubComponent = _meshSubComponents[subComponentData.componentIndex - 1];
			subComponentData.geometryDataRef = gmutil::advanceGeometryDataReferenceBase(
					previousSubComponent.geometryDataRef,
					pVertexElementsNum,
					pIndexElementsNum );
		}

		updateStoredGeometrySize( pVertexElementsNum, pIndexElementsNum );

		return &( _meshSubComponents.back() );
	}


	MeshData & MeshGroupData::addMeshData( std::unique_ptr<MeshData> pMeshData )
	{
		_geometrySize.append( pMeshData->getGeometrySize() );
		_meshDataArray.push_back( std::move( pMeshData ) );

		return *( _meshDataArray.back() );
	}

	MeshData & MeshGroupData::addMeshData( uint32 pVertexElementsNum, uint32 pIndexElementsNum )
	{
		auto meshData = std::make_unique<MeshData>( mGeometryDataFormatBase );
		meshData->initializeStorage( pVertexElementsNum, pIndexElementsNum );

		return addMeshData( std::move( meshData ) );
	}

}
