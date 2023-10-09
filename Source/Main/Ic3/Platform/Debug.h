
#pragma once

#ifndef __IC3_PLATFORM_DEBUG_H__
#define __IC3_PLATFORM_DEBUG_H__

#include "FileLocationInfo.h"
#include <cstdarg>

namespace Ic3
{

	using DebugAssertionFailHandlerType = void(*)( const FileLocationInfo &, const char * );
	using DebugInterruptHandlerType = void(*)( const FileLocationInfo & );
	using DebugPrintHandlerType = void(*)( const char *, size_t );

	// The three macros defined below allow to override the default handlers used for standard debug macros.
	// Obviously, this works at build time and is useful when building a customized version of the engine.
	// Also, since this doesn't change the already built engine modules, re-definition can enable different
	// behaviour for the target app/game while keeping the default one for the engine itself.
	// - _ic3DebugAssertionFailHandler must match the signature of DebugAssertionFailHandlerType
	// - _ic3DebugInterruptHandler must match the signature of DebugInterruptHandlerType
	// - _ic3DebugPrintHandler must match the signature of DebugPrintHandlerType

	#if !defined( _ic3DebugAssertionFailHandler )
	// Custom handler for the assertion fail macro.
	#  define _ic3DebugAssertionFailHandler nullptr
	#endif

	#if !defined( _ic3DebugInterruptHandler )
	// Custom handler for the interrupt macro.
	#  define _ic3DebugInterruptHandler nullptr
	#endif

	#if !defined( _ic3DebugPrintHandler )
	// Custom handler for the print macro.
	#  define _ic3DebugPrintHandler nullptr
	#endif


	/// @brief
	class DebugInterface
	{
	public:
		/// @brief Called when an interrupt is triggered. Breaks execution using provided mechanism (default or user-specified).
		static void assertionFail( DebugAssertionFailHandlerType pHandler, const FileLocationInfo & pLocationInfo, const char * pConditionStr );

		/// @brief Called when an interrupt is triggered. Breaks execution using provided mechanism (default or user-specified).
		static void interrupt( DebugInterruptHandlerType pHandler, const FileLocationInfo & pLocationInfo );

		/// @brief Handles the print forwarding and vararg handling.
		static void printDebug( DebugPrintHandlerType pHandler, const char * pFormat, ... );

	private:
		// Default assertion handler: prints assertion info using IC3_PCL_DEBUG_OUTPUT() and then triggers an interrupt with IC3_PCL_DEBUG_BREAK().
		static void _defaultAssertionFailHandler( const FileLocationInfo & pLocationInfo, const char * pConditionStr );

		// Default interrupt handler: triggers an interrupt with IC3_PCL_DEBUG_BREAK().
		static void _defaultInterruptHandler( const FileLocationInfo & pLocationInfo );

		// Default print handler: prints specified text with IC3_PCL_DEBUG_OUTPUT().
		static void _defaultPrintHandler( const char * pText, size_t pLength );
	};


	/// @brief Issues a debug interrupt if provided condition is not met. Usually implemented via { asm int 3 } or its equivalent.
	/// If _ic3DebugAssertionFailHandler is defined, provided handler is used instead.
	#define ic3DebugAssert( pCondition ) \
		{ \
			if(!(pCondition)) \
			{ \
				::Ic3::DebugInterface::assertionFail( _ic3DebugAssertionFailHandler, ic3CurrentFileLocationInfo(), #pCondition ); \
			} \
		}

	/// @brief Issues a debug interrupt if provided condition is not met. Interruption is triggered only once, at the first condition failure.
	/// If _ic3DebugAssertionFailHandler is defined, provided handler is used instead.
	#define ic3DebugAssertOnce( pCondition ) \
		{ \
			static bool cFlag = false; \
			if( !cFlag && !(pCondition) ) \
			{ \
				::Ic3::DebugInterface::assertionFail( _ic3DebugAssertionFailHandler, ic3CurrentFileLocationInfo(), #pCondition ); \
				cFlag = true; \
			} \
		}

	/// @brief Issues a debug interrupt. Usually implemented via { asm int 3 } or its equivalent.
	/// If _ic3DebugInterruptHandler is defined, provided handler is used instead.
	#define ic3DebugInterrupt() \
		{ \
			::Ic3::DebugInterface::interrupt( _ic3DebugInterruptHandler, ic3CurrentFileLocationInfo() ); \
		}

	/// @brief Issues a debug interrupt. Interruption is triggered only once, at the first condition failure.
	/// If _ic3DebugInterruptHandler is defined, provided handler is used instead.
	#define ic3DebugInterruptOnce() \
		{ \
			static bool cFlag = false; \
			if( !cFlag ) \
			{ \
				::Ic3::DebugInterface::interrupt( _ic3DebugInterruptHandler, ic3CurrentFileLocationInfo() ); \
				cFlag = true; \
			} \
		}

	/// @brief Prints formatted output to the default debug output stream.
	/// If _ic3DebugPrintHandler is defined, provided handler is used instead.
	#define ic3DebugOutput( pText ) \
		::Ic3::DebugInterface::printDebug( _ic3DebugPrintHandler, "%s", pText  )

	/// @brief Prints formatted output to the default debug output stream.
	/// If _ic3DebugPrintHandler is defined, provided handler is used instead.
	#define ic3DebugOutputFmt( pFormat, ... ) \
		::Ic3::DebugInterface::printDebug( _ic3DebugPrintHandler, pFormat, __VA_ARGS__  )


	/// @brief Similar to ic3DebugAssert, but uses explicitly specified handler to handle a condition failure.
	/// If _ic3DebugAssertionFailHandler is defined, provided handler is used instead.
	#define ic3DebugAssertEx( pHandler, pCondition ) \
		{ \
			if(!(pCondition)) \
			{ \
				::Ic3::DebugInterface::assertionFail( pHandler, ic3CurrentFileLocationInfo(), #pCondition ); \
			} \
		}

	/// @brief Similar to ic3DebugAssertOnce, but uses explicitly specified handler to handle a condition failure.
	/// If _ic3DebugAssertionFailHandler is defined, provided handler is used instead.
	#define ic3DebugAssertOnceEx( pHandler, pCondition ) \
		{ \
			static bool cFlag = false; \
			if( !cFlag && !(pCondition) ) \
			{ \
				::Ic3::DebugInterface::assertionFail( pHandler, ic3CurrentFileLocationInfo(), #pCondition ); \
				cFlag = true; \
			} \
		}

	/// @brief Similar to ic3DebugInterrupt, but uses explicitly specified handler to handle a condition failure.
	/// If _ic3DebugInterruptHandler is defined, provided handler is used instead.
	#define ic3DebugInterruptEx( pHandler ) \
		{ \
			::Ic3::DebugInterface::interrupt( pHandler, ic3CurrentFileLocationInfo() ); \
		}

	/// @brief Similar to ic3DebugInterruptOnce, but uses explicitly specified handler to handle a condition failure.
	/// If _ic3DebugInterruptHandler is defined, provided handler is used instead.
	#define ic3DebugInterruptOnceEx( pHandler ) \
		{ \
			static bool cFlag = false; \
			if( !cFlag ) \
			{ \
				::Ic3::DebugInterface::interrupt( pHandler, ic3CurrentFileLocationInfo() ); \
				cFlag = true; \
			} \
		}

	/// @brief Similar to ic3DebugOutput, but uses explicitly specified handler to handle a condition failure.
	/// If _ic3DebugPrintHandler is defined, provided handler is used instead.
	#define ic3DebugOutputEx( pHandler, pText ) \
		::Ic3::DebugInterface::printDebug( pHandler, "%s", pText  )

	/// @brief Similar to ic3DebugOutput, but uses explicitly specified handler to handle a condition failure.
	/// If _ic3DebugPrintHandler is defined, provided handler is used instead.
	#define ic3DebugOutputExFmt( pHandler, pFormat, ... ) \
		::Ic3::DebugInterface::printDebug( pHandler, pFormat, __VA_ARGS__  )

}

#endif // __IC3_PLATFORM_DEBUG_H__
