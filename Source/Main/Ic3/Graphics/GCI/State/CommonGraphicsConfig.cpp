
#include "CommonGraphicsConfig.h"

namespace Ic3::Graphics::GCI
{

	namespace defaults
	{

		const RTColorAttachmentBlendSettings cvCommonRTColorAttachmentBlendSettingsDefault =
		{
				EBlendFactor::One,
				EBlendFactor::One,
				EBlendFactor::Zero,
				EBlendFactor::Zero,
				EBlendOp::Add,
				EBlendOp::Add,
				eBlendWriteMaskAll
		};

		const DepthTestSettings cvCommonDepthTestSettingsDefault =
		{
			ECompFunc::Less,
			EDepthWriteMask::All
		};

		const StencilTestSettings cvCommonStencilTestSettingsDefault =
		{
			{
				ECompFunc::Always,
				EStencilOp::Keep,
				EStencilOp::Keep,
				EStencilOp::Keep
			},
			{
				ECompFunc::Always,
				EStencilOp::Keep,
				EStencilOp::Keep,
				EStencilOp::Keep
			},
			0xFF,
			0xFF
		};

		const BlendConfig cvPipelineBlendConfigDefault =
		{
				eRTAttachmentFlagColor0Bit,
				eBlendConfigMaskDefault,
				{
				cvCommonRTColorAttachmentBlendSettingsDefault
			},
				Math::RGBAColorR32Norm{ 0.0f, 0.0f, 0.0f, 0.0f },
		};

		const DepthStencilConfig cvPipelineDepthStencilConfigDefault =
		{
			0u,
			cvCommonDepthTestSettingsDefault,
			cvCommonStencilTestSettingsDefault
		};

		const DepthStencilConfig cvPipelineDepthStencilConfigEnableDepthTest =
		{
				eDepthStencilConfigFlagEnableDepthTestBit,
				cvCommonDepthTestSettingsDefault,
				cvCommonStencilTestSettingsDefault
		};

		const RasterizerConfig cvPipelineRasterizerConfigDefault =
		{
			0u,
			ECullMode::Back,
			EPrimitiveFillMode::Solid,
			ETriangleVerticesOrder::CounterClockwise,
		};

	}

} // namespace Ic3::Graphics::GCI
