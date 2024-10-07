
#ifndef __IC3_NXMAIN_HARDWARE_BUFFER_COMMON_H__
#define __IC3_NXMAIN_HARDWARE_BUFFER_COMMON_H__

#include "CommonGCIDefs.h"
#include <Ic3/Graphics/GCI/Resources/GpuBufferReference.h>

namespace Ic3
{

	ic3DeclareClassHandle( HardwareBuffer );

	ic3DeclareClassHandle( HWGeometryBuffer );
	ic3DeclareClassHandle( HWVertexBuffer );
	ic3DeclareClassHandle( HWIndexBuffer );

	using RHardwareBufferFlagsValueType = uint64;
	using RHardwareBufferTransferUID = uint64;

	enum EEvtHardwareBufferCode : uint32 // event_code_value_t
	{
		eEvtCodeHardwareBufferBase = 0x7F,
		eEvtCodeHardwareBufferGpuBufferRefUpdated,
		eEvtCodeHardwareBufferStateChangeLocked,
		eEvtCodeHardwareBufferStateChangeUnlocked,
		eEvtCodeHardwareBufferTransferComplete,
	};

	enum class EHardwareBufferType : uint32
	{
		HBTConstantBuffer         = static_cast<uint32>( GCI::EGpuBufferTarget::ConstantBuffer ),
		HBTVertexBuffer           = static_cast<uint32>( GCI::EGpuBufferTarget::VertexBuffer ),
		HBTIndexBuffer            = static_cast<uint32>( GCI::EGpuBufferTarget::IndexBuffer ),
		HBTShaderUAVBuffer        = static_cast<uint32>( GCI::EGpuBufferTarget::ShaderUAVBuffer ),
		HBTTransferSourceBuffer   = static_cast<uint32>( GCI::EGpuBufferTarget::TransferSourceBuffer ),
		HBTTransferTargetBuffer   = static_cast<uint32>( GCI::EGpuBufferTarget::TransferTargetBuffer ),
		HBTUnknown                = static_cast<uint32>( GCI::EGpuBufferTarget::Unknown ),
	};

	enum class EHardwareBufferUsageMode : RHardwareBufferFlagsValueType
	{
		ConstantBufferDynamic       = GCI::eGpuBufferBindFlagConstantBufferBit |
		                              GCI::eGpuResourceContentFlagDynamicBit |
		                              Cppx::bshLeft<uint64, 32>( GCI::eGpuMemoryAccessFlagCpuWriteBit ),

		ConstantBufferStatic        = GCI::eGpuBufferBindFlagConstantBufferBit |
		                              GCI::eGpuResourceContentFlagStaticBit,

		ConstantBufferWriteDiscard  = GCI::eGpuBufferBindFlagConstantBufferBit |
		                              GCI::eGpuResourceContentFlagDynamicBit |
		                              Cppx::bshLeft<uint64, 32>( GCI::eGpuMemoryMapFlagWriteInvalidateBit ),

		IndexBufferDynamic          = GCI::eGpuBufferBindFlagIndexBufferBit |
		                              GCI::eGpuResourceContentFlagDynamicBit |
		                              Cppx::bshLeft<uint64, 32>( GCI::eGpuMemoryAccessFlagCpuWriteBit ),

		IndexBufferImmutable        = GCI::eGpuBufferBindFlagIndexBufferBit |
		                              GCI::eGpuResourceContentFlagImmutableBit,

		IndexBufferStatic           = GCI::eGpuBufferBindFlagIndexBufferBit |
		                              GCI::eGpuResourceContentFlagStaticBit,

		IndexBufferStreamAppend     = GCI::eGpuBufferBindFlagIndexBufferBit |
		                              GCI::eGpuResourceContentFlagDynamicBit |
		                              Cppx::bshLeft<uint64, 32>( GCI::eGpuMemoryMapFlagWriteAppendBit ),

		IndexBufferStreamOverwrite  = GCI::eGpuBufferBindFlagIndexBufferBit |
		                              GCI::eGpuResourceContentFlagDynamicBit |
		                              Cppx::bshLeft<uint64, 32>( GCI::eGpuMemoryAccessFlagCpuWriteBit ),

		VertexBufferDynamic         = GCI::eGpuBufferBindFlagVertexBufferBit |
		                              GCI::eGpuResourceContentFlagDynamicBit |
		                              Cppx::bshLeft<uint64, 32>( GCI::eGpuMemoryAccessFlagCpuWriteBit ),

		VertexBufferImmutable       = GCI::eGpuBufferBindFlagIndexBufferBit |
		                              GCI::eGpuResourceContentFlagImmutableBit,

		VertexBufferStatic          = GCI::eGpuBufferBindFlagVertexBufferBit |
		                              GCI::eGpuResourceContentFlagStaticBit,

		VertexBufferStreamAppend    = GCI::eGpuBufferBindFlagVertexBufferBit |
		                              GCI::eGpuResourceContentFlagDynamicBit |
		                              Cppx::bshLeft<uint64, 32>( GCI::eGpuMemoryMapFlagWriteAppendBit ),

		VertexBufferStreamOverwrite = GCI::eGpuBufferBindFlagVertexBufferBit |
		                              GCI::eGpuResourceContentFlagDynamicBit |
		                              Cppx::bshLeft<uint64, 32>( GCI::eGpuMemoryMapFlagWriteAppendBit ),
	};

	namespace CxDef
	{

		template <typename TBufferUsage>
		inline constexpr GCI::gpu_memory_flags_value_t getHardwareBufferUsageGpuMemoryFlags( TBufferUsage pBufferUsage )
		{
			return static_cast<GCI::gpu_memory_flags_value_t>( ( ( ( RHardwareBufferFlagsValueType ) pBufferUsage ) >> 32 ) & QLimits<uint32>::sMaxValue );
		}

		template <typename TBufferUsage>
		inline constexpr GCI::resource_flags_value_t getHardwareBufferUsageGpuResourceFlags( TBufferUsage pBufferUsage )
		{
			return static_cast<GCI::resource_flags_value_t>( ( ( RHardwareBufferFlagsValueType ) pBufferUsage ) & QLimits<uint32>::sMaxValue );
		}

	}


} // namespace Ic3

#endif // __IC3_NXMAIN_HARDWARE_BUFFER_COMMON_H__
