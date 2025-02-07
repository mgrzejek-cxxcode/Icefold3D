
#include "ShaderRootSignatureStateDescriptor.h"

namespace Ic3::Graphics::GCI
{

	ShaderRootSignatureStateDescriptor::ShaderRootSignatureStateDescriptor(
			GPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID  )
	: PipelineStateDescriptor( pGPUDevice, pDescriptorID )
	{}

	ShaderRootSignatureStateDescriptor::~ShaderRootSignatureStateDescriptor() = default;

	EPipelineStateDescriptorType ShaderRootSignatureStateDescriptor::GetDescriptorType() const noexcept
	{
		return EPipelineStateDescriptorType::DTShaderRootSignature;
	}


	namespace PIM
	{

		ShaderRootSignatureStateDescriptorGeneric::ShaderRootSignatureStateDescriptorGeneric(
				GPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID  )
		: ShaderRootSignatureStateDescriptor( pGPUDevice, pDescriptorID )
		{}

		ShaderRootSignatureStateDescriptorGeneric::~ShaderRootSignatureStateDescriptorGeneric() = default;

	} // namespace PIM

}
