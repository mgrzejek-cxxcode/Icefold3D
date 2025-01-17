
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_PIPELINE_STATE_OBJECT_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_PIPELINE_STATE_OBJECT_H__

#include "../GLPrerequisites.h"
#include <Ic3/Graphics/GCI/State/SeparablePipelineState.h>

namespace Ic3::Graphics::GCI
{

	Ic3GLDeclareOpenGLObjectHandle( GLShaderPipelineObject );
	Ic3GLDeclareOpenGLObjectHandle( GLShaderProgramObject );

	class GLGraphicsPipelineStateObject : public GraphicsPipelineStateObjectSeparable
	{
	public:
		GLGraphicsPipelineStateObject(
				GLGPUDevice & pGPUDevice,
				RenderTargetLayout pRenderTargetLayout,
				ShaderInputSignature pShaderInputSignature,
				const SeparablePSOStateSet & pPSOImmutableStates );

		virtual ~GLGraphicsPipelineStateObject();

		CPPX_ATTR_NO_DISCARD const GLBlendImmutableState & GetBlendState() const noexcept;

		CPPX_ATTR_NO_DISCARD const GLDepthStencilImmutableState & GetDepthStencilState() const noexcept;

		CPPX_ATTR_NO_DISCARD const GLRasterizerImmutableState & GetRasterizerState() const noexcept;

		CPPX_ATTR_NO_DISCARD const GLGraphicsShaderLinkageImmutableState & GetGraphicsShaderLinkageState() const noexcept;

		CPPX_ATTR_NO_DISCARD const GLIAInputLayoutImmutableState & GetIAInputLayoutState() const noexcept;

		static TGfxHandle<GLGraphicsPipelineStateObject> Create(
				GLGPUDevice & pGPUDevice,
				const GraphicsPipelineStateObjectCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_PIPELINE_STATE_OBJECT_H__
