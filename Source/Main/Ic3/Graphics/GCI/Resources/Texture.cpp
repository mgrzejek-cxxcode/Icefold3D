
#include "Texture.h"
#include "RenderTargetTexture.h"

namespace Ic3::Graphics::GCI
{

	Texture::Texture(
			GPUDevice & pGPUDevice,
			const ResourceMemoryInfo & pResourceMemory,
			const TextureProperties & pTextureProperties,
			const TextureLayout & pTextureLayout )
	: GPUResource( pGPUDevice, EGPUResourceBaseType::Texture, pResourceMemory )
	, mTextureProperties( pTextureProperties )
	, mTextureLayout( pTextureLayout )
	{}

	Texture::~Texture() = default;

	const GPUResourceProperties & Texture::getProperties() const
	{
		return mTextureProperties;
	}

	TextureSubResource Texture::getAllSubResourcesRef() const
	{
		switch( mTextureLayout.mTexClass )
		{
			case ETextureClass::T2D:
			{
				TextureSubResource2D subResource2D;
				subResource2D.mMipLevel = cxTexSubresourceMipLevelAllMips;
				return TextureSubResource{ subResource2D, ETextureClass::T2D };
			}
			case ETextureClass::T2DArray:
			{
				TextureSubResource2DArray subResource2DArray;
				subResource2DArray.mArrayIndex = cxTexSubresourceArrayIndexAllTextures;
				subResource2DArray.mMipLevel = cxTexSubresourceMipLevelAllMips;
				return TextureSubResource{ subResource2DArray, ETextureClass::T2DArray };
			}
			case ETextureClass::T2DMS:
			{
				TextureSubResource2D subResource2DMS;
				subResource2DMS.mMipLevel = 0; // MSAA textures only have a single sub-resource (mip level 0).
				return TextureSubResource{ subResource2DMS, ETextureClass::T2DMS };
			}
			case ETextureClass::T2DMSArray:
			{
				TextureSubResource2DArray subResource2DMSArray;
				subResource2DMSArray.mArrayIndex = cxTexSubresourceArrayIndexAllTextures;
				subResource2DMSArray.mMipLevel = 0; // MSAA textures only have a single sub-resource (mip level 0).
				return TextureSubResource{ subResource2DMSArray, ETextureClass::T2DMSArray };
			}
			case ETextureClass::T3D:
			{
				TextureSubResource3D subResource3D;
				subResource3D.mMipLevel = cxTexSubresourceMipLevelAllMips;
				subResource3D.mDepthLayerIndex = cxTexSubresourceDepthAllLayers;
				return TextureSubResource{ subResource3D };
			}
			case ETextureClass::TCubeMap:
			{
				TextureSubResourceCubeMap subResourceCubeMap;
				subResourceCubeMap.mFaceIndex = cxTexSubresourceCubeMapFaceAllFaces;
				subResourceCubeMap.mMipLevel = cxTexSubresourceMipLevelAllMips;
				return TextureSubResource{ subResourceCubeMap };
			}
			default:
			{
				return TextureSubResource{};
			}
		}
	}

	TextureSubResource Texture::getDefaultSubResourceRef() const
	{
		switch( mTextureLayout.mTexClass )
		{
			case ETextureClass::T2D:
			{
				TextureSubResource2D subResource2D;
				subResource2D.mMipLevel = 0;
				return TextureSubResource{ subResource2D, ETextureClass::T2D };
			}
			case ETextureClass::T2DArray:
			{
				TextureSubResource2DArray subResource2DArray;
				subResource2DArray.mArrayIndex = 0;
				subResource2DArray.mMipLevel = 0;
				return TextureSubResource{ subResource2DArray, ETextureClass::T2DArray };
			}
			case ETextureClass::T2DMS:
			{
				TextureSubResource2D subResource2DMS;
				subResource2DMS.mMipLevel = 0;
				return TextureSubResource{ subResource2DMS, ETextureClass::T2DMS };
			}
			case ETextureClass::T2DMSArray:
			{
				TextureSubResource2DArray subResource2DMSArray;
				subResource2DMSArray.mArrayIndex = 0;
				subResource2DMSArray.mMipLevel = 0;
				return TextureSubResource{ subResource2DMSArray, ETextureClass::T2DMSArray };
			}
			case ETextureClass::T3D:
			{
				TextureSubResource3D subResource3D;
				subResource3D.mMipLevel = 0;
				subResource3D.mDepthLayerIndex = cxTexSubresourceDepthAllLayers;
				return TextureSubResource{ subResource3D };
			}
			case ETextureClass::TCubeMap:
			{
				TextureSubResourceCubeMap subResourceCubeMap;
				subResourceCubeMap.mFaceIndex = eTextureCubeMapFaceFirst;
				subResourceCubeMap.mMipLevel = 0;
				return TextureSubResource{ subResourceCubeMap };
			}
			default:
			{
				return TextureSubResource{};
			}
		}
	}

	RenderTargetTextureHandle Texture::createDefaultRenderTargetTextureView(
			GPUDevice & pGPUDevice,
			const RenderTargetTextureCreateInfo & pCreateInfo )
	{
		if( pCreateInfo.targetTexture )
		{
			const auto rttType = RCU::queryRenderTargetTextureType( pCreateInfo.targetTexture->mTextureLayout.mInternalFormat );
			const auto rttLayout = RCU::queryRenderTargetTextureLayout( pCreateInfo.targetTexture->mTextureLayout );

			auto existingTextureRTT = createGPUAPIObject<RenderTargetTexture>( pGPUDevice, rttType, rttLayout, pCreateInfo.targetTexture );

			return existingTextureRTT;
		}

		return nullptr;
	}


	namespace RCU
	{

		TextureDimensions getValidTextureDimensions( ETextureClass pTexClass, const TextureDimensions & pDimensions )
		{
			TextureDimensions validTextureDimensions = pDimensions;

			if( pTexClass == ETextureClass::T2D )
			{
				validTextureDimensions.mArraySize = 1;
				validTextureDimensions.mDepth = 1;
			}
			else if( pTexClass == ETextureClass::T2DArray )
			{
				validTextureDimensions.mDepth = 1;
			}
			else if( pTexClass == ETextureClass::T2DMS )
			{
				validTextureDimensions.mArraySize = 1;
				validTextureDimensions.mDepth = 1;
				validTextureDimensions.mMipLevelsNum = 1;
			}
			else if( pTexClass == ETextureClass::T2DMSArray )
			{
				validTextureDimensions.mDepth = 1;
				validTextureDimensions.mMipLevelsNum = 1;
			}
			else if( pTexClass == ETextureClass::T3D )
			{
				validTextureDimensions.mArraySize = 1;
			}
			else if( pTexClass == ETextureClass::TCubeMap )
			{
				validTextureDimensions.mDepth = 1;
				validTextureDimensions.mArraySize = 1;
			}

			return validTextureDimensions;
		}

	}

} // namespace Ic3::Graphics::GCI
