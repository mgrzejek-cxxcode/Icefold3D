
#pragma once

#ifndef __IC3_GRAPHICS_GCI_DISPLAY_COMMON_H__
#define __IC3_GRAPHICS_GCI_DISPLAY_COMMON_H__

#include "Prerequisites.h"
#include <Ic3/System/Core/DisplayCommon.h>

namespace Ic3::Graphics::GCI
{

	using display_system_id_t = uint64;

	constexpr display_system_id_t cvDisplaySystemIDDefault = cppx::meta::limits<display_system_id_t>::max_value;
	constexpr display_system_id_t cvDisplaySystemIDInvalid = cvDisplaySystemIDDefault - 1;

	enum EAdapterFlags : uint32
	{
		eAdapterFlagPrimaryBit = static_cast<uint32>( System::eDisplayAdapterFlagPrimaryBit ),
		eAdapterFlagMultiNodeBit = static_cast<uint32>( System::eDisplayAdapterFlagMultiNodeBit ),
		eAdapterFlagSoftwareBit = static_cast<uint32>( System::eDisplayAdapterFlagSoftwareBit ),
	};

	enum EOutputFlags : uint32
	{
		eOutputFlagPrimaryBit = static_cast<uint32>( System::eDisplayOutputFlagPrimaryBit )
	};

	enum EVideoSettingsFlags : uint16
	{
		eVideoSettingsFlagScanInterlacedBit = static_cast<uint32>( System::eDisplayVideoSettingsFlagScanInterlacedBit ),
		eVideoSettingsFlagScanProgressiveBit = static_cast<uint32>( System::eDisplayVideoSettingsFlagScanProgressiveBit ),
		eVideoSettingsFlagStereoBit = static_cast<uint32>( System::eDisplayVideoSettingsFlagStereoBit )
	};

	enum EDisplayConfigurationFlags : uint32
	{
		eDisplayConfigurationFlagFullscreenBit = 0x0001,
		eDisplayConfigurationFlagSyncModeAdaptiveBit = 0x0100,
		eDisplayConfigurationFlagSyncModeVerticalBit = 0x0200,
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

	inline cppx::bitmask<EAdapterFlags> TranslateSysAdapterFlags( cppx::bitmask<System::EDisplayAdapterFlags> pSysAdapterFlags )
	{
		cppx::bitmask<EAdapterFlags> result = 0;
		if( pSysAdapterFlags.is_set( System::eDisplayAdapterFlagPrimaryBit ) )
		{
			result.set( eAdapterFlagPrimaryBit );
		}
		if( pSysAdapterFlags.is_set( System::eDisplayAdapterFlagMultiNodeBit ) )
		{
			result.set( eAdapterFlagMultiNodeBit );
		}
		if( pSysAdapterFlags.is_set( System::eDisplayAdapterFlagSoftwareBit ) )
		{
			result.set( eAdapterFlagSoftwareBit );
		}
		return result;
	}

	inline cppx::bitmask<EOutputFlags> TranslateSysOutputFlags( cppx::bitmask<System::EDisplayOutputFlags> pSysOutputFlags )
	{
		cppx::bitmask<EOutputFlags> result = 0;
		if( pSysOutputFlags.is_set( System::eDisplayOutputFlagPrimaryBit ) )
		{
			result.set( eOutputFlagPrimaryBit );
		}
		return result;
	}

	inline cppx::bitmask<EVideoSettingsFlags> TranslateSysVideoSettingsFlags( cppx::bitmask<System::EDisplayVideoSettingsFlags> pSysVideoSettingsFlags )
	{
		cppx::bitmask<EVideoSettingsFlags> result = 0;
		if( pSysVideoSettingsFlags.is_set( System::eDisplayVideoSettingsFlagScanInterlacedBit ) )
		{
			result.set( eVideoSettingsFlagScanInterlacedBit );
		}
		if( pSysVideoSettingsFlags.is_set( System::eDisplayVideoSettingsFlagScanProgressiveBit ) )
		{
			result.set( eVideoSettingsFlagScanProgressiveBit );
		}
		if( pSysVideoSettingsFlags.is_set( System::eDisplayVideoSettingsFlagStereoBit ) )
		{
			result.set( eVideoSettingsFlagStereoBit );
		}
		return result;
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_DISPLAY_COMMON_H__
