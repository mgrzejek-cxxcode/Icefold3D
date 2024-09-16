
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
        windowCreateInfo.mFrameGeometry = pCreateInfo.mFrameGeometry;
        windowCreateInfo.mTitle = "TS3 Metal Window";

        Platform::osxCreateWindow( displaySurface->mNativeData, nullptr, windowCreateInfo );
        Platform::osxCreateSurfaceMetalView( displaySurface->mNativeData, pCreateInfo );
        Platform::osxCreateEventListener( displaySurface->mNativeData );
        Platform::osxSetInputWindow( displaySurface->mNativeData );

		displaySurface->mSurfaceData->mCAMetalLayer = displaySurface->mNativeData.mCAMetalLayer;

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
        const auto & drawableSize = [mNativeData.mCAMetalLayer drawableSize];

        FrameSize result;
        result.x = static_cast<uint32>( drawableSize.width );
        result.y = static_cast<uint32>( drawableSize.height );

        return result;
    }

    bool OSXMetalDisplaySurface::_nativeSysValidate() const
    {
        return mNativeData.mCAMetalLayer != nil;
    }

    void OSXMetalDisplaySurface::_nativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode )
    {
    }

    void OSXMetalDisplaySurface::_nativeSetFullscreenMode( bool pEnable )
    {
    }

    void OSXMetalDisplaySurface::_nativeSetTitle( const std::string & pTitle )
    {
        Platform::osxSetFrameTitle( mNativeData.mNSWindow, pTitle );
    }

    void OSXMetalDisplaySurface::_nativeUpdateGeometry( const FrameGeometry & pFrameGeometry,
                                                        TBitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
    {
    }

    FrameSize OSXMetalDisplaySurface::_nativeGetSize( EFrameSizeMode pSizeMode ) const
    {
        return Platform::osxGetFrameSize( mNativeData.mNSWindow, pSizeMode );
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
            if( ![( id )pSurfaceNativeData.mNSWindow isKindOfClass:[NSOSXWindow class]] )
            {
                return;
            }

            auto * nsWindow = static_cast<NSOSXWindow *>( pSurfaceNativeData.mNSWindow );

            @try
            {
                auto * nsMetalView = [[NSOSXMetalView alloc] initForWindow:nsWindow];
				auto * caMetalLayer = nsMetalView->mMetalLayer;

	            const auto mtlPixelFormat = Platform::mtlChoosePixelFormatForVisualConfig( pCreateInfo.mVisualConfig );
	            [caMetalLayer setPixelFormat:mtlPixelFormat];

	            const auto layerRect = [nsWindow contentRectForFrameRect:[nsWindow frame]];
	            [caMetalLayer setDrawableSize:CGSizeMake( layerRect.size.width, layerRect.size.height )];

                pSurfaceNativeData.mNSView = nsMetalView;
                pSurfaceNativeData.mCAMetalLayer = caMetalLayer;
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
