
#include "CommonGraphicsConfigImmutableStates.h"

namespace Ic3::Graphics::GCI
{

	BlendImmutableState::BlendImmutableState( GPUDevice & pGPUDevice, TBitmask<EBlendConfigFlags> pBlendFlags )
	: GraphicsPipelineImmutableState( pGPUDevice )
	, mBlendFlags( pBlendFlags & eBlendConfigMaskAll )
	{}

	BlendImmutableState::~BlendImmutableState() = default;


	DepthStencilImmutableState::DepthStencilImmutableState( GPUDevice & pGPUDevice, TBitmask<EDepthStencilConfigFlags> pDepthStencilFlags )
	: GraphicsPipelineImmutableState( pGPUDevice )
	, mDepthStencilFlags( pDepthStencilFlags & eDepthStencilConfigMaskAll )
	{}

	DepthStencilImmutableState::~DepthStencilImmutableState() = default;


	RasterizerImmutableState::RasterizerImmutableState( GPUDevice & pGPUDevice, TBitmask<ERasterizerConfigFlags> pRasterizerFlags )
	: GraphicsPipelineImmutableState( pGPUDevice )
	, mRasterizerFlags( pRasterizerFlags & eRasterizerConfigMaskAll )
	{}

	RasterizerImmutableState::~RasterizerImmutableState() = default;

} // namespace Ic3::Graphics::GCI
