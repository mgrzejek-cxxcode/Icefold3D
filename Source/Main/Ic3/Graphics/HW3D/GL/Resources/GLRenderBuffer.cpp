
#include "GLRenderBuffer.h"
#include <Ic3/Graphics/HW3D/GL/GLApiTranslationLayer.h>
#include <Ic3/Graphics/HW3D/GL/GLGPUDevice.h>

namespace Ic3::Graphics::GCI
{

	GLInternalRenderBuffer::GLInternalRenderBuffer(
			GLGPUDevice & pGPUDevice,
			GLRenderbufferObjectHandle pGLRenderbufferObject )
	: GPUDeviceChildObject( pGPUDevice )
	, mGLRenderbufferObject( std::move( pGLRenderbufferObject ) )
	{}

	GLInternalRenderBuffer::GLInternalRenderBuffer(
			GLGPUDevice & pGPUDevice,
			GLTextureObjectHandle pGLTextureObject )
	: GPUDeviceChildObject( pGPUDevice )
	, mGLTextureObject( std::move( pGLTextureObject ) )
	{}

	TGfxHandle<GLInternalRenderBuffer> GLInternalRenderBuffer::CreateInstance(
			GLGPUDevice & pGPUDevice,
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

		auto renderBuffer = CreateGfxObject<GLInternalRenderBuffer>( pGPUDevice, std::move( openglRenderbuffer ) );

		return renderBuffer;
	}

} // namespace Ic3::Graphics::GCI
