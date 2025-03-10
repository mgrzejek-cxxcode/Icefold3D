
#ifndef __IC3_SYSTEM_MESSAGE_PIPE_H__
#define __IC3_SYSTEM_MESSAGE_PIPE_H__

#include "Pipe.h"

namespace Ic3::System
{

	using pipe_message_key_t = uint32;

	inline constexpr pipe_message_key_t kPipeMessageKey = 0xC7D7E7F7;

	struct PipeMessageHeader
	{
		uint32 messageKey;
		uint32 messageSize;
	};

	struct RawPipeMessage
	{
		cppx::dynamic_byte_array bytes;

		void * GetData() noexcept
		{
			return bytes.data();
		}

		size_t GetSize() const noexcept
		{
			return bytes.size();
		}

		void Resize( size_t pBytesNum ) noexcept
		{
			bytes.resize( pBytesNum );
		}
	};

	template <typename TPMessage>
	class MessagePipe
	{
	public:
		using MessageType = TPMessage;

	public:
		explicit MessagePipe( PipeHandle pPipeHandle )
		: _pipeHandle( pPipeHandle )
		{}

		~MessagePipe() = default;

		CPPX_ATTR_NO_DISCARD bool IsValid() const noexcept
		{
			return _pipeHandle->IsValid();
		}

		CPPX_ATTR_NO_DISCARD io_size_t GetAvailableDataSize() const
		{
			return _pipeHandle->GetAvailableDataSize();
		}

		CPPX_ATTR_NO_DISCARD bool IsDataAvailable() const noexcept
		{
			return _pipeHandle->IsDataAvailable();
		}

		bool Reconnect( const IOTimeoutSettings & pTimeoutSettings = kIODefaultTimeoutSettings )
		{
			return _pipeHandle->Reconnect( pTimeoutSettings );
		}

		bool WriteMessage( const TPMessage & pMessage ) noexcept
		{
			if( !_pipeHandle->CheckAccess( eIOAccessFlagOpWrite ) )
			{
				Ic3ThrowDesc( eExcCodeSystemIOBadAccess, "Attempt to write to a read-only pipe" );
			}

			PipeMessageHeader messageHeader;
			messageHeader.messageKey = kPipeMessageKey;
			messageHeader.messageSize = ( uint32 )pMessage.GetSize();

			const auto headerWriteSize = _pipeHandle->Write( &messageHeader, sizeof( messageHeader ) );
			if( headerWriteSize != sizeof( messageHeader ) )
			{
				Ic3DebugInterrupt();
				return false;
			}

			const auto messageWriteSize = _pipeHandle->Write( pMessage.GetData(), pMessage.GetSize() );
			if( messageWriteSize != pMessage.GetSize() )
			{
				Ic3DebugInterrupt();
				return false;
			}

			return true;
		}

		bool ReadMessage( TPMessage & pMessage ) noexcept
		{
			if( !_pipeHandle->CheckAccess( eIOAccessFlagOpRead ) )
			{
				Ic3ThrowDesc( eExcCodeSystemIOBadAccess, "Attempt to read from a write-only pipe" );
			}

			if( !_pipeHandle->IsDataAvailable() )
			{
				return false;
			}

			PipeMessageHeader messageHeader;
			const auto headerReadSize = _pipeHandle->Read( &messageHeader, sizeof( messageHeader ) );
			if( headerReadSize != sizeof( messageHeader ) )
			{
				Ic3DebugInterrupt();
				return false;
			}

			while( !_pipeHandle->IsDataAvailable() )
			{
				std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
			}

			pMessage.Resize( messageHeader.messageSize );
			const auto messageReadSize = _pipeHandle->Read( pMessage.GetData(), pMessage.GetSize() );
			if( messageReadSize != pMessage.GetSize() )
			{
				Ic3DebugInterrupt();
				return false;
			}

			return true;
		}

	private:
		PipeHandle _pipeHandle;
	};

	template <typename TPMessage>
	MessagePipe<TPMessage> CreateMessageReadPipe(
		PipeFactory & pPipeFactory,
		const cppx::immutable_string & pPipeName,
		const IOTimeoutSettings & pTimeoutSettings = {} )
	{
		auto basePipe = pPipeFactory.CreateReadPipe(
			{ pPipeName, EPipeDataMode::MessageStream },
			pTimeoutSettings );
		return MessagePipe<TPMessage>( basePipe );
	}

	template <typename TPMessage>
	MessagePipe<TPMessage> CreateMessageWritePipe(
		PipeFactory & pPipeFactory,
		const cppx::immutable_string & pPipeName,
		const IOTimeoutSettings & pTimeoutSettings = {} )
	{
		auto basePipe = pPipeFactory.CreateWritePipe(
			{ pPipeName, EPipeDataMode::MessageStream },
			pTimeoutSettings );
		return MessagePipe<TPMessage>( basePipe );
	}

} // namespace Ic3::System

#endif // __IC3_SYSTEM_MESSAGE_PIPE_H__
