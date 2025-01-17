
#include "X11DisplaySystem.h"

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_X11 )
namespace Ic3::System
{

	namespace Platform
	{

		bool _x11CheckColorFormatSupport( XDisplay pXDisplay, int pScreenIndex, EColorFormat pColorFormat );

	}


	X11DisplayManager::X11DisplayManager( SysContextHandle pSysContext )
	: X11NativeObject( std::move( pSysContext ) )
	{
		_InitializeX11DisplayManagerState();
	}

	X11DisplayManager::~X11DisplayManager() noexcept
	{
		_ReleaseX11DisplayManagerState();
	}

	void X11DisplayManager::_InitializeX11DisplayManagerState()
	{
		auto & xSessionData = Platform::X11GetXSessionData( *this );

		// Get default screen depth for the X screen.
		auto screenDepth = XDefaultDepth( xSessionData.displayHandle, xSessionData.screenIndex );

		if( screenDepth < 32 )
		{
			XVisualInfo visualInfo;
			if( XMatchVisualInfo( xSessionData.displayHandle, xSessionData.screenIndex, 32, TrueColor, &visualInfo ) == True )
			{
				screenDepth = 32;
			}
		}

		mNativeData.mScreenDepth = screenDepth;

		if( mNativeData.mXRRVersion == CX_VERSION_UNKNOWN )
		{
			int xrrVersionMajor = 0;
			int xrrVersionMinor = 0;

			if( XRRQueryVersion( xSessionData.displayHandle, &xrrVersionMajor, &xrrVersionMinor ) == False )
			{
				return;
			}

			mNativeData.mXRRVersion.num_major = static_cast<uint16>( xrrVersionMajor );
			mNativeData.mXRRVersion.num_major = static_cast<uint16>( xrrVersionMinor );
		}

		if( auto * xrrScreenResources = XRRGetScreenResources( xSessionData.displayHandle, xSessionData.rootWindowXID ) )
		{
			int xrrMonitorsNum = 0;
			auto * xrrMonitorList = XRRGetMonitors( xSessionData.displayHandle, xSessionData.rootWindowXID, False, &xrrMonitorsNum );

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

	void X11DisplayManager::_ReleaseX11DisplayManagerState()
	{
		mNativeData.mScreenDepth = 0u;
	}

	DisplayDriverHandle X11DisplayManager::_NativeCreateDisplayDriver()
	{
		return CreateSysObject<X11DisplayDriver>( GetHandle<X11DisplayManager>() );
	}

	void X11DisplayManager::_NativeQueryDefaultDisplayOffset( DisplayOffset & pOutOffset ) const
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

	void X11DisplayManager::_NativeQueryDefaultDisplaySize( DisplaySize & pOutSize ) const
	{
		if( mNativeData.mXRRVersion != CX_VERSION_UNKNOWN )
		{
			pOutSize.x = static_cast<uint32>( mNativeData.mXRRDefaultMonitorInfo.width );
			pOutSize.y = static_cast<uint32>( mNativeData.mXRRDefaultMonitorInfo.height );
		}
		else
		{
			auto & xSessionData = Platform::X11GetXSessionData( *this );

			const auto displayWidth = XDisplayWidth( xSessionData.displayHandle, xSessionData.screenIndex );
			const auto displayHeight = XDisplayHeight( xSessionData.displayHandle, xSessionData.screenIndex );

			pOutSize.x = static_cast<uint32>( displayWidth );
			pOutSize.y = static_cast<uint32>( displayHeight );
		}
	}

	void X11DisplayManager::_NativeQueryMinWindowSize( DisplaySize & pOutSize ) const
	{
		pOutSize.x = 0u;
		pOutSize.y = 0u;
	}


	X11DisplayDriver::X11DisplayDriver( X11DisplayManagerHandle pDisplayManager )
	: X11NativeObject( std::move( pDisplayManager ), EDisplayDriverType::Generic )
	{
		_InitializeX11DisplayDriverState();
	}

	X11DisplayDriver::~X11DisplayDriver() noexcept
	{
		_ReleaseX11DisplayDriverState();
	}

	void X11DisplayDriver::_InitializeX11DisplayDriverState()
	{
		auto & xSessionData = Platform::X11GetXSessionData( *this );

		//
		mNativeData.mXRRScreenResources = XRRGetScreenResources( xSessionData.displayHandle, xSessionData.rootWindowXID );
		if( mNativeData.mXRRScreenResources == nullptr )
		{
			Ic3Throw( eExcCodeDebugPlaceholder );
		}

		//
		mNativeData.mXRRMonitorList = XRRGetMonitors( xSessionData.displayHandle,
		                                             xSessionData.rootWindowXID,
		                                             False,
		                                             &( mNativeData.mXRRMonitorsNum ) );

		if( ( mNativeData.mXRRMonitorList == nullptr ) || ( mNativeData.mXRRMonitorsNum <= 0 ) )
		{
			Ic3Throw( eExcCodeDebugPlaceholder ); // ExsThrowException( EXC_Internal_Error );
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
	
	void X11DisplayDriver::_ReleaseX11DisplayDriverState()
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
		mNativeData.ResetSessionData();
	}

	void X11DisplayDriver::_NativeEnumDisplayDevices()
	{
		auto & xSessionData = Platform::X11GetXSessionData( *this );

		auto adapterObject = CreateAdapter<X11DisplayAdapter>( *this );

		auto & adapterDesc = GetAdapterDescInternal( *adapterObject );
		adapterDesc.mName = "XRR Default Adapter";
		adapterDesc.vendorID = EDisplayAdapterVendorID::Unknown;
		adapterDesc.flags.set( eDisplayAdapterFlagActiveBit );
		adapterDesc.flags.set( eDisplayAdapterFlagPrimaryBit );

		for( size_t monitorIndex = 0; monitorIndex < mNativeData.mXRRMonitorsNum; ++monitorIndex )
		{
			auto & monitorInfo = mNativeData.mXRRMonitorList[monitorIndex];
			//char * monitorName = XGetAtomName( xSessionData.displayHandle, monitorInfo.mName );

			// Iterate over monitor's outputs and find the one used by the system.
			for( int monitorOutputIndex = 0; monitorOutputIndex < monitorInfo.noutput; ++monitorOutputIndex )
			{
				auto monitorOutputID = monitorInfo.outputs[monitorOutputIndex];
				auto * monitorOutputInfo = XRRGetOutputInfo( xSessionData.displayHandle,
				                                             mNativeData.mXRRScreenResources,
				                                             monitorOutputID );

				if( monitorOutputInfo )
				{
					if( ( monitorInfo.width != 0 ) && ( monitorInfo.height != 0 ) )
					{
						auto outputObject = CreateOutput<X11DisplayOutput>( *adapterObject );
						outputObject->mNativeData.mXRROutputID = monitorOutputID;
						outputObject->mNativeData.mXRRCrtcID = monitorOutputInfo->crtc;

						auto & outputDesc = GetOutputDescInternal( *outputObject );
						outputDesc.mName = monitorOutputInfo->name;
						outputDesc.screenRect.offset.x = monitorInfo.x;
						outputDesc.screenRect.offset.y = monitorInfo.y;
						outputDesc.screenRect.size.x = monitorInfo.width;
						outputDesc.screenRect.size.y = monitorInfo.height;

						if( monitorOutputInfo->connection == RR_Connected )
						{
							outputDesc.flags.set( eDisplayOutputFlagActiveBit );
						}

						if( monitorInfo.primary != 0 )
						{
							outputDesc.flags.set( eDisplayOutputFlagPrimaryBit );
						}
					}

					XRRFreeOutputInfo( monitorOutputInfo );
				}
			}
		}
	}

	void X11DisplayDriver::_NativeEnumVideoModes( DisplayOutput & pOutput, EColorFormat pColorFormat )
	{
		if( ( pColorFormat != EColorFormat::B8G8R8 ) && ( pColorFormat != EColorFormat::B8G8R8A8 ) )
		{
			return;
		}

		auto & xSessionData = Platform::X11GetXSessionData( *this );
		auto * outputX11 = pOutput.QueryInterface<X11DisplayOutput>();

		if( !Platform::_x11CheckColorFormatSupport( xSessionData.displayHandle, xSessionData.screenIndex, pColorFormat ) )
		{
			return;
		}

		auto * outputInfo = XRRGetOutputInfo( xSessionData.displayHandle,
		                                      mNativeData.mXRRScreenResources,
		                                      outputX11->mNativeData.mXRROutputID );
		if( outputInfo == nullptr )
		{
			Ic3Throw( eExcCodeDebugPlaceholder );
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
				videoSettings.resolution.x = static_cast<uint32>( displayModeInfo.width );
				videoSettings.resolution.y = static_cast<uint32>( displayModeInfo.height );
				videoSettings.mRefreshRate = static_cast<uint16>( refreshRate );

				if( ( displayModeInfo.modeFlags & RR_Interlace ) != 0 )
				{
					videoSettings.flags.set( eDisplayVideoSettingsFlagScanInterlacedBit );
				}
				else
				{
					videoSettings.flags.set( eDisplayVideoSettingsFlagScanProgressiveBit );
				}

				auto settingsHash = DSMComputeVideoSettingsHash( pColorFormat, videoSettings );
				if( settingsHash == lastSettingsHash )
				{
					continue;
				}

				auto videoModeObject = CreateVideoMode<X11DisplayVideoMode>( *outputX11, pColorFormat );
				videoModeObject->mNativeData.mXRRModeID = displayModeInfo.id;
				videoModeObject->mNativeData.mXRRModeInfo = &displayModeInfo;

				auto & videoModeDesc = GetVideoModeDescInternal( *videoModeObject );
				videoModeDesc.settings = videoSettings;
				videoModeDesc.settingsHash = settingsHash;

				lastSettingsHash = settingsHash;
			}
		}

		XRRFreeOutputInfo( outputInfo );
	}

	EColorFormat X11DisplayDriver::_NativeQueryDefaultSystemColorFormat() const
	{
		return EColorFormat::B8G8R8A8;
	}


	namespace Platform
	{

		bool _x11CheckColorFormatSupport( XDisplay pXDisplay, int pScreenIndex, EColorFormat pColorFormat )
		{
			const auto & colorFormatDesc = VisGetDescForColorFormat( pColorFormat );

			XVisualInfo visualInfo;
			if( XMatchVisualInfo( pXDisplay, pScreenIndex, colorFormatDesc.size, TrueColor, &visualInfo ) == True )
			{
				return true;
			}
			return false;
		}

	}

} // namespace Ic3::System
#endif // PCL_TARGET_SYSAPI_X11
