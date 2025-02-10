
#include "CommonGraphicsConfig.h"

namespace Ic3::Graphics::GCI
{

	namespace defaults
	{

		const RenderTargetColorAttachmentBlendSettings cvCommonRenderTargetColorAttachmentBlendSettingsDefault =
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

		const BlendSettings cvPipelineBlendSettingsDefault =
		{
				eRTAttachmentFlagColor0Bit,
				eBlendConfigMaskDefault,
				{
				cvCommonRenderTargetColorAttachmentBlendSettingsDefault
			},
				Math::RGBAColorR32Norm{ 0.0f, 0.0f, 0.0f, 0.0f },
		};

		const DepthStencilSettings cvPipelineDepthStencilSettingsDefault =
		{
			0u,
			cvCommonDepthTestSettingsDefault,
			cvCommonStencilTestSettingsDefault
		};

		const DepthStencilSettings cvPipelineDepthStencilSettingsEnableDepthTest =
		{
				eDepthStencilConfigFlagEnableDepthTestBit,
				cvCommonDepthTestSettingsDefault,
				cvCommonStencilTestSettingsDefault
		};

		const RasterizerSettings cvPipelineRasterizerSettingsDefault =
		{
			0u,
			ECullMode::Back,
			EPrimitiveFillMode::Solid,
			ETriangleVerticesOrder::CounterClockwise,
		};

	}

} // namespace Ic3::Graphics::GCI
