
#ifndef __IC3_NXMAIN_COMMON_GPA_DEFS_H__
#define __IC3_NXMAIN_COMMON_GPA_DEFS_H__

#include "../Prerequisites.h"
#include <Ic3/Graphics/GCI/Resources/GPUBufferCommon.h>

namespace Ic3
{

	enum class EGPUBufferUsagePolicy : uint64
	{
		Undefined = 0,

		ConstantBufferDynamic =
				GCI::E_GPU_BUFFER_BIND_FLAG_CONSTANT_BUFFER_BIT |
				GCI::E_GPU_RESOURCE_CONTENT_FLAG_DYNAMIC_BIT |
				bshLeft<uint64, 32>( GCI::E_GPU_MEMORY_ACCESS_FLAG_CPU_WRITE_BIT ),

		ConstantBufferStatic =
				GCI::E_GPU_BUFFER_BIND_FLAG_CONSTANT_BUFFER_BIT |
				GCI::E_GPU_RESOURCE_CONTENT_FLAG_STATIC_BIT,

		ConstantBufferWriteDiscard =
				GCI::E_GPU_BUFFER_BIND_FLAG_CONSTANT_BUFFER_BIT |
				GCI::E_GPU_RESOURCE_CONTENT_FLAG_DYNAMIC_BIT |
				bshLeft<uint64, 32>( GCI::E_GPU_MEMORY_MAP_FLAG_WRITE_INVALIDATE_BIT ),

		GeometryBufferDynamic =
				GCI::E_GPU_BUFFER_BIND_FLAG_INDEX_BUFFER_BIT |
				GCI::E_GPU_BUFFER_BIND_FLAG_VERTEX_BUFFER_BIT |
				GCI::E_GPU_RESOURCE_CONTENT_FLAG_DYNAMIC_BIT |
				bshLeft<uint64, 32>( GCI::E_GPU_MEMORY_ACCESS_FLAG_CPU_WRITE_BIT ),

		GeometryBufferImmutable =
				GCI::E_GPU_BUFFER_BIND_FLAG_INDEX_BUFFER_BIT |
				GCI::E_GPU_BUFFER_BIND_FLAG_VERTEX_BUFFER_BIT |
				GCI::E_GPU_RESOURCE_CONTENT_FLAG_IMMUTABLE_BIT,

		GeometryBufferStatic =
				GCI::E_GPU_BUFFER_BIND_FLAG_INDEX_BUFFER_BIT |
				GCI::E_GPU_BUFFER_BIND_FLAG_VERTEX_BUFFER_BIT |
				GCI::E_GPU_RESOURCE_CONTENT_FLAG_STATIC_BIT,

		GeometryBufferStreamAppend =
				GCI::E_GPU_BUFFER_BIND_FLAG_INDEX_BUFFER_BIT |
				GCI::E_GPU_BUFFER_BIND_FLAG_VERTEX_BUFFER_BIT |
				GCI::E_GPU_RESOURCE_CONTENT_FLAG_DYNAMIC_BIT |
				bshLeft<uint64, 32>( GCI::E_GPU_MEMORY_MAP_FLAG_WRITE_APPEND_BIT ),

		GeometryBufferStreamOverwrite =
				GCI::E_GPU_BUFFER_BIND_FLAG_INDEX_BUFFER_BIT |
				GCI::E_GPU_BUFFER_BIND_FLAG_VERTEX_BUFFER_BIT |
				GCI::E_GPU_RESOURCE_CONTENT_FLAG_DYNAMIC_BIT |
				bshLeft<uint64, 32>( GCI::E_GPU_MEMORY_MAP_FLAG_WRITE_INVALIDATE_BIT ),
	};

	namespace CxDefs
	{

		IC3_ATTR_NO_DISCARD inline constexpr uint16 getIndexDataFormatByteSize( GCI::EIndexDataFormat pIndexFormat ) noexcept
		{
			return GCI::CxDefs::getIndexDataFormatByteSize( pIndexFormat );
		}

		inline constexpr GCI::gpu_memory_flags_value_t getGPUBufferUsagePolicyMemoryFlags( EGPUBufferUsagePolicy pUsagePolicy )
		{
			return static_cast<GCI::gpu_memory_flags_value_t>( ( ( ( uint64 ) pUsagePolicy ) >> 32 ) & Limits<uint32>::maxValue );
		}

		inline constexpr GCI::resource_flags_value_t getGPUBufferUsagePolicyResourceFlags( EGPUBufferUsagePolicy pUsagePolicy )
		{
			return static_cast<GCI::resource_flags_value_t>( ( ( uint64 ) pUsagePolicy ) & Limits<uint32>::maxValue );
		}

	}
	
//	/// @brief Represents a valid GPUBuffer subregion. This is basically a sub-buffer of a larger buffer.
//	struct GPUBufferRef
//	{
//		// The buffer itself.
//		GCI::GPUBufferHandle buffer;
//
//		// Memory region of the buffer. Note, that the offset is not buffer-relative, but heap-relative.
//		// For an example, a GPUBufferRef referencing the whole memory of a buffer B1, would have here
//		// exactly same values as stored in B1.mResourceMemory.sourceHeapRegion.
//		GCI::GPUMemoryRegion memoryRegion;
//
//		// The actual region reserved to make the allocation. This is due to alignment requirements: it is
//		// possible to sub-allocate a buffer's memory using a more strict alignment than the original buffer
//		// has. To prevent extra calculation in the client code, memoryRegion always has the exact size which
//		// was requested and the actual size of the region is stored here.
//		GCI::GPUMemoryRegion reservedRegion;
//
//		GPUBufferRef() = default;
//
//		GPUBufferRef( std::nullptr_t )
//		: buffer{ nullptr }
//		, memoryRegion{ 0, GCI::CxDefs::GPU_MEMORY_SIZE_MAX }
//		{}
//
//		explicit operator bool() const
//		{
//			return buffer && !memoryRegion.empty();
//		}
//	};
//
//	struct GPUBufferUsageInfo
//	{
//		// Offset to the beginning of the free region of a buffer's memory.
//		// This is a buffer-relative offset: initially its value is zero.
//		GCI::gpu_memory_size_t currentAllocOffset;
//
//		// The size of a memory available for allocation (in bytes).
//		// Initially its value is the size of the whole GPUBuffer.
//		GCI::gpu_memory_size_t availableMemorySize;
//
//		// Number of HWBuffers allocated from the referenced GPUBuffer.
//		uint32 allocatedSubRegionCount;
//	};
//
//	struct HWBufferMetrics
//	{
//		GCI::gpu_memory_size_t bufferSize;
//	};
//
//	struct HWGeometryBufferMetrics : public HWBufferMetrics
//	{
//		uint32 instanceDataStepRate;
//		uint32 vertexCount;
//		uint32 vertexSize;
//	};
//
//	struct HWIndexBufferMetrics : public HWBufferMetrics
//	{
//		uint32 indexCount;
//		uint32 indexSize;
//	};

} // namespace Ic3

#endif // __IC3_NXMAIN_COMMON_GPA_DEFS_H__
