
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GL4_GPU_DRIVER__
#define __IC3_GRAPHICS_HW3D_GL4_GPU_DRIVER__

#include "GL4Prerequisites.h"
#include <Ic3/Graphics/HW3D/GL/GLGPUDriver.h>

namespace Ic3::Graphics::GCI
{

	struct GL4GPUDriverCreateInfo : public GLGPUDriverCreateInfo
	{
	};

	class IC3_GX_GL4_CLASS GL4GPUDriver final : public GLGPUDriver
	{
	public:
		explicit GL4GPUDriver( System::OpenGLSystemDriverHandle pSysGLDriver );
		virtual ~GL4GPUDriver();

		virtual EGPUDriverID QueryGPUDriverID() const noexcept override;

		static GL4GPUDriverHandle Create( const GL4GPUDriverCreateInfo & pCreateInfo );

	private:
		virtual DisplayManagerHandle _DrvCreateDefaultDisplayManager() override;

		virtual GPUDeviceHandle _DrvCreateDevice( const GPUDeviceCreateInfo & pCreateInfo ) override;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GL4_GPU_DRIVER__
