
#include "Win32OpenGLDriver.h"

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_WIN32 )
namespace Ic3::System
{

	namespace Platform
	{

		// Creates Win32 OpenGL surface using provided visual config.
		void _Win32CreateGLSurface( Win32OpenGLDisplaySurfaceNativeData & pGLSurfaceNativeData, const VisualConfig & pVisualConfig );

		// Destroys existing surface.
		void _Win32DestroyGLSurface( Win32OpenGLDisplaySurfaceNativeData & pGLSurfaceNativeData );

		// Destroys existing context.
		void _Win32DestroyGLContext( Win32OpenGLRenderContextNativeData & pGLContextNativeData );

		// Selects matching pixel format for surface described with a PFD. Uses legacy API.
		int _win32ChooseLegacyGLPixelFormat( HDC pDisplaySurfaceDC, PIXELFORMATDESCRIPTOR & pPfmtDescriptor );

		// Selects matching pixel format for surface described with a VisualConfig. Uses new EXT API and supports stuff like MSAA.
		int _win32ChooseCoreGLPixelFormat( HDC pDisplaySurfaceDC, const VisualConfig & pVisualConfig, PIXELFORMATDESCRIPTOR & pPfmtDescriptor );

		// Returns an array of pixel format IDs matching specified set of Win32 PF attributes.
		std::vector<int> _win32QueryCompatiblePixelFormatList( HDC pDisplaySurfaceDC, int * pPixelFormatAttribList = nullptr );

		// Returns an array of pixel format IDs matching specified VisualConfig structure.
		std::vector<int> _win32QueryCompatiblePixelFormatList( HDC pDisplaySurfaceDC, const VisualConfig & pVisualConfig );

		// Computes a "compatibility rate", i.e. how much the specified pixel format matches the visual.
		int _Win32GetPixelFormatMatchRate( HDC pDisplaySurfaceDC, int pPixelFormatIndex, const VisualConfig & pVisualConfig );

		// Translation: VisualConfig --> array of WGL_* attributes required by the system API. Used for surface/context creation.
		void _Win32GetWGLAttribArrayForVisualConfig( const VisualConfig & pVisualConfig, int * pAttribArray );

		// Useful utility. Fetches values for a set of PF attributes. Uses templated array definition to resolve the size.
		template <size_t tpSize, typename TPOutput>
		inline bool _win32QueryPixelFormatAttributes( HDC pDisplaySurfaceDC, int pPixelFormatIndex, const int( &pAttributes )[tpSize], TPOutput & pOutput )
		{
			BOOL result = wglGetPixelFormatAttribivARB( pDisplaySurfaceDC, pPixelFormatIndex, 0, tpSize, pAttributes, &( pOutput[0] ) );
			return result != FALSE;
		}

	}
	
	
	Win32OpenGLSystemDriver::Win32OpenGLSystemDriver( DisplayManagerHandle pDisplayManager )
	: Win32NativeObject( std::move( pDisplayManager ) )
	{}

	Win32OpenGLSystemDriver::~Win32OpenGLSystemDriver() noexcept
	{
		_ReleaseWin32DriverState();
	}

	void Win32OpenGLSystemDriver::_ReleaseWin32DriverState()
	{
		_NativeReleaseInitState();
	}
		
	void Win32OpenGLSystemDriver::_NativeInitializePlatform()
	{
		Ic3DebugAssert( !mNativeData.initState );
		// Init state should be first created here and destroyed as soon as proper GL
		// contexts are created (this is not enforce, though, and controlled explicitly
		// by the user and done by calling ReleaseInitState() method od the driver).
		mNativeData.initState = std::make_unique<Platform::Win32OpenGLSystemDriverNativeData::InitState>();

		WindowCreateInfo tempWindowCreateInfo;
		tempWindowCreateInfo.frameGeometry.position = { 0, 0 };
		tempWindowCreateInfo.frameGeometry.size = { 600, 600 };
		tempWindowCreateInfo.frameGeometry.style = EFrameStyle::Overlay;

		VisualConfig legacyVisualConfig;
		legacyVisualConfig = VisGetDefaultVisualConfigForSysWindow();
		legacyVisualConfig.flags.set( eVisualAttribFlagLegacyBit );

		auto & tmpSurfaceNativeData = mNativeData.initState->surfaceData;

		// Create a surface window. In case of Win32, Win32OpenGLDisplaySurfaceNativeData inherits from WindowNativeData
		// for this very reason: to allow treating surfaces as windows (as that's exactly the case on desktop).
		Platform::Win32CreateWindow( tmpSurfaceNativeData, tempWindowCreateInfo );

		// Create a surface. This sets up the PFD and configures HDC properly.
		_Win32CreateGLSurface( tmpSurfaceNativeData, legacyVisualConfig );

		auto & tmpContextNativeData = mNativeData.initState->contextData;

		// Create legacy OpenGL context for initialization phase.
		tmpContextNativeData.contextHandle = ::wglCreateContext( tmpSurfaceNativeData.hdc );
		if( tmpContextNativeData.contextHandle == nullptr )
		{
			Ic3Throw( eExcCodeDebugPlaceholder );
		}

		// Bind context as current, so GL calls may be used normally.
		BOOL makeCurrentResult = ::wglMakeCurrent( tmpSurfaceNativeData.hdc, tmpContextNativeData.contextHandle );
		if( makeCurrentResult == FALSE )
		{
			Ic3Throw( eExcCodeDebugPlaceholder );
		}

		auto glewResult = glewInit();
		if( glewResult != GLEW_OK )
		{
			Ic3Throw( eExcCodeDebugPlaceholder );
		}

		glewResult = wglewInit();
		if( glewResult != GLEW_OK )
		{
			Ic3Throw( eExcCodeDebugPlaceholder );
		}
	}

	void Win32OpenGLSystemDriver::_NativeReleaseInitState() noexcept
	{
		if( !mNativeData.initState )
		{
			return;
		}

		auto & tmpContextNativeData = mNativeData.initState->contextData;
		::wglDeleteContext( tmpContextNativeData.contextHandle );

		auto & tmpSurfaceNativeData = mNativeData.initState->surfaceData;
		_Win32DestroyGLSurface( tmpSurfaceNativeData );
		Platform::Win32DestroyWindow( tmpSurfaceNativeData );

		mNativeData.initState.reset();
	}

	OpenGLDisplaySurfaceHandle Win32OpenGLSystemDriver::_NativeCreateDisplaySurface(
			const OpenGLDisplaySurfaceCreateInfo & pCreateInfo )
	{
		auto displaySurface = CreateSysObject<Win32OpenGLDisplaySurface>( GetHandle<Win32OpenGLSystemDriver>() );

		WindowCreateInfo surfaceWindowCreateInfo;
		surfaceWindowCreateInfo.frameGeometry = pCreateInfo.frameGeometry;
		surfaceWindowCreateInfo.title = "TS3 OpenGL Window";

		Platform::Win32CreateWindow( displaySurface->mNativeData, surfaceWindowCreateInfo );

		_Win32CreateGLSurface( displaySurface->mNativeData, pCreateInfo.visualConfig );

		if( pCreateInfo.flags.is_set( eOpenGLDisplaySurfaceCreateFlagSyncDisabledBit ) )
		{
			wglSwapIntervalEXT( 0 );
		}
		else if( pCreateInfo.flags.is_set( eOpenGLDisplaySurfaceCreateFlagSyncAdaptiveBit ) )
		{
			wglSwapIntervalEXT( -1 );
		}
		else if( pCreateInfo.flags.is_set( eOpenGLDisplaySurfaceCreateFlagSyncVerticalBit ) )
		{
			wglSwapIntervalEXT( 1 );
		}

		::ShowWindow( displaySurface->mNativeData.hwnd, SW_SHOWNORMAL );

		// TODO: Workaround to properly work with current engine's implementation. That should be turned into an explicit flag.
		if( pCreateInfo.flags.is_set( eOpenGLDisplaySurfaceCreateFlagFullscreenBit ) )
		{
			::SetCapture( displaySurface->mNativeData.hwnd );
			::ShowCursor( FALSE );
		}

		return displaySurface;
	}

	OpenGLDisplaySurfaceHandle Win32OpenGLSystemDriver::_NativeCreateDisplaySurfaceForCurrentThread()
	{
		auto currentDC = ::wglGetCurrentDC();
		if( currentDC == nullptr )
		{
			return nullptr;
		}

		auto displaySurface = CreateSysObject<Win32OpenGLDisplaySurface>( GetHandle<Win32OpenGLSystemDriver>() );

		displaySurface->mNativeData.hdc = currentDC;
		displaySurface->mNativeData.hwnd = ::WindowFromDC( currentDC );
		displaySurface->mNativeData.pixelFormatIndex = ::GetPixelFormat( currentDC );

		CHAR surfaceWindowClassName[256];
		::GetClassNameA( displaySurface->mNativeData.hwnd, surfaceWindowClassName, 255 );
		auto wndProcModuleHandle = ::GetWindowLongPtrA( displaySurface->mNativeData.hwnd, GWLP_HINSTANCE );

		displaySurface->mNativeData.wndClsName = surfaceWindowClassName;
		displaySurface->mNativeData.moduleHandle = reinterpret_cast<HMODULE>( wndProcModuleHandle );

		return displaySurface;
	}

	void Win32OpenGLSystemDriver::_NativeDestroyDisplaySurface( OpenGLDisplaySurface & pDisplaySurface )
	{
	}

	OpenGLRenderContextHandle Win32OpenGLSystemDriver::_NativeCreateRenderContext( OpenGLDisplaySurface & pDisplaySurface,
	                                                                               const OpenGLRenderContextCreateInfo & pCreateInfo )
	{
		auto * win32DisplaySurface = pDisplaySurface.QueryInterface<Win32OpenGLDisplaySurface>();

		int contextAPIProfile = 0;
		cppx::bitmask<int> contextCreateFlags = 0;
		HGLRC shareContextHandle = nullptr;

		if( pCreateInfo.contextAPIProfile == EOpenGLAPIProfile::Core )
		{
			contextAPIProfile = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
		}
		else if( pCreateInfo.contextAPIProfile == EOpenGLAPIProfile::Legacy )
		{
			contextAPIProfile = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
		}
		else if( pCreateInfo.contextAPIProfile == EOpenGLAPIProfile::GLES )
		{
			contextAPIProfile = WGL_CONTEXT_ES_PROFILE_BIT_EXT;
		}

		if( pCreateInfo.flags.is_set( eOpenGLRenderContextCreateFlagEnableDebugBit ) )
		{
			contextCreateFlags |= WGL_CONTEXT_DEBUG_BIT_ARB;
		}
		if( pCreateInfo.flags.is_set( eOpenGLRenderContextCreateFlagForwardCompatibleBit ) )
		{
			contextCreateFlags |= WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;
		}
		if( pCreateInfo.flags.is_set( eOpenGLRenderContextCreateFlagEnableSharingBit ) )
		{
			if( pCreateInfo.shareContext )
			{
				auto * win32ShareContext = pCreateInfo.shareContext->QueryInterface<Win32OpenGLRenderContext>();
				shareContextHandle = win32ShareContext->mNativeData.contextHandle;
			}
			else if( pCreateInfo.flags.is_set( eOpenGLRenderContextCreateFlagShareWithCurrentBit ) )
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
			WGL_CONTEXT_MAJOR_VERSION_ARB, pCreateInfo.requestedAPIVersion.num_major,
			// Requested OpenGL API version: minor part
			WGL_CONTEXT_MINOR_VERSION_ARB, pCreateInfo.requestedAPIVersion.num_minor,
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
			Ic3Throw( eExcCodeDebugPlaceholder );
		}

		auto renderContext = CreateSysObject<Win32OpenGLRenderContext>( GetHandle<Win32OpenGLSystemDriver>() );
		renderContext->mNativeData.contextHandle = contextHandle;

		return renderContext;
	}

	OpenGLRenderContextHandle Win32OpenGLSystemDriver::_NativeCreateRenderContextForCurrentThread()
	{
		auto contextHandle = ::wglGetCurrentContext();
		if( contextHandle == nullptr )
		{
			return nullptr;
		}

		auto renderContext = CreateSysObject<Win32OpenGLRenderContext>( GetHandle<Win32OpenGLSystemDriver>() );
		renderContext->mNativeData.contextHandle = contextHandle;

		return renderContext;
	}

	void Win32OpenGLSystemDriver::_NativeDestroyRenderContext( OpenGLRenderContext & pRenderContext )
	{
	}

	void Win32OpenGLSystemDriver::_NativeResetContextBinding()
	{
		::wglMakeCurrent( nullptr, nullptr );
	}

	std::vector<EDepthStencilFormat> Win32OpenGLSystemDriver::_NativeQuerySupportedDepthStencilFormats(
			EColorFormat pColorFormat ) const
	{
		return {};
	}

	std::vector<EMSAAMode> Win32OpenGLSystemDriver::_NativeQuerySupportedMSAAModes(
			EColorFormat pColorFormat,
			EDepthStencilFormat pDepthStencilFormat ) const
	{
		return {};
	}

	bool Win32OpenGLSystemDriver::_NativeIsAPIClassSupported( EOpenGLAPIClass pAPIClass ) const
	{
		if( pAPIClass == EOpenGLAPIClass::Desktop )
		{
			return true;
		}
		return false;
	}

	bool Win32OpenGLSystemDriver::_NativeIsRenderContextBound() const
	{
		auto currentContext = ::wglGetCurrentContext();
		return currentContext != nullptr;
	}
	
	
	Win32OpenGLDisplaySurface::Win32OpenGLDisplaySurface( Win32OpenGLSystemDriverHandle pGLSystemDriver )
	: Win32NativeObject( std::move( pGLSystemDriver ), &mNativeData )
	{}
	
	Win32OpenGLDisplaySurface::~Win32OpenGLDisplaySurface() noexcept
	{
		_ReleaseWin32SurfaceState();
	}

	void Win32OpenGLDisplaySurface::_ReleaseWin32SurfaceState()
	{
		Platform::_Win32DestroyGLSurface( mNativeData );
		Platform::Win32DestroyWindow( mNativeData );
	}

	void Win32OpenGLDisplaySurface::_NativeSwapBuffers()
	{
		::SwapBuffers( mNativeData.hdc );
	}

	EOpenGLAPIClass Win32OpenGLDisplaySurface::_NativeQuerySupportedAPIClass() const noexcept
	{
		return EOpenGLAPIClass::Desktop;
	}

	VisualConfig Win32OpenGLDisplaySurface::_NativeQueryVisualConfig() const
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
		else
		{
			visualConfig.depthStencilFormat = EDepthStencilFormat::Unknown;
		}

		return visualConfig;
	}

	FrameSize Win32OpenGLDisplaySurface::_NativeQueryRenderAreaSize() const
	{
		return Platform::Win32GetFrameSize( mNativeData.hwnd, EFrameSizeMode::ClientArea );
	}

	bool Win32OpenGLDisplaySurface::_NativeSysValidate() const
	{
		return mNativeData.hwnd && mNativeData.hdc;
	}

	void Win32OpenGLDisplaySurface::_NativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode )
	{}

	void Win32OpenGLDisplaySurface::_NativeSetFullscreenMode( bool pEnable )
	{
		Platform::Win32ChangeWindowFullscreenState( mNativeData, pEnable );
	}

	void Win32OpenGLDisplaySurface::_NativeSetTitle( const std::string & pTitle )
	{
		Platform::Win32SetFrameTitle( mNativeData.hwnd, pTitle );
	}

	void Win32OpenGLDisplaySurface::_NativeUpdateGeometry(
			const FrameGeometry & pFrameGeometry,
			cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
	{
		Platform::Win32UpdateFrameGeometry( mNativeData.hwnd, pFrameGeometry, pUpdateFlags );
	}

	FrameSize Win32OpenGLDisplaySurface::_NativeGetSize( EFrameSizeMode pSizeMode ) const
	{
		return Platform::Win32GetFrameSize( mNativeData.hwnd, pSizeMode );
	}

    bool Win32OpenGLDisplaySurface::_NativeIsFullscreen() const
    {
        return Platform::Win32IsFullscreenWindow( mNativeData );
    }


	Win32OpenGLRenderContext::Win32OpenGLRenderContext( Win32OpenGLSystemDriverHandle pGLSystemDriver )
	: Win32NativeObject( std::move( pGLSystemDriver ) )
	{}

	Win32OpenGLRenderContext::~Win32OpenGLRenderContext() noexcept
	{
		_ReleaseWin32ContextState();
	}
	
	void Win32OpenGLRenderContext::_NativeBindForCurrentThread( const OpenGLDisplaySurface & pTargetSurface )
	{
		const auto * win32DisplaySurface = pTargetSurface.QueryInterface<Win32OpenGLDisplaySurface>();
		::wglMakeCurrent( win32DisplaySurface->mNativeData.hdc, mNativeData.contextHandle );
	}

	bool Win32OpenGLRenderContext::_NativeSysCheckIsCurrent() const
	{
		auto currentContextHandle = ::wglGetCurrentContext();
		return mNativeData.contextHandle && ( mNativeData.contextHandle == currentContextHandle );
	}

	bool Win32OpenGLRenderContext::_NativeSysValidate() const
	{
		return mNativeData.contextHandle != nullptr;
	}

	void Win32OpenGLRenderContext::_ReleaseWin32ContextState()
	{
		Platform::_Win32DestroyGLContext( mNativeData );
	}


	namespace Platform
	{

		void _Win32CreateGLSurface( Win32OpenGLDisplaySurfaceNativeData & pGLSurfaceNativeData,
		                            const VisualConfig & pVisualConfig )
		{
			auto hdc = ::GetWindowDC( pGLSurfaceNativeData.hwnd );
			pGLSurfaceNativeData.hdc = hdc;

			PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
			memset( &pixelFormatDescriptor, 0, sizeof( PIXELFORMATDESCRIPTOR ) );
			pixelFormatDescriptor.nSize = sizeof( PIXELFORMATDESCRIPTOR );
			pixelFormatDescriptor.nVersion = 1;

			if( pVisualConfig.flags.is_set( eVisualAttribFlagLegacyBit ) )
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
				Ic3Throw( eExcCodeDebugPlaceholder );
			}
		}

		void _Win32DestroyGLSurface( Win32OpenGLDisplaySurfaceNativeData & pGLSurfaceNativeData )
		{
			if( pGLSurfaceNativeData.hdc != nullptr )
			{
				::ReleaseDC( pGLSurfaceNativeData.hwnd, pGLSurfaceNativeData.hdc );

				pGLSurfaceNativeData.hdc = nullptr;
				pGLSurfaceNativeData.pixelFormatIndex = 0;
			}
		}

		void _Win32DestroyGLContext( Win32OpenGLRenderContextNativeData & pGLContextNativeData )
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
				int matchRate = _Win32GetPixelFormatMatchRate( pDisplaySurfaceDC, pixelFormatID, pVisualConfig );
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
			int pixelFormatArray[kWin32MaxWGLPixelFormatsNum];
			// Number of pixel formats returned by the system.
			UINT returnedPixelFormatsNum = 0U;

			// Enumerate pixel formats.
			BOOL enumResult = ::wglChoosePixelFormatARB( pDisplaySurfaceDC,
			                                             pPixelFormatAttribList,
			                                             nullptr,
			                                             kWin32MaxWGLPixelFormatsNum,
			                                             pixelFormatArray,
			                                             &returnedPixelFormatsNum );

			if( ( enumResult == FALSE ) || ( returnedPixelFormatsNum == 0 ) )
			{
				Ic3Throw( eExcCodeDebugPlaceholder );
			}

			std::vector<int> result;
			result.reserve( returnedPixelFormatsNum );
			result.assign( &( pixelFormatArray[0] ), &( pixelFormatArray[returnedPixelFormatsNum] ) );

			return result;
		}

		std::vector<int> _win32QueryCompatiblePixelFormatList( HDC pDisplaySurfaceDC, const VisualConfig & pVisualConfig )
		{
			// Array for WGL_..._ARB pixel format attributes.
			int pixelFormatAttributes[kWin32MaxWGLPixelFormatAttributesNum * 2];
			// Translate provided visual config to a WGL attribute array.
			_Win32GetWGLAttribArrayForVisualConfig( pVisualConfig, pixelFormatAttributes );

			return _win32QueryCompatiblePixelFormatList( pDisplaySurfaceDC, pixelFormatAttributes );
		}

		int _Win32GetPixelFormatMatchRate( HDC pDisplaySurfaceDC, int pPixelFormatIndex, const VisualConfig & pVisualConfig )
		{
			int doubleBufferRequestedState = TRUE;
			int stereoModeRequestedState = FALSE;

			if( pVisualConfig.flags.is_set( eVisualAttribFlagSingleBufferBit ) &&
			!pVisualConfig.flags.is_set( eVisualAttribFlagDoubleBufferBit ) )
			{
				doubleBufferRequestedState = FALSE;
			}

			if( pVisualConfig.flags.is_set( eVisualAttribFlagStereoDisplayBit ) &&
			!pVisualConfig.flags.is_set( eVisualAttribFlagMonoDisplayBit ) )
			{
				stereoModeRequestedState = TRUE;
			}

			constexpr int controlAttribArray[] =
			{
				WGL_DOUBLE_BUFFER_ARB,
				WGL_STEREO_ARB,
				WGL_DEPTH_BITS_ARB,
				WGL_STENCIL_BITS_ARB,
				WGL_SAMPLES_ARB
			};

			std::array<int, cppx::static_array_size( controlAttribArray )> attribValueArray{};
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

		void _Win32GetWGLAttribArrayForVisualConfig( const VisualConfig & pVisualConfig, int * pAttribArray )
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

			if( pVisualConfig.flags.is_set( eVisualAttribFlagDoubleBufferBit ) )
			{
				pAttribArray[attribIndex++] = WGL_DOUBLE_BUFFER_ARB;
				pAttribArray[attribIndex++] = GL_TRUE;
			}
			else if( pVisualConfig.flags.is_set( eVisualAttribFlagSingleBufferBit ) )
			{
				pAttribArray[attribIndex++] = WGL_DOUBLE_BUFFER_ARB;
				pAttribArray[attribIndex++] = GL_FALSE;
			}

			if( pVisualConfig.flags.is_set( eVisualAttribFlagMonoDisplayBit ) )
			{
				pAttribArray[attribIndex++] = WGL_STEREO_ARB;
				pAttribArray[attribIndex++] = GL_FALSE;
			}
			else if( pVisualConfig.flags.is_set( eVisualAttribFlagStereoDisplayBit ) )
			{
				pAttribArray[attribIndex++] = WGL_STEREO_ARB;
				pAttribArray[attribIndex++] = GL_TRUE;
			}

			if( pVisualConfig.flags.is_set( eVisualAttribFlagSRGBCapableBit ) )
			{
				pAttribArray[attribIndex++] = WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB;
				pAttribArray[attribIndex++] = GL_TRUE;
			}

			if( pVisualConfig.colorDesc.rgba.u32_code != 0 )
			{
				pAttribArray[attribIndex++] = WGL_RED_BITS_ARB;
				pAttribArray[attribIndex++] = pVisualConfig.colorDesc.rgba.u8_red;

				pAttribArray[attribIndex++] = WGL_GREEN_BITS_ARB;
				pAttribArray[attribIndex++] = pVisualConfig.colorDesc.rgba.u8_green;

				pAttribArray[attribIndex++] = WGL_BLUE_BITS_ARB;
				pAttribArray[attribIndex++] = pVisualConfig.colorDesc.rgba.u8_blue;

				pAttribArray[attribIndex++] = WGL_ALPHA_BITS_ARB;
				pAttribArray[attribIndex++] = pVisualConfig.colorDesc.rgba.u8_alpha;
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
				pAttribArray[attribIndex++] = WGL_DEPTH_BITS_ARB;
				pAttribArray[attribIndex++] = pVisualConfig.depthStencilDesc.depthBufferSize;
			}

			if( pVisualConfig.depthStencilDesc.stencilBufferSize != 0 )
			{
				pAttribArray[attribIndex++] = WGL_STENCIL_BITS_ARB;
				pAttribArray[attribIndex++] = pVisualConfig.depthStencilDesc.stencilBufferSize;
			}

			// Append zero as a list terminator.
			pAttribArray[attribIndex] = 0;
		}

	}

} // namespace Ic3::System
#endif // PCL_TARGET_SYSAPI_WIN32
