
#include "GPUUtils.h"
#include "GPUDevice.h"
#include <fstream>

namespace Ic3::Graphics::GCI
{

    namespace utils
    {

        ShaderHandle createShaderFromSource( GPUDevice & pGPUDevice, EShaderType pShaderType, const void * pSource, size_t pSourceLength )
        {
            ShaderCreateInfo shaderCreateInfo;
            shaderCreateInfo.mShaderType = pShaderType;
            shaderCreateInfo.mShaderSourceView = bindMemoryView( pSource, pSourceLength );

            if( pGPUDevice.isDebugDevice() )
            {
                shaderCreateInfo.mCreateFlags = eShaderCreateFlagDebugBit;
            }
            else
            {
                shaderCreateInfo.mCreateFlags = eShaderCreateFlagOptimizationL1Bit;
            }

            auto shader = pGPUDevice.createShader( shaderCreateInfo );
            return shader;
        }

        ShaderHandle createShaderFromFile( GPUDevice & pGPUDevice, EShaderType pShaderType, const char * pFilename )
	    {
            std::ifstream inputFile( pFilename, std::ios::in );
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

            return createShaderFromSource( pGPUDevice, pShaderType, fileContent.data(), fileContent.size() );
        }

    }

} // namespace Ic3::Graphics::GCI
