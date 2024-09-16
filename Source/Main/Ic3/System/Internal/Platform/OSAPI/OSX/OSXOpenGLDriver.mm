
#include "OSXOpenGLDriver.h"
#include "OSXDisplaySystem.h"
#include "NSIWindow.h"
#include "NSIOpenGL.h"
#include <Ic3/CoreLib/Exception.h>

#ifdef __clang__
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

#define ic3OSXOpenGLContextAttribAppend( pArray, pIndex, pAttrib ) \
	ic3DebugAssert( pIndex < CX_OSX_MAX_NSGL_FBCONFIG_ATTRIBUTES_NUM ); \
	pArray[pIndex++] = pAttrib

namespace Ic3::System
{

	namespace Platform
	{

		//
		NSOpenGLPixelFormatAttribute _osxGetAPIProfileForSurface( const OpenGLDisplaySurfaceCreateInfo & pCreateInfo,
		                                                          NSOpenGLPixelFormatAttribute pNSMaxSupportedVersion );

		//
		void _osxGetAttribArrayForVisualConfig( const VisualConfig & pVisualConfig, NSOpenGLPixelFormatAttribute * pAttribArray );

	}

	OSXOpenGLSystemDriver::OSXOpenGLSystemDriver( OSXDisplayManagerHandle pDisplayManager )
	: OSXNativeObject( std::move( pDisplayManager ) )
	{
		_initializeOSXDriverState();
	}

	OSXOpenGLSystemDriver::~OSXOpenGLSystemDriver() noexcept
	{
		_releaseOSXDriverState();
	}

	void OSXOpenGLSystemDriver::_initializeOSXDriverState()
	{
		if( Platform::osxCheckAppKitFrameworkVersion( NSAppKitVersionNumber10_10 ) )
		{
			_nsSupportedOpenGLVersion = NSOpenGLProfileVersion4_1Core;
		}
		else if( Platform::osxCheckAppKitFrameworkVersion( NSAppKitVersionNumber10_7 ) )
		{
			_nsSupportedOpenGLVersion = NSOpenGLProfileVersion3_2Core;
		}
		else
		{
			_nsSupportedOpenGLVersion = NSOpenGLProfileVersionLegacy;
		}
	}

	void OSXOpenGLSystemDriver::_releaseOSXDriverState()
	{
	}

	void OSXOpenGLSystemDriver::_nativeInitializePlatform()
	{
	}

	void OSXOpenGLSystemDriver::_nativeReleaseInitState() noexcept
	{
	}

	OpenGLVersionSupportInfo OSXOpenGLSystemDriver::_nativeQueryVersionSupportInfo() const noexcept
	{
		OpenGLVersionSupportInfo openGLVersionSupportInfo{};

		switch( _nsSupportedOpenGLVersion )
		{
			case NSOpenGLProfileVersion4_1Core:
			{
				openGLVersionSupportInfo.mAPIVersion.mNumMajor = 4;
				openGLVersionSupportInfo.mAPIVersion.mNumMinor = 1;
				break;
			}
			case NSOpenGLProfileVersion3_2Core:
			{
				openGLVersionSupportInfo.mAPIVersion.mNumMajor = 3;
				openGLVersionSupportInfo.mAPIVersion.mNumMinor = 2;
				break;
			}
			default:
			{
				openGLVersionSupportInfo.mAPIVersion.mNumMajor = 1;
				openGLVersionSupportInfo.mAPIVersion.mNumMinor = 0;
				break;
			}
		}

		openGLVersionSupportInfo.mAPIProfile = EOpenGLAPIProfile::Core;
		openGLVersionSupportInfo.mAPIClass = EOpenGLAPIClass::Desktop;

		return openGLVersionSupportInfo;
	}

	OpenGLDisplaySurfaceHandle OSXOpenGLSystemDriver::_nativeCreateDisplaySurface( const OpenGLDisplaySurfaceCreateInfo & pCreateInfo )
	{
	@autoreleasepool
	{
		NSOpenGLPixelFormatAttribute nsOpenGLPixelFormatAttribArray[Platform::CX_OSX_MAX_NSGL_FBCONFIG_ATTRIBUTES_NUM + 2];
		nsOpenGLPixelFormatAttribArray[0] = NSOpenGLPFAOpenGLProfile;
		nsOpenGLPixelFormatAttribArray[1] = Platform::_osxGetAPIProfileForSurface( pCreateInfo, _nsSupportedOpenGLVersion );

		Platform::_osxGetAttribArrayForVisualConfig( pCreateInfo.mVisualConfig, &( nsOpenGLPixelFormatAttribArray[2] ) );

		auto * nsOpenGLPixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:nsOpenGLPixelFormatAttribArray];

		auto displaySurface = createSysObject<OSXOpenGLDisplaySurface>( getHandle<OSXOpenGLSystemDriver>() );
		displaySurface->mNativeData.mNSPixelFormat = nsOpenGLPixelFormat;

		WindowCreateInfo windowCreateInfo;
		windowCreateInfo.mFrameGeometry = pCreateInfo.mFrameGeometry;
		windowCreateInfo.mTitle = "TS3 OpenGL Window";

		Platform::osxCreateWindow( displaySurface->mNativeData, nullptr, windowCreateInfo );
		Platform::osxCreateWindowDefaultView( displaySurface->mNativeData );
		Platform::osxCreateEventListener( displaySurface->mNativeData );
		Platform::osxSetInputWindow( displaySurface->mNativeData );

		return displaySurface;
	}
	}

	OpenGLDisplaySurfaceHandle OSXOpenGLSystemDriver::_nativeCreateDisplaySurfaceForCurrentThread()
	{
		return nullptr;
	}

	void OSXOpenGLSystemDriver::_nativeDestroyDisplaySurface( OpenGLDisplaySurface & pDisplaySurface )
	{
	}

	OpenGLRenderContextHandle OSXOpenGLSystemDriver::_nativeCreateRenderContext( OpenGLDisplaySurface & pDisplaySurface,
	                                                                             const OpenGLRenderContextCreateInfo & pCreateInfo )
	{
	@autoreleasepool
	{
		auto * osxDisplaySurface = pDisplaySurface.queryInterface<OSXOpenGLDisplaySurface>();
		auto * nsPixelFormat = osxDisplaySurface->mNativeData.mNSPixelFormat;

		auto * nsContextHandle = [[NSOSXOpenGLContext alloc] initWithFormat:nsPixelFormat shareContext:nil];

		auto renderContext = createSysObject<OSXOpenGLRenderContext>( getHandle<OSXOpenGLSystemDriver>() );
		renderContext->mNativeData.mNSContextHandle = nsContextHandle;

		return renderContext;
	}
	}

	OpenGLRenderContextHandle OSXOpenGLSystemDriver::_nativeCreateRenderContextForCurrentThread()
	{
		return nullptr;
	}

	void OSXOpenGLSystemDriver::_nativeDestroyRenderContext( OpenGLRenderContext & pRenderContext )
	{
	}

	void OSXOpenGLSystemDriver::_nativeResetContextBinding()
	{
	}

	std::vector<EDepthStencilFormat> OSXOpenGLSystemDriver::_nativeQuerySupportedDepthStencilFormats( EColorFormat pColorFormat ) const
	{
		return {};
	}

	std::vector<EMSAAMode> OSXOpenGLSystemDriver::_nativeQuerySupportedMSAAModes( EColorFormat pColorFormat,
	                                                                              EDepthStencilFormat pDepthStencilFormat ) const
	{
		return {};
	}

	bool OSXOpenGLSystemDriver::_nativeIsAPIClassSupported( EOpenGLAPIClass pAPIClass ) const
	{
		if( pAPIClass == EOpenGLAPIClass::Desktop )
		{
			return true;
		}
		return false;
	}

	bool OSXOpenGLSystemDriver::_nativeIsRenderContextBound() const
	{
		return false;
	}


	OSXOpenGLDisplaySurface::OSXOpenGLDisplaySurface( OSXOpenGLSystemDriverHandle pGLSystemDriver )
	: OSXNativeObject( std::move( pGLSystemDriver ), &mNativeData )
	{}

	OSXOpenGLDisplaySurface::~OSXOpenGLDisplaySurface() noexcept
	{
	}

	void OSXOpenGLDisplaySurface::_releaseOSXSurfaceState()
	{
	}

	void OSXOpenGLDisplaySurface::_nativeSwapBuffers()
	{
	@autoreleasepool
	{
		auto * nsThreadLocalStorage = [NSThread currentThread].threadDictionary;
		auto * nsContextHandle = ( NSOSXOpenGLContext * )nsThreadLocalStorage[@"ic3OpenGLContext"];

		[nsContextHandle flushBuffer];
		[nsContextHandle updateConditional];
	}
	}

	EOpenGLAPIClass OSXOpenGLDisplaySurface::_nativeQuerySupportedAPIClass() const noexcept
	{
		return EOpenGLAPIClass::Desktop;
	}

	VisualConfig OSXOpenGLDisplaySurface::_nativeQueryVisualConfig() const
	{
		return {};
	}

	FrameSize OSXOpenGLDisplaySurface::_nativeQueryRenderAreaSize() const
	{
	@autoreleasepool
	{
		// const auto windowFrame = [mNativeData.nsWindow frame];
		// const auto windowStyle = [mNativeData.nsWindow styleMask];
		// const auto surfaceSize = [NSWindow contentRectForFrameRect:windowFrame
		//                                                  styleMask:windowStyle];

		const NSRect contentRect = [mNativeData.mNSWindow frame];
		const NSRect framebufferRect = [mNativeData.mNSWindow convertRectToBacking:contentRect];

		return { framebufferRect.size.width, framebufferRect.size.height };
	}
	}

	bool OSXOpenGLDisplaySurface::_nativeSysValidate() const
	{
		return true;
	}

	void OSXOpenGLDisplaySurface::_nativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode )
	{
	}

	void OSXOpenGLDisplaySurface::_nativeSetFullscreenMode( bool pEnable )
	{
	}

	void OSXOpenGLDisplaySurface::_nativeSetTitle( const std::string & pTitle )
	{
        Platform::osxSetFrameTitle( mNativeData.mNSWindow, pTitle );
	}

	void OSXOpenGLDisplaySurface::_nativeUpdateGeometry( const FrameGeometry & pFrameGeometry,
	                                                     TBitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
	{
	}

	FrameSize OSXOpenGLDisplaySurface::_nativeGetSize( EFrameSizeMode pSizeMode ) const
	{
	@autoreleasepool
	{
		const auto windowFrame = [mNativeData.mNSWindow frame];
		return { windowFrame.size.width, windowFrame.size.height };
	}
	}

	bool OSXOpenGLDisplaySurface::_nativeIsFullscreen() const
	{
		return false;
	}


	OSXOpenGLRenderContext::OSXOpenGLRenderContext( OSXOpenGLSystemDriverHandle pGLSystemDriver )
	: OSXNativeObject( std::move( pGLSystemDriver ) )
	{}

	OSXOpenGLRenderContext::~OSXOpenGLRenderContext() noexcept
	{
	}

	void OSXOpenGLRenderContext::_nativeBindForCurrentThread( const OpenGLDisplaySurface & pTargetSurface )
	{
	@autoreleasepool
	{
		auto * osxDisplaySurface = pTargetSurface.queryInterface<OSXOpenGLDisplaySurface>();
		auto * nsTargetView = static_cast<NSView *>( osxDisplaySurface->mNativeData.mNSView );

		auto * nsThreadLocalStorage = [NSThread currentThread].threadDictionary;
		nsThreadLocalStorage[@"ic3OpenGLContext"] = mNativeData.mNSContextHandle;

		[mNativeData.mNSContextHandle setView:nsTargetView];
		[mNativeData.mNSContextHandle makeCurrentContext];
	}
	}

	bool OSXOpenGLRenderContext::_nativeSysCheckIsCurrent() const
	{
		return false;
	}

	bool OSXOpenGLRenderContext::_nativeSysValidate() const
	{
		return mNativeData.mNSContextHandle != nullptr;
	}

	void OSXOpenGLRenderContext::_releaseOSXContextState()
	{
	}

	namespace Platform
	{

		NSOpenGLPixelFormatAttribute _osxGetAPIProfileForSurface( const OpenGLDisplaySurfaceCreateInfo & pCreateInfo,
		                                                          NSOpenGLPixelFormatAttribute pNSMaxSupportedVersion )
		{
			NSOpenGLPixelFormatAttribute nsOpenGLProfile = 0;

			if((pCreateInfo.mTargetAPIClass == EOpenGLAPIClass::Desktop ) && (pCreateInfo.mMinimumAPIVersion <= Version{4, 1 } ) )
			{
				if((pCreateInfo.mMinimumAPIVersion > Version{3, 2 } ) && (pNSMaxSupportedVersion == NSOpenGLProfileVersion4_1Core ) )
				{
					nsOpenGLProfile = NSOpenGLProfileVersion4_1Core;
				}
				else if((pCreateInfo.mMinimumAPIVersion >= Version{3, 0 } ) && (pNSMaxSupportedVersion == NSOpenGLProfileVersion3_2Core ) )
				{
					nsOpenGLProfile = NSOpenGLProfileVersion3_2Core;
				}
				else
				{
					nsOpenGLProfile = NSOpenGLProfileVersionLegacy;
				}
			}

			return nsOpenGLProfile;
		}

		void _osxGetAttribArrayForVisualConfig( const VisualConfig & pVisualConfig, NSOpenGLPixelFormatAttribute * pAttribArray )
		{
			int attribIndex = 0;

			ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, NSOpenGLPFAAccelerated );
			ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, NSOpenGLPFAAllowOfflineRenderers );

			ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, NSOpenGLPFAColorSize );
			ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, pVisualConfig.mColorDesc.mSize );

			if( pVisualConfig.mFlags.isSet( eVisualAttribFlagTripleBufferBit ) )
			{
				ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, NSOpenGLPFATripleBuffer );
			}
			else if( pVisualConfig.mFlags.isSet( eVisualAttribFlagDoubleBufferBit ) )
			{
				ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, NSOpenGLPFADoubleBuffer );
			}

			if( pVisualConfig.mFlags.isSet( eVisualAttribFlagStereoDisplayBit ) )
			{
				ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, NSOpenGLPFAStereo );
			}

			if((pVisualConfig.mMSAADesc.mBufferCount != 0 ) && (pVisualConfig.mMSAADesc.mQuality != 0 ) )
			{
				ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, NSOpenGLPFASampleBuffers );
				ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, pVisualConfig.mMSAADesc.mBufferCount );
				ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, NSOpenGLPFASamples );
				ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, pVisualConfig.mMSAADesc.mQuality );
				ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, NSOpenGLPFANoRecovery );
			}

			if( pVisualConfig.mDepthStencilDesc.mDepthBufferSize != 0 )
			{
				ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, NSOpenGLPFADepthSize );
				ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, pVisualConfig.mDepthStencilDesc.mDepthBufferSize );
			}

			if( pVisualConfig.mDepthStencilDesc.mStencilBufferSize != 0 )
			{
				ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, NSOpenGLPFAStencilSize );
				ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, pVisualConfig.mDepthStencilDesc.mStencilBufferSize );
			}

			ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, 0 );
		}

	}

} // namespace Ic3::System

#ifdef __clang__
#  pragma clang diagnostic pop
#endif
