
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_DRIVER_NULL_H__
#define __IC3_GRAPHICS_GCI_GPU_DRIVER_NULL_H__

#include "GPUDriver.h"

namespace Ic3::Graphics::GCI
{

	class GPUDriverNull : public GPUDriver
	{
	public:
		explicit GPUDriverNull() noexcept
		: GPUDriver( nullptr )
		{}

		virtual ~GPUDriverNull() noexcept = default;

		IC3_ATTR_NO_DISCARD virtual EGPUDriverID queryGPUDriverID() const noexcept override final
		{
			return EGPUDriverID::GDINull;
		}

		IC3_ATTR_NO_DISCARD virtual bool isNullDriver() const noexcept override final
		{
			return true;
		}

	private:
		virtual DisplayManagerHandle _drvCreateDefaultDisplayManager() override final
		{
			return nullptr;
		}

		virtual GPUDeviceHandle _drvCreateDevice( const GPUDeviceCreateInfo & pCreateInfo ) override final
		{
			return nullptr;
		}
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_DRIVER_NULL_H__
