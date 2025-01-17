
#include "VK_presentationLayer.h"
#include <Ic3/GpuapiVK/VKUDevice.h>
#include <Ic3/GpuapiVK/VK_commandList.h>
#include <Ic3/GpuapiVK/VK_commandSystem.h>

namespace Ic3::Graphics::GCI
{

	VKPresentationLayer::VKPresentationLayer( GpuDevice & pDevice )
	: PresentationLayer( pDevice )
	{}

	VKPresentationLayer::~VKPresentationLayer() = default;


	VKScreenPresentationLayer::VKScreenPresentationLayer( GpuDevice & pDevice )
	: VKPresentationLayer( pDevice )
	{}

	VKScreenPresentationLayer::~VKScreenPresentationLayer() = default;

	void VKScreenPresentationLayer::BindRenderTarget( CommandContext * pCmdContext )
	{
	}

	void VKScreenPresentationLayer::InvalidateRenderTarget( CommandContext * pCmdContext )
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

	Ic3::Math::Vec2u32 VKScreenPresentationLayer::QueryRenderTargetSize() const
	{
		return {};
	}
	
} // namespace Ic3::Graphics::GCI
