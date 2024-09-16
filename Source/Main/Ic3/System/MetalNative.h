
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
		id<MTLDevice> mMTLDevice;
		id<MTLCommandQueue> mMTLMainCmdQueue;
	};

	struct MetalDisplaySurfaceData
	{
		CAMetalLayer * mCAMetalLayer = nil;
		MTLRenderPassDescriptor * mMTLClearRPDescriptor = nil;
	};

	struct MetalSystemDriverData
	{
	};

}

#endif // __IC3_SYSTEM_METAL_NATIVE_H__
