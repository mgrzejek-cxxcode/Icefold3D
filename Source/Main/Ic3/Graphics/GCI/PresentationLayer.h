
#pragma once

#ifndef __IC3_GRAPHICS_GCI_PRESENTATION_LAYER_H__
#define __IC3_GRAPHICS_GCI_PRESENTATION_LAYER_H__

#include "DisplayCommon.h"
#include <Ic3/System/Core/DisplayCommon.h>
#include <Ic3/System/Events/EventCommon.h>
#include <Ic3/System/Core/WindowCommon.h>

namespace Ic3::Graphics::GCI
{

	struct PresentationLayerCreateInfo
	{
	    System::SysContextHandle sysContext = nullptr;
	    System::DisplayManagerHandle sysDisplayManager = nullptr;
		System::WindowManagerHandle sysWindowManager = nullptr;
		System::VisualConfig visualConfig = System::VisGetDefaultVisualConfigForSysWindow();
		ScreenRect screenRect;
		cppx::bitmask<EDisplayConfigurationFlags> displayConfigFlags = 0u;
	};

	/// @brief
	class IC3_GRAPHICS_GCI_CLASS PresentationLayer : public GPUDriverChildObject
	{
	public:
		GPUDevice & mGPUDevice;

		explicit PresentationLayer( GPUDevice & pGPUDevice );
		virtual ~PresentationLayer();

		virtual System::EventSource * GetInternalSystemEventSource() const noexcept;

		virtual void BindRenderTarget( CommandContext & pCommandContext ) = 0;

		virtual void InvalidateRenderTarget( CommandContext & pCommandContext ) = 0;

		virtual void Present() = 0;

		virtual void Resize( uint32 pWidth, uint32 pHeight ) = 0;

		virtual void SetFullscreenMode( bool pEnable ) = 0;

		virtual Ic3::Math::Vec2u32 QueryRenderTargetSize() const = 0;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_PRESENTATION_LAYER_H__
