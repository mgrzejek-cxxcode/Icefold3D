
#include "GraphicsPipelineStateDescriptorIA.h"
#include <Ic3/Graphics/GCI/GPUDevice.h>

namespace Ic3::Graphics::GCI
{

	VertexAttributeLayoutDescriptor::VertexAttributeLayoutDescriptor(
			GPUDevice & pGPUDevice,
			const IAVertexAttributeLayoutCommonConfig & pCommonAttributeLayoutConfig )
	: CachedPipelineStateDescriptor( pGPUDevice )
	, mCommonAttributeLayoutConfig( pCommonAttributeLayoutConfig )
	{}

	VertexAttributeLayoutDescriptor::~VertexAttributeLayoutDescriptor() = default;

	EPipelineStateDescriptorType VertexAttributeLayoutDescriptor::GetDescriptorType() const noexcept
	{
		return EPipelineStateDescriptorType::DTVertexAttributeLayout;
	}


	VertexSourceBindingDescriptor::VertexSourceBindingDescriptor(
			GPUDevice & pGPUDevice,
			const IAVertexSourceBindingCommonConfig & pCommonSourceBindingConfig )
	: NonCachedPipelineStateDescriptor( pGPUDevice )
	, mCommonSourceBindingConfig( pCommonSourceBindingConfig )
	{}

	VertexSourceBindingDescriptor::~VertexSourceBindingDescriptor() = default;

	EPipelineStateDescriptorType VertexSourceBindingDescriptor::GetDescriptorType() const noexcept
	{
		return EPipelineStateDescriptorType::DTVertexSourceBinding;
	}

}
