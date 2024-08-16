
#include "MTLCommon.h"

namespace Ic3::System
{

    namespace Platform
    {

        MTLPixelFormat mtlChoosePixelFormatForVisualConfig( const VisualConfig & pVisualConfig )
        {
            switch( pVisualConfig.colorFormat )
            {
                ic3CaseReturn( EColorFormat::B8G8R8,       MTLPixelFormatBGRA8Unorm );
                ic3CaseReturn( EColorFormat::B8G8R8A8,     MTLPixelFormatBGRA8Unorm );
                ic3CaseReturn( EColorFormat::B8G8R8A8SRGB, MTLPixelFormatBGRA8Unorm_sRGB );
                ic3CaseReturn( EColorFormat::B8G8R8X8,     MTLPixelFormatBGRA8Unorm );
                ic3CaseReturn( EColorFormat::R8G8B8A8,     MTLPixelFormatBGRA8Unorm );
                ic3CaseReturn( EColorFormat::R8G8B8A8SRGB, MTLPixelFormatBGRA8Unorm_sRGB );
                ic3CaseReturn( EColorFormat::R8G8B8X8,     MTLPixelFormatBGRA8Unorm );
                ic3CaseReturn( EColorFormat::R10G10B10A2,  MTLPixelFormatRGB10A2Unorm );

                default:
                {
                    break;
                }
            }

            return MTLPixelFormatBGRA8Unorm;
        }

    }

}