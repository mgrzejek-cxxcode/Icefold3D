
#include "Shader.h"

namespace Ic3::Graphics::GCI
{

	Shader::Shader( GpuDevice & pGpuDevice, EShaderType pShaderType )
	: GpuDeviceChildObject( pGpuDevice )
	, mShaderType( pShaderType )
	, mShaderBinary()
	{}

	Shader::Shader(
			GpuDevice & pGpuDevice,
			EShaderType pShaderType,
			std::unique_ptr<ShaderBinary> pShaderBinary )
	: GpuDeviceChildObject( pGpuDevice )
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
			return CxDef::GetShaderStageIndex( pShader.mShaderType );
		}

		uint32 GetShaderObjectStageBit( Shader & pShader )
		{
			return CxDef::GetShaderStageBit( pShader.mShaderType );
		}

	}

} // namespace Ic3::Graphics::GCI
