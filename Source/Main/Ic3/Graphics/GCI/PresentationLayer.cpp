
#include "PresentationLayer.h"
#include "GpuDevice.h"

namespace Ic3::Graphics::GCI
{

	PresentationLayer::PresentationLayer( GpuDevice & pGpuDevice )
	: GpuDriverChildObject( pGpuDevice.mGpuDriver )
	, mGpuDevice( pGpuDevice )
	{}

	PresentationLayer::~PresentationLayer() = default;

	System::EventSource * PresentationLayer::GetInternalSystemEventSource() const noexcept
	{
		return nullptr;
	}

} // namespace Ic3::Graphics::GCI
