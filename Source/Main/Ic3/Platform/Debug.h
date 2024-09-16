
#pragma once

#ifndef __IC3_PLATFORM_DEBUG_H__
#define __IC3_PLATFORM_DEBUG_H__

#include "FileLocationInfo.h"
#include <cstdarg>

#if !defined( IC3_CONFIG_CORE_ENABLE_DEBUG_LAYER )
#  if( IC3_PCL_BUILD_TYPE_DEBUG )
#    define IC3_CONFIG_CORE_ENABLE_DEBUG_LAYER 1
#  else
#    define IC3_CONFIG_CORE_ENABLE_DEBUG_LAYER 0
#  endif
#endif

#if !defined( IC3_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE )
#  define IC3_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE 0
#endif

namespace Ic3
{
	
	/// @def ic3DebugAssert
	/// @brief Issues a debug interrupt if specified condition is not met. Usually implemented via { asm int 3 } or its equivalent.
	/// If IC3_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE is enabled and _ic3DebugAssertionFailHandler is defined,
	/// provided handler is used instead.

	/// @def ic3DebugAssertOnce
	/// @brief Issues a debug interrupt if specified condition is not met. Interruption is triggered only once, at the first condition failure.
	/// If IC3_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE is enabled and _ic3DebugAssertionFailHandler is defined,
	/// provided handler is used instead.

	/// @def ic3DebugAssertEx
	/// @brief Similar to ic3DebugAssert, but uses explicitly specified handler to handle a condition failure.
	/// If _ic3DebugAssertionFailHandler is defined, provided handler is used instead.

	/// @def ic3DebugAssertOnceEx
	/// @brief Similar to ic3DebugAssertOnce, but uses explicitly specified handler to handle a condition failure.
	/// If _ic3DebugAssertionFailHandler is defined, provided handler is used instead.

	/// @def ic3DebugInterrupt
	/// @brief Issues a debug interrupt. Usually implemented via { asm int 3 } or its equivalent.
	/// If IC3_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE is enabled and _ic3DebugInterruptHandler is defined,
	/// provided handler is used instead.

	/// @def ic3DebugInterruptOnce
	/// @brief Issues a debug interrupt. Interruption is triggered only once, at the first condition failure.
	/// If IC3_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE is enabled and _ic3DebugInterruptHandler is defined,
	/// provided handler is used instead.

	/// @def ic3DebugInterruptEx
	/// @brief Similar to ic3DebugInterrupt, but uses explicitly specified handler to handle a condition failure.
	/// If IC3_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE is enabled and _ic3DebugInterruptHandler is defined,
	/// provided handler is used instead.

	/// @def ic3DebugInterruptOnceEx
	/// @brief Similar to ic3DebugInterruptOnce, but uses explicitly specified handler to handle a condition failure.
	/// If IC3_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE is enabled and _ic3DebugInterruptHandler is defined,
	/// provided handler is used instead.

	/// @def ic3DebugOutput
	/// @brief Prints formatted output to the default debug output stream.
	/// If IC3_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE is enabled and _ic3DebugPrintHandler is defined,
	/// provided handler is used instead.

	/// @def ic3DebugOutputFmt
	/// @brief Prints formatted output to the default debug output stream.
	/// If IC3_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE is enabled and _ic3DebugPrintHandler is defined,
	/// provided handler is used instead.

	/// @def ic3DebugOutputEx
	/// @brief Similar to ic3DebugOutput, but uses explicitly specified handler to handle a condition failure.
	/// If IC3_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE is enabled and _ic3DebugPrintHandler is defined,
	/// provided handler is used instead.

	/// @def ic3DebugOutputExFmt
	/// @brief Similar to ic3DebugOutput, but uses explicitly specified handler to handle a condition failure.
	/// If IC3_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE is enabled and _ic3DebugPrintHandler is defined,
	/// provided handler is used instead.

	/// @def ic3DebugOnlyCode
	/// @brief ToDo

#if( IC3_CONFIG_CORE_ENABLE_DEBUG_LAYER )
#  define ic3DebugAssert( pCondition )                   ic3DebugAssertImpl( pCondition )
#  define ic3DebugAssertOnce( pCondition )               ic3DebugAssertOnceImpl( pCondition )
#  define ic3DebugAssertEx( pHandler, pCondition )       ic3DebugAssertExImpl( pHandler, pCondition )
#  define ic3DebugAssertOnceEx( pHandler, pCondition )   ic3DebugAssertOnceExImpl( pHandler, pCondition )
#  define ic3DebugInterrupt()                            ic3DebugInterruptImpl()
#  define ic3DebugInterruptOnce()                        ic3DebugInterruptOnceImpl()
#  define ic3DebugInterruptEx( pHandler )                ic3DebugInterruptExImpl( pHandler )
#  define ic3DebugInterruptOnceEx( pHandler )            ic3DebugInterruptOnceExImpl( pHandler )
#  define ic3DebugOutput( pText )                        ic3DebugOutputImpl( pText )
#  define ic3DebugOutputFmt( pFormat, ... )              ic3DebugOutputFmtImpl( pFormat, ... )
#  define ic3DebugOutputEx( pHandler, pText )            ic3DebugOutputExImpl( pHandler, pText )
#  define ic3DebugOutputExFmt( pHandler, pFormat, ... )  ic3DebugOutputExFmtImpl( pHandler, pFormat, ... )
#else
#  define ic3DebugAssert( pCondition )
#  define ic3DebugAssertOnce( pCondition )
#  define ic3DebugAssertEx( pHandler, pCondition )
#  define ic3DebugAssertOnceEx( pHandler, pCondition )
#  define ic3DebugInterrupt()
#  define ic3DebugInterruptOnce()
#  define ic3DebugInterruptEx( pHandler )
#  define ic3DebugInterruptOnceEx( pHandler )
#  define ic3DebugOutput( pText )
#  define ic3DebugOutputFmt( pFormat, ... )
#  define ic3DebugOutputEx( pHandler, pText )
#  define ic3DebugOutputExFmt( pHandler, pFormat, ... )
#endif

	using DebugPrintHandlerType = void(*)( const char *, size_t );
	using DebugAssertionFailHandlerType = void(*)( const FileLocationInfo &, const char * );
	using DebugInterruptHandlerType = void(*)( const FileLocationInfo & );

#if( IC3_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE )

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


	#define ic3DebugAssertImpl( pCondition ) \
		{ \
			if(!(pCondition)) \
			{ \
				::Ic3::DebugInterface::assertionFail( _ic3DebugAssertionFailHandler, ic3CurrentFileLocationInfo(), #pCondition ); \
			} \
		}

	#define ic3DebugAssertOnceImpl( pCondition ) \
		{ \
			static bool cFlag = false; \
			if( !cFlag && !(pCondition) ) \
			{ \
				::Ic3::DebugInterface::assertionFail( _ic3DebugAssertionFailHandler, ic3CurrentFileLocationInfo(), #pCondition ); \
				cFlag = true; \
			} \
		}

	#define ic3DebugAssertExImpl( pHandler, pCondition ) \
		{ \
			if(!(pCondition)) \
			{ \
				::Ic3::DebugInterface::assertionFail( pHandler, ic3CurrentFileLocationInfo(), #pCondition ); \
			} \
		}

	#define ic3DebugAssertOnceExImpl( pHandler, pCondition ) \
		{ \
			static bool cFlag = false; \
			if( !cFlag && !(pCondition) ) \
			{ \
				::Ic3::DebugInterface::assertionFail( pHandler, ic3CurrentFileLocationInfo(), #pCondition ); \
				cFlag = true; \
			} \
		}

	#define ic3DebugInterruptImpl() \
		{ \
			::Ic3::DebugInterface::interrupt( _ic3DebugInterruptHandler, ic3CurrentFileLocationInfo() ); \
		}

	#define ic3DebugInterruptOnceImpl() \
		{ \
			static bool cFlag = false; \
			if( !cFlag ) \
			{ \
				::Ic3::DebugInterface::interrupt( _ic3DebugInterruptHandler, ic3CurrentFileLocationInfo() ); \
				cFlag = true; \
			} \
		}

	#define ic3DebugInterruptExImpl( pHandler ) \
		{ \
			::Ic3::DebugInterface::interrupt( pHandler, ic3CurrentFileLocationInfo() ); \
		}

	#define ic3DebugInterruptOnceExImpl( pHandler ) \
		{ \
			static bool cFlag = false; \
			if( !cFlag ) \
			{ \
				::Ic3::DebugInterface::interrupt( pHandler, ic3CurrentFileLocationInfo() ); \
				cFlag = true; \
			} \
		}

	#define ic3DebugOutputImpl( pText ) \
		::Ic3::DebugInterface::printDebug( _ic3DebugPrintHandler, "%s", pText  )

	#define ic3DebugOutputFmtImpl( pFormat, ... ) \
		::Ic3::DebugInterface::printDebug( _ic3DebugPrintHandler, pFormat, __VA_ARGS__  )

	#define ic3DebugOutputExImpl( pHandler, pText ) \
		::Ic3::DebugInterface::printDebug( pHandler, "%s", pText  )

	#define ic3DebugOutputExFmtImpl( pHandler, pFormat, ... ) \
		::Ic3::DebugInterface::printDebug( pHandler, pFormat, __VA_ARGS__  )

#else

	namespace Impl
	{

		void debugOutputImpl( DebugPrintHandlerType pHandler, const char * pFormat, ... );

		void debugDefaultPrintHandler( const char * pText, size_t pLength );

	}

	#define ic3DebugAssertImpl( pCondition ) \
		{ \
			if(!(pCondition)) \
			{ \
				IC3_PCL_DEBUG_BREAK(); \
			} \
		}

	#define ic3DebugAssertOnceImpl( pCondition ) \
		{ \
			static bool cFlag = false; \
			if( !cFlag && !(pCondition) ) \
			{ \
				IC3_PCL_DEBUG_BREAK(); \
				cFlag = true; \
			} \
		}

	#define ic3DebugAssertExImpl( pHandler, pCondition ) \
		{ \
			if(!(pCondition)) \
			{ \
				pHandler(); \
			} \
		}

	#define ic3DebugAssertOnceExImpl( pHandler, pCondition ) \
		{ \
			static bool cFlag = false; \
			if( !cFlag && !(pCondition) ) \
			{ \
				pHandler(); \
				cFlag = true; \
			} \
		}

	#define ic3DebugInterruptImpl() \
		{ \
			IC3_PCL_DEBUG_BREAK(); \
		}

	#define ic3DebugInterruptOnceImpl() \
		{ \
			static bool cFlag = false; \
			if( !cFlag ) \
			{ \
				IC3_PCL_DEBUG_BREAK(); \
				cFlag = true; \
			} \
		}

	#define ic3DebugInterruptExImpl( pHandler ) \
		{ \
			pHandler(); \
		}

	#define ic3DebugInterruptOnceExImpl( pHandler ) \
		{ \
			static bool cFlag = false; \
			if( !cFlag ) \
			{ \
				pHandler(); \
				cFlag = true; \
			} \
		}

	#define ic3DebugOutputImpl( pText ) \
		::Ic3::Impl::debugOutputImpl( ::Ic3::Impl::debugDefaultPrintHandler, "%s", pText  )

	#define ic3DebugOutputFmtImpl( pFormat, ... ) \
		::Ic3::Impl::debugOutputImpl( ::Ic3::Impl::debugDefaultPrintHandler, pFormat, __VA_ARGS__  )

	#define ic3DebugOutputExImpl( pHandler, pText ) \
		::Ic3::Impl::debugOutputImpl( pHandler, "%s", pText  )

	#define ic3DebugOutputExFmtImpl( pHandler, pFormat, ... ) \
		::Ic3::Impl::debugOutputImpl( pHandler, pFormat, __VA_ARGS__  )

#endif // IC3_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE



}

#endif // __IC3_PLATFORM_DEBUG_H__
