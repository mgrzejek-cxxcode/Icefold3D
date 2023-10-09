
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_UTILS_H__
#define __IC3_GRAPHICS_GCI_GPU_UTILS_H__

#include "Resources/ShaderCommon.h"

namespace Ic3::Graphics::GCI
{

    namespace utils
    {

        IC3_GRAPHICS_GCI_API ShaderHandle createShaderFromSource( GPUDevice & pGPUDevice, EShaderType pShaderType, const void * pSource, size_t pSourceLength );

        inline ShaderHandle createShaderFromSource( GPUDevice & pGPUDevice, EShaderType pShaderType, const std::string & pSource )
        {
            return createShaderFromSource( pGPUDevice, pShaderType, pSource.c_str(), pSource.length() );
        }

        IC3_GRAPHICS_GCI_API ShaderHandle createShaderFromFile( GPUDevice & pGPUDevice, EShaderType pShaderType, const char * pFilename );

        inline ShaderHandle createShaderFromFile( GPUDevice & pGPUDevice, EShaderType pShaderType, const std::string & pFilename )
        {
            return createShaderFromFile( pGPUDevice, pShaderType, pFilename.c_str() );
        }

    }

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_UTILS_H__
