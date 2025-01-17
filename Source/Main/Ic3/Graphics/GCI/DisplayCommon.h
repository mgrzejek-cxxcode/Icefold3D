
#pragma once

#ifndef __IC3_GRAPHICS_GCI_DISPLAY_COMMON_H__
#define __IC3_GRAPHICS_GCI_DISPLAY_COMMON_H__

#include "Prerequisites.h"
#include <Ic3/System/DisplayCommon.h>

namespace Ic3::Graphics::GCI
{

	using display_system_id_t = uint64;

	constexpr display_system_id_t cvDisplaySystemIDDefault = cppx::meta::limits<display_system_id_t>::max_value;
	constexpr display_system_id_t cvDisplaySystemIDInvalid = cvDisplaySystemIDDefault - 1;

	enum EAdapterFlags : uint32
	{
		EAdapterFlagPrimaryBit = static_cast<uint32>( System::eDisplayAdapterFlagPrimaryBit ),
		EAdapterFlagMultiNodeBit = static_cast<uint32>( System::eDisplayAdapterFlagMultiNodeBit ),
		EAdapterFlagSoftwareBit = static_cast<uint32>( System::eDisplayAdapterFlagSoftwareBit ),
	};

	enum EOutputFlags : uint32
	{
		E_OUTPUT_FLAG_PRIMARY_BIT = static_cast<uint32>( System::eDisplayOutputFlagPrimaryBit )
	};

	enum EVideoSettingsFlags : uint16
	{
		EVideoSettingsFlagScanInterlacedBit = static_cast<uint32>( System::eDisplayVideoSettingsFlagScanInterlacedBit ),
		EVideoSettingsFlagScanProgressiveBit = static_cast<uint32>( System::eDisplayVideoSettingsFlagScanProgressiveBit ),
		E_VIDEO_SETTINGS_FLAG_STEREO_BIT = static_cast<uint32>( System::eDisplayVideoSettingsFlagStereoBit )
	};

	enum EDisplayConfigurationFlags : uint32
	{
		E_DISPLAY_CONFIGURATION_FLAG_FULLSCREEN_BIT = 0x0001,
		E_DISPLAY_CONFIGURATION_FLAG_SYNC_MODE_ADAPTIVE_BIT = 0x0100,
		E_DISPLAY_CONFIGURATION_FLAG_SYNC_MODE_VERTICAL_BIT = 0x0200,
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
		cppx::bitmask<EVideoSettingsFlags> flags = 0u;
	};

	struct AdapterDesc
	{
		display_system_id_t id = cvDisplaySystemIDInvalid;
		std::string name;
		cppx::bitmask<EAdapterFlags> flags = 0u;
	};

	struct OutputDesc
	{
		display_system_id_t id = cvDisplaySystemIDInvalid;
		std::string name;
		System::ScreenRect screenRect;
		cppx::bitmask<EOutputFlags> flags = 0u;
	};

	struct VideoModeDesc
	{
		display_system_id_t id = cvDisplaySystemIDInvalid;
		System::EColorFormat format = System::EColorFormat::Unknown;
		VideoSettings settings;
	};

	inline EAdapterFlags translateSysAdapterFlags( cppx::bitmask<System::EDisplayAdapterFlags> pSysAdapterFlags )
	{
		cppx::bitmask<EAdapterFlags> result = 0;
		if( pSysAdapterFlags.is_set( System::eDisplayAdapterFlagPrimaryBit ) )
		{
			result.set( EAdapterFlagPrimaryBit );
		}
		if( pSysAdapterFlags.is_set( System::eDisplayAdapterFlagMultiNodeBit ) )
		{
			result.set( EAdapterFlagMultiNodeBit );
		}
		if( pSysAdapterFlags.is_set( System::eDisplayAdapterFlagSoftwareBit ) )
		{
			result.set( EAdapterFlagSoftwareBit );
		}
		return result;
	}

	inline EOutputFlags translateSysOutputFlags( cppx::bitmask<System::EDisplayOutputFlags> pSysOutputFlags )
	{
		cppx::bitmask<EOutputFlags> result = 0;
		if( pSysOutputFlags.is_set( System::eDisplayOutputFlagPrimaryBit ) )
		{
			result.set( E_OUTPUT_FLAG_PRIMARY_BIT );
		}
		return result;
	}

	inline EVideoSettingsFlags translateSysVideoSettingsFlags( cppx::bitmask<System::EDisplayVideoSettingsFlags> pSysVideoSettingsFlags )
	{
		cppx::bitmask<EVideoSettingsFlags> result = 0;
		if( pSysVideoSettingsFlags.is_set( System::eDisplayVideoSettingsFlagScanInterlacedBit ) )
		{
			result.set( EVideoSettingsFlagScanInterlacedBit );
		}
		if( pSysVideoSettingsFlags.is_set( System::eDisplayVideoSettingsFlagScanProgressiveBit ) )
		{
			result.set( EVideoSettingsFlagScanProgressiveBit );
		}
		if( pSysVideoSettingsFlags.is_set( System::eDisplayVideoSettingsFlagStereoBit ) )
		{
			result.set( E_VIDEO_SETTINGS_FLAG_STEREO_BIT );
		}
		return result;
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_DISPLAY_COMMON_H__
