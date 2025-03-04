
#pragma once

#ifndef __IC3_NXMAIN_SHADER_UTILS_H__
#define __IC3_NXMAIN_SHADER_UTILS_H__

#include "../Prerequisites.h"
#include <Ic3/Graphics/GCI/State/ShaderInputSignature.h>

namespace Ic3
{

	namespace GCIUtils
	{

		/**
		 * 
		 * @param pGPUDevice 
		 * @param pShaderType 
		 * @param pSource 
		 * @param pSourceLength 
		 * @return 
		 */
		IC3_GRAPHICS_GCI_API_NO_DISCARD GCI::ShaderHandle CreateShaderFromSource(
				GCI::GPUDevice & pGPUDevice,
				GCI::EShaderType pShaderType,
				const void * pSource,
				size_t pSourceLength );

		/**
		 * 
		 * @param pGPUDevice 
		 * @param pShaderType 
		 * @param pSource 
		 * @return 
		 */
		inline GCI::ShaderHandle CreateShaderFromSource(
				GCI::GPUDevice & pGPUDevice,
				GCI::EShaderType pShaderType,
				const std::string & pSource )
		{
			return CreateShaderFromSource( pGPUDevice, pShaderType, pSource.c_str(), pSource.length() );
		}

		/**
		 * 
		 * @param pGPUDevice 
		 * @param pShaderType 
		 * @param pFilename 
		 * @return 
		 */
		IC3_GRAPHICS_GCI_API_NO_DISCARD GCI::ShaderHandle CreateShaderFromFile(
				GCI::GPUDevice & pGPUDevice,
				GCI::EShaderType pShaderType,
				const char * pFilename );

		/**
		 * 
		 * @param pGPUDevice 
		 * @param pShaderType 
		 * @param pFilename 
		 * @return 
		 */
		inline GCI::ShaderHandle CreateShaderFromFile(
				GCI::GPUDevice & pGPUDevice,
				GCI::EShaderType pShaderType,
				const std::string & pFilename )
		{
			return CreateShaderFromFile( pGPUDevice, pShaderType, pFilename.c_str() );
		}
		
	} // namespace GCIUtils

} // namespace Ic3::Graphics::GCI

#endif // __IC3_NXMAIN_SHADER_UTILS_H__
