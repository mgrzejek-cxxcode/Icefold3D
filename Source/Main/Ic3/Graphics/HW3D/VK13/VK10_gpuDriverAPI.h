
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_VK13_GPU_DRIVER_API_H__
#define __IC3_GRAPHICS_HW3D_VK13_GPU_DRIVER_API_H__

#include "ic3/GPUapiVK/VKPrerequisites.h"
#include <Ic3/Graphics/GCI/GPUDriverAPI.h>

namespace Ic3::Graphics::GCI
{

	class VK10GPUDriverInterface : public GPUDriverInterface
	{
	public:
		virtual GPUDriverHandle createDriver( const GPUDriverCreateInfo & pCreateInfo ) override final;
		virtual PresentationLayerHandle createScreenPresentationLayer( GPUDevice * pDevice, const PresentationLayerCreateInfo & pCreateInfo ) override final;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_VK13_GPU_DRIVER_API_H__
