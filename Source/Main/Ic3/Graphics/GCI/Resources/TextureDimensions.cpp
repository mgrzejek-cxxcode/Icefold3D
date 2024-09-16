
#include "TextureDimensions.h"
#include "Texture.h"

namespace Ic3::Graphics::GCI
{

	namespace RCU
	{

		TextureDimensions queryMipLevelDimensions( const TextureDimensions & pDimensions, uint32 pMipLevel ) noexcept
		{
			TextureDimensions mipLevelDimensions;
			mipLevelDimensions.mArraySize = pDimensions.mArraySize;
			mipLevelDimensions.mMipLevelsNum = pDimensions.mMipLevelsNum;

			if( pMipLevel >= pDimensions.mMipLevelsNum )
			{
				mipLevelDimensions.mWidth = 0;
				mipLevelDimensions.mHeight = 0;
				mipLevelDimensions.mDepth = 0;
			}
			else
			{

				mipLevelDimensions.mWidth = pDimensions.mWidth;
				mipLevelDimensions.mHeight = pDimensions.mHeight;
				mipLevelDimensions.mDepth = pDimensions.mDepth;

				for( uint32 mipLevelIndex = 0; mipLevelIndex < pMipLevel; ++mipLevelIndex )
				{
					mipLevelDimensions.mWidth = Cppx::getMaxOf( 1u, mipLevelDimensions.mWidth / 2 );
					mipLevelDimensions.mHeight= Cppx::getMaxOf( 1u, mipLevelDimensions.mHeight / 2 );
					mipLevelDimensions.mDepth = Cppx::getMaxOf( 1u, mipLevelDimensions.mDepth / 2 );
				}
			}

			return mipLevelDimensions;
		}

		TextureSubResource getTextureSubResourceWholeTexture( TextureHandle pTexture ) noexcept
		{
			TextureSubResource subResource{};
			if( pTexture )
			{
				subResource = pTexture->getAllSubResourcesRef();
			}
			return subResource;
		}

		bool checkTextureSubRegion( TextureHandle pTexture, const TextureSubRegion & pSubRegion ) noexcept
		{
			return pTexture && checkTextureSubRegion( pTexture->mTextureLayout.mDimensions, pSubRegion );
		}

		bool checkTextureSubRegion( const TextureDimensions & pDimensions, const TextureSubRegion & pSubRegion ) noexcept
		{
			if( pSubRegion.mTexClass == ETextureClass::T2D )
			{
				if( checkTextureSubRegion2D( pDimensions, pSubRegion.uSubReg2D ) )
				{
					return true;
				}
			}
			else if( pSubRegion.mTexClass == ETextureClass::T2DArray )
			{
				if( checkTextureSubRegion2DArray( pDimensions, pSubRegion.uSubReg2DArray ) )
				{
					return true;
				}
			}
			else if( pSubRegion.mTexClass == ETextureClass::T2DMS )
			{
				if( checkTextureSubRegion2DMS( pDimensions, pSubRegion.uSubReg2D ) )
				{
					return true;
				}
			}
			else if( pSubRegion.mTexClass == ETextureClass::T3D )
			{
				if( checkTextureSubRegion3D( pDimensions, pSubRegion.uSubReg3D ) )
				{
					return true;
				}
			}
			else if( pSubRegion.mTexClass == ETextureClass::TCubeMap )
			{
				if( checkTextureSubRegionCubeMap( pDimensions, pSubRegion.uSubRegCubeMap ) )
				{
					return true;
				}
			}
			else
			{
				ic3DebugInterrupt();
			}

			return false;
		}

		bool checkTextureSubRegion2D( const TextureDimensions & pDimensions, const TextureSubRegion2D & pSubRegion ) noexcept
		{
			const auto mipLevelDimensions = queryMipLevelDimensions( pDimensions, pSubRegion.mOffset.mMipLevel );

			const bool validSubRegion =
					( pSubRegion.mSize.mWidth > 0 ) &&
					( pSubRegion.mSize.mHeight > 0 ) &&
					( pSubRegion.mOffset.mPX < mipLevelDimensions.mWidth ) &&
					( pSubRegion.mOffset.mPY < mipLevelDimensions.mHeight ) &&
					( pSubRegion.mSize.mWidth < (mipLevelDimensions.mWidth - pSubRegion.mOffset.mPX ) ) &&
					( pSubRegion.mSize.mHeight > (mipLevelDimensions.mHeight - pSubRegion.mOffset.mPY ) );

			return validSubRegion;
		}

		bool checkTextureSubRegion2DArray( const TextureDimensions & pDimensions, const TextureSubRegion2DArray & pSubRegion ) noexcept
		{
			const bool validSubRegion =
					( pSubRegion.mOffset.mArrayIndex < pDimensions.mArraySize ) &&
					checkTextureSubRegion2D( pDimensions, TextureSubRegion2D{pSubRegion.mOffset, pSubRegion.mSize } );

			return validSubRegion;
		}

		bool checkTextureSubRegion2DMS( const TextureDimensions & pDimensions, const TextureSubRegion2D & pSubRegion ) noexcept
		{
			const bool validSubRegion =
					( pSubRegion.mOffset.mMipLevel == 0 ) &&
					checkTextureSubRegion2D( pDimensions, pSubRegion );

			return validSubRegion;
		}

		bool checkTextureSubRegion3D( const TextureDimensions & pDimensions, const TextureSubRegion3D & pSubRegion ) noexcept
		{
			const auto mipLevelDimensions = queryMipLevelDimensions( pDimensions, pSubRegion.mOffset.mMipLevel );

			const bool validSubRegion =
					( pSubRegion.mSize.mWidth > 0 ) &&
					( pSubRegion.mSize.mHeight > 0 ) &&
					( pSubRegion.mSize.mDepth > 0 ) &&
					( pSubRegion.mOffset.mPX < mipLevelDimensions.mWidth ) &&
					( pSubRegion.mOffset.mPY < mipLevelDimensions.mHeight ) &&
					( pSubRegion.mOffset.mPZ < mipLevelDimensions.mDepth ) &&
					( pSubRegion.mSize.mWidth < (mipLevelDimensions.mWidth - pSubRegion.mOffset.mPX ) ) &&
					( pSubRegion.mSize.mHeight < (mipLevelDimensions.mHeight - pSubRegion.mOffset.mPY ) ) &&
					( pSubRegion.mSize.mDepth < (mipLevelDimensions.mDepth - pSubRegion.mOffset.mPZ ) );

			return validSubRegion;
		}

		bool checkTextureSubRegionCubeMap( const TextureDimensions & pDimensions, const TextureSubRegionCubeMap & pSubRegion ) noexcept
		{
			const bool validSubRegion =
					( pSubRegion.mOffset.mFaceIndex <= 6 ) &&
					checkTextureSubRegion2D( pDimensions, TextureSubRegion2D{pSubRegion.mOffset, pSubRegion.mSize } );

			return validSubRegion;
		}

		bool checkTextureSubResource( TextureHandle pTexture, const TextureSubResource & pSubResource ) noexcept
		{
			return pTexture && checkTextureSubResource( pTexture->mTextureLayout.mDimensions, pSubResource );
		}

		bool checkTextureSubResource( const TextureDimensions & pDimensions, const TextureSubResource & pSubResource ) noexcept
		{
			if( pSubResource.mTexClass == ETextureClass::T2D )
			{
				if( checkTextureSubResource2D( pDimensions, pSubResource.uSubRes2D ) )
				{
					return true;
				}
			}
			else if( pSubResource.mTexClass == ETextureClass::T2DArray )
			{
				if( checkTextureSubResource2DArray( pDimensions, pSubResource.uSubRes2DArray ) )
				{
					return true;
				}
			}
			else if( pSubResource.mTexClass == ETextureClass::T2DMS )
			{
				if( checkTextureSubResource2DMS( pDimensions, pSubResource.uSubRes2D ) )
				{
					return true;
				}
			}
			else if( pSubResource.mTexClass == ETextureClass::T3D )
			{
				if( checkTextureSubResource3D( pDimensions, pSubResource.uSubRes3D ) )
				{
					return true;
				}
			}
			else if( pSubResource.mTexClass == ETextureClass::TCubeMap )
			{
				if( checkTextureSubResourceCubeMap( pDimensions, pSubResource.uSubResCubeMap ) )
				{
					return true;
				}
			}
			else
			{
				ic3DebugInterrupt();
			}

			return false;
		}

		bool checkTextureSubResource2D( const TextureDimensions & pDimensions, const TextureSubResource2D & pSubResource ) noexcept
		{
			const bool validSubResource =
				( pSubResource.mMipLevel < pDimensions.mMipLevelsNum );

			return validSubResource;
		}

		bool checkTextureSubResource2DArray( const TextureDimensions & pDimensions, const TextureSubResource2DArray & pSubResource ) noexcept
		{
			const bool validSubResource =
					( pSubResource.mArrayIndex < pDimensions.mArraySize ) &&
					( pSubResource.mMipLevel < pDimensions.mMipLevelsNum );

			return validSubResource;
		}

		bool checkTextureSubResource2DMS( const TextureDimensions & pDimensions, const TextureSubResource2D & pSubResource ) noexcept
		{
			const bool validSubResource =
				( pSubResource.mMipLevel == 0 );

			return validSubResource;
		}

		bool checkTextureSubResource3D( const TextureDimensions & pDimensions, const TextureSubResource3D & pSubResource ) noexcept
		{
			const bool validSubResource =
					( pSubResource.mDepthLayerIndex < pDimensions.mDepth ) &&
					( pSubResource.mMipLevel < pDimensions.mMipLevelsNum );

			return validSubResource;
		}

		bool checkTextureSubResourceCubeMap( const TextureDimensions & pDimensions, const TextureSubResourceCubeMap & pSubResource ) noexcept
		{
			const bool validSubResource =
					( pSubResource.mFaceIndex < 6 ) &&
					( pSubResource.mMipLevel < pDimensions.mMipLevelsNum );

			return validSubResource;
		}

		bool cmpEqTextureSubRegion( const TextureSubRegion & pSubRegion1, const TextureSubRegion & pSubRegion2 ) noexcept
		{
			if( pSubRegion1.mTexClass != pSubRegion2.mTexClass )
			{
				return false;
			}
			else
			{
				if( pSubRegion1.mTexClass == ETextureClass::T2D )
				{
					return cmpEqTextureSubRegion2D( pSubRegion1.uSubReg2D, pSubRegion2.uSubReg2D );
				}
				else if( pSubRegion1.mTexClass == ETextureClass::T2DArray )
				{
					return cmpEqTextureSubRegion2DArray( pSubRegion1.uSubReg2DArray, pSubRegion2.uSubReg2DArray );
				}
				else if( pSubRegion1.mTexClass == ETextureClass::T2DMS )
				{
					return cmpEqTextureSubRegion2DMS( pSubRegion1.uSubReg2D, pSubRegion2.uSubReg2D );
				}
				else if( pSubRegion1.mTexClass == ETextureClass::T3D )
				{
					return cmpEqTextureSubRegion3D( pSubRegion1.uSubReg3D, pSubRegion2.uSubReg3D );
				}
				else if( pSubRegion1.mTexClass == ETextureClass::TCubeMap )
				{
					return cmpEqTextureSubRegionCubeMap( pSubRegion1.uSubRegCubeMap, pSubRegion2.uSubRegCubeMap );
				}
				else
				{
					// Two uninitialized subregions (both with texClass == ETextureClass::Unknown) are considered equal.
					return true;
				}
			}
		}

		bool cmpEqTextureSubRegion2D( const TextureSubRegion2D & pSubRegion1, const TextureSubRegion2D & pSubRegion2 ) noexcept
		{
			const bool subRegionsEqual =
					( pSubRegion1.mOffset.mMipLevel == pSubRegion2.mOffset.mMipLevel ) &&
					( pSubRegion1.mOffset.mPX == pSubRegion2.mOffset.mPX ) &&
					( pSubRegion1.mOffset.mPY == pSubRegion2.mOffset.mPY ) &&
					( pSubRegion1.mSize.mWidth == pSubRegion2.mSize.mWidth ) &&
					( pSubRegion1.mSize.mHeight == pSubRegion2.mSize.mHeight );

			return subRegionsEqual;
		}

		bool cmpEqTextureSubRegion2DArray( const TextureSubRegion2DArray & pSubRegion1, const TextureSubRegion2DArray & pSubRegion2 ) noexcept
		{
			const bool subRegionsEqual =
					( pSubRegion1.mOffset.mArrayIndex == pSubRegion2.mOffset.mArrayIndex ) &&
					( pSubRegion1.mOffset.mPX == pSubRegion2.mOffset.mPX ) &&
					( pSubRegion1.mOffset.mPY == pSubRegion2.mOffset.mPY ) &&
					( pSubRegion1.mSize.mArraySize == pSubRegion2.mSize.mArraySize ) &&
					( pSubRegion1.mSize.mWidth == pSubRegion2.mSize.mWidth ) &&
					( pSubRegion1.mSize.mHeight == pSubRegion2.mSize.mHeight );

			return subRegionsEqual;
		}

		bool cmpEqTextureSubRegion2DMS( const TextureSubRegion2D & pSubRegion1, const TextureSubRegion2D & pSubRegion2 ) noexcept
		{
			const bool subRegionsEqual =
					( pSubRegion1.mOffset.mMipLevel == pSubRegion2.mOffset.mMipLevel ) &&
					( pSubRegion1.mOffset.mPX == pSubRegion2.mOffset.mPX ) &&
					( pSubRegion1.mOffset.mPY == pSubRegion2.mOffset.mPY ) &&
					( pSubRegion1.mSize.mWidth == pSubRegion2.mSize.mWidth ) &&
					( pSubRegion1.mSize.mHeight == pSubRegion2.mSize.mHeight );

			return subRegionsEqual;
		}

		bool cmpEqTextureSubRegion3D( const TextureSubRegion3D & pSubRegion1, const TextureSubRegion3D & pSubRegion2 ) noexcept
		{
			const bool subRegionsEqual =
					( pSubRegion1.mOffset.mMipLevel == pSubRegion2.mOffset.mMipLevel ) &&
					( pSubRegion1.mOffset.mPX == pSubRegion2.mOffset.mPX ) &&
					( pSubRegion1.mOffset.mPY == pSubRegion2.mOffset.mPY ) &&
					( pSubRegion1.mOffset.mPZ == pSubRegion2.mOffset.mPZ ) &&
					( pSubRegion1.mSize.mWidth == pSubRegion2.mSize.mWidth ) &&
					( pSubRegion1.mSize.mHeight == pSubRegion2.mSize.mHeight ) &&
					( pSubRegion1.mSize.mDepth == pSubRegion2.mSize.mDepth );

			return subRegionsEqual;
		}

		bool cmpEqTextureSubRegionCubeMap( const TextureSubRegionCubeMap & pSubRegion1, const TextureSubRegionCubeMap & pSubRegion2 ) noexcept
		{
			const bool subRegionsEqual =
					( pSubRegion1.mOffset.mFaceIndex == pSubRegion2.mOffset.mFaceIndex ) &&
					( pSubRegion1.mOffset.mMipLevel == pSubRegion2.mOffset.mMipLevel ) &&
					( pSubRegion1.mOffset.mPX == pSubRegion2.mOffset.mPX ) &&
					( pSubRegion1.mOffset.mPY == pSubRegion2.mOffset.mPY ) &&
					( pSubRegion1.mSize.mWidth == pSubRegion2.mSize.mWidth ) &&
					( pSubRegion1.mSize.mHeight == pSubRegion2.mSize.mHeight );

			return subRegionsEqual;
		}

		bool cmpEqTextureSubResource( const TextureSubResource & pSubResource1, const TextureSubResource & pSubResource2 ) noexcept
		{
			if( pSubResource1.mTexClass != pSubResource2.mTexClass )
			{
				return false;
			}
			else
			{
				if( pSubResource1.mTexClass == ETextureClass::T2D )
				{
					return cmpEqTextureSubResource2D( pSubResource1.uSubRes2D, pSubResource2.uSubRes2D );
				}
				else if( pSubResource1.mTexClass == ETextureClass::T2DArray )
				{
					return cmpEqTextureSubResource2DArray( pSubResource1.uSubRes2DArray, pSubResource2.uSubRes2DArray );
				}
				else if( pSubResource1.mTexClass == ETextureClass::T2DMS )
				{
					return cmpEqTextureSubResource2DMS( pSubResource1.uSubRes2D, pSubResource2.uSubRes2D );
				}
				else if( pSubResource1.mTexClass == ETextureClass::T3D )
				{
					return cmpEqTextureSubResource3D( pSubResource1.uSubRes3D, pSubResource2.uSubRes3D );
				}
				else if( pSubResource1.mTexClass == ETextureClass::TCubeMap )
				{
					return cmpEqTextureSubResourceCubeMap( pSubResource1.uSubResCubeMap, pSubResource2.uSubResCubeMap );
				}
				else
				{
					// Two uninitialized subregions (both with texClass == ETextureClass::Unknown) are considered equal.
					return true;
				}
			}
		}

		bool cmpEqTextureSubResource2D( const TextureSubResource2D & pSubResource1, const TextureSubResource2D & pSubResource2 ) noexcept
		{
			const bool subResourcesEqual =
				( pSubResource1.mMipLevel == pSubResource2.mMipLevel );

			return subResourcesEqual;
		}

		bool cmpEqTextureSubResource2DArray( const TextureSubResource2DArray & pSubResource1, const TextureSubResource2DArray & pSubResource2 ) noexcept
		{
			const bool subResourcesEqual =
					( pSubResource1.mArrayIndex == pSubResource2.mArrayIndex ) &&
					( pSubResource1.mMipLevel == pSubResource2.mMipLevel );

			return subResourcesEqual;
		}

		bool cmpEqTextureSubResource2DMS( const TextureSubResource2D & pSubResource1, const TextureSubResource2D & pSubResource2 ) noexcept
		{
			const bool subResourcesEqual =
				( pSubResource1.mMipLevel == pSubResource2.mMipLevel );

			return subResourcesEqual;
		}

		bool cmpEqTextureSubResource3D( const TextureSubResource3D & pSubResource1, const TextureSubResource3D & pSubResource2 ) noexcept
		{
			const bool subResourcesEqual =
					( pSubResource1.mMipLevel == pSubResource2.mMipLevel ) &&
					( pSubResource1.mDepthLayerIndex == pSubResource2.mDepthLayerIndex );

			return subResourcesEqual;
		}

		bool cmpEqTextureSubResourceCubeMap( const TextureSubResourceCubeMap & pSubResource1, const TextureSubResourceCubeMap & pSubResource2 ) noexcept
		{
			const bool subResourcesEqual =
					( pSubResource1.mFaceIndex == pSubResource2.mFaceIndex ) &&
					( pSubResource1.mMipLevel == pSubResource2.mMipLevel );

			return subResourcesEqual;
		}
		
	}

}
