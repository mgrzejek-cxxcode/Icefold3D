
#include "GLRenderBuffer.h"
#include <Ic3/Graphics/HW3D/GL/GLApiTranslationLayer.h>
#include <Ic3/Graphics/HW3D/GL/GLGpuDevice.h>

namespace Ic3::Graphics::GCI
{

	GLInternalRenderBuffer::GLInternalRenderBuffer(
			GLGpuDevice & pGpuDevice,
			GLRenderbufferObjectHandle pGLRenderbufferObject )
	: GpuDeviceChildObject( pGpuDevice )
	, mGLRenderbufferObject( std::move( pGLRenderbufferObject ) )
	{}

	GLInternalRenderBuffer::GLInternalRenderBuffer(
			GLGpuDevice & pGpuDevice,
			GLTextureObjectHandle pGLTextureObject )
	: GpuDeviceChildObject( pGpuDevice )
	, mGLTextureObject( std::move( pGLTextureObject ) )
	{}

	TGfxHandle<GLInternalRenderBuffer> GLInternalRenderBuffer::CreateInstance(
			GLGpuDevice & pGpuDevice,
			const RenderTargetTextureCreateInfo & pCreateInfo )
	{
		GLRenderbufferCreateInfo openglRenderbufferCreateInfo;
		openglRenderbufferCreateInfo.dimensions.x = pCreateInfo.rtTextureLayout.imageRect.width;
		openglRenderbufferCreateInfo.dimensions.y = pCreateInfo.rtTextureLayout.imageRect.height;
		openglRenderbufferCreateInfo.msaaLevel = pCreateInfo.rtTextureLayout.msaaLevel;
		openglRenderbufferCreateInfo.internalFormat =
				ATL::TranslateGLTextureInternalFormat( pCreateInfo.rtTextureLayout.internalFormat );

		auto openglRenderbuffer = GLRenderbufferObject::Create( openglRenderbufferCreateInfo );
		if( !openglRenderbuffer )
		{
			return nullptr;
		}

		auto renderBuffer = CreateGfxObject<GLInternalRenderBuffer>( pGpuDevice, std::move( openglRenderbuffer ) );

		return renderBuffer;
	}

} // namespace Ic3::Graphics::GCI
