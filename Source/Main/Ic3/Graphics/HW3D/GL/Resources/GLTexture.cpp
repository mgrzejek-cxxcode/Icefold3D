
#include "GLTexture.h"
#include "GLRenderBuffer.h"
#include <Ic3/Graphics/HW3D/GL/GLApiTranslationLayer.h>
#include <Ic3/Graphics/HW3D/GL/GLGpuDevice.h>
#include <Ic3/Graphics/GCI/Resources/RenderTargetTexture.h>

namespace Ic3::Graphics::GCI
{

	GLTexture::GLTexture(
			GLGpuDevice & pGpuDevice,
			const ResourceMemoryInfo & pResourceMemory,
			const TextureProperties & pTextureProperties,
			const TextureLayout & pTextureLayout,
			GLTextureObjectHandle pGLTextureObject )
	: Texture( pGpuDevice, pResourceMemory, pTextureProperties, pTextureLayout )
	, mGLTextureObject( std::move( pGLTextureObject ) )
	{ }

	GLTexture::~GLTexture() = default;

	GLTextureHandle GLTexture::CreateDefault( GLGpuDevice & pGpuDevice, const TextureCreateInfo & pCreateInfo )
	{
		GLTextureCreateInfo openglCreateInfo;
		openglCreateInfo.bindTarget = ATL::TranslateGLTextureBindTarget( pCreateInfo.texClass );
		openglCreateInfo.dimensions = RCU::GetValidTextureDimensions( pCreateInfo.texClass, pCreateInfo.dimensions );
		openglCreateInfo.msaaLevel = pCreateInfo.msaaLevel;
		openglCreateInfo.internalFormat = ATL::TranslateGLTextureInternalFormat( pCreateInfo.internalFormat );
		openglCreateInfo.openglInitDataDesc.subTextureInitDataPtr = pCreateInfo.mInitDataDesc.subTextureInitDataBasePtr;
		openglCreateInfo.openglInitDataDesc.textureInitFlags = pCreateInfo.mInitDataDesc.textureInitFlags;
		auto textureInitDataBaseType = CxDef::GetTextureFormatBaseDataType( pCreateInfo.internalFormat );
		openglCreateInfo.openglInitDataDesc.openglPixelDataLayout = ATL::TranslateGLTexturePixelDataLayout( pCreateInfo.internalFormat );
		openglCreateInfo.openglInitDataDesc.openglPixelDataType = ATL::TranslateGLBaseDataType( textureInitDataBaseType );

		GLTextureObjectHandle openglTextureObject = nullptr;
		if( pGpuDevice.IsCompatibilityDevice() )
		{
			openglTextureObject = GLTextureObject::CreateCompat( openglCreateInfo );
			ic3DebugAssert( openglTextureObject );
		}
		else
		{
			openglTextureObject = GLTextureObject::CreateCore( openglCreateInfo );
			ic3DebugAssert( openglTextureObject );
		}

		const auto glcTextureInternalFormat = openglTextureObject->QueryInternalFormat();
		const auto glcTextureByteSize = openglTextureObject->QuerySize();

		ResourceMemoryInfo textureMemoryInfo;
		textureMemoryInfo.sourceHeapRegion.offset = 0;
		textureMemoryInfo.sourceHeapRegion.size = glcTextureByteSize;
		textureMemoryInfo.baseAlignment = pCreateInfo.memoryBaseAlignment;
		textureMemoryInfo.memoryFlags = pCreateInfo.memoryFlags;

		TextureProperties textureProperties;
		textureProperties.resourceFlags = pCreateInfo.resourceFlags;

		TextureLayout textureLayout;
		textureLayout.texClass = pCreateInfo.texClass;
		textureLayout.dimensions = pCreateInfo.dimensions;
		textureLayout.internalFormat = pCreateInfo.internalFormat;
		textureLayout.msaaLevel = pCreateInfo.msaaLevel;
		textureLayout.storageSize = cppx::numeric_cast<uint32>( glcTextureByteSize );
		textureLayout.bitsPerPixel = ATL::QueryGLTextureInternalFormatBPP( glcTextureInternalFormat );

		auto openglTexture = CreateDynamicObject<GLTexture>(
			pGpuDevice,
			textureMemoryInfo,
			textureProperties,
			textureLayout,
			std::move( openglTextureObject ) );

		return openglTexture;
	}

	GLTextureHandle GLTexture::CreateForRenderTarget( GLGpuDevice & pGpuDevice, const RenderTargetTextureCreateInfo & pCreateInfo )
	{
		TextureCreateInfo textureCreateInfo;
		textureCreateInfo.texClass = ETextureClass::T2D;
		textureCreateInfo.dimensions.width = pCreateInfo.rtTextureLayout.imageRect.width;
		textureCreateInfo.dimensions.height = pCreateInfo.rtTextureLayout.imageRect.height;
		textureCreateInfo.memoryFlags = eGpuMemoryAccessFlagGpuReadBit;
		textureCreateInfo.resourceFlags = ( pCreateInfo.bindFlags & eGpuResourceUsageMaskAll );
		textureCreateInfo.internalFormat = pCreateInfo.rtTextureLayout.internalFormat;
		textureCreateInfo.dimensions.arraySize = 1;
		textureCreateInfo.dimensions.depth = 1;
		textureCreateInfo.dimensions.mipLevelsNum = 1;

		auto glcTexture = GLTexture::CreateDefault( pGpuDevice, textureCreateInfo );

		return glcTexture;
	}

	RenderTargetTextureHandle GLTexture::CreateRenderTargetTextureView( GLGpuDevice & pGpuDevice, const RenderTargetTextureCreateInfo & pCreateInfo )
	{
		if( pCreateInfo.targetTexture )
		{
			return CreateDefaultRenderTargetTextureView( pGpuDevice, pCreateInfo );
		}

		const auto renderBufferIncompatibleBindFlags =
				eGpuResourceUsageFlagShaderInputBit |
				eGpuResourceUsageFlagRenderTargetColorBit |
				eGpuResourceUsageFlagTransferTargetBit;

		const auto rtTextureType = RCU::QueryRenderTargetTextureType( pCreateInfo.targetTexture->mTextureLayout.internalFormat );

		if( pCreateInfo.bindFlags.is_set_any_of( renderBufferIncompatibleBindFlags ) )
		{
			auto glcTexture = CreateForRenderTarget( pGpuDevice, pCreateInfo );
			if( !glcTexture )
			{
				return nullptr;
			}

			auto textureRTView = CreateGfxObject<RenderTargetTexture>(
					pGpuDevice,
					rtTextureType,
					pCreateInfo.rtTextureLayout,
					TextureReference{ glcTexture } );

			return textureRTView;
		}
		else
		{
			auto glcRenderBuffer = GLInternalRenderBuffer::CreateInstance( pGpuDevice, pCreateInfo );
			if( !glcRenderBuffer )
			{
				return nullptr;
			}

			auto renderBufferRTView = CreateGfxObject<RenderTargetTexture>(
					pGpuDevice,
					rtTextureType,
					pCreateInfo.rtTextureLayout,
					glcRenderBuffer,
					pCreateInfo.bindFlags );

			return renderBufferRTView;
		}
	}

} // namespace Ic3::Graphics::GCI
