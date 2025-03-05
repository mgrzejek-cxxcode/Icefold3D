
#include "ShaderInputTexture.h"
#include "Texture.h"

namespace Ic3::Graphics::GCI
{

	ShaderInputTexture::ShaderInputTexture(
			GPUDevice & pGPUDevice,
			const ShaderInputTextureLayout & pSITextureLayout,
			TextureReference pTargetTexture )
	: GPUResourceView( pGPUDevice, EGPUResourceBaseType::Texture, pTargetTexture->mTextureProperties.resourceFlags )
	, mSITextureLayout( pSITextureLayout )
	, mTargetTexture( pTargetTexture )
	{}

	ShaderInputTexture::~ShaderInputTexture() = default;

	bool ShaderInputTexture::IsEmpty() const noexcept
	{
		return !mTargetTexture;
	}

}
