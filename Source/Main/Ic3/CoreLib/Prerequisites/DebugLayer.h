
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

#endif // __IC3_CORELIB_DEBUG_LAYER_H__
