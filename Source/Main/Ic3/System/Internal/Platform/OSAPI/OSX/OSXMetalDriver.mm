
#include "OSXMetalDriver.h"
#include "OSXDisplaySystem.h"
#include <Ic3/System/Internal/Platform/Shared/MTL/MTLCommon.h>

namespace Ic3::System
{

	OSXMetalSystemDriver::OSXMetalSystemDriver( OSXDisplayManagerHandle pDisplayManager )
	: OSXNativeObject( std::move( pDisplayManager ) )
	{}

	OSXMetalSystemDriver::~OSXMetalSystemDriver() noexcept = default;

	MetalDisplaySurfaceHandle OSXMetalSystemDriver::_NativeCreateDisplaySurface(
			MetalDevice & pMetalDevice,
			const MetalDisplaySurfaceCreateInfo & pCreateInfo )
	{
        auto displaySurface = CreateSysObject<OSXMetalDisplaySurface>( GetHandle<OSXMetalSystemDriver>() );

        WindowCreateInfo windowCreateInfo;
        windowCreateInfo.frameGeometry = pCreateInfo.frameGeometry;
        windowCreateInfo.title = "TS3 Metal Window";

        Platform::OSXCreateWindow( displaySurface->mNativeData, nullptr, windowCreateInfo );
        Platform::OSXCreateSurfaceMetalView( displaySurface->mNativeData, pCreateInfo );
        Platform::OSXCreateEventListener( displaySurface->mNativeData );
        Platform::OSXSetInputWindow( displaySurface->mNativeData );

		displaySurface->surfaceData->caMetalLayer = displaySurface->mNativeData.caMetalLayer;

        return displaySurface;
	}

	void OSXMetalSystemDriver::_NativeDestroyDisplaySurface( MetalDisplaySurface & pDisplaySurface )
	{
    }


	OSXMetalDisplaySurface::OSXMetalDisplaySurface( OSXMetalSystemDriverHandle pMTLSystemDriver )
	: OSXNativeObject( std::move( pMTLSystemDriver ), &mNativeData )
	{}

	OSXMetalDisplaySurface::~OSXMetalDisplaySurface() noexcept = default;
	
    FrameSize OSXMetalDisplaySurface::_NativeQueryRenderAreaSize() const
    {
        const auto & drawableSize = [mNativeData.caMetalLayer drawableSize];

        FrameSize result;
        result.x = static_cast<uint32>( drawableSize.width );
        result.y = static_cast<uint32>( drawableSize.height );

        return result;
    }

    bool OSXMetalDisplaySurface::_NativeSysValidate() const
    {
        return mNativeData.caMetalLayer != nil;
    }

    void OSXMetalDisplaySurface::_NativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode )
    {
    }

    void OSXMetalDisplaySurface::_NativeSetFullscreenMode( bool pEnable )
    {
    }

    void OSXMetalDisplaySurface::_NativeSetTitle( const std::string & pTitle )
    {
        Platform::OSXSetFrameTitle( mNativeData.mNSWindow, pTitle );
    }

    void OSXMetalDisplaySurface::_NativeUpdateGeometry(
			const FrameGeometry & pFrameGeometry,
			cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
    {
    }

    FrameSize OSXMetalDisplaySurface::_NativeGetSize( EFrameSizeMode pSizeMode ) const
    {
        return Platform::OSXGetFrameSize( mNativeData.mNSWindow, pSizeMode );
    }

    bool OSXMetalDisplaySurface::_NativeIsFullscreen() const
    {
        return false;
    }

    namespace Platform
    {

        void OSXCreateSurfaceMetalView(
				OSXMetalDisplaySurfaceNativeData & pSurfaceNativeData,
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

	            const auto mtlPixelFormat = Platform::MTAChoosePixelFormatForVisualConfig( pCreateInfo.visualConfig );
	            [caMetalLayer setPixelFormat:mtlPixelFormat];

	            const auto layerRect = [nsWindow contentRectForFrameRect:[nsWindow frame]];
	            [caMetalLayer setDrawableSize:CGSizeMake( layerRect.size.width, layerRect.size.height )];

                pSurfaceNativeData.mNSView = nsMetalView;
                pSurfaceNativeData.caMetalLayer = caMetalLayer;
            }
            @catch( NSException * pException )
            {
                const auto message = [[pException reason] UTF8String];
                Ic3DebugInterrupt();
            }
        }
        }

    }
	
}
