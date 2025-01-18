
#pragma once

#ifndef __IC3_NXMAIN_GEOMETRY_MANAGER_H__
#define __IC3_NXMAIN_GEOMETRY_MANAGER_H__

#include "GeometryStorage.h"

namespace Ic3
{

	/// @brief
	struct GeometryStorageCreateInfo
	{
		struct GeometryStorageBufferCreateInfo
		{
			EGPUBufferUsagePolicy bufferUsagePolicy = EGPUBufferUsagePolicy::Undefined;

			uint16 baseAlignment = 0;

			uint16 elementStrideInBytes = 0;

			uint32 sizeInBytes = 0;
		};

		struct GeometryStorageBufferShareInfo
		{
			GeometryBufferHandle buffer;

			MemoryRegion sharedRange;
		};

		union BufferInitDesc
		{
			GeometryStorageBufferCreateInfo uCreateInfo;
			GeometryStorageBufferShareInfo uShareInfo;
		};

		EGPUBufferUsagePolicy commonBufferUsagePolicy = EGPUBufferUsagePolicy::GeometryBufferStatic;

		BufferInitDesc indexBufferInitDesc;

		RGeometryVertexBufferGenericArray<BufferInitDesc> vertexBufferInitDescArray;
	};

	class GPUResourceContainer : public CoreEngineObject
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

} // namespace Ic3

#endif // __IC3_NXMAIN_GEOMETRY_MANAGER_H__
