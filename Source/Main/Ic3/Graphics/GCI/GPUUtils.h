
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_UTILS_H__
#define __IC3_GRAPHICS_GCI_GPU_UTILS_H__

#include "Resources/ShaderCommon.h"

namespace Ic3::Graphics::GCI
{

    namespace utils
    {

        IC3_GRAPHICS_GCI_API ShaderHandle CreateShaderFromSource( GpuDevice & pGpuDevice, EShaderType pShaderType, const void * pSource, size_t pSourceLength );

        inline ShaderHandle CreateShaderFromSource( GpuDevice & pGpuDevice, EShaderType pShaderType, const std::string & pSource )
        {
            return CreateShaderFromSource( pGpuDevice, pShaderType, pSource.c_str(), pSource.length() );
        }

        IC3_GRAPHICS_GCI_API ShaderHandle CreateShaderFromFile( GpuDevice & pGpuDevice, EShaderType pShaderType, const char * pFilename );

        inline ShaderHandle CreateShaderFromFile( GpuDevice & pGpuDevice, EShaderType pShaderType, const std::string & pFilename )
        {
            return CreateShaderFromFile( pGpuDevice, pShaderType, pFilename.c_str() );
        }

    }

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_UTILS_H__
