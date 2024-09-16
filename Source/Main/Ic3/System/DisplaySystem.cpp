
#include "Internal/DisplaySystemPrivate.h"

#if( IC3_SYSTEM_DSM_DRIVER_TYPE_SUPPORT_DXGI )
#  include "Internal/Platform/Shared/DXGI/DXGIDisplaySystem.h"
#endif

namespace Ic3::System
{

	DisplayManager::DisplayManager( SysContextHandle pSysContext )
	: SysObject( std::move( pSysContext ) )
	, _privateData( std::make_unique<DisplayManagerPrivateData>() )
	{
		_privateData->mDriverFactoryMap[EDisplayDriverType::Generic] = [this]() {
			return createDisplayDriver();
		};
	#if( IC3_SYSTEM_DSM_DRIVER_TYPE_SUPPORT_DXGI )
		_privateData->driverFactoryMap[EDisplayDriverType::DXGI] = [this]() {
			return createSysObject<DisplayDriverDXGI>( getHandle<DisplayManager>() );
		};
	#endif
	#if( IC3_SYSTEM_DSM_DRIVER_TYPE_SUPPORT_SDL )
		_privateData->driverFactoryMap[EDisplayDriverType::SDL] = [this]() {
			return createSysObject<DisplayDriverSDL>( getHandle<DisplayManager>() );
		};
	#endif
	}

	DisplayManager::~DisplayManager() noexcept = default;

	DisplayDriverHandle DisplayManager::createDisplayDriver( EDisplayDriverType pDriverID )
	{
		pDriverID = resolveDisplayDriverID( pDriverID );
		if( pDriverID == EDisplayDriverType::Unknown )
		{
			return nullptr;
		}

		auto & factoryCallback = _privateData->mDriverFactoryMap.at( pDriverID );
		auto displayDriver = factoryCallback();

		return displayDriver;
	}

	DisplayDriverHandle DisplayManager::createDisplayDriver()
	{
		return _nativeCreateDisplayDriver();
	}

	bool DisplayManager::checkDriverSupport( EDisplayDriverType pDriverID ) const
	{
		// The default driver is always supported. If no driver is available on
		// a given system, DisplayManager will fail to create in the first place,
		if( pDriverID == EDisplayDriverType::Default )
		{
			return true;
		}

		// Resolve the driver ID. This will return EDisplayDriverType::Unknown
		// if the specified driver is not supported on the current system.
		auto resolvedDriverID = resolveDisplayDriverID( pDriverID );

		return resolvedDriverID != EDisplayDriverType::Unknown;
	}

	EDisplayDriverType DisplayManager::resolveDisplayDriverID( EDisplayDriverType pDriverID ) const
	{
		EDisplayDriverType resolvedDriverID = EDisplayDriverType::Unknown;

		if( pDriverID == EDisplayDriverType::Generic )
		{
			resolvedDriverID = EDisplayDriverType::Generic;
		}
		else if( pDriverID == EDisplayDriverType::Default )
		{
		#if( IC3_PCL_TARGET_OS & IC3_PCL_TARGET_FLAG_OS_MSE )
			resolvedDriverID = EDisplayDriverType::DXGI;
		#else
			resolvedDriverID = EDisplayDriverType::Generic;
		#endif
		}
	#if( IC3_PCL_TARGET_OS & IC3_PCL_TARGET_FLAG_OS_MSE )
		else if( pDriverID == EDisplayDriverType::DXGI )
		{
			resolvedDriverID = EDisplayDriverType::DXGI;
		}
	#endif

		return resolvedDriverID;
	}

	DisplayOffset DisplayManager::queryDefaultDisplayOffset() const
	{
		DisplayOffset result;
		_nativeQueryDefaultDisplayOffset( result );
		return result;
	}

	DisplaySize DisplayManager::queryDefaultDisplaySize() const
	{
		DisplaySize result;
		_nativeQueryDefaultDisplaySize( result );
		return result;
	}

	DisplaySize DisplayManager::queryMinWindowSize() const
	{
		DisplaySize result;
		_nativeQueryMinWindowSize( result );
		return result;
	}

	bool DisplayManager::checkFrameGeometry( const FrameGeometry & pFrameGeometry ) const
	{
		const auto & framePos = pFrameGeometry.mPosition;
		const auto & frameSize = pFrameGeometry.mSize;

		auto screenSize = queryDefaultDisplaySize();
		auto minFrameSize = queryMinWindowSize();

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

			auto maxPosX = static_cast<int32>( screenSize.x - frameSize.x );
			auto maxPosY = static_cast<int32>( screenSize.y - frameSize.y );

			if( ( framePos.x > maxPosX ) || ( framePos.y > maxPosY ) )
			{
				return false;
			}
		}

		return true;
	}

	FrameGeometry DisplayManager::validateFrameGeometry( const FrameGeometry & pFrameGeometry ) const
	{
		auto resultGeometry = pFrameGeometry;

		const auto & framePos = pFrameGeometry.mPosition;
		const auto & frameSize = pFrameGeometry.mSize;

		auto screenOffset = queryDefaultDisplayOffset();
		auto screenSize = queryDefaultDisplaySize();
		auto minFrameSize = queryMinWindowSize();

		if( frameSize == cxFrameSizeMax )
		{
			// Frame is defined as maximum allowed for the current display/screen. Make it that big.
			resultGeometry.mSize.x = screenSize.x;
			resultGeometry.mSize.y = screenSize.y;
		}
		else
		{
			if( ( frameSize.x == 0 ) || ( frameSize.y == 0 ) )
			{
				// Any dimension set to 0 means "use default size". By default,
				// we just use the ratio of the screen and 70% of its dimensions.
				resultGeometry.mSize.x = static_cast<uint32>( screenSize.x * 0.7 );
				resultGeometry.mSize.y = static_cast<uint32>( screenSize.y * 0.7 );
			}
			else
			{
				// Size of the window must be less than the size of the screen...
				resultGeometry.mSize.x = Cppx::getMinOf( resultGeometry.mSize.x, screenSize.x );
				resultGeometry.mSize.y = Cppx::getMinOf( resultGeometry.mSize.y, screenSize.y );

				// ... but at the same time bigger than the minimum allowed size (Win32-specific, really).
				resultGeometry.mSize.x = Cppx::getMaxOf( resultGeometry.mSize.x, minFrameSize.x );
				resultGeometry.mSize.y = Cppx::getMaxOf( resultGeometry.mSize.y, minFrameSize.y );
			}
		}

		if( ( framePos.x < 0 ) || ( framePos.y < 0 ) )
		{
			resultGeometry.mPosition.x = static_cast<int32>( ( screenSize.x - resultGeometry.mSize.x ) / 2 );
			resultGeometry.mPosition.y = static_cast<int32>( ( screenSize.y - resultGeometry.mSize.y ) / 2 );
		}
		else
		{
			auto maxPosX = static_cast<int32>( screenSize.x - resultGeometry.mSize.x );
			auto maxPosY = static_cast<int32>( screenSize.y - resultGeometry.mSize.y );
			resultGeometry.mPosition.x = Cppx::getMinOf( resultGeometry.mPosition.x, maxPosX );
			resultGeometry.mPosition.y = Cppx::getMinOf( resultGeometry.mPosition.y, maxPosY );
		}

		resultGeometry.mPosition.x += screenOffset.x;
		resultGeometry.mPosition.y += screenOffset.y;

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
		reset();
	}

	DisplayDriver::operator bool() const
	{
		return hasValidConfiguration();
	}

	void DisplayDriver::syncDisplayConfiguration()
	{
		if( !_privateData->mAdapterInstanceList.empty() )
		{
			_resetDisplayConfiguration();
		}
		_initializeDisplayConfiguration();
	}

	void DisplayDriver::reset()
	{
		_resetDisplayConfiguration();
	}

	EColorFormat DisplayDriver::queryDefaultSystemColorFormat() const
	{
		return _nativeQueryDefaultSystemColorFormat();
	}

	DisplayAdapter * DisplayDriver::findAdapter( DisplayAdapterPredicate pPredicate ) const
	{
		for( auto & adapterPtr : _privateData->mAdapterInstanceList )
		{
			if( pPredicate( *adapterPtr ) )
			{
				return adapterPtr.get();
			}
		}
		return nullptr;
	}

	DisplayAdapterList DisplayDriver::findAdapters( DisplayAdapterPredicate pPredicate ) const
	{
		DisplayAdapterList adapterList;
		for( auto & adapterPtr : _privateData->mAdapterInstanceList )
		{
			if( pPredicate( *adapterPtr ) )
			{
				adapterList.push_back( adapterPtr.get() );
			}
		}
		return adapterList;
	}

	const DisplayAdapterList & DisplayDriver::getAdapterList() const
	{
		return _privateData->mAdapterList;
	}

	const DisplayOutputList & DisplayDriver::getOutputList( dsm_index_t pAdapterIndex ) const
	{
		auto * adapter = getAdapter( pAdapterIndex );
		return adapter->getOutputList();
	}

	DisplayAdapter * DisplayDriver::getAdapter( dsm_index_t pAdapterIndex ) const
	{
		if( pAdapterIndex == CX_DSM_INDEX_DEFAULT )
		{
			return _privateData->mPrimaryAdapter;
		}
		else
		{
			return _privateData->mAdapterList.at( pAdapterIndex );
		}
	}

	DisplayAdapter * DisplayDriver::getDefaultAdapter() const
	{
		return _privateData->mPrimaryAdapter;
	}

	DisplayOutput * DisplayDriver::getDefaultOutput( dsm_index_t pAdapterIndex ) const
	{
		auto * adapter = getAdapter( pAdapterIndex );
		return adapter->getDefaultOutput();
	}

	DisplayOutput * DisplayDriver::getOutput( dsm_output_id_t pOutputID ) const
	{
		auto adapterIndex = dsmExtractOutputIDAdapterIndex( pOutputID );
		auto outputIndex = dsmExtractOutputIDOutputIndex( pOutputID );

		auto * adapter = getAdapter( adapterIndex );

		if( outputIndex == CX_DSM_INDEX_DEFAULT )
		{
			return adapter->getDefaultOutput();
		}
		else
		{
			return adapter->getOutput( outputIndex );
		}
	}

	bool DisplayDriver::hasActiveAdapters() const
	{
		return _privateData->mActiveAdaptersNum > 0;
	}

	bool DisplayDriver::hasAnyAdapters() const
	{
		return !_privateData->mAdapterInstanceList.empty();
	}

	bool DisplayDriver::hasValidConfiguration() const
	{
		return !_privateData->mAdapterInstanceList.empty() && (_privateData->mCombinedActiveOutputsNum > 0 );
	}

	std::string DisplayDriver::generateConfigurationDump( const std::string & pLinePrefix ) const
	{
		const auto adaptersNum = _privateData->mAdapterInstanceList.size();
		const auto displayDevicesNum = adaptersNum * 2;
		const auto averageDisplayModesNum = 16;
		const auto averageLineLength = 40 + pLinePrefix.length();
		const auto strReserveSize = ( displayDevicesNum * averageDisplayModesNum + displayDevicesNum ) * averageLineLength;

		std::string result;
		result.reserve( strReserveSize );

		if( !hasAnyAdapters() )
		{
			result.append( pLinePrefix );
			result.append( "<empty>" );
		}
		else
		{
			const auto & adapterList = getAdapterList();
			for( const auto * adapter : adapterList )
			{
				const auto & adapterDesc = adapter->getAdapterDesc();
				if( adapterDesc.mAdapterIndex > 0 )
				{
					result.append( 1, '\n' );
				}
				result.append( pLinePrefix );
				result.append( adapterDesc.toString() );

				if( !adapter->hasAnyOutputs() )
				{
					result.append( 1, '\n' );
					result.append( pLinePrefix );
					result.append( 1, '\t' );
					result.append( "<empty>" );
				}
				else
				{
					const auto & outputList = adapter->getOutputList();
					for( const auto * output : outputList )
					{
						const auto & outputDesc = output->getOutputDesc();
						result.append( 1, '\n' );
						result.append( pLinePrefix );
						result.append( 1, '\t' );
						result.append( outputDesc.toString() );

						auto colorFormats = output->getSupportedColorFormatList();
						for( auto colorFormat : colorFormats )
						{
							auto colorFormatStr = vsxQueryColorFormatStr( colorFormat );
							result.append( 1, '\n' );
							result.append( pLinePrefix );
							result.append( 2, '\t' );
							result.append( colorFormatStr );

							const auto & videoModeList = output->getVideoModeList( colorFormat );
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
									const auto & videoModeDesc = videoMode->getModeDesc();
									result.append( 1, '\n' );
									result.append( pLinePrefix );
									result.append( 3, '\t' );
									result.append( videoModeDesc.toString() );
								}
							}
						}

					}
				}
			}
		}

		return result;
	}

	DisplayAdapterDesc & DisplayDriver::getAdapterDescInternal( DisplayAdapter & pAdapter )
	{
		return pAdapter.getAdapterDescInternal();
	}

	DisplayOutputDesc & DisplayDriver::getOutputDescInternal( DisplayOutput & pOutput )
	{
		return pOutput.getOutputDescInternal();
	}

	DisplayVideoModeDesc & DisplayDriver::getVideoModeDescInternal( DisplayVideoMode & pVideoMode )
	{
		return pVideoMode.getModeDescInternal();
	}

	void DisplayDriver::_initializeDisplayConfiguration()
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

	void DisplayDriver::_resetDisplayConfiguration()
	{
		_privateData->mAdapterInstanceList.clear();
		_privateData->mAdapterList.clear();
		_privateData->mPrimaryAdapter = nullptr;
		_privateData->mActiveAdaptersNum = 0u;
		_privateData->mCombinedActiveOutputsNum = 0u;
	}

	void DisplayDriver::_enumDisplayDevices()
	{
		_nativeEnumDisplayDevices();
	}

	void DisplayDriver::_enumVideoModes()
	{
		for( auto & adapterPtr : _privateData->mAdapterInstanceList )
		{
			const auto & adapterOutputList = adapterPtr->getOutputList();

			for( auto * output : adapterOutputList )
			{
				for( auto colorFormat : cvColorFormatArray )
				{
					_nativeEnumVideoModes( *output, colorFormat );
				}
			}
		}
	}

	void DisplayDriver::_registerAdapter( DisplayAdapterHandle pAdapter )
	{
		const auto adapterIndex = _privateData->mAdapterInstanceList.size();

		auto & adapterDesc = pAdapter->getAdapterDescInternal();
		adapterDesc.mDriverType = mDriverType;
		adapterDesc.mAdapterIndex = static_cast<dsm_index_t>( adapterIndex );

		_privateData->mAdapterInstanceList.push_back( std::move( pAdapter ) );

		// Adapters are not added to the helper list at this point.
		// This is done as a post-process step later in DisplayDriver::_enumAdapters().
		// Assertion added to prevent problems in case of refactoring.
		ic3DebugAssert( _privateData->mAdapterList.empty() );
	}

	void DisplayDriver::_registerOutput( DisplayAdapter & pAdapter, DisplayOutputHandle pOutput )
	{
		ic3DebugAssert( _privateData->mAdapterList.empty() );
		pAdapter.registerOutput( std::move( pOutput ) );
	}

	void DisplayDriver::_registerVideoMode( DisplayOutput & pOutput, EColorFormat pColorFormat, DisplayVideoModeHandle pVideoMode )
	{
		ic3DebugAssert( _privateData->mAdapterList.empty() );
		pOutput.registerVideoMode( pColorFormat, std::move( pVideoMode ) );
	}

	void DisplayDriver::_validateAdaptersConfiguration()
	{
		if( !_privateData->mAdapterInstanceList.empty() )
		{
			// Reserve space for the list of pointers/handles for adapters.
			_privateData->mAdapterList.reserve( _privateData->mAdapterInstanceList.size() );

			for( auto & adapterPtr : _privateData->mAdapterInstanceList )
			{
				// Update the non-driver-specific part of the adapter info
				auto & adapterDesc = adapterPtr->getAdapterDescInternal();

				if( adapterDesc.mVendorID == EDisplayAdapterVendorID::Unknown )
				{
					// Driver can set the vendor ID internally, but in case it is missing,
					// this function tries to resolve the ID by looking at the adapter desc.
					adapterDesc.mVendorID = dsmResolveAdapterVendorID( adapterDesc.mName );
				}
				if( adapterPtr->isPrimaryAdapter() && !_privateData->mPrimaryAdapter )
				{
					// Driver can also explicitly set the primary system adapter.
					// If it has not been set, we use the first adapter with proper flag set.
					_privateData->mPrimaryAdapter = adapterPtr.get();
				}
				if( adapterPtr->isActiveAdapter() )
				{
					_privateData->mActiveAdaptersNum += 1;
				}

				auto adapterActiveOutputsNum = adapterPtr->validateOutputsConfiguration();

				_privateData->mCombinedActiveOutputsNum += adapterActiveOutputsNum;

				_privateData->mAdapterList.push_back( adapterPtr.get() );
			}

			// Validate if the default system adapter has been properly set.
			if( _privateData->mPrimaryAdapter )
			{
				// Default./primary adapter will usually have the proper bit set (all drivers should do that).
				// In case the adapter has been set, but this bit is missing, emit a warning. It may be an
				// intentional choice, but also an error or missing driver-specific init code.

				auto & adapterDesc = _privateData->mPrimaryAdapter->getAdapterDescInternal();
				if( !adapterDesc.mFlags.isSet( eDisplayAdapterFlagPrimaryBit ) )
				{
					ic3DebugOutput(
						"Primary/Default adapter selected by the driver does not have "\
						"E_DISPLAY_ADAPTER_FLAG_PRIMARYBit set. Is that intentional?" );
				}
			}
			else
			{
				// If there is no default adapter set, it means it has not been done by the driver AND
				// there has not been any adapter marked as PRIMARY. In this case, just select the first
				// one, update its state and set as the default one.

				auto & firstAdapter = _privateData->mAdapterInstanceList.front();
				auto & firstAdapterDesc = firstAdapter->getAdapterDescInternal();
				firstAdapterDesc.mFlags.set( eDisplayAdapterFlagPrimaryBit );
				_privateData->mPrimaryAdapter = firstAdapter.get();
			}
		}
	}

} // namespace Ic3::System
