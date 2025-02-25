
#include "Shader.h"

namespace Ic3::Graphics::GCI
{

	Shader::Shader( GPUDevice & pGPUDevice, EShaderType pShaderType )
	: GPUDeviceChildObject( pGPUDevice )
	, mShaderType( pShaderType )
	, mShaderBinary()
	{}

	Shader::Shader(
			GPUDevice & pGPUDevice,
			EShaderType pShaderType,
			std::unique_ptr<ShaderBinary> pShaderBinary )
	: GPUDeviceChildObject( pGPUDevice )
	, mShaderType( pShaderType )
	, mShaderBinary( std::move( pShaderBinary ) )
	{}

	Shader::~Shader()
	{
		const_cast< std::unique_ptr<ShaderBinary> * >( &mShaderBinary )->release();
	}


	std::unique_ptr<ShaderBinary> ShaderBinary::Create( size_t pBinarySize )
	{
		const auto requiredBinaryStorageSize = pBinarySize - dataBufferFixedSize;
		auto * shaderBinary = new ( cvAllocNewSizeExplicit, requiredBinaryStorageSize ) ShaderBinary();
		return std::unique_ptr<ShaderBinary>{ shaderBinary };
	}


	namespace RCU
	{

		EShaderType GetShaderObjectType( Shader & pShader )
		{
			return pShader.mShaderType;
		}

		uint32 GetShaderObjectStageIndex( Shader & pShader )
		{
			return CXU::SHGetShaderStageIndex( pShader.mShaderType );
		}

		uint32 GetShaderObjectStageBit( Shader & pShader )
		{
			return CXU::SHGetShaderStageBit( pShader.mShaderType );
		}

	}

} // namespace Ic3::Graphics::GCI
