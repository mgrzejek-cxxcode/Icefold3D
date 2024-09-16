
#pragma once

#ifndef __STDX_VERSION_H__
#define __STDX_VERSION_H__

#include "Prerequisites.h"

#define IC3_VERSION_MAJOR    0
#define IC3_VERSION_MINOR    0
#define IC3_VERSION_RELEASE  0
#define IC3_VERSION_BUILD    0

#define IC3_VER_PRODUCTVERSION      0,0,0,0
#define IC3_VER_PRODUCTVERSION_STR  "0.0.0.0"

#define IC3_VER_COMPANY_STR      "Exeos Software Development"
#define IC3_VER_COPYRIGHT_STR    "Copyright (c) 2020 Mateusz Grzejek"
#define IC3_VER_PRODUCTNAME_STR  "Exeos Framework"

#if( IC3_PCL_COMPILER & IC3_PCL_COMPILER_CLANG )
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#  pragma clang diagnostic ignored "-Wnested-anon-types"
#elif( IC3_PCL_COMPILER & IC3_PCL_COMPILER_GCC )
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wpedantic"
#elif( IC3_PCL_COMPILER & IC3_PCL_COMPILER_MSVC )
#  pragma warning( push )
#  pragma warning( disable: 4201 )  // 'Nonstandard extension used: nameless struct/union'
#endif

namespace Ic3
{

	/// @brief Representation of a version in 'major.mNumMinor' format.
	union Version
	{
	public:
		struct
		{
			/// Major component of the version number.
			uint16 mNumMajor;

			/// Minor component of the version number.
			uint16 mNumMinor;
		};

		/// 32-bit hash of the version, which is assembled by combining both uint16 components into a single 32-bit value.
		uint32 mU32Hash;

	public:
		/// @brief Returns the string representation of the version.
		/// @return The string representation of the version, formatted as "major.mNumMinor".
		IC3_ATTR_NO_DISCARD std::string toString() const
		{
			return std::to_string( mNumMajor ) + "." + std::to_string( mNumMinor );
		}
	};

	/// @brief An invalid version constant - both major and minor components are set to UINT16_MAX.
	inline constexpr Version CX_VERSION_INVALID{Cppx::cxUint16Max, Cppx::cxUint16Max };

	/// @brief An unknown version constant - both major and minor components are set to 0.
	inline constexpr Version CX_VERSION_UNKNOWN{ 0, 0 };

	inline bool operator==( const Version & pLhs, const Version & pRhs )
	{
		return ( pLhs.mNumMajor == pRhs.mNumMajor ) && ( pLhs.mNumMinor == pRhs.mNumMinor );
	}

	inline bool operator!=( const Version & pLhs, const Version & pRhs )
	{
		return ( pLhs.mNumMajor != pRhs.mNumMajor ) || ( pLhs.mNumMinor != pRhs.mNumMinor );
	}

	inline bool operator>( const Version & pLhs, const Version & pRhs )
	{
		return ( pLhs.mNumMajor > pRhs.mNumMajor ) || ( ( pLhs.mNumMajor == pRhs.mNumMajor ) && ( pLhs.mNumMinor > pRhs.mNumMinor ) );
	}

	inline bool operator>=( const Version & pLhs, const Version & pRhs )
	{
		return ( pLhs > pRhs ) || ( pLhs == pRhs );
	}

	inline bool operator<( const Version & pLhs, const Version & pRhs )
	{
		return ( pLhs.mNumMajor < pRhs.mNumMajor ) || ( ( pLhs.mNumMajor == pRhs.mNumMajor ) && ( pLhs.mNumMinor < pRhs.mNumMinor ) );
	}

	inline bool operator<=( const Version & pLhs, const Version & pRhs )
	{
		return ( pLhs < pRhs ) || ( pLhs == pRhs );
	}

}

#if( IC3_PCL_COMPILER & IC3_PCL_COMPILER_CLANG )
#  pragma clang diagnostic pop
#elif( IC3_PCL_COMPILER & IC3_PCL_COMPILER_GCC )
#  pragma GCC diagnostic pop
#elif( IC3_PCL_COMPILER & IC3_PCL_COMPILER_MSVC )
#  pragma warning( pop )
#endif

#endif // __STDX_VERSION_H__
