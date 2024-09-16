
#include "OpenGLDriver.h"
#include "DisplaySystem.h"

namespace Ic3::System
{

	OpenGLSystemDriver::OpenGLSystemDriver( DisplayManagerHandle pDisplayManager )
	: SysObject( pDisplayManager->mSysContext )
	, mDisplayManager( std::move( pDisplayManager ) )
	{}

	OpenGLSystemDriver::~OpenGLSystemDriver() noexcept = default;

	OpenGLVersionSupportInfo OpenGLSystemDriver::initializePlatform()
	{
		_nativeInitializePlatform();

		_versionSupportInfo = _nativeQueryVersionSupportInfo();

		return _versionSupportInfo;
	}

	const OpenGLVersionSupportInfo & OpenGLSystemDriver::getVersionSupportInfo() const
	{
		return _versionSupportInfo;
	}

	void OpenGLSystemDriver::releaseInitState( OpenGLRenderContext & /* pGLRenderContext */ )
	{
		_nativeReleaseInitState();
	}

	OpenGLDisplaySurfaceHandle OpenGLSystemDriver::createDisplaySurface( const OpenGLDisplaySurfaceCreateInfo & pCreateInfo )
	{
		OpenGLDisplaySurfaceCreateInfo surfaceCreateInfo = pCreateInfo;

		if( pCreateInfo.mFlags.isSet( eOpenGLDisplaySurfaceCreateFlagFullscreenBit ) )
		{
			surfaceCreateInfo.mFrameGeometry.mSize = cxFrameSizeMax;
			surfaceCreateInfo.mFrameGeometry.mStyle = EFrameStyle::Overlay;
		}
		else
		{
			surfaceCreateInfo.mFrameGeometry.mPosition = pCreateInfo.mFrameGeometry.mPosition;
			surfaceCreateInfo.mFrameGeometry.mSize = pCreateInfo.mFrameGeometry.mSize;
			surfaceCreateInfo.mFrameGeometry.mStyle = pCreateInfo.mFrameGeometry.mStyle;
		}

		if( pCreateInfo.mMinimumAPIVersion == cxGLVersionBestSupported )
		{
			const auto versionSupportInfo = _nativeQueryVersionSupportInfo();
			surfaceCreateInfo.mMinimumAPIVersion = versionSupportInfo.mAPIVersion;
		}

		surfaceCreateInfo.mFrameGeometry = mDisplayManager->validateFrameGeometry( surfaceCreateInfo.mFrameGeometry );

		auto displaySurface = _nativeCreateDisplaySurface( surfaceCreateInfo );
		displaySurface->setInternalOwnershipFlag( true );

		return displaySurface;
	}

	OpenGLDisplaySurfaceHandle OpenGLSystemDriver::createDisplaySurfaceForCurrentThread()
	{
		auto displaySurface = _nativeCreateDisplaySurfaceForCurrentThread();
		displaySurface->setInternalOwnershipFlag( false );

		return displaySurface;
	}

	OpenGLRenderContextHandle OpenGLSystemDriver::createRenderContext( OpenGLDisplaySurface & pSurface,
                                                                       const OpenGLRenderContextCreateInfo & pCreateInfo )
	{
		OpenGLRenderContextCreateInfo contextCreateInfo = pCreateInfo;

		if( contextCreateInfo.mRequestedAPIVersion == CX_VERSION_UNKNOWN  )
		{
			contextCreateInfo.mRequestedAPIVersion = Version{1, 0 };
		}
		else if( contextCreateInfo.mRequestedAPIVersion == cxGLVersionBestSupported )
		{
			contextCreateInfo.mRequestedAPIVersion = _versionSupportInfo.mAPIVersion;
		}

		const auto surfaceAPIClass = pSurface.querySupportedAPIClass();
		if( surfaceAPIClass == EOpenGLAPIClass::GLES )
		{
			if( contextCreateInfo.mRequestedAPIVersion > cxGLVersionMaxES )
			{
				return nullptr;
			}
		}
		else
		{
			if( contextCreateInfo.mRequestedAPIVersion > cxGLVersionMaxDesktop )
			{
				return nullptr;
			}
		}

		if( contextCreateInfo.mRequestedAPIVersion == Version{3, 1 } )
		{
			if((contextCreateInfo.mContextAPIProfile == EOpenGLAPIProfile::Auto ) || (contextCreateInfo.mContextAPIProfile == EOpenGLAPIProfile::Core ) )
			{
				contextCreateInfo.mFlags.set( eOpenGLRenderContextCreateFlagForwardCompatibleBit );
			}
			else
			{
				contextCreateInfo.mFlags.unset( eOpenGLRenderContextCreateFlagForwardCompatibleBit );
			}
		}

		if( contextCreateInfo.mRequestedAPIVersion >= Version{3, 2 } )
		{
			if( contextCreateInfo.mContextAPIProfile == EOpenGLAPIProfile::Auto )
			{
				contextCreateInfo.mContextAPIProfile = EOpenGLAPIProfile::Core;
			}
		}

		auto renderContext = _nativeCreateRenderContext( pSurface, contextCreateInfo );
		renderContext->setInternalOwnershipFlag( true );

		return renderContext;
	}

	OpenGLRenderContextHandle OpenGLSystemDriver::createRenderContextForCurrentThread()
	{
		auto renderContext = _nativeCreateRenderContextForCurrentThread();
		renderContext->setInternalOwnershipFlag( false );

		return renderContext;
	}

	std::vector<EDepthStencilFormat> OpenGLSystemDriver::querySupportedDepthStencilFormats( EColorFormat pColorFormat ) const
	{
		return _nativeQuerySupportedDepthStencilFormats( pColorFormat);
	}

	std::vector<EMSAAMode> OpenGLSystemDriver::querySupportedMSAAModes( EColorFormat pColorFormat,
	                                                                     EDepthStencilFormat pDepthStencilFormat ) const
	{
		return _nativeQuerySupportedMSAAModes( pColorFormat, pDepthStencilFormat );
	}

	void OpenGLSystemDriver::resetContextBinding()
	{
		_nativeResetContextBinding();
	}

	bool OpenGLSystemDriver::isAPIClassSupported( EOpenGLAPIClass pAPIClass ) const
	{
		return _nativeIsAPIClassSupported( pAPIClass );
	}

	bool OpenGLSystemDriver::isRenderContextBound() const
	{
		return _nativeIsRenderContextBound();
	}

	void OpenGLSystemDriver::releaseSystemDisplaySurface( OpenGLDisplaySurface & pDisplaySurface ) noexcept
	{
		try
		{
			if( pDisplaySurface.hasInternalOwnershipFlag() )
			{
				if( pDisplaySurface.sysValidate() )
				{
					_nativeDestroyDisplaySurface( pDisplaySurface );
				}

				pDisplaySurface.setInternalOwnershipFlag( false );
			}
		}
		catch( const Exception & pException )
		{
			( pException );
			ic3DebugInterrupt();
		}
		catch( ... )
		{
			ic3DebugInterrupt();
		}
	}

	void OpenGLSystemDriver::releaseSystemRenderContext( OpenGLRenderContext & pRenderContext ) noexcept
	{
		try
		{
			if( pRenderContext.hasInternalOwnershipFlag() )
			{
				if( pRenderContext.sysValidate() )
				{
					_nativeDestroyRenderContext( pRenderContext );
				}

				pRenderContext.setInternalOwnershipFlag( false );
			}
		}
		catch( const Exception & pException )
		{
			( pException );
			ic3DebugInterrupt();
		}
		catch( ... )
		{
			ic3DebugInterrupt();
		}
	}

	OpenGLVersionSupportInfo OpenGLSystemDriver::_nativeQueryVersionSupportInfo() const noexcept
	{
		OpenGLVersionSupportInfo openGLVersionSupportInfo{};

		openGLVersionSupportInfo.mAPIVersion = OpenGLCoreAPI::queryRuntimeVersion();
		openGLVersionSupportInfo.mAPIClass = EOpenGLAPIClass::Desktop;
		openGLVersionSupportInfo.mAPIProfile = EOpenGLAPIProfile::Legacy;

		if( openGLVersionSupportInfo.mAPIVersion.mNumMajor == 0 )
		{
			openGLVersionSupportInfo.mAPIVersion.mNumMajor = 1;
			openGLVersionSupportInfo.mAPIVersion.mNumMinor = 0;
		}

		return openGLVersionSupportInfo;
	}


	OpenGLDisplaySurface::OpenGLDisplaySurface( OpenGLSystemDriverHandle pGLSystemDriver, void * pNativeData )
	: Frame( pGLSystemDriver->mSysContext )
	, mGLSystemDriver( std::move( pGLSystemDriver ) )
	{
		setEventSourceNativeData( pNativeData );
	}

	OpenGLDisplaySurface::~OpenGLDisplaySurface() noexcept
	{
		resetEventSourceNativeData();
	}

	void OpenGLDisplaySurface::clearColorBuffer()
	{
		glClearColor( 0.24f, 0.72f, 0.4f, 1.0f );
		ic3OpenGLHandleLastError();

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		ic3OpenGLHandleLastError();
	}

	void OpenGLDisplaySurface::swapBuffers()
	{
		_nativeSwapBuffers();
	}

	EOpenGLAPIClass OpenGLDisplaySurface::querySupportedAPIClass() const
	{
		return _nativeQuerySupportedAPIClass();
	}

	VisualConfig OpenGLDisplaySurface::queryVisualConfig() const
	{
		return _nativeQueryVisualConfig();
	}

	FrameSize OpenGLDisplaySurface::queryRenderAreaSize() const
	{
		return _nativeQueryRenderAreaSize();
	}

	bool OpenGLDisplaySurface::sysValidate() const
	{
		return _nativeSysValidate();
	}

	void OpenGLDisplaySurface::resizeClientArea( const FrameSize & pSize )
	{
		FrameGeometry newFrameGeometry{};
		newFrameGeometry.mPosition = cxFramePosAuto;
		newFrameGeometry.mSize = pSize;
		newFrameGeometry.mStyle = EFrameStyle::Unspecified;

		newFrameGeometry = mGLSystemDriver->mDisplayManager->validateFrameGeometry( newFrameGeometry );

		const auto updateFlags = eFrameGeometryUpdateFlagPositionBit | eFrameGeometryUpdateFlagSizeClientAreaBit;
		_nativeUpdateGeometry( newFrameGeometry, updateFlags );
	}

	void OpenGLDisplaySurface::resizeFrame( const FrameSize & pSize )
	{
		FrameGeometry newFrameGeometry{};
		newFrameGeometry.mPosition = cxFramePosAuto;
		newFrameGeometry.mSize = pSize;
		newFrameGeometry.mStyle = EFrameStyle::Unspecified;

		newFrameGeometry = mGLSystemDriver->mDisplayManager->validateFrameGeometry( newFrameGeometry );

		const auto updateFlags = eFrameGeometryUpdateFlagPositionBit | eFrameGeometryUpdateFlagSizeOuterRectBit;
		_nativeUpdateGeometry( newFrameGeometry, updateFlags );
	}

	void OpenGLDisplaySurface::setFullscreenMode( bool pEnable )
	{
		_nativeSetFullscreenMode( pEnable );
	}

	void OpenGLDisplaySurface::setTitle( const std::string & pTitleText )
	{
		_nativeSetTitle( pTitleText );
	}

	void OpenGLDisplaySurface::updateGeometry( const FrameGeometry & pFrameGeometry,
	                                           TBitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
	{
		_nativeUpdateGeometry( pFrameGeometry, pUpdateFlags );
	}

	FrameSize OpenGLDisplaySurface::getClientAreaSize() const
	{
		return _nativeGetSize( EFrameSizeMode::ClientArea );
	}

	FrameSize OpenGLDisplaySurface::getFrameSize() const
	{
		return _nativeGetSize( EFrameSizeMode::OuterRect );
	}

	bool OpenGLDisplaySurface::isFullscreen() const
	{
		return _nativeIsFullscreen();
	}

	void OpenGLDisplaySurface::onDestroySystemObjectRequested()
	{
		EventSource::onDestroySystemObjectRequested();

		mGLSystemDriver->releaseSystemDisplaySurface( *this );
	}

	void OpenGLDisplaySurface::setInternalOwnershipFlag( bool pOwnershipFlag )
	{
		_internalOwnershipFlag = pOwnershipFlag;
	}

	bool OpenGLDisplaySurface::hasInternalOwnershipFlag() const
	{
		return _internalOwnershipFlag;
	}


	OpenGLRenderContext::OpenGLRenderContext( OpenGLSystemDriverHandle pDriver )
	: SysObject( pDriver->mSysContext )
	, mGLSystemDriver( std::move( pDriver ) )
	{}

	OpenGLRenderContext::~OpenGLRenderContext() noexcept = default;

	void OpenGLRenderContext::bindForCurrentThread( const OpenGLDisplaySurface & pTargetSurface )
	{
		_nativeBindForCurrentThread( pTargetSurface );
	}

	bool OpenGLRenderContext::sysCheckIsCurrent() const
	{
		return _nativeSysCheckIsCurrent();
	}

	bool OpenGLRenderContext::sysValidate() const
	{
		return _nativeSysValidate();
	}

	OpenGLSystemVersionInfo OpenGLRenderContext::querySystemVersionInfo() const
	{
		if( !sysValidate() )
		{
			ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
		}

		OpenGLSystemVersionInfo systemVersionInfo;
		systemVersionInfo.mAPIVersion = OpenGLCoreAPI::queryRuntimeVersion();

		if( const auto * versionStr = glGetString( GL_VERSION ) )
		{
			systemVersionInfo.mAPIVersionStr.assign( reinterpret_cast<const char *>( versionStr ) );
		}
		if( const auto * glslVersionStr = glGetString( GL_SHADING_LANGUAGE_VERSION ) )
		{
			systemVersionInfo.mGLSLVersionStr.assign( reinterpret_cast<const char *>( glslVersionStr ) );
		}
		if( const auto * rendererNameStr = glGetString( GL_RENDERER ) )
		{
			systemVersionInfo.mRendererName.assign( reinterpret_cast<const char *>( rendererNameStr ) );
		}
		if( const auto * vendorNameStr = glGetString( GL_VENDOR ) )
		{
			systemVersionInfo.mVendorName.assign( reinterpret_cast<const char *>( vendorNameStr ) );
		}

		ic3OpenGLResetErrorQueue();

		return systemVersionInfo;
	}

	void OpenGLRenderContext::onDestroySystemObjectRequested()
	{
		mGLSystemDriver->releaseSystemRenderContext( *this );
	}

	void OpenGLRenderContext::setInternalOwnershipFlag( bool pOwnershipFlag )
	{
		_internalOwnershipFlag = pOwnershipFlag;
	}

	bool OpenGLRenderContext::hasInternalOwnershipFlag() const
	{
		return _internalOwnershipFlag;
	}

} // namespace Ic3::System
