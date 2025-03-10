
#include "Pipe.h"

namespace Ic3::System
{

	PipeFactory::PipeFactory( SysContextHandle pSysContext )
	: SysObject( std::move( pSysContext ) )
	{}

	PipeFactory::~PipeFactory() = default;

	PipeHandle PipeFactory::GetPipe(  const cppx::immutable_string & pPipeName  ) const noexcept
	{
		Ic3DebugInterrupt();
		return nullptr;
	}

	PipeHandle PipeFactory::CreateNamedPipeObject(
			const cppx::immutable_string & pPipeName,
			EPipeDataMode pPipeDataMode,
			EIOAccessMode pAccessMode,
			const IOTimeoutSettings & pTimeoutSettings )
	{
		if( pAccessMode == EIOAccessMode::ReadWrite )
		{
			Ic3DebugOutput( "EIOAccessMode::ReadWrite is not a valid access for pipes" );
			return nullptr;
		}
		else
		{
			if( pAccessMode == EIOAccessMode::ReadOnly )
			{
				return _NativeCreateNamedPipeReadAccess( pPipeName, pPipeDataMode, pTimeoutSettings );
			}
			else
			{
				return _NativeCreateNamedPipeWriteAccess( pPipeName, pPipeDataMode, pTimeoutSettings );
			}
		}
	}


	Pipe::Pipe( SysContextHandle pSysContext, const PipeProperties & pPipeProperties )
	: IODataStream( std::move( pSysContext ), pPipeProperties )
	, mFullyQualifiedPipeName( pPipeProperties.fullyQualifiedPipeName )
	, mPipeDataMode( pPipeProperties.pipeDataMode )
	{}

	Pipe::~Pipe() = default;

	bool Pipe::IsSeekSupported() const noexcept
	{
		return false;
	}

	bool Pipe::IsValid() const noexcept
	{
		return _NativePipeIsValid();
	}

	io_size_t Pipe::GetAvailableDataSize() const
	{
		return _NativePipeGetAvailableDataSize();
	}

	bool Pipe::ReconnectReadPipe( const IOTimeoutSettings & pTimeoutSettings )
	{
		if( !CheckAccess( eIOAccessFlagOpRead ) )
		{
			Ic3DebugInterrupt();
			return false;
		}

		return _NativeReconnectReadPipe( pTimeoutSettings );
	}

	bool Pipe::ReconnectWritePipe( const IOTimeoutSettings & pTimeoutSettings )
	{
		if( !CheckAccess( eIOAccessFlagOpWrite ) )
		{
			Ic3DebugInterrupt();
			return false;
		}

		return _NativeReconnectWritePipe( pTimeoutSettings );
	}

	io_size_t Pipe::ReadImpl( void * pTargetBuffer, io_size_t pReadSize )
	{
		return _NativePipeReadData( pTargetBuffer, pReadSize );
	}

	io_size_t Pipe::WriteImpl( const void * pData , io_size_t pWriteSize )
	{
		return _NativePipeWriteData( pData, pWriteSize );
	}

} // namespace Ic3::System
