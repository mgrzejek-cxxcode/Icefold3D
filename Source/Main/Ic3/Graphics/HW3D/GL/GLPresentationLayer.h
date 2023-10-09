
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_PRESENTATION_LAYER_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_PRESENTATION_LAYER_H__

#include "GLPrerequisites.h"
#include <Ic3/Graphics/GCI/PresentationLayer.h>
#include <Ic3/Graphics/GCI/State/CommonGPUStateDefs.h>

namespace Ic3::Graphics::GCI
{

	ic3DeclareClassHandle( GLPresentationLayer );
	ic3DeclareClassHandle( GLScreenPresentationLayer );

	struct GLPresentationLayerCreateInfo : public PresentationLayerCreateInfo
	{
	};

	/// @brief
	class ICFGX_GL_CLASS GLPresentationLayer : public PresentationLayer
	{
	public:
		System::OpenGLDisplaySurfaceHandle const mSysGLDisplaySurface;

	public:
	    GLPresentationLayer( GLGPUDevice & pGPUDevice, System::OpenGLDisplaySurfaceHandle pSysGLDisplaySurface );
		virtual ~GLPresentationLayer();

		virtual System::EventSource * getInternalSystemEventSource() const noexcept override;
	};

	class GLScreenPresentationLayer final : public GLPresentationLayer
	{
		friend GLGPUDevice;

	public:
		RenderTargetBindingImmutableStateHandle const mScreenRenderTargetBindingState;

	public:
	    GLScreenPresentationLayer(
			GLGPUDevice & pGPUDevice,
			System::OpenGLDisplaySurfaceHandle pSysGLDisplaySurface,
			RenderTargetBindingImmutableStateHandle pScreenRenderTargetBindingState );

		virtual ~GLScreenPresentationLayer();

		virtual void bindRenderTarget( CommandContext * pCmdContext ) override;

		virtual void invalidateRenderTarget( CommandContext * pCmdContext ) override;

		virtual void present() override;

		virtual void resize( uint32 pWidth, uint32 pHeight ) override;

		virtual void setFullscreenMode( bool pEnable ) override;

		virtual Ic3::Math::Vec2u32 queryRenderTargetSize() const override;

		/// @brief Creates new swap chain using provided create params.
		static GLScreenPresentationLayerHandle create( GLGPUDevice & pDevice, const GLPresentationLayerCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_PRESENTATION_LAYER_H__
