
#include "GPUBufferCommon.h"
#include "../Resources/GPUBuffer.h"

namespace Ic3::Graphics::GCI
{

	namespace RCU
	{

		EGPUBufferTarget GetGPUBufferDefaultTargetFromBindFlags( cppx::bitmask<resource_flags_value_t> pBindFlags )
		{
			const uint32 orderedBindMaskArray[] =
			{
					eGPUBufferBindFlagConstantBufferBit,
					eGPUBufferBindFlagVertexBufferBit,
					eGPUBufferBindFlagIndexBufferBit,
					eGPUBufferBindFlagStreamOutputBufferBit,
					eGPUBufferBindFlagShaderInputBufferBit,
					eGPUBufferBindFlagShaderUAVBufferBit,
					eGPUBufferBindFlagIndirectDispatchBufferBit,
					eGPUBufferBindFlagIndirectDrawBufferBit,
					eGPUBufferBindFlagTransferSourceBufferBit,
					eGPUBufferBindFlagTransferTargetBufferBit
			};

			for( auto bindMask : orderedBindMaskArray )
			{
				if( pBindFlags.is_set( bindMask ) )
				{
					return static_cast<EGPUBufferTarget>( bindMask );
				}
			}

			return EGPUBufferTarget::Unknown;
		}

		gpu_memory_size_t QueryGPUBufferByteSize( GPUBufferHandle pGPUBuffer )
		{
			return pGPUBuffer->mBufferProperties.byteSize;
		}

		bool CheckGPUBufferRegion( GPUBufferHandle pGPUBuffer, gpu_memory_size_t pOffset, gpu_memory_size_t pSize )
		{
			const auto bufferSize = RCU::QueryGPUBufferByteSize( pGPUBuffer );
			return ( pSize > 0 ) && ( pOffset < bufferSize ) && ( pSize <= ( bufferSize - pOffset ) );
		}

		bool CheckGPUBufferRegion( GPUBufferHandle pGPUBuffer, const GPUMemoryRegion & pRegion )
		{
			return CheckGPUBufferRegion( pGPUBuffer, pRegion.offset, pRegion.size );
		}

		GPUMemoryRegion ValidateGPUBufferRegion( GPUBufferHandle pGPUBuffer, gpu_memory_size_t pOffset, gpu_memory_size_t pSize )
		{
			const auto bufferSize = RCU::QueryGPUBufferByteSize( pGPUBuffer );

			GPUMemoryRegion validRegion{ 0, 0 };
			validRegion.offset = cppx::get_min_of( pOffset, bufferSize );
			validRegion.size = cppx::get_min_of( pSize, bufferSize - pOffset );

			return validRegion;
		}

		GPUMemoryRegion ValidateGPUBufferRegion( GPUBufferHandle pGPUBuffer, const GPUMemoryRegion & pRegion )
		{
			return ValidateGPUBufferRegion( pGPUBuffer, pRegion.offset, pRegion.size );
		}

	}

}
