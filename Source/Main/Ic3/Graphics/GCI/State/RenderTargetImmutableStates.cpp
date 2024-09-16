
#include "RenderTargetImmutableStates.h"
#include <Ic3/Graphics/GCI/GPUDevice.h>

namespace Ic3::Graphics::GCI
{

	class RenderTargetBindingImmutableStateDynamicOverride : public RenderTargetBindingImmutableState
	{
	public:
		RenderTargetBindingImmutableStateDynamicOverride()
		: RenderTargetBindingImmutableState( GPUDevice::nullDevice(), {} )
		{}

		virtual ~RenderTargetBindingImmutableStateDynamicOverride() = default;

		bool isDynamicOverrideState() const noexcept override final
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

	const RenderTargetBindingImmutableState & RenderTargetBindingImmutableState::getDynamicOverrideState()
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

	TGPAHandle<RenderPassConfigurationImmutableStateDefault> RenderPassConfigurationImmutableStateDefault::createInstance(
			GPUDevice & pGPUDevice,
			const RenderPassConfiguration & pConfiguration )
	{
		auto immutableState = createGPUAPIObject<RenderPassConfigurationImmutableStateDefault>(
				pGPUDevice,
				pConfiguration );

		return immutableState;
	}

}
