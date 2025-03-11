
#include "GraphicsPipelineStateDescriptorIA.h"
#include <Ic3/Graphics/GCI/GPUDevice.h>

namespace Ic3::Graphics::GCI
{

	VertexAttributeLayoutDescriptor::VertexAttributeLayoutDescriptor(
			GPUDevice & pGPUDevice,
			const IAVertexAttributeLayoutCommonConfig & pCommonAttributeLayoutConfig )
	: PipelineStateDescriptor( pGPUDevice )
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
	: PipelineStateDescriptor( pGPUDevice )
	, mCommonSourceBindingConfig( pCommonSourceBindingConfig )
	{}

	VertexSourceBindingDescriptor::~VertexSourceBindingDescriptor() = default;

	EPipelineStateDescriptorType VertexSourceBindingDescriptor::GetDescriptorType() const noexcept
	{
		return EPipelineStateDescriptorType::DTVertexSourceBinding;
	}


	namespace PIM
	{

		//!! VertexAttributeLayoutDescriptorNative

		VertexAttributeLayoutDescriptorNative::VertexAttributeLayoutDescriptorNative(
				GPUDevice & pGPUDevice,
				const IAVertexAttributeLayoutCommonConfig & pCommonAttributeLayoutConfig )
		: VertexAttributeLayoutDescriptor( pGPUDevice, pCommonAttributeLayoutConfig )
		{}

		VertexAttributeLayoutDescriptorNative::~VertexAttributeLayoutDescriptorNative() = default;


		//!! VertexSourceBindingDescriptorNative

		VertexSourceBindingDescriptorNative::VertexSourceBindingDescriptorNative(
				GPUDevice & pGPUDevice,
				const IAVertexSourceBindingCommonConfig & pCommonSourceBindingConfig )
		: VertexSourceBindingDescriptor( pGPUDevice, pCommonSourceBindingConfig )
		{}

		VertexSourceBindingDescriptorNative::~VertexSourceBindingDescriptorNative() = default;

	} // namespace PIM

}
