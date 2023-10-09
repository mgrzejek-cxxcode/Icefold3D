
#pragma once

#ifndef __IC3_NXMAIN_GEOMETRY_STORAGE_H__
#define __IC3_NXMAIN_GEOMETRY_STORAGE_H__

#include "GeometryContainer.h"
#include <Ic3/Graphics/GCI/State/InputAssemblerImmutableStates.h>
#include <deque>

namespace Ic3
{

	enum class EGeometryBufferAllocationMode : enum_default_value_t
	{
		AllocLocal,
		ShareExternal
	};

	struct GeometryStorageCreateInfo
	{
		struct GeometryBufferDesc
		{
			EGeometryBufferAllocationMode allocationMode = EGeometryBufferAllocationMode::AllocLocal;
			EGPUBufferUsagePolicy bufferUsagePolicy = EGPUBufferUsagePolicy::Undefined;
		};

		EGPUBufferUsagePolicy commonBufferUsagePolicy = EGPUBufferUsagePolicy::Undefined;

		const GeometryDataFormatBase * dataFormat;

		GeometryContainerStorageCapacity capacity;

		GeometryBufferDesc indexBufferDesc;

		GeometryVertexStreamGenericArray<GeometryBufferDesc> vertexBufferDescArray;
	};

	template <size_t tVertexStreamArraySize>
	class GeometrySharedStorage : public GeometryContainer<tVertexStreamArraySize>
	{
	public:
		using GeometryRefList = std::deque<GeometryReference>;

	public:
		GeometrySharedStorage( const GeometryDataFormatBase & pDataFormat );

		virtual ~GeometrySharedStorage() = default;

		GCI::IAVertexStreamImmutableStateHandle getGpaVertexStreamState() const noexcept;

		SharedGeometryRefHandle addIndexedGeometry( uint32 pVertexElementsNum, uint32 pIndexElementsNum );

		SharedGeometryRefHandle addNonIndexedGeometry( uint32 pVertexElementsNum );

		void releaseStorage() {}

		static GeometryStoragePtr createStorage(
				const CoreEngineState & pCES,
				const GeometryStorageCreateInfo & pCreateInfo,
				const GeometryStorage * pSharedStorage = nullptr );

	private:
		void createStorageGPUBuffers( const CoreEngineState & pCES, const GeometryStorageCreateInfo & pCreateInfo );

		void bindSharedGPUBuffers( const GeometryStorageCreateInfo & pCreateInfo, const GeometryStorage & pSharedStorage );

		void initializeVertexStreamState( const CoreEngineState & pCES );

		GeometryReference * addGeometry( uint32 pVertexElementsNum, uint32 pIndexElementsNum );

		IC3_ATTR_NO_DISCARD static GCI::GPUBufferHandle createIndexBuffer(
				const CoreEngineState & pCES,
				GCI::gpu_memory_size_t pBufferSize,
				EGPUBufferUsagePolicy pUsagePolicy );

		IC3_ATTR_NO_DISCARD static GCI::GPUBufferHandle createVertexBuffer(
				const CoreEngineState & pCES,
				GCI::gpu_memory_size_t pBufferSize,
				EGPUBufferUsagePolicy pUsagePolicy );

		IC3_ATTR_NO_DISCARD static bool validateSharedBuffersConfiguration(
				const GeometryStorageCreateInfo & pCreateInfo,
				const GeometryStorage & pSharedStorage );

		IC3_ATTR_NO_DISCARD static EGPUBufferUsagePolicy resolveGeometryBufferUsagePolicy(
				EGPUBufferUsagePolicy pBufferUsagePolicy,
				EGPUBufferUsagePolicy pCommonUsagePolicy );

	private:
		GeometryReferenceList _geometryRefList;
		GCI::GPUBufferHandle _indexBuffer;
		GeometryVertexStreamGenericArray<GCI::GPUBufferHandle> _vertexBufferArray;
		Bitmask<GCI::EIAVertexStreamBindingFlags> _vertexStreamBindingMask;
		GCI::IAVertexStreamImmutableStateHandle _gpaVertexStreamState;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_GEOMETRY_STORAGE_H__
