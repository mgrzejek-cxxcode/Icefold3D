
#include "OSXOpenGLDriver.h"
#include "OSXDisplaySystem.h"
#include "NSIWindow.h"
#include "NSIOpenGL.h"
#include <Ic3/CoreLib/Exception.h>

#ifdef __clang__
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

#define Ic3OSXOpenGLContextAttribAppend( pArray, pIndex, pAttrib ) \
	Ic3DebugAssert( pIndex < CX_OSX_MAX_NSGL_FBCONFIG_ATTRIBUTES_NUM ); \
	pArray[pIndex++] = pAttrib

namespace Ic3::System
{

	namespace Platform
	{

		//
		NSOpenGLPixelFormatAttribute _OSXGetAPIProfileForSurface(
				const OpenGLDisplaySurfaceCreateInfo & pCreateInfo,
				NSOpenGLPixelFormatAttribute pNSMaxSupportedVersion );

		//
		void _OSXGetAttribArrayForVisualConfig(
				const VisualConfig & pVisualConfig,
				NSOpenGLPixelFormatAttribute * pAttribArray );

	}

	OSXOpenGLSystemDriver::OSXOpenGLSystemDriver( OSXDisplayManagerHandle pDisplayManager )
	: OSXNativeObject( std::move( pDisplayManager ) )
	{
		_InitializeOSXDriverState();
	}

	OSXOpenGLSystemDriver::~OSXOpenGLSystemDriver() noexcept
	{
		_ReleaseOSXDriverState();
	}

	void OSXOpenGLSystemDriver::_InitializeOSXDriverState()
	{
		if( Platform::OSXCheckAppKitFrameworkVersion( NSAppKitVersionNumber10_10 ) )
		{
			_nsSupportedOpenGLVersion = NSOpenGLProfileVersion4_1Core;
		}
		else if( Platform::OSXCheckAppKitFrameworkVersion( NSAppKitVersionNumber10_7 ) )
		{
			_nsSupportedOpenGLVersion = NSOpenGLProfileVersion3_2Core;
		}
		else
		{
			_nsSupportedOpenGLVersion = NSOpenGLProfileVersionLegacy;
		}
	}

	void OSXOpenGLSystemDriver::_ReleaseOSXDriverState()
	{
	}

	void OSXOpenGLSystemDriver::_NativeInitializePlatform()
	{
	}

	void OSXOpenGLSystemDriver::_NativeReleaseInitState() noexcept
	{
	}

	OpenGLVersionSupportInfo OSXOpenGLSystemDriver::_NativeQueryVersionSupportInfo() const noexcept
	{
		OpenGLVersionSupportInfo openGLVersionSupportInfo{};

		switch( _nsSupportedOpenGLVersion )
		{
			case NSOpenGLProfileVersion4_1Core:
			{
				openGLVersionSupportInfo.apiVersion.num_major = 4;
				openGLVersionSupportInfo.apiVersion.num_minor = 1;
				break;
			}
			case NSOpenGLProfileVersion3_2Core:
			{
				openGLVersionSupportInfo.apiVersion.num_major = 3;
				openGLVersionSupportInfo.apiVersion.num_minor = 2;
				break;
			}
			default:
			{
				openGLVersionSupportInfo.apiVersion.num_major = 1;
				openGLVersionSupportInfo.apiVersion.num_minor = 0;
				break;
			}
		}

		openGLVersionSupportInfo.apiProfile = EOpenGLAPIProfile::Core;
		openGLVersionSupportInfo.apiClass = EOpenGLAPIClass::Desktop;

		return openGLVersionSupportInfo;
	}

	OpenGLDisplaySurfaceHandle OSXOpenGLSystemDriver::_NativeCreateDisplaySurface(
			const OpenGLDisplaySurfaceCreateInfo & pCreateInfo )
	{
	@autoreleasepool
	{
		NSOpenGLPixelFormatAttribute nsOpenGLPixelFormatAttribArray[Platform::CX_OSX_MAX_NSGL_FBCONFIG_ATTRIBUTES_NUM + 2];
		nsOpenGLPixelFormatAttribArray[0] = NSOpenGLPFAOpenGLProfile;
		nsOpenGLPixelFormatAttribArray[1] = Platform::_OSXGetAPIProfileForSurface( pCreateInfo, _nsSupportedOpenGLVersion );

		Platform::_OSXGetAttribArrayForVisualConfig( pCreateInfo.visualConfig, &( nsOpenGLPixelFormatAttribArray[2] ) );

		auto * nsOpenGLPixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:nsOpenGLPixelFormatAttribArray];

		auto displaySurface = CreateSysObject<OSXOpenGLDisplaySurface>( GetHandle<OSXOpenGLSystemDriver>() );
		displaySurface->mNativeData.mNSPixelFormat = nsOpenGLPixelFormat;

		WindowCreateInfo windowCreateInfo;
		windowCreateInfo.frameGeometry = pCreateInfo.frameGeometry;
		windowCreateInfo.title = "TS3 OpenGL Window";

		Platform::OSXCreateWindow( displaySurface->mNativeData, nullptr, windowCreateInfo );
		Platform::OSXCreateWindowDefaultView( displaySurface->mNativeData );
		Platform::OSXCreateEventListener( displaySurface->mNativeData );
		Platform::OSXSetInputWindow( displaySurface->mNativeData );

		return displaySurface;
	}
	}

	OpenGLDisplaySurfaceHandle OSXOpenGLSystemDriver::_NativeCreateDisplaySurfaceForCurrentThread()
	{
		return nullptr;
	}

	void OSXOpenGLSystemDriver::_NativeDestroyDisplaySurface( OpenGLDisplaySurface & pDisplaySurface )
	{
	}

	OpenGLRenderContextHandle OSXOpenGLSystemDriver::_NativeCreateRenderContext(
			OpenGLDisplaySurface & pDisplaySurface,
			const OpenGLRenderContextCreateInfo & pCreateInfo )
	{
	@autoreleasepool
	{
		auto * osxDisplaySurface = pDisplaySurface.QueryInterface<OSXOpenGLDisplaySurface>();
		auto * nsPixelFormat = osxDisplaySurface->mNativeData.mNSPixelFormat;

		auto * nsContextHandle = [[NSOSXOpenGLContext alloc] initWithFormat:nsPixelFormat shareContext:nil];

		auto renderContext = CreateSysObject<OSXOpenGLRenderContext>( GetHandle<OSXOpenGLSystemDriver>() );
		renderContext->mNativeData.mNSContextHandle = nsContextHandle;

		return renderContext;
	}
	}

	OpenGLRenderContextHandle OSXOpenGLSystemDriver::_NativeCreateRenderContextForCurrentThread()
	{
		return nullptr;
	}

	void OSXOpenGLSystemDriver::_NativeDestroyRenderContext( OpenGLRenderContext & pRenderContext )
	{
	}

	void OSXOpenGLSystemDriver::_NativeResetContextBinding()
	{
	}

	std::vector<EDepthStencilFormat> OSXOpenGLSystemDriver::_NativeQuerySupportedDepthStencilFormats(
			EColorFormat pColorFormat ) const
	{
		return {};
	}

	std::vector<EMSAAMode> OSXOpenGLSystemDriver::_NativeQuerySupportedMSAAModes(
			EColorFormat pColorFormat,
			EDepthStencilFormat pDepthStencilFormat ) const
	{
		return {};
	}

	bool OSXOpenGLSystemDriver::_NativeIsAPIClassSupported( EOpenGLAPIClass pAPIClass ) const
	{
		if( pAPIClass == EOpenGLAPIClass::Desktop )
		{
			return true;
		}
		return false;
	}

	bool OSXOpenGLSystemDriver::_NativeIsRenderContextBound() const
	{
		return false;
	}


	OSXOpenGLDisplaySurface::OSXOpenGLDisplaySurface( OSXOpenGLSystemDriverHandle pGLSystemDriver )
	: OSXNativeObject( std::move( pGLSystemDriver ), &mNativeData )
	{}

	OSXOpenGLDisplaySurface::~OSXOpenGLDisplaySurface() noexcept
	{
	}

	void OSXOpenGLDisplaySurface::_ReleaseOSXSurfaceState()
	{
	}

	void OSXOpenGLDisplaySurface::_NativeSwapBuffers()
	{
	@autoreleasepool
	{
		auto * nsThreadLocalStorage = [NSThread currentThread].threadDictionary;
		auto * nsContextHandle = ( NSOSXOpenGLContext * )nsThreadLocalStorage[@"Ic3OpenGLContext"];

		[nsContextHandle flushBuffer];
		[nsContextHandle updateConditional];
	}
	}

	EOpenGLAPIClass OSXOpenGLDisplaySurface::_NativeQuerySupportedAPIClass() const noexcept
	{
		return EOpenGLAPIClass::Desktop;
	}

	VisualConfig OSXOpenGLDisplaySurface::_NativeQueryVisualConfig() const
	{
		GLint colorBufferSize = 0;
		[mNativeData.mNSPixelFormat getValues:&colorBufferSize forAttribute:NSOpenGLPFAColorSize forVirtualScreen:0];
		GLint alphaBufferSize = 0;
		[mNativeData.mNSPixelFormat getValues:&alphaBufferSize forAttribute:NSOpenGLPFAAlphaSize forVirtualScreen:0];
		GLint depthBufferSize = 0;
		[mNativeData.mNSPixelFormat getValues:&depthBufferSize forAttribute:NSOpenGLPFADepthSize forVirtualScreen:0];
		GLint stencilBufferSize = 0;
		[mNativeData.mNSPixelFormat getValues:&stencilBufferSize forAttribute:NSOpenGLPFAStencilSize forVirtualScreen:0];
		GLint multiSamplingEnabled = 0;
		[mNativeData.mNSPixelFormat getValues:&multiSamplingEnabled forAttribute:NSOpenGLPFAMultisample forVirtualScreen:0];
		GLint sampleBuffersNum = 0;
		[mNativeData.mNSPixelFormat getValues:&sampleBuffersNum forAttribute:NSOpenGLPFASampleBuffers forVirtualScreen:0];
		GLint doubleBufferMode = 0;
		[mNativeData.mNSPixelFormat getValues:&doubleBufferMode forAttribute:NSOpenGLPFADoubleBuffer forVirtualScreen:0];
		GLint tripleBufferMode = 0;
		[mNativeData.mNSPixelFormat getValues:&tripleBufferMode forAttribute:NSOpenGLPFATripleBuffer forVirtualScreen:0];

		VisualConfig visualConfig;

		if( ( colorBufferSize == 32 ) && ( alphaBufferSize == 8 ) )
		{
			visualConfig.colorFormat = EColorFormat::B8G8R8A8;
		}
		else if( ( colorBufferSize == 24 ) && ( alphaBufferSize == 0 ) )
		{
			visualConfig.colorFormat = EColorFormat::B8G8R8;
		}
		else
		{
			visualConfig.colorFormat = EColorFormat::Unknown;
		}

		if( ( depthBufferSize == 24 ) && ( stencilBufferSize == 8 ) )
		{
			visualConfig.depthStencilFormat = EDepthStencilFormat::D24S8;
		}
		else if( ( depthBufferSize == 24 ) && ( stencilBufferSize == 0 ) )
		{
			visualConfig.depthStencilFormat = EDepthStencilFormat::D24X8;
		}
		else if( ( depthBufferSize == 32 ) && ( stencilBufferSize == 0 ) )
		{
			visualConfig.depthStencilFormat = EDepthStencilFormat::D32F;
		}
		else if( ( depthBufferSize == 32 ) && ( stencilBufferSize == 8 ) )
		{
			visualConfig.depthStencilFormat = EDepthStencilFormat::D32FS8;
		}
		else
		{
			visualConfig.depthStencilFormat = EDepthStencilFormat::Unknown;
		}

		if( multiSamplingEnabled )
		{
			visualConfig.msaaDesc.bufferCount = sampleBuffersNum;
			visualConfig.msaaDesc.quality = 1;
		}
		else
		{
			visualConfig.msaaDesc.bufferCount = 0;
			visualConfig.msaaDesc.quality = 0;
		}

		if( tripleBufferMode )
		{
			visualConfig.flags.set( eVisualAttribFlagTripleBufferBit );
		}
		else if( doubleBufferMode )
		{
			visualConfig.flags.set( eVisualAttribFlagDoubleBufferBit );
		}
		else
		{
			visualConfig.flags.set( eVisualAttribFlagSingleBufferBit );
		}

		return visualConfig;
	}

	FrameSize OSXOpenGLDisplaySurface::_NativeQueryRenderAreaSize() const
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

	bool OSXOpenGLDisplaySurface::_NativeSysValidate() const
	{
		return true;
	}

	void OSXOpenGLDisplaySurface::_NativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode )
	{
	}

	void OSXOpenGLDisplaySurface::_NativeSetFullscreenMode( bool pEnable )
	{
	}

	void OSXOpenGLDisplaySurface::_NativeSetTitle( const std::string & pTitle )
	{
        Platform::OSXSetFrameTitle( mNativeData.mNSWindow, pTitle );
	}

	void OSXOpenGLDisplaySurface::_NativeUpdateGeometry(
			const FrameGeometry & pFrameGeometry,
			cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
	{
	}

	FrameSize OSXOpenGLDisplaySurface::_NativeGetSize( EFrameSizeMode pSizeMode ) const
	{
	@autoreleasepool
	{
		const auto windowFrame = [mNativeData.mNSWindow frame];
		return { windowFrame.size.width, windowFrame.size.height };
	}
	}

	bool OSXOpenGLDisplaySurface::_NativeIsFullscreen() const
	{
		return false;
	}


	OSXOpenGLRenderContext::OSXOpenGLRenderContext( OSXOpenGLSystemDriverHandle pGLSystemDriver )
	: OSXNativeObject( std::move( pGLSystemDriver ) )
	{}

	OSXOpenGLRenderContext::~OSXOpenGLRenderContext() noexcept
	{
	}

	void OSXOpenGLRenderContext::_NativeBindForCurrentThread( const OpenGLDisplaySurface & pTargetSurface )
	{
	@autoreleasepool
	{
		auto * osxDisplaySurface = pTargetSurface.QueryInterface<OSXOpenGLDisplaySurface>();
		auto * nsTargetView = static_cast<NSView *>( osxDisplaySurface->mNativeData.mNSView );

		auto * nsThreadLocalStorage = [NSThread currentThread].threadDictionary;
		nsThreadLocalStorage[@"Ic3OpenGLContext"] = mNativeData.mNSContextHandle;

		[mNativeData.mNSContextHandle setView:nsTargetView];
		[mNativeData.mNSContextHandle makeCurrentContext];
	}
	}

	bool OSXOpenGLRenderContext::_NativeSysCheckIsCurrent() const
	{
		return false;
	}

	bool OSXOpenGLRenderContext::_NativeSysValidate() const
	{
		return mNativeData.mNSContextHandle != nullptr;
	}

	void OSXOpenGLRenderContext::_ReleaseOSXContextState()
	{
	}

	namespace Platform
	{

		NSOpenGLPixelFormatAttribute _OSXGetAPIProfileForSurface(
				const OpenGLDisplaySurfaceCreateInfo & pCreateInfo,
				NSOpenGLPixelFormatAttribute pNSMaxSupportedVersion )
		{
			NSOpenGLPixelFormatAttribute nsOpenGLProfile = 0;

			if((pCreateInfo.targetAPIClass == EOpenGLAPIClass::Desktop ) && (pCreateInfo.minimumAPIVersion <= cppx::version{4, 1 } ) )
			{
				if( ( pCreateInfo.minimumAPIVersion > cppx::version{3, 2 } ) && ( pNSMaxSupportedVersion == NSOpenGLProfileVersion4_1Core ) )
				{
					nsOpenGLProfile = NSOpenGLProfileVersion4_1Core;
				}
				else if( ( pCreateInfo.minimumAPIVersion >= cppx::version{3, 0 } ) && ( pNSMaxSupportedVersion == NSOpenGLProfileVersion3_2Core ) )
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

		void _OSXGetAttribArrayForVisualConfig(
				const VisualConfig & pVisualConfig,
				NSOpenGLPixelFormatAttribute * pAttribArray )
		{
			int attribIndex = 0;

			Ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, NSOpenGLPFAAccelerated );
			Ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, NSOpenGLPFAAllowOfflineRenderers );

			Ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, NSOpenGLPFAColorSize );
			Ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, pVisualConfig.colorDesc.size );

			if( pVisualConfig.flags.is_set( eVisualAttribFlagTripleBufferBit ) )
			{
				Ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, NSOpenGLPFATripleBuffer );
			}
			else if( pVisualConfig.flags.is_set( eVisualAttribFlagDoubleBufferBit ) )
			{
				Ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, NSOpenGLPFADoubleBuffer );
			}

			if( pVisualConfig.flags.is_set( eVisualAttribFlagStereoDisplayBit ) )
			{
				Ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, NSOpenGLPFAStereo );
			}

			if( ( pVisualConfig.msaaDesc.bufferCount != 0 ) && ( pVisualConfig.msaaDesc.quality != 0 ) )
			{
				Ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, NSOpenGLPFASampleBuffers );
				Ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, pVisualConfig.msaaDesc.bufferCount );
				Ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, NSOpenGLPFASamples );
				Ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, pVisualConfig.msaaDesc.quality );
				Ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, NSOpenGLPFANoRecovery );
			}

			if( pVisualConfig.depthStencilDesc.depthBufferSize != 0 )
			{
				Ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, NSOpenGLPFADepthSize );
				Ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, pVisualConfig.depthStencilDesc.depthBufferSize );
			}

			if( pVisualConfig.depthStencilDesc.stencilBufferSize != 0 )
			{
				Ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, NSOpenGLPFAStencilSize );
				Ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, pVisualConfig.depthStencilDesc.stencilBufferSize );
			}

			Ic3OSXOpenGLContextAttribAppend( pAttribArray, attribIndex, 0 );
		}

	}

} // namespace Ic3::System

#ifdef __clang__
#  pragma clang diagnostic pop
#endif
