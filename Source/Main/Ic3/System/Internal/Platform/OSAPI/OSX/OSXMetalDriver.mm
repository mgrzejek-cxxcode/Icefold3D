
#include "OSXMetalDriver.h"
#include "OSXDisplaySystem.h"
#include <Ic3/System/Internal/Platform/Shared/MTL/MTLCommon.h>

namespace Ic3::System
{

	OSXMetalSystemDriver::OSXMetalSystemDriver( OSXDisplayManagerHandle pDisplayManager )
	: OSXNativeObject( std::move( pDisplayManager ) )
	{}

	OSXMetalSystemDriver::~OSXMetalSystemDriver() noexcept = default;

	MetalDisplaySurfaceHandle OSXMetalSystemDriver::_nativeCreateDisplaySurface(
			MetalDevice & pMetalDevice,
			const MetalDisplaySurfaceCreateInfo & pCreateInfo )
	{
        auto displaySurface = createSysObject<OSXMetalDisplaySurface>( getHandle<OSXMetalSystemDriver>() );

        WindowCreateInfo windowCreateInfo;
        windowCreateInfo.frameGeometry = pCreateInfo.frameGeometry;
        windowCreateInfo.title = "TS3 Metal Window";

        Platform::osxCreateWindow( displaySurface->mNativeData, nullptr, windowCreateInfo );
        Platform::osxCreateSurfaceMetalView( displaySurface->mNativeData, pCreateInfo );
        Platform::osxCreateEventListener( displaySurface->mNativeData );
        Platform::osxSetInputWindow( displaySurface->mNativeData );

		displaySurface->mSurfaceData->caMetalLayer = displaySurface->mNativeData.caMetalLayer;

        return displaySurface;
	}

	void OSXMetalSystemDriver::_nativeDestroyDisplaySurface( MetalDisplaySurface & pDisplaySurface )
	{
    }


	OSXMetalDisplaySurface::OSXMetalDisplaySurface( OSXMetalSystemDriverHandle pMTLSystemDriver )
	: OSXNativeObject( std::move( pMTLSystemDriver ), &mNativeData )
	{}

	OSXMetalDisplaySurface::~OSXMetalDisplaySurface() noexcept = default;
	
    FrameSize OSXMetalDisplaySurface::_nativeQueryRenderAreaSize() const
    {
        const auto & drawableSize = [mNativeData.caMetalLayer drawableSize];

        FrameSize result;
        result.x = static_cast<uint32>( drawableSize.width );
        result.y = static_cast<uint32>( drawableSize.height );

        return result;
    }

    bool OSXMetalDisplaySurface::_nativeSysValidate() const
    {
        return mNativeData.caMetalLayer != nil;
    }

    void OSXMetalDisplaySurface::_nativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode )
    {
    }

    void OSXMetalDisplaySurface::_nativeSetFullscreenMode( bool pEnable )
    {
    }

    void OSXMetalDisplaySurface::_nativeSetTitle( const std::string & pTitle )
    {
        Platform::osxSetFrameTitle( mNativeData.nsWindow, pTitle );
    }

    void OSXMetalDisplaySurface::_nativeUpdateGeometry( const FrameGeometry & pFrameGeometry,
                                                        Bitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
    {
    }

    FrameSize OSXMetalDisplaySurface::_nativeGetSize( EFrameSizeMode pSizeMode ) const
    {
        return Platform::osxGetFrameSize( mNativeData.nsWindow, pSizeMode );
    }

    bool OSXMetalDisplaySurface::_nativeIsFullscreen() const
    {
        return false;
    }

    namespace Platform
    {

        void osxCreateSurfaceMetalView( OSXMetalDisplaySurfaceNativeData & pSurfaceNativeData,
		                                const MetalDisplaySurfaceCreateInfo & pCreateInfo )
        {
        @autoreleasepool
        {
            if( ![( id )pSurfaceNativeData.nsWindow isKindOfClass:[NSOSXWindow class]] )
            {
                return;
            }

            auto * nsWindow = static_cast<NSOSXWindow *>( pSurfaceNativeData.nsWindow );

            @try
            {
                auto * nsMetalView = [[NSOSXMetalView alloc] initForWindow:nsWindow];
				auto * caMetalLayer = nsMetalView->mMetalLayer;

	            const auto mtlPixelFormat = Platform::mtlChoosePixelFormatForVisualConfig( pCreateInfo.visualConfig );
	            [caMetalLayer setPixelFormat:mtlPixelFormat];

	            const auto layerRect = [nsWindow contentRectForFrameRect:[nsWindow frame]];
	            [caMetalLayer setDrawableSize:CGSizeMake( layerRect.size.width, layerRect.size.height )];

                pSurfaceNativeData.nsView = nsMetalView;
                pSurfaceNativeData.caMetalLayer = caMetalLayer;
            }
            @catch( NSException * pException )
            {
                const auto message = [[pException reason] UTF8String];
                ic3DebugInterrupt();
            }
        }
        }

    }
	
}
