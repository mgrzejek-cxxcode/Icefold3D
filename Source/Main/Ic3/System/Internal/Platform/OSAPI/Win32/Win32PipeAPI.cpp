
#include "Win32PipeAPI.h"
#include <IC3/System/PerfCounter.h>

namespace Ic3::System
{

	namespace Platform
	{

		static const char * kWin32PipeNameBaseComponent = "\\\\.\\pipe\\LOCAL\\";
		static constexpr auto kWin32PipeInOutBufferSize = 4 * 1024;


		DWORD _Win32TranslatePipeDataModeToWriteCreateFlags( EPipeDataMode pPipeDataMode );

		DWORD _Win32TranslatePipeDataModeToReadMode( EPipeDataMode pPipeDataMode );

		HANDLE _Win32CreateReadPipe(
			const std::string & pFullyQualifiedPipeName,
			EPipeDataMode pPipeDataMode,
			const IOTimeoutSettings & pTimeoutSettings );

		HANDLE _Win32CreateWritePipe(
			const std::string & pFullyQualifiedPipeName,
			EPipeDataMode pPipeDataMode,
			const IOTimeoutSettings & pTimeoutSettings );

		void _Win32ClosePipe( HANDLE pPipeHandle, EIOAccessMode pAccessMode, bool pIsBrokenPipe );

	}


	Win32PipeFactory::Win32PipeFactory( SysContextHandle pSysContext )
	: Win32NativeObject( std::move( pSysContext ) )
	{}

	Win32PipeFactory::~Win32PipeFactory() noexcept = default;

	PipeHandle Win32PipeFactory::_NativeCreateNamedPipeReadAccess(
		const cppx::immutable_string & pPipeName,
		EPipeDataMode pPipeDataMode,
		const IOTimeoutSettings & pTimeoutSettings )
	{
		std::string fullyQualifiedPipeName = Platform::kWin32PipeNameBaseComponent;
		fullyQualifiedPipeName.append( pPipeName.str() );

		auto win32PipeHandle = Platform::_Win32CreateReadPipe( fullyQualifiedPipeName, pPipeDataMode, pTimeoutSettings );
		if( !win32PipeHandle )
		{
			Ic3DebugOutputFmt( "Failed to open read pipe '%s'.", pPipeName.data() );
			return nullptr;
		}

		PipeProperties pipeProperties{};
		pipeProperties.accessMode = EIOAccessMode::ReadOnly;
		pipeProperties.fullyQualifiedPipeName = std::move( fullyQualifiedPipeName );
		pipeProperties.pipeDataMode = pPipeDataMode;

		auto pipeObject = CreateSysObject<Win32Pipe>( mSysContext, pipeProperties );
		pipeObject->SetInternalWin32PipeHandle( win32PipeHandle );

		return pipeObject;
	}

	PipeHandle Win32PipeFactory::_NativeCreateNamedPipeWriteAccess(
		const cppx::immutable_string & pPipeName,
		EPipeDataMode pPipeDataMode,
		const IOTimeoutSettings & pTimeoutSettings )
	{
		std::string fullyQualifiedPipeName = Platform::kWin32PipeNameBaseComponent;
		fullyQualifiedPipeName.append( pPipeName.str() );

		auto win32PipeHandle = Platform::_Win32CreateWritePipe( fullyQualifiedPipeName, pPipeDataMode, pTimeoutSettings );
		if( !win32PipeHandle )
		{
			Ic3DebugOutputFmt( "Failed to create write pipe '%s'.", pPipeName.data() );
			return nullptr;
		}

		PipeProperties pipeProperties{};
		pipeProperties.accessMode = EIOAccessMode::WriteAppend;
		pipeProperties.fullyQualifiedPipeName = std::move( fullyQualifiedPipeName );
		pipeProperties.pipeDataMode = pPipeDataMode;

		auto pipeObject = CreateSysObject<Win32Pipe>( mSysContext, pipeProperties );
		pipeObject->SetInternalWin32PipeHandle( win32PipeHandle );

		return pipeObject;
	}


	Win32Pipe::Win32Pipe( SysContextHandle pSysContext, const PipeProperties & pPipeProperties )
	: Win32NativeObject( std::move( pSysContext ), pPipeProperties )
	{}

	Win32Pipe::~Win32Pipe() noexcept
	{
		_ReleaseWin32PipeHandle();
	}

	void Win32Pipe::SetInternalWin32PipeHandle( HANDLE pPipeHandle )
	{
		mNativeData.pipeHandle = pPipeHandle;
	}

	bool Win32Pipe::_NativePipeIsValid() const noexcept
	{
		if( mNativeData.pipeHandle != nullptr )
		{
			const auto peekPipeResult = ::PeekNamedPipe(
				mNativeData.pipeHandle,
				nullptr,
				0,
				nullptr,
				nullptr,
				nullptr );

			if( peekPipeResult )
			{
				return true;
			}
		}

		return false;
	}

	io_size_t Win32Pipe::_NativePipeGetAvailableDataSize() const
	{
		if( !mNativeData.pipeHandle )
		{
			return 0;
		}

		DWORD totalBytesAvailableInPipe = 0;
		DWORD currentMessageBytesLeft = 0;

		const auto readResult = ::PeekNamedPipe(
			mNativeData.pipeHandle,
			nullptr,
			0,
			nullptr,
			&totalBytesAvailableInPipe,
			&currentMessageBytesLeft );

		if( !readResult )
		{
			Platform::WFAGetLastSystemErrorAndPrintToDebugOutput( "PeekNamedPipe" );
		}

		return ( currentMessageBytesLeft > 0 ) ? currentMessageBytesLeft : totalBytesAvailableInPipe;
	}

	io_size_t Win32Pipe::_NativePipeReadData( void * pTargetBuffer, io_size_t pReadSize )
	{
		BOOL readResult = FALSE;

		DWORD readBytesNum = 0;
		DWORD totalBytesAvailableInPipe = 0;
		DWORD remainingMessageSize = 0;

		// First, use PeekNamedPipe to fetch the size the next message. We need that info
		// to properly handle reads smaller than the size of the whole message chunk.
		readResult = ::PeekNamedPipe(
			mNativeData.pipeHandle,
			nullptr,
			0,
			nullptr,
			&totalBytesAvailableInPipe,
			&remainingMessageSize );

		if( ( totalBytesAvailableInPipe == 0 ) && ( remainingMessageSize > 0 ) )
		{
			return 0;
		}

		if( !readResult )
		{
			const auto lastError = ::GetLastError();
			if( lastError == ERROR_BROKEN_PIPE )
			{
				_ReleaseWin32PipeHandle( true );
			}

			Platform::WFAPrintSystemErrorToDebugOutput( lastError, "PeekNamedPipe" );
		}

		if( readResult )
		{
			readResult = ::ReadFile(
				mNativeData.pipeHandle,
				pTargetBuffer,
				cppx::numeric_cast<DWORD>( pReadSize ),
				&readBytesNum,
				nullptr );

			if( !readResult )
			{
				const auto lastError = ::GetLastError();
				if( lastError == ERROR_MORE_DATA )
				{
				}
				else
				{
					_ReleaseWin32PipeHandle( true );
					Platform::WFAPrintSystemErrorToDebugOutput( lastError, "ReadFile( Pipe )" );
				}
			}
		}

		return cppx::numeric_cast<io_size_t>( readBytesNum );
	}

	io_size_t Win32Pipe::_NativePipeWriteData( const void * pData, io_size_t pWriteSize )
	{
		DWORD writeSize = 0;
		const auto writeResult = ::WriteFile(
			mNativeData.pipeHandle,
			pData,
			cppx::numeric_cast<DWORD>( pWriteSize ),
			&writeSize,
			nullptr );

		if( !writeResult )
		{
			const auto lastError = ::GetLastError();
			if( lastError == ERROR_BROKEN_PIPE )
			{
				_ReleaseWin32PipeHandle( true );
			}

			Platform::WFAPrintSystemErrorToDebugOutput( lastError, "WriteFile( Pipe )" );
		}

		return cppx::numeric_cast<io_size_t>( writeSize );
	}

	bool Win32Pipe::_NativeReconnectReadPipe( const IOTimeoutSettings & pTimeoutSettings )
	{
		return false;
	}

	bool Win32Pipe::_NativeReconnectWritePipe( const IOTimeoutSettings & pTimeoutSettings )
	{
		return false;
	}

	void Win32Pipe::_ReleaseWin32PipeHandle( bool pIsBrokenPipe )
	{
		if( mNativeData.pipeHandle )
		{
			Platform::_Win32ClosePipe( mNativeData.pipeHandle, mAccessMode, pIsBrokenPipe );
			mNativeData.pipeHandle = nullptr;
		}
	}


	namespace Platform
	{

		DWORD _Win32TranslatePipeDataModeToWriteCreateFlags( EPipeDataMode pPipeDataMode )
		{
			if( pPipeDataMode == EPipeDataMode::ByteStream )
			{
				return PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT;
			}

			if( pPipeDataMode == EPipeDataMode::MessageStream )
			{
				return PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT;
			}

			return 0;
		}

		DWORD _Win32TranslatePipeDataModeToReadMode( EPipeDataMode pPipeDataMode )
		{
			if( pPipeDataMode == EPipeDataMode::ByteStream )
			{
				return PIPE_READMODE_BYTE;
			}

			if( pPipeDataMode == EPipeDataMode::MessageStream )
			{
				return PIPE_READMODE_MESSAGE;
			}

			return 0;
		}

		HANDLE _Win32CreateReadPipe(
			const std::string & pFullyQualifiedPipeName,
			EPipeDataMode pPipeDataMode,
			const IOTimeoutSettings & pTimeoutSettings )
		{
			// This is the "client" connection function for a named pipe.
			// The server creates the pipe in to steps:
			// 1. First, it creates the named pipe using either CreateFile() or CallNamedPipe().
			// 2. Then, the server opens a pipe connection via ConnectNamedPipe().

			HANDLE pipeHandle = INVALID_HANDLE_VALUE;

			const auto win32PipeNameStr = pFullyQualifiedPipeName.c_str();
			const auto startTimeStampCounter = PerfCounter::QueryCounter();

			while( PerfCounter::ConvertToMilliseconds( PerfCounter::QueryCounter() - startTimeStampCounter ) < pTimeoutSettings.waitTimeout )
			{
				if( pipeHandle == INVALID_HANDLE_VALUE )
				{
					// Open the pipe file. GENERIC_READ specifies the primary access, but we also need
					// FILE_WRITE_ATTRIBUTES to update the pipe read mode (BYTE/MESSAGE) later on.
					pipeHandle = ::CreateFileA(
						win32PipeNameStr,
						GENERIC_READ | FILE_WRITE_ATTRIBUTES,
						0,
						nullptr,
						OPEN_EXISTING,
						0,
						nullptr );

					// A valid handle returned means everything is good, and we are ready to use our pipe.
					if( pipeHandle != INVALID_HANDLE_VALUE )
					{
						break;
					}

					const auto lastError = ::GetLastError();
					if( lastError == ERROR_FILE_NOT_FOUND )
					{
						// If the result is ERROR_FILE_NOT_FOUND, it means that the server/writer has not created the pipe file yet (step 1).
						// In this case, we need to keep waiting until it is done, according to the specified timeout settings.
						std::this_thread::sleep_for( pTimeoutSettings.yieldTimeBetweenRetries.get_std() );
					}
					else if( lastError == ERROR_PIPE_BUSY )
					{
						// If the result is ERROR_PIPE_BUSY, it means that all allowed pipe instances are busy. In this case, we use the
						// dedicated function, WaitNamedPipe() to wait the configured timeout until an instance is available. This has
						// the advantage of this wait being signaled as soon as the server makes ConnectNamedPipe() call on the pipe.
						::WaitNamedPipeA( win32PipeNameStr, pTimeoutSettings.yieldTimeBetweenRetries.get_count_as_milli<DWORD>() );
					}
					else
					{
						// Any other result is unexpected and means error. Log the message and return.
						WFAGetLastSystemErrorAndPrintToDebugOutput();
						break;
					}
				}
			}

			if( pipeHandle == INVALID_HANDLE_VALUE )
			{
				pipeHandle = nullptr;
			}

			if( pipeHandle )
			{
				// Update the pipe read mode according to the specified parameter. This works by modifying
				// pipe's file descriptor attributes, hence FILE_WRITE_ATTRIBUTES option to CreateFileA().
				DWORD requestedPipeMode = _Win32TranslatePipeDataModeToReadMode( pPipeDataMode );

				const auto updateModeResult = ::SetNamedPipeHandleState(
					pipeHandle,
					&requestedPipeMode,
					nullptr,
					nullptr );

				if( !updateModeResult )
				{
					WFAGetLastSystemErrorAndPrintToDebugOutput();
					::CloseHandle( pipeHandle );
					pipeHandle = nullptr;
				}
			}

			if( !pipeHandle )
			{
				Ic3DebugOutputFmt( "Could not connect to pipe %s.", win32PipeNameStr );
			}
			else
			{
				Ic3DebugOutputFmt( "Successfully connected to pipe %s.", win32PipeNameStr );
			}

			return pipeHandle;
		}

		HANDLE _Win32CreateWritePipe(
			const std::string & pFullyQualifiedPipeName,
			EPipeDataMode pPipeDataMode,
			const IOTimeoutSettings & pTimeoutSettings )
		{
			HANDLE pipeHandle = INVALID_HANDLE_VALUE;

			const auto win32PipeNameStr = pFullyQualifiedPipeName.c_str();
			const auto win32PipeMode = _Win32TranslatePipeDataModeToWriteCreateFlags( pPipeDataMode );

			SECURITY_ATTRIBUTES win32SecurityAttributes{};
			win32SecurityAttributes.nLength = sizeof( SECURITY_ATTRIBUTES );
			win32SecurityAttributes.bInheritHandle = FALSE;
			win32SecurityAttributes.lpSecurityDescriptor = nullptr;

			pipeHandle = ::CreateNamedPipeA(
				win32PipeNameStr,
				PIPE_ACCESS_OUTBOUND,
				win32PipeMode,
				1,
				kWin32PipeInOutBufferSize,
				kWin32PipeInOutBufferSize,
				NMPWAIT_USE_DEFAULT_WAIT,
				&win32SecurityAttributes );

			if( pipeHandle == INVALID_HANDLE_VALUE )
			{
				WFAGetLastSystemErrorAndPrintToDebugOutput( "CreateNamedPipeA");
				pipeHandle = nullptr;
			}

			if( pipeHandle )
			{
				const auto startTimeStampCounter = PerfCounter::QueryCounter();

				while( PerfCounter::ConvertToMilliseconds( PerfCounter::QueryCounter() - startTimeStampCounter ) < pTimeoutSettings.waitTimeout )
				{
					auto connectResult = ::ConnectNamedPipe( pipeHandle, nullptr );
					if( connectResult != FALSE )
					{
						break;
					}
					else
					{
						const auto lastError = ::GetLastError();
						if( lastError == ERROR_PIPE_CONNECTED )
						{
							// This can happen if a client connects in the interval between the call to CreateNamedPipe()
							// and the call to ConnectNamedPipe(). In this situation, there is a good connection between
							// the client and the server, even though the function returns zero.
							break;
						}
						else if( lastError == ERROR_NO_DATA )
						{
							// This means the pipe is somehow still in the connected state. Since the handle can be moved
							// around by the OS-agnostic Pipe class, it is entirely possible we can end up in this situation.
							// To fix this, we need to disconnect on the server side and retry the connection attempt.
							::DisconnectNamedPipe( pipeHandle );
						}
						else
						{
							WFAGetLastSystemErrorAndPrintToDebugOutput( "ConnectNamedPipe" );

							::CloseHandle( pipeHandle );
							pipeHandle = nullptr;

							break;
						}
					}

					std::this_thread::sleep_for( pTimeoutSettings.yieldTimeBetweenRetries.get_std() );
				}
			}


			if( !pipeHandle )
			{
				Ic3DebugOutputFmt( "Could not initialize pipe %s.", win32PipeNameStr );
			}
			else
			{
				Ic3DebugOutputFmt( "Successfully initialized pipe %s.", win32PipeNameStr );
			}

			return pipeHandle;
		}

		void _Win32ClosePipe( HANDLE pPipeHandle, EIOAccessMode pAccessMode, bool pIsBrokenPipe )
		{
			if( pPipeHandle )
			{
				if( !pIsBrokenPipe && cppx::make_bitmask( pAccessMode ).is_set( eIOAccessFlagOpWrite ) )
				{
					// If the pipe is owned by the server component (has write access), we need to flush
					// all buffers first, so the client gets chance to retrieve all the data from this pipe.
					::FlushFileBuffers( pPipeHandle );
					// After flush has been made, we can now disconnect the pipe.
					::DisconnectNamedPipe( pPipeHandle );
				}

				// Finally, we close the handle to the pipe. For clients (read access),
				// this is the only thing that needs to be done to clear up the pipe.
				::CloseHandle( pPipeHandle );
			}
		}

	}
	
}
