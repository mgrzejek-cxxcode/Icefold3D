
#include "PipelineImmutableStateCache.h"

namespace Ic3::Graphics::GCI
{


	PipelineImmutableStateFactoryCacheAdapter::PipelineImmutableStateFactoryCacheAdapter( PipelineImmutableStateFactory & pFactory )
			: _stateFactory( &pFactory )
	{}

	PipelineImmutableStateFactoryCacheAdapter::~PipelineImmutableStateFactoryCacheAdapter() = default;

	BlendImmutableStateHandle PipelineImmutableStateFactoryCacheAdapter::CreateState( const BlendConfig & pConfig )
	{
		return _stateFactory->CreateBlendState( pConfig );
	}

	DepthStencilImmutableStateHandle PipelineImmutableStateFactoryCacheAdapter::CreateState( const DepthStencilConfig & pConfig )
	{
		return _stateFactory->CreateDepthStencilState( pConfig );
	}

	GraphicsShaderLinkageImmutableStateHandle PipelineImmutableStateFactoryCacheAdapter::CreateState( const GraphicsShaderSet & pShaderSet )
	{
		return _stateFactory->CreateGraphicsShaderLinkageState( pShaderSet );
	}

	IAInputLayoutImmutableStateHandle PipelineImmutableStateFactoryCacheAdapter::CreateState( const IAInputLayoutDefinition & pDefinition, Shader & pVertexShaderWithBinary )
	{
		return _stateFactory->CreateIAInputLayoutState( pDefinition, pVertexShaderWithBinary );
	}

	IAVertexStreamImmutableStateHandle PipelineImmutableStateFactoryCacheAdapter::CreateState( const IAVertexStreamDefinition & pDefinition )
	{
		return _stateFactory->CreateIAVertexStreamState( pDefinition );
	}

	RasterizerImmutableStateHandle PipelineImmutableStateFactoryCacheAdapter::CreateState( const RasterizerConfig & pConfig )
	{
		return _stateFactory->CreateRasterizerState( pConfig );
	}

	RenderTargetBindingImmutableStateHandle PipelineImmutableStateFactoryCacheAdapter::CreateState( const RenderTargetBindingDefinition & pDefinition )
	{
		return _stateFactory->CreateRenderTargetBindingState( pDefinition );
	}

	RenderPassConfigurationImmutableStateHandle PipelineImmutableStateFactoryCacheAdapter::CreateState( const RenderPassConfiguration & pConfiguration )
	{
		return _stateFactory->CreateRenderPassState( pConfiguration );
	}

} // namespace Ic3::Graphics::GCI
