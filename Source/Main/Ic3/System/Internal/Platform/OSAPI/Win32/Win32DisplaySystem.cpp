
#include "Win32DisplaySystem.h"
#include <Ic3/System/Internal/DisplaySystemPrivate.h>
#include <cppx/stringUtils.h>

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_WIN32 )
namespace Ic3::System
{

	namespace Platform
	{

		// Returns a name for an output by extracting the output part from the DisplayDevice registry key.
		std::string _Win32GetAdapterOutputName( const std::string & pAdapterRegistryKey );

	}


	Win32DisplayManager::Win32DisplayManager( SysContextHandle pSysContext )
	: DisplayManager( std::move( pSysContext ) )
	{}

	Win32DisplayManager::~Win32DisplayManager() noexcept = default;

	DisplayDriverHandle Win32DisplayManager::_NativeCreateDisplayDriver()
	{
		return CreateSysObject<Win32DisplayDriver>( GetHandle<Win32DisplayManager>() );
	}

    void Win32DisplayManager::_NativeQueryDefaultDisplayOffset( DisplayOffset & pOutOffset ) const
    {
        pOutOffset.x = 0;
        pOutOffset.y = 0;
    }

	void Win32DisplayManager::_NativeQueryDefaultDisplaySize( DisplaySize & pOutSize ) const
	{
		auto cxScreen = ::GetSystemMetrics( SM_CXSCREEN );
		auto cyScreen = ::GetSystemMetrics( SM_CYSCREEN );
		pOutSize.x = static_cast<uint32>( cxScreen );
		pOutSize.y = static_cast<uint32>( cyScreen );
	}

	void Win32DisplayManager::_NativeQueryMinWindowSize( DisplaySize & pOutSize ) const
	{
		auto cxMin = ::GetSystemMetrics( SM_CXMIN );
		auto cyMin = ::GetSystemMetrics( SM_CYMIN );
		pOutSize.x = static_cast<uint32>( cxMin );
		pOutSize.y = static_cast<uint32>( cyMin );
	}


	Win32DisplayDriver::Win32DisplayDriver( DisplayManagerHandle pDisplayManager )
	: Win32NativeObject( std::move( pDisplayManager ), EDisplayDriverType::Generic )
	{}

	Win32DisplayDriver::~Win32DisplayDriver() noexcept = default;

	// -- Note on adapters enumeration:
	// Without the awesome DXGI, EnumDisplayDevices is the only reliable way of enumerating the display stuff.
	// With its first parameter set to NULL, it queries the information about an adapter with the specified index.
	// Adapter is identified as a registry key, however - it is tied with its connected monitors. Example:
	// In my current setup (AMD Radeon RX580 and two 1440p monitors connected via DisplayPort), EnumDisplayDevices
	// gives me two entries with the following keys (DISPLAY_DEVICEA::DeviceKey):
	// 1) \\Registry\\Machine\\System\\CurrentControlSet\\Control\\Video\\{79BD17DD-B591-11EA-B520-AC9E17ECDDE5}\\0000
	// 2) \\Registry\\Machine\\System\\CurrentControlSet\\Control\\Video\\{79BD17DD-B591-11EA-B520-AC9E17ECDDE5}\\0001
	// So, to enumerate adapters properly, we must check the UUID of the adapter to not duplicate the entries.
	// See SysDisplayDriverImplProxy::nativeEnuadapterList below.
	void Win32DisplayDriver::_NativeEnumDisplayDevices()
	{
		// Represents information about a display device in the system. String properties have the following meaning:
		// ::DeviceID - PCI-specific ID, not really interesting
		// ::DeviceString - name of the adapter, e.g. "Radeon RX580 Series"
		// ::DeviceName - detailed name of the device, e.g. "\\\\.\\DISPLAY1". For adapters, this is the name of a connected display
		// ::DeviceKey - device's key in the registry, e.g. "\\Registry\\Machine\\System\\CurrentControlSet\\Control\\Video\\{SOME_UUID}"

		DISPLAY_DEVICEA adapterInfoGDI;
		adapterInfoGDI.cb = sizeof( DISPLAY_DEVICEA );

		DISPLAY_DEVICEA outputInfoGDI;
		outputInfoGDI.cb = sizeof( DISPLAY_DEVICEA );

		for( UINT displayDeviceIndex = 0; ; ++displayDeviceIndex )
		{
			// Enumerate next display device in the system. Multiple devices may refer to the same physical adapter, i.e.:
			// "\\Registry\\Machine\\System\\CurrentControlSet\\Control\\Video\\{79BD17DD-B591-11EA-B520-AC9E17ECDDE5}\\0000"
			// "\\Registry\\Machine\\System\\CurrentControlSet\\Control\\Video\\{79BD17DD-B591-11EA-B520-AC9E17ECDDE5}\\0001"
			// are two different physical outputs (0000 and 0001) connected to the same adapter (UUID={79BD17DD-B591-11EA-B520-AC9E17ECDDE5}).
			BOOL enumResult = ::EnumDisplayDevicesA( nullptr, displayDeviceIndex, &adapterInfoGDI, 0 );

			// Display devices are indexed starting from 0 up to DEVICES_NUM - 1.
			// If EnumDisplayDevicesA returns FALSE, it means we enumerated all of them already.
			if( enumResult == FALSE )
			{
				break;
			}

			// Extract adapter UUID from the registry key. Devices referring to the same adapter have the same adapter UUID.
			auto adapterUUID = cppx::get_uuid_string( adapterInfoGDI.DeviceKey );
			// Check if that adapter has been already added to the list of adapters (if there was already another device which referred to it).
			auto adapterObject = _FindAdapterByUUID( adapterUUID );

			if( adapterObject == nullptr )
			{
				auto newAdapterObject = CreateAdapter<Win32DisplayAdapter>( *this );
				newAdapterObject->mNativeData.deviceUUID = std::move( adapterUUID );
				newAdapterObject->mNativeData.deviceName = adapterInfoGDI.DeviceName;

				auto & adapterDesc = GetAdapterDescInternal( *newAdapterObject );
				adapterDesc.name = adapterInfoGDI.DeviceString;

				if( cppx::make_bitmask( adapterInfoGDI.StateFlags ).is_set( DISPLAY_DEVICE_ATTACHED_TO_DESKTOP ) )
				{
					adapterDesc.flags.set( eDisplayAdapterFlagActiveBit );
				}
				if( cppx::make_bitmask( adapterInfoGDI.StateFlags ).is_set( DISPLAY_DEVICE_PRIMARY_DEVICE ) )
				{
					adapterDesc.flags.set( eDisplayAdapterFlagPrimaryBit );
				}

				adapterObject = std::move( newAdapterObject );
			}

			for( UINT adapterOutputIndex = 0; ; ++adapterOutputIndex )
			{
				//
				enumResult = ::EnumDisplayDevicesA( adapterInfoGDI.DeviceName, adapterOutputIndex, &outputInfoGDI, 0 );
				if( enumResult == FALSE )
				{
					break;
				}

				auto outputObject = CreateOutput<Win32DisplayOutput>( *adapterObject );
				outputObject->mNativeData.displayDeviceName = adapterInfoGDI.DeviceName;
				outputObject->mNativeData.outputID = outputInfoGDI.DeviceName;

				auto & outputDesc = GetOutputDescInternal( *outputObject );
				// NOTE: 'DISPLAY_DEVICE_PRIMARY_DEVICE' flag is not set in case of output devices (unlike adapters).
				// Primary output can be detected by analysing at monitor flags and looking for MONITORINFOF_PRIMARY.
				// See _win32MonitorEnumProc function above where this gets done.
				if( cppx::make_bitmask( outputInfoGDI.StateFlags ).is_set( DISPLAY_DEVICE_ATTACHED_TO_DESKTOP ) )
				{
					outputDesc.flags.set( eDisplayOutputFlagActiveBit );
				}
			}
		}

		::EnumDisplayMonitors( nullptr, nullptr, _win32MonitorEnumProc, reinterpret_cast<LPARAM>( this ) );
	}

	// Monitor enumeration function. Called for each monitor in the system (as a part of _NativeEnumOutputs).
	// This function is called after all output devices in the system have been enumerated and added to the
	// internal list inside the adapter. IMPORTANT: again, this gets called *within* _NativeEnumOutputs()
	// function *per each adapter* (that's why we pass DisplayAdapter* and check for monitors connected to it.
	BOOL CALLBACK Win32DisplayDriver::_win32MonitorEnumProc( HMONITOR pMonitorHandle, HDC pHDC, LPRECT pMonitorRect, LPARAM pUserParam )
	{
		// Adapter object for which we are enumerating outputs/monitors.
		auto * win32DisplayDriver = reinterpret_cast<Win32DisplayDriver *>( pUserParam );

		MONITORINFOEXA gdiMonitorInfo;
		gdiMonitorInfo.cbSize = sizeof( MONITORINFOEXA );

		if( ::GetMonitorInfoA( pMonitorHandle, &gdiMonitorInfo ) != FALSE )
		{
			// 'szDevice' of the MONITORINFOEXA matches the DISPLAY_DEVICEA::DeviceName property of the output.
			// Thanks to that, we can obtain the output this monitor refers to.
			if( auto outputObject = win32DisplayDriver->_FindAnyOutputForDisplayDeviceName( gdiMonitorInfo.szDevice ) )
			{
				auto * win32OutputObject = outputObject->QueryInterface<Win32DisplayOutput>();
				win32OutputObject->mNativeData.gdiMonitorHandle = pMonitorHandle;

				auto & outputDesc = GetOutputDescInternal( *win32OutputObject );
				outputDesc.name = cppx::strutil::convert_string_representation<char>( gdiMonitorInfo.szDevice );
				outputDesc.screenRect.offset.x = gdiMonitorInfo.rcMonitor.left;
				outputDesc.screenRect.offset.y = gdiMonitorInfo.rcMonitor.top;
				outputDesc.screenRect.size.x = gdiMonitorInfo.rcMonitor.right - gdiMonitorInfo.rcMonitor.left;
				outputDesc.screenRect.size.y = gdiMonitorInfo.rcMonitor.bottom - gdiMonitorInfo.rcMonitor.top;

				if( cppx::make_bitmask( gdiMonitorInfo.dwFlags ).is_set( MONITORINFOF_PRIMARY ) )
				{
					outputDesc.flags.set( eDisplayOutputFlagPrimaryBit );
				}
			}
		}

		return TRUE;
	}

	void Win32DisplayDriver::_NativeEnumVideoModes( DisplayOutput & pOutput, EColorFormat pColorFormat )
	{
		auto * outputWin32 = pOutput.QueryInterface<Win32DisplayOutput>();

		const auto & colorFormatDesc = VisGetDescForColorFormat( pColorFormat );

		if( colorFormatDesc.colorSpace != EColorSpace::Linear )
		{
			return;
		}

		if( ( colorFormatDesc.rgba.u8_red != 8 ) || ( colorFormatDesc.rgba.u8_green != 8 ) || ( colorFormatDesc.rgba.u8_blue != 8 ) )
		{
			return;
		}

		DEVMODEA gdiDevMode;
		gdiDevMode.dmSize = sizeof( DEVMODEA );
		gdiDevMode.dmDriverExtra = 0;

		dsm_video_settings_hash_t lastSettingsHash = CX_DSM_VIDEO_SETTINGS_HASH_INVALID;

		for( UINT displayModeIndex = 0; ; ++displayModeIndex )
		{
			auto * displayDeviceNameStr = outputWin32->mNativeData.displayDeviceName.c_str();

			BOOL edsResult = ::EnumDisplaySettingsExA( displayDeviceNameStr, displayModeIndex, &gdiDevMode, 0 );

			if( edsResult == FALSE )
			{
				break;
			}

			if( gdiDevMode.dmBitsPerPel != colorFormatDesc.size )
			{
				continue;
			}

			DisplayVideoSettings videoSettings;
			videoSettings.resolution.x = static_cast<uint32>( gdiDevMode.dmPelsWidth );
			videoSettings.resolution.y = static_cast<uint32>( gdiDevMode.dmPelsHeight );
			videoSettings.refreshRate = static_cast<uint16>( gdiDevMode.dmDisplayFrequency );

			if( cppx::make_bitmask( gdiDevMode.dmDisplayFlags ).is_set( DM_INTERLACED ) )
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

			auto videoModeObject = CreateVideoMode<Win32DisplayVideoMode>( *outputWin32, pColorFormat );
			videoModeObject->mNativeData.gdiModeInfo = gdiDevMode;

			auto & videoModeDesc = GetVideoModeDescInternal( *videoModeObject );
			videoModeDesc.settings = videoSettings;
			videoModeDesc.settingsHash = settingsHash;

			lastSettingsHash = settingsHash;
		}
	}

	EColorFormat Win32DisplayDriver::_NativeQueryDefaultSystemColorFormat() const
	{
		return EColorFormat::B8G8R8A8;
	}


	TSysHandle<Win32DisplayAdapter> Win32DisplayDriver::_FindAdapterByUUID( const std::string & pUUID )
	{
		auto displayAdapter = FindAdapter( [&pUUID]( const DisplayAdapter & pAdapter ) -> bool {
			auto * win32Adapter = pAdapter.QueryInterface<Win32DisplayAdapter>();
			return win32Adapter->mNativeData.deviceUUID == pUUID;
		} );
		return displayAdapter ? displayAdapter->GetHandle<Win32DisplayAdapter>() : nullptr;
	}

	TSysHandle<Win32DisplayOutput> Win32DisplayDriver::_FindAdapterOutputForDisplayDeviceName(
			DisplayAdapter & pAdapter,
			const char * pDeviceName )
	{
		auto displayOutput = pAdapter.FindOutput( [pDeviceName]( const DisplayOutput & pOutput ) -> bool {
			auto * win32Output = pOutput.QueryInterface<Win32DisplayOutput>();
			return win32Output->mNativeData.displayDeviceName == pDeviceName;
		} );
		return displayOutput ? displayOutput->GetHandle<Win32DisplayOutput>() : nullptr;
	}

	TSysHandle<Win32DisplayOutput> Win32DisplayDriver::_FindAnyOutputForDisplayDeviceName( const char * pDeviceName )
	{
		for( auto & adapter : _privateData->adapterInstanceList )
		{
			if( auto adapterOutput = _FindAdapterOutputForDisplayDeviceName( *adapter, pDeviceName ) )
			{
				return adapterOutput;
			}
		}

		return nullptr;
	}


	namespace Platform
	{

		std::string _Win32GetAdapterOutputName( const std::string & pAdapterRegistryKey )
		{
			auto lastSepPos = pAdapterRegistryKey.find_last_of( '\\', 0 );
			return pAdapterRegistryKey.substr( 0, lastSepPos );
		}

	}
	
} // namespace Ic3::System
#endif // PCL_TARGET_SYSAPI_WIN32
