
#pragma once

#ifndef __STDX_VERSION_H__
#define __STDX_VERSION_H__

#include "typeLimits.h"

#define CPPX_VERSION_MAJOR    0
#define CPPX_VERSION_MINOR    0
#define CPPX_VERSION_RELEASE  0
#define CPPX_VERSION_BUILD    0

#define CPPX_VER_PRODUCTVERSION      0,0,0,0
#define CPPX_VER_PRODUCTVERSION_STR  "0.0.0.0"

#define CPPX_VER_COMPANY_STR      "Exeos Software Development"
#define CPPX_VER_COPYRIGHT_STR    "Copyright (c) 2020 Mateusz Grzejek"
#define CPPX_VER_PRODUCTNAME_STR  "Exeos Framework"

#if( PCL_COMPILER & PCL_COMPILER_CLANG )
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#  pragma clang diagnostic ignored "-Wnested-anon-types"
#elif( PCL_COMPILER & PCL_COMPILER_GCC )
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wpedantic"
#elif( PCL_COMPILER & PCL_COMPILER_MSVC )
#  pragma warning( push )
#  pragma warning( disable: 4201 )  // 'Nonstandard extension used: nameless struct/union'
#endif

namespace cppx
{

	/// @brief Representation of a version in 'major.mNumMinor' format.
	union version
	{
	public:
		struct
		{
			/// Major component of the version number.
			uint16 num_major;

			/// Minor component of the version number.
			uint16 num_minor;
		};

		/// 32-bit hash of the version, which is assembled by combining both uint16 components into a single 32-bit value.
		uint32 u32_hash;

	public:
		/// @brief Returns the string representation of the version.
		/// @return The string representation of the version, formatted as "major.mNumMinor".
		CPPX_ATTR_NO_DISCARD std::string to_string() const
		{
			return std::to_string( num_major ) + "." + std::to_string( num_minor );
		}
	};

	namespace cve
	{

		/// @brief An invalid version constant - both major and minor components are set to UINT16_MAX.
		inline constexpr version version_invalid{ uint16_max, uint16_max };

		/// @brief An unknown version constant - both major and minor components are set to 0.
		inline constexpr version version_unknown{ 0, 0 };

	}

	inline bool operator==( const version & pLhs, const version & pRhs )
	{
		return ( pLhs.num_major == pRhs.num_major ) && ( pLhs.num_minor == pRhs.num_minor );
	}

	inline bool operator!=( const version & pLhs, const version & pRhs )
	{
		return ( pLhs.num_major != pRhs.num_major ) || ( pLhs.num_minor != pRhs.num_minor );
	}

	inline bool operator>( const version & pLhs, const version & pRhs )
	{
		return ( pLhs.num_major > pRhs.num_major ) || ( ( pLhs.num_major == pRhs.num_major ) && ( pLhs.num_minor > pRhs.num_minor ) );
	}

	inline bool operator>=( const version & pLhs, const version & pRhs )
	{
		return ( pLhs > pRhs ) || ( pLhs == pRhs );
	}

	inline bool operator<( const version & pLhs, const version & pRhs )
	{
		return ( pLhs.num_major < pRhs.num_major ) || ( ( pLhs.num_major == pRhs.num_major ) && ( pLhs.num_minor < pRhs.num_minor ) );
	}

	inline bool operator<=( const version & pLhs, const version & pRhs )
	{
		return ( pLhs < pRhs ) || ( pLhs == pRhs );
	}

}

#if( PCL_COMPILER & PCL_COMPILER_CLANG )
#  pragma clang diagnostic pop
#elif( PCL_COMPILER & PCL_COMPILER_GCC )
#  pragma GCC diagnostic pop
#elif( PCL_COMPILER & PCL_COMPILER_MSVC )
#  pragma warning( pop )
#endif

#endif // __STDX_VERSION_H__
