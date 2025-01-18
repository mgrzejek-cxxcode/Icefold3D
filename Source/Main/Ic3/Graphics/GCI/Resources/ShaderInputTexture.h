
#pragma once

#ifndef __IC3_GRAPHICS_GCI_SHADER_INPUT_TEXTURE_H__
#define __IC3_GRAPHICS_GCI_SHADER_INPUT_TEXTURE_H__

#include "GPUResource.h"
#include "TextureReference.h"

namespace Ic3::Graphics::GCI
{

	/// @brief
	struct ShaderInputTextureCreateInfo
	{
		TextureReference mTargetTexture;

		ShaderInputTextureLayout mInputTextureLayout;

		cppx::bitmask<ETextureBindFlags> mBindFlags;
	};

	/// @brief
	class ShaderInputTexture : public GPUResourceView
	{
		friend class GPUDevice;
		friend class Texture;

	public:
		ShaderInputTextureLayout const mSITextureLayout;

		/// Internal texture used by this SIT.
		TextureReference const mTargetTexture;

	public:
		ShaderInputTexture(
				GPUDevice & pGPUDevice,
				const ShaderInputTextureLayout & pSITextureLayout,
				TextureReference pTargetTexture );

		virtual ~ShaderInputTexture();

		///
		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_SHADER_INPUT_TEXTURE_H__
