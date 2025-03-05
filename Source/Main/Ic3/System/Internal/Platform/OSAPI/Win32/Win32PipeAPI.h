
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_PIPE_API_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_PIPE_API_H__

#include "Win32Common.h"
#include <Ic3/System/IO/DataPipe.h>

namespace Ic3::System
{

	namespace Platform
	{

		struct Win32PipeNativeData
		{
			HANDLE pipeHandle = nullptr;
		};

	}

	class IC3_SYSTEM_CLASS Win32PipeFactory : public Win32NativeObject<PipeFactory, void>
	{
	public:
		explicit Win32PipeFactory( SysContextHandle pSysContext );
		virtual ~Win32PipeFactory() noexcept;

	private:
		virtual PipeHandle _NativeCreateNamedPipe( const cppx::immutable_string & pPipeName ) override final;
	};

	class IC3_SYSTEM_CLASS Win32Pipe : public Win32NativeObject<Pipe, Platform::Win32PipeNativeData>
	{
	public:
		explicit Win32Pipe( SysContextHandle pSysContext );
		virtual ~Win32Pipe() noexcept;

	private:
		virtual io_size_t _NativeReadData( void * pTargetBuffer, io_size_t pReadSize ) override final;
		virtual io_size_t _NativeWriteData( const void * pData, io_size_t pWriteSize ) override final;
		virtual io_size_t _NativeGetRemainingBytes() const override final;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_PIPE_API_H__
