
#ifndef __IC3_SYSTEM_PIPE_H__
#define __IC3_SYSTEM_PIPE_H__

#include "IODataStream.h"
#include <cppx/immutableString.h>

namespace Ic3::System
{

	Ic3SysDeclareHandle( Pipe );
	Ic3SysDeclareHandle( PipeFactory );

	enum class EPipeType : enum_default_value_t
	{
		Unknown,
		PTWrite,
		PTRead
	};

	struct PipeProperties : public IODataStreamProperties
	{
		EPipeType pipeType = EPipeType::Unknown;
		EPipeDataMode pipeDataMode;
		cppx::immutable_string fullyQualifiedPipeName;
	};

	struct PipeCreateInfo
	{
		cppx::immutable_string pipeName;
		EPipeDataMode pipeDataMode = EPipeDataMode::ByteStream;
	};

	class PipeFactory : public SysObject
	{
	public:
		explicit PipeFactory( SysContextHandle pSysContext );
		virtual ~PipeFactory();

		PipeHandle GetPipe(  const cppx::immutable_string & pPipeName  ) const noexcept;

		PipeHandle CreateWritePipe(
			const PipeCreateInfo & pPipeCreateInfo,
			const IOTimeoutSettings & pTimeoutSettings = kIODefaultTimeoutSettings );

		PipeHandle CreateReadPipe(
			const PipeCreateInfo & pPipeCreateInfo,
			const IOTimeoutSettings & pTimeoutSettings = kIODefaultTimeoutSettings );

	private:
		virtual PipeHandle _NativeCreateWritePipe(
			const PipeCreateInfo & pPipeCreateInfo,
			const IOTimeoutSettings & pTimeoutSettings ) = 0;

		virtual PipeHandle _NativeCreateReadPipe(
			const PipeCreateInfo & pPipeCreateInfo,
			const IOTimeoutSettings & pTimeoutSettings ) = 0;

	private:
		using PipeMap = std::unordered_map<cppx::immutable_string, PipeHandle>;
		PipeMap _pipeMap;
	};

	class Pipe : public IODataStream
	{
	public:
		EPipeType const mPipeType;
		EPipeDataMode const mPipeDataMode;
		cppx::immutable_string const mFullyQualifiedPipeName;

	public:
		explicit Pipe( SysContextHandle pSysContext, const PipeProperties & pPipeProperties );
		virtual ~Pipe();

		CPPX_ATTR_NO_DISCARD virtual bool IsSeekSupported() const noexcept override final;

		CPPX_ATTR_NO_DISCARD virtual bool IsValid() const noexcept override final;

		CPPX_ATTR_NO_DISCARD virtual io_size_t GetAvailableDataSize() const override final;

		bool Reconnect( const IOTimeoutSettings & pTimeoutSettings = kIODefaultTimeoutSettings );

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
