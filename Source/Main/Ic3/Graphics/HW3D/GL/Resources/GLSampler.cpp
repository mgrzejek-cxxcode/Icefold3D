
#include "GLSampler.h"
#include "../GLApiTranslationLayer.h"
#include "../GLGpuDevice.h"

namespace Ic3::Graphics::GCI
{

	GLSampler::GLSampler( GLGpuDevice & pGpuDevice, GLSamplerObjectHandle pGLSamplerObject )
	: Sampler( pGpuDevice )
	, mGLSamplerObject( std::move( pGLSamplerObject ) )
	{ }

	GLSampler::~GLSampler() = default;

	GLSamplerHandle GLSampler::CreateSampler( GLGpuDevice & pGpuDevice, const SamplerCreateInfo & pCreateInfo )
	{
		GLSamplerState openglSamplerState;
		if( !TranslateSamplerConfig( pCreateInfo.samplerConfig, openglSamplerState ) )
		{
			return nullptr;
		}

		auto openglSamplerObject = GLSamplerObject::Create( openglSamplerState );
		if( !openglSamplerObject )
		{
			return nullptr;
		}

		auto sampler = CreateGfxObject<GLSampler>( pGpuDevice, std::move( openglSamplerObject ) );

		return sampler;
	}

	bool GLSampler::TranslateSamplerConfig( const SamplerConfig & pSamplerConfig, GLSamplerState & pOutSamplerState )
	{
		pOutSamplerState.borderColor = pSamplerConfig.borderColor;
		pOutSamplerState.mipLODRange.first = pSamplerConfig.mipLODRange.begin;
		pOutSamplerState.mipLODRange.second = pSamplerConfig.mipLODRange.end;

		pOutSamplerState.addressModeS = ATL::TranslateGLTextureAddressMode( pSamplerConfig.addressModeConfig.coordU );
		pOutSamplerState.addressModeT = ATL::TranslateGLTextureAddressMode( pSamplerConfig.addressModeConfig.coordV );
		pOutSamplerState.addressModeR = ATL::TranslateGLTextureAddressMode( pSamplerConfig.addressModeConfig.coordW );

		pOutSamplerState.magFilter = ATL::ChooseGLTextureMagFilter( pSamplerConfig.filterConfig.magFilter, pSamplerConfig.filterConfig.mipMode );
		if( pOutSamplerState.magFilter == GL_INVALID_VALUE )
		{
			return false;
		}

		pOutSamplerState.minFilter = ATL::ChooseGLTextureMinFilter( pSamplerConfig.filterConfig.minFilter, pSamplerConfig.filterConfig.mipMode );
		if( pOutSamplerState.minFilter == GL_INVALID_VALUE )
		{
			return false;
		}

	#if( IC3_GX_GL_FEATURE_SUPPORT_TEXTURE_ANISOTROPIC_FILTER )
		pOutSamplerState.anisotropyLevel = pSamplerConfig.filterConfig.anisotropyLevel;
		if( pOutSamplerState.anisotropyLevel > 0 )
		{
			GLint maxAnisotropy = 0;

			glGetIntegerv( GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy );
			Ic3OpenGLCheckLastResult();

			if( pOutSamplerState.anisotropyLevel > maxAnisotropy )
			{
				pOutSamplerState.anisotropyLevel = maxAnisotropy;
			}
		}
	#endif

		if( pSamplerConfig.textureCompareMode == ETextureCompareMode::RefToTexture )
		{
			pOutSamplerState.textureCompareMode = GL_COMPARE_REF_TO_TEXTURE;
			pOutSamplerState.textureCompareFunc = ATL::TranslateGLCompFunc( pSamplerConfig.textureCompareFunc );
		}
		else
		{
			pOutSamplerState.textureCompareMode = GL_NONE;
			pOutSamplerState.textureCompareFunc = GL_INVALID_VALUE;
		}

		return true;
	}

} // namespace Ic3::Graphics::GCI
