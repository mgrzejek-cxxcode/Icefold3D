
#include "PresentationLayer.h"
#include "GPUDevice.h"

namespace Ic3::Graphics::GCI
{

	PresentationLayer::PresentationLayer( GPUDevice & pGPUDevice )
	: GPUDriverChildObject( pGPUDevice.mGPUDriver )
	, mGPUDevice( pGPUDevice )
	{}

	PresentationLayer::~PresentationLayer() = default;

	System::EventSource * PresentationLayer::GetInternalSystemEventSource() const noexcept
	{
		return nullptr;
	}

} // namespace Ic3::Graphics::GCI
