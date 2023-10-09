
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_MTLCOMMON_SHADER_INTERFACE_H__
#define __IC3_GRAPHICS_HW3D_MTLCOMMON_SHADER_INTERFACE_H__

#include "../MTLPrerequisites.h"
#include <Ic3/Graphics/GCI/Resources/Shader.h>

namespace Ic3::Graphics::GCI
{

	ic3DeclareClassHandle( MetalShader );

	class  MetalShader : public Shader
	{
	public:
		MetalShaderObjectHandle const mMetalShaderObject;
		MetalShaderProgramObjectHandle const mMetalShaderProgramObject;

	public:
		MetalShader( MetalGPUDevice & pGPUDevice, EShaderType pShaderType, MetalShaderObjectHandle pMetalShaderObject );
		MetalShader( MetalGPUDevice & pGPUDevice, EShaderType pShaderType, MetalShaderProgramObjectHandle pMetalShaderProgramObject );
		virtual ~MetalShader();

		static MetalShaderHandle createShaderObjectWithBinary( MetalGPUDevice & pGPUDevice, EShaderType pShaderType, const ShaderBinary & pShaderBinary );
		static MetalShaderHandle createShaderObjectWithSource( MetalGPUDevice & pGPUDevice, EShaderType pShaderType, const void * pSource, size_t pSourceLength );
		static MetalShaderHandle createShaderSeparableStageWithBinary( MetalGPUDevice & pGPUDevice, EShaderType pShaderType, const ShaderBinary & pShaderBinary );
		static MetalShaderHandle createShaderSeparableStageWithSource( MetalGPUDevice & pGPUDevice, EShaderType pShaderType, const void * pSource, size_t pSourceLength );
	};

	namespace rcutil
	{

		MetalShaderHandle createShaderObject( MetalGPUDevice & pGPUDevice, const ShaderCreateInfo & pCreateInfo );

		MetalShaderHandle createShaderSeparableStage( MetalGPUDevice & pGPUDevice, const ShaderCreateInfo & pCreateInfo );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_MTLCOMMON_SHADER_INTERFACE_H__
