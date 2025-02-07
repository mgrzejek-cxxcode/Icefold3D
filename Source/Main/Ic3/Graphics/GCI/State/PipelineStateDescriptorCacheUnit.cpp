
#include "PipelineStateDescriptorCacheUnit.h"

namespace Ic3::Graphics::GCI
{


	GraphicsPipelineStateDescriptorFactoryCacheAdapter::GraphicsPipelineStateDescriptorFactoryCacheAdapter( GraphicsPipelineStateDescriptorFactory & pFactory )
			: _stateFactory( &pFactory )
	{}

	GraphicsPipelineStateDescriptorFactoryCacheAdapter::~GraphicsPipelineStateDescriptorFactoryCacheAdapter() = default;

	BlendStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateState( const BlendConfig & pConfig )
	{
		return _stateFactory->CreateBlendState( pConfig );
	}

	DepthStencilStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateState( const DepthStencilConfig & pConfig )
	{
		return _stateFactory->CreateDepthStencilState( pConfig );
	}

	GraphicsShaderLinkageStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateState( const GraphicsShaderSet & pShaderSet )
	{
		return _stateFactory->CreateGraphicsShaderLinkageState( pShaderSet );
	}

	IAInputLayoutStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateState( const IAInputLayoutDefinition & pDefinition, Shader & pVertexShaderWithBinary )
	{
		return _stateFactory->CreateIAInputLayoutState( pDefinition, pVertexShaderWithBinary );
	}

	IAVertexStreamStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateState( const IAVertexStreamConfiguration & pDefinition )
	{
		return _stateFactory->CreateIAVertexStreamState( pDefinition );
	}

	RasterizerStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateState( const RasterizerConfig & pConfig )
	{
		return _stateFactory->CreateRasterizerState( pConfig );
	}

	RenderTargetBindingStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateState( const RenderTargetBindingDefinition & pDefinition )
	{
		return _stateFactory->CreateRenderTargetBindingState( pDefinition );
	}

	RenderPassConfigStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateState( const RenderPassConfiguration & pConfiguration )
	{
		return _stateFactory->CreateRenderPassState( pConfiguration );
	}

} // namespace Ic3::Graphics::GCI
