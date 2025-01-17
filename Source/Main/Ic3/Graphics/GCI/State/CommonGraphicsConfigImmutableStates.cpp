
#include "CommonGraphicsConfigImmutableStates.h"

namespace Ic3::Graphics::GCI
{

	BlendImmutableState::BlendImmutableState( GpuDevice & pGpuDevice, cppx::bitmask<EBlendConfigFlags> pBlendFlags )
	: GraphicsPipelineImmutableState( pGpuDevice )
	, mBlendFlags( pBlendFlags & eBlendConfigMaskAll )
	{}

	BlendImmutableState::~BlendImmutableState() = default;


	DepthStencilImmutableState::DepthStencilImmutableState( GpuDevice & pGpuDevice, cppx::bitmask<EDepthStencilConfigFlags> pDepthStencilFlags )
	: GraphicsPipelineImmutableState( pGpuDevice )
	, mDepthStencilFlags( pDepthStencilFlags & eDepthStencilConfigMaskAll )
	{}

	DepthStencilImmutableState::~DepthStencilImmutableState() = default;


	RasterizerImmutableState::RasterizerImmutableState( GpuDevice & pGpuDevice, cppx::bitmask<ERasterizerConfigFlags> pRasterizerFlags )
	: GraphicsPipelineImmutableState( pGpuDevice )
	, mRasterizerFlags( pRasterizerFlags & eRasterizerConfigMaskAll )
	{}

	RasterizerImmutableState::~RasterizerImmutableState() = default;

} // namespace Ic3::Graphics::GCI
