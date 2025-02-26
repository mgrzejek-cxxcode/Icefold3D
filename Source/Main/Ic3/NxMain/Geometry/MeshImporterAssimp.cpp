
#include "MeshLoader.h"
#include "dataTypesConv.h"
#include "GeometryVertexFormat.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/StringUtils.h>

namespace Ic3
{

	namespace aidetail
	{

		std::unique_ptr<MeshData> assimpImportScene( const aiScene * pAiScene, const GeometryDataFormatBase & pGeometryDataFormatBase );

	}

	std::unique_ptr<MeshData> MeshImporterAssimp::importMesh(
			const std::string & pFilename,
			const GeometryDataFormatBase & pGeometryDataFormatBase )
	{
		Assimp::Importer aImporter;

		auto * aiSceneObject = aImporter.ReadFile(
				pFilename.c_str(),
				aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_FlipWindingOrder | aiProcess_GenSmoothNormals | aiProcess_Triangulate );

		if( !aiSceneObject )
		{
			Ic3DebugInterrupt();
			throw 0;
		}

		return aidetail::assimpImportScene( aiSceneObject, pGeometryDataFormatBase );
	}


	namespace aidetail
	{

		struct AssimpMeshMetrics
		{
			uint32 meshComponentsNum;
			uint32 indexElementsNum;
			uint32 vertexElementsNum;
		};

		AssimpMeshMetrics getAssimpMeshSizeMetrics( const aiScene * pAiScene )
		{
			AssimpMeshMetrics metrics;
			metrics.meshComponentsNum = pAiScene->mNumMeshes;
			metrics.indexElementsNum = 0;
			metrics.vertexElementsNum = 0;

			for( native_uint iMesh = 0; iMesh < pAiScene->mNumMeshes; ++iMesh )
			{
				const auto * aiMesh = pAiScene->mMeshes[iMesh];
				metrics.indexElementsNum += aiMesh->mNumFaces * 3;
				metrics.vertexElementsNum += aiMesh->mNumVertices;
			}

			return metrics;
		}

		template <typename TPInput>
		void assimpReadMeshVertexAttributeData(
				const TPInput * pInputData,
				const size_t pElementsNum,
				const InterleavedBufferElementRefReadWrite & pWriteRegion,
				const VertexAttributeFormat & pAttributeFormat,
				const DataTypeConversionFunction & pConversionFunction )
		{
			const auto vertexAttributeComponentsNum = GCI::CXU::getVertexAttribFormatComponentsNum( pAttributeFormat.componentFormat );

			auto * currentWritePtr = pWriteRegion.basePtr;

			for( native_uint iElement = 0; iElement < pElementsNum; ++iElement )
			{
				const auto & aiAttributeData = pInputData[iElement];

				pConversionFunction( &aiAttributeData, currentWritePtr, vertexAttributeComponentsNum );

				currentWritePtr += pWriteRegion.strideInBytes;
			}
		}

		void assimpImportMesh( const aiMesh * pAiMesh, MeshData & pMeshData )
		{
			auto * meshSubComponentData = pMeshData.addMeshComponent( pAiMesh->mNumVertices, pAiMesh->mNumFaces * 3 );
			meshSubComponentData->name = std::string( pAiMesh->mName.data, pAiMesh->mName.length );

			if( pAiMesh->mNumFaces > 0 )
			{
				const auto indexDataWriteRegion = pMeshData.getIndexDataSubRegionReadWrite( meshSubComponentData->geometryDataRef );
				const auto conversionFunction = gmutil::GetGeometryConversionFunction<Math::Vec3u32>( GCI::EBaseDataType::Uint32 );

				auto * currentWritePtr = indexDataWriteRegion.basePtr;

				for( native_uint iFace = 0; iFace < pAiMesh->mNumFaces; ++iFace )
				{
					const auto & vFace = pAiMesh->mFaces[iFace];
					conversionFunction( vFace.mIndices, currentWritePtr, 3 );
					currentWritePtr += indexDataWriteRegion.strideInBytes * 3;
				}
			}

			for( uint32 iAttribute = 0; iAttribute < gpa::MAX_GEOMETRY_VERTEX_ATTRIBUTES_NUM; ++iAttribute )
			{
				if( pMeshData.mDataFormat.IsAttributeActive( iAttribute ) )
				{
					const auto & attributeFormat = pMeshData.mDataFormat.attribute( iAttribute );
					const auto attributeDataBaseType = GCI::CXU::getVertexAttribFormatBaseDataType( attributeFormat.componentFormat );
					const auto attributeDataWriteRegion = pMeshData.getVertexAttributeDataSubRegionReadWrite( meshSubComponentData->geometryDataRef, iAttribute );

					switch( attributeFormat.mSemantics.mSmtID )
					{
						case EShaderInputSemanticID::Position:
						{
							const auto conversionFunction = gmutil::GetGeometryConversionFunction<Math::Vec3d>( attributeDataBaseType );

							assimpReadMeshVertexAttributeData(
									pAiMesh->mVertices,
									pAiMesh->mNumVertices,
									attributeDataWriteRegion,
									attributeFormat,
									conversionFunction );

							const auto p3 = ( P3SVertex* )pMeshData.getVertexStreamBaseDataPtr(0);

							break;
						}
						case EShaderInputSemanticID::Normal:
						{
							const auto conversionFunction = gmutil::GetGeometryConversionFunction<Math::Vec3d>( attributeDataBaseType );

							assimpReadMeshVertexAttributeData(
									pAiMesh->mNormals,
									pAiMesh->mNumVertices,
									attributeDataWriteRegion,
									attributeFormat,
									conversionFunction );

							const auto p3 = ( P3SVertex * )pMeshData.getVertexStreamBaseDataPtr( 0 );

							break;
						}
						case EShaderInputSemanticID::TexCoord0:
						{
							const auto conversionFunction = gmutil::GetGeometryConversionFunction<Math::Vec3d>( attributeDataBaseType );

							assimpReadMeshVertexAttributeData(
									pAiMesh->mTextureCoords[0],
									pAiMesh->mNumVertices,
									attributeDataWriteRegion,
									attributeFormat,
									conversionFunction );

							const auto p3 = ( P3SVertex * )pMeshData.getVertexStreamBaseDataPtr( 0 );

							break;
						}
						default:
						{
							break;
						}
					}
				}
			}
		}

		std::unique_ptr<MeshData> assimpImportScene( const aiScene * pAiScene, const GeometryDataFormatBase & pGeometryDataFormatBase )
		{
			const auto meshSizeMetrics = getAssimpMeshSizeMetrics( pAiScene );

			auto meshData = std::make_unique<MeshData>( pGeometryDataFormatBase );
			meshData->initializeStorage( meshSizeMetrics.indexElementsNum, meshSizeMetrics.vertexElementsNum );

			for( native_uint iMesh = 0; iMesh < pAiScene->mNumMeshes; ++iMesh )
			{
				const auto * aiMesh = pAiScene->mMeshes[iMesh];
				assimpImportMesh( aiMesh, *meshData );
			}

			return meshData;
		}

	}

}
