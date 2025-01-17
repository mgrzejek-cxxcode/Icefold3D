
#include "GpuBuffer.h"

namespace Ic3::Graphics::GCI
{

	GpuBuffer::GpuBuffer(
			GpuDevice & pGpuDevice,
			const ResourceMemoryInfo & pResourceMemory,
			const GpuBufferProperties & pBufferProperties )
	: GpuResource( pGpuDevice, EGpuResourceBaseType::Buffer, pResourceMemory )
	, mBufferProperties( pBufferProperties )
	{}

	GpuBuffer::~GpuBuffer() = default;

	const GpuResourceProperties & GpuBuffer::GetProperties() const
	{
		return mBufferProperties;
	}

	GpuMemoryRegion GpuBuffer::GetWholeBufferRegion() const
	{
		return GpuMemoryRegion{ 0, mBufferProperties.byteSize };
	}

	bool GpuBuffer::ValidateMapRequest( const GpuMemoryRegion & pRegion, const EGpuMemoryMapMode & pMapMode )
	{
		if( IsMapped() )
		{
			return false;
		}

		if( !memutil::checkMemoryMapAccess( pMapMode, mResourceMemory.memoryFlags ) )
		{
			return false;
		}

		const GpuMemoryRegion bufferDataRegion{ 0, mBufferProperties.byteSize };
		if( !cppx::range_is_sub_range_of( pRegion.as_range(), bufferDataRegion.as_range() ) )
		{
			return false;
		}

		return true;
	}

	bool GpuBuffer::ValidateBufferCreateInfo( GpuBufferCreateInfo & pCreateInfo )
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
