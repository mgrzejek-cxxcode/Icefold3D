
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX12_GPU_DRIVER_API_H__
#define __IC3_GRAPHICS_HW3D_DX12_GPU_DRIVER_API_H__

#include "DX12Prerequisites.h"
#include <Ic3/Graphics/GCI/GPUDriverAPI.h>

namespace Ic3::Graphics::GCI
{

	class DX12GPUDriverInterface : public GPUDriverInterface
	{
	public:
		virtual GPUDriverHandle createDriver( const GPUDriverCreateInfo & pCreateInfo ) override final;
		virtual PresentationLayerHandle createScreenPresentationLayer( GPUDevice & pDevice, const PresentationLayerCreateInfo & pCreateInfo ) override final;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX12_GPU_DRIVER_API_H__
