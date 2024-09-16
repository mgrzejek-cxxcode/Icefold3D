
#pragma once

#ifndef __IC3_GRAPHICS_GCI_SHADER_H__
#define __IC3_GRAPHICS_GCI_SHADER_H__

#include "CommonGPUResourceDefs.h"
#include "ShaderCommon.h"

namespace Ic3::Graphics::GCI
{

	class IC3_GRAPHICS_GCI_CLASS Shader : public GPUDeviceChildObject
	{
	public:
		EShaderType const mShaderType;

		std::unique_ptr<ShaderBinary> const mShaderBinary;

	public:
		Shader( GPUDevice & pGPUDevice, EShaderType pShaderType );
		Shader( GPUDevice & pGPUDevice, EShaderType pShaderType, std::unique_ptr<ShaderBinary> pShaderBinary );
		virtual ~Shader();

//		virtual ResultCode compileFromSource( const void * pSource, size_t pSourceLength ) = 0;
//
//		virtual ResultCode compileFromSource(
//				const void * pSource,
//				size_t pSourceLength,
//				const std::string & pOptions,
//				TBitmask<uint64> pFlags ) = 0;
//
//		virtual ResultCode loadPrecompiledBinary( const void * pBinary, size_t pBinaryLength ) = 0;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_SHADER_H__
