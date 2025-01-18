
#pragma once

#ifndef __CPPX_PLATFORM_DEBUG_H__
#define __CPPX_PLATFORM_DEBUG_H__

#include <cstdarg>

#if !defined( CPPX_CONFIG_CORE_ENABLE_DEBUG )
#  if( PCL_BUILD_TYPE_DEBUG )
#    define CPPX_CONFIG_CORE_ENABLE_DEBUG 1
#  else
#    define CPPX_CONFIG_CORE_ENABLE_DEBUG 0
#  endif
#endif

namespace cppx
{

	namespace impl
	{

		void debug_output_impl( const char * pFormat, ... );

		void debug_default_print_handler( const char * pText, size_t pLength );

	}

} // namespace cppx
	
/// @def cppx_debug_assert
/// @brief Issues a debug interrupt if specified condition is not met. Usually implemented via { asm int 3 } or its equivalent.

/// @def cppx_debug_assert_once
/// @brief Issues a debug interrupt if specified condition is not met. Interruption is triggered only once, at the first condition failure.

/// @def cppx_debug_interrupt
/// @brief Issues a debug interrupt. Usually implemented via { asm int 3 } or its equivalent.

/// @def cppx_debug_interrupt_once
/// @brief Issues a debug interrupt. Interruption is triggered only once, at the first condition failure.

/// @def cppx_debug_output
/// @brief Prints formatted output to the default debug output stream.

/// @def cppx_debug_output_fmt
/// @brief Prints formatted output to the default debug output stream.

/// @def cppx_debug_only_code
/// @brief TODO

#define cppx_debug_assert_impl( pCondition ) \
	{ if(!(pCondition)) { PCL_DEBUG_BREAK(); } }

#define cppx_debug_assert_once_impl( pCondition ) \
	{ static bool cFlag = false; if( !cFlag && !(pCondition) ) { PCL_DEBUG_BREAK(); cFlag = true; } }

#define cppx_debug_interrupt_impl() \
	{ PCL_DEBUG_BREAK(); }

#define cppx_debug_interrupt_once_impl() \
	{ static bool cFlag = false; if( !cFlag ) { PCL_DEBUG_BREAK(); cFlag = true; } }

#define cppx_debug_output_impl( pText ) \
	::cppx::impl::debug_output_impl( "%s", pText  )

#define cppx_debug_output_fmt_impl( pFormat, ... ) \
	::cppx::impl::debug_output_impl( pFormat, __VA_ARGS__  )

#if( CPPX_CONFIG_CORE_ENABLE_DEBUG )
#  define cppx_debug_assert( pCondition )        cppx_debug_assert_impl( pCondition )
#  define cppx_debug_assert_once( pCondition )   cppx_debug_assert_once_impl( pCondition )
#  define cppx_debug_interrupt()                 cppx_debug_interrupt_impl()
#  define cppx_debug_interrupt_once()            cppx_debug_interrupt_once_impl()
#  define cppx_debug_output( pText )             cppx_debug_output_impl( pText )
#  define cppx_debug_output_fmt( pFormat, ... )  cppx_debug_output_fmt_impl( pFormat, __VA_ARGS__ )
#  define cppx_debug_only_code( pCode )          pCode
#else
#  define cppx_debug_assert( pCondition )
#  define cppx_debug_assert_once( pCondition )
#  define cppx_debug_interrupt()
#  define cppx_debug_interrupt_once()
#  define cppx_debug_output( pText )
#  define cppx_debug_output_fmt( pFormat, ... )
#  define cppx_debug_only_code( pCode )
#endif

#endif // __CPPX_PLATFORM_DEBUG_H__
