
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLC_SHADER_H__
#define __IC3_GRAPHICS_HW3D_GLC_SHADER_H__

#include "../GLPrerequisites.h"
#include "../Objects/GLShaderObject.h"
#include "../Objects/GLShaderPipelineObject.h"
#include "../Objects/GLShaderProgramObject.h"
#include <Ic3/Graphics/GCI/Resources/Shader.h>

namespace Ic3::Graphics::GCI
{

	Ic3DeclareClassHandle( GLShader );

	class GLShader : public Shader
	{
	public:
		GLShaderObjectHandle const mGLShaderObject;
		GLShaderProgramObjectHandle const mGLShaderProgramObject;

	public:
		GLShader(
				GLGPUDevice & pGPUDevice,
				EShaderType pShaderType,
				GLShaderObjectHandle pGLShaderObject );

		GLShader(
				GLGPUDevice & pGPUDevice,
				EShaderType pShaderType,
				GLShaderProgramObjectHandle pGLShaderProgramObject );

		virtual ~GLShader();

		static GLShaderHandle CreateInstance( GLGPUDevice & pGPUDevice, const ShaderCreateInfo & pCreateInfo );

	private:
		static GLShaderHandle CreateInstanceFromSource(
				GLGPUDevice & pGPUDevice,
				EShaderType pShaderType,
				const void * pSource,
				size_t pSourceLength );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLC_SHADER_H__
