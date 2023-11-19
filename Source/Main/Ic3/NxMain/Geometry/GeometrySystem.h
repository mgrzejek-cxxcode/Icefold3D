
#pragma once

#ifndef __IC3_NXMAIN_GEOMETRY_MANAGER_H__
#define __IC3_NXMAIN_GEOMETRY_MANAGER_H__

#include "GeometryVertexFormat.h"

namespace Ic3
{

	/*
	 * class GPUResourceContainer : public CoreEngineObject
	{
	public:
		GPUResourceContainer( const CoreEngineState & pCES )
		: CoreEngineObject( pCES )
		{}
	};

	class GeometryManager : public GPUResourceContainer
	{
	public:
		GeometryManager( const CoreEngineState & pCES );
		virtual ~GeometryManager();

		GeometryStorage * createGeometryStorage(
				const GeometryStorageCreateInfo & pCreateInfo,
				const GeometryStorage * pSharedStorage = nullptr );

		GeometryStorage * createNamedGeometryStorage(
				std::string pUniqueStorageName,
				const GeometryStorageCreateInfo & pCreateInfo,
				const GeometryStorage * pSharedStorage = nullptr );

		GeometryStorage * getStorageByIndex( uint32 pStorageIndex ) const noexcept;

		GeometryStorage * getStorageByName( const std::string & pStorageName ) const noexcept;

	private:
		GeometryStorage * createStorageInternal(
				const GeometryStorageCreateInfo & pCreateInfo,
				const GeometryStorage * pSharedStorage );

	private:
		using GeometryStorageList = std::vector<GeometryStoragePtr>;
		using NamedGeometryStorageMap = std::map<std::string, GeometryStorage *>;

		GeometryStorageList _storages;

		NamedGeometryStorageMap _namedStoragesMap;
	};
	 */

} // namespace Ic3

#endif // __IC3_NXMAIN_GEOMETRY_MANAGER_H__
