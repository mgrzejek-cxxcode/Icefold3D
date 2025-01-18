
#include "GeometryDataTransfer.h"
#include "CVertexPipelineConfig.h"
#include "GeometryStorageGPU.h"

namespace Ic3
{

	GeometryDataGPUTransfer::GeometryDataGPUTransfer(
			CoreEngineState & pCES,
			GpaTransferCommandContext & pTransferCmdContext )
	: CoreEngineObject( pCES )
	, mTransferCmdContext( pTransferCmdContext )
	{}

	GeometryDataGPUTransfer::~GeometryDataGPUTransfer() = default;


	GeometryDataGPUTransferUpload::GeometryDataGPUTransferUpload(
			CoreEngineState & pCES,
			GpaTransferCommandContext & pTransferCmdContext )
	: GeometryDataGPUTransfer( pCES, pTransferCmdContext )
	{}

	GeometryDataGPUTransferUpload::~GeometryDataGPUTransferUpload() = default;

	void GeometryDataGPUTransferUpload::initializeMeshData(
			const GeometryReference & pGeometryRef,
			const GeometryDataReferenceBase & pInputDataRef )
	{
		for( auto iVertexStream : pGeometryRef.dataReference.dataFormat->activeVertexStreams() )
		{
			const auto & vertexStreamFormat = pGeometryRef.dataReference.dataFormat->vertexStream( iVertexStream );
			const auto & inputDataRegion = pInputDataRef.vertexStreamDataRegions[iVertexStream];
			const auto & vertexBufferRegion = pGeometryRef.dataReference.vertexStreamDataRegions[iVertexStream];

			const auto vertexBuffer = pGeometryRef.storage->getVertexBuffer( iVertexStream );

			GCI::GPUBufferSubDataUploadDesc gpaDataUploadDesc;
			gpaDataUploadDesc.bufferRegion.offset = vertexBufferRegion.offsetInElementsNum * vertexStreamFormat.elementSizeInBytes;
			gpaDataUploadDesc.bufferRegion.size = vertexBufferRegion.sizeInElementsNum * vertexStreamFormat.elementSizeInBytes;
			gpaDataUploadDesc.inputDataDesc.mPointer = inputDataRegion.mDataPtr;
			gpaDataUploadDesc.inputDataDesc.size = inputDataRegion.sizeInElementsNum * inputDataRegion.elementSize;

			mTransferCmdContext.updateBufferSubDataUpload( *vertexBuffer, gpaDataUploadDesc );
		}

		if( pGeometryRef.dataReference.dataFormat->isIndexedGeometry() )
		{
			const auto & inputDataRegion = pInputDataRef.indexDataRegion;
			const auto & indexBufferRegion = pGeometryRef.dataReference.indexDataRegion;

			const auto indexBuffer = pGeometryRef.storage->getIndexBuffer();

			GCI::GPUBufferSubDataUploadDesc gpaDataUploadDesc;
			gpaDataUploadDesc.bufferRegion.offset = indexBufferRegion.offsetInElementsNum * pGeometryRef.dataReference.dataFormat->indexElementSizeInBytes();
			gpaDataUploadDesc.bufferRegion.size = indexBufferRegion.sizeInElementsNum * pGeometryRef.dataReference.dataFormat->indexElementSizeInBytes();
			gpaDataUploadDesc.inputDataDesc.mPointer = inputDataRegion.mDataPtr;
			gpaDataUploadDesc.inputDataDesc.size = inputDataRegion.sizeInElementsNum * inputDataRegion.elementSize;

			mTransferCmdContext.updateBufferSubDataUpload( *indexBuffer, gpaDataUploadDesc );
		}
	}

}
