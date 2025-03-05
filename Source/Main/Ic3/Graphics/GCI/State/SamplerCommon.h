
#pragma once

#ifndef __IC3_GRAPHICS_GCI_SAMPLER_COMMON_H__
#define __IC3_GRAPHICS_GCI_SAMPLER_COMMON_H__

#include "CommonGraphicsConfig.h"

namespace Ic3::Graphics::GCI
{

	Ic3GCIDeclareClassHandle( Sampler );

	enum class ETextureAddressMode : enum_default_value_t
	{
		Undefined,
		ClampToColor,
		ClampToEdge,
		MirrorClampToEdge,
		MirrorRepeat,
		Repeat,
	};

	enum class ETextureCompareMode : enum_default_value_t
	{
		Undefined,
		None,
		RefToTexture,
	};

	enum class ETextureBorderPredefinedColor : enum_default_value_t
	{
		Undefined,
		OpaqueBlack,
		OpaqueWhite,
		TransparentBlack,
	};

	enum class ETextureFilter : enum_default_value_t
	{
		Undefined,
		Point,
		Linear,
		Anisotropic,
	};

	enum class ETextureMipMode : enum_default_value_t
	{
		Undefined,
		Disable,
		Nearest,
		Linear,
	};

	struct TextureCoordAddressModeConfig
	{
		ETextureAddressMode coordU;
		ETextureAddressMode coordV;
		ETextureAddressMode coordW;
	};

	struct TextureFilterConfig
	{
		uint32 anisotropyLevel;
		ETextureFilter magFilter;
		ETextureFilter minFilter;
		ETextureMipMode mipMode;
	};

	struct SamplerConfig
	{
		using MipLODRange = cppx::range<float>;
		TextureCoordAddressModeConfig addressModeConfig;
		Math::RGBAColorR32Norm borderColor;
		ETextureBorderPredefinedColor borderPredefinedColor;
		TextureFilterConfig filterConfig;
		MipLODRange mipLODRange;
		float mipLODBias;
		ETextureCompareMode textureCompareMode;
		ECompFunc textureCompareFunc;
	};

	struct SamplerCreateInfo
	{
		SamplerConfig samplerConfig;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_SAMPLER_COMMON_H__
