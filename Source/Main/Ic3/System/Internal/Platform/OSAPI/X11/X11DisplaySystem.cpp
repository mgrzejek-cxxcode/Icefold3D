
#include "X11DisplaySystem.h"

#if( IC3_PCL_TARGET_SYSAPI == IC3_PCL_TARGET_SYSAPI_X11 )
namespace Ic3::System
{

	namespace Platform
	{

		bool _x11CheckColorFormatSupport( XDisplay pXDisplay, int pScreenIndex, EColorFormat pColorFormat );

	}


	X11DisplayManager::X11DisplayManager( SysContextHandle pSysContext )
	: X11NativeObject( std::move( pSysContext ) )
	{
		_initializeX11DisplayManagerState();
	}

	X11DisplayManager::~X11DisplayManager() noexcept
	{
		_releaseX11DisplayManagerState();
	}

	void X11DisplayManager::_initializeX11DisplayManagerState()
	{
		auto & xSessionData = Platform::x11GetXSessionData( *this );

		// Get default screen depth for the X screen.
		auto screenDepth = XDefaultDepth( xSessionData.mDisplay, xSessionData.mScreenIndex );

		if( screenDepth < 32 )
		{
			XVisualInfo visualInfo;
			if( XMatchVisualInfo( xSessionData.mDisplay, xSessionData.mScreenIndex, 32, TrueColor, &visualInfo ) == True )
			{
				screenDepth = 32;
			}
		}

		mNativeData.mScreenDepth = screenDepth;

		if( mNativeData.mXRRVersion == CX_VERSION_UNKNOWN )
		{
			int xrrVersionMajor = 0;
			int xrrVersionMinor = 0;

			if( XRRQueryVersion( xSessionData.mDisplay, &xrrVersionMajor, &xrrVersionMinor ) == False )
			{
				return;
			}

			mNativeData.mXRRVersion.mNumMajor = static_cast<uint16>( xrrVersionMajor );
			mNativeData.mXRRVersion.mNumMajor = static_cast<uint16>( xrrVersionMinor );
		}

		if( auto * xrrScreenResources = XRRGetScreenResources( xSessionData.mDisplay, xSessionData.mRootWindowXID ) )
		{
			int xrrMonitorsNum = 0;
			auto * xrrMonitorList = XRRGetMonitors( xSessionData.mDisplay, xSessionData.mRootWindowXID, False, &xrrMonitorsNum );

			if( xrrMonitorList && ( xrrMonitorsNum > 0 ) )
			{
                mNativeData.mXRRDefaultMonitorInfo = xrrMonitorList[0];
                for( int monitorIndex = 0; monitorIndex < xrrMonitorsNum; ++monitorIndex )
				{
					auto & xrrMonitorInfo = xrrMonitorList[monitorIndex];
					if( xrrMonitorInfo.primary != 0 )
					{
						mNativeData.mXRRDefaultMonitorInfo = xrrMonitorInfo;
						break;
					}
				}

				XRRFreeMonitors( xrrMonitorList );
			}

			XRRFreeScreenResources( xrrScreenResources );
		}
	}

	void X11DisplayManager::_releaseX11DisplayManagerState()
	{
		mNativeData.mScreenDepth = 0u;
	}

	DisplayDriverHandle X11DisplayManager::_nativeCreateDisplayDriver()
	{
		return createSysObject<X11DisplayDriver>( getHandle<X11DisplayManager>() );
	}

	void X11DisplayManager::_nativeQueryDefaultDisplayOffset( DisplayOffset & pOutOffset ) const
	{
		if( mNativeData.mXRRVersion != CX_VERSION_UNKNOWN )
		{
			pOutOffset.x = static_cast<int32>( mNativeData.mXRRDefaultMonitorInfo.x );
			pOutOffset.y = static_cast<int32>( mNativeData.mXRRDefaultMonitorInfo.y );
		}
		else
		{
			pOutOffset.x = 0;
			pOutOffset.y = 0;
		}
	}

	void X11DisplayManager::_nativeQueryDefaultDisplaySize( DisplaySize & pOutSize ) const
	{
		if( mNativeData.mXRRVersion != CX_VERSION_UNKNOWN )
		{
			pOutSize.x = static_cast<uint32>( mNativeData.mXRRDefaultMonitorInfo.width );
			pOutSize.y = static_cast<uint32>( mNativeData.mXRRDefaultMonitorInfo.height );
		}
		else
		{
			auto & xSessionData = Platform::x11GetXSessionData( *this );

			const auto displayWidth = XDisplayWidth( xSessionData.mDisplay, xSessionData.mScreenIndex );
			const auto displayHeight = XDisplayHeight( xSessionData.mDisplay, xSessionData.mScreenIndex );

			pOutSize.x = static_cast<uint32>( displayWidth );
			pOutSize.y = static_cast<uint32>( displayHeight );
		}
	}

	void X11DisplayManager::_nativeQueryMinWindowSize( DisplaySize & pOutSize ) const
	{
		pOutSize.x = 0u;
		pOutSize.y = 0u;
	}


	X11DisplayDriver::X11DisplayDriver( X11DisplayManagerHandle pDisplayManager )
	: X11NativeObject( std::move( pDisplayManager ), EDisplayDriverType::Generic )
	{
		_initializeX11DisplayDriverState();
	}

	X11DisplayDriver::~X11DisplayDriver() noexcept
	{
		_releaseX11DisplayDriverState();
	}

	void X11DisplayDriver::_initializeX11DisplayDriverState()
	{
		auto & xSessionData = Platform::x11GetXSessionData( *this );

		//
		mNativeData.mXRRScreenResources = XRRGetScreenResources( xSessionData.mDisplay, xSessionData.mRootWindowXID );
		if( mNativeData.mXRRScreenResources == nullptr )
		{
			ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
		}

		//
		mNativeData.mXRRMonitorList = XRRGetMonitors( xSessionData.mDisplay,
		                                             xSessionData.mRootWindowXID,
		                                             False,
		                                             &( mNativeData.mXRRMonitorsNum ) );

		if( ( mNativeData.mXRRMonitorList == nullptr ) || ( mNativeData.mXRRMonitorsNum <= 0 ) )
		{
			ic3Throw( E_EXC_DEBUG_PLACEHOLDER ); // ExsThrowException( EXC_Internal_Error );
		}

        mNativeData.mXRRDefaultMonitorInfo = &( mNativeData.mXRRMonitorList[0] );
		for( int monitorIndex = 0; monitorIndex < mNativeData.mXRRMonitorsNum; ++monitorIndex )
		{
			auto & monitorInfo = mNativeData.mXRRMonitorList[monitorIndex];
			if( monitorInfo.primary != 0 )
			{
				mNativeData.mXRRDefaultMonitorInfo = &monitorInfo;
                break;
			}
		}

		for( int xrrModeIndex = 0; xrrModeIndex < mNativeData.mXRRScreenResources->nmode; ++xrrModeIndex )
		{
			auto & xrrModeInfo = mNativeData.mXRRScreenResources->modes[xrrModeIndex];
			mNativeData.mXRRModeInfoMap[xrrModeInfo.id] = &xrrModeInfo;
		}
	}
	
	void X11DisplayDriver::_releaseX11DisplayDriverState()
	{
		if( mNativeData.mXRRScreenResources != nullptr )
		{
			XRRFreeScreenResources( mNativeData.mXRRScreenResources );
			mNativeData.mXRRScreenResources = nullptr;
		}

		if( mNativeData.mXRRMonitorList != nullptr )
		{
			XRRFreeMonitors( mNativeData.mXRRMonitorList );
			mNativeData.mXRRMonitorList = nullptr;
			mNativeData.mXRRMonitorsNum = 0;
		}

		mNativeData.mScreenDepth = 0;
		mNativeData.resetSessionData();
	}

	void X11DisplayDriver::_nativeEnumDisplayDevices()
	{
		auto & xSessionData = Platform::x11GetXSessionData( *this );

		auto adapterObject = createAdapter<X11DisplayAdapter>( *this );

		auto & adapterDesc = getAdapterDescInternal( *adapterObject );
		adapterDesc.mName = "XRR Default Adapter";
		adapterDesc.mVendorID = EDisplayAdapterVendorID::Unknown;
		adapterDesc.mFlags.set( eDisplayAdapterFlagActiveBit );
		adapterDesc.mFlags.set( eDisplayAdapterFlagPrimaryBit );

		for( size_t monitorIndex = 0; monitorIndex < mNativeData.mXRRMonitorsNum; ++monitorIndex )
		{
			auto & monitorInfo = mNativeData.mXRRMonitorList[monitorIndex];
			//char * monitorName = XGetAtomName( xSessionData.mDisplay, monitorInfo.mName );

			// Iterate over monitor's outputs and find the one used by the system.
			for( int monitorOutputIndex = 0; monitorOutputIndex < monitorInfo.noutput; ++monitorOutputIndex )
			{
				auto monitorOutputID = monitorInfo.outputs[monitorOutputIndex];
				auto * monitorOutputInfo = XRRGetOutputInfo( xSessionData.mDisplay,
				                                             mNativeData.mXRRScreenResources,
				                                             monitorOutputID );

				if( monitorOutputInfo )
				{
					if( ( monitorInfo.width != 0 ) && ( monitorInfo.height != 0 ) )
					{
						auto outputObject = createOutput<X11DisplayOutput>( *adapterObject );
						outputObject->mNativeData.mXRROutputID = monitorOutputID;
						outputObject->mNativeData.mXRRCrtcID = monitorOutputInfo->crtc;

						auto & outputDesc = getOutputDescInternal( *outputObject );
						outputDesc.mName = monitorOutputInfo->name;
						outputDesc.mScreenRect.mOffset.x = monitorInfo.x;
						outputDesc.mScreenRect.mOffset.y = monitorInfo.y;
						outputDesc.mScreenRect.mSize.x = monitorInfo.width;
						outputDesc.mScreenRect.mSize.y = monitorInfo.height;

						if( monitorOutputInfo->connection == RR_Connected )
						{
							outputDesc.mFlags.set( eDisplayOutputFlagActiveBit );
						}

						if( monitorInfo.primary != 0 )
						{
							outputDesc.mFlags.set( eDisplayOutputFlagPrimaryBit );
						}
					}

					XRRFreeOutputInfo( monitorOutputInfo );
				}
			}
		}
	}

	void X11DisplayDriver::_nativeEnumVideoModes( DisplayOutput & pOutput, EColorFormat pColorFormat )
	{
		if( ( pColorFormat != EColorFormat::B8G8R8 ) && ( pColorFormat != EColorFormat::B8G8R8A8 ) )
		{
			return;
		}

		auto & xSessionData = Platform::x11GetXSessionData( *this );
		auto * outputX11 = pOutput.queryInterface<X11DisplayOutput>();

		if( !Platform::_x11CheckColorFormatSupport( xSessionData.mDisplay, xSessionData.mScreenIndex, pColorFormat ) )
		{
			return;
		}

		auto * outputInfo = XRRGetOutputInfo( xSessionData.mDisplay,
		                                      mNativeData.mXRRScreenResources,
		                                      outputX11->mNativeData.mXRROutputID );
		if( outputInfo == nullptr )
		{
			ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
		}

		if( outputInfo->modes != nullptr )
		{
			dsm_video_settings_hash_t lastSettingsHash = CX_DSM_VIDEO_SETTINGS_HASH_INVALID;

			for( size_t modeIndex = 0; modeIndex < outputInfo->nmode; ++modeIndex )
			{
				auto outputModeID = outputInfo->modes[modeIndex];
				auto displayModeInfoPtrIter = mNativeData.mXRRModeInfoMap.find( outputModeID );
				if( displayModeInfoPtrIter == mNativeData.mXRRModeInfoMap.end() )
				{
					continue;
				}

				auto & displayModeInfo = *( displayModeInfoPtrIter->second );

				if( ( displayModeInfo.modeFlags & ( RR_DoubleScan | RR_PixelMultiplex ) ) != 0 )
				{
					continue;
				}

				auto rateVHTotal = displayModeInfo.hTotal * displayModeInfo.vTotal;
				auto refreshRate = 0U;

				if( rateVHTotal != 0 )
				{
					refreshRate = static_cast<uint32_t>( std::round( ( double )displayModeInfo.dotClock / ( double )rateVHTotal ) );
				}

				DisplayVideoSettings videoSettings;
				videoSettings.mResolution.x = static_cast<uint32>( displayModeInfo.width );
				videoSettings.mResolution.y = static_cast<uint32>( displayModeInfo.height );
				videoSettings.mRefreshRate = static_cast<uint16>( refreshRate );

				if( ( displayModeInfo.modeFlags & RR_Interlace ) != 0 )
				{
					videoSettings.mFlags.set( eDisplayVideoSettingsFlagScanInterlacedBit );
				}
				else
				{
					videoSettings.mFlags.set( eDisplayVideoSettingsFlagScanProgressiveBit );
				}

				auto settingsHash = dsmComputeVideoSettingsHash( pColorFormat, videoSettings );
				if( settingsHash == lastSettingsHash )
				{
					continue;
				}

				auto videoModeObject = createVideoMode<X11DisplayVideoMode>( *outputX11, pColorFormat );
				videoModeObject->mNativeData.mXRRModeID = displayModeInfo.id;
				videoModeObject->mNativeData.mXRRModeInfo = &displayModeInfo;

				auto & videoModeDesc = getVideoModeDescInternal( *videoModeObject );
				videoModeDesc.mSettings = videoSettings;
				videoModeDesc.mSettingsHash = settingsHash;

				lastSettingsHash = settingsHash;
			}
		}

		XRRFreeOutputInfo( outputInfo );
	}

	EColorFormat X11DisplayDriver::_nativeQueryDefaultSystemColorFormat() const
	{
		return EColorFormat::B8G8R8A8;
	}


	namespace Platform
	{

		bool _x11CheckColorFormatSupport( XDisplay pXDisplay, int pScreenIndex, EColorFormat pColorFormat )
		{
			const auto & colorFormatDesc = vsxGetDescForColorFormat( pColorFormat );

			XVisualInfo visualInfo;
			if( XMatchVisualInfo( pXDisplay, pScreenIndex, colorFormatDesc.mSize, TrueColor, &visualInfo ) == True )
			{
				return true;
			}
			return false;
		}

	}

} // namespace Ic3::System
#endif // IC3_PCL_TARGET_SYSAPI_X11
