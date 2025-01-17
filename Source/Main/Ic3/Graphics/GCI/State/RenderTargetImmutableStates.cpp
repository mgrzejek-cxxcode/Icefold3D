
#include "RenderTargetImmutableStates.h"
#include <Ic3/Graphics/GCI/GpuDevice.h>

namespace Ic3::Graphics::GCI
{

	class RenderTargetBindingImmutableStateDynamicOverride : public RenderTargetBindingImmutableState
	{
	public:
		RenderTargetBindingImmutableStateDynamicOverride()
		: RenderTargetBindingImmutableState( GpuDevice::nullDevice(), {} )
		{}

		virtual ~RenderTargetBindingImmutableStateDynamicOverride() = default;

		bool IsDynamicOverrideState() const noexcept override final
		{
			return true;
		}
	};


	RenderTargetBindingImmutableState::RenderTargetBindingImmutableState(
			GpuDevice & pGpuDevice,
			const RenderTargetLayout & pRenderTargetLayout )
	: GraphicsPipelineImmutableState( pGpuDevice )
	, mRenderTargetLayout( pRenderTargetLayout )
	{}

	RenderTargetBindingImmutableState::~RenderTargetBindingImmutableState() = default;

	const RenderTargetBindingImmutableState & RenderTargetBindingImmutableState::GetDynamicOverrideState()
	{
		static const RenderTargetBindingImmutableStateDynamicOverride sDynamicOverrideState{};
		return sDynamicOverrideState;
	}


	RenderPassConfigurationImmutableState::RenderPassConfigurationImmutableState( GpuDevice & pGpuDevice )
	: GraphicsPipelineImmutableState( pGpuDevice )
	{}

	RenderPassConfigurationImmutableState::~RenderPassConfigurationImmutableState() = default;



	RenderPassConfigurationImmutableStateDefault::RenderPassConfigurationImmutableStateDefault(
			GpuDevice & pGpuDevice,
			const RenderPassConfiguration & pRenderPassConfiguration )
	: RenderPassConfigurationImmutableState( pGpuDevice )
	, mRenderPassConfiguration( pRenderPassConfiguration )
	{}

	RenderPassConfigurationImmutableStateDefault::~RenderPassConfigurationImmutableStateDefault() = default;

	TGfxHandle<RenderPassConfigurationImmutableStateDefault> RenderPassConfigurationImmutableStateDefault::CreateInstance(
			GpuDevice & pGpuDevice,
			const RenderPassConfiguration & pConfiguration )
	{
		auto immutableState = CreateGfxObject<RenderPassConfigurationImmutableStateDefault>(
				pGpuDevice,
				pConfiguration );

		return immutableState;
	}

}
