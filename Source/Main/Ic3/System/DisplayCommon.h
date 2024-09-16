
#ifndef __IC3_SYSTEM_DISPLAY_COMMON_H__
#define __IC3_SYSTEM_DISPLAY_COMMON_H__

#include "Visual.h"

#include <Ic3/Math/VectorOps.h>
#include <Ic3/Cppx/ArrayView.h>
#include <Ic3/Cppx/Bitmask.h>
#include <Ic3/Cppx/UUID.h>
#include <functional>

#if( IC3_PCL_COMPILER & IC3_PCL_COMPILER_CLANG )
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#  pragma clang diagnostic ignored "-Wnested-anon-types"
#elif( IC3_PCL_COMPILER & IC3_PCL_COMPILER_MSVC )
#  pragma warning( push )
#  pragma warning( disable: 4201 )  // 'Nonstandard extension used: nameless struct/union'
#elif( IC3_PCL_COMPILER & ( IC3_PCL_COMPILER_GCC | IC3_PCL_COMPILER_MINGW64 ) )
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wpedantic"
#endif

#if !defined( IC3_SYSTEM_DSM_DRIVER_TYPE_SUPPORT_DXGI )
#  if( IC3_PCL_TARGET_OS & IC3_PCL_TARGET_FLAG_OS_MSE )
#    define IC3_SYSTEM_DSM_DRIVER_TYPE_SUPPORT_DXGI 1
#  else
#    define IC3_SYSTEM_DSM_DRIVER_TYPE_SUPPORT_DXGI 0
#  endif
#endif

#if !defined( IC3_SYSTEM_DSM_DRIVER_TYPE_SUPPORT_SDL )
#  define IC3_SYSTEM_DSM_DRIVER_TYPE_SUPPORT_SDL 0
#endif

#pragma push
#undef INTEL

namespace Ic3::System
{

	ic3SysDeclareHandle( DisplayManager );
	ic3SysDeclareHandle( DisplayDriver );

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
	inline constexpr dsm_index_t CX_DSM_INDEX_INVALID = Cppx::QLimits<dsm_index_t>::sMaxValue - 1;

	/// @brief
	inline constexpr dsm_index_t CX_DSM_INDEX_DEFAULT = Cppx::QLimits<dsm_index_t>::sMaxValue;

	/// @brief
	inline constexpr dsm_output_id_t CX_DSM_OUTPUT_ID_DEFAULT = Cppx::QLimits<dsm_output_id_t>::sMaxValue;

	/// @brief Represents invalid display settings hash. Used to identify/report invalid and/or empty configurations.
	inline constexpr dsm_video_settings_hash_t CX_DSM_VIDEO_SETTINGS_HASH_INVALID = Cppx::QLimits<dsm_video_settings_hash_t>::sMaxValue;

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
		// to have, for example, any active outputs. Use DisplayAdapter::hasActiveOutputs() to check that.
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
		DisplayOffset mOffset;
		DisplaySize mSize;
	};

	/// @brief
	struct DisplayVideoSettings
	{
		DisplaySize mResolution;

		uint16 mRefreshRate = 0u;

		TBitmask<EDisplayVideoSettingsFlags> mFlags = 0u;

		IC3_ATTR_NO_DISCARD bool equals( const DisplayVideoSettings & pOther ) const
		{
			return ( mResolution == pOther.mResolution ) && (mRefreshRate == pOther.mRefreshRate ) && ( mFlags == pOther.mFlags );
		}

		IC3_ATTR_NO_DISCARD bool matches( const DisplayVideoSettings & pOther ) const
		{
			return ( mResolution == pOther.mResolution ) && (mRefreshRate == pOther.mRefreshRate ) && mFlags.isSet( pOther.mFlags );
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
		DisplayVideoSettings mRefSettings;

		// Filter flags used to control the process. Flags set by default: none.
		TBitmask<EDisplayVideoSettingsFilterFlags> mFlags = 0;
	};

	inline bool operator==( const DisplayVideoSettingsFilter & pLhs, const DisplayVideoSettingsFilter & pRhs )
	{
		return ( pLhs.mRefSettings == pRhs.mRefSettings ) && ( pLhs.mFlags == pRhs.mFlags );
	}

	inline bool operator!=( const DisplayVideoSettingsFilter & pLhs, const DisplayVideoSettingsFilter & pRhs )
	{
		return ( pLhs.mRefSettings != pRhs.mRefSettings ) || ( pLhs.mFlags != pRhs.mFlags );
	}

	inline constexpr DisplayVideoSettingsFilter cvDisplayVideoSettingsFilterNone { cvDisplayVideoSettingsEmpty, 0U };

	struct DisplayAdapterDesc
	{
		EDisplayDriverType mDriverType = EDisplayDriverType::Unknown;
		dsm_index_t mAdapterIndex = CX_DSM_INDEX_INVALID;
		EDisplayAdapterVendorID mVendorID = EDisplayAdapterVendorID::Unknown;
		TBitmask<EDisplayAdapterFlags> mFlags = 0u;
		std::string mName;

		std::string toString() const;
	};

	struct DisplayOutputDesc
	{
		EDisplayDriverType mDriverType = EDisplayDriverType::Unknown;
		dsm_index_t mOutputIndex = CX_DSM_INDEX_INVALID;
		dsm_output_id_t mOutputID = 0u;
		TBitmask<EDisplayOutputFlags> mFlags = 0u;
		std::string mName;
		ScreenRect mScreenRect;

		std::string toString() const;
	};

	struct DisplayVideoModeDesc
	{
		EDisplayDriverType mDriverType = EDisplayDriverType::Unknown;
		dsm_index_t mVideoModeIndex = CX_DSM_INDEX_INVALID;
		dsm_video_mode_id_t mVideoModeID = 0u;
		dsm_video_settings_hash_t mSettingsHash = 0u;
		EColorFormat mColorFormat = EColorFormat::Unknown;
		DisplayVideoSettings mSettings;

		std::string toString() const;
	};

	IC3_ATTR_NO_DISCARD dsm_output_id_t dsmCreateDisplayOutputID( dsm_index_t pAdapterIndex, dsm_index_t pOutputIndex );

	IC3_ATTR_NO_DISCARD dsm_video_settings_hash_t dsmComputeVideoSettingsHash( EColorFormat pFormat, const DisplayVideoSettings & pSettings );

	IC3_ATTR_NO_DISCARD std::string dsmGetVideoSettingsString( EColorFormat pFormat, const DisplayVideoSettings & pSettings );

	IC3_ATTR_NO_DISCARD EDisplayAdapterVendorID dsmResolveAdapterVendorID( const std::string & pAdapterName );

	IC3_ATTR_NO_DISCARD bool dsmCheckSettingsFilterMatch( const DisplayVideoSettingsFilter & pFilter, const DisplayVideoSettings & pSettings );


} // namespace Ic3::System

#pragma pop

#if( IC3_PCL_COMPILER & IC3_PCL_COMPILER_CLANG )
#  pragma clang diagnostic pop
#elif( IC3_PCL_COMPILER & IC3_PCL_COMPILER_MSVC )
#  pragma warning( pop )
#elif( IC3_PCL_COMPILER & ( IC3_PCL_COMPILER_GCC | IC3_PCL_COMPILER_MINGW64 ) )
#  pragma GCC diagnostic pop
#endif

#endif // __IC3_SYSTEM_DISPLAY_COMMON_H__
