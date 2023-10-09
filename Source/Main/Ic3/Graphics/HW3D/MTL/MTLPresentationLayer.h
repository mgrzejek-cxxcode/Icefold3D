
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_MTLCOMMON_PRESENTATION_LAYER_H__
#define __IC3_GRAPHICS_HW3D_MTLCOMMON_PRESENTATION_LAYER_H__

#include "MTLPrerequisites.h"
#include <Ic3/Graphics/GCI/PresentationLayer.h>

namespace Ic3::Graphics::GCI
{

	ic3DeclareClassHandle( MetalPresentationLayer );
	ic3DeclareClassHandle( MetalScreenPresentationLayer );

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

		virtual System::EventSource * getInternalSystemEventSource() const noexcept override;
	};

	class GLScreenPresentationLayer final : public MetalPresentationLayer
	{
	public:
		GLScreenPresentationLayer( MetalGPUDevice & pGPUDevice, System::MetalDisplaySurfaceHandle pSysMetalDisplaySurface );
		virtual ~GLScreenPresentationLayer();

		virtual void bindRenderTarget( CommandContext * pCmdContext ) override;

		virtual void invalidateRenderTarget( CommandContext * pCmdContext ) override;

		virtual void present() override;

		virtual void resize( uint32 pWidth, uint32 pHeight ) override;

		virtual void setFullscreenMode( bool pEnable ) override;

		virtual Ic3::Math::Vec2u32 queryRenderTargetSize() const override;

		/// @brief Creates new swap chain using provided create params.
		static GLScreenPresentationLayerHandle create( GLGPUDevice & pDevice, const MetalPresentationLayerCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_MTLCOMMON_PRESENTATION_LAYER_H__
