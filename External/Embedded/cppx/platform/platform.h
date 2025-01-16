
#ifndef __CPPX_PLATFORM_CONFIG_PLATFORM_H__
#define __CPPX_PLATFORM_CONFIG_PLATFORM_H__

#include "internal/environment.h"
#include "internal/include.h"
#include "internal/types.h"
#include "internal/simdExt.h"

/// @def PCL_DEBUG
/// @brief Identifies a debug build. Enables additional logging, build-time info, extra API checks and more.
#if( PCL_BUILD_TYPE == PCL_BUILD_TYPE_DEBUG )
#  define PCL_DEBUG 1
#else
#  define PCL_DEBUG 0
#endif

/// @def CPPX_RELEASE_OPT_MAX
/// @brief Identifies a production-oriented build, with all possible code-level optimisations enabled. Disabled by default.
#if !defined( CPPX_RELEASE_OPT_MAX )
#  define CPPX_RELEASE_OPT_MAX 0
#endif

/// @def CPPX_ATTR_MAYBE_UNUSED
/// @brief Replaced with compiler-specific version (or the standard one - if supported) [[maybe_unused]] class attribute.
#define CPPX_ATTR_MAYBE_UNUSED \
	PCL_ATTR_MAYBE_UNUSED

/// @def CPPX_ATTR_NO_DISCARD
/// @brief Replaced with compiler-specific version (or the standard one - if supported) [[nodiscard]] function attribute.
#define CPPX_ATTR_NO_DISCARD \
	PCL_ATTR_FUNC_NO_DISCARD

#define CPPX_DEFAULT_COPYABLE( pClassName ) \
	pClassName( const pClassName & ) = default; \
	pClassName & operator=( const pClassName & ) = default;

#define CPPX_DEFAULT_MOVABLE( pClassName ) \
	pClassName( pClassName && ) = default; \
	pClassName & operator=( pClassName && ) = default;

#define CPPX_NON_COPYABLE( pClassName ) \
	pClassName( const pClassName & ) = delete; \
	pClassName & operator=( const pClassName & ) = delete;

#define CPPX_NON_MOVABLE( pClassName ) \
	pClassName( pClassName && ) = delete; \
	pClassName & operator=( pClassName && ) = delete;

namespace cppx
{

	/// @brief Represents one of the possible byte orders in binary data representation.
	enum class byte_order : uint32
	{
		/// Big-Endian order: MSB comes first.
		big_endian = PCL_ENDIANNESS_BE,

		/// Little-Endian order: LSB comes first.
		little_endian = PCL_ENDIANNESS_LE,

		/// PDP-Endian order: LSB comes first in word, MSW comes first in DWORD..
		pdp_endian = PCL_ENDIANNESS_PDP,

		/// The native byte order for the current target architecture (e.g. Big-Endian for ARM, Little-Endian for x86 family).
		platform_native = PCL_ENDIANNESS_NATIVE,

		/// A default byte order used by the framework. This is always Big-Endian as it is the default one for network.
		lib_default = static_cast<uint32>( big_endian )
	};

}

#endif /* __CPPX_PLATFORM_CONFIG_PLATFORM_H__ */
