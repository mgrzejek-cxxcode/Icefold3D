
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

	DisplayOutput * DisplayAdapter::findOutput( DisplayOutputPredicate pPredicate ) const
	{
		for( auto & outputPtr : _privateData->mOutputInstanceList )
		{
			if( pPredicate( *outputPtr ) )
			{
				return outputPtr.get();
			}
		}
		return nullptr;
	}

	DisplayOutputList DisplayAdapter::findOutputs( DisplayOutputPredicate pPredicate ) const
	{
		DisplayOutputList outputList;
		for( auto & outputPtr : _privateData->mOutputInstanceList )
		{
			if( pPredicate( *outputPtr ) )
			{
				outputList.push_back( outputPtr.get() );
			}
		}
		return outputList;
	}

	DisplayOutput * DisplayAdapter::getOutput( dsm_index_t pOutputIndex ) const
	{
		if( pOutputIndex == CX_DSM_INDEX_DEFAULT )
		{
			return _privateData->mPrimaryOutput;
		}
		else
		{
			return _privateData->mOutputList.at( pOutputIndex );
		}
	}

	DisplayOutput * DisplayAdapter::getDefaultOutput() const
	{
		return _privateData->mPrimaryOutput;
	}

	const DisplayAdapterDesc & DisplayAdapter::getAdapterDesc() const
	{
		return _privateData->mAdapterDesc;
	}

	const DisplayOutputList & DisplayAdapter::getOutputList() const
	{
		return _privateData->mOutputList;
	}

	bool DisplayAdapter::isActiveAdapter() const
	{
		return _privateData->mAdapterDesc.mFlags.isSet( eDisplayAdapterFlagActiveBit );
	}

	bool DisplayAdapter::isPrimaryAdapter() const
	{
		return _privateData->mAdapterDesc.mFlags.isSet( eDisplayAdapterFlagPrimaryBit );
	}

	bool DisplayAdapter::hasActiveOutputs() const
	{
		return _privateData->mActiveOutputsNum > 0;
	}

	bool DisplayAdapter::hasAnyOutputs() const
	{
		return !_privateData->mOutputInstanceList.empty();
	}

	void DisplayAdapter::registerOutput( DisplayOutputHandle pOutput )
	{
		const auto outputIndex = _privateData->mOutputInstanceList.size();

		DisplayOutputIDGen outputIDGen;
		outputIDGen.uAdapterIndex = _privateData->mAdapterDesc.mAdapterIndex;
		outputIDGen.uOutputIndex = static_cast<dsm_index_t>( outputIndex );

		auto & outputDesc = pOutput->getOutputDescInternal();
		outputDesc.mDriverType = mDriverType;
		outputDesc.mOutputIndex = outputIDGen.uOutputIndex;
		outputDesc.mOutputID = outputIDGen.mOutputID;

		_privateData->mOutputInstanceList.push_back( std::move( pOutput ) );

		// Outputs are not added to the helper list at this point.
		// This is done as a post-process step later in DisplayDriver::_enumOutputs().
		// Assertion added to prevent problems in case of refactoring.
		ic3DebugAssert( _privateData->mOutputList.empty() );
	}

	uint32 DisplayAdapter::validateOutputsConfiguration()
	{
		if( !_privateData->mOutputInstanceList.empty() )
		{
			// Reserve space for the list of pointers/handles for outputs.
			_privateData->mOutputList.reserve( _privateData->mOutputInstanceList.size() );

			for( auto & outputPtr : _privateData->mOutputInstanceList )
			{
				if( outputPtr->isPrimaryOutput() && !_privateData->mPrimaryOutput )
				{
					// Similar to the default/primary system adapter, we select default
					// output of an adapter if the driver has not set it during enumeration.
					_privateData->mPrimaryOutput = outputPtr.get();
				}

				if( outputPtr->isActiveOutput() )
				{
					_privateData->mActiveOutputsNum += 1;
				}

				for( auto colorFormat : cvColorFormatArray )
				{
					outputPtr->validateVideoModesConfiguration( colorFormat );
				}

				_privateData->mOutputList.push_back( outputPtr.get() );
			}

			// Validate if the default output for this adapter has been properly set.
			if( _privateData->mPrimaryOutput )
			{
				auto & outputDesc = _privateData->mPrimaryOutput->getOutputDescInternal();
				if( !outputDesc.mFlags.isSet( eDisplayOutputFlagPrimaryBit ) )
				{
					ic3DebugOutputFmt(
						"Primary/Default output of [%s] selected by the driver does not have "\
						"E_DISPLAY_ADAPTER_FLAG_PRIMARYBit set. Is that intentional?",
						_privateData->mAdapterDesc.mName.c_str() );
				}
			}
			else
			{
				auto & firstOutput = _privateData->mOutputInstanceList.front();
				auto & firstOutputDesc = firstOutput->getOutputDescInternal();
				firstOutputDesc.mFlags.set( eDisplayOutputFlagPrimaryBit );
				_privateData->mPrimaryOutput = firstOutput.get();
			}
		}

		return _privateData->mActiveOutputsNum;
	}

	DisplayAdapterDesc & DisplayAdapter::getAdapterDescInternal()
	{
		return _privateData->mAdapterDesc;
	}


	DisplayOutput::DisplayOutput( DisplayAdapter & pAdapter )
	: SysObject( pAdapter.mSysContext )
	, mDriverType( pAdapter.mDriverType )
	, mDisplayDriver( pAdapter.mDisplayDriver )
	, mParentAdapter( &pAdapter )
	, _privateData( std::make_unique<DisplayOutputPrivateData>() )
	{}

	DisplayOutput::~DisplayOutput() noexcept = default;

	TArrayView<const EColorFormat> DisplayOutput::getSupportedColorFormatList() const
	{
		return Cppx::bindArrayView( _privateData->mSupportedColorFormatList.data(),
		              _privateData->mSupportedColorFormatList.size() );
	}

	bool DisplayOutput::checkVideoSettingsSupport( const DisplayVideoSettings & pVideoSettings, EColorFormat pColorFormat ) const
	{
		const auto & colorFormatData = _privateData->mColorFormatMap.at( pColorFormat );
		for( const auto & videoMode : colorFormatData.mVideoModeInstanceList )
		{
			const auto & videoModeDesc = videoMode->getModeDesc();
			if( videoModeDesc.mSettings.matches( pVideoSettings ) )
			{
				return true;
			}
		}
		return false;
	}

	DisplayVideoMode * DisplayOutput::findVideoMode( EColorFormat pColorFormat, DisplayVideoModePredicate pPredicate ) const
	{
		for( auto & colorFormatData : _privateData->mColorFormatMap )
		{
			if( ( colorFormatData.first == pColorFormat ) || ( pColorFormat == EColorFormat::Unknown ) )
			{
				for( auto & videoModePtr : colorFormatData.second.mVideoModeInstanceList )
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

	DisplayVideoModeList DisplayOutput::findVideoModes( EColorFormat pColorFormat, DisplayVideoModePredicate pPredicate ) const
	{
		DisplayVideoModeList videoModeList;
		for( auto & colorFormatData : _privateData->mColorFormatMap )
		{
			if( ( colorFormatData.first == pColorFormat ) || ( pColorFormat == EColorFormat::Unknown ) )
			{
				for( auto & videoModePtr : colorFormatData.second.mVideoModeInstanceList )
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

	const DisplayOutputDesc & DisplayOutput::getOutputDesc() const
	{
		return _privateData->mOutputDesc;
	}

	const DisplayVideoModeList & DisplayOutput::getVideoModeList() const
	{
		auto colorFormat = mDisplayDriver->queryDefaultSystemColorFormat();
		return getVideoModeList( colorFormat );
	}

	const DisplayVideoModeList & DisplayOutput::getVideoModeList( EColorFormat pColorFormat ) const
	{
		const auto & colorFormatData = _privateData->mColorFormatMap.at( pColorFormat );
		return colorFormatData.mVideoModeList;
	}

	bool DisplayOutput::isActiveOutput() const
	{
		return _privateData->mOutputDesc.mFlags.isSet( eDisplayOutputFlagActiveBit );
	}

	bool DisplayOutput::isPrimaryOutput() const
	{
		return _privateData->mOutputDesc.mFlags.isSet( eDisplayOutputFlagPrimaryBit );
	}

	bool DisplayOutput::isColorFormatSupported( EColorFormat pColorFormat ) const
	{
		const auto & colorFormatData = _privateData->mColorFormatMap.at( pColorFormat );
		return !colorFormatData.mVideoModeInstanceList.empty();
	}

	void DisplayOutput::registerVideoMode( EColorFormat pColorFormat, DisplayVideoModeHandle pVideoMode )
	{
		auto & colorFormatData = _privateData->mColorFormatMap[pColorFormat];

		if( colorFormatData.mColorFormat == EColorFormat::Unknown )
		{
			colorFormatData.mColorFormat = pColorFormat;
			_privateData->mSupportedColorFormatList.push_back( pColorFormat );
		}

		const auto videoModeIndex = colorFormatData.mVideoModeInstanceList.size();

		DisplayVideoModeIDGen videoModeIDGen;
		videoModeIDGen.uOutputID = _privateData->mOutputDesc.mOutputID;
		videoModeIDGen.uColorFormatIndex = static_cast<dsm_index_t>( colorFormatData.mColorFormat );
		videoModeIDGen.uModeIndex = static_cast<dsm_index_t>( videoModeIndex );

		auto & videoModeDesc = pVideoMode->getModeDescInternal();
		videoModeDesc.mDriverType = mDriverType;
		videoModeDesc.mVideoModeIndex = videoModeIDGen.uModeIndex;
		videoModeDesc.mVideoModeID = videoModeIDGen.mModeID;
		videoModeDesc.mColorFormat = pColorFormat;

		colorFormatData.mVideoModeInstanceList.push_back( pVideoMode );

		// Video modes are not added to the helper list at this point.
		// This is done as a post-process step later in DisplayDriver::_enumVideoModes().
		// Assertion added to prevent problems in case of refactoring.
		ic3DebugAssert( colorFormatData.mVideoModeList.empty() );
	}

	uint32 DisplayOutput::validateVideoModesConfiguration( EColorFormat pColorFormat )
	{
		auto & colorFormatData = _privateData->mColorFormatMap.at( pColorFormat );
		auto videoModesNum = colorFormatData.mVideoModeInstanceList.size();

		if( videoModesNum > 0 )
		{
			colorFormatData.mVideoModeList.reserve( videoModesNum );

			for( auto & videoModePtr : colorFormatData.mVideoModeInstanceList )
			{
				colorFormatData.mVideoModeList.push_back( videoModePtr.get() );
			}
		}

		return static_cast<uint32>( videoModesNum );
	}

	DisplayOutputDesc & DisplayOutput::getOutputDescInternal()
	{
		return _privateData->mOutputDesc;
	}


	DisplayVideoMode::DisplayVideoMode( DisplayOutput & pOutput )
	: SysObject( pOutput.mSysContext )
	, mDriverType( pOutput.mDriverType )
	, mDisplayDriver( pOutput.mDisplayDriver )
	, mParentOutput( &pOutput )
	, _privateData( std::make_unique<DisplayVideoModePrivateData>() )
	{}

	DisplayVideoMode::~DisplayVideoMode() noexcept = default;

	const DisplayVideoModeDesc & DisplayVideoMode::getModeDesc() const
	{
		return _privateData->mModeDesc;
	}

	DisplayVideoModeDesc & DisplayVideoMode::getModeDescInternal()
	{
		return _privateData->mModeDesc;
	}

} // namespace Ic3::System
