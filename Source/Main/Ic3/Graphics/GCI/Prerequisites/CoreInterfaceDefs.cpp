
#include "../GpuDriver.h"
#include "../GpuDevice.h"

namespace Ic3::Graphics::GCI
{

	GpuDriverChildObject::GpuDriverChildObject( GpuDriver & pGpuDriver )
	: mGpuDriver( pGpuDriver )
	{}

	GpuDriverChildObject::~GpuDriverChildObject() = default;


	GpuDeviceChildObject::GpuDeviceChildObject( GpuDevice & pGpuDevice )
	: mGpuDriver( pGpuDevice.mGpuDriver )
	, mGpuDevice( pGpuDevice )
	{}

	GpuDeviceChildObject::~GpuDeviceChildObject() = default;

	GfxObjectID GpuDeviceChildObject::QueryObjectID() const
	{
		return cxGpuObjectIDEmpty;
	}

} // namespace Ic3::Graphics::GCI
