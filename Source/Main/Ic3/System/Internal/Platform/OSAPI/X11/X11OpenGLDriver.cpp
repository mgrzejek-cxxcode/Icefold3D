
#include "X11OpenGLDriver.h"
#include "X11DisplaySystem.h"

#define Ic3X11OpenGLContextAttribAppend( pArray, pIndex, pAttrib ) \
	Ic3DebugAssert( pIndex < CX_X11_MAX_GLX_FBCONFIG_ATTRIBUTES_NUM ); \
	pArray[pIndex++] = pAttrib

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_X11 )
namespace Ic3::System
{

	namespace Platform
	{

		PFNGLXCREATECONTEXTATTRIBSARBPROC _X11QueryGLXCreateContextAttribsProc();

		PFNGLXSWAPINTERVALEXTPROC _X11QueryGLXSwapIntervalProc();

		// Creates X11 OpenGL surface using provided window create attributes and visual config.
		void _x11CreateGLWindowAndSurface( X11OpenGLDisplaySurfaceNativeData & pGLSurfaceNativeData,
		                                   X11WindowCreateInfo & pWindowCreateInfo,
		                                   const VisualConfig & pVisualConfig );

		// Destroys existing surface and corresponding window.
		void _x11DestroyGLWindowAndSurface( X11OpenGLDisplaySurfaceNativeData & pGLSurfaceNativeData );

		// Destroys existing context.
		void _x11DestroyGLContext( X11OpenGLRenderContextNativeData & pGLContextNativeData );

		// Destroys existing surface and corresponding window.
		void _x11CreateAndBindLegacyRenderContext( X11OpenGLRenderContextNativeData & pGLContextNativeData,
		                                           const X11OpenGLDisplaySurfaceNativeData & pGLSurfaceNativeData );

		// Selects matching FBConfig for a GL surface. Format is selected using current system's configuration. Uses legacy API.
		X11OpenGLVisualConfig _x11ChooseLegacyGLFBConfig( XDisplay pDisplay, int pScreenIndex );

		// Selects matching pixel format for surface described with a VisualConfig. Uses new EXT API and supports stuff like MSAA.
		X11OpenGLVisualConfig _x11ChooseCoreGLFBConfig( XDisplay pDisplay, int pScreenIndex, const VisualConfig & pVisualConfig );

		// Returns an array of FBConfigs matching specified VisualConfig definition.
		GLXFBConfigArray _X11QueryCompatibleFBConfigList( XDisplay pDisplay,
		                                                  int pScreenIndex,
		                                                  const VisualConfig & pVisualConfig );

		// Computes a "compatibility rate", i.e. how much the specified FBConfig matches the visual.
		int _X11GetFBConfigMatchRate( XDisplay pDisplay, GLXFBConfig pFBConfig, const VisualConfig & pVisualConfig );

		// Translation: VisualConfig --> array of GLX_* attributes required by the system API.
		// Used for surface/context creation.
		void _X11GetAttribArrayForVisualConfig( const VisualConfig & pVisualConfig, int * pAttribArray );

	}


	X11OpenGLSystemDriver::X11OpenGLSystemDriver( X11DisplayManagerHandle pDisplayManager )
	: X11NativeObject( std::move( pDisplayManager ) )
	{}

	X11OpenGLSystemDriver::~X11OpenGLSystemDriver() noexcept
	{
		_NativeReleaseInitState();
	}

	void X11OpenGLSystemDriver::_NativeInitializePlatform()
	{
		Ic3DebugAssert( !mNativeData.initState );
		// Init state should be first created here and destroyed as soon as proper GL
		// contexts are created (this is not enforce, though, and controlled explicitly
		// by the user and done by calling ReleaseInitState() method od the driver).
		mNativeData.initState = std::make_unique<Platform::X11OpenGLSystemDriverNativeData::InitState>();

		auto & xSessionData = Platform::X11GetXSessionData( *this );

		int glxVersionMajor = 0;
		int glxVersionMinor = 0;

		::glXQueryVersion( xSessionData.displayHandle, &glxVersionMajor, &glxVersionMinor );
		// We need at least version 1.3 of the GLX runtime (that's rather a pretty old one...).
		if( ( glxVersionMajor <= 0 ) || ( ( glxVersionMajor == 1 ) && ( glxVersionMinor < 3 ) ) )
		{
			Ic3Throw( eExcCodeDebugPlaceholder );
		}

		VisualConfig legacyVisualConfig;
		legacyVisualConfig = VisGetDefaultVisualConfigForSysWindow();
		legacyVisualConfig.flags.set( eVisualAttribFlagLegacyBit );

		Platform::X11WindowCreateInfo windowCreateInfo;
		windowCreateInfo.frameGeometry.mPosition = { 0, 0 };
		windowCreateInfo.frameGeometry.size = { 600, 600 };
		windowCreateInfo.frameGeometry.mStyle = EFrameStyle::Overlay;

		auto & tmpSurfaceNativeData = mNativeData.initState->surfaceData;
		tmpSurfaceNativeData.SetSessionData( xSessionData );
		Platform::_x11CreateGLWindowAndSurface( tmpSurfaceNativeData, windowCreateInfo, legacyVisualConfig );

		auto & tmpContextNativeData = mNativeData.initState->contextData;
		tmpContextNativeData.SetSessionData( xSessionData );
		Platform::_x11CreateAndBindLegacyRenderContext( tmpContextNativeData, tmpSurfaceNativeData );
	}

	void X11OpenGLSystemDriver::_NativeReleaseInitState() noexcept
	{
		if( !mNativeData.initState )
		{
			return;
		}

		auto & xSessionData = Platform::X11GetXSessionData( *this );

		auto & tmpSurfaceNativeData = mNativeData.initState->surfaceData;
		auto & tmpContextNativeData = mNativeData.initState->contextData;

		if( tmpContextNativeData.contextHandle != nullptr )
		{
			::glXDestroyContext( xSessionData.displayHandle, tmpContextNativeData.contextHandle );
			tmpContextNativeData.contextHandle = nullptr;
			tmpContextNativeData.ResetSessionData();
		}

		if( tmpSurfaceNativeData.mWindowXID != Platform::eXIDNone )
		{
			Platform::_x11DestroyGLWindowAndSurface( tmpSurfaceNativeData );
			tmpSurfaceNativeData.mFBConfig = nullptr;
			tmpSurfaceNativeData.ResetSessionData();
		}

		if( tmpSurfaceNativeData.mVisualInfo != nullptr )
		{
			XFree( tmpSurfaceNativeData.mVisualInfo );
			tmpSurfaceNativeData.mVisualInfo = nullptr;
		}

		mNativeData.initState.Reset();
	}

	OpenGLDisplaySurfaceHandle X11OpenGLSystemDriver::_NativeCreateDisplaySurface(
			const OpenGLDisplaySurfaceCreateInfo & pCreateInfo )
	{
		auto displaySurface = CreateSysObject<X11OpenGLDisplaySurface>( GetHandle<X11OpenGLSystemDriver>() );

		Platform::X11WindowCreateInfo x11WindowCreateInfo;
		x11WindowCreateInfo.frameGeometry = pCreateInfo.frameGeometry;
		x11WindowCreateInfo.title = "TS3 OpenGL Window";
		x11WindowCreateInfo.fullscreenMode = pCreateInfo.flags.is_set( eOpenGLDisplaySurfaceCreateFlagFullscreenBit );

		Platform::_x11CreateGLWindowAndSurface( displaySurface->mNativeData, x11WindowCreateInfo, pCreateInfo.visualConfig );

		Platform::X11WindowPostCreateUpdate( displaySurface->mNativeData, x11WindowCreateInfo );

		if( auto glXSwapIntervalEXTProc = Platform::_X11QueryGLXSwapIntervalProc() )
		{
			auto & xSessionData = Platform::X11GetXSessionData( *this );

			if( pCreateInfo.flags.is_set( eOpenGLDisplaySurfaceCreateFlagSyncDisabledBit ) )
			{
				glXSwapIntervalEXTProc( xSessionData.displayHandle, displaySurface->mNativeData.mWindowXID, 0 );
			}
			else if( pCreateInfo.flags.is_set( eOpenGLDisplaySurfaceCreateFlagSyncAdaptiveBit ) )
			{
				glXSwapIntervalEXTProc( xSessionData.displayHandle, displaySurface->mNativeData.mWindowXID, -1 );
			}
			else if( pCreateInfo.flags.is_set( eOpenGLDisplaySurfaceCreateFlagSyncVerticalBit ) )
			{
				glXSwapIntervalEXTProc( xSessionData.displayHandle, displaySurface->mNativeData.mWindowXID, 1 );
			}
		}

		return displaySurface;
	}

	OpenGLDisplaySurfaceHandle X11OpenGLSystemDriver::_NativeCreateDisplaySurfaceForCurrentThread()
	{
		auto & xSessionData = Platform::X11GetXSessionData( *this );

		auto currentWindowXID = glXGetCurrentDrawable();

		XWindowAttributes windowAttributes;
		XGetWindowAttributes( xSessionData.displayHandle, currentWindowXID, &windowAttributes );

		auto displaySurface = CreateSysObject<X11OpenGLDisplaySurface>( GetHandle<X11OpenGLSystemDriver>() );

		displaySurface->mNativeData.mWindowXID = currentWindowXID;
		displaySurface->mNativeData.xColormap = windowAttributes.colormap;

		return displaySurface;
	}

	void X11OpenGLSystemDriver::_NativeDestroyDisplaySurface( OpenGLDisplaySurface & pDisplaySurface )
	{
	}

	OpenGLRenderContextHandle X11OpenGLSystemDriver::_NativeCreateRenderContext(
			OpenGLDisplaySurface & pDisplaySurface,
			const OpenGLRenderContextCreateInfo & pCreateInfo )
	{
		auto & xSessionData = Platform::X11GetXSessionData( *this );

		auto * x11DisplaySurface = pDisplaySurface.QueryInterface<X11OpenGLDisplaySurface>();

		auto glXCreateContextAttribsProc = Platform::_X11QueryGLXCreateContextAttribsProc();

		int contextAPIProfile = 0;
		cppx::bitmask<int> contextCreateFlags = 0;
		GLXContext shareContextHandle = nullptr;

		if( pCreateInfo.contextAPIProfile == EOpenGLAPIProfile::Core )
		{
			contextAPIProfile = GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
		}
		else if( pCreateInfo.contextAPIProfile == EOpenGLAPIProfile::Legacy )
		{
			contextAPIProfile = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
		}
		else if( pCreateInfo.contextAPIProfile == EOpenGLAPIProfile::GLES )
		{
			contextAPIProfile = GLX_CONTEXT_ES_PROFILE_BIT_EXT;
		}

		if( pCreateInfo.flags.is_set( eOpenGLRenderContextCreateFlagEnableDebugBit ) )
		{
			contextCreateFlags |= GLX_CONTEXT_DEBUG_BIT_ARB;
		}
		if( pCreateInfo.flags.is_set( eOpenGLRenderContextCreateFlagForwardCompatibleBit ) )
		{
			contextCreateFlags |= GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;
		}
		if( pCreateInfo.flags.is_set( eOpenGLRenderContextCreateFlagEnableSharingBit ) )
		{
			if( pCreateInfo.shareContext )
			{
				auto * x11ShareContext = pCreateInfo.shareContext->QueryInterface<X11OpenGLRenderContext>();
				shareContextHandle = x11ShareContext->mNativeData.contextHandle;
			}
			else if( pCreateInfo.flags.is_set( eOpenGLRenderContextCreateFlagShareWithCurrentBit ) )
			{
				if( auto * currentGLXContext = ::glXGetCurrentContext() )
				{
					shareContextHandle = currentGLXContext;
				}
			}
		}

		const int contextAttributes[] =
		{
			// Requested OpenGL API version: major part
			GLX_CONTEXT_MAJOR_VERSION_ARB, pCreateInfo.requestedAPIVersion.num_major,
			// Requested OpenGL API version: minor part
			GLX_CONTEXT_MINOR_VERSION_ARB, pCreateInfo.requestedAPIVersion.mNumMinor,
			//
			GLX_CONTEXT_PROFILE_MASK_ARB, contextAPIProfile,
			//
			GLX_CONTEXT_FLAGS_ARB, contextCreateFlags,
			// Terminator
			FALSE
		};

		GLXContext contextHandle = glXCreateContextAttribsProc( xSessionData.displayHandle,
		                                                        x11DisplaySurface->mNativeData.mFBConfig,
		                                                        shareContextHandle,
		                                                        True,
		                                                        &( contextAttributes[0] ) );

		if( !contextHandle )
		{
			Ic3Throw( eExcCodeDebugPlaceholder );
		}

		auto renderContext = CreateSysObject<X11OpenGLRenderContext>( GetHandle<X11OpenGLSystemDriver>() );
		renderContext->mNativeData.contextHandle = contextHandle;

		return renderContext;
	}

	OpenGLRenderContextHandle X11OpenGLSystemDriver::_NativeCreateRenderContextForCurrentThread()
	{
		auto contextHandle = ::glXGetCurrentContext();
		if( contextHandle == nullptr )
		{
			Ic3Throw( eExcCodeDebugPlaceholder );
		}

		auto renderContext = CreateSysObject<X11OpenGLRenderContext>( GetHandle<X11OpenGLSystemDriver>() );
		renderContext->mNativeData.contextHandle = contextHandle;

		return renderContext;
	}

	void X11OpenGLSystemDriver::_NativeDestroyRenderContext( OpenGLRenderContext & pRenderContext )
	{
		auto * x11RenderContext = pRenderContext.QueryInterface<X11OpenGLRenderContext>();
		Platform::_x11DestroyGLContext( x11RenderContext->mNativeData );
	}

	void X11OpenGLSystemDriver::_NativeResetContextBinding()
	{
		auto & xSessionData = Platform::X11GetXSessionData( *this );
		::glXMakeContextCurrent( xSessionData.displayHandle, Platform::eXIDNone, Platform::eXIDNone, nullptr );
	}

	std::vector<EDepthStencilFormat> X11OpenGLSystemDriver::_NativeQuerySupportedDepthStencilFormats(
			EColorFormat pColorFormat ) const
	{
		return {};
	}

	std::vector<EMSAAMode> X11OpenGLSystemDriver::_NativeQuerySupportedMSAAModes(
			EColorFormat pColorFormat,
			EDepthStencilFormat pDepthStencilFormat ) const
	{
		return {};
	}

	bool X11OpenGLSystemDriver::_NativeIsAPIClassSupported( EOpenGLAPIClass pAPIClass ) const
	{
		if( pAPIClass == EOpenGLAPIClass::Desktop )
		{
			return true;
		}
		return false;
	}

	bool X11OpenGLSystemDriver::_NativeIsRenderContextBound() const
	{
		auto currentContext = ::glXGetCurrentContext();
		return currentContext != nullptr;
	}


	X11OpenGLDisplaySurface::X11OpenGLDisplaySurface( X11OpenGLSystemDriverHandle pGLSystemDriver )
	: X11NativeObject( std::move( pGLSystemDriver ), &mNativeData )
	{}

	X11OpenGLDisplaySurface::~X11OpenGLDisplaySurface() noexcept
	{
		_ReleaseX11SurfaceState();
	}

	void X11OpenGLDisplaySurface::_ReleaseX11SurfaceState()
	{
		Platform::_x11DestroyGLWindowAndSurface( mNativeData );
	}

	void X11OpenGLDisplaySurface::_NativeSwapBuffers()
	{
		auto & xSessionData = Platform::X11GetXSessionData( *this );
		glXSwapBuffers( xSessionData.displayHandle, mNativeData.mWindowXID );
	}

    EOpenGLAPIClass X11OpenGLDisplaySurface::_NativeQuerySupportedAPIClass() const noexcept
    {
        return EOpenGLAPIClass::Desktop;
    }

	VisualConfig X11OpenGLDisplaySurface::_NativeQueryVisualConfig() const
	{
		auto & xSessionData = Platform::X11GetXSessionData( *this );

		XWindowAttributes windowAttributes;
		XGetWindowAttributes( xSessionData.displayHandle, mNativeData.mWindowXID, &windowAttributes );

		int glxFBRenderType = 0;
		int glxFBDepthBits = 0;
		int glxFBStencilBits = 0;

		glXGetFBConfigAttrib( xSessionData.displayHandle, mNativeData.mFBConfig, GLX_RENDER_TYPE, &glxFBRenderType );
		glXGetFBConfigAttrib( xSessionData.displayHandle, mNativeData.mFBConfig, GLX_DEPTH_SIZE, &glxFBDepthBits );
		glXGetFBConfigAttrib( xSessionData.displayHandle, mNativeData.mFBConfig, GLX_STENCIL_SIZE, &glxFBStencilBits );

		VisualConfig visualConfig;

		if( glxFBRenderType == GLX_RGBA_BIT )
		{
			visualConfig.colorFormat = EColorFormat::B8G8R8A8;
		}
		else
		{
			visualConfig.colorFormat = EColorFormat::Unknown;
		}

		if( ( glxFBDepthBits == 24 ) && ( glxFBStencilBits == 8 ) )
		{
			visualConfig.depthStencilFormat = EDepthStencilFormat::D24S8;
		}
		else if( ( glxFBDepthBits == 24 ) && ( glxFBStencilBits == 0 ) )
		{
			visualConfig.depthStencilFormat = EDepthStencilFormat::D24X8;
		}
		else if( ( glxFBDepthBits == 32 ) && ( glxFBStencilBits == 0 ) )
		{
			visualConfig.depthStencilFormat = EDepthStencilFormat::D32F;
		}
		else
		{
			visualConfig.depthStencilFormat = EDepthStencilFormat::Unknown;
		}

		return visualConfig;
	}

	FrameSize X11OpenGLDisplaySurface::_NativeQueryRenderAreaSize() const
	{
		return Platform::X11GetFrameSize( mNativeData, EFrameSizeMode::ClientArea );
	}

	bool X11OpenGLDisplaySurface::_NativeSysValidate() const
	{
		return ( mNativeData.mWindowXID != Platform::eXIDNone ) && mNativeData.mFBConfig;
	}

	void X11OpenGLDisplaySurface::_NativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode )
	{}

	void X11OpenGLDisplaySurface::_NativeSetFullscreenMode( bool pEnable )
	{
		Platform::X11SetWindowFullscreenState( mNativeData, pEnable );
	}

	void X11OpenGLDisplaySurface::_NativeSetTitle( const std::string & pTitle )
	{
		return Platform::X11SetFrameTitle( mNativeData, pTitle );
	}

	void X11OpenGLDisplaySurface::_NativeUpdateGeometry(
			const FrameGeometry & pFrameGeometry,
			cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
	{
		return Platform::X11UpdateFrameGeometry( mNativeData, pFrameGeometry, pUpdateFlags );
	}

	FrameSize X11OpenGLDisplaySurface::_NativeGetSize( EFrameSizeMode pSizeMode ) const
	{
		return Platform::X11GetFrameSize( mNativeData, pSizeMode );
	}

	bool X11OpenGLDisplaySurface::_NativeIsFullscreen() const
	{
		return Platform::X11IsFullscreenWindow( mNativeData );
	}


	X11OpenGLRenderContext::X11OpenGLRenderContext( X11OpenGLSystemDriverHandle pGLSystemDriver )
	: X11NativeObject( std::move( pGLSystemDriver ) )
	{}

	X11OpenGLRenderContext::~X11OpenGLRenderContext() noexcept
	{
		_ReleaseX11ContextState();
	}

	void X11OpenGLRenderContext::_NativeBindForCurrentThread( const OpenGLDisplaySurface & pTargetSurface )
	{
		auto & xSessionData = Platform::X11GetXSessionData( *this );

		const auto * x11DisplaySurface = pTargetSurface.QueryInterface<X11OpenGLDisplaySurface>();

		::glXMakeContextCurrent( xSessionData.displayHandle,
		                         x11DisplaySurface->mNativeData.mWindowXID,
		                         x11DisplaySurface->mNativeData.mWindowXID,
		                         mNativeData.contextHandle );
	}

	bool X11OpenGLRenderContext::_NativeSysCheckIsCurrent() const
	{
		auto currentContextHandle = glXGetCurrentContext();
		return mNativeData.contextHandle && ( mNativeData.contextHandle == currentContextHandle );
	}

	bool X11OpenGLRenderContext::_NativeSysValidate() const
	{
		return mNativeData.contextHandle != nullptr;
	}

	void X11OpenGLRenderContext::_ReleaseX11ContextState()
	{
		Platform::_x11DestroyGLContext( mNativeData );
	}

	
	namespace Platform
	{

		PFNGLXCREATECONTEXTATTRIBSARBPROC _X11QueryGLXCreateContextAttribsProc()
		{
			static PFNGLXCREATECONTEXTATTRIBSARBPROC cvGLXCreateContextAttribsProc = nullptr;

			if( !cvGLXCreateContextAttribsProc )
			{
				auto procAddress = glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" );
				cvGLXCreateContextAttribsProc = reinterpret_cast<PFNGLXCREATECONTEXTATTRIBSARBPROC>( procAddress );
			}

			return cvGLXCreateContextAttribsProc;
		}

		PFNGLXSWAPINTERVALEXTPROC _X11QueryGLXSwapIntervalProc()
		{
			static PFNGLXSWAPINTERVALEXTPROC cvGLXSwapIntervalEXTProc = nullptr;

			if( !cvGLXSwapIntervalEXTProc )
			{
				auto procAddress = glXGetProcAddressARB( (const GLubyte *) "glXSwapIntervalEXT" );
				cvGLXSwapIntervalEXTProc = reinterpret_cast<PFNGLXSWAPINTERVALEXTPROC>( procAddress );
			}

			return cvGLXSwapIntervalEXTProc;
		}

		void _x11CreateGLWindowAndSurface( X11OpenGLDisplaySurfaceNativeData & pGLSurfaceNativeData,
		                                   X11WindowCreateInfo & pWindowCreateInfo,
		                                   const VisualConfig & pVisualConfig )
		{
			auto & xSessionData = Platform::X11GetXSessionData( pGLSurfaceNativeData );

			X11OpenGLVisualConfig x11GLVisualConfig{};
			if( pVisualConfig.flags.is_set( eVisualAttribFlagLegacyBit ) )
			{
				x11GLVisualConfig = _x11ChooseLegacyGLFBConfig( xSessionData.displayHandle, xSessionData.screenIndex );
			}
			else
			{
				x11GLVisualConfig = _x11ChooseCoreGLFBConfig( xSessionData.displayHandle, xSessionData.screenIndex, pVisualConfig );
			}

			if( !x11GLVisualConfig )
			{
				Ic3Throw( eExcCodeDebugPlaceholder );
			}

			pGLSurfaceNativeData.mFBConfig = x11GLVisualConfig.mFBConfig;
			pGLSurfaceNativeData.mVisualInfo = x11GLVisualConfig.mXVisualInfo;

			pWindowCreateInfo.colorDepth = x11GLVisualConfig.mXVisualInfo->depth;
			pWindowCreateInfo.windowVisual = x11GLVisualConfig.mXVisualInfo->visual;

			Platform::X11CreateWindow( pGLSurfaceNativeData, pWindowCreateInfo );
		}

		void _x11DestroyGLWindowAndSurface( X11OpenGLDisplaySurfaceNativeData & pGLSurfaceNativeData )
		{
			Platform::X11DestroyWindow( pGLSurfaceNativeData );
		}

		void _x11DestroyGLContext( X11OpenGLRenderContextNativeData & pGLContextNativeData )
		{
			auto & xSessionData = Platform::X11GetXSessionData( pGLContextNativeData );
			if( pGLContextNativeData.contextHandle != nullptr )
			{
				auto currentContextHandle = ::glXGetCurrentContext();
				if( pGLContextNativeData.contextHandle == currentContextHandle )
				{
					::glXMakeContextCurrent( xSessionData.displayHandle, eXIDNone, eXIDNone, nullptr );
				}

				::glXDestroyContext( xSessionData.displayHandle, pGLContextNativeData.contextHandle );
				pGLContextNativeData.contextHandle = nullptr;
			}
		}

		void _x11CreateAndBindLegacyRenderContext( X11OpenGLRenderContextNativeData & pGLContextNativeData,
		                                           const X11OpenGLDisplaySurfaceNativeData & pGLSurfaceNativeData )
		{
			auto & xSessionData = Platform::X11GetXSessionData( pGLSurfaceNativeData );

			auto tempContextHandle = ::glXCreateContext( xSessionData.displayHandle, pGLSurfaceNativeData.mVisualInfo, nullptr, True );
			if( !tempContextHandle )
			{
				Ic3Throw( eExcCodeDebugPlaceholder );
			}

			pGLContextNativeData.contextHandle = tempContextHandle;

			auto makeCurrentResult = ::glXMakeCurrent( xSessionData.displayHandle, pGLSurfaceNativeData.mWindowXID, tempContextHandle );
			if( makeCurrentResult == False )
			{
				Ic3Throw( eExcCodeDebugPlaceholder );
			}
		}

		X11OpenGLVisualConfig _x11ChooseLegacyGLFBConfig( XDisplay pDisplay, int pScreenIndex )
		{
			const int cvDefaultVisualAttribs[] =
			{
				GLX_X_RENDERABLE  , True,
				GLX_DRAWABLE_TYPE , GLX_WINDOW_BIT,
				GLX_RENDER_TYPE   , GLX_RGBA_BIT,
				GLX_X_VISUAL_TYPE , GLX_TRUE_COLOR,
				GLX_RED_SIZE      , 8,
				GLX_GREEN_SIZE    , 8,
				GLX_BLUE_SIZE     , 8,
				GLX_ALPHA_SIZE    , 8,
				GLX_DEPTH_SIZE    , 24,
				GLX_STENCIL_SIZE  , 8,
				GLX_DOUBLEBUFFER  , True,
				eXIDNone,
			};

			const int cvDefaultVisualAttribsNoDepthStencil[] =
			{
				GLX_X_RENDERABLE  , True,
				GLX_DRAWABLE_TYPE , GLX_WINDOW_BIT,
				GLX_RENDER_TYPE   , GLX_RGBA_BIT,
				GLX_X_VISUAL_TYPE , GLX_TRUE_COLOR,
				GLX_RED_SIZE      , 8,
				GLX_GREEN_SIZE    , 8,
				GLX_BLUE_SIZE     , 8,
				GLX_ALPHA_SIZE    , 8,
				GLX_DOUBLEBUFFER  , True,
				eXIDNone,
			};

			int fbConfigListSize = 0;
			auto * fbConfigList = ::glXChooseFBConfig( pDisplay, pScreenIndex, cvDefaultVisualAttribs, &fbConfigListSize );

			if( ( fbConfigList == nullptr ) || ( fbConfigListSize == 0 ) )
			{
				fbConfigList = glXChooseFBConfig( pDisplay, pScreenIndex, cvDefaultVisualAttribsNoDepthStencil, &fbConfigListSize );
				if( ( fbConfigList == nullptr ) || ( fbConfigListSize == 0 ) )
				{
					Ic3Throw( eExcCodeDebugPlaceholder );
				}
			}

			GLXFBConfig bestFBConfig = nullptr;
			GLXFBConfig bestFBConfigRGB8 = nullptr;

			for( int fbcIndex = 0; fbcIndex < fbConfigListSize; ++fbcIndex )
			{
				GLXFBConfig fbConfig = fbConfigList[fbcIndex];
				XVisualInfo * fbcVisualInfo = glXGetVisualFromFBConfig( pDisplay, fbConfig );

				if( fbcVisualInfo != nullptr )
				{
					if( ( fbcVisualInfo->bits_per_rgb == 8 ) && ( bestFBConfigRGB8 == nullptr ) )
					{
						bestFBConfigRGB8 = fbConfig;
					}
					else if( bestFBConfig == nullptr )
					{
						bestFBConfig = fbConfig;
					}
					XFree( fbcVisualInfo );
				}

				if( bestFBConfigRGB8 != nullptr )
				{
					bestFBConfig = bestFBConfigRGB8;
					break;
				}
			}

			XFree( fbConfigList );

			X11OpenGLVisualConfig x11GLVisualConfig;
			x11GLVisualConfig.mFBConfig = bestFBConfig;
			x11GLVisualConfig.mXVisualInfo = glXGetVisualFromFBConfig( pDisplay, bestFBConfig );

			return x11GLVisualConfig;
		}

		X11OpenGLVisualConfig _x11ChooseCoreGLFBConfig( XDisplay pDisplay, int pScreenIndex, const VisualConfig & pVisualConfig )
		{
			auto fbConfigList = _X11QueryCompatibleFBConfigList( pDisplay, pScreenIndex, pVisualConfig );

			int bestMatchRate = 0;
			GLXFBConfig bestFBConfig = nullptr;

			for( auto fbConfig : fbConfigList )
			{
				int matchRate = _X11GetFBConfigMatchRate( pDisplay, fbConfig, pVisualConfig );
				if( matchRate > bestMatchRate )
				{
					bestMatchRate = matchRate;
					bestFBConfig = fbConfig;
				}
			}

			X11OpenGLVisualConfig x11GLVisualConfig;
			x11GLVisualConfig.mFBConfig = bestFBConfig;
			x11GLVisualConfig.mXVisualInfo = glXGetVisualFromFBConfig( pDisplay, bestFBConfig );

			return x11GLVisualConfig;
		}

		GLXFBConfigArray _X11QueryCompatibleFBConfigList( XDisplay pDisplay, int pScreenIndex, const VisualConfig & pVisualConfig )
		{
			GLXFBConfigArray result;

			int fbConfigAttribArray[CX_X11_MAX_GLX_FBCONFIG_ATTRIBUTES_NUM];
			_X11GetAttribArrayForVisualConfig( pVisualConfig, fbConfigAttribArray );

			int fbConfigListSize = 0;
			auto * fbConfigList = glXChooseFBConfig( pDisplay, pScreenIndex, fbConfigAttribArray, &fbConfigListSize );

			if( ( fbConfigList == nullptr ) || ( fbConfigListSize == 0 ) )
			{
				Ic3Throw( eExcCodeDebugPlaceholder );
			}

			for( int pFBConfigIndex = 0; pFBConfigIndex < fbConfigListSize; ++pFBConfigIndex )
			{
				auto pFBConfig = fbConfigList[pFBConfigIndex];
				auto * visualInfo = glXGetVisualFromFBConfig( pDisplay, pFBConfig );

				if( visualInfo != nullptr )
				{
					int fbConfigAlphaSize = 0;
					glXGetFBConfigAttrib( pDisplay, pFBConfig, GLX_ALPHA_SIZE, &fbConfigAlphaSize );

					XFree( visualInfo );

					if( fbConfigAlphaSize > 0 )
					{
						result.push_back( pFBConfig );
					}
				}
			}

			XFree( fbConfigList );

			return result;
		}

		int _X11GetFBConfigMatchRate( XDisplay pDisplay, GLXFBConfig pFBConfig, const VisualConfig & pVisualConfig )
		{
			int doubleBufferRequestedState = True;
			int stereoModeRequestedState = False;

			if( pVisualConfig.flags.is_set( eVisualAttribFlagSingleBufferBit ) &&
			    !pVisualConfig.flags.is_set( eVisualAttribFlagDoubleBufferBit ) )
			{
				doubleBufferRequestedState = False;
			}

			if( pVisualConfig.flags.is_set( eVisualAttribFlagStereoDisplayBit ) &&
			    !pVisualConfig.flags.is_set( eVisualAttribFlagMonoDisplayBit ) )
			{
				stereoModeRequestedState = True;
			}

			int matchRate = 0;
			int fbConfigAttribValue = 0;

			glXGetFBConfigAttrib( pDisplay, pFBConfig, GLX_BUFFER_SIZE, &fbConfigAttribValue );
			matchRate += ( fbConfigAttribValue == 32 );

			glXGetFBConfigAttrib( pDisplay, pFBConfig, GLX_DOUBLEBUFFER, &fbConfigAttribValue );
			matchRate += ( fbConfigAttribValue == doubleBufferRequestedState );

			glXGetFBConfigAttrib( pDisplay, pFBConfig, GLX_STEREO, &fbConfigAttribValue );
			matchRate += ( fbConfigAttribValue == stereoModeRequestedState );

			glXGetFBConfigAttrib( pDisplay, pFBConfig, GLX_DEPTH_SIZE, &fbConfigAttribValue );
			matchRate += ( fbConfigAttribValue == pVisualConfig.depthStencilDesc.depthBufferSize );

			glXGetFBConfigAttrib( pDisplay, pFBConfig, GLX_STENCIL_SIZE, &fbConfigAttribValue );
			matchRate += ( fbConfigAttribValue == pVisualConfig.depthStencilDesc.depthBufferSize );

			glXGetFBConfigAttrib( pDisplay, pFBConfig, GLX_SAMPLES, &fbConfigAttribValue );
			matchRate += ( fbConfigAttribValue == pVisualConfig.msaaDesc.mQuality );

			auto * fbConfigXVisualInfo =  glXGetVisualFromFBConfig( pDisplay, pFBConfig );
			matchRate += ( fbConfigXVisualInfo->bits_per_rgb == 8 );
			XFree( fbConfigXVisualInfo );

			return matchRate;
		}

		void _X11GetAttribArrayForVisualConfig( const VisualConfig & pVisualConfig, int * pAttribArray )
		{
			int attribIndex = 0;

			Ic3X11OpenGLContextAttribAppend( pAttribArray, attribIndex, GLX_DRAWABLE_TYPE );
			Ic3X11OpenGLContextAttribAppend( pAttribArray, attribIndex, GLX_WINDOW_BIT );

			Ic3X11OpenGLContextAttribAppend( pAttribArray, attribIndex, GLX_RENDER_TYPE );
			Ic3X11OpenGLContextAttribAppend( pAttribArray, attribIndex, GLX_RGBA_BIT );

			Ic3X11OpenGLContextAttribAppend( pAttribArray, attribIndex, GLX_X_RENDERABLE );
			Ic3X11OpenGLContextAttribAppend( pAttribArray, attribIndex, True );

			Ic3X11OpenGLContextAttribAppend( pAttribArray, attribIndex, GLX_X_VISUAL_TYPE );
			Ic3X11OpenGLContextAttribAppend( pAttribArray, attribIndex, GLX_TRUE_COLOR );

			if( pVisualConfig.flags.is_set( eVisualAttribFlagDoubleBufferBit ) )
			{
				pAttribArray[attribIndex++] = GLX_DOUBLEBUFFER;
				pAttribArray[attribIndex++] = True;
			}
			else if( pVisualConfig.flags.is_set( eVisualAttribFlagSingleBufferBit ) )
			{
				pAttribArray[attribIndex++] = GLX_DOUBLEBUFFER;
				pAttribArray[attribIndex++] = False;
			}

			if( pVisualConfig.flags.is_set( eVisualAttribFlagMonoDisplayBit ) )
			{
				pAttribArray[attribIndex++] = GLX_STEREO;
				pAttribArray[attribIndex++] = False;
			}
			else if( pVisualConfig.flags.is_set( eVisualAttribFlagStereoDisplayBit ) )
			{
				pAttribArray[attribIndex++] = GLX_STEREO;
				pAttribArray[attribIndex++] = True;
			}

			if( ( pVisualConfig.msaaDesc.mBufferCount != 0 ) && ( pVisualConfig.msaaDesc.mQuality != 0 ) )
			{
				pAttribArray[attribIndex++] = GLX_SAMPLE_BUFFERS;
				pAttribArray[attribIndex++] = pVisualConfig.msaaDesc.mBufferCount;

				pAttribArray[attribIndex++] = GLX_SAMPLES;
				pAttribArray[attribIndex++] = pVisualConfig.msaaDesc.mQuality;
			}

			if( pVisualConfig.mColorDesc.mRGBA.u32Code != 0 )
			{
				pAttribArray[attribIndex++] = GLX_RED_SIZE;
				pAttribArray[attribIndex++] = pVisualConfig.mColorDesc.mRGBA.u8Red;

				pAttribArray[attribIndex++] = GLX_GREEN_SIZE;
				pAttribArray[attribIndex++] = pVisualConfig.mColorDesc.mRGBA.u8Green;

				pAttribArray[attribIndex++] = GLX_BLUE_SIZE;
				pAttribArray[attribIndex++] = pVisualConfig.mColorDesc.mRGBA.u8Blue;

				pAttribArray[attribIndex++] = GLX_ALPHA_SIZE;
				pAttribArray[attribIndex++] = pVisualConfig.mColorDesc.mRGBA.u8Alpha;
			}

			if( pVisualConfig.depthStencilDesc.depthBufferSize != 0 )
			{
				pAttribArray[attribIndex++] = GLX_DEPTH_SIZE;
				pAttribArray[attribIndex++] = pVisualConfig.depthStencilDesc.depthBufferSize;
			}

			if( pVisualConfig.depthStencilDesc.mStencilBufferSize != 0 )
			{
				pAttribArray[attribIndex++] = GLX_STENCIL_SIZE;
				pAttribArray[attribIndex++] = pVisualConfig.depthStencilDesc.mStencilBufferSize;
			}

			pAttribArray[attribIndex++] = eXIDNone;
		}

	}

} // namespace Ic3::System
#endif // PCL_TARGET_SYSAPI_X11
