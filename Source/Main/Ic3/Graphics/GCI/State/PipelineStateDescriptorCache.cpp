
#include "PipelineStateDescriptorCache.h"
#include "PipelineStateDescriptorFactory.h"
#include "GraphicsPipelineStateCommon.h"

namespace Ic3::Graphics::GCI
{


	GraphicsPipelineStateDescriptorFactoryCacheAdapter::GraphicsPipelineStateDescriptorFactoryCacheAdapter(
			PipelineStateDescriptorFactory & pDescriptorFactory )
	: _descriptorFactory( &pDescriptorFactory )
	{}

	GraphicsPipelineStateDescriptorFactoryCacheAdapter::~GraphicsPipelineStateDescriptorFactoryCacheAdapter() = default;

	BlendStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateDescriptor(
			const BlendStateDescriptorCreateInfo & pCreateInfo )
	{
		return _descriptorFactory->CreateBlendStateDescriptor( pCreateInfo );
	}

	DepthStencilStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateDescriptor(
			const DepthStencilStateDescriptorCreateInfo & pCreateInfo )
	{
		return _descriptorFactory->CreateDepthStencilStateDescriptor( pCreateInfo );
	}

	RasterizerStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateDescriptor(
			const RasterizerStateDescriptorCreateInfo & pCreateInfo )
	{
		return _descriptorFactory->CreateRasterizerStateDescriptor( pCreateInfo );
	}

	GraphicsShaderLinkageDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateDescriptor(
			const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo )
	{
		return _descriptorFactory->CreateGraphicsShaderLinkageDescriptor( pCreateInfo );
	}

	VertexAttributeLayoutDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateDescriptor(
			const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo )
	{
		return _descriptorFactory->CreateVertexAttributeLayoutDescriptor( pCreateInfo );
	}

	RootSignatureDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateDescriptor(
			const RootSignatureDescriptorCreateInfo & pCreateInfo )
	{
		return _descriptorFactory->CreateRootSignatureDescriptor( pCreateInfo );
	}

} // namespace Ic3::Graphics::GCI
