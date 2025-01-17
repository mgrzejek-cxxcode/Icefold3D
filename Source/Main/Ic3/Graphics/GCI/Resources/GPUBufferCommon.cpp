
#include "GpuBufferCommon.h"
#include "../Resources/GpuBuffer.h"

namespace Ic3::Graphics::GCI
{

	namespace RCU
	{

		EGpuBufferTarget GetGpuBufferDefaultTargetFromBindFlags( cppx::bitmask<resource_flags_value_t> pBindFlags )
		{
			const uint32 orderedBindMaskArray[] =
			{
					eGpuBufferBindFlagConstantBufferBit,
					eGpuBufferBindFlagVertexBufferBit,
					eGpuBufferBindFlagIndexBufferBit,
					eGpuBufferBindFlagStreamOutputBufferBit,
					eGpuBufferBindFlagShaderInputBufferBit,
					eGpuBufferBindFlagShaderUAVBufferBit,
					eGpuBufferBindFlagIndirectDispatchBufferBit,
					eGpuBufferBindFlagIndirectDrawBufferBit,
					eGpuBufferBindFlagTransferSourceBufferBit,
					eGpuBufferBindFlagTransferTargetBufferBit
			};

			for( auto bindMask : orderedBindMaskArray )
			{
				if( pBindFlags.is_set( bindMask ) )
				{
					return static_cast<EGpuBufferTarget>( bindMask );
				}
			}

			return EGpuBufferTarget::Unknown;
		}

		gpu_memory_size_t QueryGpuBufferByteSize( GpuBufferHandle pGpuBuffer )
		{
			return pGpuBuffer->mBufferProperties.byteSize;
		}

		bool CheckGpuBufferRegion( GpuBufferHandle pGpuBuffer, gpu_memory_size_t pOffset, gpu_memory_size_t pSize )
		{
			const auto bufferSize = RCU::QueryGpuBufferByteSize( pGpuBuffer );
			return ( pSize > 0 ) && ( pOffset < bufferSize ) && ( pSize <= ( bufferSize - pOffset ) );
		}

		bool CheckGpuBufferRegion( GpuBufferHandle pGpuBuffer, const GpuMemoryRegion & pRegion )
		{
			return CheckGpuBufferRegion( pGpuBuffer, pRegion.offset, pRegion.size );
		}

		GpuMemoryRegion ValidateGpuBufferRegion( GpuBufferHandle pGpuBuffer, gpu_memory_size_t pOffset, gpu_memory_size_t pSize )
		{
			const auto bufferSize = RCU::QueryGpuBufferByteSize( pGpuBuffer );

			GpuMemoryRegion validRegion{ 0, 0 };
			validRegion.offset = cppx::get_min_of( pOffset, bufferSize );
			validRegion.size = cppx::get_min_of( pSize, bufferSize - pOffset );

			return validRegion;
		}

		GpuMemoryRegion ValidateGpuBufferRegion( GpuBufferHandle pGpuBuffer, const GpuMemoryRegion & pRegion )
		{
			return ValidateGpuBufferRegion( pGpuBuffer, pRegion.offset, pRegion.size );
		}

	}

}
