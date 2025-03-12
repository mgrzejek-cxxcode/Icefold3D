
#include "PipelineStateDescriptorRootSignature.h"

namespace Ic3::Graphics::GCI
{

	RootSignatureDescriptor::RootSignatureDescriptor( GPUDevice & pGPUDevice )
	: GCIPipelineStateDescriptor( pGPUDevice )
	{}

	RootSignatureDescriptor::~RootSignatureDescriptor() = default;

	EPipelineStateDescriptorType RootSignatureDescriptor::GetDescriptorType() const noexcept
	{
		return EPipelineStateDescriptorType::DTRootSignature;
	}

}
