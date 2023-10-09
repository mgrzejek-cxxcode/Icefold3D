
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_RENDER_TARGET_BUFFER_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_RENDER_TARGET_BUFFER_H__

#include <Ic3/Graphics/GCI/Resources/RenderTargetTexture.h>
#include <Ic3/Graphics/HW3D/GL/Objects/GLRenderbufferObject.h>
#include <Ic3/Graphics/HW3D/GL/Objects/GLTextureObject.h>

namespace Ic3::Graphics::GCI
{

	class GLInternalRenderBuffer : public GPUDeviceChildObject
	{
	public:
		GLRenderbufferObjectHandle const mGLRenderbufferObject;
		GLTextureObjectHandle const mGLTextureObject;

	public:
		GLInternalRenderBuffer(
				GLGPUDevice & pGPUDevice,
				GLRenderbufferObjectHandle pGLRenderbufferObject );

		GLInternalRenderBuffer(
				GLGPUDevice & pGPUDevice,
				GLTextureObjectHandle pGLTextureObject );

		static GpaHandle<GLInternalRenderBuffer> createInstance(
				GLGPUDevice & pGPUDevice,
				const RenderTargetTextureCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_RENDER_TARGET_BUFFER_H__
