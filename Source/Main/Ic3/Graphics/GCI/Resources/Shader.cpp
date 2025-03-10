
#include "Shader.h"
#include <Ic3/CoreLib/Memory/CommonMemoryDefs.h>

namespace Ic3::Graphics::GCI
{

	std::unique_ptr<ShaderBinary> ShaderBinary::Create( size_t pBinarySize )
	{
		const auto requiredBinaryStorageSize = pBinarySize - dataBufferFixedSize;
		auto * shaderBinary = new( kAllocNewSizeExplicit, requiredBinaryStorageSize ) ShaderBinary();
		shaderBinary->dataSizeInBytes = cppx::numeric_cast<uint32>( pBinarySize );
		return std::unique_ptr<ShaderBinary>{ shaderBinary };
	}


	Shader::Shader( GPUDevice & pGPUDevice, EShaderType pShaderType )
	: GPUDeviceChildObject( pGPUDevice )
	, mShaderType( pShaderType )
	{}

	Shader::~Shader()
	{
		if( _shaderBinary )
		{
			_shaderBinary.release();
		}

		if( _shaderInputSignature )
		{
			_shaderInputSignature.release();
		}
	}

	ShaderBinary * Shader::InitializeShaderBinaryStorage( size_t pBinarySizeInBytes )
	{
		Ic3DebugAssert( !_shaderBinary );
		_shaderBinary = ShaderBinary::Create( pBinarySizeInBytes );
		return _shaderBinary.get();
	}

	ShaderBinary * Shader::SetShaderBinary( std::unique_ptr<ShaderBinary> pShaderBinary )
	{
		Ic3DebugAssert( !_shaderBinary );
		_shaderBinary = std::move( pShaderBinary );
		return _shaderBinary.get();
	}

	ShaderInputSignature * Shader::InitializeShaderInputSignature()
	{
		Ic3DebugAssert( !_shaderInputSignature );
		_shaderInputSignature = std::make_unique<ShaderInputSignature>();
		return _shaderInputSignature.get();
	}

	ShaderInputSignature * Shader::SetShaderInputSignature( std::unique_ptr<ShaderInputSignature> pShaderInputSignature )
	{
		Ic3DebugAssert( !_shaderInputSignature );
		_shaderInputSignature = std::move( pShaderInputSignature );
		return _shaderInputSignature.get();
	}

} // namespace Ic3::Graphics::GCI
