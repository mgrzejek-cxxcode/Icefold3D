
#pragma once

#ifndef __IC3_NXMAIN_MESH_DATA_H__
#define __IC3_NXMAIN_MESH_DATA_H__

#include "GeometryContainer.h"
#include <cppx/memoryBuffer.h>

namespace Ic3
{

	class Mesh;
	class MeshGroup;
	class MeshLoader;

	struct MeshSubComponentData
	{
		std::string name;

		uint32 componentIndex;

		GeometryDataReferenceBase geometryDataRef;

		MeshSubComponentData( const GeometryDataFormatBase & pDataFormat )
		: geometryDataRef( pDataFormat )
		{}
	};

	class MeshData : public GeometryContainer
	{
	public:
		MeshData( const GeometryDataFormatBase & pGeometryDataFormatBase )
		: GeometryContainer( pGeometryDataFormatBase )
		{}

		CPPX_ATTR_NO_DISCARD uint32 getMeshSubComponentsNum() const noexcept;

		CPPX_ATTR_NO_DISCARD const std::string & getMeshName() const noexcept;

		CPPX_ATTR_NO_DISCARD const MeshSubComponentData * getMeshSubComponentData( uint32 pIndex ) const noexcept;

		CPPX_ATTR_NO_DISCARD const void * getVertexStreamBaseDataPtr( uint32 pIndex ) const noexcept;

		CPPX_ATTR_NO_DISCARD const void * getIndexBaseDataPtr() const noexcept;

		CPPX_ATTR_NO_DISCARD InterleavedBufferElementRefReadOnly getIndexDataSubRegionReadOnly(
				const CPUGeometryDataReferenceBase & pMeshDataRef ) const noexcept;

		CPPX_ATTR_NO_DISCARD InterleavedBufferElementRefReadOnly getVertexAttributeDataSubRegionReadOnly(
				const CPUGeometryDataReferenceBase & pMeshDataRef,
				uint32 pAttributeIndex ) const noexcept;

		CPPX_ATTR_NO_DISCARD InterleavedBufferElementRefReadWrite getIndexDataSubRegionReadWrite(
				const CPUGeometryDataReferenceBase & pMeshDataRef ) noexcept;

		CPPX_ATTR_NO_DISCARD InterleavedBufferElementRefReadWrite getVertexAttributeDataSubRegionReadWrite(
				const CPUGeometryDataReferenceBase & pMeshDataRef,
				uint32 pAttributeIndex ) noexcept;

		void initializeStorage( uint32 pVertexElementsNum, uint32 pIndexElementsNum );

		void setMeshName( std::string pName );

		MeshSubComponentData * addMeshComponent( uint32 pVertexElementsNum, uint32 pIndexElementsNum );

	private:
		using VertexDataBufferArray = std::array<dynamic_memory_buffer, gpa::MAX_GEOMETRY_VERTEX_STREAMS_NUM>;
		GeometrySize _geometrySize;
		std::string _meshName;
		dynamic_memory_buffer _indexDataBuffer;
		VertexDataBufferArray _vertexDataBuffers;
		std::vector<MeshSubComponentData> _meshSubComponents;
	};

	inline uint32 MeshData::getMeshSubComponentsNum() const noexcept
	{
		return static_cast<uint32>( _meshSubComponents.size() );
	}

	inline const std::string & MeshData::getMeshName() const noexcept
	{
		return _meshName;
	}

	inline const MeshSubComponentData * MeshData::getMeshSubComponentData( uint32 pIndex ) const noexcept
	{
		return ( pIndex < _meshSubComponents.size() ) ? &( _meshSubComponents[pIndex] ) : nullptr;
	}

	inline const void * MeshData::getVertexStreamBaseDataPtr( uint32 pIndex ) const noexcept
	{
		return _vertexDataBuffers[pIndex].data();
	}

	inline const void * MeshData::getIndexBaseDataPtr() const noexcept
	{
		return _indexDataBuffer.data();
	}


	class MeshGroupData
	{
	public:
		GeometryDataFormatBase const mGeometryDataFormatBase;

	public:
		MeshGroupData( const GeometryDataFormatBase & pGeometryDataFormatBase )
		: mGeometryDataFormatBase( pGeometryDataFormatBase )
		{}

		CPPX_ATTR_NO_DISCARD uint32 getMeshesNum() const noexcept
		{
			return static_cast<uint32>( _meshDataArray.size() );
		}

		CPPX_ATTR_NO_DISCARD const GeometrySize & geometrySize() const noexcept
		{
			return _geometrySize;
		}

		CPPX_ATTR_NO_DISCARD bool empty() const noexcept
		{
			return _meshDataArray.empty();
		}

		CPPX_ATTR_NO_DISCARD const MeshData * getMeshData( uint32 pIndex ) const noexcept
		{
			return ( pIndex < _meshDataArray.size() ) ? _meshDataArray[pIndex].get() : nullptr;
		}

		MeshData & addMeshData( std::unique_ptr<MeshData> pMeshData );

		MeshData & addMeshData( uint32 pVertexElementsNum, uint32 pIndexElementsNum );

	private:
		using MeshDataPtr = std::unique_ptr<MeshData>;
		using MeshDataArray = std::vector<MeshDataPtr>;
		GeometrySize _geometrySize;
		MeshDataArray _meshDataArray;
	};

	struct VertexDefaultP3N3T
	{
		cxm::vec3f position;
		cxm::vec3f normal;
		cxm::vec4f color;
		cxm::vec2f texCoord;
		cxm::vec3f tangent;
		cxm::vec3f biTangent;
	};

//	namespace geom
//	{
//
//		template <typename TVertex, typename TIndex>
//		inline void generateTangentsAndBiNormals( MeshDataFixed<TVertex, TIndex> & pMeshData )
//		{
//			auto * vertexArray = reinterpret_cast<TVertex *>( pMeshData.verticesData.data() );
//
//			for( native_uint iVertex = 0; iVertex < pMeshData.verticesNum; ++iVertex )
//			{
//				cxm::vec3f & vTangent = vertexArray[iVertex].tangent;
//				cxm::vec3f & vBiNormal = vertexArray[iVertex].biTangent;
//				vTangent = { 0.0f, 0.0f, 0.0f };
//				vBiNormal = { 0.0f, 0.0f, 0.0f };
//				continue;
//			}
//
//			const auto * indexArray = reinterpret_cast<const TIndex *>( pMeshData.indicesData.data() );
//
//			for( native_uint iIndex = 0; iIndex < pMeshData.indicesNum; iIndex += 3 )
//			{
//				const auto faceIndex0 = indexArray[iIndex + 0];
//				const auto faceIndex1 = indexArray[iIndex + 1];
//				const auto faceIndex2 = indexArray[iIndex + 2];
//
//				const cxm::vec3f & vPosition0 = vertexArray[faceIndex0].position;
//				const cxm::vec3f & vPosition1 = vertexArray[faceIndex1].position;
//				const cxm::vec3f & vPosition2 = vertexArray[faceIndex2].position;
//
//				const cxm::vec2f & vTexCoord0 = vertexArray[faceIndex0].texCoord;
//				const cxm::vec2f & vTexCoord1 = vertexArray[faceIndex1].texCoord;
//				const cxm::vec2f & vTexCoord2 = vertexArray[faceIndex2].texCoord;
//
//				const cxm::vec3f & vNormal0 = vertexArray[faceIndex0].normal;
//				const cxm::vec3f & vNormal1 = vertexArray[faceIndex1].normal;
//				const cxm::vec3f & vNormal2 = vertexArray[faceIndex2].normal;
//
//				const cxm::vec3f dPosition1 = vPosition1 - vPosition0;
//				const cxm::vec3f dPosition2 = vPosition2 - vPosition0;
//
//				const cxm::vec2f dTexCoord1 = vTexCoord1 - vTexCoord0;
//				const cxm::vec2f dTexCoord2 = vTexCoord2 - vTexCoord0;
//
//				// Calculate tangent and bitangent
//				const auto r = 1.0f / ( dTexCoord1.x * dTexCoord2.y - dTexCoord1.y * dTexCoord2.x );
//				const auto tangent = ( dPosition1 * dTexCoord2.y - dPosition2 * dTexCoord1.y ) * r;
//				const auto biTangent = ( dPosition2 * dTexCoord1.x - dPosition1 * dTexCoord2.x ) * r;
//
//
//				cxm::vec3f & vTangent0 = vertexArray[faceIndex0].tangent;
//				cxm::vec3f & vTangent1 = vertexArray[faceIndex1].tangent;
//				cxm::vec3f & vTangent2 = vertexArray[faceIndex2].tangent;
//
//				cxm::vec3f & vBiNormal0 = vertexArray[faceIndex0].biTangent;
//				cxm::vec3f & vBiNormal1 = vertexArray[faceIndex1].biTangent;
//				cxm::vec3f & vBiNormal2 = vertexArray[faceIndex2].biTangent;
//
//				vTangent0 += cxm::normalize(tangent - vNormal0 * cxm::dot( vNormal0, tangent ) );
//				vTangent1 += cxm::normalize(tangent - vNormal1 * cxm::dot( vNormal1, tangent ) );
//				vTangent2 += cxm::normalize(tangent - vNormal2 * cxm::dot( vNormal2, tangent ) );
//
//				vBiNormal0 += cxm::normalize( biTangent - vNormal0 * cxm::dot( vNormal0, biTangent ) );
//				vBiNormal1 += cxm::normalize( biTangent - vNormal1 * cxm::dot( vNormal1, biTangent ) );
//				vBiNormal2 += cxm::normalize( biTangent - vNormal2 * cxm::dot( vNormal2, biTangent ) );
//			}
//
//			for( native_uint iVertex = 0; iVertex < pMeshData.verticesNum; ++iVertex )
//			{
//				cxm::vec3f & vTangent = vertexArray[iVertex].tangent;
//				cxm::vec3f & vBiNormal = vertexArray[iVertex].biTangent;
//				vTangent = cxm::normalize( vTangent );
//				vBiNormal = cxm::normalize( vBiNormal );
//				continue;
//			}
//		}
//
//	}

} // namespace Ic3

#endif // __IC3_NXMAIN_MESH_DATA_H__
