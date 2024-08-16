
#pragma once

#ifndef __IC3_NXMAIN_GEOMETRY_SYSTEM_H__
#define __IC3_NXMAIN_GEOMETRY_SYSTEM_H__

#include "VertexPipelineConfig.h"
#include <deque>
#include <unordered_map>

namespace Ic3
{

	enum EGeometryStoragePropertyFlags : uint32
	{
		/// The storage has been correctly initialized. All buffers have been
		/// created/allocated and match the internal storage specification.
		E_GEOMETRY_STORAGE_PROPERTY_FLAG_STATE_INITIALIZED_BIT = 0x0001,

		///
		E_GEOMETRY_STORAGE_PROPERTY_MASK_STATE_ALL = 0x000F,

		///
		E_GEOMETRY_STORAGE_PROPERTY_FLAG_FEATURE_CONTINUOUS_STORAGE_BIT = 0x0010,

		///
		E_GEOMETRY_STORAGE_PROPERTY_FLAG_FEATURE_DIRECT_MEMORY_ACCESS_BIT = 0x0020,

		///
		E_GEOMETRY_STORAGE_PROPERTY_FLAG_FEATURE_ALL = 0x00F0,
	};

	struct SGeometryBufferMemoryState
	{
		size_t sizeInBytes = 0;
		uint32 baseAlignment = 0;
		uintptr_t currentAllocationOffset = 0;
		uintptr_t freeSpace = 0;
	};

	struct SGeometryStorageIndexBufferState : public SGeometryBufferMemoryState
	{};

	struct SGeometryStorageVertexBufferState : public SGeometryBufferMemoryState
	{};

	struct SGeometryBufferAllocationDesc
	{
		uint32 elementSizeInBytes = 0;
		uint32 elementsNum = 0;
		uint32 explicitAllocationSize = 0;

		explicit operator bool() const noexcept
		{
			return ( elementSizeInBytes > 0 ) && ( elementsNum > 0 );
		}
	};

	/// @brief
	class IC3_NXMAIN_CLASS IGeometryStorage
	{
	public:
		IGeometryStorage() = default;
		virtual ~IGeometryStorage() = default;

		IC3_ATTR_NO_DISCARD virtual GeometryBufferDataRefReadOnly getBufferPtrReadOnly( const SGeometryBufferMemoryRef & pMemoryRef ) const noexcept = 0;

		IC3_ATTR_NO_DISCARD virtual GeometryBufferDataRefReadWrite getBufferPtrReadWrite( const SGeometryBufferMemoryRef & pMemoryRef ) const noexcept = 0;

		IC3_ATTR_NO_DISCARD virtual bool hasDirectMemoryAccess() const noexcept = 0;

		IC3_ATTR_NO_DISCARD virtual bool hasContinuousStorage() const noexcept = 0;

		IC3_ATTR_NO_DISCARD bool isIndexedGeometry() const noexcept;

		IC3_ATTR_NO_DISCARD uint32 getActiveVertexBuffersNum() const noexcept;

		IC3_ATTR_NO_DISCARD bool isVertexBufferActive( size_t pBufferIndex ) const noexcept;

	protected:
		Bitmask<GCI::EIAVertexStreamBindingFlags> _activeBuffersMask;
	};

	struct SManagedGeometryAllocationDesc
	{
		SGeometryBufferAllocationDesc indexDataAllocDesc;
		RGeometryVertexBufferGenericArray<SGeometryBufferAllocationDesc> vertexDataAllocDescArray;
	};

	struct SGeometrySharedMemoryDesc
	{
		uintptr_t memoryAddress = 0;
		uint32 elementSizeInBytes = 0;
		uint32 elementsNum = 0;

		explicit operator bool() const noexcept
		{
			return ( memoryAddress != 0 ) && ( elementSizeInBytes > 0 ) && ( elementsNum > 0 );
		}
	};

	struct SSharedGeometryDataDesc
	{
		SGeometrySharedMemoryDesc indexDataDesc;
		RGeometryVertexBufferGenericArray<SGeometrySharedMemoryDesc> vertexDataDescArray;
	};

	class IGeometryStorageManaged : public IGeometryStorage
	{
	public:
		virtual bool initializeIndexBuffer( size_t pBufferSizeInBytes, uint32 pAlignment ) = 0;

		virtual bool initializeVertexBuffer( size_t pBufferIndex, size_t pBufferSizeInBytes, uint32 pAlignment ) = 0;

		virtual void releaseIndexBuffer() = 0;

		virtual void releaseVertexBuffer( size_t pBufferIndex ) = 0;

		virtual void updateStorageInternal() = 0;

		IC3_ATTR_NO_DISCARD virtual SGeometryInstanceMemoryRef allocateGeometryMemory( const SGeometryBufferAllocationDesc & pAllocationDesc ) = 0;

		IC3_ATTR_NO_DISCARD virtual GeometryBufferDataRefReadOnly getIndexDataPtrReadOnly() const noexcept = 0;

		IC3_ATTR_NO_DISCARD virtual GeometryBufferDataRefReadWrite getIndexDataPtrReadWrite() = 0;

		IC3_ATTR_NO_DISCARD virtual GeometryBufferDataRefReadOnly getVertexDataPtrReadOnly( size_t pBufferIndex ) const noexcept = 0;

		IC3_ATTR_NO_DISCARD virtual GeometryBufferDataRefReadWrite getVertexDataPtrReadWrite( size_t pBufferIndex ) = 0;

		IC3_ATTR_NO_DISCARD const SGeometryStorageIndexBufferState & getIndexBufferState() const noexcept;

		IC3_ATTR_NO_DISCARD const SGeometryStorageVertexBufferState & getVertexBufferState( size_t pBufferIndex ) const noexcept;

	protected:
		using VertexBufferStateArray = std::array<SGeometryStorageVertexBufferState, GCM::IA_MAX_VERTEX_BUFFER_BINDINGS_NUM>;
		SGeometryStorageIndexBufferState _indexBufferState;
		VertexBufferStateArray _vertexBufferStateArray;
	};

	class IGeometryStorageShared : public IGeometryStorage
	{
	public:
		const SGeometryInstanceMemoryRef * addGeometry( const SSharedGeometryDataDesc & pGeometryDataDesc );
	};

	class CGeometryStorageSharedExternal : public IGeometryStorageShared
	{
	public:
		IC3_ATTR_NO_DISCARD virtual int addGeometrySharedData( const SSharedGeometryDataDesc & pSharedDataDesc ) = 0;
	};

	class CGeometryStorageSharedSubAlloc : public IGeometryStorageShared
	{
	public:
		IC3_ATTR_NO_DISCARD virtual SGeometryInstanceMemoryRef allocateGeometryMemory( const SGeometryBufferAllocationDesc & pAllocationDesc ) = 0;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_GEOMETRY_SYSTEM_H__
