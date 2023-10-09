
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GL4_GRAPHICS_DEVICE_H__
#define __IC3_GRAPHICS_HW3D_GL4_GRAPHICS_DEVICE_H__

#include "GL4Prerequisites.h"
#include <Ic3/Graphics/HW3D/GL/GLGPUDevice.h>
#include <Ic3/Graphics/HW3D/GL/State/GLPipelineStateObject.h>

namespace Ic3::Graphics::GCI
{

#if( ICFGX_GL4_USE_COMPAT_API )
	using GL4GPUDeviceBase = GLGPUDeviceCompat;
#else
	using GL4GPUDeviceBase = GLGPUDeviceCore;
#endif

	struct GL4GPUDeviceCreateInfo : public GPUDeviceCreateInfo
	{
	};

	class ICFGX_GL4_CLASS GL4GPUDevice final : public GL4GPUDeviceBase
	{
	public:
		explicit GL4GPUDevice( GL4GPUDriver & pDriver );
		virtual ~GL4GPUDevice();

		static GL4GPUDeviceHandle create( GL4GPUDriver & pDriver, const GL4GPUDeviceCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GL4_GRAPHICS_DEVICE_H__
