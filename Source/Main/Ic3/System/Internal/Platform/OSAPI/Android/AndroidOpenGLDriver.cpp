
#include "AndroidOpenGLDriver.h"
#include "AndroidDisplaySystem.h"

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_ANDROID )
namespace Ic3::System
{

	AndroidOpenGLSystemDriver::AndroidOpenGLSystemDriver( AndroidDisplayManagerHandle pDisplayManager )
	: AndroidNativeObject( std::move( pDisplayManager ) )
	{
		_InitializeAndroidDriverState();
	}

	AndroidOpenGLSystemDriver::~AndroidOpenGLSystemDriver() noexcept
	{
		_ReleaseAndroidDriverState();
	}

	void AndroidOpenGLSystemDriver::_InitializeAndroidDriverState()
	{
		auto & aSessionData = Platform::AndroidGetASessionData( *this );

		Platform::EAInitializeGLDriver( mNativeData );

		mNativeData.eglNativeWindowHandle = aSessionData.aNativeWindow;
	}

	void AndroidOpenGLSystemDriver::_ReleaseAndroidDriverState()
	{
		Platform::EAReleaseGLDriver( mNativeData );

		mNativeData.eglNativeWindowHandle = nullptr;
	}

	void AndroidOpenGLSystemDriver::_NativeInitializePlatform()
	{}

	void AndroidOpenGLSystemDriver::_NativeReleaseInitState() noexcept
	{}

	OpenGLDisplaySurfaceHandle AndroidOpenGLSystemDriver::_NativeCreateDisplaySurface(
			const OpenGLDisplaySurfaceCreateInfo & pCreateInfo )
	{
		auto & aSessionData = Platform::AndroidGetASessionData( *this );

		// Choose an EGLConfig that matches the specified requirements (pCreateInfo.visualConfig)
		// and version. cppx::version is required to properly query configs with the correct API level
		// support (EGL_OPENGL_ES_BIT/EGL_OPENGL_ES2_BIT/EGL_OPENGL_ES3_BIT).
		EGLConfig fbConfig = Platform::EAChooseCoreFBConfig( mNativeData.eglDisplay,
		                                                      pCreateInfo.visualConfig,
		                                                      pCreateInfo.runtimeVersionDesc.apiVersion );

		// EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is guaranteed to be
		// accepted by ANativeWindow_setBuffersGeometry(). As soon as we retrieve an EGLConfig,
		// we can reconfigure the ANativeWindow buffers using the value of EGL_NATIVE_VISUAL_ID.
		EGLint fbConfigNativeVisualID = Platform::EAQueryFBConfigAttribute( mNativeData.eglDisplay,
		                                                                     fbConfig,
		                                                                     EGL_NATIVE_VISUAL_ID );

		auto aNativeResult = ANativeWindow_setBuffersGeometry( mNativeData.eglNativeWindowHandle,
		                                                       0,
		                                                       0,
		                                                       fbConfigNativeVisualID );
		if( aNativeResult < 0 )
		{
			ic3Throw( eExcCodeDebugPlaceholder );
		}

		auto displaySurface = CreateSysObject<AndroidOpenGLDisplaySurface>( GetHandle<AndroidOpenGLSystemDriver>() );

		Platform::EACreateSurface(
				displaySurface->mNativeData,
				mNativeData.eglDisplay,
				mNativeData.eglNativeWindowHandle,
				fbConfig,
				pCreateInfo.visualConfig );

		return displaySurface;
	}

	OpenGLDisplaySurfaceHandle AndroidOpenGLSystemDriver::_NativeCreateDisplaySurfaceForCurrentThread()
	{
		auto displaySurface = CreateSysObject<AndroidOpenGLDisplaySurface>( GetHandle<AndroidOpenGLSystemDriver>() );
		Platform::EACreateSurfaceForCurrentThread( displaySurface->mNativeData );
		return displaySurface;
	}

	void AndroidOpenGLSystemDriver::_NativeDestroyDisplaySurface( OpenGLDisplaySurface & pDisplaySurface )
	{
		auto * androidDisplaySurface = pDisplaySurface.QueryInterface<AndroidOpenGLDisplaySurface>();
		Platform::EADestroySurface( androidDisplaySurface->mNativeData );
	}

	OpenGLRenderContextHandle AndroidOpenGLSystemDriver::_NativeCreateRenderContext(
			OpenGLDisplaySurface & pDisplaySurface,
			const OpenGLRenderContextCreateInfo & pCreateInfo )
	{
		auto * androidDisplaySurface = pDisplaySurface.QueryInterface<AndroidOpenGLDisplaySurface>();

		auto renderContext = CreateSysObject<AndroidOpenGLRenderContext>( GetHandle<AndroidOpenGLSystemDriver>() );

		Platform::EACreateCoreContext( renderContext->mNativeData,
		                                androidDisplaySurface->mNativeData,
		                                pCreateInfo );

		return renderContext;
	}

	OpenGLRenderContextHandle AndroidOpenGLSystemDriver::_NativeCreateRenderContextForCurrentThread()
	{
		auto renderContext = CreateSysObject<AndroidOpenGLRenderContext>( GetHandle<AndroidOpenGLSystemDriver>() );
		Platform::EACreateCoreContextForCurrentThread( renderContext->mNativeData );
		return renderContext;
	}

	void AndroidOpenGLSystemDriver::_NativeDestroyRenderContext( OpenGLRenderContext & pRenderContext )
	{
		auto * androidRenderContext = pRenderContext.QueryInterface<AndroidOpenGLRenderContext>();
		Platform::EADestroyRenderContext( androidRenderContext->mNativeData );
	}

	void AndroidOpenGLSystemDriver::_NativeResetContextBinding()
	{
		::eglMakeCurrent( mNativeData.eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
	}

	std::vector<EDepthStencilFormat> AndroidOpenGLSystemDriver::_NativeQuerySupportedDepthStencilFormats(
			EColorFormat pColorFormat ) const
	{
		return {};
	}

	std::vector<EMSAAMode> AndroidOpenGLSystemDriver::_NativeQuerySupportedMSAAModes(
			EColorFormat pColorFormat,
			EDepthStencilFormat pDepthStencilFormat ) const
	{
		return {};
	}

	bool AndroidOpenGLSystemDriver::_NativeIsAPIClassSupported( EOpenGLAPIClass pAPIClass ) const
	{
		if( pAPIClass == EOpenGLAPIClass::OpenGLES )
		{
			return true;
		}
		return false;
	}

	bool AndroidOpenGLSystemDriver::_NativeIsRenderContextBound() const
	{
		auto currentContext = ::eglGetCurrentContext();
		return currentContext != EGL_NO_CONTEXT;
	}


	AndroidOpenGLDisplaySurface::AndroidOpenGLDisplaySurface( AndroidOpenGLSystemDriverHandle pGLSystemDriver )
	: AndroidNativeObject( std::move( pGLSystemDriver ), &mNativeData )
	{}

	AndroidOpenGLDisplaySurface::~AndroidOpenGLDisplaySurface() noexcept
	{
		_ReleaseAndroidSurfaceState();
	}

	void AndroidOpenGLDisplaySurface::_ReleaseAndroidSurfaceState()
	{
	}

	void AndroidOpenGLDisplaySurface::_NativeSwapBuffers()
	{
		::eglSwapBuffers( mNativeData.eglDisplay, mNativeData.eglSurfaceHandle );
	}

	FrameSize AndroidOpenGLDisplaySurface::_NativeQueryRenderAreaSize() const
	{
		EGLint surfaceWidth = 0;
		EGLint surfaceHeight = 0;

		auto queryResult = ::eglQuerySurface( mNativeData.eglDisplay, mNativeData.eglSurfaceHandle, EGL_WIDTH, &surfaceWidth );
		if( queryResult == EGL_FALSE )
		{
			ic3EGLHandleLastError();
		}

		queryResult = ::eglQuerySurface( mNativeData.eglDisplay, mNativeData.eglSurfaceHandle, EGL_HEIGHT, &surfaceHeight );
		if( queryResult == EGL_FALSE )
		{
			ic3EGLHandleLastError();
		}

		FrameSize result;
		result.x = static_cast<decltype( result.x )>( surfaceWidth );
		result.y = static_cast<decltype( result.y )>( surfaceHeight );

		return result;
	}

	bool AndroidOpenGLDisplaySurface::_NativeSysValidate() const
	{
		return ( mNativeData.eglSurfaceHandle != EGL_NO_SURFACE ) && mNativeData.eglNativeWindowHandle;
	}

	void AndroidOpenGLDisplaySurface::_NativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode )
	{}

	void AndroidOpenGLDisplaySurface::_NativeSetFullscreenMode( bool pEnable )
	{}

	void AndroidOpenGLDisplaySurface::_NativeSetTitle( const std::string & pTitle )
	{

	}

	void AndroidOpenGLDisplaySurface::_NativeUpdateGeometry(
			const FrameGeometry & pFrameGeometry,
			cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
	{
	}

	FrameSize AndroidOpenGLDisplaySurface::_NativeGetSize( EFrameSizeMode pSizeMode ) const
	{
		return {};
	}


	AndroidOpenGLRenderContext::AndroidOpenGLRenderContext( AndroidOpenGLSystemDriverHandle pGLSystemDriver )
	: AndroidNativeObject( std::move( pGLSystemDriver ) )
	{}

	AndroidOpenGLRenderContext::~AndroidOpenGLRenderContext() noexcept
	{
		_ReleaseAndroidContextState();
	}

	void AndroidOpenGLRenderContext::_ReleaseAndroidContextState()
	{
	}

	void AndroidOpenGLRenderContext::_NativeBindForCurrentThread( const OpenGLDisplaySurface & pTargetSurface )
	{
		const auto * androidDisplaySurface = pTargetSurface.QueryInterface<AndroidOpenGLDisplaySurface>();
		Platform::EABindContextForCurrentThread( mNativeData, androidDisplaySurface->mNativeData );
	}

	bool AndroidOpenGLRenderContext::_NativeIsCurrent() const
	{
		auto currentContext = ::eglGetCurrentContext();
		return mNativeData.eContextHandle == currentContext;
	}

	bool AndroidOpenGLRenderContext::_NativeSysValidate() const
	{
		return mNativeData.eContextHandle != EGL_NO_CONTEXT;
	}

} // namespace Ic3::System
#endif // PCL_TARGET_SYSAPI_ANDROID
