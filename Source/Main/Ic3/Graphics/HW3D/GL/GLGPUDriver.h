
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_GPU_DRIVER_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_GPU_DRIVER_H__

#include "GLPrerequisites.h"
#include <Ic3/Graphics/GCI/GpuDriver.h>

namespace Ic3::Graphics::GCI
{

	struct GLGpuDriverCreateInfo : public GpuDriverCreateInfo
	{
	};

	/// @brief Middle-layer OpenGL driver class. Implements features which are common for all GL drivers (core, ES).
	class IC3_GX_GL_CLASS GLGpuDriver : public GpuDriver
	{
	public:
		System::OpenGLSystemDriverHandle const mSysGLDriver;

	public:
		explicit GLGpuDriver( System::OpenGLSystemDriverHandle pSysGLDriver );
		virtual ~GLGpuDriver();

	protected:
		/// @brief Helper method. Initializes EXF OpenGL driver component.
		/// Used by specific sub-classes (drivers) when a driver is created.
		static System::OpenGLSystemDriverHandle InitializeSysGLDriver( System::SysContextHandle pSysContext );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_GPU_DRIVER_H__
