
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_MTLCOMMON_GPU_DRIVER_H__
#define __IC3_GRAPHICS_HW3D_MTLCOMMON_GPU_DRIVER_H__

#include "MTLPrerequisites.h"
#include <Ic3/Graphics/GCI/GpuDriver.h>

namespace Ic3::Graphics::GCI
{

	struct MTLGpuDriverCreateInfo : public GpuDriverCreateInfo
	{
	};

	/// @brief Middle-layer OpenMTL driver class. Implements features which are common for all MTL drivers (core, ES).
	class MetalGpuDriver : public GpuDriver
	{
	public:
		System::MetalSystemDriverHandle const mSysMetalDriver;

	public:
		explicit MetalGpuDriver( System::MetalSystemDriverHandle pSysMetalDriver );
		virtual ~MetalGpuDriver();

	protected:
		/// @brief Helper method. Initializes EXF OpenMTL driver component.
		/// Used by specific sub-classes (drivers) when a driver is created.
		static System::MetalSystemDriverHandle initializeSysMTLDriver( System::SysContextHandle pSysContext );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_MTLCOMMON_GPU_DRIVER_H__
