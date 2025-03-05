
#include "DataPipe.h"

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

	PipeHandle PipeFactory::CreateNamedPipeObject( const cppx::immutable_string & pPipeName )
	{
		return _NativeCreateNamedPipe( pPipeName );
	}


	Pipe::Pipe( SysContextHandle pSysContext )
	: IODataStream( pSysContext )
	{}

	Pipe::~Pipe() = default;

	io_size_t Pipe::GetRemainingBytes() const noexcept
	{
		return _NativeGetRemainingBytes();
	}

	bool Pipe::IsSeekSupported() const noexcept
	{
		return true;
	}

	io_size_t Pipe::ReadImpl( void * pTargetBuffer, io_size_t pReadSize ) override
	{
		return _NativeReadData( pTargetBuffer, pReadSize );
	}

	io_size_t Pipe::WriteImpl( const void * pData , io_size_t pWriteSize ) override
	{
		return _NativeWriteData( pData, pWriteSize );
	}

} // namespace Ic3::System
