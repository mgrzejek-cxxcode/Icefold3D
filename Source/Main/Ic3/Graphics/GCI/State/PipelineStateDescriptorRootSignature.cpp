
#include "PipelineStateDescriptorRootSignature.h"

namespace Ic3::Graphics::GCI
{

	RootSignatureDescriptor::RootSignatureDescriptor(
			GPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID  )
	: PipelineStateDescriptor( pGPUDevice, pDescriptorID )
	{}

	RootSignatureDescriptor::~RootSignatureDescriptor() = default;

	EPipelineStateDescriptorType RootSignatureDescriptor::GetDescriptorType() const noexcept
	{
		return EPipelineStateDescriptorType::DTRootSignature;
	}


	namespace PIM
	{

		RootSignatureDescriptorGeneric::RootSignatureDescriptorGeneric(
				GPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID,
				RootSignature pRootSignature  )
		: RootSignatureDescriptor( pGPUDevice, pDescriptorID )
		, mRootSignature( std::move( pRootSignature ) )
		{}

		RootSignatureDescriptorGeneric::~RootSignatureDescriptorGeneric() = default;

		TGfxHandle<RootSignatureDescriptorGeneric> RootSignatureDescriptorGeneric::CreateInstance(
				GPUDevice & pGPUDevice,
				const RootSignatureDescriptorCreateInfo & pCreateInfo )
		{
			auto rootSignature = GCU::CreateRootSignature( pCreateInfo.rootSignatureDesc );
			if( !rootSignature )
			{
				return nullptr;
			}

			auto rootSignatureDescriptor = CreateDynamicObject<RootSignatureDescriptorGeneric>(
				pGPUDevice,
				pCreateInfo.descriptorID,
				std::move( rootSignature ) );

			return rootSignatureDescriptor;
		}

	} // namespace PIM

}
