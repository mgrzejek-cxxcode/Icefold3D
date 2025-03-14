
#pragma once

#ifndef __IC3_NXMAIN_MESH_GROUP_H__
#define __IC3_NXMAIN_MESH_GROUP_H__

#include "Mesh.h"
#include "../Geometry/CVertexPipelineConfig.h"
#include <unordered_map>

namespace Ic3
{

	class Mesh;
	class MeshComponent;
	class MeshGroup;

	struct MeshGroupInfo
	{
		uint32 meshesNum;
		uint32 indexElementsNum = 0;
		uint32 vertexElementsNum = 0;
	};

	class MeshGroup
	{
		friend class MeshComponent;

	public:
		std::string const mGroupName;

		GeometryDataFormatBase const mGeometryDataFormatBase;

	public:
		MeshGroup( std::string pGroupName, GeometryStorage * pGeometryStorage );
		~MeshGroup();

		GeometryStorage & getGeometryStorage() const noexcept;

		Mesh * getMesh( size_t pIndex ) const noexcept;

		Mesh * findMesh( const std::string & pName ) const noexcept;

		MeshComponent * findMeshComponent( const std::string & pName ) const noexcept;

		MeshComponent * findMeshSubComponent( const Mesh & pMesh, const std::string & pName ) const noexcept;

		Mesh * addMesh( std::string pName );

	private:
		void registerMeshComponent( MeshComponent & pMeshComponent );

	private:
		using MeshStorage = std::deque<Mesh>;
		using MeshList = std::vector<Mesh *>;
		using MeshName = std::string_view;
		using MeshComponentMap = std::unordered_map<MeshName, MeshComponent *>;

		MeshStorage _meshStorage;
		MeshList _meshes;
		MeshComponentMap _meshComponents;
		GeometryStorage * _geometryStorage;
	};

	inline GeometryStorage & MeshGroup::getGeometryStorage() const noexcept
	{
		return *_geometryStorage;
	}

} // namespace Ic3

#endif // __IC3_NXMAIN_MESH_GROUP_H__
