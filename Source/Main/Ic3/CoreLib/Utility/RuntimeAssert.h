
#pragma once

#ifndef __IC3_CORELIB_RUNTIME_ASSERT_H__
#define __IC3_CORELIB_RUNTIME_ASSERT_H__

#include <cppx/prerequisites.h>
#include <cstdarg>

#define CPPX_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE 1

#if !defined( CPPX_CONFIG_CORE_ENABLE_DEBUG_LAYER )
#  if( PCL_BUILD_TYPE_DEBUG )
#    define CPPX_CONFIG_CORE_ENABLE_DEBUG_LAYER 1
#  else
#    define CPPX_CONFIG_CORE_ENABLE_DEBUG_LAYER 0
#  endif
#endif

#if !defined( CPPX_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE )
#  define CPPX_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE 0
#endif

namespace Ic3
{

	/// @def Ic3DebugAssert
	/// @brief Issues a debug interrupt if specified condition is not met. Usually implemented via { asm int 3 } or its equivalent.
	/// If CPPX_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE is enabled and _Ic3DebugAssertionFailHandler is defined,
	/// provided handler is used instead.

	/// @def Ic3DebugAssertOnce
	/// @brief Issues a debug interrupt if specified condition is not met. Interruption is triggered only once, at the first condition failure.
	/// If CPPX_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE is enabled and _Ic3DebugAssertionFailHandler is defined,
	/// provided handler is used instead.

	/// @def Ic3DebugAssertEx
	/// @brief Similar to Ic3DebugAssert, but uses explicitly specified handler to handle a condition failure.
	/// If _Ic3DebugAssertionFailHandler is defined, provided handler is used instead.

	/// @def Ic3DebugAssertOnceEx
	/// @brief Similar to Ic3DebugAssertOnce, but uses explicitly specified handler to handle a condition failure.
	/// If _Ic3DebugAssertionFailHandler is defined, provided handler is used instead.

	/// @def Ic3DebugInterrupt
	/// @brief Issues a debug interrupt. Usually implemented via { asm int 3 } or its equivalent.
	/// If CPPX_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE is enabled and _Ic3DebugInterruptHandler is defined,
	/// provided handler is used instead.

	/// @def Ic3DebugInterruptOnce
	/// @brief Issues a debug interrupt. Interruption is triggered only once, at the first condition failure.
	/// If CPPX_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE is enabled and _Ic3DebugInterruptHandler is defined,
	/// provided handler is used instead.

	/// @def Ic3DebugInterruptEx
	/// @brief Similar to Ic3DebugInterrupt, but uses explicitly specified handler to handle a condition failure.
	/// If CPPX_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE is enabled and _Ic3DebugInterruptHandler is defined,
	/// provided handler is used instead.

	/// @def Ic3DebugInterruptOnceEx
	/// @brief Similar to Ic3DebugInterruptOnce, but uses explicitly specified handler to handle a condition failure.
	/// If CPPX_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE is enabled and _Ic3DebugInterruptHandler is defined,
	/// provided handler is used instead.

	/// @def Ic3DebugOutput
	/// @brief Prints formatted output to the default debug output stream.
	/// If CPPX_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE is enabled and _Ic3DebugPrintHandler is defined,
	/// provided handler is used instead.

	/// @def Ic3DebugOutputFmt
	/// @brief Prints formatted output to the default debug output stream.
	/// If CPPX_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE is enabled and _Ic3DebugPrintHandler is defined,
	/// provided handler is used instead.

	/// @def Ic3DebugOutputEx
	/// @brief Similar to Ic3DebugOutput, but uses explicitly specified handler to handle a condition failure.
	/// If CPPX_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE is enabled and _Ic3DebugPrintHandler is defined,
	/// provided handler is used instead.

	/// @def Ic3DebugOutputExFmt
	/// @brief Similar to Ic3DebugOutput, but uses explicitly specified handler to handle a condition failure.
	/// If CPPX_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE is enabled and _Ic3DebugPrintHandler is defined,
	/// provided handler is used instead.

	/// @def Ic3DebugOnlyCode
	/// @brief ToDo

#if( CPPX_CONFIG_CORE_ENABLE_DEBUG_LAYER )
#  define Ic3DebugAssert( pCondition )                   Ic3DebugAssertImpl( pCondition )
#  define Ic3DebugAssertOnce( pCondition )               Ic3DebugAssertOnceImpl( pCondition )
#  define Ic3DebugAssertEx( pHandler, pCondition )       Ic3DebugAssertExImpl( pHandler, pCondition )
#  define Ic3DebugAssertOnceEx( pHandler, pCondition )   Ic3DebugAssertOnceExImpl( pHandler, pCondition )
#  define Ic3DebugInterrupt()                            Ic3DebugInterruptImpl()
#  define Ic3DebugInterruptOnce()                        Ic3DebugInterruptOnceImpl()
#  define Ic3DebugInterruptEx( pHandler )                Ic3DebugInterruptExImpl( pHandler )
#  define Ic3DebugInterruptOnceEx( pHandler )            Ic3DebugInterruptOnceExImpl( pHandler )
#  define Ic3DebugOutput( pText )                        Ic3DebugOutputImpl( pText )
#  define Ic3DebugOutputFmt( pFormat, ... )              Ic3DebugOutputFmtImpl( pFormat, __VA_ARGS__ )
#  define Ic3DebugOutputEx( pHandler, pText )            Ic3DebugOutputExImpl( pHandler, pText )
#  define Ic3DebugOutputExFmt( pHandler, pFormat, ... )  Ic3DebugOutputExFmtImpl( pHandler, pFormat, __VA_ARGS__ )
#  define Ic3DebugOnlyCode( pCode )                      pCode
#else
	#  define Ic3DebugAssert( pCondition )
#  define Ic3DebugAssertOnce( pCondition )
#  define Ic3DebugAssertEx( pHandler, pCondition )
#  define Ic3DebugAssertOnceEx( pHandler, pCondition )
#  define Ic3DebugInterrupt()
#  define Ic3DebugInterruptOnce()
#  define Ic3DebugInterruptEx( pHandler )
#  define Ic3DebugInterruptOnceEx( pHandler )
#  define Ic3DebugOutput( pText )
#  define Ic3DebugOutputFmt( pFormat, ... )
#  define Ic3DebugOutputEx( pHandler, pText )
#  define Ic3DebugOutputExFmt( pHandler, pFormat, ... )
#  define Ic3DebugOnlyCode( pCode )
#endif

	using DebugPrintHandlerType = void(*)( const char *, size_t );
	using DebugAssertionFailHandlerType = void(*)( const FileLocationInfo &, const char * );
	using DebugInterruptHandlerType = void(*)( const FileLocationInfo & );

#if( CPPX_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE )

	// The three macros defined below allow to override the default handlers used for standard debug macros.
	// Obviously, this works at build time and is useful when building a customized version of the engine.
	// Also, since this doesn't change the already built engine modules, re-definition can enable different
	// behaviour for the target app/game while keeping the default one for the engine itself.
	// - _Ic3DebugAssertionFailHandler must match the signature of DebugAssertionFailHandlerType
	// - _Ic3DebugInterruptHandler must match the signature of DebugInterruptHandlerType
	// - _Ic3DebugPrintHandler must match the signature of DebugPrintHandlerType

#if !defined( _Ic3DebugAssertionFailHandler )
	// Custom handler for the assertion fail macro.
#  define _Ic3DebugAssertionFailHandler nullptr
#endif

#if !defined( _Ic3DebugInterruptHandler )
	// Custom handler for the interrupt macro.
#  define _Ic3DebugInterruptHandler nullptr
#endif

#if !defined( _Ic3DebugPrintHandler )
	// Custom handler for the print macro.
#  define _Ic3DebugPrintHandler nullptr
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
		// Default assertion handler: prints assertion info using PCL_DEBUG_OUTPUT() and then triggers an interrupt with PCL_DEBUG_BREAK().
		static void _defaultAssertionFailHandler( const FileLocationInfo & pLocationInfo, const char * pConditionStr );

		// Default interrupt handler: triggers an interrupt with PCL_DEBUG_BREAK().
		static void _defaultInterruptHandler( const FileLocationInfo & pLocationInfo );

		// Default print handler: prints specified text with PCL_DEBUG_OUTPUT().
		static void _defaultPrintHandler( const char * pText, size_t pLength );
	};


#define Ic3DebugAssertImpl( pCondition ) \
		{ \
			if(!(pCondition)) \
			{ \
				::Ic3::DebugInterface::assertionFail( _Ic3DebugAssertionFailHandler, cppx_current_file_location_info(), #pCondition ); \
			} \
		}

#define Ic3DebugAssertOnceImpl( pCondition ) \
		{ \
			static bool cFlag = false; \
			if( !cFlag && !(pCondition) ) \
			{ \
				::Ic3::DebugInterface::assertionFail( _Ic3DebugAssertionFailHandler, cppx_current_file_location_info(), #pCondition ); \
				cFlag = true; \
			} \
		}

#define Ic3DebugAssertExImpl( pHandler, pCondition ) \
		{ \
			if(!(pCondition)) \
			{ \
				::Ic3::DebugInterface::assertionFail( pHandler, cppx_current_file_location_info(), #pCondition ); \
			} \
		}

#define Ic3DebugAssertOnceExImpl( pHandler, pCondition ) \
		{ \
			static bool cFlag = false; \
			if( !cFlag && !(pCondition) ) \
			{ \
				::Ic3::DebugInterface::assertionFail( pHandler, cppx_current_file_location_info(), #pCondition ); \
				cFlag = true; \
			} \
		}

#define Ic3DebugInterruptImpl() \
		{ \
			::Ic3::DebugInterface::interrupt( _Ic3DebugInterruptHandler, cppx_current_file_location_info() ); \
		}

#define Ic3DebugInterruptOnceImpl() \
		{ \
			static bool cFlag = false; \
			if( !cFlag ) \
			{ \
				::Ic3::DebugInterface::interrupt( _Ic3DebugInterruptHandler, cppx_current_file_location_info() ); \
				cFlag = true; \
			} \
		}

#define Ic3DebugInterruptExImpl( pHandler ) \
		{ \
			::Ic3::DebugInterface::interrupt( pHandler, cppx_current_file_location_info() ); \
		}

#define Ic3DebugInterruptOnceExImpl( pHandler ) \
		{ \
			static bool cFlag = false; \
			if( !cFlag ) \
			{ \
				::Ic3::DebugInterface::interrupt( pHandler, cppx_current_file_location_info() ); \
				cFlag = true; \
			} \
		}

#define Ic3DebugOutputImpl( pText ) \
		::cppx::DebugInterface::printDebug( _Ic3DebugPrintHandler, "%s", pText  )

#define Ic3DebugOutputFmtImpl( pFormat, ... ) \
		::cppx::DebugInterface::printDebug( _Ic3DebugPrintHandler, pFormat, __VA_ARGS__  )

#define Ic3DebugOutputExImpl( pHandler, pText ) \
		::cppx::DebugInterface::printDebug( pHandler, "%s", pText  )

#define Ic3DebugOutputExFmtImpl( pHandler, pFormat, ... ) \
		::cppx::DebugInterface::printDebug( pHandler, pFormat, __VA_ARGS__  )

#else

	namespace Impl
	{

		void debugOutputImpl( DebugPrintHandlerType pHandler, const char * pFormat, ... );

		void debugDefaultPrintHandler( const char * pText, size_t pLength );

	}

	#define Ic3DebugAssertImpl( pCondition ) \
		{ \
			if(!(pCondition)) \
			{ \
				PCL_DEBUG_BREAK(); \
			} \
		}

	#define Ic3DebugAssertOnceImpl( pCondition ) \
		{ \
			static bool cFlag = false; \
			if( !cFlag && !(pCondition) ) \
			{ \
				PCL_DEBUG_BREAK(); \
				cFlag = true; \
			} \
		}

	#define Ic3DebugAssertExImpl( pHandler, pCondition ) \
		{ \
			if(!(pCondition)) \
			{ \
				pHandler(); \
			} \
		}

	#define Ic3DebugAssertOnceExImpl( pHandler, pCondition ) \
		{ \
			static bool cFlag = false; \
			if( !cFlag && !(pCondition) ) \
			{ \
				pHandler(); \
				cFlag = true; \
			} \
		}

	#define Ic3DebugInterruptImpl() \
		{ \
			PCL_DEBUG_BREAK(); \
		}

	#define Ic3DebugInterruptOnceImpl() \
		{ \
			static bool cFlag = false; \
			if( !cFlag ) \
			{ \
				PCL_DEBUG_BREAK(); \
				cFlag = true; \
			} \
		}

	#define Ic3DebugInterruptExImpl( pHandler ) \
		{ \
			pHandler(); \
		}

	#define Ic3DebugInterruptOnceExImpl( pHandler ) \
		{ \
			static bool cFlag = false; \
			if( !cFlag ) \
			{ \
				pHandler(); \
				cFlag = true; \
			} \
		}

	#define Ic3DebugOutputImpl( pText ) \
		::Ic3::Impl::debugOutputImpl( ::Ic3::Impl::debugDefaultPrintHandler, "%s", pText  )

	#define Ic3DebugOutputFmtImpl( pFormat, ... ) \
		::Ic3::Impl::debugOutputImpl( ::Ic3::Impl::debugDefaultPrintHandler, pFormat, __VA_ARGS__  )

	#define Ic3DebugOutputExImpl( pHandler, pText ) \
		::Ic3::Impl::debugOutputImpl( pHandler, "%s", pText  )

	#define Ic3DebugOutputExFmtImpl( pHandler, pFormat, ... ) \
		::Ic3::Impl::debugOutputImpl( pHandler, pFormat, __VA_ARGS__  )

#endif // CPPX_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE

}

#endif // __IC3_CORELIB_RUNTIME_ASSERT_H__
