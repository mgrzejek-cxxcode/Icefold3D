
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

	void VKScreenPresentationLayer::bindRenderTarget( CommandContext * pCmdContext )
	{
	}

	void VKScreenPresentationLayer::invalidateRenderTarget( CommandContext * pCmdContext )
	{
	}

	void VKScreenPresentationLayer::present()
	{
	}

	void VKScreenPresentationLayer::resize( uint32 pWidth, uint32 pHeight )
	{
	}

	void VKScreenPresentationLayer::setFullscreenMode( bool pEnable )
	{
	}

	Ic3::Math::Vec2u32 VKScreenPresentationLayer::queryRenderTargetSize() const
	{
		return {};
	}
	
} // namespace Ic3::Graphics::GCI
