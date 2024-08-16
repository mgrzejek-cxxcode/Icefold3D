
#include "../GPUDriver.h"
#include "../GPUDevice.h"

namespace Ic3::Graphics::GCI
{

	GPUDriverChildObject::GPUDriverChildObject( GPUDriver & pGPUDriver )
	: mGPUDriver( pGPUDriver )
	{}

	GPUDriverChildObject::~GPUDriverChildObject() = default;


	GPUDeviceChildObject::GPUDeviceChildObject( GPUDevice & pGPUDevice )
	: mGPUDriver( pGPUDevice.mGPUDriver )
	, mGPUDevice( pGPUDevice )
	{}

	GPUDeviceChildObject::~GPUDeviceChildObject() = default;

	UniqueGPUObjectID GPUDeviceChildObject::queryObjectID() const
	{
		return CxDef::GPU_OBJECT_ID_EMPTY;
	}

} // namespace Ic3::Graphics::GCI
