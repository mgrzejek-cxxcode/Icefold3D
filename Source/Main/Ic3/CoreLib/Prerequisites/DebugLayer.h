
#pragma once

#ifndef __IC3_CORELIB_DEBUG_LAYER_H__
#define __IC3_CORELIB_DEBUG_LAYER_H__

#include <cppx/prerequisites.h>
#include <cstdarg>

#define Ic3DebugAssert( pCondition )       cppx_debug_assert( pCondition )
#define Ic3DebugAssertOnce( pCondition )   cppx_debug_assert_once( pCondition )
#define Ic3DebugInterrupt()                cppx_debug_interrupt()
#define Ic3DebugInterruptOnce()            cppx_debug_interrupt_once()
#define Ic3DebugOutput( pText )            cppx_debug_output( pText )
#define Ic3DebugOutputFmt( pFormat, ... )  cppx_debug_output_fmt( pFormat, __VA_ARGS__ )
#define Ic3DebugOnlyCode( pCode )          pCode

namespace Ic3
{

// #if( CPPX_CONFIG_CORE_ENABLE_DEBUG_LAYER )
// #  define Ic3DebugAssert( pCondition )                   Ic3DebugAssertImpl( pCondition )
// #  define Ic3DebugAssertOnce( pCondition )               Ic3DebugAssertOnceImpl( pCondition )
// #  define Ic3DebugAssertEx( pHandler, pCondition )       Ic3DebugAssertExImpl( pHandler, pCondition )
// #  define Ic3DebugAssertOnceEx( pHandler, pCondition )   Ic3DebugAssertOnceExImpl( pHandler, pCondition )
// #  define Ic3DebugInterrupt()                            Ic3DebugInterruptImpl()
// #  define Ic3DebugInterruptOnce()                        Ic3DebugInterruptOnceImpl()
// #  define Ic3DebugInterruptEx( pHandler )                Ic3DebugInterruptExImpl( pHandler )
// #  define Ic3DebugInterruptOnceEx( pHandler )            Ic3DebugInterruptOnceExImpl( pHandler )
// #  define Ic3DebugOutput( pText )                        Ic3DebugOutputImpl( pText )
// #  define Ic3DebugOutputFmt( pFormat, ... )              Ic3DebugOutputFmtImpl( pFormat, __VA_ARGS__ )
// #  define Ic3DebugOutputEx( pHandler, pText )            Ic3DebugOutputExImpl( pHandler, pText )
// #  define Ic3DebugOutputExFmt( pHandler, pFormat, ... )  Ic3DebugOutputExFmtImpl( pHandler, pFormat, __VA_ARGS__ )
// #  define Ic3DebugOnlyCode( pCode )                      pCode
// #else
// #  define Ic3DebugAssert( pCondition )
// #  define Ic3DebugAssertOnce( pCondition )
// #  define Ic3DebugAssertEx( pHandler, pCondition )
// #  define Ic3DebugAssertOnceEx( pHandler, pCondition )
// #  define Ic3DebugInterrupt()
// #  define Ic3DebugInterruptOnce()
// #  define Ic3DebugInterruptEx( pHandler )
// #  define Ic3DebugInterruptOnceEx( pHandler )
// #  define Ic3DebugOutput( pText )
// #  define Ic3DebugOutputFmt( pFormat, ... )
// #  define Ic3DebugOutputEx( pHandler, pText )
// #  define Ic3DebugOutputExFmt( pHandler, pFormat, ... )
// #  define Ic3DebugOnlyCode( pCode )
// #endif

}

#endif // __IC3_CORELIB_DEBUG_LAYER_H__
