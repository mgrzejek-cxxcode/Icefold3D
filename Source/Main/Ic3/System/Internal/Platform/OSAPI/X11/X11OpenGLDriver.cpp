
#include "X11OpenGLDriver.h"
#include "X11DisplaySystem.h"

#define ic3X11OpenGLContextAttribAppend( pArray, pIndex, pAttrib ) \
	ic3DebugAssert( pIndex < CX_X11_MAX_GLX_FBCONFIG_ATTRIBUTES_NUM ); \
	pArray[pIndex++] = pAttrib

#if( IC3_PCL_TARGET_SYSAPI == IC3_PCL_TARGET_SYSAPI_X11 )
namespace Ic3::System
{

	namespace Platform
	{

		PFNGLXCREATECONTEXTATTRIBSARBPROC _x11QueryGLXCreateContextAttribsProc();

		PFNGLXSWAPINTERVALEXTPROC _x11QueryGLXSwapIntervalProc();

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
		GLXFBConfigArray _x11QueryCompatibleFBConfigList( XDisplay pDisplay,
		                                                  int pScreenIndex,
		                                                  const VisualConfig & pVisualConfig );

		// Computes a "compatibility rate", i.e. how much the specified FBConfig matches the visual.
		int _x11GetFBConfigMatchRate( XDisplay pDisplay, GLXFBConfig pFBConfig, const VisualConfig & pVisualConfig );

		// Translation: VisualConfig --> array of GLX_* attributes required by the system API.
		// Used for surface/context creation.
		void _x11GetAttribArrayForVisualConfig( const VisualConfig & pVisualConfig, int * pAttribArray );

	}


	X11OpenGLSystemDriver::X11OpenGLSystemDriver( X11DisplayManagerHandle pDisplayManager )
	: X11NativeObject( std::move( pDisplayManager ) )
	{}

	X11OpenGLSystemDriver::~X11OpenGLSystemDriver() noexcept
	{
		_nativeReleaseInitState();
	}

	void X11OpenGLSystemDriver::_nativeInitializePlatform()
	{
		ic3DebugAssert( !mNativeData.mInitState );
		// Init state should be first created here and destroyed as soon as proper GL
		// contexts are created (this is not enforce, though, and controlled explicitly
		// by the user and done by calling releaseInitState() method od the driver).
		mNativeData.mInitState = std::make_unique<Platform::X11OpenGLSystemDriverNativeData::InitState>();

		auto & xSessionData = Platform::x11GetXSessionData( *this );

		int glxVersionMajor = 0;
		int glxVersionMinor = 0;

		::glXQueryVersion( xSessionData.mDisplay, &glxVersionMajor, &glxVersionMinor );
		// We need at least version 1.3 of the GLX runtime (that's rather a pretty old one...).
		if( ( glxVersionMajor <= 0 ) || ( ( glxVersionMajor == 1 ) && ( glxVersionMinor < 3 ) ) )
		{
			ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
		}

		VisualConfig legacyVisualConfig;
		legacyVisualConfig = vsxGetDefaultVisualConfigForSysWindow();
		legacyVisualConfig.mFlags.set( eVisualAttribFlagLegacyBit );

		Platform::X11WindowCreateInfo windowCreateInfo;
		windowCreateInfo.mFrameGeometry.mPosition = { 0, 0 };
		windowCreateInfo.mFrameGeometry.mSize = { 600, 600 };
		windowCreateInfo.mFrameGeometry.mStyle = EFrameStyle::Overlay;

		auto & tmpSurfaceNativeData = mNativeData.mInitState->mSurfaceData;
		tmpSurfaceNativeData.setSessionData( xSessionData );
		Platform::_x11CreateGLWindowAndSurface( tmpSurfaceNativeData, windowCreateInfo, legacyVisualConfig );

		auto & tmpContextNativeData = mNativeData.mInitState->mContextData;
		tmpContextNativeData.setSessionData( xSessionData );
		Platform::_x11CreateAndBindLegacyRenderContext( tmpContextNativeData, tmpSurfaceNativeData );
	}

	void X11OpenGLSystemDriver::_nativeReleaseInitState() noexcept
	{
		if( !mNativeData.mInitState )
		{
			return;
		}

		auto & xSessionData = Platform::x11GetXSessionData( *this );

		auto & tmpSurfaceNativeData = mNativeData.mInitState->mSurfaceData;
		auto & tmpContextNativeData = mNativeData.mInitState->mContextData;

		if( tmpContextNativeData.mContextHandle != nullptr )
		{
			::glXDestroyContext( xSessionData.mDisplay, tmpContextNativeData.mContextHandle );
			tmpContextNativeData.mContextHandle = nullptr;
			tmpContextNativeData.resetSessionData();
		}

		if( tmpSurfaceNativeData.mWindowXID != Platform::eXIDNone )
		{
			Platform::_x11DestroyGLWindowAndSurface( tmpSurfaceNativeData );
			tmpSurfaceNativeData.mFBConfig = nullptr;
			tmpSurfaceNativeData.resetSessionData();
		}

		if( tmpSurfaceNativeData.mVisualInfo != nullptr )
		{
			XFree( tmpSurfaceNativeData.mVisualInfo );
			tmpSurfaceNativeData.mVisualInfo = nullptr;
		}

		mNativeData.mInitState.reset();
	}

	OpenGLDisplaySurfaceHandle X11OpenGLSystemDriver::_nativeCreateDisplaySurface( const OpenGLDisplaySurfaceCreateInfo & pCreateInfo )
	{
		auto displaySurface = createSysObject<X11OpenGLDisplaySurface>( getHandle<X11OpenGLSystemDriver>() );

		Platform::X11WindowCreateInfo x11WindowCreateInfo;
		x11WindowCreateInfo.mFrameGeometry = pCreateInfo.mFrameGeometry;
		x11WindowCreateInfo.mTitle = "TS3 OpenGL Window";
		x11WindowCreateInfo.mFullscreenMode = pCreateInfo.mFlags.isSet( eOpenGLDisplaySurfaceCreateFlagFullscreenBit );

		Platform::_x11CreateGLWindowAndSurface( displaySurface->mNativeData, x11WindowCreateInfo, pCreateInfo.mVisualConfig );

		Platform::x11WindowPostCreateUpdate( displaySurface->mNativeData, x11WindowCreateInfo );

		if( auto glXSwapIntervalEXTProc = Platform::_x11QueryGLXSwapIntervalProc() )
		{
			auto & xSessionData = Platform::x11GetXSessionData( *this );

			if( pCreateInfo.mFlags.isSet( eOpenGLDisplaySurfaceCreateFlagSyncDisabledBit ) )
			{
				glXSwapIntervalEXTProc( xSessionData.mDisplay, displaySurface->mNativeData.mWindowXID, 0 );
			}
			else if( pCreateInfo.mFlags.isSet( eOpenGLDisplaySurfaceCreateFlagSyncAdaptiveBit ) )
			{
				glXSwapIntervalEXTProc( xSessionData.mDisplay, displaySurface->mNativeData.mWindowXID, -1 );
			}
			else if( pCreateInfo.mFlags.isSet( eOpenGLDisplaySurfaceCreateFlagSyncVerticalBit ) )
			{
				glXSwapIntervalEXTProc( xSessionData.mDisplay, displaySurface->mNativeData.mWindowXID, 1 );
			}
		}

		return displaySurface;
	}

	OpenGLDisplaySurfaceHandle X11OpenGLSystemDriver::_nativeCreateDisplaySurfaceForCurrentThread()
	{
		auto & xSessionData = Platform::x11GetXSessionData( *this );

		auto currentWindowXID = glXGetCurrentDrawable();

		XWindowAttributes windowAttributes;
		XGetWindowAttributes( xSessionData.mDisplay, currentWindowXID, &windowAttributes );

		auto displaySurface = createSysObject<X11OpenGLDisplaySurface>( getHandle<X11OpenGLSystemDriver>() );

		displaySurface->mNativeData.mWindowXID = currentWindowXID;
		displaySurface->mNativeData.mXColormap = windowAttributes.colormap;

		return displaySurface;
	}

	void X11OpenGLSystemDriver::_nativeDestroyDisplaySurface( OpenGLDisplaySurface & pDisplaySurface )
	{
	}

	OpenGLRenderContextHandle X11OpenGLSystemDriver::_nativeCreateRenderContext( OpenGLDisplaySurface & pDisplaySurface,
	                                                                             const OpenGLRenderContextCreateInfo & pCreateInfo )
	{
		auto & xSessionData = Platform::x11GetXSessionData( *this );

		auto * x11DisplaySurface = pDisplaySurface.queryInterface<X11OpenGLDisplaySurface>();

		auto glXCreateContextAttribsProc = Platform::_x11QueryGLXCreateContextAttribsProc();

		int contextAPIProfile = 0;
		TBitmask<int> contextCreateFlags = 0;
		GLXContext shareContextHandle = nullptr;

		if( pCreateInfo.mContextAPIProfile == EOpenGLAPIProfile::Core )
		{
			contextAPIProfile = GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
		}
		else if( pCreateInfo.mContextAPIProfile == EOpenGLAPIProfile::Legacy )
		{
			contextAPIProfile = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
		}
		else if( pCreateInfo.mContextAPIProfile == EOpenGLAPIProfile::GLES )
		{
			contextAPIProfile = GLX_CONTEXT_ES_PROFILE_BIT_EXT;
		}

		if( pCreateInfo.mFlags.isSet( eOpenGLRenderContextCreateFlagEnableDebugBit ) )
		{
			contextCreateFlags |= GLX_CONTEXT_DEBUG_BIT_ARB;
		}
		if( pCreateInfo.mFlags.isSet( eOpenGLRenderContextCreateFlagForwardCompatibleBit ) )
		{
			contextCreateFlags |= GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;
		}
		if( pCreateInfo.mFlags.isSet( eOpenGLRenderContextCreateFlagEnableSharingBit ) )
		{
			if( pCreateInfo.mShareContext )
			{
				auto * x11ShareContext = pCreateInfo.mShareContext->queryInterface<X11OpenGLRenderContext>();
				shareContextHandle = x11ShareContext->mNativeData.mContextHandle;
			}
			else if( pCreateInfo.mFlags.isSet( eOpenGLRenderContextCreateFlagShareWithCurrentBit ) )
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
			GLX_CONTEXT_MAJOR_VERSION_ARB, pCreateInfo.mRequestedAPIVersion.mNumMajor,
			// Requested OpenGL API version: minor part
			GLX_CONTEXT_MINOR_VERSION_ARB, pCreateInfo.mRequestedAPIVersion.mNumMinor,
			//
			GLX_CONTEXT_PROFILE_MASK_ARB, contextAPIProfile,
			//
			GLX_CONTEXT_FLAGS_ARB, contextCreateFlags,
			// Terminator
			FALSE
		};

		GLXContext contextHandle = glXCreateContextAttribsProc( xSessionData.mDisplay,
		                                                        x11DisplaySurface->mNativeData.mFBConfig,
		                                                        shareContextHandle,
		                                                        True,
		                                                        &( contextAttributes[0] ) );

		if( !contextHandle )
		{
			ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
		}

		auto renderContext = createSysObject<X11OpenGLRenderContext>( getHandle<X11OpenGLSystemDriver>() );
		renderContext->mNativeData.mContextHandle = contextHandle;

		return renderContext;
	}

	OpenGLRenderContextHandle X11OpenGLSystemDriver::_nativeCreateRenderContextForCurrentThread()
	{
		auto contextHandle = ::glXGetCurrentContext();
		if( contextHandle == nullptr )
		{
			ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
		}

		auto renderContext = createSysObject<X11OpenGLRenderContext>( getHandle<X11OpenGLSystemDriver>() );
		renderContext->mNativeData.mContextHandle = contextHandle;

		return renderContext;
	}

	void X11OpenGLSystemDriver::_nativeDestroyRenderContext( OpenGLRenderContext & pRenderContext )
	{
		auto * x11RenderContext = pRenderContext.queryInterface<X11OpenGLRenderContext>();
		Platform::_x11DestroyGLContext( x11RenderContext->mNativeData );
	}

	void X11OpenGLSystemDriver::_nativeResetContextBinding()
	{
		auto & xSessionData = Platform::x11GetXSessionData( *this );
		::glXMakeContextCurrent( xSessionData.mDisplay, Platform::eXIDNone, Platform::eXIDNone, nullptr );
	}

	std::vector<EDepthStencilFormat> X11OpenGLSystemDriver::_nativeQuerySupportedDepthStencilFormats( EColorFormat pColorFormat ) const
	{
		return {};
	}

	std::vector<EMSAAMode> X11OpenGLSystemDriver::_nativeQuerySupportedMSAAModes( EColorFormat pColorFormat,
	                                                                              EDepthStencilFormat pDepthStencilFormat ) const
	{
		return {};
	}

	bool X11OpenGLSystemDriver::_nativeIsAPIClassSupported( EOpenGLAPIClass pAPIClass ) const
	{
		if( pAPIClass == EOpenGLAPIClass::Desktop )
		{
			return true;
		}
		return false;
	}

	bool X11OpenGLSystemDriver::_nativeIsRenderContextBound() const
	{
		auto currentContext = ::glXGetCurrentContext();
		return currentContext != nullptr;
	}


	X11OpenGLDisplaySurface::X11OpenGLDisplaySurface( X11OpenGLSystemDriverHandle pGLSystemDriver )
	: X11NativeObject( std::move( pGLSystemDriver ), &mNativeData )
	{}

	X11OpenGLDisplaySurface::~X11OpenGLDisplaySurface() noexcept
	{
		_releaseX11SurfaceState();
	}

	void X11OpenGLDisplaySurface::_releaseX11SurfaceState()
	{
		Platform::_x11DestroyGLWindowAndSurface( mNativeData );
	}

	void X11OpenGLDisplaySurface::_nativeSwapBuffers()
	{
		auto & xSessionData = Platform::x11GetXSessionData( *this );
		glXSwapBuffers( xSessionData.mDisplay, mNativeData.mWindowXID );
	}

    EOpenGLAPIClass X11OpenGLDisplaySurface::_nativeQuerySupportedAPIClass() const noexcept
    {
        return EOpenGLAPIClass::Desktop;
    }

	VisualConfig X11OpenGLDisplaySurface::_nativeQueryVisualConfig() const
	{
		auto & xSessionData = Platform::x11GetXSessionData( *this );

		XWindowAttributes windowAttributes;
		XGetWindowAttributes( xSessionData.mDisplay, mNativeData.mWindowXID, &windowAttributes );

		int glxFBRenderType = 0;
		int glxFBDepthBits = 0;
		int glxFBStencilBits = 0;

		glXGetFBConfigAttrib( xSessionData.mDisplay, mNativeData.mFBConfig, GLX_RENDER_TYPE, &glxFBRenderType );
		glXGetFBConfigAttrib( xSessionData.mDisplay, mNativeData.mFBConfig, GLX_DEPTH_SIZE, &glxFBDepthBits );
		glXGetFBConfigAttrib( xSessionData.mDisplay, mNativeData.mFBConfig, GLX_STENCIL_SIZE, &glxFBStencilBits );

		VisualConfig visualConfig;

		if( glxFBRenderType == GLX_RGBA_BIT )
		{
			visualConfig.mColorFormat = EColorFormat::B8G8R8A8;
		}
		else
		{
			visualConfig.mColorFormat = EColorFormat::Unknown;
		}

		if( ( glxFBDepthBits == 24 ) && ( glxFBStencilBits == 8 ) )
		{
			visualConfig.mDepthStencilFormat = EDepthStencilFormat::D24S8;
		}
		else if( ( glxFBDepthBits == 24 ) && ( glxFBStencilBits == 0 ) )
		{
			visualConfig.mDepthStencilFormat = EDepthStencilFormat::D24X8;
		}
		else if( ( glxFBDepthBits == 32 ) && ( glxFBStencilBits == 0 ) )
		{
			visualConfig.mDepthStencilFormat = EDepthStencilFormat::D32F;
		}
		else if( ( glxFBDepthBits == 32 ) && ( glxFBStencilBits == 8 ) )
		{
			visualConfig.mDepthStencilFormat = EDepthStencilFormat::D32FS8;
		}
		else
		{
			visualConfig.mDepthStencilFormat = EDepthStencilFormat::Unknown;
		}

		return visualConfig;
	}

	FrameSize X11OpenGLDisplaySurface::_nativeQueryRenderAreaSize() const
	{
		return Platform::x11GetFrameSize( mNativeData, EFrameSizeMode::ClientArea );
	}

	bool X11OpenGLDisplaySurface::_nativeSysValidate() const
	{
		return ( mNativeData.mWindowXID != Platform::eXIDNone ) && mNativeData.mFBConfig;
	}

	void X11OpenGLDisplaySurface::_nativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode )
	{}

	void X11OpenGLDisplaySurface::_nativeSetFullscreenMode( bool pEnable )
	{
		Platform::x11SetWindowFullscreenState( mNativeData, pEnable );
	}

	void X11OpenGLDisplaySurface::_nativeSetTitle( const std::string & pTitle )
	{
		return Platform::x11SetFrameTitle( mNativeData, pTitle );
	}

	void X11OpenGLDisplaySurface::_nativeUpdateGeometry( const FrameGeometry & pFrameGeometry,
	                                                     TBitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
	{
		return Platform::x11UpdateFrameGeometry( mNativeData, pFrameGeometry, pUpdateFlags );
	}

	FrameSize X11OpenGLDisplaySurface::_nativeGetSize( EFrameSizeMode pSizeMode ) const
	{
		return Platform::x11GetFrameSize( mNativeData, pSizeMode );
	}

	bool X11OpenGLDisplaySurface::_nativeIsFullscreen() const
	{
		return Platform::x11IsFullscreenWindow( mNativeData );
	}


	X11OpenGLRenderContext::X11OpenGLRenderContext( X11OpenGLSystemDriverHandle pGLSystemDriver )
	: X11NativeObject( std::move( pGLSystemDriver ) )
	{}

	X11OpenGLRenderContext::~X11OpenGLRenderContext() noexcept
	{
		_releaseX11ContextState();
	}

	void X11OpenGLRenderContext::_nativeBindForCurrentThread( const OpenGLDisplaySurface & pTargetSurface )
	{
		auto & xSessionData = Platform::x11GetXSessionData( *this );

		const auto * x11DisplaySurface = pTargetSurface.queryInterface<X11OpenGLDisplaySurface>();

		::glXMakeContextCurrent( xSessionData.mDisplay,
		                         x11DisplaySurface->mNativeData.mWindowXID,
		                         x11DisplaySurface->mNativeData.mWindowXID,
		                         mNativeData.mContextHandle );
	}

	bool X11OpenGLRenderContext::_nativeSysCheckIsCurrent() const
	{
		auto currentContextHandle = glXGetCurrentContext();
		return mNativeData.mContextHandle && ( mNativeData.mContextHandle == currentContextHandle );
	}

	bool X11OpenGLRenderContext::_nativeSysValidate() const
	{
		return mNativeData.mContextHandle != nullptr;
	}

	void X11OpenGLRenderContext::_releaseX11ContextState()
	{
		Platform::_x11DestroyGLContext( mNativeData );
	}

	
	namespace Platform
	{

		PFNGLXCREATECONTEXTATTRIBSARBPROC _x11QueryGLXCreateContextAttribsProc()
		{
			static PFNGLXCREATECONTEXTATTRIBSARBPROC cvGLXCreateContextAttribsProc = nullptr;

			if( !cvGLXCreateContextAttribsProc )
			{
				auto procAddress = glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" );
				cvGLXCreateContextAttribsProc = reinterpret_cast<PFNGLXCREATECONTEXTATTRIBSARBPROC>( procAddress );
			}

			return cvGLXCreateContextAttribsProc;
		}

		PFNGLXSWAPINTERVALEXTPROC _x11QueryGLXSwapIntervalProc()
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
			auto & xSessionData = Platform::x11GetXSessionData( pGLSurfaceNativeData );

			X11OpenGLVisualConfig x11GLVisualConfig{};
			if( pVisualConfig.mFlags.isSet( eVisualAttribFlagLegacyBit ) )
			{
				x11GLVisualConfig = _x11ChooseLegacyGLFBConfig( xSessionData.mDisplay, xSessionData.mScreenIndex );
			}
			else
			{
				x11GLVisualConfig = _x11ChooseCoreGLFBConfig( xSessionData.mDisplay, xSessionData.mScreenIndex, pVisualConfig );
			}

			if( !x11GLVisualConfig )
			{
				ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
			}

			pGLSurfaceNativeData.mFBConfig = x11GLVisualConfig.mFBConfig;
			pGLSurfaceNativeData.mVisualInfo = x11GLVisualConfig.mXVisualInfo;

			pWindowCreateInfo.mColorDepth = x11GLVisualConfig.mXVisualInfo->depth;
			pWindowCreateInfo.mWindowVisual = x11GLVisualConfig.mXVisualInfo->visual;

			Platform::x11CreateWindow( pGLSurfaceNativeData, pWindowCreateInfo );
		}

		void _x11DestroyGLWindowAndSurface( X11OpenGLDisplaySurfaceNativeData & pGLSurfaceNativeData )
		{
			Platform::x11DestroyWindow( pGLSurfaceNativeData );
		}

		void _x11DestroyGLContext( X11OpenGLRenderContextNativeData & pGLContextNativeData )
		{
			auto & xSessionData = Platform::x11GetXSessionData( pGLContextNativeData );
			if( pGLContextNativeData.mContextHandle != nullptr )
			{
				auto currentContextHandle = ::glXGetCurrentContext();
				if( pGLContextNativeData.mContextHandle == currentContextHandle )
				{
					::glXMakeContextCurrent( xSessionData.mDisplay, eXIDNone, eXIDNone, nullptr );
				}

				::glXDestroyContext( xSessionData.mDisplay, pGLContextNativeData.mContextHandle );
				pGLContextNativeData.mContextHandle = nullptr;
			}
		}

		void _x11CreateAndBindLegacyRenderContext( X11OpenGLRenderContextNativeData & pGLContextNativeData,
		                                           const X11OpenGLDisplaySurfaceNativeData & pGLSurfaceNativeData )
		{
			auto & xSessionData = Platform::x11GetXSessionData( pGLSurfaceNativeData );

			auto tempContextHandle = ::glXCreateContext( xSessionData.mDisplay, pGLSurfaceNativeData.mVisualInfo, nullptr, True );
			if( !tempContextHandle )
			{
				ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
			}

			pGLContextNativeData.mContextHandle = tempContextHandle;

			auto makeCurrentResult = ::glXMakeCurrent( xSessionData.mDisplay, pGLSurfaceNativeData.mWindowXID, tempContextHandle );
			if( makeCurrentResult == False )
			{
				ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
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
					ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
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
			auto fbConfigList = _x11QueryCompatibleFBConfigList( pDisplay, pScreenIndex, pVisualConfig );

			int bestMatchRate = 0;
			GLXFBConfig bestFBConfig = nullptr;

			for( auto fbConfig : fbConfigList )
			{
				int matchRate = _x11GetFBConfigMatchRate( pDisplay, fbConfig, pVisualConfig );
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

		GLXFBConfigArray _x11QueryCompatibleFBConfigList( XDisplay pDisplay, int pScreenIndex, const VisualConfig & pVisualConfig )
		{
			GLXFBConfigArray result;

			int fbConfigAttribArray[CX_X11_MAX_GLX_FBCONFIG_ATTRIBUTES_NUM];
			_x11GetAttribArrayForVisualConfig( pVisualConfig, fbConfigAttribArray );

			int fbConfigListSize = 0;
			auto * fbConfigList = glXChooseFBConfig( pDisplay, pScreenIndex, fbConfigAttribArray, &fbConfigListSize );

			if( ( fbConfigList == nullptr ) || ( fbConfigListSize == 0 ) )
			{
				ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
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

		int _x11GetFBConfigMatchRate( XDisplay pDisplay, GLXFBConfig pFBConfig, const VisualConfig & pVisualConfig )
		{
			int doubleBufferRequestedState = True;
			int stereoModeRequestedState = False;

			if( pVisualConfig.mFlags.isSet( eVisualAttribFlagSingleBufferBit ) &&
			    !pVisualConfig.mFlags.isSet( eVisualAttribFlagDoubleBufferBit ) )
			{
				doubleBufferRequestedState = False;
			}

			if( pVisualConfig.mFlags.isSet( eVisualAttribFlagStereoDisplayBit ) &&
			    !pVisualConfig.mFlags.isSet( eVisualAttribFlagMonoDisplayBit ) )
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
			matchRate += ( fbConfigAttribValue == pVisualConfig.mDepthStencilDesc.mDepthBufferSize );

			glXGetFBConfigAttrib( pDisplay, pFBConfig, GLX_STENCIL_SIZE, &fbConfigAttribValue );
			matchRate += ( fbConfigAttribValue == pVisualConfig.mDepthStencilDesc.mDepthBufferSize );

			glXGetFBConfigAttrib( pDisplay, pFBConfig, GLX_SAMPLES, &fbConfigAttribValue );
			matchRate += ( fbConfigAttribValue == pVisualConfig.mMSAADesc.mQuality );

			auto * fbConfigXVisualInfo =  glXGetVisualFromFBConfig( pDisplay, pFBConfig );
			matchRate += ( fbConfigXVisualInfo->bits_per_rgb == 8 );
			XFree( fbConfigXVisualInfo );

			return matchRate;
		}

		void _x11GetAttribArrayForVisualConfig( const VisualConfig & pVisualConfig, int * pAttribArray )
		{
			int attribIndex = 0;

			ic3X11OpenGLContextAttribAppend( pAttribArray, attribIndex, GLX_DRAWABLE_TYPE );
			ic3X11OpenGLContextAttribAppend( pAttribArray, attribIndex, GLX_WINDOW_BIT );

			ic3X11OpenGLContextAttribAppend( pAttribArray, attribIndex, GLX_RENDER_TYPE );
			ic3X11OpenGLContextAttribAppend( pAttribArray, attribIndex, GLX_RGBA_BIT );

			ic3X11OpenGLContextAttribAppend( pAttribArray, attribIndex, GLX_X_RENDERABLE );
			ic3X11OpenGLContextAttribAppend( pAttribArray, attribIndex, True );

			ic3X11OpenGLContextAttribAppend( pAttribArray, attribIndex, GLX_X_VISUAL_TYPE );
			ic3X11OpenGLContextAttribAppend( pAttribArray, attribIndex, GLX_TRUE_COLOR );

			if( pVisualConfig.mFlags.isSet( eVisualAttribFlagDoubleBufferBit ) )
			{
				pAttribArray[attribIndex++] = GLX_DOUBLEBUFFER;
				pAttribArray[attribIndex++] = True;
			}
			else if( pVisualConfig.mFlags.isSet( eVisualAttribFlagSingleBufferBit ) )
			{
				pAttribArray[attribIndex++] = GLX_DOUBLEBUFFER;
				pAttribArray[attribIndex++] = False;
			}

			if( pVisualConfig.mFlags.isSet( eVisualAttribFlagMonoDisplayBit ) )
			{
				pAttribArray[attribIndex++] = GLX_STEREO;
				pAttribArray[attribIndex++] = False;
			}
			else if( pVisualConfig.mFlags.isSet( eVisualAttribFlagStereoDisplayBit ) )
			{
				pAttribArray[attribIndex++] = GLX_STEREO;
				pAttribArray[attribIndex++] = True;
			}

			if( ( pVisualConfig.mMSAADesc.mBufferCount != 0 ) && ( pVisualConfig.mMSAADesc.mQuality != 0 ) )
			{
				pAttribArray[attribIndex++] = GLX_SAMPLE_BUFFERS;
				pAttribArray[attribIndex++] = pVisualConfig.mMSAADesc.mBufferCount;

				pAttribArray[attribIndex++] = GLX_SAMPLES;
				pAttribArray[attribIndex++] = pVisualConfig.mMSAADesc.mQuality;
			}

			if( pVisualConfig.mColorDesc.mRGBA.mU32Code != 0 )
			{
				pAttribArray[attribIndex++] = GLX_RED_SIZE;
				pAttribArray[attribIndex++] = pVisualConfig.mColorDesc.mRGBA.mU8Red;

				pAttribArray[attribIndex++] = GLX_GREEN_SIZE;
				pAttribArray[attribIndex++] = pVisualConfig.mColorDesc.mRGBA.mU8Green;

				pAttribArray[attribIndex++] = GLX_BLUE_SIZE;
				pAttribArray[attribIndex++] = pVisualConfig.mColorDesc.mRGBA.mU8Blue;

				pAttribArray[attribIndex++] = GLX_ALPHA_SIZE;
				pAttribArray[attribIndex++] = pVisualConfig.mColorDesc.mRGBA.mU8Alpha;
			}

			if( pVisualConfig.mDepthStencilDesc.mDepthBufferSize != 0 )
			{
				pAttribArray[attribIndex++] = GLX_DEPTH_SIZE;
				pAttribArray[attribIndex++] = pVisualConfig.mDepthStencilDesc.mDepthBufferSize;
			}

			if( pVisualConfig.mDepthStencilDesc.mStencilBufferSize != 0 )
			{
				pAttribArray[attribIndex++] = GLX_STENCIL_SIZE;
				pAttribArray[attribIndex++] = pVisualConfig.mDepthStencilDesc.mStencilBufferSize;
			}

			pAttribArray[attribIndex++] = eXIDNone;
		}

	}

} // namespace Ic3::System
#endif // IC3_PCL_TARGET_SYSAPI_X11
