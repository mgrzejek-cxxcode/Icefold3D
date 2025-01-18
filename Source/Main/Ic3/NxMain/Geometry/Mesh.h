
#pragma once

#ifndef __IC3_NXMAIN_MESH_H__
#define __IC3_NXMAIN_MESH_H__

#include "MeshData.h"
#include <deque>

namespace Ic3
{

	enum class EMeshObjectType : enum_default_value_t
	{
		Mesh,
		SubComponent
	};

	class MeshComponent
	{
	public:
		MeshComponent( Mesh & pParentMesh, std::string pName, GeometryRefHandle pGeometryRef );
		virtual ~MeshComponent();

		CPPX_ATTR_NO_DISCARD virtual EMeshObjectType getMeshObjectType() const noexcept;

		CPPX_ATTR_NO_DISCARD bool isMeshSubComponent() const noexcept;

		CPPX_ATTR_NO_DISCARD Mesh * parentMesh() const noexcept;
		CPPX_ATTR_NO_DISCARD MeshGroup * meshGroup() const noexcept;
		CPPX_ATTR_NO_DISCARD const std::string & name() const noexcept;
		CPPX_ATTR_NO_DISCARD GeometryRefHandle geometryDataRef() const noexcept;

	protected:
		MeshComponent( MeshGroup & pMeshGroup, std::string pName, GeometryRefHandle pGeometryRef );

	protected:
		MeshGroup * _meshGroup;
		Mesh * _parentMesh;
		std::string _name;
		GeometryRefHandle _geometryRef;
	};

	inline bool MeshComponent::isMeshSubComponent() const noexcept
	{
		return getMeshObjectType() == EMeshObjectType::SubComponent;
	}

	inline Mesh * MeshComponent::parentMesh() const noexcept
	{
		return _parentMesh;
	}

	inline MeshGroup * MeshComponent::meshGroup() const noexcept
	{
		return _meshGroup;
	}

	inline const std::string & MeshComponent::name() const noexcept
	{
		return _name;
	}

	inline GeometryRefHandle MeshComponent::geometryDataRef() const noexcept
	{
		return _geometryRef;
	}


	class Mesh : public MeshComponent
	{
	public:
		Mesh( MeshGroup & pMeshGroup, std::string pName );
		virtual ~Mesh();

		CPPX_ATTR_NO_DISCARD virtual EMeshObjectType getMeshObjectType() const noexcept override final;

		CPPX_ATTR_NO_DISCARD uint32 getComponentsNum() const noexcept;

		CPPX_ATTR_NO_DISCARD MeshComponent * getComponent( uint32 pIndex ) const noexcept;

		CPPX_ATTR_NO_DISCARD MeshComponent * findSubComponent( const std::string & pName ) const noexcept;

		MeshComponent * addMeshComponent( std::string pName, GeometryRefHandle pGeometryRef );

	private:
		using MeshSubComponents = std::deque<MeshComponent>;
		using MeshSubComponentsIndex = std::vector<MeshComponent *>;
		MeshSubComponents _meshSubComponents;
		MeshSubComponentsIndex _meshSubComponentsIndex;
		GeometryReference _meshGeometryRef;
	};

	inline uint32 Mesh::getComponentsNum() const noexcept
	{
		return static_cast<uint32>( _meshSubComponents.size() );
	}

	inline MeshComponent * Mesh::getComponent( uint32 pIndex ) const noexcept
	{
		return ( pIndex < _meshSubComponentsIndex.size() ) ? _meshSubComponentsIndex[pIndex] : nullptr;
	}

} // namespace Ic3

#endif // __IC3_NXMAIN_MESH_H__
