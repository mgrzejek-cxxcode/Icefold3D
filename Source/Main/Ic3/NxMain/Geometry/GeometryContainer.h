
#pragma once

#ifndef __IC3_NXMAIN_GEOMETRY_CONTAINER_H__
#define __IC3_NXMAIN_GEOMETRY_CONTAINER_H__

#include "VertexPipelineConfig.h"
#include "GeometryBuffer.h"

namespace Ic3
{

	ic3DeclareInterfaceHandle( IGeometryContainer );
	ic3DeclareInterfaceHandle( IGeometryStorage );

	struct SGeometryReference
	{
		IGeometryContainer * container = nullptr;
		uint32 geometryIndex = 0;
	};

	class IGeometryContainer
	{
	public:
		IGeometryContainer();
		virtual ~IGeometryContainer();

		IC3_ATTR_NO_DISCARD virtual IGeometryStorage * getStorage() const noexcept = 0;

		virtual bool setStorage( IGeometryStorageHandle pStorage ) = 0;

	protected:
		CVertexAttributeArrayLayout _vertexAttributeLayout;
	};

	class CGeometryDataSource : public IGeometryContainer
	{
	public:
	private:
		IGeometryStorageHandle _internalStorage;
	};

	class CMeshContainer : public IGeometryContainer
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

	template <typename TBufferData, size_t tVertexStreamArraySize>
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

		IC3_ATTR_NO_DISCARD bool isStorageInitialized() const noexcept;

		IC3_ATTR_NO_DISCARD bool isIndexedGeometryContainer() const noexcept;

		IC3_ATTR_NO_DISCARD const GeometryDataReferenceBase & getAllGeometryDataRef() const noexcept;

		IC3_ATTR_NO_DISCARD const GeometryContainerStorageMetrics & getStorageMetrics() const noexcept;

		IC3_ATTR_NO_DISCARD const GeometrySize & getGeometrySize() const noexcept;

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
