
#ifndef __IC3_SYSTEM_METAL_NATIVE_H__
#define __IC3_SYSTEM_METAL_NATIVE_H__

#include "MetalDriver.h"

#import <Metal/MTLDevice.h>
#import <Metal/MTLCommandQueue.h>
#import <Metal/MTLRenderPass.h>
#import <QuartzCore/CAMetalLayer.h>

namespace Ic3::System
{

	struct MetalDeviceData
	{
		id<MTLDevice> mtlDevice;
		id<MTLCommandQueue> mtlMainCmdQueue;
	};

	struct MetalDisplaySurfaceData
	{
		CAMetalLayer * caMetalLayer = nil;
		MTLRenderPassDescriptor * mtlClearRPDescriptor = nil;
	};

	struct MetalSystemDriverData
	{
	};

}

#endif // __IC3_SYSTEM_METAL_NATIVE_H__
