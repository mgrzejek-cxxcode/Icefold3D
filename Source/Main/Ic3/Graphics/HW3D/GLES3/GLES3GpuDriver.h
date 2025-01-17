
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLES3_GPU_DRIVER__
#define __IC3_GRAPHICS_HW3D_GLES3_GPU_DRIVER__

#include "GLES3Prerequisites.h"
#include <Ic3/Graphics/HW3D/GL/GLGPUDriver.h>

namespace Ic3::Graphics::GCI
{

	struct GLES3GPUDriverCreateInfo : public GLGPUDriverCreateInfo
	{
	};

	class IC3_GX_GLES3_CLASS GLES3GPUDriver final : public GLGPUDriver
	{
	public:
	explicit GLES3GPUDriver( System::OpenGLSystemDriverHandle pSysGLDriver );
		virtual ~GLES3GPUDriver();

		virtual DisplayManagerHandle CreateDefaultDisplayManager() override;

		virtual GPUDeviceHandle CreateDevice( const GPUDeviceCreateInfo & pCreateInfo ) override;

		virtual EGPUDriverID QueryGPUDriverID() const noexcept override;

		static GLES3GPUDriverHandle Create( const GLES3GPUDriverCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLES3_GPU_DRIVER__
