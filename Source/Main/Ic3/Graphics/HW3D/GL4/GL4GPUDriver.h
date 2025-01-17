
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GL4_GPU_DRIVER__
#define __IC3_GRAPHICS_HW3D_GL4_GPU_DRIVER__

#include "GL4Prerequisites.h"
#include <Ic3/Graphics/HW3D/GL/GLGpuDriver.h>

namespace Ic3::Graphics::GCI
{

	struct GL4GpuDriverCreateInfo : public GLGpuDriverCreateInfo
	{
	};

	class IC3_GX_GL4_CLASS GL4GpuDriver final : public GLGpuDriver
	{
	public:
		explicit GL4GpuDriver( System::OpenGLSystemDriverHandle pSysGLDriver );
		virtual ~GL4GpuDriver();

		virtual EGpuDriverID QueryGpuDriverID() const noexcept override;

		static GL4GpuDriverHandle Create( const GL4GpuDriverCreateInfo & pCreateInfo );

	private:
		virtual DisplayManagerHandle _DrvCreateDefaultDisplayManager() override;

		virtual GpuDeviceHandle _DrvCreateDevice( const GpuDeviceCreateInfo & pCreateInfo ) override;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GL4_GPU_DRIVER__
