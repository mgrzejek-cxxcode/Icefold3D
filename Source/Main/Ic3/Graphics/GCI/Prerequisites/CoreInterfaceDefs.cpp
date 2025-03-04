
#include "../GPUDriver.h"
#include "../GPUDevice.h"

namespace Ic3::Graphics::GCI
{

	GfxObject::GfxObject() = default;

	GfxObject::~GfxObject() = default;

	GfxObjectID GfxObject::GetObjectID() const noexcept
	{
		return kGfxObjectIDEmpty;
	}

	GfxObjectName GfxObject::GetObjectName() const noexcept
	{
		return nullptr;
	}


	GPUDriverChildObject::GPUDriverChildObject( GPUDriver & pGPUDriver )
	: mGPUDriver( pGPUDriver )
	{}

	GPUDriverChildObject::~GPUDriverChildObject() = default;


	GPUDeviceChildObject::GPUDeviceChildObject( GPUDevice & pGPUDevice )
	: mGPUDevice( pGPUDevice )
	{}

	GPUDeviceChildObject::~GPUDeviceChildObject() = default;

} // namespace Ic3::Graphics::GCI
