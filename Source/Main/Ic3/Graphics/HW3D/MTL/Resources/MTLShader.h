
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_MTLCOMMON_SHADER_INTERFACE_H__
#define __IC3_GRAPHICS_HW3D_MTLCOMMON_SHADER_INTERFACE_H__

#include "../MTLPrerequisites.h"
#include <Ic3/Graphics/GCI/Resources/Shader.h>

namespace Ic3::Graphics::GCI
{

	Ic3DeclareClassHandle( MetalShader );

	class  MetalShader : public Shader
	{
	public:
		MetalShaderObjectHandle const mMetalShaderObject;
		MetalShaderProgramObjectHandle const mMetalShaderProgramObject;

	public:
		MetalShader( MetalGPUDevice & pGPUDevice, EShaderType pShaderType, MetalShaderObjectHandle pMetalShaderObject );
		MetalShader( MetalGPUDevice & pGPUDevice, EShaderType pShaderType, MetalShaderProgramObjectHandle pMetalShaderProgramObject );
		virtual ~MetalShader();

		static MetalShaderHandle CreateShaderObjectWithBinary( MetalGPUDevice & pGPUDevice, EShaderType pShaderType, const ShaderBinary & pShaderBinary );
		static MetalShaderHandle CreateShaderObjectWithSource( MetalGPUDevice & pGPUDevice, EShaderType pShaderType, const void * pSource, size_t pSourceLength );
		static MetalShaderHandle CreateShaderSeparableStageWithBinary( MetalGPUDevice & pGPUDevice, EShaderType pShaderType, const ShaderBinary & pShaderBinary );
		static MetalShaderHandle CreateShaderSeparableStageWithSource( MetalGPUDevice & pGPUDevice, EShaderType pShaderType, const void * pSource, size_t pSourceLength );
	};

	namespace RCU
	{

		MetalShaderHandle CreateShaderObject( MetalGPUDevice & pGPUDevice, const ShaderCreateInfo & pCreateInfo );

		MetalShaderHandle CreateShaderSeparableStage( MetalGPUDevice & pGPUDevice, const ShaderCreateInfo & pCreateInfo );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_MTLCOMMON_SHADER_INTERFACE_H__
