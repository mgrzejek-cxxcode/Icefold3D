
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
		MetalShader( MetalGpuDevice & pGpuDevice, EShaderType pShaderType, MetalShaderObjectHandle pMetalShaderObject );
		MetalShader( MetalGpuDevice & pGpuDevice, EShaderType pShaderType, MetalShaderProgramObjectHandle pMetalShaderProgramObject );
		virtual ~MetalShader();

		static MetalShaderHandle CreateShaderObjectWithBinary( MetalGpuDevice & pGpuDevice, EShaderType pShaderType, const ShaderBinary & pShaderBinary );
		static MetalShaderHandle CreateShaderObjectWithSource( MetalGpuDevice & pGpuDevice, EShaderType pShaderType, const void * pSource, size_t pSourceLength );
		static MetalShaderHandle CreateShaderSeparableStageWithBinary( MetalGpuDevice & pGpuDevice, EShaderType pShaderType, const ShaderBinary & pShaderBinary );
		static MetalShaderHandle CreateShaderSeparableStageWithSource( MetalGpuDevice & pGpuDevice, EShaderType pShaderType, const void * pSource, size_t pSourceLength );
	};

	namespace RCU
	{

		MetalShaderHandle CreateShaderObject( MetalGpuDevice & pGpuDevice, const ShaderCreateInfo & pCreateInfo );

		MetalShaderHandle CreateShaderSeparableStage( MetalGpuDevice & pGpuDevice, const ShaderCreateInfo & pCreateInfo );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_MTLCOMMON_SHADER_INTERFACE_H__
