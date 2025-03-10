
#include "Win32PipeAPI.h"
#include <IC3/System/PerfCounter.h>

#include <thread>

namespace Ic3::System
{

	namespace Platform
	{

		static const char * kWin32PipeNameBaseComponent = "\\\\.\\pipe\\LOCAL\\";

		static constexpr auto kWin32PipeInOutBufferSize = 4 * 1024;

		DWORD _Win32TranslatePipeDataModeToWriteCreateFlags( EPipeDataMode pPipeDataMode );

		DWORD _Win32TranslatePipeDataModeToReadMode( EPipeDataMode pPipeDataMode );

		HANDLE _Win32CreateWritePipe(
			const std::string & pFullyQualifiedPipeName,
			EPipeDataMode pPipeDataMode,
			const IOTimeoutSettings & pTimeoutSettings );

		HANDLE _Win32CreateReadPipe(
			const std::string & pFullyQualifiedPipeName,
			EPipeDataMode pPipeDataMode,
			const IOTimeoutSettings & pTimeoutSettings );

	}


	Win32PipeFactory::Win32PipeFactory( SysContextHandle pSysContext )
	: Win32NativeObject( std::move( pSysContext ) )
	{}

	Win32PipeFactory::~Win32PipeFactory() noexcept = default;

	ReadPipeHandle Win32PipeFactory::_NativeCreateReadPipe(
		const PipeCreateInfo & pPipeCreateInfo,
		const IOTimeoutSettings & pTimeoutSettings )
	{
		std::string fullyQualifiedPipeName = Platform::kWin32PipeNameBaseComponent;
		fullyQualifiedPipeName.append( pPipeCreateInfo.pipeName.str() );

		auto win32PipeHandle = Platform::_Win32CreateReadPipe( fullyQualifiedPipeName, pPipeCreateInfo.pipeDataMode, pTimeoutSettings );
		if( !win32PipeHandle )
		{
			Ic3DebugOutputFmt( "Failed to open read pipe '%s'.", fullyQualifiedPipeName.data() );
			return nullptr;
		}

		PipeProperties pipeProperties{};
		pipeProperties.pipeType = EPipeType::PTRead;
		pipeProperties.accessMode = EIOAccessMode::ReadOnly;
		pipeProperties.fullyQualifiedPipeName = std::move( fullyQualifiedPipeName );
		pipeProperties.pipeDataMode = pPipeCreateInfo.pipeDataMode;

		auto pipeObject = CreateSysObject<Win32ReadPipe>( mSysContext, pipeProperties );
		pipeObject->SetWin32PipeHandle( win32PipeHandle );

		return pipeObject;
	}

	WritePipeHandle Win32PipeFactory::_NativeCreateWritePipe(
		const PipeCreateInfo & pPipeCreateInfo,
		const IOTimeoutSettings & pTimeoutSettings )
	{
		std::string fullyQualifiedPipeName = Platform::kWin32PipeNameBaseComponent;
		fullyQualifiedPipeName.append( pPipeCreateInfo.pipeName.str() );

		auto win32PipeHandle = Platform::_Win32CreateWritePipe( fullyQualifiedPipeName, pPipeCreateInfo.pipeDataMode, pTimeoutSettings );
		if( !win32PipeHandle )
		{
			Ic3DebugOutputFmt( "Failed to create write pipe '%s'.", fullyQualifiedPipeName.data() );
			return nullptr;
		}

		PipeProperties pipeProperties{};
		pipeProperties.pipeType = EPipeType::PTWrite;
		pipeProperties.accessMode = EIOAccessMode::WriteAppend;
		pipeProperties.fullyQualifiedPipeName = std::move( fullyQualifiedPipeName );
		pipeProperties.pipeDataMode = pPipeCreateInfo.pipeDataMode;

		auto pipeObject = CreateSysObject<Win32WritePipe>( mSysContext, pipeProperties );
		pipeObject->SetWin32PipeHandle( win32PipeHandle );

		return pipeObject;
	}


	Win32ReadPipe::Win32ReadPipe( SysContextHandle pSysContext, const PipeProperties & pPipeProperties )
	: Win32BasePipe<ReadPipe>( std::move( pSysContext ), pPipeProperties )
	{}

	Win32ReadPipe::~Win32ReadPipe() noexcept = default;

	io_size_t Win32ReadPipe::_NativePipeReadData( void * pTargetBuffer, io_size_t pReadSize )
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
				ReleaseWin32PipeHandle();
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
					// This is not an error, not really. It simply means that this read requested for
					// less data than the message chunk in the pipe (only for message-based pipes).
				}
				else
				{
					// Any other error means the pipe got broken. Clean up and report.
					ReleaseWin32PipeHandle();
					Platform::WFAPrintSystemErrorToDebugOutput( lastError, "ReadFile( Pipe )" );
				}
			}
		}

		return cppx::numeric_cast<io_size_t>( readBytesNum );
	}

	bool Win32ReadPipe::_NativeReconnectReadPipe( const IOTimeoutSettings & pTimeoutSettings )
	{
		if( mNativeData.pipeHandle )
		{
			Platform::Win32ClosePipe( mNativeData.pipeHandle, EPipeType::PTRead );
			mNativeData.pipeHandle = nullptr;
		}

		mNativeData.pipeHandle = Platform::_Win32CreateReadPipe( mFullyQualifiedPipeName.str(), mPipeDataMode, pTimeoutSettings );

		return mNativeData.pipeHandle != nullptr;
	}

	io_size_t Win32ReadPipe::_NativePipeGetAvailableDataSize() const
	{
		return Platform::Win32GetPipeAvailableDataSize( this->mNativeData.pipeHandle );
	}


	Win32WritePipe::Win32WritePipe( SysContextHandle pSysContext, const PipeProperties & pPipeProperties )
	: Win32BasePipe<WritePipe>( std::move( pSysContext ), pPipeProperties )
	{}

	Win32WritePipe::~Win32WritePipe() noexcept = default;

	io_size_t Win32WritePipe::_NativePipeWriteData( const void * pData, io_size_t pWriteSize )
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
				ReleaseWin32PipeHandle();
			}

			Platform::WFAPrintSystemErrorToDebugOutput( lastError, "WriteFile( Pipe )" );
		}

		return cppx::numeric_cast<io_size_t>( writeSize );
	}

	bool Win32WritePipe::_NativeReconnectWritePipe( const IOTimeoutSettings & pTimeoutSettings )
	{
		if( mNativeData.pipeHandle )
		{
			Platform::Win32ClosePipe( mNativeData.pipeHandle, EPipeType::PTWrite );
			mNativeData.pipeHandle = nullptr;
		}

		mNativeData.pipeHandle = Platform::_Win32CreateReadPipe( mFullyQualifiedPipeName.str(), mPipeDataMode, pTimeoutSettings );

		return mNativeData.pipeHandle != nullptr;
	}


	namespace Platform
	{

		bool Win32IsPipeBroken( HANDLE pPipeHandle )
		{
			// First, check the native pipe handle.
			if( pPipeHandle )
			{
				// Use PeekNamedPipe to query the pipe status. We don't query any properties
				// (not needed) - PeekNamedPipe will fail immediately of the specified pipe
				// handle does not refer to a valid, active pipe.
				const auto peekPipeResult = ::PeekNamedPipe(
					pPipeHandle,
					nullptr,
					0,
					nullptr,
					nullptr,
					nullptr );

				if( !peekPipeResult && ( ::GetLastError() == ERROR_BROKEN_PIPE ) )
				{
					return true;
				}
			}

			return false;
		}

		void Win32ClosePipe( HANDLE pPipeHandle, EPipeType pPipeType )
		{
			if( pPipeHandle )
			{
				if( ( pPipeType == EPipeType::PTWrite ) && !Win32IsPipeBroken( pPipeHandle ) )
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

		io_size_t Win32GetPipeAvailableDataSize( HANDLE pPipeHandle )
		{
			if( !pPipeHandle )
			{
				return 0;
			}

			DWORD totalBytesAvailableInPipe = 0;
			DWORD currentMessageBytesLeft = 0;

			// Get both totalBytes and bytesLeftForMessage. This will handle properly
			// all possible pipes in both BYTES and MESSAGE modes (can be either here).
			const auto readResult = ::PeekNamedPipe(
				pPipeHandle,
				nullptr,
				0,
				nullptr,
				&totalBytesAvailableInPipe,
				&currentMessageBytesLeft );

			if( !readResult )
			{
				Platform::WFAGetLastSystemErrorAndPrintToDebugOutput( "PeekNamedPipe" );
			}

			// If there is data left for the message (message-based pipes) use that. Otherwise,
			// simply return the total size of data available in the pipe for reading.
			return ( currentMessageBytesLeft > 0 ) ? currentMessageBytesLeft : totalBytesAvailableInPipe;
		}

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
				WFAGetLastSystemErrorAndPrintToDebugOutput( "CreateNamedPipeA" );
				pipeHandle = nullptr;
			}

			if( pipeHandle )
			{
				const auto startTimeStampCounter = PerfCounter::QueryCounter();

				while( !PerfCounter::CheckTimeoutElapsed( startTimeStampCounter, pTimeoutSettings.waitTimeout ) )
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

							// Wait before the next try using the specified timeout settings.
							std::this_thread::sleep_for( pTimeoutSettings.yieldTimeBetweenRetries.get_std() );
						}
						else
						{
							WFAGetLastSystemErrorAndPrintToDebugOutput( "ConnectNamedPipe" );

							::CloseHandle( pipeHandle );
							pipeHandle = nullptr;

							break;
						}
					}
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

			while( !PerfCounter::CheckTimeoutElapsed( startTimeStampCounter, pTimeoutSettings.waitTimeout ) )
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

			if( pipeHandle == INVALID_HANDLE_VALUE )
			{
				// In case we ended up with an invalid handle, set it to null to indicate the failure.
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

	}
	
}
