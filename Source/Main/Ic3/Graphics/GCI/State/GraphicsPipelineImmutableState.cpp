
#include "GraphicsPipelineImmutableState.h"
#include "GraphicsShaderLinkageImmutableState.h"
#include "RenderTargetImmutableStates.h"

namespace Ic3::Graphics::GCI
{

	GraphicsPipelineImmutableState::GraphicsPipelineImmutableState( GPUDevice & pGPUDevice )
	: GPUDeviceChildObject( pGPUDevice )
	{}

	GraphicsPipelineImmutableState::~GraphicsPipelineImmutableState() = default;

	EGraphicsPipelineImmutableStateType GraphicsPipelineImmutableState::QueryStateType() const noexcept
	{
		return EGraphicsPipelineImmutableStateType::Unknown;
	}

	bool GraphicsPipelineImmutableState::IsDynamicOverrideState() const noexcept
	{
		return false;
	}


	PipelineImmutableStateFactorySeparableShader::PipelineImmutableStateFactorySeparableShader( GPUDevice & pGPUDevice )
	: mGPUDevice( pGPUDevice )
	{}

	PipelineImmutableStateFactorySeparableShader::~PipelineImmutableStateFactorySeparableShader() = default;

	GraphicsShaderLinkageImmutableStateHandle PipelineImmutableStateFactorySeparableShader::CreateGraphicsShaderLinkageState(
			const GraphicsShaderSet & pShaderSet )
	{
		if( !pShaderSet[EShaderType::GSVertex] || !pShaderSet[EShaderType::GSPixel] )
		{
			return nullptr;
		}

		if( !pShaderSet.ValidateShaders() )
		{
			return nullptr;
		}

		const auto commonProperties = SMU::GetGraphicsShaderLinkageCommonPropertiesForShaderSet( pShaderSet );

		const auto shaderImmutableState = CreateGfxObject<GraphicsShaderLinkageImmutableStateSeparable>(
				mGPUDevice,
				commonProperties,
				pShaderSet );

		return shaderImmutableState;
	}


	RenderPassConfigurationImmutableStateHandle PipelineImmutableStateFactory::CreateRenderPassStateDefault(
			GPUDevice & pGPUDevice,
			const RenderPassConfiguration & pConfiguration )
	{
		return RenderPassConfigurationImmutableStateDefault::CreateInstance( pGPUDevice, pConfiguration );
	}


	BlendImmutableStateHandle PipelineImmutableStateFactoryNull::CreateBlendState( const BlendConfig & )
	{
		return nullptr;
	}

	DepthStencilImmutableStateHandle PipelineImmutableStateFactoryNull::CreateDepthStencilState( const DepthStencilConfig & )
	{
		return nullptr;
	}

	GraphicsShaderLinkageImmutableStateHandle PipelineImmutableStateFactoryNull::CreateGraphicsShaderLinkageState( const GraphicsShaderSet & )
	{
		return nullptr;
	}

	IAInputLayoutImmutableStateHandle PipelineImmutableStateFactoryNull::CreateIAInputLayoutState( const IAInputLayoutDefinition &, Shader & )
	{
		return nullptr;
	}

	IAVertexStreamImmutableStateHandle PipelineImmutableStateFactoryNull::CreateIAVertexStreamState( const IAVertexStreamDefinition & )
	{
		return nullptr;
	}

	RasterizerImmutableStateHandle PipelineImmutableStateFactoryNull::CreateRasterizerState( const RasterizerConfig & )
	{
		return nullptr;
	}

	RenderTargetBindingImmutableStateHandle PipelineImmutableStateFactoryNull::CreateRenderTargetBindingState( const RenderTargetBindingDefinition & )
	{
		return nullptr;
	}

	RenderPassConfigurationImmutableStateHandle PipelineImmutableStateFactoryNull::CreateRenderPassState( const RenderPassConfiguration & )
	{
		return nullptr;
	}

} // namespace Ic3::Graphics::GCI
