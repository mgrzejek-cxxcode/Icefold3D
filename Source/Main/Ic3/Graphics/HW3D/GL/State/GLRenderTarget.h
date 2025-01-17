
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_RENDER_TARGET_STATE_OBJECT_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_RENDER_TARGET_STATE_OBJECT_H__

#include "../GLPrerequisites.h"
#include <Ic3/Graphics/GCI/State/RenderTargetImmutableStates.h>

namespace Ic3::Graphics::GCI
{

	Ic3GLDeclareOpenGLObjectHandle( GLFramebufferObject );
	Ic3GLDeclareOpenGLObjectHandle( GLRenderbufferObject );

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
				GLGpuDevice & pGpuDevice,
				const RenderTargetLayout & pRenderTargetLayout,
				GLRenderTargetBindingFBOData pGLFBOData );

		virtual ~GLRenderTargetBindingImmutableState();

		CPPX_ATTR_NO_DISCARD GLRenderTargetBindingInfo GetGLRenderTargetBindingInfo() const;

		static TGfxHandle<GLRenderTargetBindingImmutableState> CreateInstance(
			GLGpuDevice & pGpuDevice,
			const RenderTargetBindingDefinition & pBindingDefinition );

		static TGfxHandle<GLRenderTargetBindingImmutableState> CreateForScreen(
			GLGpuDevice & pGpuDevice,
			const RenderTargetLayout & pRenderTargetLayout );
	};


	namespace SMU
	{

		CPPX_ATTR_NO_DISCARD GLRenderTargetBindingInfo GetGLRenderTargetBindingInfo(
				const GLRenderTargetBindingDefinition & pBindingDefinition );

		CPPX_ATTR_NO_DISCARD GLRenderTargetBindingDefinition TranslateRenderTargetBindingDefinition(
				const RenderTargetBindingDefinition & pBindingDefinition );

		CPPX_ATTR_NO_DISCARD GLFramebufferObjectHandle CreateFramebufferObject(
				const RenderTargetBindingDefinition & pBindingDefinition,
				cppx::bitmask<ERTAttachmentFlags> pAttachmentMask );

		CPPX_ATTR_NO_DISCARD RenderTargetLayout TranslateSystemVisualConfigToRenderTargetLayout( const System::VisualConfig & pSysVisualConfig );

		void ClearRenderPassFramebuffer(
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicState & pDynamicState );

		void ResolveRenderPassFramebuffer(
				const GLRenderTargetBindingInfo & pRTBindingInfo,
				const RenderPassConfiguration & pRenderPassConfiguration );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_RENDER_TARGET_STATE_OBJECT_H__
