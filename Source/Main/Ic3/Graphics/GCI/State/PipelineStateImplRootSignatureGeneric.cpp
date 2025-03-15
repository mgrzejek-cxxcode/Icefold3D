
#include "PipelineStateImplRootSignatureGeneric.h"

namespace Ic3::Graphics::GCI
{

	RootSignatureDescriptorGeneric::RootSignatureDescriptorGeneric( GPUDevice & pGPUDevice, RootSignature pRootSignature  )
	: GCIPipelineStateDescriptor( pGPUDevice )
	, mRootSignature( std::move( pRootSignature ) )
	{}

	RootSignatureDescriptorGeneric::~RootSignatureDescriptorGeneric() = default;

	TGfxHandle<RootSignatureDescriptorGeneric> RootSignatureDescriptorGeneric::CreateInstance(
			GPUDevice & pGPUDevice,
			const RootSignatureDescriptorCreateInfo & pCreateInfo )
	{
		auto rootSignature = Utilities::CreateRootSignature( pCreateInfo.rootSignatureDesc );
		if( !rootSignature )
		{
			return nullptr;
		}

		const auto rootSignatureDescriptor = CreateDynamicObject<RootSignatureDescriptorGeneric>( pGPUDevice, std::move( rootSignature ) );

		return rootSignatureDescriptor;
	}

}
