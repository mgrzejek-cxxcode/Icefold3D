
#ifndef __IC3_NXMAIN_HARDWARE_BUFFER_MANAGER_H__
#define __IC3_NXMAIN_HARDWARE_BUFFER_MANAGER_H__

#include "HWBuffer.h"
#include <Ic3/Cppx/Memory.h>

namespace Ic3
{

	using gpuapi_buffer_ref_id_t = uint64;

	struct HWBufferCreateInfo
	{
		union Metrics
		{
			HWBufferMetrics uGeneric;
			HWVertexBufferMetrics uVertexBuffer;
			HWIndexBufferMetrics uIndexBuffer;
		};

		EHWBufferType baseType = EHWBufferType::HBTUnknown;

		TBitmask<hardware_buffer_flags_value_t> flags = 0;

		Metrics metrics;

		GCI::GpuBufferInitDataDesc initData;

		HWBufferCreateInfo()
		{
			memZero( metrics );
		}
	};

	class HWBufferManager
	{
	public:
		struct GpuBufferState
		{
			using HWBufferList = std::list<HWBufferWeakHandle>;
			gpuapi_buffer_ref_id_t bufferRefID;
			GCI::GpuBufferHandle gpuBuffer;
			HWBufferList allocatedHWBufferList;
			GCI::gpu_memory_size_t availableMemorySize;
			GCI::gpu_memory_size_t currentAllocOffset;
		};

		ResultCode allocateGpuBufferExplicit( gpuapi_buffer_ref_id_t pGpuBufferRefID,
											  const GCI::GpuBufferCreateInfo & pGpuBufferCreateInfo );

		HWVertexBufferHandle createVertexBuffer( const HWBufferCreateInfo & pHWBCreateInfo );

		HWVertexBufferHandle createVertexBufferEx( gpuapi_buffer_ref_id_t pGBUBufferRefID,
		                                           const HWBufferCreateInfo & pHWBCreateInfo );

		HWIndexBufferHandle createIndexBuffer( const HWBufferCreateInfo & pHWBCreateInfo );

		HWIndexBufferHandle createIndexBufferEx( gpuapi_buffer_ref_id_t pGBUBufferRefID,
		                                         const HWBufferCreateInfo & pHWBCreateInfo );

		IC3_ATTR_NO_DISCARD GpuBufferUsageInfo getGpuBufferInfo( gpuapi_buffer_ref_id_t pGpuBufferRefID ) const;

		static memory_align_t queryAlignmentRequirementsForBuffer( EHWBufferType pBufferType,
		                                                           GCI::gpu_memory_size_t pBufferSize,
		                                                           TBitmask<GCI::gpu_memory_flags_value_t> pMemoryFlags = 0 );

	private:
		GCI::GpuBufferHandle _createGpuBuffer( gpuapi_buffer_ref_id_t pGpuBufferRefID,
		                                          const HWBufferCreateInfo & pHWBCreateInfo );

		GpuBufferRef _reserveGpuBufferRegion( gpuapi_buffer_ref_id_t pGpuBufferRefID,
		                                      GCI::gpu_memory_size_t pSize,
		                                      memory_align_t pAlignment = 0 );

		static void _validateBufferCreateInfo( EHWBufferType pBufferType, HWBufferCreateInfo & pHWBCreateInfo );

	private:
		using GpuBufferMap = std::unordered_map<gpuapi_buffer_ref_id_t, GpuBufferState>;
		GCI::GpuDeviceHandle _gpuDevice;
		GpuBufferMap _gpuBufferMap;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_HARDWARE_BUFFER_MANAGER_H__
