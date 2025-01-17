
#include "TextureDimensions.h"
#include "Texture.h"

namespace Ic3::Graphics::GCI
{

	namespace RCU
	{

		TextureDimensions QueryMipLevelDimensions( const TextureDimensions & pDimensions, uint32 pMipLevel ) noexcept
		{
			TextureDimensions mipLevelDimensions;
			mipLevelDimensions.arraySize = pDimensions.arraySize;
			mipLevelDimensions.mipLevelsNum = pDimensions.mipLevelsNum;

			if( pMipLevel >= pDimensions.mipLevelsNum )
			{
				mipLevelDimensions.width = 0;
				mipLevelDimensions.height = 0;
				mipLevelDimensions.depth = 0;
			}
			else
			{

				mipLevelDimensions.width = pDimensions.width;
				mipLevelDimensions.height = pDimensions.height;
				mipLevelDimensions.depth = pDimensions.depth;

				for( uint32 mipLevelIndex = 0; mipLevelIndex < pMipLevel; ++mipLevelIndex )
				{
					mipLevelDimensions.width = cppx::get_max_of( 1u, mipLevelDimensions.width / 2 );
					mipLevelDimensions.height= cppx::get_max_of( 1u, mipLevelDimensions.height / 2 );
					mipLevelDimensions.depth = cppx::get_max_of( 1u, mipLevelDimensions.depth / 2 );
				}
			}

			return mipLevelDimensions;
		}

		TextureSubResource GetTextureSubResourceWholeTexture( TextureHandle pTexture ) noexcept
		{
			TextureSubResource subResource{};
			if( pTexture )
			{
				subResource = pTexture->GetAllSubResourcesRef();
			}
			return subResource;
		}

		bool CheckTextureSubRegion( TextureHandle pTexture, const TextureSubRegion & pSubRegion ) noexcept
		{
			return pTexture && CheckTextureSubRegion( pTexture->mTextureLayout.dimensions, pSubRegion );
		}

		bool CheckTextureSubRegion( const TextureDimensions & pDimensions, const TextureSubRegion & pSubRegion ) noexcept
		{
			if( pSubRegion.texClass == ETextureClass::T2D )
			{
				if( CheckTextureSubRegion2D( pDimensions, pSubRegion.uSubReg2D ) )
				{
					return true;
				}
			}
			else if( pSubRegion.texClass == ETextureClass::T2DArray )
			{
				if( CheckTextureSubRegion2DArray( pDimensions, pSubRegion.uSubReg2DArray ) )
				{
					return true;
				}
			}
			else if( pSubRegion.texClass == ETextureClass::T2DMS )
			{
				if( CheckTextureSubRegion2DMS( pDimensions, pSubRegion.uSubReg2D ) )
				{
					return true;
				}
			}
			else if( pSubRegion.texClass == ETextureClass::T3D )
			{
				if( CheckTextureSubRegion3D( pDimensions, pSubRegion.uSubReg3D ) )
				{
					return true;
				}
			}
			else if( pSubRegion.texClass == ETextureClass::TCubeMap )
			{
				if( CheckTextureSubRegionCubeMap( pDimensions, pSubRegion.uSubRegCubeMap ) )
				{
					return true;
				}
			}
			else
			{
				Ic3DebugInterrupt();
			}

			return false;
		}

		bool CheckTextureSubRegion2D( const TextureDimensions & pDimensions, const TextureSubRegion2D & pSubRegion ) noexcept
		{
			const auto mipLevelDimensions = QueryMipLevelDimensions( pDimensions, pSubRegion.offset.mipLevel );

			const bool validSubRegion =
					( pSubRegion.size.width > 0 ) &&
					( pSubRegion.size.height > 0 ) &&
					( pSubRegion.offset.x < mipLevelDimensions.width ) &&
					( pSubRegion.offset.y < mipLevelDimensions.height ) &&
					( pSubRegion.size.width < (mipLevelDimensions.width - pSubRegion.offset.x ) ) &&
					( pSubRegion.size.height > (mipLevelDimensions.height - pSubRegion.offset.y ) );

			return validSubRegion;
		}

		bool CheckTextureSubRegion2DArray( const TextureDimensions & pDimensions, const TextureSubRegion2DArray & pSubRegion ) noexcept
		{
			const bool validSubRegion =
					( pSubRegion.offset.arrayIndex < pDimensions.arraySize ) &&
					CheckTextureSubRegion2D( pDimensions, TextureSubRegion2D{pSubRegion.offset, pSubRegion.size } );

			return validSubRegion;
		}

		bool CheckTextureSubRegion2DMS( const TextureDimensions & pDimensions, const TextureSubRegion2D & pSubRegion ) noexcept
		{
			const bool validSubRegion =
					( pSubRegion.offset.mipLevel == 0 ) &&
					CheckTextureSubRegion2D( pDimensions, pSubRegion );

			return validSubRegion;
		}

		bool CheckTextureSubRegion3D( const TextureDimensions & pDimensions, const TextureSubRegion3D & pSubRegion ) noexcept
		{
			const auto mipLevelDimensions = QueryMipLevelDimensions( pDimensions, pSubRegion.offset.mipLevel );

			const bool validSubRegion =
					( pSubRegion.size.width > 0 ) &&
					( pSubRegion.size.height > 0 ) &&
					( pSubRegion.size.depth > 0 ) &&
					( pSubRegion.offset.x < mipLevelDimensions.width ) &&
					( pSubRegion.offset.y < mipLevelDimensions.height ) &&
					( pSubRegion.offset.z < mipLevelDimensions.depth ) &&
					( pSubRegion.size.width < (mipLevelDimensions.width - pSubRegion.offset.x ) ) &&
					( pSubRegion.size.height < (mipLevelDimensions.height - pSubRegion.offset.y ) ) &&
					( pSubRegion.size.depth < (mipLevelDimensions.depth - pSubRegion.offset.z ) );

			return validSubRegion;
		}

		bool CheckTextureSubRegionCubeMap( const TextureDimensions & pDimensions, const TextureSubRegionCubeMap & pSubRegion ) noexcept
		{
			const bool validSubRegion =
					( pSubRegion.offset.faceIndex <= 6 ) &&
					CheckTextureSubRegion2D( pDimensions, TextureSubRegion2D{pSubRegion.offset, pSubRegion.size } );

			return validSubRegion;
		}

		bool CheckTextureSubResource( TextureHandle pTexture, const TextureSubResource & pSubResource ) noexcept
		{
			return pTexture && CheckTextureSubResource( pTexture->mTextureLayout.dimensions, pSubResource );
		}

		bool CheckTextureSubResource( const TextureDimensions & pDimensions, const TextureSubResource & pSubResource ) noexcept
		{
			if( pSubResource.texClass == ETextureClass::T2D )
			{
				if( CheckTextureSubResource2D( pDimensions, pSubResource.uSubRes2D ) )
				{
					return true;
				}
			}
			else if( pSubResource.texClass == ETextureClass::T2DArray )
			{
				if( CheckTextureSubResource2DArray( pDimensions, pSubResource.uSubRes2DArray ) )
				{
					return true;
				}
			}
			else if( pSubResource.texClass == ETextureClass::T2DMS )
			{
				if( CheckTextureSubResource2DMS( pDimensions, pSubResource.uSubRes2D ) )
				{
					return true;
				}
			}
			else if( pSubResource.texClass == ETextureClass::T3D )
			{
				if( CheckTextureSubResource3D( pDimensions, pSubResource.uSubRes3D ) )
				{
					return true;
				}
			}
			else if( pSubResource.texClass == ETextureClass::TCubeMap )
			{
				if( CheckTextureSubResourceCubeMap( pDimensions, pSubResource.uSubResCubeMap ) )
				{
					return true;
				}
			}
			else
			{
				Ic3DebugInterrupt();
			}

			return false;
		}

		bool CheckTextureSubResource2D( const TextureDimensions & pDimensions, const TextureSubResource2D & pSubResource ) noexcept
		{
			const bool validSubResource =
				( pSubResource.mipLevel < pDimensions.mipLevelsNum );

			return validSubResource;
		}

		bool CheckTextureSubResource2DArray( const TextureDimensions & pDimensions, const TextureSubResource2DArray & pSubResource ) noexcept
		{
			const bool validSubResource =
					( pSubResource.arrayIndex < pDimensions.arraySize ) &&
					( pSubResource.mipLevel < pDimensions.mipLevelsNum );

			return validSubResource;
		}

		bool CheckTextureSubResource2DMS( const TextureDimensions & pDimensions, const TextureSubResource2D & pSubResource ) noexcept
		{
			const bool validSubResource =
				( pSubResource.mipLevel == 0 );

			return validSubResource;
		}

		bool CheckTextureSubResource3D( const TextureDimensions & pDimensions, const TextureSubResource3D & pSubResource ) noexcept
		{
			const bool validSubResource =
					( pSubResource.depthLayerIndex < pDimensions.depth ) &&
					( pSubResource.mipLevel < pDimensions.mipLevelsNum );

			return validSubResource;
		}

		bool CheckTextureSubResourceCubeMap( const TextureDimensions & pDimensions, const TextureSubResourceCubeMap & pSubResource ) noexcept
		{
			const bool validSubResource =
					( pSubResource.faceIndex < 6 ) &&
					( pSubResource.mipLevel < pDimensions.mipLevelsNum );

			return validSubResource;
		}

		bool CmpEqTextureSubRegion( const TextureSubRegion & pSubRegion1, const TextureSubRegion & pSubRegion2 ) noexcept
		{
			if( pSubRegion1.texClass != pSubRegion2.texClass )
			{
				return false;
			}
			else
			{
				if( pSubRegion1.texClass == ETextureClass::T2D )
				{
					return CmpEqTextureSubRegion2D( pSubRegion1.uSubReg2D, pSubRegion2.uSubReg2D );
				}
				else if( pSubRegion1.texClass == ETextureClass::T2DArray )
				{
					return CmpEqTextureSubRegion2DArray( pSubRegion1.uSubReg2DArray, pSubRegion2.uSubReg2DArray );
				}
				else if( pSubRegion1.texClass == ETextureClass::T2DMS )
				{
					return CmpEqTextureSubRegion2DMS( pSubRegion1.uSubReg2D, pSubRegion2.uSubReg2D );
				}
				else if( pSubRegion1.texClass == ETextureClass::T3D )
				{
					return CmpEqTextureSubRegion3D( pSubRegion1.uSubReg3D, pSubRegion2.uSubReg3D );
				}
				else if( pSubRegion1.texClass == ETextureClass::TCubeMap )
				{
					return CmpEqTextureSubRegionCubeMap( pSubRegion1.uSubRegCubeMap, pSubRegion2.uSubRegCubeMap );
				}
				else
				{
					// Two uninitialized subregions (both with texClass == ETextureClass::Unknown) are considered equal.
					return true;
				}
			}
		}

		bool CmpEqTextureSubRegion2D( const TextureSubRegion2D & pSubRegion1, const TextureSubRegion2D & pSubRegion2 ) noexcept
		{
			const bool subRegionsEqual =
					( pSubRegion1.offset.mipLevel == pSubRegion2.offset.mipLevel ) &&
					( pSubRegion1.offset.x == pSubRegion2.offset.x ) &&
					( pSubRegion1.offset.y == pSubRegion2.offset.y ) &&
					( pSubRegion1.size.width == pSubRegion2.size.width ) &&
					( pSubRegion1.size.height == pSubRegion2.size.height );

			return subRegionsEqual;
		}

		bool CmpEqTextureSubRegion2DArray( const TextureSubRegion2DArray & pSubRegion1, const TextureSubRegion2DArray & pSubRegion2 ) noexcept
		{
			const bool subRegionsEqual =
					( pSubRegion1.offset.arrayIndex == pSubRegion2.offset.arrayIndex ) &&
					( pSubRegion1.offset.x == pSubRegion2.offset.x ) &&
					( pSubRegion1.offset.y == pSubRegion2.offset.y ) &&
					( pSubRegion1.size.arraySize == pSubRegion2.size.arraySize ) &&
					( pSubRegion1.size.width == pSubRegion2.size.width ) &&
					( pSubRegion1.size.height == pSubRegion2.size.height );

			return subRegionsEqual;
		}

		bool CmpEqTextureSubRegion2DMS( const TextureSubRegion2D & pSubRegion1, const TextureSubRegion2D & pSubRegion2 ) noexcept
		{
			const bool subRegionsEqual =
					( pSubRegion1.offset.mipLevel == pSubRegion2.offset.mipLevel ) &&
					( pSubRegion1.offset.x == pSubRegion2.offset.x ) &&
					( pSubRegion1.offset.y == pSubRegion2.offset.y ) &&
					( pSubRegion1.size.width == pSubRegion2.size.width ) &&
					( pSubRegion1.size.height == pSubRegion2.size.height );

			return subRegionsEqual;
		}

		bool CmpEqTextureSubRegion3D( const TextureSubRegion3D & pSubRegion1, const TextureSubRegion3D & pSubRegion2 ) noexcept
		{
			const bool subRegionsEqual =
					( pSubRegion1.offset.mipLevel == pSubRegion2.offset.mipLevel ) &&
					( pSubRegion1.offset.x == pSubRegion2.offset.x ) &&
					( pSubRegion1.offset.y == pSubRegion2.offset.y ) &&
					( pSubRegion1.offset.z == pSubRegion2.offset.z ) &&
					( pSubRegion1.size.width == pSubRegion2.size.width ) &&
					( pSubRegion1.size.height == pSubRegion2.size.height ) &&
					( pSubRegion1.size.depth == pSubRegion2.size.depth );

			return subRegionsEqual;
		}

		bool CmpEqTextureSubRegionCubeMap( const TextureSubRegionCubeMap & pSubRegion1, const TextureSubRegionCubeMap & pSubRegion2 ) noexcept
		{
			const bool subRegionsEqual =
					( pSubRegion1.offset.faceIndex == pSubRegion2.offset.faceIndex ) &&
					( pSubRegion1.offset.mipLevel == pSubRegion2.offset.mipLevel ) &&
					( pSubRegion1.offset.x == pSubRegion2.offset.x ) &&
					( pSubRegion1.offset.y == pSubRegion2.offset.y ) &&
					( pSubRegion1.size.width == pSubRegion2.size.width ) &&
					( pSubRegion1.size.height == pSubRegion2.size.height );

			return subRegionsEqual;
		}

		bool CmpEqTextureSubResource( const TextureSubResource & pSubResource1, const TextureSubResource & pSubResource2 ) noexcept
		{
			if( pSubResource1.texClass != pSubResource2.texClass )
			{
				return false;
			}
			else
			{
				if( pSubResource1.texClass == ETextureClass::T2D )
				{
					return CmpEqTextureSubResource2D( pSubResource1.uSubRes2D, pSubResource2.uSubRes2D );
				}
				else if( pSubResource1.texClass == ETextureClass::T2DArray )
				{
					return CmpEqTextureSubResource2DArray( pSubResource1.uSubRes2DArray, pSubResource2.uSubRes2DArray );
				}
				else if( pSubResource1.texClass == ETextureClass::T2DMS )
				{
					return CmpEqTextureSubResource2DMS( pSubResource1.uSubRes2D, pSubResource2.uSubRes2D );
				}
				else if( pSubResource1.texClass == ETextureClass::T3D )
				{
					return CmpEqTextureSubResource3D( pSubResource1.uSubRes3D, pSubResource2.uSubRes3D );
				}
				else if( pSubResource1.texClass == ETextureClass::TCubeMap )
				{
					return CmpEqTextureSubResourceCubeMap( pSubResource1.uSubResCubeMap, pSubResource2.uSubResCubeMap );
				}
				else
				{
					// Two uninitialized subregions (both with texClass == ETextureClass::Unknown) are considered equal.
					return true;
				}
			}
		}

		bool CmpEqTextureSubResource2D( const TextureSubResource2D & pSubResource1, const TextureSubResource2D & pSubResource2 ) noexcept
		{
			const bool subResourcesEqual =
				( pSubResource1.mipLevel == pSubResource2.mipLevel );

			return subResourcesEqual;
		}

		bool CmpEqTextureSubResource2DArray( const TextureSubResource2DArray & pSubResource1, const TextureSubResource2DArray & pSubResource2 ) noexcept
		{
			const bool subResourcesEqual =
					( pSubResource1.arrayIndex == pSubResource2.arrayIndex ) &&
					( pSubResource1.mipLevel == pSubResource2.mipLevel );

			return subResourcesEqual;
		}

		bool CmpEqTextureSubResource2DMS( const TextureSubResource2D & pSubResource1, const TextureSubResource2D & pSubResource2 ) noexcept
		{
			const bool subResourcesEqual =
				( pSubResource1.mipLevel == pSubResource2.mipLevel );

			return subResourcesEqual;
		}

		bool CmpEqTextureSubResource3D( const TextureSubResource3D & pSubResource1, const TextureSubResource3D & pSubResource2 ) noexcept
		{
			const bool subResourcesEqual =
					( pSubResource1.mipLevel == pSubResource2.mipLevel ) &&
					( pSubResource1.depthLayerIndex == pSubResource2.depthLayerIndex );

			return subResourcesEqual;
		}

		bool CmpEqTextureSubResourceCubeMap( const TextureSubResourceCubeMap & pSubResource1, const TextureSubResourceCubeMap & pSubResource2 ) noexcept
		{
			const bool subResourcesEqual =
					( pSubResource1.faceIndex == pSubResource2.faceIndex ) &&
					( pSubResource1.mipLevel == pSubResource2.mipLevel );

			return subResourcesEqual;
		}
		
	}

}
