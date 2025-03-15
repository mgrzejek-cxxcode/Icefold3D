
#include <Ic3/System/Prerequisites.h>
#include "WFACommon.h"
#include <comdef.h>
#include <cppx/stringUtils.h>

#if( PCL_TARGET_OS & PCL_TARGET_FLAG_OS_WFA )
namespace Ic3::System
{

	namespace Platform
	{

		std::string WFAQueryComErrorMessage( HRESULT pHResult )
		{
			std::string errorMessage;

		#if( PCL_COMPILER & PCL_COMPILER_MSVC )

			const auto comError = _com_error{ pHResult };
			const auto comErrorString = _bstr_t{ comError.ErrorMessage() };

			if( comErrorString.length() > 0 )
			{
				const auto * strBuffer = static_cast<const char *>( comErrorString );
				errorMessage.assign( strBuffer, comErrorString.length() );
			}

		#elif( PCL_COMPILER & PCL_COMPILER_MINGW64 )

			const auto comError = _com_error{ pHResult };
			const auto * comErrorString = comError.ErrorMessage();

			errorMessage.assign( comErrorString, std::char_traits<TCHAR>::length( comErrorString ) );

		#else
			#error "Unsupported platform configuration!"
		#endif

			return errorMessage;
		}

		std::string WFAQuerySystemErrorMessage( DWORD pErrorCode )
		{
			char * messagePtr = nullptr;

			const auto messageLength = FormatMessageA(
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,
				nullptr, // No format string provided for FORMAT_MESSAGE_FROM_SYSTEM
				pErrorCode,
				MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
				reinterpret_cast<LPTSTR>( &messagePtr ),
				0,
				nullptr );

			std::string errorMessage;
			if( messageLength > 0 )
			{
				errorMessage.assign( messagePtr, messageLength );

				// FORMAT_MESSAGE_ALLOCATE_BUFFER requires a manual release of the buffer allocated by the system.
				// In case of 'messageLength' being 0, no buffer gets allocated!
				::LocalFree( messagePtr );
			}

			return errorMessage;
		}

		void WFAGetLastSystemErrorAndPrintToDebugOutput( const char * pPrefix )
		{
			const auto lastErrorCode = ::GetLastError();
			WFAPrintSystemErrorToDebugOutput( lastErrorCode, pPrefix );
		}

		void WFAPrintSystemErrorToDebugOutput( DWORD pErrorCode, const char * pPrefix )
		{
			const auto errorMessage = WFAQueryComErrorMessage( pErrorCode );
			if( pPrefix )
			{
				Ic3DebugOutputSys( "%s / GetLastError(): #%x - %s", pPrefix, pErrorCode, errorMessage.c_str() );
			}
			else
			{
				Ic3DebugOutputSys( "GetLastError(): #%x  - %s", pErrorCode, errorMessage.c_str() );
			}
		}

	}

} // namespace Ic3::System
#endif
