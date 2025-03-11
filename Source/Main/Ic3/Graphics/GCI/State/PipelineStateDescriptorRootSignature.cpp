
#include "PipelineStateDescriptorRootSignature.h"

namespace Ic3::Graphics::GCI
{

	RootSignatureDescriptor::RootSignatureDescriptor( GPUDevice & pGPUDevice )
	: PipelineStateDescriptor( pGPUDevice )
	{}

	RootSignatureDescriptor::~RootSignatureDescriptor() = default;

	EPipelineStateDescriptorType RootSignatureDescriptor::GetDescriptorType() const noexcept
	{
		return EPipelineStateDescriptorType::DTRootSignature;
	}


	namespace PIM
	{

		RootSignatureDescriptorGeneric::RootSignatureDescriptorGeneric( GPUDevice & pGPUDevice, RootSignature pRootSignature  )
		: RootSignatureDescriptor( pGPUDevice )
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

			const auto rootSignatureDescriptor = CreateDynamicObject<RootSignatureDescriptorGeneric>( pGPUDevice, std::move( rootSignature ) );

			return rootSignatureDescriptor;
		}

	} // namespace PIM

}
