
#include "GPUUtils.h"
#include "GPUDevice.h"
#include <fstream>

namespace Ic3::Graphics::GCI
{

    namespace utils
    {

        ShaderHandle CreateShaderFromSource( GPUDevice & pGPUDevice, EShaderType pShaderType, const void * pSource, size_t pSourceLength )
        {
            ShaderCreateInfo shaderCreateInfo;
            shaderCreateInfo.shaderType = pShaderType;
            shaderCreateInfo.shaderSourceView = cppx::bind_memory_view( pSource, pSourceLength );

            if( pGPUDevice.IsDebugDevice() )
            {
                shaderCreateInfo.createFlags = eShaderCreateFlagDebugBit;
            }
            else
            {
                shaderCreateInfo.createFlags = eShaderCreateFlagOptimizationL1Bit;
            }

            auto shader = pGPUDevice.CreateShader( shaderCreateInfo );
            return shader;
        }

        ShaderHandle CreateShaderFromFile( GPUDevice & pGPUDevice, EShaderType pShaderType, const char * pFilename )
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

    }

} // namespace Ic3::Graphics::GCI
