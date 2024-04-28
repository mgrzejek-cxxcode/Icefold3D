
#ifndef __IC3_SYSTEM_PLATFORM_SHARED_MTL_COMMON_H__
#define __IC3_SYSTEM_PLATFORM_SHARED_MTL_COMMON_H__

#include <Ic3/System/MetalCommon.h>

#import <CoreGraphics/CGDirectDisplayMetal.h>
#import <Metal/MTLPixelFormat.h>
#import <QuartzCore/CAMetalLayer.h>

namespace Ic3::System
{

    namespace Platform
    {

        IC3_ATTR_NO_DISCARD MTLPixelFormat mtlChoosePixelFormatForVisualConfig( const VisualConfig & pVisualConfig );

    }

}

#endif // __IC3_SYSTEM_PLATFORM_SHARED_MTL_COMMON_H__
