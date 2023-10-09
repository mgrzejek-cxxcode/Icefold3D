
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_RENDER_TARGET_STATE_OBJECT_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_RENDER_TARGET_STATE_OBJECT_H__

#include "../GLPrerequisites.h"
#include <Ic3/Graphics/GCI/State/RenderTargetImmutableStates.h>

namespace Ic3::Graphics::GCI
{

	ic3GLDeclareOpenGLObjectHandle( GLFramebufferObject );
	ic3GLDeclareOpenGLObjectHandle( GLRenderbufferObject );

	struct GLRenderTargetBindingFBOData
	{
		GLFramebufferObjectHandle renderFBO;
		GLFramebufferObjectHandle resolveFBO;
	};

	struct GLRenderTargetBindingDefinition
	{
		GLRenderTargetBindingFBOData fboData;
		RenderTargetLayout rtLayout;
	};

	struct GLRenderTargetBindingInfo
	{
		const GLFramebufferObject * renderFBO = nullptr;
		const GLFramebufferObject * resolveFBO = nullptr;
		const RenderTargetLayout * rtLayout = nullptr;
	};

	class GLRenderTargetBindingImmutableState : public RenderTargetBindingImmutableState
	{
	public:
		GLRenderTargetBindingFBOData const mGLFBOData;

	public:
		GLRenderTargetBindingImmutableState(
				GLGPUDevice & pGPUDevice,
				const RenderTargetLayout & pRenderTargetLayout,
				GLRenderTargetBindingFBOData pGLFBOData );

		virtual ~GLRenderTargetBindingImmutableState();

		IC3_ATTR_NO_DISCARD GLRenderTargetBindingInfo getGLRenderTargetBindingInfo() const;

		static GpaHandle<GLRenderTargetBindingImmutableState> createInstance(
			GLGPUDevice & pGPUDevice,
			const RenderTargetBindingDefinition & pBindingDefinition );

		static GpaHandle<GLRenderTargetBindingImmutableState> createForScreen(
			GLGPUDevice & pGPUDevice,
			const RenderTargetLayout & pRenderTargetLayout );
	};


	namespace smutil
	{

		IC3_ATTR_NO_DISCARD GLRenderTargetBindingInfo getGLRenderTargetBindingInfo(
				const GLRenderTargetBindingDefinition & pBindingDefinition );

		IC3_ATTR_NO_DISCARD GLRenderTargetBindingDefinition translateRenderTargetBindingDefinition(
				const RenderTargetBindingDefinition & pBindingDefinition );

		IC3_ATTR_NO_DISCARD GLFramebufferObjectHandle createFramebufferObject(
				const RenderTargetBindingDefinition & pBindingDefinition,
				Bitmask<ERTAttachmentFlags> pAttachmentMask );

		IC3_ATTR_NO_DISCARD RenderTargetLayout translateSystemVisualConfigToRenderTargetLayout( const System::VisualConfig & pSysVisualConfig );

		void clearRenderPassFramebuffer(
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicState & pDynamicState );

		void resolveRenderPassFramebuffer(
				const GLRenderTargetBindingInfo & pRTBindingInfo,
				const RenderPassConfiguration & pRenderPassConfiguration );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_RENDER_TARGET_STATE_OBJECT_H__
