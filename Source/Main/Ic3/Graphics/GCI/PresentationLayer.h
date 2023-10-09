
#pragma once

#ifndef __IC3_GRAPHICS_GCI_PRESENTATION_LAYER_H__
#define __IC3_GRAPHICS_GCI_PRESENTATION_LAYER_H__

#include "DisplayCommon.h"
#include <Ic3/System/DisplayCommon.h>
#include <Ic3/System/EventCommon.h>
#include <Ic3/System/WindowCommon.h>

namespace Ic3::Graphics::GCI
{

	struct PresentationLayerCreateInfo
	{
	    System::SysContextHandle sysContext = nullptr;
	    System::DisplayManagerHandle sysDisplayManager = nullptr;
		System::WindowManagerHandle sysWindowManager = nullptr;
		System::VisualConfig visualConfig = System::vsxGetDefaultVisualConfigForSysWindow();
		ScreenRect screenRect;
		Bitmask<EDisplayConfigurationFlags> displayConfigFlags = 0u;
	};

	/// @brief
	class IC3_GRAPHICS_GCI_CLASS PresentationLayer : public GPUDriverChildObject
	{
	public:
		GPUDevice & mGPUDevice;

		explicit PresentationLayer( GPUDevice & pGPUDevice );
		virtual ~PresentationLayer();

		virtual System::EventSource * getInternalSystemEventSource() const noexcept;

		virtual void bindRenderTarget( CommandContext * pCmdContext ) = 0;

		virtual void invalidateRenderTarget( CommandContext * pCmdContext ) = 0;

		virtual void present() = 0;

		virtual void resize( uint32 pWidth, uint32 pHeight ) = 0;

		virtual void setFullscreenMode( bool pEnable ) = 0;

		virtual Ic3::Math::Vec2u32 queryRenderTargetSize() const = 0;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_PRESENTATION_LAYER_H__
