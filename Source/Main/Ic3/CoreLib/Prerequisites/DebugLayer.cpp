
#include "../Prerequisites.h"
#include <cppx/stringUtils.h>
#include <regex>

#define IC3_DEBUG_OUTPUT_MULTILINE 0

namespace Ic3
{

	namespace Debug
	{

		const char * GetDebugMessageTypeStr( EDebugMessageType pMessageType )
		{
			switch( pMessageType )
			{
				case eDebugMTDefault:   return "Debug";
				case eDebugMTInfo:      return "Info";
				case eDebugMTWarning:   return "Warning";
				case eDebugMTError:     return "Error";
				case eDebugMTException: return "Exception";

				default:
					break;
			}

			return "Unknown";
		}

		void DebugOutputVarPrintImpl( std::string & pPrintString, const char * pFormat, va_list pVarArgs )
		{
			const size_t localBufferSize = 1024;
			char localBuffer[localBufferSize];

			size_t printCount = 0;
			char * bufferPtr = &( localBuffer[0] );

			{
				// This returns the number that *would be written* if buffer was large enough.
				// For a valid case, it should always smaller than our buffer size. If not,
				// we need to make the buffer capacity a bit bigger.
				printCount = vsnprintf( bufferPtr, localBufferSize, pFormat, pVarArgs );

				if( printCount > localBufferSize )
				{
					// Just signal via debug break. We don't need any runtime
					// error handling - this is used only in debug mode anyway.
					PCL_DEBUG_BREAK();
				}
			}

			pPrintString.append( " " );
			pPrintString.append( localBuffer );

		#if( !IC3_DEBUG_OUTPUT_MULTILINE )

			PCL_DEBUG_OUTPUT( pPrintString.c_str() );
			PCL_DEBUG_OUTPUT( "\n" );

		#else

			auto messageLines = cppx::strutil::split_string_ex< std::vector<std::string> >(
				pPrintString, '\n',
				[]( auto & pResult, auto * pStr, auto pLength ) {
					pResult.push_back( std::string( pStr, pLength ) );
				} );

			size_t longestLineLength = 0;
			for( const auto & messageLine : messageLines )
			{
				const auto lineLength = messageLine.length();
				if( lineLength > longestLineLength )
				{
					longestLineLength = lineLength;
				}
			}

			std::string horizontalFrame{};
			horizontalFrame.append( longestLineLength, '-' );

			PCL_DEBUG_OUTPUT( horizontalFrame.c_str() );
			PCL_DEBUG_OUTPUT( "\n" );
			for( const auto & messageLine : messageLines )
			{
				PCL_DEBUG_OUTPUT( messageLine.c_str() );
				PCL_DEBUG_OUTPUT( "\n" );
			}
			PCL_DEBUG_OUTPUT( horizontalFrame.c_str() );
			PCL_DEBUG_OUTPUT( "\n" );

		#endif
		}

		void DebugOutputExtImpl( EDebugMessageType pMessageType, const char * pPrefix, const char * pFormat, va_list pVarArgList )
		{
			std::string printOutput{};
			printOutput.reserve( 512 );

			printOutput.append( "[MT" );
			printOutput.append( GetDebugMessageTypeStr( pMessageType ) );
			if( pPrefix )
			{
				printOutput.append( ": " );
				printOutput.append( pPrefix );
			}
			printOutput.append( 1, ']' );

			DebugOutputVarPrintImpl( printOutput, pFormat, pVarArgList );
		}

		void DebugOutputExtOriginImpl( const cppx::file_location_info & pPrintOrigin, EDebugMessageType pMessageType, const char * pPrefix, const char * pFormat, va_list pVarArgList )
		{
			const auto shortFile = cppx::strutil::extract_short_file_path( pPrintOrigin.filename );
			const auto shortFunction = cppx::strutil::extract_short_function_name( pPrintOrigin.function );

			std::string printOutput{};
			printOutput.reserve( 512 );

			printOutput.append( "[MT" );
			printOutput.append( GetDebugMessageTypeStr( pMessageType ) );
			printOutput.append( ": " );
			if( pPrefix )
			{
				printOutput.append( pPrefix );
				printOutput.append( 1, '/' );
			}
			printOutput.append( shortFunction.c_str() );
			printOutput.append( "()" );
			printOutput.append( 1, '/' );
			printOutput.append( shortFile.c_str() );
			printOutput.append( 1, ':' );
			printOutput.append( std::to_string( pPrintOrigin.line_number ) );
			printOutput.append( 1, ']' );

			DebugOutputVarPrintImpl( printOutput, pFormat, pVarArgList );
		}

	}

}
