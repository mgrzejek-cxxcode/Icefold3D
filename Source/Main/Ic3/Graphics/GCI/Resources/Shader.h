
#pragma once

#ifndef __IC3_GRAPHICS_GCI_SHADER_H__
#define __IC3_GRAPHICS_GCI_SHADER_H__

#include "../State/ShaderInputSignature.h"

namespace Ic3::Graphics::GCI
{

	class IC3_GRAPHICS_GCI_CLASS Shader : public GPUDeviceChildObject
	{
	public:
		EShaderType const mShaderType;

	public:
		Shader( GPUDevice & pGPUDevice, EShaderType pShaderType );
		virtual ~Shader();

		bool HasShaderBinaryCached() const noexcept;

		bool HasShaderInputSignature() const noexcept;

		const ShaderBinary * GetShaderBinary() const noexcept;

		const ShaderInputSignature * GetShaderInputSignature() const noexcept;

		ShaderBinary * InitializeShaderBinaryStorage( size_t pBinarySizeInBytes );

		ShaderBinary * SetShaderBinary( std::unique_ptr<ShaderBinary> pShaderBinary );

		ShaderInputSignature * InitializeShaderInputSignature();

		ShaderInputSignature * SetShaderInputSignature( std::unique_ptr<ShaderInputSignature> pShaderInputSignature );

	private:
		std::unique_ptr<ShaderBinary> _shaderBinary;
		std::unique_ptr<ShaderInputSignature> _shaderInputSignature;
	};

	inline bool Shader::HasShaderBinaryCached() const noexcept
	{
		return static_cast<bool>( _shaderBinary );
	}

	inline bool Shader::HasShaderInputSignature() const noexcept
	{
		return static_cast<bool>( _shaderInputSignature );
	}

	inline const ShaderBinary * Shader::GetShaderBinary() const noexcept
	{
		return _shaderBinary.get();
	}

	inline const ShaderInputSignature * Shader::GetShaderInputSignature() const noexcept
	{
		return _shaderInputSignature.get();
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_SHADER_H__
