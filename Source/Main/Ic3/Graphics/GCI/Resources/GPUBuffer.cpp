
#include "GPUBuffer.h"

namespace Ic3::Graphics::GCI
{

	GPUBuffer::GPUBuffer(
			GPUDevice & pGPUDevice,
			const ResourceMemoryInfo & pResourceMemory,
			const GPUBufferProperties & pBufferProperties )
	: GPUResource( pGPUDevice, EGPUResourceBaseType::Buffer, pResourceMemory )
	, mBufferProperties( pBufferProperties )
	{}

	GPUBuffer::~GPUBuffer() = default;

	const GPUResourceProperties & GPUBuffer::GetProperties() const
	{
		return mBufferProperties;
	}

	GPUMemoryRegion GPUBuffer::GetWholeBufferRegion() const
	{
		return GPUMemoryRegion{ 0, mBufferProperties.byteSize };
	}

	bool GPUBuffer::ValidateMapRequest( const GPUMemoryRegion & pRegion, const EGPUMemoryMapMode & pMapMode )
	{
		if( IsMapped() )
		{
			return false;
		}

		if( !memutil::checkMemoryMapAccess( pMapMode, mResourceMemory.memoryFlags ) )
		{
			return false;
		}

		const GPUMemoryRegion bufferDataRegion{ 0, mBufferProperties.byteSize };
		if( !cppx::range_is_sub_range_of( pRegion.as_range(), bufferDataRegion.as_range() ) )
		{
			return false;
		}

		return true;
	}

	bool GPUBuffer::ValidateBufferCreateInfo( GPUBufferCreateInfo & pCreateInfo )
	{
		if( pCreateInfo.memoryBaseAlignment == 0 )
		{
		    pCreateInfo.memoryBaseAlignment = Ic3::cxMemoryCpuDefaultAlignment;
		}

		if( ( pCreateInfo.bufferSize == 0 ) && pCreateInfo.initDataDesc )
		{
			pCreateInfo.bufferSize = pCreateInfo.initDataDesc.size;
		}

		return true;
	}

} // namespace Ic3::Graphics::GCI
