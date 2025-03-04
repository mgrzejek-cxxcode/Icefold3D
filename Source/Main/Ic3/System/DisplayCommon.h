
#ifndef __IC3_SYSTEM_DISPLAY_COMMON_H__
#define __IC3_SYSTEM_DISPLAY_COMMON_H__

#include "Visual.h"

#include <Ic3/Math/VectorOps.h>
#include <cppx/arrayView.h>
#include <cppx/bitmask.h>
#include <cppx/uuid.h>
#include <functional>

#if( PCL_COMPILER & PCL_COMPILER_CLANG )
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#  pragma clang diagnostic ignored "-Wnested-anon-types"
#elif( PCL_COMPILER & PCL_COMPILER_MSVC )
#  pragma warning( push )
#  pragma warning( disable: 4201 )  // 'Nonstandard extension used: nameless struct/union'
#elif( PCL_COMPILER & ( PCL_COMPILER_GCC | PCL_COMPILER_MINGW64 ) )
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wpedantic"
#endif

#if !defined( IC3_SYSTEM_DSM_DRIVER_TYPE_SUPPORT_DXGI )
#  if( PCL_TARGET_OS & PCL_TARGET_FLAG_OS_WFA )
#    define IC3_SYSTEM_DSM_DRIVER_TYPE_SUPPORT_DXGI 1
#  else
#    define IC3_SYSTEM_DSM_DRIVER_TYPE_SUPPORT_DXGI 0
#  endif
#endif

#if !defined( IC3_SYSTEM_DSM_DRIVER_TYPE_SUPPORT_SDL )
#  define IC3_SYSTEM_DSM_DRIVER_TYPE_SUPPORT_SDL 0
#endif

namespace Ic3::System
{

	Ic3SysDeclareHandle( DisplayManager );
	Ic3SysDeclareHandle( DisplayDriver );

	class DisplayAdapter;
	class DisplayOutput;
	class DisplayVideoMode;

	using dsm_index_t = uint16;
	using dsm_output_id_t = uint32;
	using dsm_video_mode_id_t = uint64;
	using dsm_video_settings_hash_t = uint64;
	using DisplayOffset = Math::Pos2i;
	using DisplaySize = Math::Size2u;

	using DisplayAdapterList = std::vector<DisplayAdapter *>;
	using DisplayOutputList = std::vector<DisplayOutput *>;
	using DisplayVideoModeList = std::vector<DisplayVideoMode *>;

	using DisplayAdapterPredicate = std::function<bool( const DisplayAdapter & )>;
	using DisplayOutputPredicate = std::function<bool( const DisplayOutput & )>;
	using DisplayVideoModePredicate = std::function<bool( const DisplayVideoMode & )>;

	/// @brief Represents invalid display system index (of an adapter or an output, for example).
	inline constexpr dsm_index_t CX_DSM_INDEX_INVALID = cppx::meta::limits<dsm_index_t>::max_value - 1;

	inline constexpr auto kDSMIndexInvalid = cppx::meta::limits<dsm_index_t>::max_value - 1;

	/// @brief
	inline constexpr dsm_index_t CX_DSM_INDEX_DEFAULT = cppx::meta::limits<dsm_index_t>::max_value;

	/// @brief
	inline constexpr dsm_output_id_t CX_DSM_OUTPUT_ID_DEFAULT = cppx::meta::limits<dsm_output_id_t>::max_value;

	/// @brief Represents invalid display settings hash. Used to identify/report invalid and/or empty configurations.
	inline constexpr dsm_video_settings_hash_t CX_DSM_VIDEO_SETTINGS_HASH_INVALID = cppx::meta::limits<dsm_video_settings_hash_t>::max_value;

	/// @brief Specifies supported types of drivers available through a DisplayManager.
	/// Driver support is platform-specific and some of them may not be available on some systems.
	/// The default driver is always supported - if a certain platform does not support the required
	///  display-related functionality, DisplayManager creation will fail with NotSupported error.
	enum class EDisplayDriverType : enum_default_value_t
	{
		// Unknown driver. Cannot be used as an input.
		// Reported for an uninitialized driver object.
		Unknown,

		// Native, platform-specific driver. Implemented either with dedicated
		// libraries (like SDL) or low-level system API (Win32, XF86, XRR, Cocoa).
		// Platform availability: All
		Generic,

		// Microsoft DXGI driver. Specific DXGI version depends on the platform.
		// Platform availability: Win32, UWP
		DXGI,

		// SDL-based driver implemented using only the library's functionality.
		// Provided for compliance tests and for compatibility reasons.
		// Platform availability: Linux, macOS
		SDL,

		// Default driver. Actual type is platform-specific, but it must resolve
		// to a driver type which is always supported on the current platform.
		// Most likely, the actual driver type will be as follows:
		// - Win32/UWP: DXGI
		// - Linux/Android/macOS/iOS: Generic
		Default = 0xFFFF,
	};

	/// @brief Represents ID of the vendor of the graphics adapter/device.
	enum class EDisplayAdapterVendorID : enum_default_value_t
	{
		Unknown,
		AMD,
		ARM,
		Google,
		Intel,
		Microsoft,
		NVidia,
		Qualcomm,
	};

	/// @brief
	enum EDisplayAdapterFlags : uint32
	{
		// Adapter is active. The meaning of this flag is driver-specific - do not assume an active adapter
		// to have, for example, any active outputs. Use DisplayAdapter::HasActiveOutputs() to check that.
		eDisplayAdapterFlagActiveBit = 0x1,
		// Adapter is a primary/default adapter in the system.
		eDisplayAdapterFlagPrimaryBit = 0x2,
		// Adapter is a software adapter.
		eDisplayAdapterFlagHardwareBit = 0x4,
		// Adapter is a software adapter. May indicate WARP-capable adapter in case of a DXGI driver.
		eDisplayAdapterFlagSoftwareBit = 0x8,
		// Adapter is a multi-node adapter (e.g. classic CF- or SLI-configured adapter set).
		eDisplayAdapterFlagMultiNodeBit = 0x10,
	};

	/// @brief
	enum EDisplayOutputFlags : uint32
	{
		// Output is active. In most cases it means the output is connected to the desktop.
		eDisplayOutputFlagActiveBit = 0x1,
		// Output is the primary output of an adapter.
		eDisplayOutputFlagPrimaryBit = 0x2
	};

	/// @brief
	enum EDisplayVideoSettingsFlags : uint16
	{
		eDisplayVideoSettingsFlagScanInterlacedBit = 0x1,
		eDisplayVideoSettingsFlagScanProgressiveBit = 0x2,
		eDisplayVideoSettingsFlagStereoBit = 0x4
	};

	enum EDisplayVideoSettingsFilterFlags : uint32
	{
		// If set, resolution is not used as a filter criterion.
		eDisplayVideoSettingsFilterFlagResolutionIgnoreBit = 0x1,
		// If set, resolution must match exactly for the mode to be included.
		eDisplayVideoSettingsFilterFlagResolutionMatchExactBit = 0x2,
		// If set, refresh rate is not used as a filter criterion.
		eDisplayVideoSettingsFilterFlagRefreshRateIgnoreBit = 0x4,
		// If set, refresh rate must match exactly for the mode to be included.
		eDisplayVideoSettingsFilterFlagRefreshRateMatchExactBit = 0x8,
		// If set, additional flags are not used as a filter criterion.
		eDisplayVideoSettingsFilterFlagFlagsIgnoreBit = 0x40,
	};

	struct ScreenRect
	{
		DisplayOffset offset;
		DisplaySize size;
	};

	/// @brief
	struct DisplayVideoSettings
	{
		DisplaySize resolution;

		uint16 refreshRate = 0u;

		cppx::bitmask<EDisplayVideoSettingsFlags> flags = 0u;

		CPPX_ATTR_NO_DISCARD bool equals( const DisplayVideoSettings & pOther ) const
		{
			return ( resolution == pOther.resolution ) && ( refreshRate == pOther.refreshRate ) && ( flags == pOther.flags );
		}

		CPPX_ATTR_NO_DISCARD bool matches( const DisplayVideoSettings & pOther ) const
		{
			return ( resolution == pOther.resolution ) && ( refreshRate == pOther.refreshRate ) &&
					flags.is_set( pOther.flags );
		}
	};

	inline bool operator==( const DisplayVideoSettings & pLhs, const DisplayVideoSettings & pRhs )
	{
		return pLhs.equals( pRhs );
	}

	inline bool operator!=( const DisplayVideoSettings & pLhs, const DisplayVideoSettings & pRhs )
	{
		return !pLhs.equals( pRhs );
	}

	inline constexpr DisplayVideoSettings cvDisplayVideoSettingsEmpty { { 0U, 0U }, 0U, 0U };

	/// @brief
	struct DisplayVideoSettingsFilter
	{
		// Reference settings used to filter video modes. By default, each property in a mode
		// must be greater than or equal to the reference property (note for resolutions: both
		// dimensions must be >=, i.e. 1024x768 satisfies 1024x600, but 1280x720 does not satisfy
		// 1152x864). This does not apply to the flags, which must have exactly the same bits set.
		// Each property can be ignored by using its respective IGNORE flag. Also, "greater than
		// or equal" condition can be changed to "equal" for a given property by setting MATCH_EXACT
		// flag for that property.
		DisplayVideoSettings refSettings;

		// Filter flags used to control the process. Flags set by default: none.
		cppx::bitmask<EDisplayVideoSettingsFilterFlags> flags = 0;
	};

	inline bool operator==( const DisplayVideoSettingsFilter & pLhs, const DisplayVideoSettingsFilter & pRhs )
	{
		return ( pLhs.refSettings == pRhs.refSettings ) && ( pLhs.flags == pRhs.flags );
	}

	inline bool operator!=( const DisplayVideoSettingsFilter & pLhs, const DisplayVideoSettingsFilter & pRhs )
	{
		return ( pLhs.refSettings != pRhs.refSettings ) || ( pLhs.flags != pRhs.flags );
	}

	inline constexpr DisplayVideoSettingsFilter cvDisplayVideoSettingsFilterNone { cvDisplayVideoSettingsEmpty, 0U };

	struct DisplayAdapterDesc
	{
		EDisplayDriverType driverType = EDisplayDriverType::Unknown;
		dsm_index_t adapterIndex = CX_DSM_INDEX_INVALID;
		EDisplayAdapterVendorID vendorID = EDisplayAdapterVendorID::Unknown;
		cppx::bitmask<EDisplayAdapterFlags> flags = 0u;
		std::string name;

		std::string ToString() const;
	};

	struct DisplayOutputDesc
	{
		EDisplayDriverType driverType = EDisplayDriverType::Unknown;
		dsm_index_t outputIndex = CX_DSM_INDEX_INVALID;
		dsm_output_id_t outputID = 0u;
		cppx::bitmask<EDisplayOutputFlags> flags = 0u;
		std::string name;
		ScreenRect screenRect;

		std::string ToString() const;
	};

	struct DisplayVideoModeDesc
	{
		EDisplayDriverType driverType = EDisplayDriverType::Unknown;
		dsm_index_t videoModeIndex = CX_DSM_INDEX_INVALID;
		dsm_video_mode_id_t videoModeID = 0u;
		dsm_video_settings_hash_t settingsHash = 0u;
		EColorFormat colorFormat = EColorFormat::Unknown;
		DisplayVideoSettings settings;

		std::string ToString() const;
	};


} // namespace Ic3::System

#if( PCL_COMPILER & PCL_COMPILER_CLANG )
#  pragma clang diagnostic pop
#elif( PCL_COMPILER & PCL_COMPILER_MSVC )
#  pragma warning( pop )
#elif( PCL_COMPILER & ( PCL_COMPILER_GCC | PCL_COMPILER_MINGW64 ) )
#  pragma GCC diagnostic pop
#endif

#endif // __IC3_SYSTEM_DISPLAY_COMMON_H__
