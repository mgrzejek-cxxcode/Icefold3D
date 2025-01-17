
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLES3_GPU_DRIVER__
#define __IC3_GRAPHICS_HW3D_GLES3_GPU_DRIVER__

#include "GLES3Prerequisites.h"
#include <Ic3/Graphics/HW3D/GL/GLGpuDriver.h>

namespace Ic3::Graphics::GCI
{

	struct GLES3GpuDriverCreateInfo : public GLGpuDriverCreateInfo
	{
	};

	class IC3_GX_GLES3_CLASS GLES3GpuDriver final : public GLGpuDriver
	{
	public:
	explicit GLES3GpuDriver( System::OpenGLSystemDriverHandle pSysGLDriver );
		virtual ~GLES3GpuDriver();

		virtual DisplayManagerHandle CreateDefaultDisplayManager() override;

		virtual GpuDeviceHandle CreateDevice( const GpuDeviceCreateInfo & pCreateInfo ) override;

		virtual EGpuDriverID QueryGpuDriverID() const noexcept override;

		static GLES3GpuDriverHandle Create( const GLES3GpuDriverCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLES3_GPU_DRIVER__
