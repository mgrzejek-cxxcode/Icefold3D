
#ifndef __IC3_PLATFORM_CONFIG_PLATFORM_H__
#define __IC3_PLATFORM_CONFIG_PLATFORM_H__

#include "Internal/Environment.h"
#include "Internal/Include.h"
#include "Internal/Types.h"
#include "Internal/SIMDExt.h"

/// @def IC3_DEBUG
/// @brief Identifies a debug build. Enables additional logging, build-time info, extra API checks and more.
#if( IC3_PCL_BUILD_TYPE == IC3_PCL_BUILD_TYPE_DEBUG )
#  define IC3_DEBUG 1
#else
#  define IC3_DEBUG 0
#endif

/// @def IC3_RELEASE_OPT_MAX
/// @brief Identifies a production-oriented build, with all possible code-level optimisations enabled. Disabled by default.
#if !defined( IC3_RELEASE_OPT_MAX )
#  define IC3_RELEASE_OPT_MAX 0
#endif

/// @def IC3_ATTR_MAYBE_UNUSED
/// @brief Replaced with compiler-specific version (or the standard one - if supported) [[maybe_unused]] class attribute.
#define IC3_ATTR_MAYBE_UNUSED \
	IC3_PCL_ATTR_MAYBE_UNUSED

/// @def IC3_ATTR_NO_DISCARD
/// @brief Replaced with compiler-specific version (or the standard one - if supported) [[nodiscard]] function attribute.
#define IC3_ATTR_NO_DISCARD \
	IC3_PCL_ATTR_FUNC_NO_DISCARD

#define IC3_DEFAULT_COPYABLE( pClassName ) \
	pClassName( const pClassName & ) = default; \
	pClassName & operator=( const pClassName & ) = default;

#define IC3_DEFAULT_MOVABLE( pClassName ) \
	pClassName( pClassName && ) = default; \
	pClassName & operator=( pClassName && ) = default;

#define IC3_NON_COPYABLE( pClassName ) \
	pClassName( const pClassName & ) = delete; \
	pClassName & operator=( const pClassName & ) = delete;

#define IC3_NON_MOVABLE( pClassName ) \
	pClassName( pClassName && ) = delete; \
	pClassName & operator=( pClassName && ) = delete;

namespace Ic3
{

	/// @brief Represents one of the possible byte orders in binary data representation.
	enum class EByteOrder : uint32
	{
		/// Big-Endian order: MSB comes first.
		BigEndian = IC3_PCL_ENDIANNESS_BE,

		/// Little-Endian order: LSB comes first.
		LittleEndian = IC3_PCL_ENDIANNESS_LE,

		/// PDP-Endian order: LSB comes first in word, MSW comes first in DWORD..
		PDPEndian = IC3_PCL_ENDIANNESS_PDP,

		/// The native byte order for the current target architecture (e.g. Big-Endian for ARM, Little-Endian for x86 family).
		PlatformNative = IC3_PCL_ENDIANNESS_NATIVE,

		/// A default byte order used by the framework. This is always Big-Endian as it is the default one for network.
		Default = static_cast<uint32>( BigEndian )
	};

	/// @brief Default alignment value for CPU-side (RAM) memory allocation.
	inline constexpr uint32 cxMemoryCPUDefaultAlignment = IC3_PCL_MEMORY_BASE_ALIGNMENT;

	/// @brief Default alignment value for GPU-side (VideoRAM) memory allocation.
	inline constexpr uint32 cxMemoryGPUDefaultAlignment = 64;

}

#endif /* __IC3_PLATFORM_CONFIG_PLATFORM_H__ */
