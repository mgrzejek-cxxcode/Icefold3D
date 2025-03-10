
#include "VK_presentationLayer.h"
#include <Ic3/GPUapiVK/VKUDevice.h>
#include <Ic3/GPUapiVK/VK_commandList.h>
#include <Ic3/GPUapiVK/VK_commandSystem.h>

namespace Ic3::Graphics::GCI
{

	VKPresentationLayer::VKPresentationLayer( GPUDevice & pDevice )
	: PresentationLayer( pDevice )
	{}

	VKPresentationLayer::~VKPresentationLayer() = default;


	VKScreenPresentationLayer::VKScreenPresentationLayer( GPUDevice & pDevice )
	: VKPresentationLayer( pDevice )
	{}

	VKScreenPresentationLayer::~VKScreenPresentationLayer() = default;

	void VKScreenPresentationLayer::BindRenderTarget( CommandContext & pCommandContext )
	{
	}

	void VKScreenPresentationLayer::InvalidateRenderTarget( CommandContext & pCommandContext )
	{
	}

	void VKScreenPresentationLayer::Present()
	{
	}

	void VKScreenPresentationLayer::resize( uint32 pWidth, uint32 pHeight )
	{
	}

	void VKScreenPresentationLayer::SetFullscreenMode( bool pEnable )
	{
	}

	cxm::vec2u32 VKScreenPresentationLayer::QueryRenderTargetSize() const
	{
		return {};
	}
	
} // namespace Ic3::Graphics::GCI
