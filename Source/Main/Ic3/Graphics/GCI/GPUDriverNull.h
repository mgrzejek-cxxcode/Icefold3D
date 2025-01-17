
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_DRIVER_NULL_H__
#define __IC3_GRAPHICS_GCI_GPU_DRIVER_NULL_H__

#include "GpuDriver.h"

namespace Ic3::Graphics::GCI
{

	class GpuDriverNull : public GpuDriver
	{
	public:
		explicit GpuDriverNull() noexcept
		: GpuDriver( nullptr )
		{}

		virtual ~GpuDriverNull() noexcept = default;

		CPPX_ATTR_NO_DISCARD virtual EGpuDriverID QueryGpuDriverID() const noexcept override final
		{
			return EGpuDriverID::GDINull;
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

		virtual GpuDeviceHandle _DrvCreateDevice( const GpuDeviceCreateInfo & pCreateInfo ) override final
		{
			return nullptr;
		}
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_DRIVER_NULL_H__
