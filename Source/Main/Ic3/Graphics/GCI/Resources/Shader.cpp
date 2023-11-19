
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


	std::unique_ptr<ShaderBinary> ShaderBinary::create( size_t pBinarySize )
	{
		const auto requiredBinaryStorageSize = pBinarySize - sDataBufferFixedSize;
		auto * shaderBinary = new ( cvAllocNewSizeExplicit, requiredBinaryStorageSize ) ShaderBinary();
		return std::unique_ptr<ShaderBinary>{ shaderBinary };
	}


	namespace rcutil
	{

		EShaderType getShaderObjectType( Shader & pShader )
		{
			return pShader.mShaderType;
		}

		uint32 getShaderObjectStageIndex( Shader & pShader )
		{
			return CxDef::getShaderStageIndex( pShader.mShaderType );
		}

		uint32 getShaderObjectStageBit( Shader & pShader )
		{
			return CxDef::getShaderStageBit( pShader.mShaderType );
		}

	}

} // namespace Ic3::Graphics::GCI
