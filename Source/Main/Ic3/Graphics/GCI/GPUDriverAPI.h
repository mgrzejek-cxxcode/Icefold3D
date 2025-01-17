
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_DRIVER_API_H__
#define __IC3_GRAPHICS_GCI_GPU_DRIVER_API_H__

#include "Prerequisites.h"
#include <functional>

namespace Ic3::Graphics::GCI
{

	struct GpuDriverCreateInfo;
	struct PresentationLayerCreateInfo;

	struct GpuDriverAPI
	{
		using CreateDriverFuncType = std::function<GpuDriverHandle( const GpuDriverCreateInfo & )>;
		using CreateScreenPresentationLayerFuncType = std::function<PresentationLayerHandle( GpuDevice &, const PresentationLayerCreateInfo & )>;

		CreateDriverFuncType CreateDriver;
		CreateScreenPresentationLayerFuncType CreateScreenPresentationLayer;
	};

	class GpuDriverInterface
	{
	public:
		GpuDriverInterface() = default;
		virtual ~GpuDriverInterface() = default;

		virtual GpuDriverHandle CreateDriver( const GpuDriverCreateInfo & pCreateInfo ) = 0;
		virtual PresentationLayerHandle CreateScreenPresentationLayer( GpuDevice & pDevice, const PresentationLayerCreateInfo & pCreateInfo ) = 0;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_DRIVER_API_H__
