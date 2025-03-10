
#include "Pipe.h"

namespace Ic3::System
{

	PipeFactory::PipeFactory( SysContextHandle pSysContext )
	: SysObject( std::move( pSysContext ) )
	{}

	PipeFactory::~PipeFactory() = default;

	ReadPipeHandle PipeFactory::GetReadPipe(  const cppx::immutable_string & pPipeName  ) const noexcept
	{
		Ic3DebugInterrupt();
		return nullptr;
	}

	WritePipeHandle PipeFactory::GetWritePipe(  const cppx::immutable_string & pPipeName  ) const noexcept
	{
		Ic3DebugInterrupt();
		return nullptr;
	}

	ReadPipeHandle PipeFactory::CreateReadPipe(
		const PipeCreateInfo & pPipeCreateInfo,
		const IOTimeoutSettings & pTimeoutSettings )
	{
		return _NativeCreateReadPipe( pPipeCreateInfo, pTimeoutSettings );
	}

	WritePipeHandle PipeFactory::CreateWritePipe(
		const PipeCreateInfo & pPipeCreateInfo,
		const IOTimeoutSettings & pTimeoutSettings )
	{
		return _NativeCreateWritePipe( pPipeCreateInfo, pTimeoutSettings );
	}


	ReadPipe::ReadPipe( SysContextHandle pSysContext, const PipeProperties & pPipeProperties )
	: Pipe<IOReadOnlyStream>( std::move( pSysContext ), pPipeProperties )
	{}

	ReadPipe::~ReadPipe() = default;

	io_size_t ReadPipe::ReadImpl( void * pTargetBuffer, io_size_t pReadSize )
	{
		return _NativePipeReadData( pTargetBuffer, pReadSize );
	}


	WritePipe::WritePipe( SysContextHandle pSysContext, const PipeProperties & pPipeProperties )
	: Pipe<IOWriteOnlyStream>( std::move( pSysContext ), pPipeProperties )
	{}

	WritePipe::~WritePipe() = default;

	io_size_t WritePipe::WriteImpl( const void * pData, io_size_t pWriteSize )
	{
		return _NativePipeWriteData( pData, pWriteSize );
	}

} // namespace Ic3::System
