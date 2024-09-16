
#pragma once

#ifndef __IC3_GRAPHICS_GCI_DISPLAY_COMMON_H__
#define __IC3_GRAPHICS_GCI_DISPLAY_COMMON_H__

#include "Prerequisites.h"
#include <Ic3/System/DisplayCommon.h>

namespace Ic3::Graphics::GCI
{

	using display_system_id_t = uint64;

	constexpr display_system_id_t cvDisplaySystemIDDefault = QLimits<display_system_id_t>::sMaxValue;
	constexpr display_system_id_t cvDisplaySystemIDInvalid = cvDisplaySystemIDDefault - 1;

	enum EAdapterFlags : uint32
	{
		E_ADAPTER_FLAG_PRIMARYBit = static_cast<uint32>( System::eDisplayAdapterFlagPrimaryBit ),
		E_ADAPTER_FLAG_MULTI_NODEBit = static_cast<uint32>( System::eDisplayAdapterFlagMultiNodeBit ),
		E_ADAPTER_FLAG_SOFTWAREBit = static_cast<uint32>( System::eDisplayAdapterFlagSoftwareBit ),
	};

	enum EOutputFlags : uint32
	{
		E_OUTPUT_FLAG_PRIMARYBit = static_cast<uint32>( System::eDisplayOutputFlagPrimaryBit )
	};

	enum EVideoSettingsFlags : uint16
	{
		E_VIDEO_SETTINGS_FLAG_SCAN_INTERLACEDBit = static_cast<uint32>( System::eDisplayVideoSettingsFlagScanInterlacedBit ),
		E_VIDEO_SETTINGS_FLAG_SCAN_PROGRESSIVEBit = static_cast<uint32>( System::eDisplayVideoSettingsFlagScanProgressiveBit ),
		E_VIDEO_SETTINGS_FLAG_STEREOBit = static_cast<uint32>( System::eDisplayVideoSettingsFlagStereoBit )
	};

	enum EDisplayConfigurationFlags : uint32
	{
		E_DISPLAY_CONFIGURATION_FLAG_FULLSCREENBit = 0x0001,
		E_DISPLAY_CONFIGURATION_FLAG_SYNC_MODE_ADAPTIVEBit = 0x0100,
		E_DISPLAY_CONFIGURATION_FLAG_SYNC_MODE_VERTICALBit = 0x0200,
	};

	struct ScreenRect
	{
		Math::Vec2i32 offset;
		Math::Vec2u32 size;
	};

	/// @brief
	struct VideoSettings
	{
		Math::Size2u resolution;
		uint16 refreshRate = 0u;
		TBitmask<EVideoSettingsFlags> flags = 0u;
	};

	struct AdapterDesc
	{
		display_system_id_t id = cvDisplaySystemIDInvalid;
		std::string name;
		TBitmask<EAdapterFlags> flags = 0u;
	};

	struct OutputDesc
	{
		display_system_id_t id = cvDisplaySystemIDInvalid;
		std::string name;
		System::ScreenRect screenRect;
		TBitmask<EOutputFlags> flags = 0u;
	};

	struct VideoModeDesc
	{
		display_system_id_t id = cvDisplaySystemIDInvalid;
		System::EColorFormat format = System::EColorFormat::Unknown;
		VideoSettings settings;
	};

	inline EAdapterFlags translateSysAdapterFlags( TBitmask<System::EDisplayAdapterFlags> pSysAdapterFlags )
	{
		TBitmask<EAdapterFlags> result = 0;
		if( pSysAdapterFlags.isSet( System::eDisplayAdapterFlagPrimaryBit ) )
		{
			result.set( E_ADAPTER_FLAG_PRIMARYBit );
		}
		if( pSysAdapterFlags.isSet( System::eDisplayAdapterFlagMultiNodeBit ) )
		{
			result.set( E_ADAPTER_FLAG_MULTI_NODEBit );
		}
		if( pSysAdapterFlags.isSet( System::eDisplayAdapterFlagSoftwareBit ) )
		{
			result.set( E_ADAPTER_FLAG_SOFTWAREBit );
		}
		return result;
	}

	inline EOutputFlags translateSysOutputFlags( TBitmask<System::EDisplayOutputFlags> pSysOutputFlags )
	{
		TBitmask<EOutputFlags> result = 0;
		if( pSysOutputFlags.isSet( System::eDisplayOutputFlagPrimaryBit ) )
		{
			result.set( E_OUTPUT_FLAG_PRIMARYBit );
		}
		return result;
	}

	inline EVideoSettingsFlags translateSysVideoSettingsFlags( TBitmask<System::EDisplayVideoSettingsFlags> pSysVideoSettingsFlags )
	{
		TBitmask<EVideoSettingsFlags> result = 0;
		if( pSysVideoSettingsFlags.isSet( System::eDisplayVideoSettingsFlagScanInterlacedBit ) )
		{
			result.set( E_VIDEO_SETTINGS_FLAG_SCAN_INTERLACEDBit );
		}
		if( pSysVideoSettingsFlags.isSet( System::eDisplayVideoSettingsFlagScanProgressiveBit ) )
		{
			result.set( E_VIDEO_SETTINGS_FLAG_SCAN_PROGRESSIVEBit );
		}
		if( pSysVideoSettingsFlags.isSet( System::eDisplayVideoSettingsFlagStereoBit ) )
		{
			result.set( E_VIDEO_SETTINGS_FLAG_STEREOBit );
		}
		return result;
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_DISPLAY_COMMON_H__
