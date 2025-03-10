
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
			pipeline_state_descriptor_id_t pDescriptorID,
			const BlendStateDescriptorCreateInfo & pCreateInfo )
	{
		return _descriptorFactory->CreateBlendStateDescriptor( pDescriptorID, pCreateInfo );
	}

	DepthStencilStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const DepthStencilStateDescriptorCreateInfo & pCreateInfo )
	{
		return _descriptorFactory->CreateDepthStencilStateDescriptor( pDescriptorID, pCreateInfo );
	}

	RasterizerStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const RasterizerStateDescriptorCreateInfo & pCreateInfo )
	{
		return _descriptorFactory->CreateRasterizerStateDescriptor( pDescriptorID, pCreateInfo );
	}

	GraphicsShaderLinkageDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo )
	{
		return _descriptorFactory->CreateGraphicsShaderLinkageDescriptor( pDescriptorID, pCreateInfo );
	}

	VertexAttributeLayoutDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo )
	{
		return _descriptorFactory->CreateVertexAttributeLayoutDescriptor( pDescriptorID, pCreateInfo );
	}

	RootSignatureDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const RootSignatureDescriptorCreateInfo & pCreateInfo )
	{
		return _descriptorFactory->CreateRootSignatureDescriptor( pDescriptorID, pCreateInfo );
	}

} // namespace Ic3::Graphics::GCI
