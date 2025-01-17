
#include "GraphicsPipelineImmutableState.h"
#include "GraphicsShaderLinkageImmutableState.h"
#include "RenderTargetImmutableStates.h"

namespace Ic3::Graphics::GCI
{

	GraphicsPipelineImmutableState::GraphicsPipelineImmutableState( GpuDevice & pGpuDevice )
	: GpuDeviceChildObject( pGpuDevice )
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


	PipelineImmutableStateFactorySeparableShader::PipelineImmutableStateFactorySeparableShader( GpuDevice & pGpuDevice )
	: mGpuDevice( pGpuDevice )
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
				mGpuDevice,
				commonProperties,
				pShaderSet );

		return shaderImmutableState;
	}


	RenderPassConfigurationImmutableStateHandle PipelineImmutableStateFactory::CreateRenderPassStateDefault(
			GpuDevice & pGpuDevice,
			const RenderPassConfiguration & pConfiguration )
	{
		return RenderPassConfigurationImmutableStateDefault::CreateInstance( pGpuDevice, pConfiguration );
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


	PipelineImmutableStateFactoryAdapter::PipelineImmutableStateFactoryAdapter( PipelineImmutableStateFactory & pFactory )
	: _stateFactory( &pFactory )
	{}

	PipelineImmutableStateFactoryAdapter::~PipelineImmutableStateFactoryAdapter() = default;

	BlendImmutableStateHandle PipelineImmutableStateFactoryAdapter::CreateState( const BlendConfig & pConfig )
	{
		return _stateFactory->CreateBlendState( pConfig );
	}

	DepthStencilImmutableStateHandle PipelineImmutableStateFactoryAdapter::CreateState( const DepthStencilConfig & pConfig )
	{
		return _stateFactory->CreateDepthStencilState( pConfig );
	}

	GraphicsShaderLinkageImmutableStateHandle PipelineImmutableStateFactoryAdapter::CreateState( const GraphicsShaderSet & pShaderSet )
	{
		return _stateFactory->CreateGraphicsShaderLinkageState( pShaderSet );
	}

	IAInputLayoutImmutableStateHandle PipelineImmutableStateFactoryAdapter::CreateState( const IAInputLayoutDefinition & pDefinition, Shader & pVertexShaderWithBinary )
	{
		return _stateFactory->CreateIAInputLayoutState( pDefinition, pVertexShaderWithBinary );
	}

	IAVertexStreamImmutableStateHandle PipelineImmutableStateFactoryAdapter::CreateState( const IAVertexStreamDefinition & pDefinition )
	{
		return _stateFactory->CreateIAVertexStreamState( pDefinition );
	}

	RasterizerImmutableStateHandle PipelineImmutableStateFactoryAdapter::CreateState( const RasterizerConfig & pConfig )
	{
		return _stateFactory->CreateRasterizerState( pConfig );
	}

	RenderTargetBindingImmutableStateHandle PipelineImmutableStateFactoryAdapter::CreateState( const RenderTargetBindingDefinition & pDefinition )
	{
		return _stateFactory->CreateRenderTargetBindingState( pDefinition );
	}

	RenderPassConfigurationImmutableStateHandle PipelineImmutableStateFactoryAdapter::CreateState( const RenderPassConfiguration & pConfiguration )
	{
		return _stateFactory->CreateRenderPassState( pConfiguration );
	}

} // namespace Ic3::Graphics::GCI
