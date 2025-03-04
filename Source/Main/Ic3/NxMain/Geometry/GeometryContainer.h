
#pragma once

#ifndef __IC3_NXMAIN_GEOMETRY_CONTAINER_H__
#define __IC3_NXMAIN_GEOMETRY_CONTAINER_H__

#include "../GCI/VertexFormatSignature.h"
#include "GeometryBuffer.h"

namespace Ic3
{

	Ic3DeclareInterfaceHandle( IGeometryContainer );
	Ic3DeclareInterfaceHandle( IGeometryDataStorage );

	using geometry_ref_key_t = uint64;

	struct GeometryReference
	{
		IGeometryContainer * container = nullptr;

		geometry_ref_key_t geometryRefKey = 0;
	};

	/*
	 * !!!
	 * Renaming:
	 * - GeometryStorage -> GeometryDataStore/Storage - manages storage
	 * - GeometryContainer -> GeometryDataContainer - manages objects that uses allocated/external memory (storage)
	 * !!!
	 */

	class IGeometryContainer
	{
	public:
		VertexDataInputConfigHandle const mVDIConfig;

	public:
		IGeometryContainer( VertexDataInputConfigHandle pVDIConfig );
		virtual ~IGeometryContainer();

		CPPX_ATTR_NO_DISCARD virtual IGeometryDataStorage * getStorage() const noexcept = 0;

		virtual bool setStorage( IGeometryStorageHandle pStorage ) = 0;

		GeometryReference addExternalGeometry( const GeometryInstanceExternalDataDesc & pDataDesc )
		{
			const auto geometryRefKey = _refKeyCounter++;
			auto & geometryData = _geometryInstanceMap[geometryRefKey];
			geometryData.memoryRef.storage = nullptr;

			GeometryInstanceData geometryInstanceData;

			const auto & activeVertexStreams = mVDIConfig->g                                                                                                                                                                                                                                                                                                                                                   etActiveStreamIndexList();
			for( auto activeVertexStreamIndex : activeVertexStreams )
			{
				const auto & externalMemoryDesc = pDataDesc.vertexDataDescArray[activeVertexStreamIndex];
				auto & vertexStreamMemoryRef = geometryInstanceData.memoryRef.vertexBufferMemoryRefArray[activeVertexStreamIndex];
				vertexStreamMemoryRef.bufferIndex = activeVertexStreamIndex;
				vertexStreamMemoryRef.internalMemoryRef = externalMemoryDesc.memoryAddress;
				vertexStreamMemoryRef.memoryRegion.offset = 0u;
				vertexStreamMemoryRef.memoryRegion.size = externalMemoryDesc.elementStrideInBytes * externalMemoryDesc.elementsNum;

			}
		}

		GeometryReference allocateGeometry( const SGeometryInstanceAllocationDesc & pAllocDesc );

	protected:
		struct GeometryInstanceData
		{
			GeometryInstanceMemoryRef memoryRef;
		};

	private:
		using GeometryInstanceMap = std::unordered_map<geometry_ref_key_t, GeometryInstanceData>;
		GeometryInstanceMap _geometryInstanceMap;
		geometry_ref_key_t _refKeyCounter = 0x00000400;
	};

	class GeometryDataSource : public IGeometryContainer
	{
	public:
	};

	class MeshContainer : public IGeometryContainer
	{
	private:
		IGeometryStorageManagedHandle _internalManagedStorage;
	};

	struct GeometrySizeMetrics
	{
		uint32 indexDataSizeInElementsNum;
		uint32 indexElementSizeInBytes;
		GCI::EIndexDataFormat indexElementType;
		uint32 vertexDataSizeInElementsNum;
	};

	struct GeometryStorageCapacity
	{
		uint32 indexElementsNum = 0;
		uint32 vertexElementsNum = 0;
	};

	struct GeometrySharedStorageMetrics
	{
		GeometryStorageCapacity capacity;
		GeometrySize geometrySize;
		uint32 indexDataAllocationOffsetInElementsNum = 0;
		uint32 vertexDataAllocationOffsetInElementsNum = 0;
	};

	class GeometryDataStoreBase
	{
	public:
		GeometryDataFormatBase const mDataFormat;

	public:
		virtual GeometryBufferDataRefReadOnly getIndexDataPtrReadOnly() const noexcept = 0;

		virtual GeometryBufferDataRefReadWrite getIndexDataPtrReadWrite() = 0;

		virtual GeometryBufferDataRefReadOnly getVertexDataPtrReadOnly( size_t pVertexStreamIndex ) const noexcept = 0;

		virtual GeometryBufferDataRefReadWrite getVertexDataPtrReadWrite( size_t pVertexStreamIndex ) = 0;

		GeometryBufferDataRefReadOnly getVertexAttributeDataSubRegionReadOnly() const noexcept;

		GeometryBufferDataRefReadWrite getVertexAttributeDataSubRegionReadWrite();
	};

	template <typename TBufferData, size_t tVertexStreamUsageSize>
	class GeometryDataStore : public GeometryDataStoreBase
	{
	};

	template <typename TBufferData>
	class GeometryDataStore<TBufferData, 0> : public GeometryDataStoreBase
	{
	};

	class GeometryDataSource
	{
	public:
		GeometryDataSource();
		~GeometryDataSource();

	private:
		GeometryReference _geometryRef;
	};

	class GeometryContainer2
	{
	public:
		const GeometryDataFormatBase & mDataFormat;

	public:
		GeometryContainer2( const GeometryDataFormatBase & pDataFormat );

		CPPX_ATTR_NO_DISCARD bool isStorageInitialized() const noexcept;

		CPPX_ATTR_NO_DISCARD bool isIndexedGeometryContainer() const noexcept;

		CPPX_ATTR_NO_DISCARD const GeometryDataReferenceBase & getAllGeometryDataRef() const noexcept;

		CPPX_ATTR_NO_DISCARD const GeometryContainerStorageMetrics & getStorageMetrics() const noexcept;

		CPPX_ATTR_NO_DISCARD const GeometrySize & getGeometrySize() const noexcept;

	protected:
		bool checkStorageAvailableSpace( uint32 pVertexElementsNum, uint32 pIndexElementsNum ) const noexcept;

		void setContainerCapacity( uint32 pVertexElementsNum, uint32 pIndexElementsNum );

		void updateStoredGeometrySize( uint32 pVertexElementsNum, uint32 pIndexElementsNum );

	protected:
		GeometryContainerStorageMetrics _storageMetrics;
		GeometryDataReferenceBase _allGeometryDataRef;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_GEOMETRY_CONTAINER_H__
