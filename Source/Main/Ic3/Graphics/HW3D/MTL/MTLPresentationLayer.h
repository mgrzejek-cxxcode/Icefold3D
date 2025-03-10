
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_MTLCOMMON_PRESENTATION_LAYER_H__
#define __IC3_GRAPHICS_HW3D_MTLCOMMON_PRESENTATION_LAYER_H__

#include "MTLPrerequisites.h"
#include <Ic3/Graphics/GCI/PresentationLayer.h>

namespace Ic3::Graphics::GCI
{

	Ic3DeclareClassHandle( MetalPresentationLayer );
	Ic3DeclareClassHandle( MetalScreenPresentationLayer );

	struct MetalPresentationLayerCreateInfo : public PresentationLayerCreateInfo
	{
	};

	/// @brief
	class MetalPresentationLayer : public PresentationLayer
	{
	public:
		System::MetalDisplaySurfaceHandle const mSysMetalDisplaySurface;

	public:
		MetalPresentationLayer( MetalGPUDevice & pGPUDevice, System::MetalDisplaySurfaceHandle pSysMetalDisplaySurface );
		virtual ~MetalPresentationLayer();

		virtual System::EventSource * GetInternalSystemEventSource() const noexcept override;
	};

	class GLScreenPresentationLayer final : public MetalPresentationLayer
	{
	public:
		GLScreenPresentationLayer( MetalGPUDevice & pGPUDevice, System::MetalDisplaySurfaceHandle pSysMetalDisplaySurface );
		virtual ~GLScreenPresentationLayer();

		virtual void BindRenderTarget( CommandContext & pCommandContext ) override;

		virtual void InvalidateRenderTarget( CommandContext & pCommandContext ) override;

		virtual void Present() override;

		virtual void resize( uint32 pWidth, uint32 pHeight ) override;

		virtual void SetFullscreenMode( bool pEnable ) override;

		virtual cxm::vec2u32 QueryRenderTargetSize() const override;

		/// @brief Creates new swap chain using provided create params.
		static GLScreenPresentationLayerHandle Create( GLGPUDevice & pDevice, const MetalPresentationLayerCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_MTLCOMMON_PRESENTATION_LAYER_H__
