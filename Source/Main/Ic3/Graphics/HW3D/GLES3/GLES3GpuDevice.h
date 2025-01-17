
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLES3_GRAPHICS_DEVICE_H__
#define __IC3_GRAPHICS_HW3D_GLES3_GRAPHICS_DEVICE_H__

#include "GLES3Prerequisites.h"
#include <Ic3/Graphics/HW3D/GL/GLGpuDevice.h>
#include <Ic3/Graphics/HW3D/GL/State/GLPipelineStateObject.h>

namespace Ic3::Graphics::GCI
{

	struct GLES3GpuDeviceCreateInfo : public GpuDeviceCreateInfo
	{
	};

	class IC3_GX_GLES3_CLASS GLES3GpuDevice final : public GLGpuDevice
	{
	public:
		explicit GLES3GpuDevice( GLES3GpuDriver & pDriver );
		virtual ~GLES3GpuDevice();

		static GLES3GpuDeviceHandle Create( GLES3GpuDriver & pDriver, const GLES3GpuDeviceCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLES3_GRAPHICS_DEVICE_H__
