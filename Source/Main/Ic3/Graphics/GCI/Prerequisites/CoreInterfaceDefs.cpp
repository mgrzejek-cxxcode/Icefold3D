
#include "../GPUDriver.h"
#include "../GPUDevice.h"

namespace Ic3::Graphics::GCI
{

	GfxObject::GfxObject()
	: _gfxObjectID( kGfxObjectIDEmpty )
	{}

	GfxObject::~GfxObject() = default;

	void GfxObject::SetObjectUID( GfxObjectUID pGfxObjectUID )
	{
		SetObjectID( pGfxObjectUID.id );

		if( pGfxObjectUID.name )
		{
			SetObjectDebugName( std::string( pGfxObjectUID.name ) );
		}
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
