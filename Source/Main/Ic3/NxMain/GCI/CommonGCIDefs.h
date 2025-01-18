
#ifndef __IC3_NXMAIN_COMMON_GPA_DEFS_H__
#define __IC3_NXMAIN_COMMON_GPA_DEFS_H__

#include "../Prerequisites.h"
#include <Ic3/Graphics/GCI/Resources/GPUBufferCommon.h>
#include <Ic3/Graphics/GCI/State/InputAssemblerCommon.h>

namespace Ic3
{

	namespace GCM = Graphics::GCM;

	enum class EGPUBufferUsagePolicy : uint64
	{
		Undefined = 0,

		ConstantBufferDynamic =
				GCI::eGPUBufferBindFlagConstantBufferBit |
				GCI::eGPUResourceContentFlagDynamicBit |
				cppx::bsh_left<uint64, 32>( GCI::eGPUMemoryAccessFlagCpuWriteBit ),

		ConstantBufferStatic =
				GCI::eGPUBufferBindFlagConstantBufferBit |
				GCI::eGPUResourceContentFlagStaticBit,

		ConstantBufferWriteDiscard =
				GCI::eGPUBufferBindFlagConstantBufferBit |
				GCI::eGPUResourceContentFlagDynamicBit |
				cppx::bsh_left<uint64, 32>( GCI::eGPUMemoryMapFlagWriteInvalidateBit ),

		GeometryBufferDynamic =
				GCI::eGPUBufferBindFlagIndexBufferBit |
				GCI::eGPUBufferBindFlagVertexBufferBit |
				GCI::eGPUResourceContentFlagDynamicBit |
				cppx::bsh_left<uint64, 32>( GCI::eGPUMemoryAccessFlagCpuWriteBit ),

		GeometryBufferImmutable =
				GCI::eGPUBufferBindFlagIndexBufferBit |
				GCI::eGPUBufferBindFlagVertexBufferBit |
				GCI::eGPUResourceContentFlagImmutableBit,

		GeometryBufferStatic =
				GCI::eGPUBufferBindFlagIndexBufferBit |
				GCI::eGPUBufferBindFlagVertexBufferBit |
				GCI::eGPUResourceContentFlagStaticBit,

		GeometryBufferStreamAppend =
				GCI::eGPUBufferBindFlagIndexBufferBit |
				GCI::eGPUBufferBindFlagVertexBufferBit |
				GCI::eGPUResourceContentFlagDynamicBit |
				cppx::bsh_left<uint64, 32>( GCI::eGPUMemoryMapFlagWriteAppendBit ),

		GeometryBufferStreamOverwrite =
				GCI::eGPUBufferBindFlagIndexBufferBit |
				GCI::eGPUBufferBindFlagVertexBufferBit |
				GCI::eGPUResourceContentFlagDynamicBit |
				cppx::bsh_left<uint64, 32>( GCI::eGPUMemoryMapFlagWriteInvalidateBit ),
	};

	enum EGCIGeometryBufferFlags : uint32
	{
		eGCIGeometryBufferFlagVertexBuffer0Bit  = GCI::eIAVertexStreamBindingFlagVertexBuffer0Bit,
		eGCIGeometryBufferFlagVertexBuffer1Bit  = GCI::eIAVertexStreamBindingFlagVertexBuffer1Bit,
		eGCIGeometryBufferFlagVertexBuffer2Bit  = GCI::eIAVertexStreamBindingFlagVertexBuffer2Bit,
		eGCIGeometryBufferFlagVertexBuffer3Bit  = GCI::eIAVertexStreamBindingFlagVertexBuffer3Bit,
		eGCIGeometryBufferFlagVertexBuffer4Bit  = GCI::eIAVertexStreamBindingFlagVertexBuffer4Bit,
		eGCIGeometryBufferFlagVertexBuffer5Bit  = GCI::eIAVertexStreamBindingFlagVertexBuffer5Bit,
		eGCIGeometryBufferFlagVertexBuffer6Bit  = GCI::eIAVertexStreamBindingFlagVertexBuffer6Bit,
		eGCIGeometryBufferFlagVertexBuffer7Bit  = GCI::eIAVertexStreamBindingFlagVertexBuffer7Bit,
		eGCIGeometryBufferFlagVertexBuffer8Bit  = GCI::eIAVertexStreamBindingFlagVertexBuffer8Bit,
		eGCIGeometryBufferFlagVertexBuffer9Bit  = GCI::eIAVertexStreamBindingFlagVertexBuffer9Bit,
		eGCIGeometryBufferFlagVertexBuffer10Bit = GCI::eIAVertexStreamBindingFlagVertexBuffer10Bit,
		eGCIGeometryBufferFlagVertexBuffer11Bit = GCI::eIAVertexStreamBindingFlagVertexBuffer11Bit,
		eGCIGeometryBufferFlagVertexBuffer12Bit = GCI::eIAVertexStreamBindingFlagVertexBuffer12Bit,
		eGCIGeometryBufferFlagVertexBuffer13Bit = GCI::eIAVertexStreamBindingFlagVertexBuffer13Bit,
		eGCIGeometryBufferFlagVertexBuffer14Bit = GCI::eIAVertexStreamBindingFlagVertexBuffer14Bit,
		eGCIGeometryBufferFlagVertexBuffer15Bit = GCI::eIAVertexStreamBindingFlagVertexBuffer15Bit,
		eGCIGeometryBufferFlagIndexBufferBit    = GCI::eIAVertexStreamBindingFlagIndexBufferBit,
	};

	namespace CxDef
	{

		CPPX_ATTR_NO_DISCARD inline constexpr uint16 getIndexDataFormatByteSize( GCI::EIndexDataFormat pIndexFormat ) noexcept
		{
			return GCI::CxDef::GetIndexDataFormatByteSize( pIndexFormat );
		}

		inline constexpr GCI::gpu_memory_flags_value_t getGPUBufferUsagePolicyMemoryFlags( EGPUBufferUsagePolicy pUsagePolicy )
		{
			return static_cast<GCI::gpu_memory_flags_value_t>( ( ( ( uint64 ) pUsagePolicy ) >> 32 ) & cppx::meta::limits<uint32>::max_value );
		}

		inline constexpr GCI::resource_flags_value_t getGPUBufferUsagePolicyResourceFlags( EGPUBufferUsagePolicy pUsagePolicy )
		{
			return static_cast<GCI::resource_flags_value_t>( ( ( uint64 ) pUsagePolicy ) & cppx::meta::limits<uint32>::max_value );
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
//		, memoryRegion{ 0, GCI::CxDef::GPU_MEMORY_SIZE_MAX }
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
