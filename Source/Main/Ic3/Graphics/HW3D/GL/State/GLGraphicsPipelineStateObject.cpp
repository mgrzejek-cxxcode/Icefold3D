
#include "GLGraphicsPipelineStateObject.h"
#include "GLGraphicsPipelineStateCommon.h"
#include "GLGraphicsPipelineStateIA.h"
#include "GLGraphicsPipelineStateShader.h"
#include <Ic3/Graphics/GCI/State/PipelineStateDescriptorRootSignature.h>
#include <Ic3/Graphics/HW3D/GL/GLGPUDevice.h>
#include <Ic3/Graphics/HW3D/GL/Resources/GLShader.h>

namespace Ic3::Graphics::GCI
{

	GLGraphicsPipelineStateObject::GLGraphicsPipelineStateObject(
			GLGPUDevice & pGPUDevice,
			pipeline_state_object_id_t pStateObjectID,
			const RenderTargetLayout & pRenderTargetLayout,
			const SeparableGraphicsPSDSet & pSeparableDescriptors )
	: GraphicsPipelineStateObjectSeparable(
		pGPUDevice,
		pStateObjectID,
		pRenderTargetLayout,
		pSeparableDescriptors )
	{}

	GLGraphicsPipelineStateObject::~GLGraphicsPipelineStateObject() = default;

	const GLBlendStateDescriptor & GLGraphicsPipelineStateObject::GetGLBlendStateDescriptor() const noexcept
	{
		Ic3DebugAssert( mSeparableDescriptors.blendStateDescriptor );
		return *( mSeparableDescriptors.blendStateDescriptor->QueryInterface<GLBlendStateDescriptor>() );
	}

	const GLDepthStencilStateDescriptor & GLGraphicsPipelineStateObject::GetGLDepthStencilStateDescriptor() const noexcept
	{
		Ic3DebugAssert( mSeparableDescriptors.depthStencilStateDescriptor );
		return *( mSeparableDescriptors.depthStencilStateDescriptor->QueryInterface<GLDepthStencilStateDescriptor>() );
	}

	const GLRasterizerStateDescriptor & GLGraphicsPipelineStateObject::GetGLRasterizerStateDescriptor() const noexcept
	{
		Ic3DebugAssert( mSeparableDescriptors.rasterizerStateDescriptor );
		return *( mSeparableDescriptors.rasterizerStateDescriptor->QueryInterface<GLRasterizerStateDescriptor>() );
	}

	const GLGraphicsShaderLinkageDescriptor & GLGraphicsPipelineStateObject::GetGLShaderLinkageDescriptor() const noexcept
	{
		Ic3DebugAssert( mSeparableDescriptors.shaderLinkageStateDescriptor );
		return *( mSeparableDescriptors.shaderLinkageStateDescriptor->QueryInterface<GLGraphicsShaderLinkageDescriptor>() );
	}

	const GLVertexAttributeLayoutDescriptor & GLGraphicsPipelineStateObject::GetGLVertexAttributeLayoutDescriptor() const noexcept
	{
		Ic3DebugAssert( mSeparableDescriptors.vertexAttributeLayoutDescriptor );
		return *( mSeparableDescriptors.vertexAttributeLayoutDescriptor->QueryInterface<GLVertexAttributeLayoutDescriptor>() );
	}

	const GLRootSignatureDescriptor & GLGraphicsPipelineStateObject::GetGLRootSignatureDescriptor() const noexcept
	{
		Ic3DebugAssert( mSeparableDescriptors.rootSignatureDescriptor );
		return *( mSeparableDescriptors.rootSignatureDescriptor->QueryInterface<RootSignatureDescriptorGeneric>() );
	}

	TGfxHandle<GLGraphicsPipelineStateObject> GLGraphicsPipelineStateObject::CreateInstance(
			GLGPUDevice & pGPUDevice,
			const GraphicsPipelineStateObjectCreateInfo & pCreateInfo )
	{
		SeparableGraphicsPSDSet separableDescriptorSet{};
		separableDescriptorSet.blendStateDescriptor = pCreateInfo.blendStateDescriptor;
		separableDescriptorSet.depthStencilStateDescriptor = pCreateInfo.depthStencilStateDescriptor;
		separableDescriptorSet.rasterizerStateDescriptor = pCreateInfo.rasterizerStateDescriptor;
		separableDescriptorSet.rootSignatureDescriptor = pCreateInfo.rootSignatureDescriptor;
		separableDescriptorSet.shaderLinkageStateDescriptor = pCreateInfo.shaderLinkageStateDescriptor;
		separableDescriptorSet.vertexAttributeLayoutDescriptor = pCreateInfo.vertexAttributeLayoutDescriptor;

		const auto & renderTargetLayout = pCreateInfo.renderTargetLayout;

		auto graphicsPSO = CreateGfxObject<GLGraphicsPipelineStateObject>(
				pGPUDevice,
				pCreateInfo.baseObjectID,
				pCreateInfo.renderTargetLayout,
				separableDescriptorSet );

		return graphicsPSO;
	}

} // namespace Ic3::Graphics::GCI
