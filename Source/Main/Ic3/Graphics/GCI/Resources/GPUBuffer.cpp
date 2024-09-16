
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

	const GPUResourceProperties & GPUBuffer::getProperties() const
	{
		return mBufferProperties;
	}

	GPUMemoryRegion GPUBuffer::getWholeBufferRegion() const
	{
		return GPUMemoryRegion{ 0, mBufferProperties.byteSize };
	}

	bool GPUBuffer::validateMapRequest( const GPUMemoryRegion & pRegion, const EGPUMemoryMapMode & pMapMode )
	{
		if( isMapped() )
		{
			return false;
		}

		if( !memutil::checkMemoryMapAccess( pMapMode, mResourceMemory.memoryFlags ) )
		{
			return false;
		}

		const GPUMemoryRegion bufferDataRegion{ 0, mBufferProperties.byteSize };
		if( !Ic3::rangeIsSubRangeOf( pRegion.asRange(), bufferDataRegion.asRange() ) )
		{
			return false;
		}

		return true;
	}

	bool GPUBuffer::validateBufferCreateInfo( GPUBufferCreateInfo & pCreateInfo )
	{
		if( pCreateInfo.mMemoryBaseAlignment == 0 )
		{
		    pCreateInfo.mMemoryBaseAlignment = Ic3::cxMemoryCPUDefaultAlignment;
		}

		if( ( pCreateInfo.bufferSize == 0 ) && pCreateInfo.initDataDesc )
		{
			pCreateInfo.bufferSize = pCreateInfo.initDataDesc.size;
		}

		return true;
	}

} // namespace Ic3::Graphics::GCI
