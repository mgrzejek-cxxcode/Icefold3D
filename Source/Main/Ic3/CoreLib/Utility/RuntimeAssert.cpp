
#include "RuntimeAssert.h"

namespace Ic3
{

	void _assertionFail( const char * pConditionStr, const FileLocationInfo & pLocationInfo )
	{
		std::string message{ "Debug assertion failed!" };
		message.append( "\nWhere: " );
		message.append( pLocationInfo.toString() );
		message.append( "\nCondition: " );
		message.append( pConditionStr );

	#if( PCL_TARGET_OS == PCL_TARGET_OS_WINDESKTOP )
		message.append( "\n\nAbort: terminate\nRetry: break here\nIgnore: skip" );
		int result = ::MessageBoxA( nullptr, message.data(), "Runtime interrupt", MB_ABORTRETRYIGNORE | MB_ICONERROR | MB_APPLMODAL );
		if( result == IDABORT )
		{
			throw 0;
		}
		else if( result == IDRETRY )
		{
		}
	#else
		DebugInterface::_defaultPrintHandler( message.c_str(), message.length() );
	#endif
	}

	void _interrupt( const FileLocationInfo & pLocationInfo )
	{
		std::string message{ "Debug interrupt triggered!" };
		message.append( "\nWhere: " );
		message.append( pLocationInfo.toString() );

	#if( PCL_TARGET_OS == PCL_TARGET_OS_WINDESKTOP )
		message.append( "\n\nAbort: terminate\nRetry: break here\nIgnore: skip" );
		::MessageBoxA( nullptr, message.data(), "Runtime interrupt", MB_ABORTRETRYIGNORE | MB_ICONERROR | MB_APPLMODAL );
	#else
		DebugInterface::_defaultPrintHandler( message.c_str(), message.length() );
	#endif
	}

}

#include "DebugLayer.h"
#include <cstdarg>

namespace Ic3
{

#if( CPPX_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE )

	void DebugInterface::assertionFail( DebugAssertionFailHandlerType pHandler, const FileLocationInfo & pLocationInfo, const char * pConditionStr )
	{
		if( !pHandler )
		{
			pHandler = _defaultAssertionFailHandler;
		}

		pHandler( pLocationInfo, pConditionStr );
	}

	void DebugInterface::interrupt( DebugInterruptHandlerType pHandler, const FileLocationInfo & pLocationInfo )
	{
		if( !pHandler )
		{
			pHandler = _defaultInterruptHandler;
		}

		pHandler( pLocationInfo );
	}

	void DebugInterface::printDebug( DebugPrintHandlerType pHandler, const char* pFormat, ... )
	{
		if( !pHandler )
		{
			pHandler = _defaultPrintHandler;
		}

		const size_t localBufferSize = 4096;
		char localBuffer[localBufferSize];

		size_t printCount = 0;
		char * bufferPtr = &( localBuffer[0] );

		{
			va_list varArgsList;
			va_start( varArgsList, pFormat );

			// This returns the number that *would be written* if buffer was large enough.
			// For a valid case, it should always smaller than our buffer size. If not,
			// we need to make the buffer capacity a bit bigger.
			printCount = vsnprintf( bufferPtr, localBufferSize, pFormat, varArgsList );

			if( printCount > localBufferSize )
			{
				// Just signal via debug break. We don't need any runtime
				// error handling - this is used only in debug mode anyway.
				PCL_DEBUG_BREAK();
			}

			va_end( varArgsList );
		}

		pHandler( localBuffer, printCount );
	}

	void DebugInterface::_defaultAssertionFailHandler( const FileLocationInfo & pLocationInfo, const char * pConditionStr )
	{
		std::string message{ "\n|> Debug assertion failed!" };
		message.append( "\n|> " );
		message.append( pLocationInfo.toString() );
		message.append( "\n|> [ " );
		message.append( pConditionStr );
		message.append( " ] is false\n" );

		PCL_DEBUG_OUTPUT( message.c_str() );
		PCL_DEBUG_BREAK();
	}

	void DebugInterface::_defaultInterruptHandler( const FileLocationInfo & pLocationInfo )
	{
		std::string message{ "\n|> Debug interrupt triggered!" };
		message.append( "\n|> " );
		message.append( pLocationInfo.toString() );
		message.append( "\n" );

		PCL_DEBUG_OUTPUT( message.c_str() );
		PCL_DEBUG_BREAK();
	}

	void DebugInterface::_defaultPrintHandler( const char * pText, size_t pLength )
	{
		PCL_DEBUG_OUTPUT( pText );
	}

#else

	namespace Impl
	{

		void debugOutputImpl( DebugPrintHandlerType pHandler, const char * pFormat, ... )
		{
			const size_t localBufferSize = 4096;
			char localBuffer[localBufferSize];

			size_t printCount = 0;
			char * bufferPtr = &( localBuffer[0] );

			{
				va_list varArgsList;
				va_start( varArgsList, pFormat );

				// This returns the number that *would be written* if buffer was large enough.
				// For a valid case, it should always smaller than our buffer size. If not,
				// we need to make the buffer capacity a bit bigger.
				printCount = vsnprintf( bufferPtr, localBufferSize, pFormat, varArgsList );

				if( printCount > localBufferSize )
				{
					// Just signal via debug break. We don't need any runtime
					// error handling - this is used only in debug mode anyway.
					PCL_DEBUG_BREAK();
				}

				va_end( varArgsList );
			}

			PCL_DEBUG_OUTPUT( localBuffer );
		}

		void debugDefaultPrintHandler( const char * pText, size_t pLength )
		{
			PCL_DEBUG_OUTPUT( pText );
		}

	}

#endif // CPPX_CONFIG_CORE_ENABLE_EXTENDED_DEBUG_INTERFACE

}
