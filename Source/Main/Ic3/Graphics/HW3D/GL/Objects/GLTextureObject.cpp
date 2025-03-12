
#include "GLTextureObject.h"
#include <Ic3/Graphics/HW3D/GL/GLAPITranslationLayer.h>

namespace Ic3::Graphics::GCI
{

	GLTextureObject::GLTextureObject( GLuint pHandle, const GLTextureCreateInfo & pGLCreateInfo )
	: GLObject( GLObjectBaseType::Texture, pHandle )
	, dimensions( pGLCreateInfo.dimensions )
	, mGLTextureBindTarget( pGLCreateInfo.bindTarget )
	, mGLInternalFormat( pGLCreateInfo.internalFormat )
	, mMultisampleLevel( pGLCreateInfo.msaaLevel )
	{}

	GLTextureObject::~GLTextureObject() = default;

	GLTextureObjectHandle GLTextureObject::CreateCore( const GLTextureCreateInfo & pGLCreateInfo )
	{
		GLuint textureHandle = 0;

		glGenTextures( 1, &textureHandle );
		Ic3OpenGLHandleLastError();

		glBindTexture( pGLCreateInfo.bindTarget, textureHandle );
		Ic3OpenGLHandleLastError();

		GLTextureObjectHandle openglTextureObject{ new GLTextureObject( textureHandle, pGLCreateInfo ) };
		if( !openglTextureObject->InitializeCore( pGLCreateInfo ) )
		{
			return nullptr;
		}

		return openglTextureObject;
	}

	GLTextureObjectHandle GLTextureObject::CreateCompat( const GLTextureCreateInfo & pGLCreateInfo )
	{
		GLuint textureHandle = 0;

		glGenTextures( 1, &textureHandle );
		Ic3OpenGLHandleLastError();

		glBindTexture( pGLCreateInfo.bindTarget, textureHandle );
		Ic3OpenGLHandleLastError();

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0 );
		Ic3OpenGLHandleLastError();

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, pGLCreateInfo.dimensions.mipLevelsNum - 1 );
		Ic3OpenGLHandleLastError();

		GLTextureObjectHandle openglTextureObject{ new GLTextureObject( textureHandle, pGLCreateInfo ) };
		if( !openglTextureObject->InitializeCompat( pGLCreateInfo ) )
		{
			return nullptr;
		}

		if((pGLCreateInfo.dimensions.mipLevelsNum > 1 ) &&
				pGLCreateInfo.openglInitDataDesc.textureInitFlags.is_set( eTextureInitFlagGenerateMipmapsBit ) )
		{
			openglTextureObject->SetAutoMipGeneration( true );

			glGenerateMipmap( GL_TEXTURE_2D );
			Ic3OpenGLHandleLastError();
		}

		return openglTextureObject;
	}

	bool GLTextureObject::IsAutoMipGenerationEnabled() const
	{
		return _autoMipGenerationStatus;
	}

	GLenum GLTextureObject::QueryInternalFormat( GLenum pActiveBindTarget ) const
	{
		auto textureBindTarget = CheckActiveBindTarget( pActiveBindTarget );

		GLint textureInternalFormat = 0;
		glGetTexLevelParameteriv( textureBindTarget, 0, GL_TEXTURE_INTERNAL_FORMAT, &textureInternalFormat );
		Ic3OpenGLHandleLastError();

		return textureInternalFormat;
	}

	memory_size_t GLTextureObject::QuerySize( GLenum pActiveBindTarget ) const
	{
		auto textureBindTarget = CheckActiveBindTarget( pActiveBindTarget );

		memory_size_t textureImageSize = 0;
		GLint textureCompressedFlag = 0;

	#if( IC3_GX_GL_FEATURE_SUPPORT_TEXTURE_FORMAT_COMPRESSED_BCX )
		glGetTexLevelParameteriv( textureBindTarget, 0, GL_TEXTURE_COMPRESSED, &textureCompressedFlag );
		Ic3OpenGLHandleLastError();
	#endif

		if( textureCompressedFlag )
		{
		#if( IC3_GX_GL_FEATURE_SUPPORT_TEXTURE_FORMAT_COMPRESSED_BCX )
			GLint textureCompressedImageSize = 0;
			glGetTexLevelParameteriv( textureBindTarget, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &textureCompressedImageSize );
			Ic3OpenGLHandleLastError();

			textureImageSize = textureCompressedImageSize;
		#endif
		}
		else
		{
			GLint textureWidth = 0;
			glGetTexLevelParameteriv( textureBindTarget, 0, GL_TEXTURE_WIDTH, &textureWidth );
			Ic3OpenGLHandleLastError();

			GLint textureHeight = 0;
			glGetTexLevelParameteriv( textureBindTarget, 0, GL_TEXTURE_HEIGHT, &textureHeight );
			Ic3OpenGLHandleLastError();

			GLint textureDepth = 0;
			glGetTexLevelParameteriv( textureBindTarget, 0, GL_TEXTURE_DEPTH, &textureDepth );
			Ic3OpenGLHandleLastError();

			GLint textureInternalFormat = 0;
			glGetTexLevelParameteriv( textureBindTarget, 0, GL_TEXTURE_INTERNAL_FORMAT, &textureInternalFormat );
			Ic3OpenGLHandleLastError();

			auto internalFormatBPP = ATL::GLQueryTextureInternalFormatBPP( textureInternalFormat );
			auto texturePixelCount = textureWidth * textureHeight * textureDepth;

			textureImageSize = texturePixelCount * internalFormatBPP / 8;
		}

		return textureImageSize;
	}

	bool GLTextureObject::Release()
	{
		glDeleteTextures( 1, &mGLHandle );
		Ic3OpenGLHandleLastError();

		return true;
	}

	bool GLTextureObject::ValidateHandle() const
	{
		const auto checkResult = glIsTexture( mGLHandle );
		Ic3OpenGLHandleLastError();

		return checkResult != GL_FALSE;
	}

	void GLTextureObject::UpdateCopy2D( GLTextureObject & pSrcTexture, const TextureSubDataCopyDesc & pCopyDesc, GLenum pActiveBindTarget )
	{
	}

	void GLTextureObject::UpdateCopy2DArray( GLTextureObject & pSrcTexture, const TextureSubDataCopyDesc & pCopyDesc, GLenum pActiveBindTarget )
	{
	}

	void GLTextureObject::UpdateCopy3D( GLTextureObject & pSrcTexture, const TextureSubDataCopyDesc & pCopyDesc, GLenum pActiveBindTarget )
	{
	}

	void GLTextureObject::UpdateCopyCubeMap( GLTextureObject & pSrcTexture, const TextureSubDataCopyDesc & pCopyDesc, GLenum pActiveBindTarget )
	{
	}

	void GLTextureObject::UpdateUpload2D( const GLTextureSubDataUploadDesc & pGLUploadDesc, GLenum pActiveBindTarget )
	{
		Ic3DebugAssert( pGLUploadDesc.openglDimensionClass == GL_TEXTURE_2D );
		Ic3DebugAssert( ( pActiveBindTarget == 0 ) || ( pActiveBindTarget == GL_TEXTURE_2D ) );

		auto textureBindTarget = CheckActiveBindTarget( pActiveBindTarget );

		glTexSubImage2D(
			textureBindTarget,
			pGLUploadDesc.textureSubRegion.uSubReg2D.offset.mipLevel,
			pGLUploadDesc.textureSubRegion.uSubReg2D.offset.x,
			pGLUploadDesc.textureSubRegion.uSubReg2D.offset.y,
			pGLUploadDesc.textureSubRegion.uSubReg2D.size.width,
			pGLUploadDesc.textureSubRegion.uSubReg2D.size.height,
			pGLUploadDesc.openglInputDataDesc.openglPixelDataLayout,
			pGLUploadDesc.openglInputDataDesc.openglPixelDataType,
			pGLUploadDesc.openglInputDataDesc.pointer );
		Ic3OpenGLHandleLastError();
	}

	void GLTextureObject::UpdateUpload2DArray( const GLTextureSubDataUploadDesc & pGLUploadDesc, GLenum pActiveBindTarget )
	{
		Ic3DebugAssert( pGLUploadDesc.openglDimensionClass == GL_TEXTURE_2D_ARRAY );
		Ic3DebugAssert( ( pActiveBindTarget == 0 ) || ( pActiveBindTarget == GL_TEXTURE_2D_ARRAY ) );

		auto textureBindTarget = CheckActiveBindTarget( pActiveBindTarget );

		glTexSubImage3D(
			textureBindTarget,
			pGLUploadDesc.textureSubRegion.uSubReg2DArray.offset.mipLevel,
			pGLUploadDesc.textureSubRegion.uSubReg2DArray.offset.x,
			pGLUploadDesc.textureSubRegion.uSubReg2DArray.offset.y,
			pGLUploadDesc.textureSubRegion.uSubReg2DArray.offset.arrayIndex,
			pGLUploadDesc.textureSubRegion.uSubReg2DArray.size.width,
			pGLUploadDesc.textureSubRegion.uSubReg2DArray.size.height,
			1,
			pGLUploadDesc.openglInputDataDesc.openglPixelDataLayout,
			pGLUploadDesc.openglInputDataDesc.openglPixelDataType,
			pGLUploadDesc.openglInputDataDesc.pointer );
		Ic3OpenGLHandleLastError();
	}

	void GLTextureObject::UpdateUpload3D( const GLTextureSubDataUploadDesc & pGLUploadDesc, GLenum pActiveBindTarget )
	{
		Ic3DebugAssert( pGLUploadDesc.openglDimensionClass == GL_TEXTURE_3D );
		Ic3DebugAssert( ( pActiveBindTarget == 0 ) || ( pActiveBindTarget == GL_TEXTURE_3D ) );

		auto textureBindTarget = CheckActiveBindTarget( pActiveBindTarget );

		glTexSubImage3D(
			textureBindTarget,
			pGLUploadDesc.textureSubRegion.uSubReg3D.offset.mipLevel,
			pGLUploadDesc.textureSubRegion.uSubReg3D.offset.x,
			pGLUploadDesc.textureSubRegion.uSubReg3D.offset.y,
			pGLUploadDesc.textureSubRegion.uSubReg3D.offset.z,
			pGLUploadDesc.textureSubRegion.uSubReg3D.size.width,
			pGLUploadDesc.textureSubRegion.uSubReg3D.size.height,
			pGLUploadDesc.textureSubRegion.uSubReg3D.size.depth,
			pGLUploadDesc.openglInputDataDesc.openglPixelDataLayout,
			pGLUploadDesc.openglInputDataDesc.openglPixelDataType,
			pGLUploadDesc.openglInputDataDesc.pointer );
		Ic3OpenGLHandleLastError();
	}

	void GLTextureObject::UpdateUploadCubeMap( const GLTextureSubDataUploadDesc & pGLUploadDesc, GLenum pActiveBindTarget )
	{
		Ic3DebugAssert( pGLUploadDesc.openglDimensionClass == GL_TEXTURE_CUBE_MAP );
		Ic3DebugAssert( ( pActiveBindTarget == 0 ) || ( pActiveBindTarget == GL_TEXTURE_CUBE_MAP ) );

		CheckActiveBindTarget( pActiveBindTarget );

		glTexSubImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + pGLUploadDesc.textureSubRegion.uSubRegCubeMap.offset.faceIndex,
			pGLUploadDesc.textureSubRegion.uSubRegCubeMap.offset.mipLevel,
			pGLUploadDesc.textureSubRegion.uSubRegCubeMap.offset.x,
			pGLUploadDesc.textureSubRegion.uSubRegCubeMap.offset.y,
			pGLUploadDesc.textureSubRegion.uSubRegCubeMap.size.width,
			pGLUploadDesc.textureSubRegion.uSubRegCubeMap.size.height,
			pGLUploadDesc.openglInputDataDesc.openglPixelDataLayout,
			pGLUploadDesc.openglInputDataDesc.openglPixelDataType,
			pGLUploadDesc.openglInputDataDesc.pointer );
		Ic3OpenGLHandleLastError();
	}

	void GLTextureObject::SetAutoMipGeneration( bool pEnable )
	{
		_autoMipGenerationStatus = pEnable;
	}

	bool GLTextureObject::InitializeCore( const GLTextureCreateInfo & pGLCreateInfo )
	{
		switch( pGLCreateInfo.bindTarget )
		{
			case GL_TEXTURE_2D:
				return InitializeCore2D( pGLCreateInfo );
			case GL_TEXTURE_2D_ARRAY:
				return InitializeCore2DArray( pGLCreateInfo );
			case GL_TEXTURE_2D_MULTISAMPLE:
				return InitializeCore2DMS( pGLCreateInfo );
			case GL_TEXTURE_3D:
				return InitializeCore3D( pGLCreateInfo );
			case GL_TEXTURE_CUBE_MAP:
				return InitializeCoreCubeMap( pGLCreateInfo );
		}

		Ic3DebugInterrupt();

		return false;
	}

	bool GLTextureObject::InitializeCore2D( const GLTextureCreateInfo & pGLCreateInfo )
	{
		glTexStorage2D(
			pGLCreateInfo.bindTarget,
			pGLCreateInfo.dimensions.mipLevelsNum,
			pGLCreateInfo.internalFormat,
			pGLCreateInfo.dimensions.width,
			pGLCreateInfo.dimensions.height );
		Ic3OpenGLHandleLastError();

		InitializeData2D( pGLCreateInfo );

		return true;
	}

	bool GLTextureObject::InitializeCore2DArray( const GLTextureCreateInfo & pGLCreateInfo )
	{
		glTexStorage3D(
			pGLCreateInfo.bindTarget,
			pGLCreateInfo.dimensions.mipLevelsNum,
			pGLCreateInfo.internalFormat,
			pGLCreateInfo.dimensions.width,
			pGLCreateInfo.dimensions.height,
			pGLCreateInfo.dimensions.arraySize );
		Ic3OpenGLHandleLastError();

		InitializeData2DArray( pGLCreateInfo );

		return true;
	}

	bool GLTextureObject::InitializeCore2DMS( const GLTextureCreateInfo & pGLCreateInfo )
	{
		glTexStorage2DMultisample(
			pGLCreateInfo.bindTarget,
			pGLCreateInfo.msaaLevel,
			pGLCreateInfo.internalFormat,
			pGLCreateInfo.dimensions.width,
			pGLCreateInfo.dimensions.height,
			GL_FALSE );
		Ic3OpenGLHandleLastError();

		return true;
	}

	bool GLTextureObject::InitializeCore3D( const GLTextureCreateInfo & pGLCreateInfo )
	{
		glTexStorage3D(
			pGLCreateInfo.bindTarget,
            pGLCreateInfo.dimensions.mipLevelsNum,
            pGLCreateInfo.internalFormat,
            pGLCreateInfo.dimensions.width,
            pGLCreateInfo.dimensions.height,
            pGLCreateInfo.dimensions.depth );
		Ic3OpenGLHandleLastError();

		InitializeData3D( pGLCreateInfo );

		return true;
	}

	bool GLTextureObject::InitializeCoreCubeMap( const GLTextureCreateInfo & pGLCreateInfo )
	{
		Ic3DebugAssert( pGLCreateInfo.dimensions.arraySize == 6 );

		glTexStorage2D(
			GL_TEXTURE_CUBE_MAP,
			pGLCreateInfo.dimensions.mipLevelsNum,
			pGLCreateInfo.internalFormat,
			pGLCreateInfo.dimensions.width,
			pGLCreateInfo.dimensions.height );
		Ic3OpenGLHandleLastError();

		InitializeDataCubeMap( pGLCreateInfo );

		return true;
	}

	bool GLTextureObject::InitializeCompat( const GLTextureCreateInfo & pGLCreateInfo )
	{
		switch( pGLCreateInfo.bindTarget )
		{
			case GL_TEXTURE_2D:
				return InitializeCompat2D( pGLCreateInfo );
			case GL_TEXTURE_2D_ARRAY:
				return InitializeCompat2DArray( pGLCreateInfo );
			case GL_TEXTURE_2D_MULTISAMPLE:
				return InitializeCompat2DMS( pGLCreateInfo );
			case GL_TEXTURE_3D:
				return InitializeCompat3D( pGLCreateInfo );
			case GL_TEXTURE_CUBE_MAP:
				return InitializeCompatCubeMap( pGLCreateInfo );
		}

		Ic3DebugInterrupt();

		return false;
	}

	bool GLTextureObject::InitializeCompat2D( const GLTextureCreateInfo & pGLCreateInfo )
	{
		glTexImage2D(
			pGLCreateInfo.bindTarget,
			0,
			pGLCreateInfo.internalFormat,
			pGLCreateInfo.dimensions.width,
			pGLCreateInfo.dimensions.height,
			0,
			pGLCreateInfo.openglInitDataDesc.openglPixelDataLayout,
			pGLCreateInfo.openglInitDataDesc.openglPixelDataType,
			nullptr );
		Ic3OpenGLHandleLastError();

		InitializeData2D( pGLCreateInfo, true );

		return true;
	}

	bool GLTextureObject::InitializeCompat2DArray( const GLTextureCreateInfo & pGLCreateInfo )
	{
		glTexImage3D(
			pGLCreateInfo.bindTarget,
			0,
			pGLCreateInfo.internalFormat,
			pGLCreateInfo.dimensions.width,
			pGLCreateInfo.dimensions.height,
			pGLCreateInfo.dimensions.arraySize,
			0,
			pGLCreateInfo.openglInitDataDesc.openglPixelDataLayout,
			pGLCreateInfo.openglInitDataDesc.openglPixelDataType,
			nullptr );
		Ic3OpenGLHandleLastError();

		InitializeData2D( pGLCreateInfo, true );

		return true;
	}

	bool GLTextureObject::InitializeCompat2DMS( const GLTextureCreateInfo & pGLCreateInfo )
	{
		glTexImage2DMultisample(
			pGLCreateInfo.bindTarget,
			pGLCreateInfo.msaaLevel,
			pGLCreateInfo.internalFormat,
			pGLCreateInfo.dimensions.width,
			pGLCreateInfo.dimensions.height,
			GL_FALSE );
		Ic3OpenGLHandleLastError();

		return true;
	}

	bool GLTextureObject::InitializeCompat3D( const GLTextureCreateInfo & pGLCreateInfo )
	{
		glTexImage3D(
			pGLCreateInfo.bindTarget,
			0,
			pGLCreateInfo.internalFormat,
			pGLCreateInfo.dimensions.width,
			pGLCreateInfo.dimensions.height,
			pGLCreateInfo.dimensions.depth,
			0,
			pGLCreateInfo.openglInitDataDesc.openglPixelDataLayout,
			pGLCreateInfo.openglInitDataDesc.openglPixelDataType,
			nullptr );
		Ic3OpenGLHandleLastError();

		InitializeData2D( pGLCreateInfo, true );

		return true;
	}

	bool GLTextureObject::InitializeCompatCubeMap( const GLTextureCreateInfo & pGLCreateInfo )
	{
		for( uint32 cubeMapFaceIndex = 0; cubeMapFaceIndex < pGLCreateInfo.dimensions.arraySize; ++cubeMapFaceIndex )
		{
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + cubeMapFaceIndex,
				0,
				pGLCreateInfo.internalFormat,
				pGLCreateInfo.dimensions.width,
				pGLCreateInfo.dimensions.height,
				0,
				pGLCreateInfo.openglInitDataDesc.openglPixelDataLayout,
				pGLCreateInfo.openglInitDataDesc.openglPixelDataType,
				nullptr );
			Ic3OpenGLHandleLastError();
		}

		InitializeDataCubeMap( pGLCreateInfo, true );

		return true;
	}


	void GLTextureObject::InitializeData2D( const GLTextureCreateInfo & pGLCreateInfo, bool pAllocateMipMapStorage )
	{
		if( pGLCreateInfo.openglInitDataDesc )
		{
			const auto & textureInitData = *pGLCreateInfo.openglInitDataDesc.subTextureInitDataPtr;

			GLTextureSubDataUploadDesc subDataUploadDesc;
			subDataUploadDesc.openglDimensionClass = GL_TEXTURE_2D;
			subDataUploadDesc.textureSubRegion.uSubReg2D.offset.x = 0;
			subDataUploadDesc.textureSubRegion.uSubReg2D.offset.y = 0;
			subDataUploadDesc.openglInputDataDesc.openglPixelDataLayout = pGLCreateInfo.openglInitDataDesc.openglPixelDataLayout;
			subDataUploadDesc.openglInputDataDesc.openglPixelDataType = pGLCreateInfo.openglInitDataDesc.openglPixelDataType;

			for( uint32 mipLevelIndex = 0; mipLevelIndex < pGLCreateInfo.GetInitMipLevelsNum(); ++mipLevelIndex )
			{
				const auto & mipLevelInitData = textureInitData.mipLevelInitDataArray[mipLevelIndex];

				if( pAllocateMipMapStorage )
				{
					glTexImage2D(
						pGLCreateInfo.bindTarget,
						mipLevelInitData.mipLevelIndex,
						pGLCreateInfo.internalFormat,
						mipLevelInitData.mipWidth,
						mipLevelInitData.mipHeight,
						0,
						pGLCreateInfo.openglInitDataDesc.openglPixelDataLayout,
						pGLCreateInfo.openglInitDataDesc.openglPixelDataType,
						nullptr );
					Ic3OpenGLHandleLastError();
				}

				subDataUploadDesc.textureSubRegion.uSubReg2D.offset.mipLevel = mipLevelInitData.mipLevelIndex;
				subDataUploadDesc.textureSubRegion.uSubReg2D.size.width = mipLevelInitData.mipWidth;
				subDataUploadDesc.textureSubRegion.uSubReg2D.size.height = mipLevelInitData.mipHeight;
				subDataUploadDesc.openglInputDataDesc.pointer = mipLevelInitData.pointer;
				subDataUploadDesc.openglInputDataDesc.size = mipLevelInitData.size;

				UpdateUpload2D( subDataUploadDesc, pGLCreateInfo.bindTarget );
			}
		}
	}

	void GLTextureObject::InitializeData2DArray( const GLTextureCreateInfo & pGLCreateInfo, bool pAllocateMipMapStorage )
	{
		if( pGLCreateInfo.openglInitDataDesc )
		{
			GLTextureSubDataUploadDesc subDataUploadDesc;
			subDataUploadDesc.openglDimensionClass = GL_TEXTURE_2D_ARRAY;
			subDataUploadDesc.textureSubRegion.uSubReg2DArray.offset.x = 0;
			subDataUploadDesc.textureSubRegion.uSubReg2DArray.offset.y = 0;
			subDataUploadDesc.openglInputDataDesc.openglPixelDataLayout = pGLCreateInfo.openglInitDataDesc.openglPixelDataLayout;
			subDataUploadDesc.openglInputDataDesc.openglPixelDataType = pGLCreateInfo.openglInitDataDesc.openglPixelDataType;

			for( uint32 arraySubTextureIndex = 0; arraySubTextureIndex < pGLCreateInfo.dimensions.arraySize; ++arraySubTextureIndex )
			{
				const auto & textureInitData = pGLCreateInfo.openglInitDataDesc.subTextureInitDataPtr[arraySubTextureIndex];
				subDataUploadDesc.textureSubRegion.uSubReg2DArray.offset.arrayIndex = textureInitData.subTextureIndex;

				for( uint32 mipLevelIndex = 0; mipLevelIndex < pGLCreateInfo.GetInitMipLevelsNum(); ++mipLevelIndex )
				{
					const auto & mipLevelInitData = textureInitData.mipLevelInitDataArray[mipLevelIndex];

					if( pAllocateMipMapStorage )
					{
						glTexImage3D(
							pGLCreateInfo.bindTarget,
							mipLevelInitData.mipLevelIndex,
							pGLCreateInfo.internalFormat,
							mipLevelInitData.mipWidth,
							mipLevelInitData.mipHeight,
							pGLCreateInfo.dimensions.arraySize,
							0,
							pGLCreateInfo.openglInitDataDesc.openglPixelDataLayout,
							pGLCreateInfo.openglInitDataDesc.openglPixelDataType,
							nullptr );
						Ic3OpenGLHandleLastError();
					}

					subDataUploadDesc.textureSubRegion.uSubReg2DArray.offset.mipLevel = mipLevelInitData.mipLevelIndex;
					subDataUploadDesc.textureSubRegion.uSubReg2DArray.size.width = mipLevelInitData.mipWidth;
					subDataUploadDesc.textureSubRegion.uSubReg2DArray.size.height = mipLevelInitData.mipHeight;
					subDataUploadDesc.openglInputDataDesc.pointer = mipLevelInitData.pointer;
					subDataUploadDesc.openglInputDataDesc.size = mipLevelInitData.size;

					UpdateUpload2DArray( subDataUploadDesc, pGLCreateInfo.bindTarget );
				}
			}
		}
	}

	void GLTextureObject::InitializeData3D( const GLTextureCreateInfo & pGLCreateInfo, bool pAllocateMipMapStorage )
	{
		if( pGLCreateInfo.openglInitDataDesc )
		{
			const auto & textureInitData = pGLCreateInfo.openglInitDataDesc.subTextureInitDataPtr[0];

			GLTextureSubDataUploadDesc subDataUploadDesc;
			subDataUploadDesc.openglDimensionClass = GL_TEXTURE_3D;
			subDataUploadDesc.textureSubRegion.uSubReg3D.offset.x = 0;
			subDataUploadDesc.textureSubRegion.uSubReg3D.offset.y = 0;
			subDataUploadDesc.textureSubRegion.uSubReg3D.offset.z = 0;
			subDataUploadDesc.openglInputDataDesc.openglPixelDataLayout = pGLCreateInfo.openglInitDataDesc.openglPixelDataLayout;
			subDataUploadDesc.openglInputDataDesc.openglPixelDataType = pGLCreateInfo.openglInitDataDesc.openglPixelDataType;

			for( uint32 mipLevelIndex = 0; mipLevelIndex < pGLCreateInfo.GetInitMipLevelsNum(); ++mipLevelIndex )
			{
				const auto & mipLevelInitData = textureInitData.mipLevelInitDataArray[mipLevelIndex];

				if( pAllocateMipMapStorage )
				{
					glTexImage3D(
						pGLCreateInfo.bindTarget,
						mipLevelInitData.mipLevelIndex,
						pGLCreateInfo.internalFormat,
						mipLevelInitData.mipWidth,
						mipLevelInitData.mipHeight,
						pGLCreateInfo.dimensions.depth,
						0,
						pGLCreateInfo.openglInitDataDesc.openglPixelDataLayout,
						pGLCreateInfo.openglInitDataDesc.openglPixelDataType,
						nullptr );
					Ic3OpenGLHandleLastError();
				}

				subDataUploadDesc.textureSubRegion.uSubReg3D.offset.mipLevel = mipLevelInitData.mipLevelIndex;
				subDataUploadDesc.textureSubRegion.uSubReg3D.size.width = mipLevelInitData.mipWidth;
				subDataUploadDesc.textureSubRegion.uSubReg3D.size.height = mipLevelInitData.mipHeight;
				subDataUploadDesc.textureSubRegion.uSubReg3D.size.depth = mipLevelInitData.mipDepth;
				subDataUploadDesc.openglInputDataDesc.pointer = mipLevelInitData.pointer;
				subDataUploadDesc.openglInputDataDesc.size = mipLevelInitData.size;

				UpdateUpload3D( subDataUploadDesc, pGLCreateInfo.bindTarget );
			}
		}
	}

	void GLTextureObject::InitializeDataCubeMap( const GLTextureCreateInfo & pGLCreateInfo, bool pAllocateMipMapStorage )
	{
		if( pGLCreateInfo.openglInitDataDesc )
		{
			GLTextureSubDataUploadDesc subDataUploadDesc;
			subDataUploadDesc.openglDimensionClass = GL_TEXTURE_CUBE_MAP;
			subDataUploadDesc.textureSubRegion.uSubRegCubeMap.offset.x = 0;
			subDataUploadDesc.textureSubRegion.uSubRegCubeMap.offset.y = 0;
			subDataUploadDesc.openglInputDataDesc.openglPixelDataLayout = pGLCreateInfo.openglInitDataDesc.openglPixelDataLayout;
			subDataUploadDesc.openglInputDataDesc.openglPixelDataType = pGLCreateInfo.openglInitDataDesc.openglPixelDataType;

			for( uint32 cubeMapFaceIndex = 0; cubeMapFaceIndex < pGLCreateInfo.dimensions.arraySize; ++cubeMapFaceIndex )
			{
				const auto & cubeMapInitData = pGLCreateInfo.openglInitDataDesc.subTextureInitDataPtr[cubeMapFaceIndex];
				subDataUploadDesc.textureSubRegion.uSubRegCubeMap.offset.faceIndex = cubeMapInitData.subTextureIndex;

				for( uint32 mipLevelIndex = 0; mipLevelIndex < pGLCreateInfo.GetInitMipLevelsNum(); ++mipLevelIndex )
				{
					const auto & mipLevelInitData = cubeMapInitData.mipLevelInitDataArray[mipLevelIndex];

					if( pAllocateMipMapStorage )
					{
						glTexImage2D(
							GL_TEXTURE_CUBE_MAP_POSITIVE_X + cubeMapFaceIndex,
							mipLevelInitData.mipLevelIndex,
							pGLCreateInfo.internalFormat,
							mipLevelInitData.mipWidth,
							mipLevelInitData.mipHeight,
							0,
							pGLCreateInfo.openglInitDataDesc.openglPixelDataLayout,
							pGLCreateInfo.openglInitDataDesc.openglPixelDataType,
							nullptr );
						Ic3OpenGLHandleLastError();
					}


					subDataUploadDesc.textureSubRegion.uSubRegCubeMap.offset.mipLevel = mipLevelInitData.mipLevelIndex;
					subDataUploadDesc.textureSubRegion.uSubRegCubeMap.size.width = mipLevelInitData.mipWidth;
					subDataUploadDesc.textureSubRegion.uSubRegCubeMap.size.height = mipLevelInitData.mipHeight;
					subDataUploadDesc.openglInputDataDesc.pointer = mipLevelInitData.pointer;
					subDataUploadDesc.openglInputDataDesc.size = mipLevelInitData.size;

					UpdateUploadCubeMap( subDataUploadDesc, pGLCreateInfo.bindTarget );
				}
			}
		}
	}

	GLenum GLTextureObject::CheckActiveBindTarget( GLenum pBindTarget ) const
	{
		if( pBindTarget == 0 )
		{
			pBindTarget = mGLTextureBindTarget;

			glBindTexture( mGLTextureBindTarget, mGLHandle );
			Ic3OpenGLHandleLastError();
		}

		return pBindTarget;
	}

	GLuint GLTextureObject::ComputeInputPixelDataAlignment( GLenum pPixelDataLayout, GLenum pPixelDataType )
	{
		return 1;
	}

} // namespace Ic3::Graphics::GCI
