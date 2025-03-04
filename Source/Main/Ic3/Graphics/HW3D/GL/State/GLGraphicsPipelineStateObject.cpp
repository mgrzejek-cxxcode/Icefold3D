
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

	const GLBlendStateDescriptor & GLGraphicsPipelineStateObject::GetBlendStateDescriptor() const noexcept
	{
		Ic3DebugAssert( mSeparableDescriptors.blendStateDescriptor );
		return *( mSeparableDescriptors.blendStateDescriptor->QueryInterface<GLBlendStateDescriptor>() );
	}

	const GLDepthStencilStateDescriptor & GLGraphicsPipelineStateObject::GetDepthStencilStateDescriptor() const noexcept
	{
		Ic3DebugAssert( mSeparableDescriptors.depthStencilStateDescriptor );
		return *( mSeparableDescriptors.depthStencilStateDescriptor->QueryInterface<GLDepthStencilStateDescriptor>() );
	}

	const GLRasterizerStateDescriptor & GLGraphicsPipelineStateObject::GetRasterizerStateDescriptor() const noexcept
	{
		Ic3DebugAssert( mSeparableDescriptors.rasterizerStateDescriptor );
		return *( mSeparableDescriptors.rasterizerStateDescriptor->QueryInterface<GLRasterizerStateDescriptor>() );
	}

	const PIM::RootSignatureDescriptorGeneric & GLGraphicsPipelineStateObject::GetRootSignatureDescriptor() const noexcept
	{
		Ic3DebugAssert( mSeparableDescriptors.rootSignatureDescriptor );
		return *( mSeparableDescriptors.rootSignatureDescriptor->QueryInterface<PIM::RootSignatureDescriptorGeneric>() );
	}

	const GLGraphicsShaderLinkageDescriptor & GLGraphicsPipelineStateObject::GetShaderLinkageDescriptor() const noexcept
	{
		Ic3DebugAssert( mSeparableDescriptors.shaderLinkageStateDescriptor );
		return *( mSeparableDescriptors.shaderLinkageStateDescriptor->QueryInterface<GLGraphicsShaderLinkageDescriptor>() );
	}

	const GLVertexAttributeLayoutDescriptor & GLGraphicsPipelineStateObject::GetVertexAttributeLayoutDescriptor() const noexcept
	{
		Ic3DebugAssert( mSeparableDescriptors.vertexAttributeLayoutDescriptor );
		return *( mSeparableDescriptors.vertexAttributeLayoutDescriptor->QueryInterface<GLVertexAttributeLayoutDescriptor>() );
	}

	TGfxHandle<GLGraphicsPipelineStateObject> GLGraphicsPipelineStateObject::Create(
			GLGPUDevice & pGPUDevice,
			const GraphicsPipelineStateObjectCreateInfo & pCreateInfo )
	{
		SeparableGraphicsPSDSet separableStates{};
		separableStates.blendStateDescriptor = pCreateInfo.blendStateDescriptor;
		separableStates.depthStencilStateDescriptor = pCreateInfo.depthStencilStateDescriptor;
		separableStates.rasterizerStateDescriptor = pCreateInfo.rasterizerStateDescriptor;
		separableStates.rootSignatureDescriptor = pCreateInfo.rootSignatureDescriptor;
		separableStates.shaderLinkageStateDescriptor = pCreateInfo.shaderLinkageStateDescriptor;
		separableStates.vertexAttributeLayoutDescriptor = pCreateInfo.vertexAttributeLayoutDescriptor;

		const auto & renderTargetLayout = pCreateInfo.renderTargetLayout;

		auto graphicsPSO = CreateGfxObject<GLGraphicsPipelineStateObject>(
				pGPUDevice,
				pCreateInfo.baseObjectID,
				pCreateInfo.renderTargetLayout,
				separableStates );

		return graphicsPSO;
	}

} // namespace Ic3::Graphics::GCI
