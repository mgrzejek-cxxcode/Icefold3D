
#include "OpenGLDriver.h"
#include "DisplaySystem.h"

namespace Ic3::System
{

	OpenGLSystemDriver::OpenGLSystemDriver( DisplayManagerHandle pDisplayManager )
	: SysObject( pDisplayManager->mSysContext )
	, mDisplayManager( std::move( pDisplayManager ) )
	{}

	OpenGLSystemDriver::~OpenGLSystemDriver() noexcept = default;

	OpenGLVersionSupportInfo OpenGLSystemDriver::InitializePlatform()
	{
		_NativeInitializePlatform();

		_versionSupportInfo = _NativeQueryVersionSupportInfo();

		return _versionSupportInfo;
	}

	const OpenGLVersionSupportInfo & OpenGLSystemDriver::GetVersionSupportInfo() const
	{
		return _versionSupportInfo;
	}

	void OpenGLSystemDriver::ReleaseInitState( OpenGLRenderContext & /* pGLRenderContext */ )
	{
		_NativeReleaseInitState();
	}

	OpenGLDisplaySurfaceHandle OpenGLSystemDriver::CreateDisplaySurface( const OpenGLDisplaySurfaceCreateInfo & pCreateInfo )
	{
		OpenGLDisplaySurfaceCreateInfo surfaceCreateInfo = pCreateInfo;

		if( pCreateInfo.flags.is_set( eOpenGLDisplaySurfaceCreateFlagFullscreenBit ) )
		{
			surfaceCreateInfo.frameGeometry.size = cxFrameSizeMax;
			surfaceCreateInfo.frameGeometry.style = EFrameStyle::Overlay;
		}
		else
		{
			surfaceCreateInfo.frameGeometry.position = pCreateInfo.frameGeometry.position;
			surfaceCreateInfo.frameGeometry.size = pCreateInfo.frameGeometry.size;
			surfaceCreateInfo.frameGeometry.style = pCreateInfo.frameGeometry.style;
		}

		if( pCreateInfo.minimumAPIVersion == cxGLVersionBestSupported )
		{
			const auto versionSupportInfo = _NativeQueryVersionSupportInfo();
			surfaceCreateInfo.minimumAPIVersion = versionSupportInfo.apiVersion;
		}

		surfaceCreateInfo.frameGeometry = mDisplayManager->ValidateFrameGeometry( surfaceCreateInfo.frameGeometry );

		auto displaySurface = _NativeCreateDisplaySurface( surfaceCreateInfo );
		displaySurface->SetInternalOwnershipFlag( true );

		return displaySurface;
	}

	OpenGLDisplaySurfaceHandle OpenGLSystemDriver::CreateDisplaySurfaceForCurrentThread()
	{
		auto displaySurface = _NativeCreateDisplaySurfaceForCurrentThread();
		displaySurface->SetInternalOwnershipFlag( false );

		return displaySurface;
	}

	OpenGLRenderContextHandle OpenGLSystemDriver::CreateRenderContext(
			OpenGLDisplaySurface & pSurface,
			const OpenGLRenderContextCreateInfo & pCreateInfo )
	{
		OpenGLRenderContextCreateInfo contextCreateInfo = pCreateInfo;

		if( contextCreateInfo.requestedAPIVersion == cppx::cve::version_unknown  )
		{
			contextCreateInfo.requestedAPIVersion = cppx::version{1, 0 };
		}
		else if( contextCreateInfo.requestedAPIVersion == cxGLVersionBestSupported )
		{
			contextCreateInfo.requestedAPIVersion = _versionSupportInfo.apiVersion;
		}

		const auto surfaceAPIClass = pSurface.QuerySupportedAPIClass();
		if( surfaceAPIClass == EOpenGLAPIClass::GLES )
		{
			if( contextCreateInfo.requestedAPIVersion > cxGLVersionMaxES )
			{
				return nullptr;
			}
		}
		else
		{
			if( contextCreateInfo.requestedAPIVersion > cxGLVersionMaxDesktop )
			{
				return nullptr;
			}
		}

		if( contextCreateInfo.requestedAPIVersion == cppx::version{3, 1 } )
		{
			if((contextCreateInfo.contextAPIProfile == EOpenGLAPIProfile::Auto ) || (contextCreateInfo.contextAPIProfile == EOpenGLAPIProfile::Core ) )
			{
				contextCreateInfo.flags.set( eOpenGLRenderContextCreateFlagForwardCompatibleBit );
			}
			else
			{
				contextCreateInfo.flags.unset( eOpenGLRenderContextCreateFlagForwardCompatibleBit );
			}
		}

		if( contextCreateInfo.requestedAPIVersion >= cppx::version{3, 2 } )
		{
			if( contextCreateInfo.contextAPIProfile == EOpenGLAPIProfile::Auto )
			{
				contextCreateInfo.contextAPIProfile = EOpenGLAPIProfile::Core;
			}
		}

		auto renderContext = _NativeCreateRenderContext( pSurface, contextCreateInfo );
		renderContext->SetInternalOwnershipFlag( true );

		return renderContext;
	}

	OpenGLRenderContextHandle OpenGLSystemDriver::CreateRenderContextForCurrentThread()
	{
		auto renderContext = _NativeCreateRenderContextForCurrentThread();
		renderContext->SetInternalOwnershipFlag( false );

		return renderContext;
	}

	std::vector<EDepthStencilFormat> OpenGLSystemDriver::QuerySupportedDepthStencilFormats( EColorFormat pColorFormat ) const
	{
		return _NativeQuerySupportedDepthStencilFormats( pColorFormat);
	}

	std::vector<EMSAAMode> OpenGLSystemDriver::QuerySupportedMSAAModes(
			EColorFormat pColorFormat,
			EDepthStencilFormat pDepthStencilFormat ) const
	{
		return _NativeQuerySupportedMSAAModes( pColorFormat, pDepthStencilFormat );
	}

	void OpenGLSystemDriver::ResetContextBinding()
	{
		_NativeResetContextBinding();
	}

	bool OpenGLSystemDriver::IsAPIClassSupported( EOpenGLAPIClass pAPIClass ) const
	{
		return _NativeIsAPIClassSupported( pAPIClass );
	}

	bool OpenGLSystemDriver::IsRenderContextBound() const
	{
		return _NativeIsRenderContextBound();
	}

	void OpenGLSystemDriver::ReleaseSystemDisplaySurface( OpenGLDisplaySurface & pDisplaySurface ) noexcept
	{
		try
		{
			if( pDisplaySurface.HasInternalOwnershipFlag() )
			{
				if( pDisplaySurface.SysValidate() )
				{
					_NativeDestroyDisplaySurface( pDisplaySurface );
				}

				pDisplaySurface.SetInternalOwnershipFlag( false );
			}
		}
		catch( const Exception & pException )
		{
			( pException );
			Ic3DebugInterrupt();
		}
		catch( ... )
		{
			Ic3DebugInterrupt();
		}
	}

	void OpenGLSystemDriver::ReleaseSystemRenderContext( OpenGLRenderContext & pRenderContext ) noexcept
	{
		try
		{
			if( pRenderContext.HasInternalOwnershipFlag() )
			{
				if( pRenderContext.SysValidate() )
				{
					_NativeDestroyRenderContext( pRenderContext );
				}

				pRenderContext.SetInternalOwnershipFlag( false );
			}
		}
		catch( const Exception & pException )
		{
			( pException );
			Ic3DebugInterrupt();
		}
		catch( ... )
		{
			Ic3DebugInterrupt();
		}
	}

	OpenGLVersionSupportInfo OpenGLSystemDriver::_NativeQueryVersionSupportInfo() const noexcept
	{
		OpenGLVersionSupportInfo openGLVersionSupportInfo{};

		openGLVersionSupportInfo.apiVersion = OpenGLCoreAPI::QueryRuntimeVersion();
		openGLVersionSupportInfo.apiClass = EOpenGLAPIClass::Desktop;
		openGLVersionSupportInfo.apiProfile = EOpenGLAPIProfile::Legacy;

		if( openGLVersionSupportInfo.apiVersion.num_major == 0 )
		{
			openGLVersionSupportInfo.apiVersion.num_major = 1;
			openGLVersionSupportInfo.apiVersion.num_minor = 0;
		}

		return openGLVersionSupportInfo;
	}


	OpenGLDisplaySurface::OpenGLDisplaySurface( OpenGLSystemDriverHandle pGLSystemDriver, void * pNativeData )
	: Frame( pGLSystemDriver->mSysContext )
	, mGLSystemDriver( std::move( pGLSystemDriver ) )
	{
		SetEventSourceNativeData( pNativeData );
	}

	OpenGLDisplaySurface::~OpenGLDisplaySurface() noexcept
	{
		ResetEventSourceNativeData();
	}

	void OpenGLDisplaySurface::SwapBuffers()
	{
		_NativeSwapBuffers();
	}

	EOpenGLAPIClass OpenGLDisplaySurface::QuerySupportedAPIClass() const
	{
		return _NativeQuerySupportedAPIClass();
	}

	VisualConfig OpenGLDisplaySurface::QueryVisualConfig() const
	{
		return _NativeQueryVisualConfig();
	}

	FrameSize OpenGLDisplaySurface::QueryRenderAreaSize() const
	{
		return _NativeQueryRenderAreaSize();
	}

	bool OpenGLDisplaySurface::SysValidate() const
	{
		return _NativeSysValidate();
	}

	void OpenGLDisplaySurface::ResizeClientArea( const FrameSize & pSize )
	{
		FrameGeometry newFrameGeometry{};
		newFrameGeometry.position = cxFramePosAuto;
		newFrameGeometry.size = pSize;
		newFrameGeometry.style = EFrameStyle::Unspecified;

		newFrameGeometry = mGLSystemDriver->mDisplayManager->ValidateFrameGeometry( newFrameGeometry );

		const auto updateFlags = eFrameGeometryUpdateFlagPositionBit | eFrameGeometryUpdateFlagSizeClientAreaBit;
		_NativeUpdateGeometry( newFrameGeometry, updateFlags );
	}

	void OpenGLDisplaySurface::ResizeFrame( const FrameSize & pSize )
	{
		FrameGeometry newFrameGeometry{};
		newFrameGeometry.position = cxFramePosAuto;
		newFrameGeometry.size = pSize;
		newFrameGeometry.style = EFrameStyle::Unspecified;

		newFrameGeometry = mGLSystemDriver->mDisplayManager->ValidateFrameGeometry( newFrameGeometry );

		const auto updateFlags = eFrameGeometryUpdateFlagPositionBit | eFrameGeometryUpdateFlagSizeOuterRectBit;
		_NativeUpdateGeometry( newFrameGeometry, updateFlags );
	}

	void OpenGLDisplaySurface::SetFullscreenMode( bool pEnable )
	{
		_NativeSetFullscreenMode( pEnable );
	}

	void OpenGLDisplaySurface::SetTitle( const std::string & pTitleText )
	{
		_NativeSetTitle( pTitleText );
	}

	void OpenGLDisplaySurface::UpdateGeometry(
			const FrameGeometry & pFrameGeometry,
			cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
	{
		_NativeUpdateGeometry( pFrameGeometry, pUpdateFlags );
	}

	FrameSize OpenGLDisplaySurface::GetClientAreaSize() const
	{
		return _NativeGetSize( EFrameSizeMode::ClientArea );
	}

	FrameSize OpenGLDisplaySurface::GetFrameSize() const
	{
		return _NativeGetSize( EFrameSizeMode::OuterRect );
	}

	bool OpenGLDisplaySurface::IsFullscreen() const
	{
		return _NativeIsFullscreen();
	}

	void OpenGLDisplaySurface::OnDestroySystemObjectRequested()
	{
		EventSource::OnDestroySystemObjectRequested();

		mGLSystemDriver->ReleaseSystemDisplaySurface( *this );
	}

	void OpenGLDisplaySurface::SetInternalOwnershipFlag( bool pOwnershipFlag )
	{
		_internalOwnershipFlag = pOwnershipFlag;
	}

	bool OpenGLDisplaySurface::HasInternalOwnershipFlag() const
	{
		return _internalOwnershipFlag;
	}


	OpenGLRenderContext::OpenGLRenderContext( OpenGLSystemDriverHandle pDriver )
	: SysObject( pDriver->mSysContext )
	, mGLSystemDriver( std::move( pDriver ) )
	{}

	OpenGLRenderContext::~OpenGLRenderContext() noexcept = default;

	void OpenGLRenderContext::BindForCurrentThread( const OpenGLDisplaySurface & pTargetSurface )
	{
		_NativeBindForCurrentThread( pTargetSurface );
	}

	bool OpenGLRenderContext::SysCheckIsCurrent() const
	{
		return _NativeSysCheckIsCurrent();
	}

	bool OpenGLRenderContext::SysValidate() const
	{
		return _NativeSysValidate();
	}

	OpenGLSystemVersionInfo OpenGLRenderContext::QuerySystemVersionInfo() const
	{
		if( !SysValidate() )
		{
			Ic3Throw( eExcCodeDebugPlaceholder );
		}

		OpenGLSystemVersionInfo systemVersionInfo;
		systemVersionInfo.apiVersion = OpenGLCoreAPI::QueryRuntimeVersion();

		if( const auto * versionStr = glGetString( GL_VERSION ) )
		{
			systemVersionInfo.apiVersionStr.assign( reinterpret_cast<const char *>( versionStr ) );
		}
		if( const auto * glslVersionStr = glGetString( GL_SHADING_LANGUAGE_VERSION ) )
		{
			systemVersionInfo.glslVersionStr.assign( reinterpret_cast<const char *>( glslVersionStr ) );
		}
		if( const auto * rendererNameStr = glGetString( GL_RENDERER ) )
		{
			systemVersionInfo.rendererName.assign( reinterpret_cast<const char *>( rendererNameStr ) );
		}
		if( const auto * vendorNameStr = glGetString( GL_VENDOR ) )
		{
			systemVersionInfo.vendorName.assign( reinterpret_cast<const char *>( vendorNameStr ) );
		}

		Ic3OpenGLResetErrorQueue();

		return systemVersionInfo;
	}

	void OpenGLRenderContext::OnDestroySystemObjectRequested()
	{
		mGLSystemDriver->ReleaseSystemRenderContext( *this );
	}

	void OpenGLRenderContext::SetInternalOwnershipFlag( bool pOwnershipFlag )
	{
		_internalOwnershipFlag = pOwnershipFlag;
	}

	bool OpenGLRenderContext::HasInternalOwnershipFlag() const
	{
		return _internalOwnershipFlag;
	}

} // namespace Ic3::System
