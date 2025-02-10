
#include "PipelineStateDescriptorCacheUnit.h"

namespace Ic3::Graphics::GCI
{


	GraphicsPipelineStateDescriptorFactoryCacheAdapter::GraphicsPipelineStateDescriptorFactoryCacheAdapter( GraphicsPipelineStateDescriptorFactory & pFactory )
			: _stateFactory( &pFactory )
	{}

	GraphicsPipelineStateDescriptorFactoryCacheAdapter::~GraphicsPipelineStateDescriptorFactoryCacheAdapter() = default;

	BlendStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateState( const BlendSettings & pConfig )
	{
		return _stateFactory->CreateBlendState( pConfig );
	}

	DepthStencilStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateState( const DepthStencilSettings & pConfig )
	{
		return _stateFactory->CreateDepthStencilState( pConfig );
	}

	GraphicsShaderLinkageStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateState( const GraphicsShaderSet & pShaderSet )
	{
		return _stateFactory->CreateGraphicsShaderLinkageState( pShaderSet );
	}

	IAVertexAttributeLayoutStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateState( const IAVertexAttributeLayoutDefinition & pDefinition, Shader & pVertexShaderWithBinary )
	{
		return _stateFactory->CreateIAVertexAttributeLayoutState( pDefinition, pVertexShaderWithBinary );
	}

	IAVertexStreamStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateState( const IAVertexStreamBindingDefinition & pDefinition )
	{
		return _stateFactory->CreateIAVertexStreamState( pDefinition );
	}

	RasterizerStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateState( const RasterizerSettings & pConfig )
	{
		return _stateFactory->CreateRasterizerState( pConfig );
	}

	RenderTargetBindingStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateState( const RenderTargetBindingDefinition & pDefinition )
	{
		return _stateFactory->CreateRenderTargetBindingState( pDefinition );
	}

	RenderPassConfigurationStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateState( const RenderPassConfiguration & pConfiguration )
	{
		return _stateFactory->CreateRenderPassState( pConfiguration );
	}

} // namespace Ic3::Graphics::GCI
