
#ifndef __IC3_SYSTEM_PIPE_H__
#define __IC3_SYSTEM_PIPE_H__

#include "IODataStream.h"
#include <cppx/immutableString.h>

namespace Ic3::System
{

	Ic3SysDeclareHandle( Pipe );
	Ic3SysDeclareHandle( PipeFactory );

	struct PipeProperties : public IODataStreamProperties
	{
		cppx::immutable_string fullyQualifiedPipeName;
		EPipeDataMode pipeDataMode;
	};

	class PipeFactory : public SysObject
	{
	public:
		explicit PipeFactory( SysContextHandle pSysContext );
		virtual ~PipeFactory();

		PipeHandle GetPipe(  const cppx::immutable_string & pPipeName  ) const noexcept;

		PipeHandle CreateNamedPipeObject(
			const cppx::immutable_string & pPipeName,
			EPipeDataMode pPipeDataMode,
			EIOAccessMode pAccessMode,
			const IOTimeoutSettings & pTimeoutSettings = kIODefaultTimeoutSettings );

	private:
		virtual PipeHandle _NativeCreateNamedPipeReadAccess(
			const cppx::immutable_string & pPipeName,
			EPipeDataMode pPipeDataMode,
			const IOTimeoutSettings & pTimeoutSettings ) = 0;

		virtual PipeHandle _NativeCreateNamedPipeWriteAccess(
			const cppx::immutable_string & pPipeName,
			EPipeDataMode pPipeDataMode,
			const IOTimeoutSettings & pTimeoutSettings ) = 0;

	private:
		using PipeMap = std::unordered_map<cppx::immutable_string, PipeHandle>;
		PipeMap _pipeMap;
	};

	class Pipe : public IODataStream
	{
	public:
		cppx::immutable_string const mFullyQualifiedPipeName;
		EPipeDataMode const mPipeDataMode;

	public:
		explicit Pipe( SysContextHandle pSysContext, const PipeProperties & pPipeProperties );
		virtual ~Pipe();

		CPPX_ATTR_NO_DISCARD virtual bool IsSeekSupported() const noexcept override final;

		CPPX_ATTR_NO_DISCARD virtual bool IsValid() const noexcept override final;

		CPPX_ATTR_NO_DISCARD virtual io_size_t GetAvailableDataSize() const override final;

		bool ReconnectReadPipe( const IOTimeoutSettings & pTimeoutSettings = kIODefaultTimeoutSettings );
		bool ReconnectWritePipe( const IOTimeoutSettings & pTimeoutSettings = kIODefaultTimeoutSettings );

	private:
		virtual io_size_t ReadImpl( void * pTargetBuffer, io_size_t pReadSize ) override;
		virtual io_size_t WriteImpl( const void * pData , io_size_t pWriteSize ) override;

		virtual bool _NativePipeIsValid() const noexcept = 0;
		virtual io_size_t _NativePipeGetAvailableDataSize() const = 0;
		virtual io_size_t _NativePipeReadData( void * pTargetBuffer, io_size_t pReadSize ) = 0;
		virtual io_size_t _NativePipeWriteData( const void * pData, io_size_t pWriteSize ) = 0;
		virtual bool _NativeReconnectReadPipe( const IOTimeoutSettings & pTimeoutSettings ) = 0;
		virtual bool _NativeReconnectWritePipe( const IOTimeoutSettings & pTimeoutSettings ) = 0;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PIPE_H__
