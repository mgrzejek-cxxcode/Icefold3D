
#include "PipelineStateDescriptorCache.h"
#include "GraphicsPipelineStateCommon.h"

namespace Ic3::Graphics::GCI
{


	GraphicsPipelineStateDescriptorFactoryCacheAdapter::GraphicsPipelineStateDescriptorFactoryCacheAdapter( PipelineStateDescriptorFactory & pStateFactory )
	: _stateFactory( &pStateFactory )
	{}

	GraphicsPipelineStateDescriptorFactoryCacheAdapter::~GraphicsPipelineStateDescriptorFactoryCacheAdapter() = default;

	BlendStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateStateDescriptor(
			const BlendSettings & pBlendSettings )
	{
		return _stateFactory->CreateBlendStateDescriptor( pBlendSettings );
	}

	DepthStencilStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateStateDescriptor(
			const DepthStencilSettings & pDepthStencilSettings )
	{
		return _stateFactory->CreateDepthStencilStateDescriptor( pDepthStencilSettings );
	}

	RasterizerStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateStateDescriptor(
			const RasterizerSettings & pRasterizerSettings )
	{
		return _stateFactory->CreateRasterizerStateDescriptor( pRasterizerSettings );
	}

	GraphicsShaderLinkageStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateStateDescriptor( const GraphicsShaderSet & pShaderSet )
	{
		return _stateFactory->CreateGraphicsShaderLinkageStateDescriptor( pShaderSet );
	}

	IAVertexAttributeLayoutStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateStateDescriptor( const IAVertexAttributeLayoutDefinition & pDefinition, Shader & pVertexShaderWithBinary )
	{
		return _stateFactory->CreateIAVertexAttributeLayoutStateDescriptor( pDefinition, pVertexShaderWithBinary );
	}

	IAVertexStreamStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateStateDescriptor( const IAVertexStreamBindingDefinition & pDefinition )
	{
		return _stateFactory->CreateIAVertexStreamBindingStateDescriptor( pDefinition );
	}

	RenderTargetBindingStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateStateDescriptor( const RenderTargetBindingDefinition & pDefinition )
	{
		return _stateFactory->CreateRenderTargetBindingState( pDefinition );
	}

	RenderPassConfigurationStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateStateDescriptor( const RenderPassConfiguration & pConfiguration )
	{
		return _stateFactory->CreateRenderPassState( pConfiguration );
	}

} // namespace Ic3::Graphics::GCI
