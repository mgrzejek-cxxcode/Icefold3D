
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_PRESENTATION_LAYER_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_PRESENTATION_LAYER_H__

#include "GLPrerequisites.h"
#include <Ic3/Graphics/GCI/PresentationLayer.h>
#include <Ic3/Graphics/GCI/State/CommonGpuStateDefs.h>

namespace Ic3::Graphics::GCI
{

	ic3DeclareClassHandle( GLPresentationLayer );
	ic3DeclareClassHandle( GLScreenPresentationLayer );

	struct GLPresentationLayerCreateInfo : public PresentationLayerCreateInfo
	{
	};

	/// @brief
	class IC3_GX_GL_CLASS GLPresentationLayer : public PresentationLayer
	{
	public:
		System::OpenGLDisplaySurfaceHandle const mSysGLDisplaySurface;

	public:
	    GLPresentationLayer( GLGpuDevice & pGpuDevice, System::OpenGLDisplaySurfaceHandle pSysGLDisplaySurface );
		virtual ~GLPresentationLayer();

		virtual System::EventSource * GetInternalSystemEventSource() const noexcept override;
	};

	class GLScreenPresentationLayer final : public GLPresentationLayer
	{
		friend GLGpuDevice;

	public:
		RenderTargetBindingImmutableStateHandle const mScreenRenderTargetBindingState;

	public:
	    GLScreenPresentationLayer(
			GLGpuDevice & pGpuDevice,
			System::OpenGLDisplaySurfaceHandle pSysGLDisplaySurface,
			RenderTargetBindingImmutableStateHandle pScreenRenderTargetBindingState );

		virtual ~GLScreenPresentationLayer();

		virtual void BindRenderTarget( CommandContext * pCmdContext ) override;

		virtual void InvalidateRenderTarget( CommandContext * pCmdContext ) override;

		virtual void Present() override;

		virtual void Resize( uint32 pWidth, uint32 pHeight ) override;

		virtual void SetFullscreenMode( bool pEnable ) override;

		virtual Ic3::Math::Vec2u32 QueryRenderTargetSize() const override;

		/// @brief Creates new swap chain using provided Create params.
		static GLScreenPresentationLayerHandle Create( GLGpuDevice & pDevice, const GLPresentationLayerCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_PRESENTATION_LAYER_H__
