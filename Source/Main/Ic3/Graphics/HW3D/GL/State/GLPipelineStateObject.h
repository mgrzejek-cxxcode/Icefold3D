
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_PIPELINE_STATE_OBJECT_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_PIPELINE_STATE_OBJECT_H__

#include "../GLPrerequisites.h"
#include <Ic3/Graphics/GCI/State/SeparablePipelineState.h>

namespace Ic3::Graphics::GCI
{

	ic3GLDeclareOpenGLObjectHandle( GLShaderPipelineObject );
	ic3GLDeclareOpenGLObjectHandle( GLShaderProgramObject );

	class GLGraphicsPipelineStateObject : public GraphicsPipelineStateObjectSeparable
	{
	public:
		GLGraphicsPipelineStateObject(
				GLGPUDevice & pGPUDevice,
				RenderTargetLayout pRenderTargetLayout,
				ShaderInputSignature pShaderInputSignature,
				const SeparablePSOStateSet & pPSOImmutableStates );

		virtual ~GLGraphicsPipelineStateObject();

		IC3_ATTR_NO_DISCARD const GLBlendImmutableState & getBlendState() const noexcept;

		IC3_ATTR_NO_DISCARD const GLDepthStencilImmutableState & getDepthStencilState() const noexcept;

		IC3_ATTR_NO_DISCARD const GLRasterizerImmutableState & getRasterizerState() const noexcept;

		IC3_ATTR_NO_DISCARD const GLGraphicsShaderLinkageImmutableState & getGraphicsShaderLinkageState() const noexcept;

		IC3_ATTR_NO_DISCARD const GLIAInputLayoutImmutableState & getIAInputLayoutState() const noexcept;

		static GpaHandle<GLGraphicsPipelineStateObject> create(
				GLGPUDevice & pGPUDevice,
				const GraphicsPipelineStateObjectCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_PIPELINE_STATE_OBJECT_H__
