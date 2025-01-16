
#include "Internal/DisplaySystemPrivate.h"

namespace Ic3::System
{

	DisplayAdapter::DisplayAdapter( DisplayDriver & pDriver )
	: SysObject( pDriver.mSysContext )
	, mDriverType( pDriver.mDriverType )
	, mDisplayDriver( &pDriver )
	, _privateData( std::make_unique<DisplayAdapterPrivateData>() )
	{}

	DisplayAdapter::~DisplayAdapter() noexcept = default;

	DisplayOutput * DisplayAdapter::FindOutput( DisplayOutputPredicate pPredicate ) const
	{
		for( auto & outputPtr : _privateData->outputInstanceList )
		{
			if( pPredicate( *outputPtr ) )
			{
				return outputPtr.get();
			}
		}
		return nullptr;
	}

	DisplayOutputList DisplayAdapter::FindOutputs( DisplayOutputPredicate pPredicate ) const
	{
		DisplayOutputList outputList;
		for( auto & outputPtr : _privateData->outputInstanceList )
		{
			if( pPredicate( *outputPtr ) )
			{
				outputList.push_back( outputPtr.get() );
			}
		}
		return outputList;
	}

	DisplayOutput * DisplayAdapter::GetOutput( dsm_index_t pOutputIndex ) const
	{
		if( pOutputIndex == CX_DSM_INDEX_DEFAULT )
		{
			return _privateData->primaryOutput;
		}
		else
		{
			return _privateData->outputList.at( pOutputIndex );
		}
	}

	DisplayOutput * DisplayAdapter::GetDefaultOutput() const
	{
		return _privateData->primaryOutput;
	}

	const DisplayAdapterDesc & DisplayAdapter::GetAdapterDesc() const
	{
		return _privateData->adapterDesc;
	}

	const DisplayOutputList & DisplayAdapter::GetOutputList() const
	{
		return _privateData->outputList;
	}

	bool DisplayAdapter::IsActiveAdapter() const
	{
		return _privateData->adapterDesc.flags.is_set( eDisplayAdapterFlagActiveBit );
	}

	bool DisplayAdapter::IsPrimaryAdapter() const
	{
		return _privateData->adapterDesc.flags.is_set( eDisplayAdapterFlagPrimaryBit );
	}

	bool DisplayAdapter::HasActiveOutputs() const
	{
		return _privateData->activeOutputsNum > 0;
	}

	bool DisplayAdapter::HasAnyOutputs() const
	{
		return !_privateData->outputInstanceList.empty();
	}

	void DisplayAdapter::RegisterOutput( DisplayOutputHandle pOutput )
	{
		const auto outputIndex = _privateData->outputInstanceList.size();

		DisplayOutputIDGen outputIDGen;
		outputIDGen.uAdapterIndex = _privateData->adapterDesc.adapterIndex;
		outputIDGen.uOutputIndex = static_cast<dsm_index_t>( outputIndex );

		auto & outputDesc = pOutput->GetOutputDescInternal();
		outputDesc.driverType = mDriverType;
		outputDesc.outputIndex = outputIDGen.uOutputIndex;
		outputDesc.outputID = outputIDGen.outputID;

		_privateData->outputInstanceList.push_back( std::move( pOutput ) );

		// Outputs are not added to the helper list at this point.
		// This is done as a post-process step later in DisplayDriver::_enumOutputs().
		// Assertion added to prevent problems in case of refactoring.
		ic3DebugAssert( _privateData->outputList.empty() );
	}

	uint32 DisplayAdapter::ValidateOutputsConfiguration()
	{
		if( !_privateData->outputInstanceList.empty() )
		{
			// Reserve space for the list of pointers/handles for outputs.
			_privateData->outputList.reserve( _privateData->outputInstanceList.size() );

			for( auto & outputPtr : _privateData->outputInstanceList )
			{
				if( outputPtr->IsPrimaryOutput() && !_privateData->primaryOutput )
				{
					// Similar to the default/primary system adapter, we select default
					// output of an adapter if the driver has not set it during enumeration.
					_privateData->primaryOutput = outputPtr.get();
				}

				if( outputPtr->IsActiveOutput() )
				{
					_privateData->activeOutputsNum += 1;
				}

				for( auto colorFormat : cvColorFormatArray )
				{
					outputPtr->ValidateVideoModesConfiguration( colorFormat );
				}

				_privateData->outputList.push_back( outputPtr.get() );
			}

			// Validate if the default output for this adapter has been properly set.
			if( _privateData->primaryOutput )
			{
				auto & outputDesc = _privateData->primaryOutput->GetOutputDescInternal();
				if( !outputDesc.flags.is_set( eDisplayOutputFlagPrimaryBit ) )
				{
					ic3DebugOutputFmt(
						"Primary/Default output of [%s] selected by the driver does not have "\
						"E_DISPLAY_ADAPTER_FLAG_PRIMARY_BIT set. Is that intentional?",
						_privateData->adapterDesc.name.c_str() );
				}
			}
			else
			{
				auto & firstOutput = _privateData->outputInstanceList.front();
				auto & firstOutputDesc = firstOutput->GetOutputDescInternal();
				firstOutputDesc.flags.set( eDisplayOutputFlagPrimaryBit );
				_privateData->primaryOutput = firstOutput.get();
			}
		}

		return _privateData->activeOutputsNum;
	}

	DisplayAdapterDesc & DisplayAdapter::GetAdapterDescInternal()
	{
		return _privateData->adapterDesc;
	}


	DisplayOutput::DisplayOutput( DisplayAdapter & pAdapter )
	: SysObject( pAdapter.mSysContext )
	, mDriverType( pAdapter.mDriverType )
	, mDisplayDriver( pAdapter.mDisplayDriver )
	, mParentAdapter( &pAdapter )
	, _privateData( std::make_unique<DisplayOutputPrivateData>() )
	{}

	DisplayOutput::~DisplayOutput() noexcept = default;

	cppx::array_view<const EColorFormat> DisplayOutput::GetSupportedColorFormatList() const
	{
		return cppx::bind_array_view( _privateData->supportedColorFormatList.data(),
		              _privateData->supportedColorFormatList.size() );
	}

	bool DisplayOutput::CheckVideoSettingsSupport( const DisplayVideoSettings & pVideoSettings, EColorFormat pColorFormat ) const
	{
		const auto & colorFormatData = _privateData->colorFormatMap.at( pColorFormat );
		for( const auto & videoMode : colorFormatData.videoModeInstanceList )
		{
			const auto & videoModeDesc = videoMode->GetModeDesc();
			if( videoModeDesc.settings.matches( pVideoSettings ) )
			{
				return true;
			}
		}
		return false;
	}

	DisplayVideoMode * DisplayOutput::FindVideoMode( EColorFormat pColorFormat, DisplayVideoModePredicate pPredicate ) const
	{
		for( auto & colorFormatData : _privateData->colorFormatMap )
		{
			if( ( colorFormatData.first == pColorFormat ) || ( pColorFormat == EColorFormat::Unknown ) )
			{
				for( auto & videoModePtr : colorFormatData.second.videoModeInstanceList )
				{
					if( pPredicate( *videoModePtr ) )
					{
						return videoModePtr.get();
					}
				}
			}
		}
		return nullptr;
	}

	DisplayVideoModeList DisplayOutput::FindVideoModes( EColorFormat pColorFormat, DisplayVideoModePredicate pPredicate ) const
	{
		DisplayVideoModeList videoModeList;
		for( auto & colorFormatData : _privateData->colorFormatMap )
		{
			if( ( colorFormatData.first == pColorFormat ) || ( pColorFormat == EColorFormat::Unknown ) )
			{
				for( auto & videoModePtr : colorFormatData.second.videoModeInstanceList )
				{
					if( pPredicate( *videoModePtr ) )
					{
						videoModeList.push_back( videoModePtr.get() );
					}
				}
			}
		}
		return videoModeList;
	}

	const DisplayOutputDesc & DisplayOutput::GetOutputDesc() const
	{
		return _privateData->outputDesc;
	}

	const DisplayVideoModeList & DisplayOutput::GetVideoModeList() const
	{
		auto colorFormat = mDisplayDriver->QueryDefaultSystemColorFormat();
		return GetVideoModeList( colorFormat );
	}

	const DisplayVideoModeList & DisplayOutput::GetVideoModeList( EColorFormat pColorFormat ) const
	{
		const auto & colorFormatData = _privateData->colorFormatMap.at( pColorFormat );
		return colorFormatData.videoModeList;
	}

	bool DisplayOutput::IsActiveOutput() const
	{
		return _privateData->outputDesc.flags.is_set( eDisplayOutputFlagActiveBit );
	}

	bool DisplayOutput::IsPrimaryOutput() const
	{
		return _privateData->outputDesc.flags.is_set( eDisplayOutputFlagPrimaryBit );
	}

	bool DisplayOutput::IsColorFormatSupported( EColorFormat pColorFormat ) const
	{
		const auto & colorFormatData = _privateData->colorFormatMap.at( pColorFormat );
		return !colorFormatData.videoModeInstanceList.empty();
	}

	void DisplayOutput::RegisterVideoMode( EColorFormat pColorFormat, DisplayVideoModeHandle pVideoMode )
	{
		auto & colorFormatData = _privateData->colorFormatMap[pColorFormat];

		if( colorFormatData.colorFormat == EColorFormat::Unknown )
		{
			colorFormatData.colorFormat = pColorFormat;
			_privateData->supportedColorFormatList.push_back( pColorFormat );
		}

		const auto videoModeIndex = colorFormatData.videoModeInstanceList.size();

		DisplayVideoModeIDGen videoModeIDGen;
		videoModeIDGen.uOutputID = _privateData->outputDesc.outputID;
		videoModeIDGen.uColorFormatIndex = static_cast<dsm_index_t>( colorFormatData.colorFormat );
		videoModeIDGen.uModeIndex = static_cast<dsm_index_t>( videoModeIndex );

		auto & videoModeDesc = pVideoMode->GetModeDescInternal();
		videoModeDesc.driverType = mDriverType;
		videoModeDesc.videoModeIndex = videoModeIDGen.uModeIndex;
		videoModeDesc.videoModeID = videoModeIDGen.mModeID;
		videoModeDesc.colorFormat = pColorFormat;

		colorFormatData.videoModeInstanceList.push_back( pVideoMode );

		// Video modes are not added to the helper list at this point.
		// This is done as a post-process step later in DisplayDriver::_enumVideoModes().
		// Assertion added to prevent problems in case of refactoring.
		ic3DebugAssert( colorFormatData.videoModeList.empty() );
	}

	uint32 DisplayOutput::ValidateVideoModesConfiguration( EColorFormat pColorFormat )
	{
		auto & colorFormatData = _privateData->colorFormatMap.at( pColorFormat );
		auto videoModesNum = colorFormatData.videoModeInstanceList.size();

		if( videoModesNum > 0 )
		{
			colorFormatData.videoModeList.reserve( videoModesNum );

			for( auto & videoModePtr : colorFormatData.videoModeInstanceList )
			{
				colorFormatData.videoModeList.push_back( videoModePtr.get() );
			}
		}

		return static_cast<uint32>( videoModesNum );
	}

	DisplayOutputDesc & DisplayOutput::GetOutputDescInternal()
	{
		return _privateData->outputDesc;
	}


	DisplayVideoMode::DisplayVideoMode( DisplayOutput & pOutput )
	: SysObject( pOutput.mSysContext )
	, mDriverType( pOutput.mDriverType )
	, mDisplayDriver( pOutput.mDisplayDriver )
	, mParentOutput( &pOutput )
	, _privateData( std::make_unique<DisplayVideoModePrivateData>() )
	{}

	DisplayVideoMode::~DisplayVideoMode() noexcept = default;

	const DisplayVideoModeDesc & DisplayVideoMode::GetModeDesc() const
	{
		return _privateData->modeDesc;
	}

	DisplayVideoModeDesc & DisplayVideoMode::GetModeDescInternal()
	{
		return _privateData->modeDesc;
	}

} // namespace Ic3::System
