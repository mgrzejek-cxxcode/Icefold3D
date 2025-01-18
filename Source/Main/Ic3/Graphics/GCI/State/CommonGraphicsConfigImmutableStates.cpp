
#include "CommonGraphicsConfigImmutableStates.h"

namespace Ic3::Graphics::GCI
{

	BlendImmutableState::BlendImmutableState( GPUDevice & pGPUDevice, cppx::bitmask<EBlendConfigFlags> pBlendFlags )
	: GraphicsPipelineImmutableState( pGPUDevice )
	, mBlendFlags( pBlendFlags & eBlendConfigMaskAll )
	{}

	BlendImmutableState::~BlendImmutableState() = default;


	DepthStencilImmutableState::DepthStencilImmutableState( GPUDevice & pGPUDevice, cppx::bitmask<EDepthStencilConfigFlags> pDepthStencilFlags )
	: GraphicsPipelineImmutableState( pGPUDevice )
	, mDepthStencilFlags( pDepthStencilFlags & eDepthStencilConfigMaskAll )
	{}

	DepthStencilImmutableState::~DepthStencilImmutableState() = default;


	RasterizerImmutableState::RasterizerImmutableState( GPUDevice & pGPUDevice, cppx::bitmask<ERasterizerConfigFlags> pRasterizerFlags )
	: GraphicsPipelineImmutableState( pGPUDevice )
	, mRasterizerFlags( pRasterizerFlags & eRasterizerConfigMaskAll )
	{}

	RasterizerImmutableState::~RasterizerImmutableState() = default;

} // namespace Ic3::Graphics::GCI
