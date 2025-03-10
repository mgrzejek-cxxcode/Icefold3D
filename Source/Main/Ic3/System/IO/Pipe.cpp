
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

	PipeHandle PipeFactory::CreateWritePipe(
		const PipeCreateInfo & pPipeCreateInfo,
		const IOTimeoutSettings & pTimeoutSettings )
	{
		return _NativeCreateWritePipe( pPipeCreateInfo, pTimeoutSettings );
	}

	PipeHandle PipeFactory::CreateReadPipe(
		const PipeCreateInfo & pPipeCreateInfo,
		const IOTimeoutSettings & pTimeoutSettings )
	{
		return _NativeCreateReadPipe( pPipeCreateInfo, pTimeoutSettings );
	}


	Pipe::Pipe( SysContextHandle pSysContext, const PipeProperties & pPipeProperties )
	: IODataStream( std::move( pSysContext ), pPipeProperties )
	, mPipeType( pPipeProperties.pipeType )
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

	bool Pipe::Reconnect( const IOTimeoutSettings & pTimeoutSettings )
	{
		if( mPipeType == EPipeType::PTWrite )
		{
			return _NativeReconnectWritePipe( pTimeoutSettings );
		}
		else
		{
			return _NativeReconnectReadPipe( pTimeoutSettings );
		}
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
