
#pragma once

#ifndef __IC3_CORELIB_DEBUG_LAYER_H__
#define __IC3_CORELIB_DEBUG_LAYER_H__

#include <cppx/prerequisites.h>
#include <cstdarg>

#if !defined( IC3_DEBUG_OUTPUT_ENABLE_EXTENDED_INFO )
#  if( IC3_DEBUG )
#    define IC3_DEBUG_OUTPUT_ENABLE_EXTENDED_INFO 1
#  else
#    define IC3_DEBUG_OUTPUT_ENABLE_EXTENDED_INFO 0
#  endif
#endif

#if( IC3_DEBUG_OUTPUT_ENABLE_EXTENDED_INFO )
#  if !defined( IC3_DEBUG_OUTPUT_INCLUDE_ORIGIN_LOCATION )
#    define IC3_DEBUG_OUTPUT_INCLUDE_ORIGIN_LOCATION 1
#  endif
#  if !defined( IC3_DEBUG_OUTPUT_INCLUDE_PREFIX_STR )
#    define IC3_DEBUG_OUTPUT_INCLUDE_PREFIX_STR 1
#  endif
#else
#  if defined( IC3_DEBUG_OUTPUT_INCLUDE_ORIGIN_LOCATION )
#    undef IC3_DEBUG_OUTPUT_INCLUDE_ORIGIN_LOCATION
#  endif
#  if defined( IC3_DEBUG_OUTPUT_INCLUDE_PREFIX_STR )
#    undef IC3_DEBUG_OUTPUT_INCLUDE_PREFIX_STR
#  endif
#endif

namespace Ic3
{

	enum EDebugMessageType : uint32
	{
		eDebugMTDefault,
		eDebugMTInfo,
		eDebugMTWarning,
		eDebugMTError,
		eDebugMTException
	};

	namespace Debug
	{

		IC3_CORELIB_API void DebugOutputExtImpl( EDebugMessageType pMessageType, const char * pPrefix, const char * pFormat, va_list pVarArgList );

		IC3_CORELIB_API void DebugOutputExtOriginImpl( const cppx::file_location_info & pPrintOrigin, EDebugMessageType pMessageType, const char * pPrefix, const char * pFormat, va_list pVarArgList );

		inline void DebugOutputForwarder( EDebugMessageType pMessageType, const char * pPrefix, const char * pFormat, ... )
		{
			va_list varArgsList;
			va_start( varArgsList, pFormat );

			DebugOutputExtImpl( pMessageType, pPrefix, pFormat, varArgsList );

			va_end( varArgsList );
		}

		inline void DebugOutputForwarderOrigin( const cppx::file_location_info & pPrintOrigin, EDebugMessageType pMessageType, const char * pPrefix, const char * pFormat, ... )
		{
			va_list varArgsList;
			va_start( varArgsList, pFormat );

			DebugOutputExtOriginImpl( pPrintOrigin, pMessageType, pPrefix, pFormat, varArgsList );

			va_end( varArgsList );
		}

	}

}

#define Ic3DebugAssert( pCondition )      cppx_debug_assert( pCondition )
#define Ic3DebugAssertOnce( pCondition )  cppx_debug_assert_once( pCondition )
#define Ic3DebugInterrupt()               cppx_debug_interrupt()
#define Ic3DebugInterruptOnce()           cppx_debug_interrupt_once()
#define Ic3DebugOnlyCode( pCode )         cppx_debug_only_code( pCode )

#if( !IC3_DEBUG_OUTPUT_ENABLE_EXTENDED_INFO )
#
# define Ic3DebugOutput( pText )            cppx_debug_output( pText )
# define Ic3DebugOutputFmt( pFormat, ... )  cppx_debug_output_fmt( pFormat, __VA_ARGS__ )
#
#else
#
#  if( IC3_DEBUG_OUTPUT_INCLUDE_ORIGIN_LOCATION )
#    define _Ic3DebugOutputForwarder( ... )  ::Ic3::Debug::DebugOutputForwarderOrigin( cppx_current_file_location_info(), __VA_ARGS__ )
#  else
#    define _Ic3DebugOutputForwarder( ... )  ::Ic3::Debug::DebugOutputForwarder( __VA_ARGS__ )
#  endif
#
#  if( IC3_DEBUG_OUTPUT_INCLUDE_PREFIX_STR )
#    define Ic3DebugOutput( pPrefix, ... )                  _Ic3DebugOutputForwarder( ::Ic3::eDebugMTDefault, pPrefix, __VA_ARGS__ )
#    define Ic3DebugOutputEx( pMessageType, pPrefix, ... )  _Ic3DebugOutputForwarder( pMessageType, pPrefix, __VA_ARGS__ )
#  else
#    define Ic3DebugOutput( pPrefix, ... )                  _Ic3DebugOutputForwarder( ::Ic3::eDebugMTDefault, nullptr, __VA_ARGS__ )
#    define Ic3DebugOutputEx( pMessageType, pPrefix, ... )  _Ic3DebugOutputForwarder( pMessageType, nullptr, __VA_ARGS__ )
#  endif
#
#endif

#endif // __IC3_CORELIB_DEBUG_LAYER_H__

