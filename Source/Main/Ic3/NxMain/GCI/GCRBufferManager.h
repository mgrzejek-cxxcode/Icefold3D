
#ifndef __IC3_NXMAIN_HARDWARE_BUFFER_MANAGER_H__
#define __IC3_NXMAIN_HARDWARE_BUFFER_MANAGER_H__

#include "GCRBuffer.h"
#include <cppx/memory.h>

namespace Ic3
{

	using gpuapi_buffer_ref_id_t = uint64;

	struct GCRBufferCreateInfo
	{
		union Metrics
		{
			GCRBufferMetrics uGeneric;
			GCRVertexBufferMetrics uVertexBuffer;
			GCRIndexBufferMetrics uIndexBuffer;
		};

		EGCRBufferType baseType = EGCRBufferType::HBTUnknown;

		cppx::bitmask<hardware_buffer_flags_value_t> flags = 0;

		Metrics metrics;

		GCI::GPUBufferInitDataDesc initData;

		GCRBufferCreateInfo()
		{
			mem_set_zero( metrics );
		}
	};

	class GCRBufferManager
	{
	public:
		struct GPUBufferState
		{
			using GCRBufferList = std::list<GCRBufferWeakHandle>;
			gpuapi_buffer_ref_id_t bufferRefID;
			GCI::GPUBufferHandle gpuBuffer;
			GCRBufferList allocatedGCRBufferList;
			GCI::gpu_memory_size_t availableMemorySize;
			GCI::gpu_memory_size_t currentAllocOffset;
		};

		ResultCode allocateGPUBufferExplicit( gpuapi_buffer_ref_id_t pGPUBufferRefID,
											  const GCI::GPUBufferCreateInfo & pGPUBufferCreateInfo );

		GCRVertexBufferHandle createVertexBuffer( const GCRBufferCreateInfo & pGCRBCreateInfo );

		GCRVertexBufferHandle createVertexBufferEx( gpuapi_buffer_ref_id_t pGBUBufferRefID,
		                                           const GCRBufferCreateInfo & pGCRBCreateInfo );

		GCRIndexBufferHandle createIndexBuffer( const GCRBufferCreateInfo & pGCRBCreateInfo );

		GCRIndexBufferHandle createIndexBufferEx( gpuapi_buffer_ref_id_t pGBUBufferRefID,
		                                         const GCRBufferCreateInfo & pGCRBCreateInfo );

		CPPX_ATTR_NO_DISCARD GPUBufferUsageInfo getGPUBufferInfo( gpuapi_buffer_ref_id_t pGPUBufferRefID ) const;

		static memory_align_t queryAlignmentRequirementsForBuffer( EGCRBufferType pBufferType,
		                                                           GCI::gpu_memory_size_t pBufferSize,
		                                                           cppx::bitmask<GCI::gpu_memory_flags_value_t> pMemoryFlags = 0 );

	private:
		GCI::GPUBufferHandle _createGPUBuffer( gpuapi_buffer_ref_id_t pGPUBufferRefID,
		                                          const GCRBufferCreateInfo & pGCRBCreateInfo );

		GPUBufferRef _reserveGPUBufferRegion( gpuapi_buffer_ref_id_t pGPUBufferRefID,
		                                      GCI::gpu_memory_size_t pSize,
		                                      memory_align_t pAlignment = 0 );

		static void _validateBufferCreateInfo( EGCRBufferType pBufferType, GCRBufferCreateInfo & pGCRBCreateInfo );

	private:
		using GPUBufferMap = std::unordered_map<gpuapi_buffer_ref_id_t, GPUBufferState>;
		GCI::GPUDeviceHandle _gpuDevice;
		GPUBufferMap _gpuBufferMap;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_HARDWARE_BUFFER_MANAGER_H__
