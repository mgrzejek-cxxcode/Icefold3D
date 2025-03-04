
#include "ShaderUtils.h"
#include <Ic3/Graphics/GCI/GPUDevice.h>
#include <fstream>

namespace Ic3
{

	namespace GCIUtils
	{
		GCI::ShaderHandle CreateShaderFromSource(
				GCI::GPUDevice & pGPUDevice,
				GCI::EShaderType pShaderType,
				const void * pSource,
				size_t pSourceLength )
		{
			GCI::ShaderCreateInfo shaderCreateInfo;
			shaderCreateInfo.shaderType = pShaderType;
			shaderCreateInfo.shaderSourceView = cppx::bind_memory_view( pSource, pSourceLength );

			if( pGPUDevice.IsDebugDevice() )
			{
				shaderCreateInfo.createFlags = GCI::eShaderCreateFlagDebugBit;
			} else
			{
				shaderCreateInfo.createFlags = GCI::eShaderCreateFlagOptimizationL1Bit;
			}

			auto shader = pGPUDevice.CreateShader( shaderCreateInfo );
			return shader;
		}

		GCI::ShaderHandle CreateShaderFromFile(
				GCI::GPUDevice & pGPUDevice,
				GCI::EShaderType pShaderType,
				const char * pFilename )
		{
			std::ifstream inputFile{ pFilename, std::ios::in };
			if( !inputFile )
			{
				return nullptr;
			}

			inputFile.seekg( 0, std::ios::end );

			auto fileSize = static_cast<size_t>( inputFile.tellg() );
			if( fileSize == static_cast<std::ifstream::pos_type>( -1 ) )
			{
				return nullptr;
			}

			inputFile.seekg( 0, std::ios::beg );

			std::vector<char> fileContent;
			fileContent.resize( fileSize + 1 );
			inputFile.read( fileContent.data(), fileSize );
			fileContent[fileSize] = 0;

			return CreateShaderFromSource( pGPUDevice, pShaderType, fileContent.data(), fileContent.size() );
		}

	} // namespace GCIUtils

} // namespace Ic3

