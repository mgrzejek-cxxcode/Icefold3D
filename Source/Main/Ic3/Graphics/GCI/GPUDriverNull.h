
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

		CPPX_ATTR_NO_DISCARD virtual EGPUDriverID QueryGPUDriverID() const noexcept override final
		{
			return EGPUDriverID::GDINull;
		}

		CPPX_ATTR_NO_DISCARD virtual bool IsNullDriver() const noexcept override final
		{
			return true;
		}

	private:
		virtual DisplayManagerHandle _DrvCreateDefaultDisplayManager() override final
		{
			return nullptr;
		}

		virtual GPUDeviceHandle _DrvCreateDevice( const GPUDeviceCreateInfo & pCreateInfo ) override final
		{
			return nullptr;
		}
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_DRIVER_NULL_H__
