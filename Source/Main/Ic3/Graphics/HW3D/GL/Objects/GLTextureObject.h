
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLC_TEXTURE_OBJECT_H__
#define __IC3_GRAPHICS_HW3D_GLC_TEXTURE_OBJECT_H__

#include "../GLPrerequisites.h"
#include <Ic3/Graphics/GCI/Resources/TextureCommon.h>

namespace Ic3::Graphics::GCI
{

	Ic3GLDeclareOpenGLObjectHandle( GLTextureObject );

	struct GLTextureInitDataDesc
	{
		const TextureSubTextureInitDataDesc * subTextureInitDataPtr = nullptr;
		cppx::bitmask<ETextureInitFlags> textureInitFlags = 0;
		GLenum openglPixelDataLayout = 0;
		GLenum openglPixelDataType = 0;

		explicit operator bool() const
		{
			return subTextureInitDataPtr != nullptr;
		}
	};

	struct GLTextureInputDataDesc : public ResourceInputDataDesc
	{
		GLenum openglPixelDataLayout = 0;
		GLenum openglPixelDataType = 0;
	};

	struct GLTextureCreateInfo
	{
		TextureDimensions dimensions;
		GLenum bindTarget = 0;
		GLenum internalFormat = 0;
		GLuint msaaLevel = 0;
		GLTextureInitDataDesc openglInitDataDesc;

		uint32 GetInitMipLevelsNum() const
		{
			return openglInitDataDesc.textureInitFlags.is_set( eTextureInitFlagGenerateMipmapsBit ) ? 1u : dimensions.mipLevelsNum;
		}
	};

	struct GLTextureSubDataUploadDesc
	{
		GLenum openglDimensionClass;
		TextureSubRegion textureSubRegion;
		GLTextureInputDataDesc openglInputDataDesc;
	};

	class GLTextureObject : public GLObject
	{
	public:
		TextureDimensions const dimensions;
		GLenum const mGLTextureBindTarget = CX_GL_BIND_TARGET_UNKNOWN;
		GLenum const mGLInternalFormat;
		GLuint const mMultisampleLevel;

		GLTextureObject( GLuint pHandle, const GLTextureCreateInfo & pGLCreateInfo );
		virtual ~GLTextureObject();

		bool IsAutoMipGenerationEnabled() const;
		GLenum QueryInternalFormat( GLenum pActiveBindTarget = 0 ) const;
		memory_size_t QuerySize( GLenum pActiveBindTarget = 0 ) const;

		virtual bool Release();
		virtual bool ValidateHandle() const;

		void UpdateCopy2D( GLTextureObject & pSrcTexture, const TextureSubDataCopyDesc & pCopyDesc, GLenum pActiveBindTarget = 0 );
		void UpdateCopy2DArray( GLTextureObject & pSrcTexture, const TextureSubDataCopyDesc & pCopyDesc, GLenum pActiveBindTarget = 0 );
		void UpdateCopy3D( GLTextureObject & pSrcTexture, const TextureSubDataCopyDesc & pCopyDesc, GLenum pActiveBindTarget = 0 );
		void UpdateCopyCubeMap( GLTextureObject & pSrcTexture, const TextureSubDataCopyDesc & pCopyDesc, GLenum pActiveBindTarget = 0 );

		void UpdateUpload2D( const GLTextureSubDataUploadDesc & pGLUploadDesc, GLenum pActiveBindTarget = 0 );
		void UpdateUpload2DArray( const GLTextureSubDataUploadDesc & pGLUploadDesc, GLenum pActiveBindTarget = 0 );
		void UpdateUpload3D( const GLTextureSubDataUploadDesc & pGLUploadDesc, GLenum pActiveBindTarget = 0 );
		void UpdateUploadCubeMap( const GLTextureSubDataUploadDesc & pGLUploadDesc, GLenum pActiveBindTarget = 0 );

		static GLTextureObjectHandle CreateCore( const GLTextureCreateInfo & pGLCreateInfo );
		static GLTextureObjectHandle CreateCompat( const GLTextureCreateInfo & pGLCreateInfo );

	private:
		void SetAutoMipGeneration( bool pEnable );

		bool InitializeCore( const GLTextureCreateInfo & pGLCreateInfo );
		bool InitializeCore2D( const GLTextureCreateInfo & pGLCreateInfo );
		bool InitializeCore2DArray( const GLTextureCreateInfo & pGLCreateInfo );
		bool InitializeCore2DMS( const GLTextureCreateInfo & pGLCreateInfo );
		bool InitializeCore3D( const GLTextureCreateInfo & pGLCreateInfo );
		bool InitializeCoreCubeMap( const GLTextureCreateInfo & pGLCreateInfo );

		bool InitializeCompat( const GLTextureCreateInfo & pGLCreateInfo );
		bool InitializeCompat2D( const GLTextureCreateInfo & pGLCreateInfo );
		bool InitializeCompat2DArray( const GLTextureCreateInfo & pGLCreateInfo );
		bool InitializeCompat2DMS( const GLTextureCreateInfo & pGLCreateInfo );
		bool InitializeCompat3D( const GLTextureCreateInfo & pGLCreateInfo );
		bool InitializeCompatCubeMap( const GLTextureCreateInfo & pGLCreateInfo );

		void InitializeData2D( const GLTextureCreateInfo & pGLCreateInfo, bool pAllocateMipMapStorage = false );
		void InitializeData2DArray( const GLTextureCreateInfo & pGLCreateInfo, bool pAllocateMipMapStorage = false );
		void InitializeData3D( const GLTextureCreateInfo & pGLCreateInfo, bool pAllocateMipMapStorage = false );
		void InitializeDataCubeMap( const GLTextureCreateInfo & pGLCreateInfo, bool pAllocateMipMapStorage = false );

		GLenum CheckActiveBindTarget( GLenum pBindTarget ) const;

		static GLuint ComputeInputPixelDataAlignment( GLenum pPixelDataLayout, GLenum pPixelDataType );

	private:
		bool _autoMipGenerationStatus = false;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLC_TEXTURE_OBJECT_H__
