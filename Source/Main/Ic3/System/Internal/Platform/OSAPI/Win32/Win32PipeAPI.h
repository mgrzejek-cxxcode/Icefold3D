
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_PIPE_API_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_PIPE_API_H__

#include "Win32Common.h"

#include <Ic3/System/IO/Pipe.h>

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
		virtual PipeHandle _NativeCreateNamedPipeReadAccess(
			const cppx::immutable_string & pPipeName,
			EPipeDataMode pPipeDataMode,
			const IOTimeoutSettings & pTimeoutSettings ) override final;

		virtual PipeHandle _NativeCreateNamedPipeWriteAccess(
			const cppx::immutable_string & pPipeName,
			EPipeDataMode pPipeDataMode,
			const IOTimeoutSettings & pTimeoutSettings ) override final;
	};

	class IC3_SYSTEM_CLASS Win32Pipe : public Win32NativeObject<Pipe, Platform::Win32PipeNativeData>
	{
	public:
		explicit Win32Pipe( SysContextHandle pSysContext, const PipeProperties & pPipeProperties );
		virtual ~Win32Pipe() noexcept;

		void SetInternalWin32PipeHandle( HANDLE pPipeHandle );

	private:
		virtual bool _NativePipeIsValid() const noexcept override final;
		virtual io_size_t _NativePipeGetAvailableDataSize() const override final;
		virtual io_size_t _NativePipeReadData( void * pTargetBuffer, io_size_t pReadSize ) override final;
		virtual io_size_t _NativePipeWriteData( const void * pData, io_size_t pWriteSize ) override final;
		virtual bool _NativeReconnectReadPipe( const IOTimeoutSettings & pTimeoutSettings ) override final;
		virtual bool _NativeReconnectWritePipe( const IOTimeoutSettings & pTimeoutSettings ) override final;

		void _ReleaseWin32PipeHandle( bool pIsBrokenPipe = false );
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_PIPE_API_H__
