
#include "SamplerCommon.h"

namespace Ic3::Graphics::GCI
{

	const SamplerConfig cvSamplerConfigDefault =
	{
		TextureCoordAddressModeConfig
		{
			ETextureAddressMode::ClampToEdge,
			ETextureAddressMode::ClampToEdge,
			ETextureAddressMode::ClampToEdge,
		},
		Math::RGBAColorR32Norm
		{
			1.0f,
			1.0f,
			1.0f,
			1.0f,
		},
		ETextureBorderPredefinedColor::OpaqueWhite,
		TextureFilterConfig
		{
			0,
			ETextureFilter::Linear,
			ETextureFilter::Linear,
			ETextureMipMode::Linear,
		},
		SamplerConfig::MipLODRange
		{
			-1024.0f,
			1024.0f,
		},
		0.0f,
		ETextureCompareMode::None,
		ECompFunc::Never
	};

} // namespace Ic3::Graphics::GCI
