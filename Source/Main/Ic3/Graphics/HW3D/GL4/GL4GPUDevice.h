
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GL4_GRAPHICS_DEVICE_H__
#define __IC3_GRAPHICS_HW3D_GL4_GRAPHICS_DEVICE_H__

#include "GL4Prerequisites.h"
#include <Ic3/Graphics/HW3D/GL/GLGpuDevice.h>
#include <Ic3/Graphics/HW3D/GL/State/GLPipelineStateObject.h>

namespace Ic3::Graphics::GCI
{

#if( IC3_GX_GL4_USE_COMPAT_API )
	using GL4GpuDeviceBase = GLGpuDeviceCompat;
#else
	using GL4GpuDeviceBase = GLGpuDeviceCore;
#endif

	struct GL4GpuDeviceCreateInfo : public GpuDeviceCreateInfo
	{
	};

	class IC3_GX_GL4_CLASS GL4GpuDevice final : public GL4GpuDeviceBase
	{
	public:
		explicit GL4GpuDevice( GL4GpuDriver & pDriver );
		virtual ~GL4GpuDevice();

		static GL4GpuDeviceHandle Create( GL4GpuDriver & pDriver, const GL4GpuDeviceCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GL4_GRAPHICS_DEVICE_H__
