
#include "../Internal/DisplaySystemPrivate.h"
#include <cppx/stringUtils.h>
#include <sstream>

namespace Ic3::System
{

	std::string DisplayAdapterDesc::ToString() const
	{
		std::stringstream strStream;
		strStream << "Adapter <" << adapterIndex << "> (" << name << ")";

		if( flags.is_set( eDisplayAdapterFlagActiveBit ) )
		{
			strStream << ", Active";
		}

		if( flags.is_set( eDisplayAdapterFlagPrimaryBit ) )
		{
			strStream << ", Primary";
		}

		if( flags.is_set( eDisplayAdapterFlagHardwareBit ) )
		{
			strStream << ", Hardware";
		}
		else if( flags.is_set( eDisplayAdapterFlagSoftwareBit ) )
		{
			strStream << ", Software";
		}

		return strStream.str();
	}

	std::string DisplayOutputDesc::ToString() const
	{
		std::stringstream strStream;
		strStream << "Output <" << outputIndex << "> (" << name << ")";

		if( flags.is_set( eDisplayOutputFlagActiveBit ) )
		{
			strStream << ", Active";
		}

		if( flags.is_set( eDisplayOutputFlagPrimaryBit ) )
		{
			strStream << ", Primary";
		}

		strStream << ", Origin:[" << screenRect.offset.x << "x" << screenRect.offset.y << "]";
		strStream << ", Size:[" << screenRect.size.x << "x" << screenRect.size.y << "]";

		return strStream.str();
	}

	std::string DisplayVideoModeDesc::ToString() const
	{
		std::stringstream strStream;
		strStream << "DisplayMode <" << videoModeIndex << "> ";

		const auto settingsText = DSMGetVideoSettingsString( colorFormat, settings );
		strStream << settingsText;

		return strStream.str();
	}


	dsm_index_t DSMExtractOutputIDAdapterIndex( dsm_output_id_t pOutputID )
	{
		DisplayOutputIDGen outputIDGen;
		outputIDGen.outputID = pOutputID;
		return outputIDGen.uAdapterIndex;
	}

	dsm_index_t DSMExtractOutputIDOutputIndex( dsm_output_id_t pOutputID )
	{
		DisplayOutputIDGen outputIDGen;
		outputIDGen.outputID = pOutputID;
		return outputIDGen.uOutputIndex;
	}

	dsm_output_id_t DSMCreateDisplayOutputID( dsm_index_t pAdapterIndex, dsm_index_t pOutputIndex )
	{
		DisplayOutputIDGen outputIDGen;
		outputIDGen.uAdapterIndex = pAdapterIndex;
		outputIDGen.uOutputIndex = pOutputIndex;
		return outputIDGen.outputID;
	}

	inline dsm_video_settings_hash_t DSMComputeVideoSettingsHash( EColorFormat pFormat, const DisplayVideoSettings & pSettings )
	{
		DisplayVideoSettingsHashGen hashValueGen;
		hashValueGen.uResWidth = static_cast<uint16>( pSettings.resolution.x );
		hashValueGen.uResHeight = static_cast<uint16>( pSettings.resolution.x );
		hashValueGen.uRefreshRate = static_cast<uint16>( pSettings.refreshRate );
		hashValueGen.uFlags = static_cast<uint8>( pSettings.flags );
		hashValueGen.uColorFormatIndex = static_cast<uint8>( pFormat );
		return hashValueGen.hashValue;
	}

	std::string DSMGetVideoSettingsString( EColorFormat pFormat, const DisplayVideoSettings & pSettings )
	{
		auto & colorFormatDesc = VisGetDescForColorFormat( pFormat );
		auto settingsHash = DSMComputeVideoSettingsHash( pFormat, pSettings );

		std::stringstream strStream;
		strStream << pSettings.resolution.x << 'x' << pSettings.resolution.y;

		if( pSettings.flags.is_set( eDisplayVideoSettingsFlagScanProgressiveBit ) )
		{
			strStream << "p";
		}
		else if( pSettings.flags.is_set( eDisplayVideoSettingsFlagScanInterlacedBit ) )
		{
			strStream << "i";
		}

		strStream << ", " << colorFormatDesc.size << "bit, " << pSettings.refreshRate << "Hz";
		strStream << ", //0x" << std::hex << settingsHash << std::dec;

		return strStream.str();
	}

	EDisplayAdapterVendorID DSMResolveAdapterVendorID( const std::string & pAdapterName )
	{
		auto adapterVendorID = EDisplayAdapterVendorID::Unknown;
		auto adapterString = cppx::strutil::make_upper( pAdapterName );

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

	bool DSMCheckSettingsFilterMatch( const DisplayVideoSettingsFilter & pFilter, const DisplayVideoSettings & pSettings )
	{
		return true;
	}

} // namespace Ic3::System
