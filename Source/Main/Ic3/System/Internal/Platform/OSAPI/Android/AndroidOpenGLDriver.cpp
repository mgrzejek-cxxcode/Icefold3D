
#include "AndroidOpenGLDriver.h"
#include "AndroidDisplaySystem.h"

#if( IC3_PCL_TARGET_SYSAPI == IC3_PCL_TARGET_SYSAPI_ANDROID )
namespace Ic3::System
{

	AndroidOpenGLSystemDriver::AndroidOpenGLSystemDriver( AndroidDisplayManagerHandle pDisplayManager )
	: AndroidNativeObject( std::move( pDisplayManager ) )
	{
		_initializeAndroidDriverState();
	}

	AndroidOpenGLSystemDriver::~AndroidOpenGLSystemDriver() noexcept
	{
		_releaseAndroidDriverState();
	}

	void AndroidOpenGLSystemDriver::_initializeAndroidDriverState()
	{
		auto & aSessionData = Platform::androidGetASessionData( *this );

		Platform::eglInitializeGLDriver( mNativeData );

		mNativeData.eNativeWindow = aSessionData.aNativeWindow;
	}

	void AndroidOpenGLSystemDriver::_releaseAndroidDriverState()
	{
		Platform::eglReleaseGLDriver( mNativeData );

		mNativeData.eNativeWindow = nullptr;
	}

	void AndroidOpenGLSystemDriver::_nativeInitializePlatform()
	{}

	void AndroidOpenGLSystemDriver::_nativeReleaseInitState() noexcept
	{}

	OpenGLDisplaySurfaceHandle AndroidOpenGLSystemDriver::_nativeCreateDisplaySurface( const OpenGLDisplaySurfaceCreateInfo & pCreateInfo )
	{
		auto & aSessionData = Platform::androidGetASessionData( *this );

		// Choose an EGLConfig that matches the specified requirements (pCreateInfo.visualConfig)
		// and version. Version is required to properly query configs with the correct API level
		// support (EGL_OPENGL_ES_BIT/EGL_OPENGL_ES2_BIT/EGL_OPENGL_ES3_BIT).
		EGLConfig fbConfig = Platform::eglChooseCoreFBConfig( mNativeData.eDisplay,
		                                                      pCreateInfo.visualConfig,
		                                                      pCreateInfo.runtimeVersionDesc.apiVersion );

		// EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is guaranteed to be
		// accepted by ANativeWindow_setBuffersGeometry(). As soon as we retrieve an EGLConfig,
		// we can reconfigure the ANativeWindow buffers using the value of EGL_NATIVE_VISUAL_ID.
		EGLint fbConfigNativeVisualID = Platform::eglQueryFBConfigAttribute( mNativeData.eDisplay,
		                                                                     fbConfig,
		                                                                     EGL_NATIVE_VISUAL_ID );

		auto aNativeResult = ANativeWindow_setBuffersGeometry( mNativeData.eNativeWindow,
		                                                       0,
		                                                       0,
		                                                       fbConfigNativeVisualID );
		if( aNativeResult < 0 )
		{
			ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
		}

		auto displaySurface = createSysObject<AndroidOpenGLDisplaySurface>( getHandle<AndroidOpenGLSystemDriver>() );

		Platform::eglCreateSurface( displaySurface->mNativeData,
		                            mNativeData.eDisplay,
		                            mNativeData.eNativeWindow,
		                            fbConfig,
		                            pCreateInfo.visualConfig );

		return displaySurface;
	}

	OpenGLDisplaySurfaceHandle AndroidOpenGLSystemDriver::_nativeCreateDisplaySurfaceForCurrentThread()
	{
		auto displaySurface = createSysObject<AndroidOpenGLDisplaySurface>( getHandle<AndroidOpenGLSystemDriver>() );
		Platform::eglCreateSurfaceForCurrentThread( displaySurface->mNativeData );
		return displaySurface;
	}

	void AndroidOpenGLSystemDriver::_nativeDestroyDisplaySurface( OpenGLDisplaySurface & pDisplaySurface )
	{
		auto * androidDisplaySurface = pDisplaySurface.queryInterface<AndroidOpenGLDisplaySurface>();
		Platform::eglDestroySurface( androidDisplaySurface->mNativeData );
	}

	OpenGLRenderContextHandle AndroidOpenGLSystemDriver::_nativeCreateRenderContext( OpenGLDisplaySurface & pDisplaySurface,
	                                                                                 const OpenGLRenderContextCreateInfo & pCreateInfo )
	{
		auto * androidDisplaySurface = pDisplaySurface.queryInterface<AndroidOpenGLDisplaySurface>();

		auto renderContext = createSysObject<AndroidOpenGLRenderContext>( getHandle<AndroidOpenGLSystemDriver>() );

		Platform::eglCreateCoreContext( renderContext->mNativeData,
		                                androidDisplaySurface->mNativeData,
		                                pCreateInfo );

		return renderContext;
	}

	OpenGLRenderContextHandle AndroidOpenGLSystemDriver::_nativeCreateRenderContextForCurrentThread()
	{
		auto renderContext = createSysObject<AndroidOpenGLRenderContext>( getHandle<AndroidOpenGLSystemDriver>() );
		Platform::eglCreateCoreContextForCurrentThread( renderContext->mNativeData );
		return renderContext;
	}

	void AndroidOpenGLSystemDriver::_nativeDestroyRenderContext( OpenGLRenderContext & pRenderContext )
	{
		auto * androidRenderContext = pRenderContext.queryInterface<AndroidOpenGLRenderContext>();
		Platform::eglDestroyRenderContext( androidRenderContext->mNativeData );
	}

	void AndroidOpenGLSystemDriver::_nativeResetContextBinding()
	{
		::eglMakeCurrent( mNativeData.eDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
	}

	std::vector<EDepthStencilFormat> AndroidOpenGLSystemDriver::_nativeQuerySupportedDepthStencilFormats( EColorFormat pColorFormat ) const
	{
		return {};
	}

	std::vector<EMSAAMode> AndroidOpenGLSystemDriver::_nativeQuerySupportedMSAAModes( EColorFormat pColorFormat,
	                                                                                  EDepthStencilFormat pDepthStencilFormat ) const
	{
		return {};
	}

	bool AndroidOpenGLSystemDriver::_nativeIsAPIClassSupported( EOpenGLAPIClass pAPIClass ) const
	{
		if( pAPIClass == EOpenGLAPIClass::OpenGLES )
		{
			return true;
		}
		return false;
	}

	bool AndroidOpenGLSystemDriver::_nativeIsRenderContextBound() const
	{
		auto currentContext = ::eglGetCurrentContext();
		return currentContext != EGL_NO_CONTEXT;
	}


	AndroidOpenGLDisplaySurface::AndroidOpenGLDisplaySurface( AndroidOpenGLSystemDriverHandle pGLSystemDriver )
	: AndroidNativeObject( std::move( pGLSystemDriver ), &mNativeData )
	{}

	AndroidOpenGLDisplaySurface::~AndroidOpenGLDisplaySurface() noexcept
	{
		_releaseAndroidSurfaceState();
	}

	void AndroidOpenGLDisplaySurface::_releaseAndroidSurfaceState()
	{
	}

	void AndroidOpenGLDisplaySurface::_nativeSwapBuffers()
	{
		::eglSwapBuffers( mNativeData.eDisplay, mNativeData.eSurfaceHandle );
	}

	FrameSize AndroidOpenGLDisplaySurface::_nativeQueryRenderAreaSize() const
	{
		EGLint surfaceWidth = 0;
		EGLint surfaceHeight = 0;

		auto queryResult = ::eglQuerySurface( mNativeData.eDisplay, mNativeData.eSurfaceHandle, EGL_WIDTH, &surfaceWidth );
		if( queryResult == EGL_FALSE )
		{
			ic3EGLHandleLastError();
		}

		queryResult = ::eglQuerySurface( mNativeData.eDisplay, mNativeData.eSurfaceHandle, EGL_HEIGHT, &surfaceHeight );
		if( queryResult == EGL_FALSE )
		{
			ic3EGLHandleLastError();
		}

		FrameSize result;
		result.x = static_cast<decltype( result.x )>( surfaceWidth );
		result.y = static_cast<decltype( result.y )>( surfaceHeight );

		return result;
	}

	bool AndroidOpenGLDisplaySurface::_nativeSysValidate() const
	{
		return ( mNativeData.eSurfaceHandle != EGL_NO_SURFACE ) && mNativeData.eNativeWindow;
	}

	void AndroidOpenGLDisplaySurface::_nativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode )
	{}

	void AndroidOpenGLDisplaySurface::_nativeSetFullscreenMode( bool pEnable )
	{}

	void AndroidOpenGLDisplaySurface::_nativeSetTitle( const std::string & pTitle )
	{

	}

	void AndroidOpenGLDisplaySurface::_nativeUpdateGeometry( const FrameGeometry & pFrameGeometry,
	                                                         Bitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
	{
	}

	FrameSize AndroidOpenGLDisplaySurface::_nativeGetSize( EFrameSizeMode pSizeMode ) const
	{
		return {};
	}


	AndroidOpenGLRenderContext::AndroidOpenGLRenderContext( AndroidOpenGLSystemDriverHandle pGLSystemDriver )
	: AndroidNativeObject( std::move( pGLSystemDriver ) )
	{}

	AndroidOpenGLRenderContext::~AndroidOpenGLRenderContext() noexcept
	{
		_releaseAndroidContextState();
	}

	void AndroidOpenGLRenderContext::_releaseAndroidContextState()
	{
	}

	void AndroidOpenGLRenderContext::_nativeBindForCurrentThread( const OpenGLDisplaySurface & pTargetSurface )
	{
		const auto * androidDisplaySurface = pTargetSurface.queryInterface<AndroidOpenGLDisplaySurface>();
		Platform::eglBindContextForCurrentThread( mNativeData, androidDisplaySurface->mNativeData );
	}

	bool AndroidOpenGLRenderContext::_nativeIsCurrent() const
	{
		auto currentContext = ::eglGetCurrentContext();
		return mNativeData.eContextHandle == currentContext;
	}

	bool AndroidOpenGLRenderContext::_nativeSysValidate() const
	{
		return mNativeData.eContextHandle != EGL_NO_CONTEXT;
	}

} // namespace Ic3::System
#endif // IC3_PCL_TARGET_SYSAPI_ANDROID
