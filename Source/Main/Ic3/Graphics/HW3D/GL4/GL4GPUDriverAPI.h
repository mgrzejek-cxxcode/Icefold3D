
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GL4_GRAPHICS_DRIVER_API_H__
#define __IC3_GRAPHICS_HW3D_GL4_GRAPHICS_DRIVER_API_H__

#include "GL4Prerequisites.h"
#include <Ic3/Graphics/GCI/GPUDriverAPI.h>

namespace Ic3::Graphics::GCI
{

	class GL4GPUDriverInterface : public GPUDriverInterface
	{
	public:
		virtual GPUDriverHandle createDriver( const GPUDriverCreateInfo & pCreateInfo ) override final;
		virtual PresentationLayerHandle createScreenPresentationLayer( GPUDevice & pDevice, const PresentationLayerCreateInfo & pCreateInfo ) override final;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GL4_GRAPHICS_DRIVER_API_H__
