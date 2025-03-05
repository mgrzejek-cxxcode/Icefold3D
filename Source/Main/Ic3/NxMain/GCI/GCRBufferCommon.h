
#ifndef __IC3_NXMAIN_HARDWARE_BUFFER_COMMON_H__
#define __IC3_NXMAIN_HARDWARE_BUFFER_COMMON_H__

#include "CommonGCIDefs.h"
#include <Ic3/Graphics/GCI/Resources/GPUBufferReference.h>

namespace Ic3
{

	Ic3DeclareClassHandle( GCRBuffer );
	Ic3DeclareClassHandle( GCRConstantBuffer );
	Ic3DeclareClassHandle( GCRGeometryBuffer );
	Ic3DeclareClassHandle( GCRRawBuffer );

	using RGCRBufferFlagsValueType = uint64;

	enum class EGCRBufferType : uint32
	{
		GBTConstantBuffer         = static_cast<uint32>( GCI::EGPUBufferTarget::ConstantBuffer ),
		GBTVertexBuffer           = static_cast<uint32>( GCI::EGPUBufferTarget::VertexBuffer ),
		GBTIndexBuffer            = static_cast<uint32>( GCI::EGPUBufferTarget::IndexBuffer ),
		GBTShaderUAVBuffer        = static_cast<uint32>( GCI::EGPUBufferTarget::ShaderUAVBuffer ),
		GBTTransferSourceBuffer   = static_cast<uint32>( GCI::EGPUBufferTarget::TransferSourceBuffer ),
		GBTTransferTargetBuffer   = static_cast<uint32>( GCI::EGPUBufferTarget::TransferTargetBuffer ),
		GBTUnknown                = static_cast<uint32>( GCI::EGPUBufferTarget::Unknown ),
	};

	enum class EGCRBufferUsageMode : RGCRBufferFlagsValueType
	{
		ConstantBufferDynamic =
				GCI::eGPUBufferBindFlagConstantBufferBit |
				GCI::eGPUResourceContentFlagDynamicBit |
				cppx::bsh_left<uint64, 32>( GCI::eGPUMemoryAccessFlagCPUWriteBit ),

		ConstantBufferStatic =
				GCI::eGPUBufferBindFlagConstantBufferBit |
				GCI::eGPUResourceContentFlagStaticBit,

		ConstantBufferWriteDiscard =
				GCI::eGPUBufferBindFlagConstantBufferBit |
				GCI::eGPUResourceContentFlagDynamicBit |
				cppx::bsh_left<uint64, 32>( GCI::eGPUMemoryMapFlagWriteInvalidateBit ),

		IndexBufferDynamic =
				GCI::eGPUBufferBindFlagIndexBufferBit |
				GCI::eGPUResourceContentFlagDynamicBit |
				cppx::bsh_left<uint64, 32>( GCI::eGPUMemoryAccessFlagCPUWriteBit ),

		IndexBufferImmutable =
				GCI::eGPUBufferBindFlagIndexBufferBit |
				GCI::eGPUResourceContentFlagImmutableBit,

		IndexBufferStatic =
				GCI::eGPUBufferBindFlagIndexBufferBit |
				GCI::eGPUResourceContentFlagStaticBit,

		IndexBufferStreamAppend =
				GCI::eGPUBufferBindFlagIndexBufferBit |
				GCI::eGPUResourceContentFlagDynamicBit |
				cppx::bsh_left<uint64, 32>( GCI::eGPUMemoryMapFlagWriteAppendBit ),

		IndexBufferStreamOverwrite =
				GCI::eGPUBufferBindFlagIndexBufferBit |
				GCI::eGPUResourceContentFlagDynamicBit |
				cppx::bsh_left<uint64, 32>( GCI::eGPUMemoryAccessFlagCPUWriteBit ),

		VertexBufferDynamic =
				GCI::eGPUBufferBindFlagVertexBufferBit |
				GCI::eGPUResourceContentFlagDynamicBit |
				cppx::bsh_left<uint64, 32>( GCI::eGPUMemoryAccessFlagCPUWriteBit ),

		VertexBufferImmutable =
				GCI::eGPUBufferBindFlagIndexBufferBit |
				GCI::eGPUResourceContentFlagImmutableBit,

		VertexBufferStatic =
				GCI::eGPUBufferBindFlagVertexBufferBit |
				GCI::eGPUResourceContentFlagStaticBit,

		VertexBufferStreamAppend =
				GCI::eGPUBufferBindFlagVertexBufferBit |
				GCI::eGPUResourceContentFlagDynamicBit |
				cppx::bsh_left<uint64, 32>( GCI::eGPUMemoryMapFlagWriteAppendBit ),

		VertexBufferStreamOverwrite =
				GCI::eGPUBufferBindFlagVertexBufferBit |
				GCI::eGPUResourceContentFlagDynamicBit |
				cppx::bsh_left<uint64, 32>( GCI::eGPUMemoryMapFlagWriteAppendBit ),
	};

	namespace CXU
	{

		template <typename TPBufferUsage>
		inline constexpr GCI::gpu_memory_flags_value_t getGCRBufferUsageGPUMemoryFlags( TPBufferUsage pBufferUsage )
		{
			return static_cast<GCI::gpu_memory_flags_value_t>(
					( ( ( RGCRBufferFlagsValueType ) pBufferUsage ) >> 32 ) & cppx::meta::limits<uint32>::max_value );
		}

		template <typename TPBufferUsage>
		inline constexpr GCI::resource_flags_value_t getGCRBufferUsageGPUResourceFlags( TPBufferUsage pBufferUsage )
		{
			return static_cast<GCI::resource_flags_value_t>(
					( ( RGCRBufferFlagsValueType ) pBufferUsage ) & cppx::meta::limits<uint32>::max_value );
		}

	}


} // namespace Ic3

#endif // __IC3_NXMAIN_HARDWARE_BUFFER_COMMON_H__
