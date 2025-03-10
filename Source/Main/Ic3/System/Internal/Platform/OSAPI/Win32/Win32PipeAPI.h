
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

		bool Win32IsPipeBroken( HANDLE pPipeHandle );

		void Win32ClosePipe( HANDLE pPipeHandle, EPipeType pPipeType );

		io_size_t Win32GetPipeAvailableDataSize( HANDLE pPipeHandle );

	}


	/**
	 *
	 */
	class IC3_SYSTEM_CLASS Win32PipeFactory : public Win32NativeObject<PipeFactory, void>
	{
	public:
		explicit Win32PipeFactory( SysContextHandle pSysContext );
		virtual ~Win32PipeFactory() noexcept;

	private:
		virtual ReadPipeHandle _NativeCreateReadPipe(
			const PipeCreateInfo & pPipeCreateInfo,
			const IOTimeoutSettings & pTimeoutSettings ) override final;

		virtual WritePipeHandle _NativeCreateWritePipe(
			const PipeCreateInfo & pPipeCreateInfo,
			const IOTimeoutSettings & pTimeoutSettings ) override final;
	};

	/**
	 *
	 * @tparam TPBasePipe
	 */
	template <typename TPBasePipe>
	class Win32BasePipe : public Win32NativeObject<TPBasePipe, Platform::Win32PipeNativeData>
	{
	public:
		explicit Win32BasePipe( SysContextHandle pSysContext, const PipeProperties & pPipeProperties )
		: Win32NativeObject<TPBasePipe, Platform::Win32PipeNativeData>( std::move( pSysContext ), pPipeProperties )
		{}

		virtual ~Win32BasePipe() noexcept
		{
			ReleaseWin32PipeHandle();
		}

		bool IsPipeBroken() const noexcept
		{
			return Platform::Win32IsPipeBroken( this->mNativeData.pipeHandle );
		}

		void SetWin32PipeHandle( HANDLE pPipeHandle )
		{
			this->mNativeData.pipeHandle = pPipeHandle;
		}

	protected:
		void ReleaseWin32PipeHandle()
		{
			if( this->mNativeData.pipeHandle )
			{
				Platform::Win32ClosePipe( this->mNativeData.pipeHandle, this->mPipeType );
				this->mNativeData.pipeHandle = nullptr;
			}
		}

	private:
		virtual bool _NativePipeIsValid() const noexcept override final
		{
			return this->mNativeData.pipeHandle && !IsPipeBroken();
		}
	};

	/**
	 *
	 */
	class IC3_SYSTEM_CLASS Win32ReadPipe : public Win32BasePipe<ReadPipe>
	{
	public:
		explicit Win32ReadPipe( SysContextHandle pSysContext, const PipeProperties & pPipeProperties );
		virtual ~Win32ReadPipe() noexcept;

	private:
		virtual io_size_t _NativePipeReadData( void * pTargetBuffer, io_size_t pReadSize ) override final;
		virtual bool _NativeReconnectReadPipe( const IOTimeoutSettings & pTimeoutSettings ) override final;
		virtual io_size_t _NativePipeGetAvailableDataSize() const override final;
	};

	/**
	 *
	 */
	class IC3_SYSTEM_CLASS Win32WritePipe : public Win32BasePipe<WritePipe>
	{
	public:
		explicit Win32WritePipe( SysContextHandle pSysContext, const PipeProperties & pPipeProperties );
		virtual ~Win32WritePipe() noexcept;

	private:
		virtual io_size_t _NativePipeWriteData( const void * pData, io_size_t pWriteSize ) override final;
		virtual bool _NativeReconnectWritePipe( const IOTimeoutSettings & pTimeoutSettings ) override final;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_PIPE_API_H__
