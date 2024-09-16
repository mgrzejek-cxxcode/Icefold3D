
#include "Internal/DisplaySystemPrivate.h"
#include <Ic3/Cppx/StringUtils.h>
#include <sstream>

namespace Ic3::System
{

	std::string DisplayAdapterDesc::toString() const
	{
		std::stringstream strStream;
		strStream << "Adapter <" << mAdapterIndex << "> (" << mName << ")";

		if( mFlags.isSet( eDisplayAdapterFlagActiveBit ) )
		{
			strStream << ", Active";
		}

		if( mFlags.isSet( eDisplayAdapterFlagPrimaryBit ) )
		{
			strStream << ", Primary";
		}

		if( mFlags.isSet( eDisplayAdapterFlagHardwareBit ) )
		{
			strStream << ", Hardware";
		}
		else if( mFlags.isSet( eDisplayAdapterFlagSoftwareBit ) )
		{
			strStream << ", Software";
		}

		return strStream.str();
	}

	std::string DisplayOutputDesc::toString() const
	{
		std::stringstream strStream;
		strStream << "Output <" << mOutputIndex << "> (" << mName << ")";

		if( mFlags.isSet( eDisplayOutputFlagActiveBit ) )
		{
			strStream << ", Active";
		}

		if( mFlags.isSet( eDisplayOutputFlagPrimaryBit ) )
		{
			strStream << ", Primary";
		}

		strStream << ", Origin:[" << mScreenRect.mOffset.x << "x" << mScreenRect.mOffset.y << "]";
		strStream << ", Size:[" << mScreenRect.mSize.x << "x" << mScreenRect.mSize.y << "]";

		return strStream.str();
	}

	std::string DisplayVideoModeDesc::toString() const
	{
		std::stringstream strStream;
		strStream << "DisplayMode <" << mVideoModeIndex << "> ";

		const auto settingsText = dsmGetVideoSettingsString( mColorFormat, mSettings );
		strStream << settingsText;

		return strStream.str();
	}

	dsm_output_id_t dsmCreateDisplayOutputID( dsm_index_t pAdapterIndex, dsm_index_t pOutputIndex )
	{
		DisplayOutputIDGen outputIDGen;
		outputIDGen.uAdapterIndex = pAdapterIndex;
		outputIDGen.uOutputIndex = pOutputIndex;
		return outputIDGen.mOutputID;
	}

	dsm_video_settings_hash_t dsmComputeVideoSettingsHash( EColorFormat pFormat, const DisplayVideoSettings & pSettings )
	{
		DisplayVideoSettingsHashGen hashValueGen;
		hashValueGen.uResWidth = static_cast<uint16>( pSettings.mResolution.x );
		hashValueGen.uResHeight = static_cast<uint16>( pSettings.mResolution.x );
		hashValueGen.uRefreshRate = static_cast<uint16>( pSettings.mRefreshRate );
		hashValueGen.uFlags = static_cast<uint8>( pSettings.mFlags );
		hashValueGen.uColorFormatIndex = static_cast<uint8>( pFormat );
		return hashValueGen.mHashValue;
	}

	std::string dsmGetVideoSettingsString( EColorFormat pFormat, const DisplayVideoSettings & pSettings )
	{
		auto & colorFormatDesc = vsxGetDescForColorFormat( pFormat );
		auto settingsHash = dsmComputeVideoSettingsHash( pFormat, pSettings );

		std::stringstream strStream;
		strStream << pSettings.mResolution.x << 'x' << pSettings.mResolution.y;

		if( pSettings.mFlags.isSet( eDisplayVideoSettingsFlagScanProgressiveBit ) )
		{
			strStream << "p";
		}
		else if( pSettings.mFlags.isSet( eDisplayVideoSettingsFlagScanInterlacedBit ) )
		{
			strStream << "i";
		}

		strStream << ", " << colorFormatDesc.mSize << "bit, " << pSettings.mRefreshRate << "Hz";
		strStream << ", //0x" << std::hex << settingsHash << std::dec;

		return strStream.str();
	}

	EDisplayAdapterVendorID dsmResolveAdapterVendorID( const std::string & pAdapterName )
	{
		auto adapterVendorID = EDisplayAdapterVendorID::Unknown;
		auto adapterString = Cppx::strUtils::makeUpper( pAdapterName );

		if( ( adapterString.find( "AMD" ) != std::string::npos ) || ( adapterString.find( "ATI" ) != std::string::npos ) )
		{
			adapterVendorID = EDisplayAdapterVendorID::AMD;
		}
		else if( adapterString.find( "ARM" ) != std::string::npos )
		{
			adapterVendorID = EDisplayAdapterVendorID::ARM;
		}
		else if( adapterString.find( "GOOGLE" ) != std::string::npos )
		{
			adapterVendorID = EDisplayAdapterVendorID::Google;
		}
		else if( adapterString.find( "INTEL" ) != std::string::npos )
		{
			adapterVendorID = EDisplayAdapterVendorID::Intel;
		}
		else if( adapterString.find( "MICROSOFT" ) != std::string::npos )
		{
			adapterVendorID = EDisplayAdapterVendorID::Microsoft;
		}
		else if( adapterString.find( "NVIDIA" ) != std::string::npos )
		{
			adapterVendorID = EDisplayAdapterVendorID::NVidia;
		}
		else if( adapterString.find( "QUALCOMM" ) != std::string::npos )
		{
			adapterVendorID = EDisplayAdapterVendorID::Qualcomm;
		}

		return adapterVendorID;
	}

	bool dsmCheckSettingsFilterMatch( const DisplayVideoSettingsFilter & pFilter, const DisplayVideoSettings & pSettings )
	{
		return true;
	}

} // namespace Ic3::System
