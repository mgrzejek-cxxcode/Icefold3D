
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLES3_GRAPHICS_DEVICE_H__
#define __IC3_GRAPHICS_HW3D_GLES3_GRAPHICS_DEVICE_H__

#include "GLES3Prerequisites.h"
#include <Ic3/Graphics/HW3D/GL/GLGPUDevice.h>
#include <Ic3/Graphics/HW3D/GL/State/GLPipelineStateObject.h>

namespace Ic3::Graphics::GCI
{

	struct GLES3GPUDeviceCreateInfo : public GPUDeviceCreateInfo
	{
	};

	class ICFGX_GLES3_CLASS GLES3GPUDevice final : public GLGPUDevice
	{
	public:
		explicit GLES3GPUDevice( GLES3GPUDriver & pDriver );
		virtual ~GLES3GPUDevice();

		static GLES3GPUDeviceHandle create( GLES3GPUDriver & pDriver, const GLES3GPUDeviceCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLES3_GRAPHICS_DEVICE_H__
