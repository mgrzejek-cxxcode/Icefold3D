
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_RENDER_TARGET_BUFFER_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_RENDER_TARGET_BUFFER_H__

#include <Ic3/Graphics/GCI/Resources/RenderTargetTexture.h>
#include <Ic3/Graphics/HW3D/GL/Objects/GLRenderbufferObject.h>
#include <Ic3/Graphics/HW3D/GL/Objects/GLTextureObject.h>

namespace Ic3::Graphics::GCI
{

	class GLInternalRenderBuffer : public GpuDeviceChildObject
	{
	public:
		GLRenderbufferObjectHandle const mGLRenderbufferObject;
		GLTextureObjectHandle const mGLTextureObject;

	public:
		GLInternalRenderBuffer(
				GLGpuDevice & pGpuDevice,
				GLRenderbufferObjectHandle pGLRenderbufferObject );

		GLInternalRenderBuffer(
				GLGpuDevice & pGpuDevice,
				GLTextureObjectHandle pGLTextureObject );

		static TGfxHandle<GLInternalRenderBuffer> CreateInstance(
				GLGpuDevice & pGpuDevice,
				const RenderTargetTextureCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_RENDER_TARGET_BUFFER_H__
