
#include "MTLCommon.h"

namespace Ic3::System
{

    namespace Platform
    {

        MTLPixelFormat MTAChoosePixelFormatForVisualConfig( const VisualConfig & pVisualConfig )
        {
            switch( pVisualConfig.colorFormat )
            {
                Ic3CaseReturn( EColorFormat::B8G8R8,       MTLPixelFormatBGRA8Unorm );
                Ic3CaseReturn( EColorFormat::B8G8R8A8,     MTLPixelFormatBGRA8Unorm );
                Ic3CaseReturn( EColorFormat::B8G8R8A8SRGB, MTLPixelFormatBGRA8Unorm_sRGB );
                Ic3CaseReturn( EColorFormat::B8G8R8X8,     MTLPixelFormatBGRA8Unorm );
                Ic3CaseReturn( EColorFormat::R8G8B8A8,     MTLPixelFormatBGRA8Unorm );
                Ic3CaseReturn( EColorFormat::R8G8B8A8SRGB, MTLPixelFormatBGRA8Unorm_sRGB );
                Ic3CaseReturn( EColorFormat::R8G8B8X8,     MTLPixelFormatBGRA8Unorm );
                Ic3CaseReturn( EColorFormat::R10G10B10A2,  MTLPixelFormatRGB10A2Unorm );

                default:
                {
                    break;
                }
            }

            return MTLPixelFormatBGRA8Unorm;
        }

    }

}