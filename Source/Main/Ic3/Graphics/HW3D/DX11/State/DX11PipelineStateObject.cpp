
#include "DX11pipelineStateObject.h"
#include "DX11CommonGraphicsConfig.h"
#include "DX11inputAssembler.h"
#include "DX11graphicsShaderState.h"
#include "../DX11gpuDevice.h"
#include "../Resources/DX11Shader.h"
#include <Ic3/Graphics/GCI/Resources/Shader.h>

namespace Ic3::Graphics::GCI
{

	DX11GraphicsPipelineStateObject::DX11GraphicsPipelineStateObject(
			DX11GPUDevice & pGPUDevice,
			RenderTargetLayout pRenderTargetLayout,
			ShaderInputSignature pShaderInputSignature,
			const SeparableGraphicsPSDSet & pPSOCompiledStates,
			const GraphicsShaderSet & pSeparableShaders )
	: GraphicsPipelineStateObjectSeparableShader(
			pGPUDevice,
			std::move( pRenderTargetLayout ),
			std::move( pShaderInputSignature ),
			pPSOCompiledStates,
			pSeparableShaders )
	{}

	DX11GraphicsPipelineStateObject::~DX11GraphicsPipelineStateObject() = default;

	const DX11BlendStateDescriptor & DX11GraphicsPipelineStateObject::GetBlendState() const noexcept
	{
		Ic3DebugAssert( mSeparableStates.blendState );
		return *( mSeparableStates.blendState->QueryInterface<DX11BlendStateDescriptor>() );
	}

	const DX11DepthStencilStateDescriptor & DX11GraphicsPipelineStateObject::GetDepthStencilState() const noexcept
	{
		Ic3DebugAssert( mSeparableStates.depthStencilState );
		return *( mSeparableStates.depthStencilState->QueryInterface<DX11DepthStencilStateDescriptor>() );
	}

	const DX11RasterizerStateDescriptor & DX11GraphicsPipelineStateObject::GetRasterizerState() const noexcept
	{
		Ic3DebugAssert( mSeparableStates.rasterizerState );
		return *( mSeparableStates.rasterizerState->QueryInterface<DX11RasterizerStateDescriptor>() );
	}

	const DX11GraphicsShaderLinkageCompiledState & DX11GraphicsPipelineStateObject::GetGraphicsShaderLinkageState() const noexcept
	{
		Ic3DebugAssert( mSeparableStates.shaderLinkageState );
		return *( mSeparableStates.shaderLinkageState->QueryInterface<DX11GraphicsShaderLinkageCompiledState>() );
	}

	const DX11VertexAttributeLayoutDescriptor & DX11GraphicsPipelineStateObject::GetIAVertexAttributeLayout() const noexcept
	{
		Ic3DebugAssert( mSeparableStates.iaInputLayout );
		return *( mSeparableStates.iaInputLayout->QueryInterface<DX11VertexAttributeLayoutDescriptor>() );
	}

	GpaHandle<DX11GraphicsPipelineStateObject> DX11GraphicsPipelineStateObject::Create(
			DX11GPUDevice & pGPUDevice,
			const GraphicsPipelineStateObjectCreateInfo & pCreateInfo )
	{
		if( pCreateInfo.shaderSet.empty() && !pCreateInfo.shaderLinkageState )
		{
			return nullptr;
		}

		SeparableGraphicsPSDSet separableStates{};
		separableStates.blendState = pCreateInfo.blendState;
		separableStates.depthStencilState = pCreateInfo.depthStencilState;
		separableStates.rasterizerState = pCreateInfo.rasterizerState;
		separableStates.shaderLinkageState = pCreateInfo.shaderLinkageState;
		separableStates.iaInputLayout = pCreateInfo.vertexAttributeLayout;

		auto & renderTargetLayout = pCreateInfo.renderTargetLayout;
		auto & shaderInputSignature = pCreateInfo.shaderInputSignature;
		auto * separableShaderState = pCreateInfo.shaderLinkageState->QueryInterface<GraphicsShaderLinkageCompiledStateSeparable>();

		auto pipelineStateObject = CreateDynamicObject<DX11GraphicsPipelineStateObject>(
				pGPUDevice,
				std::move( renderTargetLayout ),
				std::move( shaderInputSignature ),
				separableStates,
			separableShaderState->mShaderSet );

		return pipelineStateObject;
	}

} // namespace Ic3::Graphics::GCI
