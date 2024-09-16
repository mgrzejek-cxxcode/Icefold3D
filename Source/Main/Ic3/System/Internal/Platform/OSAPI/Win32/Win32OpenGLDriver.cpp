
#include "Win32OpenGLDriver.h"

#if( IC3_PCL_TARGET_SYSAPI == IC3_PCL_TARGET_SYSAPI_WIN32 )
namespace Ic3::System
{

	namespace Platform
	{

		// Creates Win32 OpenGL surface using provided visual config.
		void _win32CreateGLSurface( Win32OpenGLDisplaySurfaceNativeData & pGLSurfaceNativeData, const VisualConfig & pVisualConfig );

		// Destroys existing surface.
		void _win32DestroyGLSurface( Win32OpenGLDisplaySurfaceNativeData & pGLSurfaceNativeData );

		// Destroys existing context.
		void _win32DestroyGLContext( Win32OpenGLRenderContextNativeData & pGLContextNativeData );

		// Selects matching pixel format for surface described with a PFD. Uses legacy API.
		int _win32ChooseLegacyGLPixelFormat( HDC pDisplaySurfaceDC, PIXELFORMATDESCRIPTOR & pPfmtDescriptor );

		// Selects matching pixel format for surface described with a VisualConfig. Uses new EXT API and supports stuff like MSAA.
		int _win32ChooseCoreGLPixelFormat( HDC pDisplaySurfaceDC, const VisualConfig & pVisualConfig, PIXELFORMATDESCRIPTOR & pPfmtDescriptor );

		// Returns an array of pixel format IDs matching specified set of Win32 PF attributes.
		std::vector<int> _win32QueryCompatiblePixelFormatList( HDC pDisplaySurfaceDC, int * pPixelFormatAttribList = nullptr );

		// Returns an array of pixel format IDs matching specified VisualConfig structure.
		std::vector<int> _win32QueryCompatiblePixelFormatList( HDC pDisplaySurfaceDC, const VisualConfig & pVisualConfig );

		// Computes a "compatibility rate", i.e. how much the specified pixel format matches the visual.
		int _win32GetPixelFormatMatchRate( HDC pDisplaySurfaceDC, int pPixelFormatIndex, const VisualConfig & pVisualConfig );

		// Translation: VisualConfig --> array of WGL_* attributes required by the system API. Used for surface/context creation.
		void _win32GetWGLAttribArrayForVisualConfig( const VisualConfig & pVisualConfig, int * pAttribArray );

		// Useful utility. Fetches values for a set of PF attributes. Uses templated array definition to resolve the size.
		template <size_t tSize, typename TOutput>
		inline bool _win32QueryPixelFormatAttributes( HDC pDisplaySurfaceDC, int pPixelFormatIndex, const int( &pAttributes )[tSize], TOutput & pOutput )
		{
			BOOL result = wglGetPixelFormatAttribivARB( pDisplaySurfaceDC, pPixelFormatIndex, 0, tSize, pAttributes, &( pOutput[0] ) );
			return result != FALSE;
		}

	}
	
	
	Win32OpenGLSystemDriver::Win32OpenGLSystemDriver( DisplayManagerHandle pDisplayManager )
	: Win32NativeObject( std::move( pDisplayManager ) )
	{}

	Win32OpenGLSystemDriver::~Win32OpenGLSystemDriver() noexcept
	{
		_releaseWin32DriverState();
	}

	void Win32OpenGLSystemDriver::_releaseWin32DriverState()
	{
		_nativeReleaseInitState();
	}
		
	void Win32OpenGLSystemDriver::_nativeInitializePlatform()
	{
		ic3DebugAssert( !mNativeData.initState );
		// Init state should be first created here and destroyed as soon as proper GL
		// contexts are created (this is not enforce, though, and controlled explicitly
		// by the user and done by calling releaseInitState() method od the driver).
		mNativeData.initState = std::make_unique<Platform::Win32OpenGLSystemDriverNativeData::InitState>();

		WindowCreateInfo tempWindowCreateInfo;
		tempWindowCreateInfo.mFrameGeometry.position = { 0, 0 };
		tempWindowCreateInfo.mFrameGeometry.size = { 600, 600 };
		tempWindowCreateInfo.mFrameGeometry.style = EFrameStyle::OVERLAY;

		VisualConfig legacyVisualConfig;
		legacyVisualConfig = vsxGetDefaultVisualConfigForSysWindow();
		legacyVisualConfig.flags.set( E_VISUAL_ATTRIB_FLAG_LEGACYBit );

		auto & tmpSurfaceNativeData = mNativeData.initState->surfaceData;

		// Create a surface window. In case of Win32, Win32OpenGLDisplaySurfaceNativeData inherits from WindowNativeData
		// for this very reason: to allow treating surfaces as windows (as that's exactly the case on desktop).
		Platform::win32CreateWindow( tmpSurfaceNativeData, tempWindowCreateInfo );

		// Create a surface. This sets up the PFD and configures HDC properly.
		_win32CreateGLSurface( tmpSurfaceNativeData, legacyVisualConfig );

		auto & tmpContextNativeData = mNativeData.initState->contextData;

		// Create legacy OpenGL context for initialization phase.
		tmpContextNativeData.contextHandle = ::wglCreateContext( tmpSurfaceNativeData.hdc );
		if( tmpContextNativeData.contextHandle == nullptr )
		{
			ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
		}

		// Bind context as current, so GL calls may be used normally.
		BOOL makeCurrentResult = ::wglMakeCurrent( tmpSurfaceNativeData.hdc, tmpContextNativeData.contextHandle );
		if( makeCurrentResult == FALSE )
		{
			ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
		}

		auto glewResult = glewInit();
		if( glewResult != GLEW_OK )
		{
			ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
		}

		glewResult = wglewInit();
		if( glewResult != GLEW_OK )
		{
			ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
		}
	}

	void Win32OpenGLSystemDriver::_nativeReleaseInitState() noexcept
	{
		if( !mNativeData.initState )
		{
			return;
		}

		auto & tmpContextNativeData = mNativeData.initState->contextData;
		::wglDeleteContext( tmpContextNativeData.contextHandle );

		auto & tmpSurfaceNativeData = mNativeData.initState->surfaceData;
		_win32DestroyGLSurface( tmpSurfaceNativeData );
		Platform::win32DestroyWindow( tmpSurfaceNativeData );

		mNativeData.initState.reset();
	}

	OpenGLDisplaySurfaceHandle Win32OpenGLSystemDriver::_nativeCreateDisplaySurface( const OpenGLDisplaySurfaceCreateInfo & pCreateInfo )
	{
		auto displaySurface = createSysObject<Win32OpenGLDisplaySurface>( getHandle<Win32OpenGLSystemDriver>() );

		WindowCreateInfo surfaceWindowCreateInfo;
		surfaceWindowCreateInfo.mFrameGeometry = pCreateInfo.mFrameGeometry;
		surfaceWindowCreateInfo.mTitle = "TS3 OpenGL Window";

		Platform::win32CreateWindow( displaySurface->mNativeData, surfaceWindowCreateInfo );

		_win32CreateGLSurface( displaySurface->mNativeData, pCreateInfo.mVisualConfig );

		if( pCreateInfo.flags.isSet( E_OPENGL_DISPLAY_SURFACE_CREATE_FLAG_SYNC_DISABLEDBit ) )
		{
			wglSwapIntervalEXT( 0 );
		}
		else if( pCreateInfo.flags.isSet( E_OPENGL_DISPLAY_SURFACE_CREATE_FLAG_SYNC_ADAPTIVEBit ) )
		{
			wglSwapIntervalEXT( -1 );
		}
		else if( pCreateInfo.flags.isSet( E_OPENGL_DISPLAY_SURFACE_CREATE_FLAG_SYNC_VERTICALBit ) )
		{
			wglSwapIntervalEXT( 1 );
		}

		::ShowWindow( displaySurface->mNativeData.mHWND, SW_SHOWNORMAL );

		// TODO: Workaround to properly work with current engine's implementation. That should be turned into an explicit flag.
		if( pCreateInfo.flags.isSet( E_OPENGL_DISPLAY_SURFACE_CREATE_FLAG_FULLSCREENBit ) )
		{
			::SetCapture( displaySurface->mNativeData.mHWND );
			::ShowCursor( FALSE );
		}

		return displaySurface;
	}

	OpenGLDisplaySurfaceHandle Win32OpenGLSystemDriver::_nativeCreateDisplaySurfaceForCurrentThread()
	{
		auto currentDC = ::wglGetCurrentDC();
		if( currentDC == nullptr )
		{
			return nullptr;
		}

		auto displaySurface = createSysObject<Win32OpenGLDisplaySurface>( getHandle<Win32OpenGLSystemDriver>() );

		displaySurface->mNativeData.hdc = currentDC;
		displaySurface->mNativeData.mHWND = ::WindowFromDC( currentDC );
		displaySurface->mNativeData.pixelFormatIndex = ::GetPixelFormat( currentDC );

		CHAR surfaceWindowClassName[256];
		::GetClassNameA( displaySurface->mNativeData.mHWND, surfaceWindowClassName, 255 );
		auto wndProcModuleHandle = ::GetWindowLongPtrA( displaySurface->mNativeData.mHWND, GWLP_HINSTANCE );

		displaySurface->mNativeData.wndClsName = surfaceWindowClassName;
		displaySurface->mNativeData.moduleHandle = reinterpret_cast<HMODULE>( wndProcModuleHandle );

		return displaySurface;
	}

	void Win32OpenGLSystemDriver::_nativeDestroyDisplaySurface( OpenGLDisplaySurface & pDisplaySurface )
	{
	}

	OpenGLRenderContextHandle Win32OpenGLSystemDriver::_nativeCreateRenderContext( OpenGLDisplaySurface & pDisplaySurface,
	                                                                               const OpenGLRenderContextCreateInfo & pCreateInfo )
	{
		auto * win32DisplaySurface = pDisplaySurface.queryInterface<Win32OpenGLDisplaySurface>();

		int contextAPIProfile = 0;
		TBitmask<int> contextCreateFlags = 0;
		HGLRC shareContextHandle = nullptr;

		if( pCreateInfo.contextAPIProfile == EOpenGLAPIProfile::Core )
		{
			contextAPIProfile = WGL_CONTEXT_CORE_PROFILEBit_ARB;
		}
		else if( pCreateInfo.contextAPIProfile == EOpenGLAPIProfile::Legacy )
		{
			contextAPIProfile = WGL_CONTEXT_COMPATIBILITY_PROFILEBit_ARB;
		}
		else if( pCreateInfo.contextAPIProfile == EOpenGLAPIProfile::GLES )
		{
			contextAPIProfile = WGL_CONTEXT_ES_PROFILEBit_EXT;
		}

		if( pCreateInfo.flags.isSet( E_OPENGL_RENDER_CONTEXT_CREATE_FLAG_ENABLE_DEBUGBit ) )
		{
			contextCreateFlags |= WGL_CONTEXT_DEBUGBit_ARB;
		}
		if( pCreateInfo.flags.isSet( E_OPENGL_RENDER_CONTEXT_CREATE_FLAG_FORWARD_COMPATIBLEBit ) )
		{
			contextCreateFlags |= WGL_CONTEXT_FORWARD_COMPATIBLEBit_ARB;
		}
		if( pCreateInfo.flags.isSet( E_OPENGL_RENDER_CONTEXT_CREATE_FLAG_ENABLE_SHARINGBit ) )
		{
			if( pCreateInfo.shareContext )
			{
				auto * win32ShareContext = pCreateInfo.shareContext->queryInterface<Win32OpenGLRenderContext>();
				shareContextHandle = win32ShareContext->mNativeData.contextHandle;
			}
			else if( pCreateInfo.flags.isSet( E_OPENGL_RENDER_CONTEXT_CREATE_FLAG_SHARE_WITH_CURRENTBit ) )
			{
				if( auto * currentWGLContext = ::wglGetCurrentContext() )
				{
					shareContextHandle = currentWGLContext;
				}
			}
		}

		const int contextAttributes[] =
		{
			// Requested OpenGL API version: major part
			WGL_CONTEXT_MAJOR_VERSION_ARB, pCreateInfo.requestedAPIVersion.mNumMajor,
			// Requested OpenGL API version: minor part
			WGL_CONTEXT_MINOR_VERSION_ARB, pCreateInfo.requestedAPIVersion.mNumMinor,
			//
			WGL_CONTEXT_PROFILE_MASK_ARB, contextAPIProfile,
			//
			WGL_CONTEXT_FLAGS_ARB, contextCreateFlags,
			// Terminator
			FALSE
		};

		HGLRC contextHandle = wglCreateContextAttribsARB( win32DisplaySurface->mNativeData.hdc,
		                                                  shareContextHandle,
		                                                  &( contextAttributes[0] ) );

		if( !contextHandle )
		{
			ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
		}

		auto renderContext = createSysObject<Win32OpenGLRenderContext>( getHandle<Win32OpenGLSystemDriver>() );
		renderContext->mNativeData.contextHandle = contextHandle;

		return renderContext;
	}

	OpenGLRenderContextHandle Win32OpenGLSystemDriver::_nativeCreateRenderContextForCurrentThread()
	{
		auto contextHandle = ::wglGetCurrentContext();
		if( contextHandle == nullptr )
		{
			return nullptr;
		}

		auto renderContext = createSysObject<Win32OpenGLRenderContext>( getHandle<Win32OpenGLSystemDriver>() );
		renderContext->mNativeData.contextHandle = contextHandle;

		return renderContext;
	}

	void Win32OpenGLSystemDriver::_nativeDestroyRenderContext( OpenGLRenderContext & pRenderContext )
	{
	}

	void Win32OpenGLSystemDriver::_nativeResetContextBinding()
	{
		::wglMakeCurrent( nullptr, nullptr );
	}

	std::vector<EDepthStencilFormat> Win32OpenGLSystemDriver::_nativeQuerySupportedDepthStencilFormats( EColorFormat pColorFormat ) const
	{
		return {};
	}

	std::vector<EMSAAMode> Win32OpenGLSystemDriver::_nativeQuerySupportedMSAAModes( EColorFormat pColorFormat,
	                                                                                EDepthStencilFormat pDepthStencilFormat ) const
	{
		return {};
	}

	bool Win32OpenGLSystemDriver::_nativeIsAPIClassSupported( EOpenGLAPIClass pAPIClass ) const
	{
		if( pAPIClass == EOpenGLAPIClass::OpenGLDesktop )
		{
			return true;
		}
		return false;
	}

	bool Win32OpenGLSystemDriver::_nativeIsRenderContextBound() const
	{
		auto currentContext = ::wglGetCurrentContext();
		return currentContext != nullptr;
	}
	
	
	Win32OpenGLDisplaySurface::Win32OpenGLDisplaySurface( Win32OpenGLSystemDriverHandle pGLSystemDriver )
	: Win32NativeObject( std::move( pGLSystemDriver ), &mNativeData )
	{}
	
	Win32OpenGLDisplaySurface::~Win32OpenGLDisplaySurface() noexcept
	{
		_releaseWin32SurfaceState();
	}

	void Win32OpenGLDisplaySurface::_releaseWin32SurfaceState()
	{
		Platform::_win32DestroyGLSurface( mNativeData );
		Platform::win32DestroyWindow( mNativeData );
	}

	void Win32OpenGLDisplaySurface::_nativeSwapBuffers()
	{
		::SwapBuffers( mNativeData.hdc );
	}

	EOpenGLAPIClass Win32OpenGLDisplaySurface::_nativeQuerySupportedAPIClass() const noexcept
	{
		return EOpenGLAPIClass::OpenGLDesktop;
	}

	VisualConfig Win32OpenGLDisplaySurface::_nativeQueryVisualConfig() const
	{
		int surfacePixelFormat = ::GetPixelFormat( mNativeData.hdc );

		PIXELFORMATDESCRIPTOR pixelFormatDesc;
		pixelFormatDesc.nSize = sizeof( PIXELFORMATDESCRIPTOR );

		::DescribePixelFormat(
			mNativeData.hdc,
			surfacePixelFormat,
			sizeof( PIXELFORMATDESCRIPTOR ),
			&pixelFormatDesc );

		VisualConfig visualConfig;

		if( pixelFormatDesc.cColorBits == 32 )
		{
			visualConfig.colorFormat = EColorFormat::B8G8R8A8;
		}
		else
		{
			visualConfig.colorFormat = EColorFormat::Unknown;
		}

		if( ( pixelFormatDesc.cDepthBits == 24 ) && ( pixelFormatDesc.cStencilBits == 8 ) )
		{
			visualConfig.depthStencilFormat = EDepthStencilFormat::D24S8;
		}
		else if( ( pixelFormatDesc.cDepthBits == 24 ) && ( pixelFormatDesc.cStencilBits == 0 ) )
		{
			visualConfig.depthStencilFormat = EDepthStencilFormat::D24X8;
		}
		else if( ( pixelFormatDesc.cDepthBits == 32 ) && ( pixelFormatDesc.cStencilBits == 0 ) )
		{
			visualConfig.depthStencilFormat = EDepthStencilFormat::D32F;
		}
		else if( ( pixelFormatDesc.cDepthBits == 32 ) && ( pixelFormatDesc.cStencilBits == 8 ) )
		{
			visualConfig.depthStencilFormat = EDepthStencilFormat::D32FS8;
		}
		else
		{
			visualConfig.depthStencilFormat = EDepthStencilFormat::Unknown;
		}

		return visualConfig;
	}

	FrameSize Win32OpenGLDisplaySurface::_nativeQueryRenderAreaSize() const
	{
		return Platform::win32GetFrameSize( mNativeData.mHWND, EFrameSizeMode::CLIENT_AREA );
	}

	bool Win32OpenGLDisplaySurface::_nativeSysValidate() const
	{
		return mNativeData.mHWND && mNativeData.hdc;
	}

	void Win32OpenGLDisplaySurface::_nativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode )
	{}

	void Win32OpenGLDisplaySurface::_nativeSetFullscreenMode( bool pEnable )
	{
		Platform::win32ChangeWindowFullscreenState( mNativeData, pEnable );
	}

	void Win32OpenGLDisplaySurface::_nativeSetTitle( const std::string & pTitle )
	{
		Platform::win32SetFrameTitle( mNativeData.mHWND, pTitle );
	}

	void Win32OpenGLDisplaySurface::_nativeUpdateGeometry( const FrameGeometry & pFrameGeometry,
	                                                       TBitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
	{
		Platform::win32UpdateFrameGeometry( mNativeData.mHWND, pFrameGeometry, pUpdateFlags );
	}

	FrameSize Win32OpenGLDisplaySurface::_nativeGetSize( EFrameSizeMode pSizeMode ) const
	{
		return Platform::win32GetFrameSize( mNativeData.mHWND, pSizeMode );
	}

    bool Win32OpenGLDisplaySurface::_nativeIsFullscreen() const
    {
        return Platform::win32IsFullscreenWindow( mNativeData );
    }


	Win32OpenGLRenderContext::Win32OpenGLRenderContext( Win32OpenGLSystemDriverHandle pGLSystemDriver )
	: Win32NativeObject( std::move( pGLSystemDriver ) )
	{}

	Win32OpenGLRenderContext::~Win32OpenGLRenderContext() noexcept
	{
		_releaseWin32ContextState();
	}
	
	void Win32OpenGLRenderContext::_nativeBindForCurrentThread( const OpenGLDisplaySurface & pTargetSurface )
	{
		const auto * win32DisplaySurface = pTargetSurface.queryInterface<Win32OpenGLDisplaySurface>();
		::wglMakeCurrent( win32DisplaySurface->mNativeData.hdc, mNativeData.contextHandle );
	}

	bool Win32OpenGLRenderContext::_nativeSysCheckIsCurrent() const
	{
		auto currentContextHandle = ::wglGetCurrentContext();
		return mNativeData.contextHandle && ( mNativeData.contextHandle == currentContextHandle );
	}

	bool Win32OpenGLRenderContext::_nativeSysValidate() const
	{
		return mNativeData.contextHandle != nullptr;
	}

	void Win32OpenGLRenderContext::_releaseWin32ContextState()
	{
		Platform::_win32DestroyGLContext( mNativeData );
	}


	namespace Platform
	{

		void _win32CreateGLSurface( Win32OpenGLDisplaySurfaceNativeData & pGLSurfaceNativeData,
		                            const VisualConfig & pVisualConfig )
		{
			auto hdc = ::GetWindowDC( pGLSurfaceNativeData.mHWND );
			pGLSurfaceNativeData.hdc = hdc;

			PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
			memset( &pixelFormatDescriptor, 0, sizeof( PIXELFORMATDESCRIPTOR ) );
			pixelFormatDescriptor.nSize = sizeof( PIXELFORMATDESCRIPTOR );
			pixelFormatDescriptor.nVersion = 1;

			if( pVisualConfig.flags.isSet( E_VISUAL_ATTRIB_FLAG_LEGACYBit ) )
			{
				pGLSurfaceNativeData.pixelFormatIndex = _win32ChooseLegacyGLPixelFormat( hdc, pixelFormatDescriptor );
			}
			else
			{
				pGLSurfaceNativeData.pixelFormatIndex = _win32ChooseCoreGLPixelFormat( hdc, pVisualConfig, pixelFormatDescriptor );
			}

			BOOL spfResult = ::SetPixelFormat( hdc, pGLSurfaceNativeData.pixelFormatIndex, &pixelFormatDescriptor );
			if( spfResult == FALSE )
			{
				ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
			}
		}

		void _win32DestroyGLSurface( Win32OpenGLDisplaySurfaceNativeData & pGLSurfaceNativeData )
		{
			if( pGLSurfaceNativeData.hdc != nullptr )
			{
				::ReleaseDC( pGLSurfaceNativeData.mHWND, pGLSurfaceNativeData.hdc );

				pGLSurfaceNativeData.hdc = nullptr;
				pGLSurfaceNativeData.pixelFormatIndex = 0;
			}
		}

		void _win32DestroyGLContext( Win32OpenGLRenderContextNativeData & pGLContextNativeData )
		{
			if( pGLContextNativeData.contextHandle != nullptr )
			{
				auto currentContextHandle = ::wglGetCurrentContext();
				if( pGLContextNativeData.contextHandle == currentContextHandle )
				{
					// Explicitly unbind the context if it's currently bound.
					// TODO: What about multi-threaded scenario? Is it safe?
					::wglMakeCurrent( nullptr, nullptr );
				}

				::wglDeleteContext( pGLContextNativeData.contextHandle );

				pGLContextNativeData.contextHandle = nullptr;
			}
		}

		int _win32ChooseLegacyGLPixelFormat( HDC pDisplaySurfaceDC, PIXELFORMATDESCRIPTOR & pPfmtDescriptor )
		{
			// For legacy pixel format, we first try the most reasonable one:
			// 24-bit color (RGB8) with 8-bit alpha channel and 32-bit combined
			// depth/stencil buffer. That should be the best pick in most cases.
			pPfmtDescriptor.cRedBits = 8;
			pPfmtDescriptor.cGreenBits = 8;
			pPfmtDescriptor.cBlueBits = 8;
			pPfmtDescriptor.cAlphaBits = 8;
			pPfmtDescriptor.cDepthBits = 24;
			pPfmtDescriptor.cStencilBits = 8;
			pPfmtDescriptor.iLayerType = PFD_MAIN_PLANE;
			pPfmtDescriptor.iPixelType = PFD_TYPE_RGBA;
			pPfmtDescriptor.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;

			int pixelFormatIndex = ::ChoosePixelFormat( pDisplaySurfaceDC, &pPfmtDescriptor );

			if( pixelFormatIndex == 0 )
			{
				pPfmtDescriptor.cColorBits = 24; // 24-bit color, no alpha, no explicit RGB
				pPfmtDescriptor.cDepthBits = 32; // 32-bit depth buffer, no stencil attachment
				pixelFormatIndex = ::ChoosePixelFormat( pDisplaySurfaceDC, &pPfmtDescriptor );
			}

			return ( pixelFormatIndex > 0 ) ? pixelFormatIndex : -1;
		}

		int _win32ChooseCoreGLPixelFormat( HDC pDisplaySurfaceDC, const VisualConfig & pVisualConfig, PIXELFORMATDESCRIPTOR & pPfmtDescriptor )
		{
			auto pixelFormatList = _win32QueryCompatiblePixelFormatList( pDisplaySurfaceDC, pVisualConfig );

			int bestMatchRate = 0;
			int bestPixelFormatID = 0;

			for( auto pixelFormatID : pixelFormatList )
			{
				int matchRate = _win32GetPixelFormatMatchRate( pDisplaySurfaceDC, pixelFormatID, pVisualConfig );
				if( matchRate > bestMatchRate )
				{
					bestMatchRate = matchRate;
					bestPixelFormatID = pixelFormatID;
				}
			}

			int maxPixelFormatIndex = ::DescribePixelFormat( pDisplaySurfaceDC, bestPixelFormatID, sizeof( PIXELFORMATDESCRIPTOR ), &pPfmtDescriptor );

			return ( maxPixelFormatIndex > 0 ) ? bestPixelFormatID : -1;
		}

		std::vector<int> _win32QueryCompatiblePixelFormatList( HDC pDisplaySurfaceDC, int * pPixelFormatAttribList )
		{
			// Output array where system will store IDs of enumerated pixel formats.
			int pixelFormatArray[CX_WIN32_WGL_MAX_PIXEL_FORMATS_NUM];
			// Number of pixel formats returned by the system.
			UINT returnedPixelFormatsNum = 0U;

			// Enumerate pixel formats.
			BOOL enumResult = ::wglChoosePixelFormatARB( pDisplaySurfaceDC,
			                                             pPixelFormatAttribList,
			                                             nullptr,
			                                             CX_WIN32_WGL_MAX_PIXEL_FORMATS_NUM,
			                                             pixelFormatArray,
			                                             &returnedPixelFormatsNum );

			if( ( enumResult == FALSE ) || ( returnedPixelFormatsNum == 0 ) )
			{
				ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
			}

			std::vector<int> result;
			result.reserve( returnedPixelFormatsNum );
			result.assign( &( pixelFormatArray[0] ), &( pixelFormatArray[returnedPixelFormatsNum] ) );

			return result;
		}

		std::vector<int> _win32QueryCompatiblePixelFormatList( HDC pDisplaySurfaceDC, const VisualConfig & pVisualConfig )
		{
			// Array for WGL_..._ARB pixel format attributes.
			int pixelFormatAttributes[CX_WIN32_WGL_MAX_PIXEL_FORMAT_ATTRIBUTES_NUM * 2];
			// Translate provided visual config to a WGL attribute array.
			_win32GetWGLAttribArrayForVisualConfig( pVisualConfig, pixelFormatAttributes );

			return _win32QueryCompatiblePixelFormatList( pDisplaySurfaceDC, pixelFormatAttributes );
		}

		int _win32GetPixelFormatMatchRate( HDC pDisplaySurfaceDC, int pPixelFormatIndex, const VisualConfig & pVisualConfig )
		{
			int doubleBufferRequestedState = TRUE;
			int stereoModeRequestedState = FALSE;

			if( pVisualConfig.flags.isSet( E_VISUAL_ATTRIB_FLAG_SINGLE_BUFFERBit ) &&
			!pVisualConfig.flags.isSet( E_VISUAL_ATTRIB_FLAG_DOUBLE_BUFFERBit ) )
			{
				doubleBufferRequestedState = FALSE;
			}

			if( pVisualConfig.flags.isSet( E_VISUAL_ATTRIB_FLAG_STEREO_DISPLAYBit ) &&
			!pVisualConfig.flags.isSet( E_VISUAL_ATTRIB_FLAG_MONO_DISPLAYBit ) )
			{
				stereoModeRequestedState = TRUE;
			}

			constexpr int controlAttribArray[] =
			{
				WGL_DOUBLE_BUFFER_ARB,
				WGL_STEREO_ARB,
				WGL_DEPTHBitS_ARB,
				WGL_STENCILBitS_ARB,
				WGL_SAMPLES_ARB
			};

			std::array<int, staticArraySize( controlAttribArray )> attribValueArray{};
			if( !_win32QueryPixelFormatAttributes( pDisplaySurfaceDC, pPixelFormatIndex, controlAttribArray, attribValueArray ) )
			{
				return -1;
			}

			int matchRate = 0;
			int attribIndex = 0;

			if( attribValueArray[attribIndex++] == doubleBufferRequestedState )
			{
				++matchRate;
			}
			if( attribValueArray[attribIndex++] == stereoModeRequestedState )
			{
				++matchRate;
			}
			if( attribValueArray[attribIndex++] == pVisualConfig.depthStencilDesc.depthBufferSize )
			{
				++matchRate;
			}
			if( attribValueArray[attribIndex++] == pVisualConfig.depthStencilDesc.stencilBufferSize )
			{
				++matchRate;
			}
			if( attribValueArray[attribIndex] == pVisualConfig.msaaDesc.quality )
			{
				++matchRate;
			}

			return matchRate;
		}

		void _win32GetWGLAttribArrayForVisualConfig( const VisualConfig & pVisualConfig, int * pAttribArray )
		{
			int attribIndex = 0;

			pAttribArray[attribIndex++] = WGL_SUPPORT_OPENGL_ARB;
			pAttribArray[attribIndex++] = GL_TRUE;

			pAttribArray[attribIndex++] = WGL_DRAW_TO_WINDOW_ARB;
			pAttribArray[attribIndex++] = GL_TRUE;

			pAttribArray[attribIndex++] = WGL_ACCELERATION_ARB;
			pAttribArray[attribIndex++] = WGL_FULL_ACCELERATION_ARB;

			pAttribArray[attribIndex++] = WGL_PIXEL_TYPE_ARB;
			pAttribArray[attribIndex++] = WGL_TYPE_RGBA_ARB;

			if( pVisualConfig.flags.isSet( E_VISUAL_ATTRIB_FLAG_DOUBLE_BUFFERBit ) )
			{
				pAttribArray[attribIndex++] = WGL_DOUBLE_BUFFER_ARB;
				pAttribArray[attribIndex++] = GL_TRUE;
			}
			else if( pVisualConfig.flags.isSet( E_VISUAL_ATTRIB_FLAG_SINGLE_BUFFERBit ) )
			{
				pAttribArray[attribIndex++] = WGL_DOUBLE_BUFFER_ARB;
				pAttribArray[attribIndex++] = GL_FALSE;
			}

			if( pVisualConfig.flags.isSet( E_VISUAL_ATTRIB_FLAG_MONO_DISPLAYBit ) )
			{
				pAttribArray[attribIndex++] = WGL_STEREO_ARB;
				pAttribArray[attribIndex++] = GL_FALSE;
			}
			else if( pVisualConfig.flags.isSet( E_VISUAL_ATTRIB_FLAG_STEREO_DISPLAYBit ) )
			{
				pAttribArray[attribIndex++] = WGL_STEREO_ARB;
				pAttribArray[attribIndex++] = GL_TRUE;
			}

			if( pVisualConfig.flags.isSet( E_VISUAL_ATTRIB_FLAG_SRGB_CAPABLEBit ) )
			{
				pAttribArray[attribIndex++] = WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB;
				pAttribArray[attribIndex++] = GL_TRUE;
			}

			if( pVisualConfig.colorDesc.rgba.mU32Code != 0 )
			{
				pAttribArray[attribIndex++] = WGL_REDBitS_ARB;
				pAttribArray[attribIndex++] = pVisualConfig.colorDesc.rgba.mU8Red;

				pAttribArray[attribIndex++] = WGL_GREENBitS_ARB;
				pAttribArray[attribIndex++] = pVisualConfig.colorDesc.rgba.mU8Green;

				pAttribArray[attribIndex++] = WGL_BLUEBitS_ARB;
				pAttribArray[attribIndex++] = pVisualConfig.colorDesc.rgba.mU8Blue;

				pAttribArray[attribIndex++] = WGL_ALPHABitS_ARB;
				pAttribArray[attribIndex++] = pVisualConfig.colorDesc.rgba.mU8Alpha;
			}

			if( ( pVisualConfig.msaaDesc.bufferCount != 0 ) && ( pVisualConfig.msaaDesc.quality != 0 ) )
			{
				pAttribArray[attribIndex++] = WGL_SAMPLE_BUFFERS_ARB;
				pAttribArray[attribIndex++] = pVisualConfig.msaaDesc.bufferCount;

				pAttribArray[attribIndex++] = WGL_SAMPLES_ARB;
				pAttribArray[attribIndex++] = pVisualConfig.msaaDesc.quality;
			}

			if( pVisualConfig.depthStencilDesc.depthBufferSize != 0 )
			{
				pAttribArray[attribIndex++] = WGL_DEPTHBitS_ARB;
				pAttribArray[attribIndex++] = pVisualConfig.depthStencilDesc.depthBufferSize;
			}

			if( pVisualConfig.depthStencilDesc.stencilBufferSize != 0 )
			{
				pAttribArray[attribIndex++] = WGL_STENCILBitS_ARB;
				pAttribArray[attribIndex++] = pVisualConfig.depthStencilDesc.stencilBufferSize;
			}

			// Append zero as a list terminator.
			pAttribArray[attribIndex] = 0;
		}

	}

} // namespace Ic3::System
#endif // IC3_PCL_TARGET_SYSAPI_WIN32
