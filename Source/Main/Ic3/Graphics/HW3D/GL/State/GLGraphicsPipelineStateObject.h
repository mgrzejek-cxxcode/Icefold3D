
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLC_GRAPHICS_PIPELINE_STATE_OBJECT_H__
#define __IC3_GRAPHICS_HW3D_GLC_GRAPHICS_PIPELINE_STATE_OBJECT_H__

#include "../GLPrerequisites.h"
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateImplSeparableState.h>
#include <Ic3/Graphics/GCI/State/PipelineStateImplRootSignatureGeneric.h>

namespace Ic3::Graphics::GCI
{

	Ic3GLDeclareOpenGLObjectHandle( GLShaderPipelineObject );
	Ic3GLDeclareOpenGLObjectHandle( GLShaderProgramObject );

	class GLGraphicsPipelineStateObject : public GraphicsPipelineStateObjectSeparable
	{
	public:
		GLGraphicsPipelineStateObject(
				GLGPUDevice & pGPUDevice,
				pipeline_state_object_id_t pStateObjectID,
				const RenderTargetLayout & pRenderTargetLayout,
				const SeparableGraphicsPSDSet & pSeparableDescriptors );

		virtual ~GLGraphicsPipelineStateObject();

		CPPX_ATTR_NO_DISCARD const GLBlendStateDescriptor & GetBlendStateDescriptor() const noexcept;

		CPPX_ATTR_NO_DISCARD const GLDepthStencilStateDescriptor & GetDepthStencilStateDescriptor() const noexcept;

		CPPX_ATTR_NO_DISCARD const GLRasterizerStateDescriptor & GetRasterizerStateDescriptor() const noexcept;

		CPPX_ATTR_NO_DISCARD const RootSignatureDescriptorGeneric & GetRootSignatureDescriptor() const noexcept;

		CPPX_ATTR_NO_DISCARD const GLGraphicsShaderLinkageDescriptor & GetShaderLinkageDescriptor() const noexcept;

		CPPX_ATTR_NO_DISCARD const GLVertexAttributeLayoutDescriptor & GetVertexAttributeLayoutDescriptor() const noexcept;

		static TGfxHandle<GLGraphicsPipelineStateObject> Create(
				GLGPUDevice & pGPUDevice,
				const GraphicsPipelineStateObjectCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLC_GRAPHICS_PIPELINE_STATE_OBJECT_H__
