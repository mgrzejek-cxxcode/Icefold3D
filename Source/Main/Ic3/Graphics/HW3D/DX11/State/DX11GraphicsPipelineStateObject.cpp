
#include "DX11GraphicsPipelineStateObject.h"
#include "DX11GraphicsPipelineStateCommon.h"
#include "DX11GraphicsPipelineStateIA.h"
#include "DX11GraphicsPipelineStateShader.h"
#include "DX11PipelineStateDescriptorFactory.h"
#include <Ic3/Graphics/HW3D/DX11/DX11GPUDevice.h>
#include <Ic3/Graphics/HW3D/DX11/Resources/DX11Shader.h>

namespace Ic3::Graphics::GCI
{

	DX11GraphicsPipelineStateObject::DX11GraphicsPipelineStateObject(
			DX11GPUDevice & pGPUDevice,
			pipeline_state_object_id_t pStateObjectID,
			const RenderTargetLayout & pRenderTargetLayout,
			const SeparableGraphicsPSDSet & pSeparableDescriptors,
			const GraphicsShaderBinding & pGraphicsShaderBinding )
	: GraphicsPipelineStateObjectSeparableShader(
		pGPUDevice,
		pStateObjectID,
		pRenderTargetLayout,
		pSeparableDescriptors,
		pGraphicsShaderBinding )
	{}

	DX11GraphicsPipelineStateObject::~DX11GraphicsPipelineStateObject() = default;

	const DX11BlendStateDescriptor & DX11GraphicsPipelineStateObject::GetDX11BlendStateDescriptor() const noexcept
	{
		Ic3DebugAssert( mSeparableDescriptors.blendStateDescriptor );
		return *( mSeparableDescriptors.blendStateDescriptor->QueryInterface<DX11BlendStateDescriptor>() );
	}

	const DX11DepthStencilStateDescriptor & DX11GraphicsPipelineStateObject::GetDX11DepthStencilStateDescriptor() const noexcept
	{
		Ic3DebugAssert( mSeparableDescriptors.depthStencilStateDescriptor );
		return *( mSeparableDescriptors.depthStencilStateDescriptor->QueryInterface<DX11DepthStencilStateDescriptor>() );
	}

	const DX11RasterizerStateDescriptor & DX11GraphicsPipelineStateObject::GetDX11RasterizerStateDescriptor() const noexcept
	{
		Ic3DebugAssert( mSeparableDescriptors.rasterizerStateDescriptor );
		return *( mSeparableDescriptors.rasterizerStateDescriptor->QueryInterface<DX11RasterizerStateDescriptor>() );
	}

	const DX11GraphicsShaderLinkageDescriptor & DX11GraphicsPipelineStateObject::GetDX11ShaderLinkageDescriptor() const noexcept
	{
		Ic3DebugAssert( mSeparableDescriptors.shaderLinkageStateDescriptor );
		return *( mSeparableDescriptors.shaderLinkageStateDescriptor->QueryInterface<DX11GraphicsShaderLinkageDescriptor>() );
	}

	const DX11VertexAttributeLayoutDescriptor & DX11GraphicsPipelineStateObject::GetDX11VertexAttributeLayoutDescriptor() const noexcept
	{
		Ic3DebugAssert( mSeparableDescriptors.vertexAttributeLayoutDescriptor );
		return *( mSeparableDescriptors.vertexAttributeLayoutDescriptor->QueryInterface<DX11VertexAttributeLayoutDescriptor>() );
	}

	const DX11RootSignatureDescriptor & DX11GraphicsPipelineStateObject::GetDX11RootSignatureDescriptor() const noexcept
	{
		Ic3DebugAssert( mSeparableDescriptors.rootSignatureDescriptor );
		return *( mSeparableDescriptors.rootSignatureDescriptor->QueryInterface<DX11RootSignatureDescriptor>() );
	}

	TGfxHandle<DX11GraphicsPipelineStateObject> DX11GraphicsPipelineStateObject::CreateInstance(
			DX11GPUDevice & pGPUDevice,
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

		const auto * separableShaderLinkageDescriptor = pCreateInfo.shaderLinkageStateDescriptor->QueryInterface<DX11GraphicsShaderLinkageDescriptor>();

		auto pipelineStateObject = CreateDynamicObject<DX11GraphicsPipelineStateObject>(
			pGPUDevice,
			pCreateInfo.baseObjectID,
			pCreateInfo.renderTargetLayout,
			separableDescriptorSet,
			separableShaderLinkageDescriptor->mShaderBinding );

		return pipelineStateObject;
	}

} // namespace Ic3::Graphics::GCI
