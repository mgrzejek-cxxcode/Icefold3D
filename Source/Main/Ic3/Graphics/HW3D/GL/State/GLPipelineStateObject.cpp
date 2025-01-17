
#include "GLPipelineStateObject.h"
#include "GLCommonGraphicsConfig.h"
#include "GLInputAssembler.h"
#include "GLGraphicsShaderState.h"
#include <Ic3/Graphics/HW3D/GL/GLGPUDevice.h>
#include <Ic3/Graphics/HW3D/GL/Objects/GLVertexArrayObject.h>
#include <Ic3/Graphics/HW3D/GL/Resources/GLShader.h>
#include <Ic3/Graphics/GCI/Resources/Shader.h>

namespace Ic3::Graphics::GCI
{

	GLGraphicsPipelineStateObject::GLGraphicsPipelineStateObject(
			GLGPUDevice & pGPUDevice,
			RenderTargetLayout pRenderTargetLayout,
			ShaderInputSignature pShaderInputSignature,
			const SeparablePSOStateSet & pPSOImmutableStates )
	: GraphicsPipelineStateObjectSeparable(
		pGPUDevice,
		std::move( pRenderTargetLayout ),
		std::move( pShaderInputSignature ),
		pPSOImmutableStates )
	{}

	GLGraphicsPipelineStateObject::~GLGraphicsPipelineStateObject() = default;

	const GLBlendImmutableState & GLGraphicsPipelineStateObject::GetBlendState() const noexcept
	{
		Ic3DebugAssert( mSeparableStates.blendState );
		return *( mSeparableStates.blendState->QueryInterface<GLBlendImmutableState>() );
	}

	const GLDepthStencilImmutableState & GLGraphicsPipelineStateObject::GetDepthStencilState() const noexcept
	{
		Ic3DebugAssert( mSeparableStates.depthStencilState );
		return *( mSeparableStates.depthStencilState->QueryInterface<GLDepthStencilImmutableState>() );
	}

	const GLRasterizerImmutableState & GLGraphicsPipelineStateObject::GetRasterizerState() const noexcept
	{
		Ic3DebugAssert( mSeparableStates.rasterizerState );
		return *( mSeparableStates.rasterizerState->QueryInterface<GLRasterizerImmutableState>() );
	}

	const GLGraphicsShaderLinkageImmutableState & GLGraphicsPipelineStateObject::GetGraphicsShaderLinkageState() const noexcept
	{
		Ic3DebugAssert( mSeparableStates.shaderLinkageState );
		return *( mSeparableStates.shaderLinkageState->QueryInterface<GLGraphicsShaderLinkageImmutableState>() );
	}

	const GLIAInputLayoutImmutableState & GLGraphicsPipelineStateObject::GetIAInputLayoutState() const noexcept
	{
		Ic3DebugAssert( mSeparableStates.iaInputLayoutState );
		return *( mSeparableStates.iaInputLayoutState->QueryInterface<GLIAInputLayoutImmutableState>() );
	}

	TGfxHandle<GLGraphicsPipelineStateObject> GLGraphicsPipelineStateObject::Create(
			GLGPUDevice & pGPUDevice,
			const GraphicsPipelineStateObjectCreateInfo & pCreateInfo )
	{
		SeparablePSOStateSet separableStates{};
		separableStates.blendState = pCreateInfo.blendState;
		separableStates.depthStencilState = pCreateInfo.depthStencilState;
		separableStates.rasterizerState = pCreateInfo.rasterizerState;
		separableStates.shaderLinkageState = pCreateInfo.shaderLinkageState;
		separableStates.iaInputLayoutState = pCreateInfo.inputLayoutState;

		auto & renderTargetLayout = pCreateInfo.renderTargetLayout;
		auto & shaderInputSignature = pCreateInfo.shaderInputSignature;

		auto graphicsPSO = CreateGfxObject<GLGraphicsPipelineStateObject>(
				pGPUDevice,
				std::move( renderTargetLayout ),
				std::move( shaderInputSignature ),
				separableStates );

		return graphicsPSO;
	}

} // namespace Ic3::Graphics::GCI
