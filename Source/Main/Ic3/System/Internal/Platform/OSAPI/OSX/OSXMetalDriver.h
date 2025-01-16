
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_OSX_Metal_DRIVER_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_OSX_Metal_DRIVER_H__

#include "OSXWindowSystem.h"
#include "NSIMetalSupport.h"
#include <Ic3/System/MetalNative.h>

#import <Metal/MTLDevice.h>

namespace Ic3::System
{

	ic3SysDeclareHandle( OSXMetalSystemDriver );
	ic3SysDeclareHandle( OSXMetalRenderContext );

	namespace Platform
	{

		struct OSXMetalDisplaySurfaceNativeData : public OSXWindowNativeData
		{
            CAMetalLayer * caMetalLayer = nil;
		};

		struct OSXMetalRenderContextNativeData : public OSXNativeDataCommon
		{
		};

		struct OSXMetalSystemDriverNativeData : public OSXNativeDataCommon
		{
		};

        void OSXCreateSurfaceMetalView(
				OSXMetalDisplaySurfaceNativeData & pSurfaceNativeData,
				const MetalDisplaySurfaceCreateInfo & pCreateInfo );

	}

	/// @brief OSX-specific implementation of the MetalSystemDriver class.
	class OSXMetalSystemDriver : public OSXNativeObject<MetalSystemDriver, Platform::OSXMetalSystemDriverNativeData>
	{
	public:
		OSXMetalSystemDriver( OSXDisplayManagerHandle pDisplayManager );
		virtual ~OSXMetalSystemDriver() noexcept;
		
	private:
		virtual MetalDisplaySurfaceHandle _NativeCreateDisplaySurface(
				MetalDevice & pMetalDevice,
				const MetalDisplaySurfaceCreateInfo & pCreateInfo ) override final;

		virtual void _NativeDestroyDisplaySurface( MetalDisplaySurface & pDisplaySurface ) override final;
	};

    /// @brief OSX-specific implementation of the MetalDisplaySurface class.
    class OSXMetalDisplaySurface : public OSXNativeObject<MetalDisplaySurface, Platform::OSXMetalDisplaySurfaceNativeData>
    {
    public:
        explicit OSXMetalDisplaySurface( OSXMetalSystemDriverHandle pGLSystemDriver );
        virtual ~OSXMetalDisplaySurface() noexcept;

    private:
	    /// @copybrief MetalDisplaySurface::_NativeQueryRenderAreaSize
        virtual FrameSize _NativeQueryRenderAreaSize() const override final;

        /// @copybrief MetalDisplaySurface::_NativeSysValidate
        virtual bool _NativeSysValidate() const override final;

        /// @copybrief MetalDisplaySurface::_NativeResize
        virtual void _NativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode ) override final;

        /// @copybrief MetalDisplaySurface::_NativeSetFullscreenMode
        virtual void _NativeSetFullscreenMode( bool pEnable ) override final;

        /// @copybrief MetalDisplaySurface::_NativeSetTitle
        virtual void _NativeSetTitle( const std::string & pTitle ) override final;

        /// @copybrief MetalDisplaySurface::_NativeUpdateGeometry
        virtual void _NativeUpdateGeometry(
				const FrameGeometry & pFrameGeometry,
				cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags ) override final;

        /// @copybrief MetalDisplaySurface::_NativeGetSize
        virtual FrameSize _NativeGetSize( EFrameSizeMode pSizeMode ) const override final;

        /// @copybrief MetalDisplaySurface::_NativeIsFullscreen
        virtual bool _NativeIsFullscreen() const override final;
    };

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_OSX_Metal_DRIVER_H__
