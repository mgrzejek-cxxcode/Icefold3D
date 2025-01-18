
#include "RenderTargetImmutableStates.h"
#include <Ic3/Graphics/GCI/GPUDevice.h>

namespace Ic3::Graphics::GCI
{

	class RenderTargetBindingImmutableStateDynamicOverride : public RenderTargetBindingImmutableState
	{
	public:
		RenderTargetBindingImmutableStateDynamicOverride()
		: RenderTargetBindingImmutableState( GPUDevice::GetNullDevice(), {} )
		{}

		virtual ~RenderTargetBindingImmutableStateDynamicOverride() = default;

		bool IsDynamicOverrideState() const noexcept override final
		{
			return true;
		}
	};


	RenderTargetBindingImmutableState::RenderTargetBindingImmutableState(
			GPUDevice & pGPUDevice,
			const RenderTargetLayout & pRenderTargetLayout )
	: GraphicsPipelineImmutableState( pGPUDevice )
	, mRenderTargetLayout( pRenderTargetLayout )
	{}

	RenderTargetBindingImmutableState::~RenderTargetBindingImmutableState() = default;

	const RenderTargetBindingImmutableState & RenderTargetBindingImmutableState::GetDynamicOverrideState()
	{
		static const RenderTargetBindingImmutableStateDynamicOverride sDynamicOverrideState{};
		return sDynamicOverrideState;
	}


	RenderPassConfigurationImmutableState::RenderPassConfigurationImmutableState( GPUDevice & pGPUDevice )
	: GraphicsPipelineImmutableState( pGPUDevice )
	{}

	RenderPassConfigurationImmutableState::~RenderPassConfigurationImmutableState() = default;



	RenderPassConfigurationImmutableStateDefault::RenderPassConfigurationImmutableStateDefault(
			GPUDevice & pGPUDevice,
			const RenderPassConfiguration & pRenderPassConfiguration )
	: RenderPassConfigurationImmutableState( pGPUDevice )
	, mRenderPassConfiguration( pRenderPassConfiguration )
	{}

	RenderPassConfigurationImmutableStateDefault::~RenderPassConfigurationImmutableStateDefault() = default;

	TGfxHandle<RenderPassConfigurationImmutableStateDefault> RenderPassConfigurationImmutableStateDefault::CreateInstance(
			GPUDevice & pGPUDevice,
			const RenderPassConfiguration & pConfiguration )
	{
		auto immutableState = CreateGfxObject<RenderPassConfigurationImmutableStateDefault>(
				pGPUDevice,
				pConfiguration );

		return immutableState;
	}

}
