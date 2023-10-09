
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

	const GLBlendImmutableState & GLGraphicsPipelineStateObject::getBlendState() const noexcept
	{
		ic3DebugAssert( mSeparableStates.blendState );
		return *( mSeparableStates.blendState->queryInterface<GLBlendImmutableState>() );
	}

	const GLDepthStencilImmutableState & GLGraphicsPipelineStateObject::getDepthStencilState() const noexcept
	{
		ic3DebugAssert( mSeparableStates.depthStencilState );
		return *( mSeparableStates.depthStencilState->queryInterface<GLDepthStencilImmutableState>() );
	}

	const GLRasterizerImmutableState & GLGraphicsPipelineStateObject::getRasterizerState() const noexcept
	{
		ic3DebugAssert( mSeparableStates.rasterizerState );
		return *( mSeparableStates.rasterizerState->queryInterface<GLRasterizerImmutableState>() );
	}

	const GLGraphicsShaderLinkageImmutableState & GLGraphicsPipelineStateObject::getGraphicsShaderLinkageState() const noexcept
	{
		ic3DebugAssert( mSeparableStates.shaderLinkageState );
		return *( mSeparableStates.shaderLinkageState->queryInterface<GLGraphicsShaderLinkageImmutableState>() );
	}

	const GLIAInputLayoutImmutableState & GLGraphicsPipelineStateObject::getIAInputLayoutState() const noexcept
	{
		ic3DebugAssert( mSeparableStates.iaInputLayoutState );
		return *( mSeparableStates.iaInputLayoutState->queryInterface<GLIAInputLayoutImmutableState>() );
	}

	GpaHandle<GLGraphicsPipelineStateObject> GLGraphicsPipelineStateObject::create(
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

		auto graphicsPSO = createGPUAPIObject<GLGraphicsPipelineStateObject>(
				pGPUDevice,
				std::move( renderTargetLayout ),
				std::move( shaderInputSignature ),
				separableStates );

		return graphicsPSO;
	}

} // namespace Ic3::Graphics::GCI
