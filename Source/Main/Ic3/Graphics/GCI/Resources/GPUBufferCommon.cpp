
#include "GPUBufferCommon.h"
#include "../Resources/GPUBuffer.h"

namespace Ic3::Graphics::GCI
{

	namespace RCU
	{

		EGPUBufferTarget getGPUBufferDefaultTargetFromBindFlags( TBitmask<resource_flags_value_t> pBindFlags )
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
				if( pBindFlags.isSet( bindMask ) )
				{
					return static_cast<EGPUBufferTarget>( bindMask );
				}
			}

			return EGPUBufferTarget::Unknown;
		}

		gpu_memory_size_t queryGPUBufferByteSize( GPUBufferHandle pGPUBuffer )
		{
			return pGPUBuffer->mBufferProperties.byteSize;
		}

		bool checkGPUBufferRegion( GPUBufferHandle pGPUBuffer, gpu_memory_size_t pOffset, gpu_memory_size_t pSize )
		{
			const auto bufferSize = RCU::queryGPUBufferByteSize( pGPUBuffer );
			return ( pSize > 0 ) && ( pOffset < bufferSize ) && ( pSize <= ( bufferSize - pOffset ) );
		}

		bool checkGPUBufferRegion( GPUBufferHandle pGPUBuffer, const GPUMemoryRegion & pRegion )
		{
			return checkGPUBufferRegion( pGPUBuffer, pRegion.offset, pRegion.size );
		}

		GPUMemoryRegion validateGPUBufferRegion( GPUBufferHandle pGPUBuffer, gpu_memory_size_t pOffset, gpu_memory_size_t pSize )
		{
			const auto bufferSize = RCU::queryGPUBufferByteSize( pGPUBuffer );

			GPUMemoryRegion validRegion{ 0, 0 };
			validRegion.offset = getMinOf( pOffset, bufferSize );
			validRegion.size = getMinOf( pSize, bufferSize - pOffset );

			return validRegion;
		}

		GPUMemoryRegion validateGPUBufferRegion( GPUBufferHandle pGPUBuffer, const GPUMemoryRegion & pRegion )
		{
			return validateGPUBufferRegion( pGPUBuffer, pRegion.offset, pRegion.size );
		}

	}

}
