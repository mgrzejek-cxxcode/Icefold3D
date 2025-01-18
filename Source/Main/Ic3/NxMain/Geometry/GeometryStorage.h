
#pragma once

#ifndef __IC3_NXMAIN_GEOMETRY_STORAGE_H__
#define __IC3_NXMAIN_GEOMETRY_STORAGE_H__

#include "VertexPipelineConfig.h"
#include <deque>
#include <unordered_map>

namespace Ic3
{

	/// @brief
	enum class EGeometryMemoryPool : enum_default_value_t
	{
		SysMem,
		VRAM,
	};

	/// @brief
	enum EGeometryStoragePropertyFlags : uint32
	{
		/// The storage has been correctly initialized. All buffers have been
		/// created/allocated and match the internal storage specification.
		eGeometryStoragePropertyFlagStateInitializedBit = 0x0001,

		///
		eGeometryStoragePropertyMaskStateAll = 0x000F,

		///
		eGeometryStoragePropertyFlagFeatureContinuousStorageBit = 0x0010,

		///
		eGeometryStoragePropertyFlagFeatureDirectMemoryAccessBit = 0x0020,

		///
		eGeometryStoragePropertyMaskFeatureAll = 0x00F0,
	};


	/// @brief
	class IC3_NXMAIN_CLASS IGeometryDataStorage
	{
	public:
		IGeometryDataStorage() = default;
		virtual ~IGeometryDataStorage() = default;


		CPPX_ATTR_NO_DISCARD virtual EGeometryMemoryPool getMemoryPoolType() const noexcept = 0;

		CPPX_ATTR_NO_DISCARD virtual bool hasDirectMemoryAccess() const noexcept = 0;

		CPPX_ATTR_NO_DISCARD virtual GeometryBufferDataRefReadOnly getBufferPtrReadOnly( const GeometryStorageMemoryRef & pMemoryRef ) const noexcept = 0;

		CPPX_ATTR_NO_DISCARD virtual GeometryBufferDataRefReadWrite getBufferPtrReadWrite( const GeometryStorageMemoryRef & pMemoryRef ) const noexcept = 0;

		CPPX_ATTR_NO_DISCARD virtual GeometryMemoryReference allocateGeometryMemory( const GeometryMemoryAllocationDesc & pAllocationDesc ) = 0;


		CPPX_ATTR_NO_DISCARD bool isCpuMemoryStorage() const noexcept;

		CPPX_ATTR_NO_DISCARD bool isGPUMemoryStorage() const noexcept;

		CPPX_ATTR_NO_DISCARD bool isIndexBufferUsed() const noexcept;

		CPPX_ATTR_NO_DISCARD cppx::bitmask<EGCIGeometryBufferFlags> getActiveBuffersMask() const noexcept;

		CPPX_ATTR_NO_DISCARD uint32 getActiveVertexBuffersNum() const noexcept;

		CPPX_ATTR_NO_DISCARD bool isVertexBufferActive( size_t pVBufferIASlot ) const noexcept;


		CPPX_ATTR_NO_DISCARD virtual GeometryBufferDataRefReadOnly acquireIndexDataPtrReadOnly() const noexcept = 0;

		CPPX_ATTR_NO_DISCARD virtual GeometryBufferDataRefReadWrite acquireIndexDataPtrReadWrite() = 0;

		CPPX_ATTR_NO_DISCARD virtual GeometryBufferDataRefReadOnly acquireVertexDataPtrReadOnly( size_t pVertexBufferIndex ) const noexcept = 0;

		CPPX_ATTR_NO_DISCARD virtual GeometryBufferDataRefReadWrite acquireVertexDataPtrReadWrite( size_t pVertexBufferIndex ) = 0;

	protected:
		struct InternalGeometryBufferState : public GeometryBufferInfo
		{
		};

		//using GeometryMemoryRefMap = std::unordered_map<RGeometryReferenceKey, SInternalGeometryInstanceMemoryReference>;

	protected:
		cppx::bitmask<EGCIGeometryBufferFlags> _activeBuffersMask;
		uint32 _activeBuffersNum = 0;
		//GeometryMemoryRefMap _memoryRefMap;
	};

	/// @brief
	class IC3_NXMAIN_CLASS IGeometryDataStorageCpu : public IGeometryDataStorage
	{
	public:
		CPPX_ATTR_NO_DISCARD virtual EGeometryMemoryPool getMemoryPoolType() const noexcept override final;

		CPPX_ATTR_NO_DISCARD virtual bool hasDirectMemoryAccess() const noexcept override final;
	};

	/// @brief
	class IC3_NXMAIN_CLASS IGeometryDataStorageGPU : public IGeometryDataStorage
	{
	public:
		CPPX_ATTR_NO_DISCARD virtual EGeometryMemoryPool getMemoryPoolType() const noexcept override final;
	};

	struct GeometryDataStorageAllocProxyTraits
	{
		using IndexBufferState = GeometryBufferInfo;
		using VertexBufferState = GeometryBufferInfo;
	};

	template <typename TBaseStorage, typename TProxyTraits>
	class GeometryDataStorageBase : public TBaseStorage
	{
	public:
		using Traits = TProxyTraits;
		using IndexBufferState = typename TProxyTraits::IndexBufferState;
		using VertexBufferState = typename TProxyTraits::VertexBufferState;

		struct GeometryInstanceMemoryRef
		{
			GeometryStorageMemoryRef indexBufferMemRef;
			RGeometryVertexBufferGenericArray<GeometryStorageMemoryRef> vertexBufferMemRefArray;
		};

	};

} // namespace Ic3

#endif // __IC3_NXMAIN_GEOMETRY_STORAGE_H__
