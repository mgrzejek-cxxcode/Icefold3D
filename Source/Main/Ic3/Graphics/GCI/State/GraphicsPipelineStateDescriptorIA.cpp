
#include "GraphicsPipelineStateDescriptorIA.h"
#include <Ic3/Graphics/GCI/GPUDevice.h>

namespace Ic3::Graphics::GCI
{

	VertexAttributeLayoutDescriptor::VertexAttributeLayoutDescriptor(
			GPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID,
			const IAVertexAttributeLayoutCommonConfig & pCommonAttributeLayoutConfig )
	: PipelineStateDescriptor( pGPUDevice, pDescriptorID )
	, mCommonAttributeLayoutConfig( pCommonAttributeLayoutConfig )
	{}

	VertexAttributeLayoutDescriptor::~VertexAttributeLayoutDescriptor() = default;

	EPipelineStateDescriptorType VertexAttributeLayoutDescriptor::GetDescriptorType() const noexcept
	{
		return EPipelineStateDescriptorType::DTVertexAttributeLayout;
	}


	VertexSourceBindingDescriptor::VertexSourceBindingDescriptor(
			GPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID,
			const IAVertexSourceBindingCommonConfig & pCommonSourceBindingConfig )
	: PipelineStateDescriptor( pGPUDevice, pDescriptorID )
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
				pipeline_state_descriptor_id_t pDescriptorID,
				const IAVertexAttributeLayoutCommonConfig & pCommonAttributeLayoutConfig )
		: VertexAttributeLayoutDescriptor( pGPUDevice, pDescriptorID, pCommonAttributeLayoutConfig )
		{}

		VertexAttributeLayoutDescriptorNative::~VertexAttributeLayoutDescriptorNative() = default;


		//!! VertexSourceBindingDescriptorNative

		VertexSourceBindingDescriptorNative::VertexSourceBindingDescriptorNative(
				GPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID,
				const IAVertexSourceBindingCommonConfig & pCommonSourceBindingConfig )
		: VertexSourceBindingDescriptor( pGPUDevice, pDescriptorID, pCommonSourceBindingConfig )
		{}

		VertexSourceBindingDescriptorNative::~VertexSourceBindingDescriptorNative() = default;

	} // namespace PIM

}
