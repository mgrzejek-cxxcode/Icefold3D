
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLC_PRESENTATION_LAYER_H__
#define __IC3_GRAPHICS_HW3D_GLC_PRESENTATION_LAYER_H__

#include "GLPrerequisites.h"
#include <Ic3/Graphics/GCI/PresentationLayer.h>
#include <Ic3/Graphics/GCI/State/CommonGPUStateDefs.h>


namespace Ic3::Graphics::GCI
{

	Ic3DeclareClassHandle( GLPresentationLayer );
	Ic3DeclareClassHandle( GLScreenPresentationLayer );
	Ic3DeclareClassHandle( GLRenderTargetDescriptor );

	struct GLPresentationLayerCreateInfo : public PresentationLayerCreateInfo
	{
	};

	/// @brief
	class IC3_GX_GL_CLASS GLPresentationLayer : public PresentationLayer
	{
	public:
		System::OpenGLDisplaySurfaceHandle const mSysGLDisplaySurface;

	public:
	    GLPresentationLayer( GLGPUDevice & pGPUDevice, System::OpenGLDisplaySurfaceHandle pSysGLDisplaySurface );
		virtual ~GLPresentationLayer();

		virtual System::EventSource * GetInternalSystemEventSource() const noexcept override;
	};

	class GLScreenPresentationLayer final : public GLPresentationLayer
	{
		friend GLGPUDevice;

	public:
		GLRenderTargetDescriptorHandle const mScreenRenderTargetDescriptor;

	public:
		GLScreenPresentationLayer(
				GLGPUDevice & pGPUDevice,
				System::OpenGLDisplaySurfaceHandle pSysGLDisplaySurface,
				GLRenderTargetDescriptorHandle pScreenRenderTargetDescriptor );

		virtual ~GLScreenPresentationLayer();

		virtual void BindRenderTarget( CommandContext & pCommandContext ) override;

		virtual void InvalidateRenderTarget( CommandContext & pCommandContext ) override;

		virtual void Present() override;

		virtual void Resize( uint32 pWidth, uint32 pHeight ) override;

		virtual void SetFullscreenMode( bool pEnable ) override;

		virtual cxm::vec2u32 QueryRenderTargetSize() const override;

		/// @brief Creates new swap chain using provided Create params.
		static GLScreenPresentationLayerHandle Create( GLGPUDevice & pDevice, const GLPresentationLayerCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLC_PRESENTATION_LAYER_H__
