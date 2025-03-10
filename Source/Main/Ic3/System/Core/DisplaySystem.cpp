
#include "../Internal/DisplaySystemPrivate.h"

#if( IC3_SYSTEM_DSM_DRIVER_TYPE_SUPPORT_DXGI )
#  include "../Internal/Platform/Shared/DXGI/DXGIDisplaySystem.h"
#endif

namespace Ic3::System
{

	DisplayManager::DisplayManager( SysContextHandle pSysContext )
	: SysObject( std::move( pSysContext ) )
	, _privateData( std::make_unique<DisplayManagerPrivateData>() )
	{
		_privateData->driverFactoryMap[EDisplayDriverType::Generic] = [this]() {
			return CreateDisplayDriver();
		};
	#if( IC3_SYSTEM_DSM_DRIVER_TYPE_SUPPORT_DXGI )
		_privateData->driverFactoryMap[EDisplayDriverType::DXGI] = [this]() {
			return CreateSysObject<DXGIDisplayDriver>( this->GetHandle<DisplayManager>() );
		};
	#endif
	#if( IC3_SYSTEM_DSM_DRIVER_TYPE_SUPPORT_SDL )
		_privateData->driverFactoryMap[EDisplayDriverType::SDL] = [this]() {
			return CreateSysObject<DisplayDriverSDL>( GetHandle<DisplayManager>() );
		};
	#endif
	}

	DisplayManager::~DisplayManager() noexcept = default;

	DisplayDriverHandle DisplayManager::CreateDisplayDriver( EDisplayDriverType pDriverID )
	{
		pDriverID = ResolveDisplayDriverID( pDriverID );
		if( pDriverID == EDisplayDriverType::Unknown )
		{
			return nullptr;
		}

		const auto & factoryCallback = _privateData->driverFactoryMap.at( pDriverID );
		auto displayDriver = factoryCallback();

		return displayDriver;
	}

	DisplayDriverHandle DisplayManager::CreateDisplayDriver()
	{
		return _NativeCreateDisplayDriver();
	}

	bool DisplayManager::CheckDriverSupport( EDisplayDriverType pDriverID ) const
	{
		// The default driver is always supported. If no driver is available on
		// a given system, DisplayManager will fail to create in the first place,
		if( pDriverID == EDisplayDriverType::Default )
		{
			return true;
		}

		// Resolve the driver ID. This will return EDisplayDriverType::Unknown
		// if the specified driver is not supported on the current system.
		const auto resolvedDriverID = ResolveDisplayDriverID( pDriverID );

		return resolvedDriverID != EDisplayDriverType::Unknown;
	}

	EDisplayDriverType DisplayManager::ResolveDisplayDriverID( EDisplayDriverType pDriverID ) const
	{
		EDisplayDriverType resolvedDriverID = EDisplayDriverType::Unknown;

		if( pDriverID == EDisplayDriverType::Generic )
		{
			resolvedDriverID = EDisplayDriverType::Generic;
		}
		else if( pDriverID == EDisplayDriverType::Default )
		{
		#if( PCL_TARGET_OS & PCL_TARGET_FLAG_OS_WFA )
			resolvedDriverID = EDisplayDriverType::DXGI;
		#else
			resolvedDriverID = EDisplayDriverType::Generic;
		#endif
		}
	#if( PCL_TARGET_OS & PCL_TARGET_FLAG_OS_WFA )
		else if( pDriverID == EDisplayDriverType::DXGI )
		{
			resolvedDriverID = EDisplayDriverType::DXGI;
		}
	#endif

		return resolvedDriverID;
	}

	DisplayOffset DisplayManager::QueryDefaultDisplayOffset() const
	{
		DisplayOffset result;
		_NativeQueryDefaultDisplayOffset( result );
		return result;
	}

	DisplaySize DisplayManager::QueryDefaultDisplaySize() const
	{
		DisplaySize result;
		_NativeQueryDefaultDisplaySize( result );
		return result;
	}

	DisplaySize DisplayManager::QueryMinWindowSize() const
	{
		DisplaySize result;
		_NativeQueryMinWindowSize( result );
		return result;
	}

	bool DisplayManager::CheckFrameGeometry( const FrameGeometry & pFrameGeometry ) const
	{
		const auto & framePos = pFrameGeometry.position;
		const auto & frameSize = pFrameGeometry.size;

		const auto screenSize = QueryDefaultDisplaySize();
		const auto minFrameSize = QueryMinWindowSize();

		if( frameSize != cxFrameSizeMax )
		{
			if( ( frameSize.x == 0 ) || ( frameSize.y == 0 ) )
			{
				return false;
			}
			if( ( frameSize.x > screenSize.x ) || ( frameSize.y > screenSize.y ) )
			{
				return false;
			}
			if( ( frameSize.x < minFrameSize.x ) || ( frameSize.y < minFrameSize.y ) )
			{
				return false;
			}
		}

		if( framePos != cxFramePosAuto )
		{
			if( ( framePos.x < 0 ) || ( framePos.y < 0 ) )
			{
				return false;
			}

			const auto maxPosX = static_cast<int32>( screenSize.x - frameSize.x );
			const auto maxPosY = static_cast<int32>( screenSize.y - frameSize.y );

			if( ( framePos.x > maxPosX ) || ( framePos.y > maxPosY ) )
			{
				return false;
			}
		}

		return true;
	}

	FrameGeometry DisplayManager::ValidateFrameGeometry( const FrameGeometry & pFrameGeometry ) const
	{
		auto resultGeometry = pFrameGeometry;

		const auto & framePos = pFrameGeometry.position;
		const auto & frameSize = pFrameGeometry.size;

		const auto screenOffset = QueryDefaultDisplayOffset();
		const auto screenSize = QueryDefaultDisplaySize();
		const auto minFrameSize = QueryMinWindowSize();

		if( frameSize == cxFrameSizeMax )
		{
			// Frame is defined as maximum allowed for the current display/screen. Make it that big.
			resultGeometry.size.x = screenSize.x;
			resultGeometry.size.y = screenSize.y;
		}
		else
		{
			if( ( frameSize.x == 0 ) || ( frameSize.y == 0 ) )
			{
				// Any dimension set to 0 means "use default size". By default,
				// we just use the ratio of the screen and 70% of its dimensions.
				resultGeometry.size.x = static_cast<uint32>( screenSize.x * 0.7 );
				resultGeometry.size.y = static_cast<uint32>( screenSize.y * 0.7 );
			}
			else
			{
				// Size of the window must be less than the size of the screen...
				resultGeometry.size.x = cppx::get_min_of( resultGeometry.size.x, screenSize.x );
				resultGeometry.size.y = cppx::get_min_of( resultGeometry.size.y, screenSize.y );

				// ... but at the same time bigger than the minimum allowed size (Win32-specific, really).
				resultGeometry.size.x = cppx::get_max_of( resultGeometry.size.x, minFrameSize.x );
				resultGeometry.size.y = cppx::get_max_of( resultGeometry.size.y, minFrameSize.y );
			}
		}

		if( ( framePos.x < 0 ) || ( framePos.y < 0 ) )
		{
			resultGeometry.position.x = static_cast<int32>( ( screenSize.x - resultGeometry.size.x ) / 2 );
			resultGeometry.position.y = static_cast<int32>( ( screenSize.y - resultGeometry.size.y ) / 2 );
		}
		else
		{
			const auto maxPosX = static_cast<int32>( screenSize.x - resultGeometry.size.x );
			const auto maxPosY = static_cast<int32>( screenSize.y - resultGeometry.size.y );
			resultGeometry.position.x = cppx::get_min_of( resultGeometry.position.x, maxPosX );
			resultGeometry.position.y = cppx::get_min_of( resultGeometry.position.y, maxPosY );
		}

		resultGeometry.position.x += screenOffset.x;
		resultGeometry.position.y += screenOffset.y;

		return resultGeometry;
	}


	DisplayDriver::DisplayDriver( DisplayManagerHandle pDisplayManager,
	                              EDisplayDriverType pDriverType )
	: SysObject( pDisplayManager->mSysContext )
	, mDisplayManager( std::move( pDisplayManager ) )
	, mDriverType( pDriverType )
	, _privateData( std::make_unique<DisplayDriverPrivateData>() )
	{}

	DisplayDriver::~DisplayDriver()
	{
		Reset();
	}

	DisplayDriver::operator bool() const
	{
		return HasValidConfiguration();
	}

	void DisplayDriver::SyncDisplayConfiguration()
	{
		if( !_privateData->adapterInstanceList.empty() )
		{
			_ResetDisplayConfiguration();
		}
		_InitializeDisplayConfiguration();
	}

	void DisplayDriver::Reset()
	{
		_ResetDisplayConfiguration();
	}

	EColorFormat DisplayDriver::QueryDefaultSystemColorFormat() const
	{
		return _NativeQueryDefaultSystemColorFormat();
	}

	DisplayAdapter * DisplayDriver::FindAdapter( DisplayAdapterPredicate pPredicate ) const
	{
		for( auto & adapterPtr : _privateData->adapterInstanceList )
		{
			if( pPredicate( *adapterPtr ) )
			{
				return adapterPtr.get();
			}
		}
		return nullptr;
	}

	DisplayAdapterList DisplayDriver::FindAdapters( DisplayAdapterPredicate pPredicate ) const
	{
		DisplayAdapterList adapterList;
		for( auto & adapterPtr : _privateData->adapterInstanceList )
		{
			if( pPredicate( *adapterPtr ) )
			{
				adapterList.push_back( adapterPtr.get() );
			}
		}
		return adapterList;
	}

	const DisplayAdapterList & DisplayDriver::GetAdapterList() const
	{
		return _privateData->adapterList;
	}

	const DisplayOutputList & DisplayDriver::GetOutputList( dsm_index_t pAdapterIndex ) const
	{
		const auto * adapter = GetAdapter( pAdapterIndex );
		return adapter->GetOutputList();
	}

	DisplayAdapter * DisplayDriver::GetAdapter( dsm_index_t pAdapterIndex ) const
	{
		if( pAdapterIndex == CX_DSM_INDEX_DEFAULT )
		{
			return _privateData->primaryAdapter;
		}
		else
		{
			return _privateData->adapterList.at( pAdapterIndex );
		}
	}

	DisplayAdapter * DisplayDriver::GetDefaultAdapter() const
	{
		return _privateData->primaryAdapter;
	}

	DisplayOutput * DisplayDriver::GetDefaultOutput( dsm_index_t pAdapterIndex ) const
	{
		const auto * adapter = GetAdapter( pAdapterIndex );
		return adapter->GetDefaultOutput();
	}

	DisplayOutput * DisplayDriver::GetOutput( dsm_output_id_t pOutputID ) const
	{
		const auto adapterIndex = DSMExtractOutputIDAdapterIndex( pOutputID );
		const auto outputIndex = DSMExtractOutputIDOutputIndex( pOutputID );

		auto * adapter = GetAdapter( adapterIndex );

		if( outputIndex == CX_DSM_INDEX_DEFAULT )
		{
			return adapter->GetDefaultOutput();
		}
		else
		{
			return adapter->GetOutput( outputIndex );
		}
	}

	bool DisplayDriver::HasActiveAdapters() const
	{
		return _privateData->activeAdaptersNum > 0;
	}

	bool DisplayDriver::HasAnyAdapters() const
	{
		return !_privateData->adapterInstanceList.empty();
	}

	bool DisplayDriver::HasValidConfiguration() const
	{
		return !_privateData->adapterInstanceList.empty() && (_privateData->combinedActiveOutputsNum > 0 );
	}

	std::string DisplayDriver::GenerateConfigurationDump( const std::string & pLinePrefix ) const
	{
		const auto adaptersNum = _privateData->adapterInstanceList.size();
		const auto displayDevicesNum = adaptersNum * 2;
		const auto averageDisplayModesNum = 16;
		const auto averageLineLength = 40 + pLinePrefix.length();
		const auto strReserveSize = ( displayDevicesNum * averageDisplayModesNum + displayDevicesNum ) * averageLineLength;

		std::string result;
		result.reserve( strReserveSize );

		if( !HasAnyAdapters() )
		{
			result.append( pLinePrefix );
			result.append( "<empty>" );
		}
		else
		{
			const auto & adapterList = GetAdapterList();
			for( const auto * adapter : adapterList )
			{
				const auto & adapterDesc = adapter->GetAdapterDesc();
				if( adapterDesc.adapterIndex > 0 )
				{
					result.append( 1, '\n' );
				}
				result.append( pLinePrefix );
				result.append( adapterDesc.ToString() );

				if( !adapter->HasAnyOutputs() )
				{
					result.append( 1, '\n' );
					result.append( pLinePrefix );
					result.append( 1, '\t' );
					result.append( "<empty>" );
				}
				else
				{
					const auto & outputList = adapter->GetOutputList();
					for( const auto * output : outputList )
					{
						const auto & outputDesc = output->GetOutputDesc();
						result.append( 1, '\n' );
						result.append( pLinePrefix );
						result.append( 1, '\t' );
						result.append( outputDesc.ToString() );

						const auto colorFormats = output->GetSupportedColorFormatList();
						for( auto colorFormat : colorFormats )
						{
							auto colorFormatStr = VisQueryColorFormatStr( colorFormat );
							result.append( 1, '\n' );
							result.append( pLinePrefix );
							result.append( 2, '\t' );
							result.append( colorFormatStr );

							const auto & videoModeList = output->GetVideoModeList( colorFormat );
							if( videoModeList.empty() )
							{
								result.append( 1, '\n' );
								result.append( pLinePrefix );
								result.append( 3, '\t' );
								result.append( "<empty>" );
							}
							else
							{
								for( const auto * videoMode : videoModeList )
								{
									const auto & videoModeDesc = videoMode->GetModeDesc();
									result.append( 1, '\n' );
									result.append( pLinePrefix );
									result.append( 3, '\t' );
									result.append( videoModeDesc.ToString() );
								}
							}
						}

					}
				}
			}
		}

		return result;
	}

	DisplayAdapterDesc & DisplayDriver::GetAdapterDescInternal( DisplayAdapter & pAdapter )
	{
		return pAdapter.GetAdapterDescInternal();
	}

	DisplayOutputDesc & DisplayDriver::GetOutputDescInternal( DisplayOutput & pOutput )
	{
		return pOutput.GetOutputDescInternal();
	}

	DisplayVideoModeDesc & DisplayDriver::GetVideoModeDescInternal( DisplayVideoMode & pVideoMode )
	{
		return pVideoMode.GetModeDescInternal();
	}

	void DisplayDriver::_InitializeDisplayConfiguration()
	{
		// Enum all display devices (adapters and outputs). This process is combined,
		// because some drivers (like GDI, for example) may perform better if this is
		// done atomically. After this is done, lists of adapters and their outputs
		// are populated and all configuration/properties are valid and available.
		_enumDisplayDevices();

		// Enumerate video modes.
		_enumVideoModes();

		// "Post-processing" part. This function analyzes all adapters, validates
		// their properties and fixes up missing things (primary outputs, missing
		// flags etc.). It also calls a validation method for each output of an
		// adapter which internally does the same for all video modes of an output.
		_validateAdaptersConfiguration();
	}

	void DisplayDriver::_ResetDisplayConfiguration()
	{
		_privateData->adapterInstanceList.clear();
		_privateData->adapterList.clear();
		_privateData->primaryAdapter = nullptr;
		_privateData->activeAdaptersNum = 0u;
		_privateData->combinedActiveOutputsNum = 0u;
	}

	void DisplayDriver::_enumDisplayDevices()
	{
		_NativeEnumDisplayDevices();
	}

	void DisplayDriver::_enumVideoModes()
	{
		for( const auto & adapterPtr : _privateData->adapterInstanceList )
		{
			const auto & adapterOutputList = adapterPtr->GetOutputList();

			for( auto * output : adapterOutputList )
			{
				for( const auto colorFormat : cvColorFormatArray )
				{
					_NativeEnumVideoModes( *output, colorFormat );
				}
			}
		}
	}

	void DisplayDriver::_registerAdapter( DisplayAdapterHandle pAdapter )
	{
		const auto adapterIndex = _privateData->adapterInstanceList.size();

		auto & adapterDesc = pAdapter->GetAdapterDescInternal();
		adapterDesc.driverType = mDriverType;
		adapterDesc.adapterIndex = static_cast<dsm_index_t>( adapterIndex );

		_privateData->adapterInstanceList.push_back( std::move( pAdapter ) );

		// Adapters are not added to the helper list at this point.
		// This is done as a post-process step later in DisplayDriver::_enumAdapters().
		// Assertion added to prevent problems in case of refactoring.
		Ic3DebugAssert( _privateData->adapterList.empty() );
	}

	void DisplayDriver::_RegisterOutput( DisplayAdapter & pAdapter, DisplayOutputHandle pOutput )
	{
		Ic3DebugAssert( _privateData->adapterList.empty() );
		pAdapter.RegisterOutput( std::move( pOutput ) );
	}

	void DisplayDriver::_RegisterVideoMode( DisplayOutput & pOutput, EColorFormat pColorFormat, DisplayVideoModeHandle pVideoMode )
	{
		Ic3DebugAssert( _privateData->adapterList.empty() );
		pOutput.RegisterVideoMode( pColorFormat, std::move( pVideoMode ) );
	}

	void DisplayDriver::_validateAdaptersConfiguration()
	{
		if( !_privateData->adapterInstanceList.empty() )
		{
			// Reserve space for the list of pointers/handles for adapters.
			_privateData->adapterList.reserve( _privateData->adapterInstanceList.size() );

			for( auto & adapterPtr : _privateData->adapterInstanceList )
			{
				// Update the non-driver-specific part of the adapter info
				auto & adapterDesc = adapterPtr->GetAdapterDescInternal();

				if( adapterDesc.vendorID == EDisplayAdapterVendorID::Unknown )
				{
					// Driver can set the vendor ID internally, but in case it is missing,
					// this function tries to resolve the ID by looking at the adapter desc.
					adapterDesc.vendorID = DSMResolveAdapterVendorID( adapterDesc.name );
				}
				if( adapterPtr->IsPrimaryAdapter() && !_privateData->primaryAdapter )
				{
					// Driver can also explicitly set the primary system adapter.
					// If it Has not been set, we use the first adapter with proper flag set.
					_privateData->primaryAdapter = adapterPtr.get();
				}
				if( adapterPtr->IsActiveAdapter() )
				{
					_privateData->activeAdaptersNum += 1;
				}

				const auto adapterActiveOutputsNum = adapterPtr->ValidateOutputsConfiguration();

				_privateData->combinedActiveOutputsNum += adapterActiveOutputsNum;

				_privateData->adapterList.push_back( adapterPtr.get() );
			}

			// Validate if the default system adapter Has been properly set.
			if( _privateData->primaryAdapter )
			{
				// Default./primary adapter will usually have the proper bit set (all drivers should do that).
				// In case the adapter Has been set, but this bit is missing, emit a warning. It may be an
				// intentional choice, but also an error or missing driver-specific init code.

				const auto & adapterDesc = _privateData->primaryAdapter->GetAdapterDescInternal();
				if( !adapterDesc.flags.is_set( eDisplayAdapterFlagPrimaryBit ) )
				{
					Ic3DebugOutput(
						"Primary/Default adapter selected by the driver does not have "\
						"E_DISPLAY_ADAPTER_FLAG_PRIMARY_BIT set. Is that intentional?" );
				}
			}
			else
			{
				// If there is no default adapter set, it means it Has not been done by the driver AND
				// there Has not been any adapter marked as PRIMARY. In this case, just select the first
				// one, update its state and set as the default one.

				const auto & firstAdapter = _privateData->adapterInstanceList.front();
				auto & firstAdapterDesc = firstAdapter->GetAdapterDescInternal();
				firstAdapterDesc.flags.set( eDisplayAdapterFlagPrimaryBit );
				_privateData->primaryAdapter = firstAdapter.get();
			}
		}
	}

} // namespace Ic3::System
