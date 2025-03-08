
#include "Win32PipeAPI.h"

namespace Ic3::System
{

	Win32PipeFactory::Win32PipeFactory( SysContextHandle pSysContext )
	: Win32NativeObject( std::move( pSysContext ) )
	{}

	Win32PipeFactory::~Win32PipeFactory() noexcept = default;

	PipeHandle Win32PipeFactory::_NativeCreateNamedPipe( const cppx::immutable_string & pPipeName )
	{
		return 0;
	}


	Win32Pipe::Win32Pipe( SysContextHandle pSysContext )
	: Win32NativeObject( std::move( pSysContext ) )
	{}

	Win32Pipe::~Win32Pipe() noexcept = default;

	io_size_t Win32Pipe::_NativeReadData( void * pTargetBuffer, io_size_t pReadSize )
	{
		return 0;
	}

	io_size_t Win32Pipe::_NativeWriteData( const void * pData, io_size_t pWriteSize )
	{
		return 0;
	}

	io_size_t Win32Pipe::_NativeGetRemainingBytes() const
	{
		return 0;
	}
	
}
