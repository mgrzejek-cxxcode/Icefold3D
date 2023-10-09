
#ifndef __IC3_NXMAIN_HARDWARE_BUFFER_COMMON_H__
#define __IC3_NXMAIN_HARDWARE_BUFFER_COMMON_H__

#include "CommonGpaDefs.h"
#include <Ic3/CoreLib/signals/signalCommon.h>

namespace Ic3
{

	ic3DeclareClassHandle( HWBuffer );
	ic3DeclareClassHandle( HWGeometryBuffer );
	ic3DeclareClassHandle( HWVertexBuffer );
	ic3DeclareClassHandle( HWIndexBuffer );

	using hardware_buffer_flags_value_t = uint64;

	enum EEventHWBCodes : event_code_value_t
	{
		E_EVT_HWB_BASE_ID = 0x7F,
		E_EVT_HWB_GPU_BUFFER_REF_UPDATED,
		E_EVT_HWB_LOCKED,
		E_EVT_HWB_UNLOCKED,
	};

	enum class EHWBufferType : uint32
	{
		HBTConstantBuffer         = static_cast<uint32>( GCI::EGPUBufferTarget::ConstantBuffer ),
		HBTVertexBuffer           = static_cast<uint32>( GCI::EGPUBufferTarget::VertexBuffer ),
		HBTIndexBuffer            = static_cast<uint32>( GCI::EGPUBufferTarget::IndexBuffer ),
		HBTShaderUAVBuffer        = static_cast<uint32>( GCI::EGPUBufferTarget::ShaderUAVBuffer ),
		HBTTransferSourceBuffer   = static_cast<uint32>( GCI::EGPUBufferTarget::TransferSourceBuffer ),
		HBTTransferTargetBuffer   = static_cast<uint32>( GCI::EGPUBufferTarget::TransferTargetBuffer ),
		HBTUnknown                = static_cast<uint32>( GCI::EGPUBufferTarget::Unknown ),
	};

	enum class EHWBufferUsageMode : hardware_buffer_flags_value_t
	{
		ConstantBufferDynamic       = GCI::E_GPU_BUFFER_BIND_FLAG_CONSTANT_BUFFER_BIT |
		                              GCI::E_GPU_RESOURCE_CONTENT_FLAG_DYNAMIC_BIT |
		                              bshLeft<uint64, 32>( GCI::E_GPU_MEMORY_ACCESS_FLAG_CPU_WRITE_BIT ),

		ConstantBufferStatic        = GCI::E_GPU_BUFFER_BIND_FLAG_CONSTANT_BUFFER_BIT |
		                              GCI::E_GPU_RESOURCE_CONTENT_FLAG_STATIC_BIT,

		ConstantBufferWriteDiscard  = GCI::E_GPU_BUFFER_BIND_FLAG_CONSTANT_BUFFER_BIT |
		                              GCI::E_GPU_RESOURCE_CONTENT_FLAG_DYNAMIC_BIT |
		                              bshLeft<uint64, 32>( GCI::E_GPU_MEMORY_MAP_FLAG_WRITE_INVALIDATE_BIT ),

		IndexBufferDynamic          = GCI::E_GPU_BUFFER_BIND_FLAG_INDEX_BUFFER_BIT |
		                              GCI::E_GPU_RESOURCE_CONTENT_FLAG_DYNAMIC_BIT |
		                              bshLeft<uint64, 32>( GCI::E_GPU_MEMORY_ACCESS_FLAG_CPU_WRITE_BIT ),

		IndexBufferImmutable        = GCI::E_GPU_BUFFER_BIND_FLAG_INDEX_BUFFER_BIT |
		                              GCI::E_GPU_RESOURCE_CONTENT_FLAG_IMMUTABLE_BIT,

		IndexBufferStatic           = GCI::E_GPU_BUFFER_BIND_FLAG_INDEX_BUFFER_BIT |
		                              GCI::E_GPU_RESOURCE_CONTENT_FLAG_STATIC_BIT,

		IndexBufferStreamAppend     = GCI::E_GPU_BUFFER_BIND_FLAG_INDEX_BUFFER_BIT |
		                              GCI::E_GPU_RESOURCE_CONTENT_FLAG_DYNAMIC_BIT |
		                              bshLeft<uint64, 32>( GCI::E_GPU_MEMORY_MAP_FLAG_WRITE_APPEND_BIT ),

		IndexBufferStreamOverwrite  = GCI::E_GPU_BUFFER_BIND_FLAG_INDEX_BUFFER_BIT |
		                              GCI::E_GPU_RESOURCE_CONTENT_FLAG_DYNAMIC_BIT |
		                              bshLeft<uint64, 32>( GCI::E_GPU_MEMORY_ACCESS_FLAG_CPU_WRITE_BIT ),

		VertexBufferDynamic         = GCI::E_GPU_BUFFER_BIND_FLAG_VERTEX_BUFFER_BIT |
		                              GCI::E_GPU_RESOURCE_CONTENT_FLAG_DYNAMIC_BIT |
		                              bshLeft<uint64, 32>( GCI::E_GPU_MEMORY_ACCESS_FLAG_CPU_WRITE_BIT ),

		VertexBufferImmutable       = GCI::E_GPU_BUFFER_BIND_FLAG_INDEX_BUFFER_BIT |
		                              GCI::E_GPU_RESOURCE_CONTENT_FLAG_IMMUTABLE_BIT,

		VertexBufferStatic          = GCI::E_GPU_BUFFER_BIND_FLAG_VERTEX_BUFFER_BIT |
		                              GCI::E_GPU_RESOURCE_CONTENT_FLAG_STATIC_BIT,

		VertexBufferStreamAppend    = GCI::E_GPU_BUFFER_BIND_FLAG_VERTEX_BUFFER_BIT |
		                              GCI::E_GPU_RESOURCE_CONTENT_FLAG_DYNAMIC_BIT |
		                              bshLeft<uint64, 32>( GCI::E_GPU_MEMORY_MAP_FLAG_WRITE_APPEND_BIT ),

		VertexBufferStreamOverwrite = GCI::E_GPU_BUFFER_BIND_FLAG_VERTEX_BUFFER_BIT |
		                              GCI::E_GPU_RESOURCE_CONTENT_FLAG_DYNAMIC_BIT |
		                              bshLeft<uint64, 32>( GCI::E_GPU_MEMORY_MAP_FLAG_WRITE_APPEND_BIT ),
	};

	namespace CxDefs
	{

		template <typename TBufferUsage>
		inline constexpr GCI::gpu_memory_flags_value_t getHWBufferUsageGPUMemoryFlags( TBufferUsage pBufferUsage )
		{
			return static_cast<GCI::gpu_memory_flags_value_t>( ( ( ( hardware_buffer_flags_value_t ) pBufferUsage ) >> 32 ) & Limits<uint32>::maxValue );
		}

		template <typename TBufferUsage>
		inline constexpr GCI::resource_flags_value_t getHWBufferUsageGPUResourceFlags( TBufferUsage pBufferUsage )
		{
			return static_cast<GCI::resource_flags_value_t>( ( ( hardware_buffer_flags_value_t ) pBufferUsage ) & Limits<uint32>::maxValue );
		}

	}


} // namespace Ic3

#endif // __IC3_NXMAIN_HARDWARE_BUFFER_COMMON_H__
