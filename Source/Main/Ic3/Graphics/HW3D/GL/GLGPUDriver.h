
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLC_GPU_DRIVER_H__
#define __IC3_GRAPHICS_HW3D_GLC_GPU_DRIVER_H__

#include "GLPrerequisites.h"
#include <Ic3/Graphics/GCI/GPUDriver.h>

namespace Ic3::Graphics::GCI
{

	struct GLGPUDriverCreateInfo : public GPUDriverCreateInfo
	{
	};

	/// @brief Middle-layer OpenGL driver class. Implements features which are common for all GL drivers (core, ES).
	class IC3_GX_GL_CLASS GLGPUDriver : public GPUDriver
	{
	public:
		System::OpenGLSystemDriverHandle const mSysGLDriver;

	public:
		explicit GLGPUDriver( System::OpenGLSystemDriverHandle pSysGLDriver );
		virtual ~GLGPUDriver();

	protected:
		/// @brief Helper method. Initializes EXF OpenGL driver component.
		/// Used by specific sub-classes (drivers) when a driver is created.
		static System::OpenGLSystemDriverHandle InitializeSysGLDriver( System::SysContextHandle pSysContext );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLC_GPU_DRIVER_H__
