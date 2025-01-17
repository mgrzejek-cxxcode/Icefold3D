
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX12_GPU_DRIVER_API_H__
#define __IC3_GRAPHICS_HW3D_DX12_GPU_DRIVER_API_H__

#include "DX12Prerequisites.h"
#include <Ic3/Graphics/GCI/GpuDriverAPI.h>

namespace Ic3::Graphics::GCI
{

	class DX12GpuDriverInterface : public GpuDriverInterface
	{
	public:
		virtual GpuDriverHandle CreateDriver( const GpuDriverCreateInfo & pCreateInfo ) override final;
		virtual PresentationLayerHandle CreateScreenPresentationLayer( GpuDevice & pDevice, const PresentationLayerCreateInfo & pCreateInfo ) override final;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX12_GPU_DRIVER_API_H__
