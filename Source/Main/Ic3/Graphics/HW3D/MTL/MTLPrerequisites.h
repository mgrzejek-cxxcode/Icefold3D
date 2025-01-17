
#ifndef __IC3_GRAPHICS_HW3D_MTLCOMMON_PREREQUISITES_H__
#define __IC3_GRAPHICS_HW3D_MTLCOMMON_PREREQUISITES_H__

#include <Ic3/Graphics/GCI/Memory/CommonGpuMemoryDefs.h>
#include <Ic3/System/MetalDriver.h>
#include <Ic3/System/MetalNative.h>

#include <Metal/MTLDefines.h>
#include <Metal/MTLDevice.h>
#include <Metal/MTLEvent.h>
#include <Metal/MTLFence.h>

#include "Prerequisites/MTL_featureLevel.h"

namespace Ic3::Graphics::GCI
{

	Ic3DeclareClassHandle( MetalCommandList );
	Ic3DeclareClassHandle( MetalCommandSystem );
	Ic3DeclareClassHandle( MetalGpuDevice );
	Ic3DeclareClassHandle( MetalGpuDriver );
	Ic3DeclareClassHandle( MetalPresentationLayer );
	
} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_MTLCOMMON_PREREQUISITES_H__
