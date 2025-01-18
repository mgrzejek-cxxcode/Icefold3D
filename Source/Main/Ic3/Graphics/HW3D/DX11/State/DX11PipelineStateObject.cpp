
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
			const SeparablePSOStateSet & pPSOImmutableStates,
			const GraphicsShaderSet & pSeparableShaders )
	: GraphicsPipelineStateObjectSeparableShader(
			pGPUDevice,
			std::move( pRenderTargetLayout ),
			std::move( pShaderInputSignature ),
			pPSOImmutableStates,
			pSeparableShaders )
	{}

	DX11GraphicsPipelineStateObject::~DX11GraphicsPipelineStateObject() = default;

	const DX11BlendImmutableState & DX11GraphicsPipelineStateObject::GetBlendState() const noexcept
	{
		Ic3DebugAssert( mSeparableStates.blendState );
		return *( mSeparableStates.blendState->QueryInterface<DX11BlendImmutableState>() );
	}

	const DX11DepthStencilImmutableState & DX11GraphicsPipelineStateObject::GetDepthStencilState() const noexcept
	{
		Ic3DebugAssert( mSeparableStates.depthStencilState );
		return *( mSeparableStates.depthStencilState->QueryInterface<DX11DepthStencilImmutableState>() );
	}

	const DX11RasterizerImmutableState & DX11GraphicsPipelineStateObject::GetRasterizerState() const noexcept
	{
		Ic3DebugAssert( mSeparableStates.rasterizerState );
		return *( mSeparableStates.rasterizerState->QueryInterface<DX11RasterizerImmutableState>() );
	}

	const DX11GraphicsShaderLinkageImmutableState & DX11GraphicsPipelineStateObject::GetGraphicsShaderLinkageState() const noexcept
	{
		Ic3DebugAssert( mSeparableStates.shaderLinkageState );
		return *( mSeparableStates.shaderLinkageState->QueryInterface<DX11GraphicsShaderLinkageImmutableState>() );
	}

	const DX11IAInputLayoutImmutableState & DX11GraphicsPipelineStateObject::GetIAInputLayoutState() const noexcept
	{
		Ic3DebugAssert( mSeparableStates.iaInputLayoutState );
		return *( mSeparableStates.iaInputLayoutState->QueryInterface<DX11IAInputLayoutImmutableState>() );
	}

	GpaHandle<DX11GraphicsPipelineStateObject> DX11GraphicsPipelineStateObject::Create(
			DX11GPUDevice & pGPUDevice,
			const GraphicsPipelineStateObjectCreateInfo & pCreateInfo )
	{
		if( pCreateInfo.shaderSet.empty() && !pCreateInfo.shaderLinkageState )
		{
			return nullptr;
		}

		SeparablePSOStateSet separableStates{};
		separableStates.blendState = pCreateInfo.blendState;
		separableStates.depthStencilState = pCreateInfo.depthStencilState;
		separableStates.rasterizerState = pCreateInfo.rasterizerState;
		separableStates.shaderLinkageState = pCreateInfo.shaderLinkageState;
		separableStates.iaInputLayoutState = pCreateInfo.inputLayoutState;

		auto & renderTargetLayout = pCreateInfo.renderTargetLayout;
		auto & shaderInputSignature = pCreateInfo.shaderInputSignature;
		auto * separableShaderState = pCreateInfo.shaderLinkageState->QueryInterface<GraphicsShaderLinkageImmutableStateSeparable>();

		auto pipelineStateObject = CreateDynamicObject<DX11GraphicsPipelineStateObject>(
				pGPUDevice,
				std::move( renderTargetLayout ),
				std::move( shaderInputSignature ),
				separableStates,
			separableShaderState->mShaderSet );

		return pipelineStateObject;
	}

} // namespace Ic3::Graphics::GCI
