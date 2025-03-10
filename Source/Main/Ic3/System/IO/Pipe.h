
#ifndef __IC3_SYSTEM_PIPE_H__
#define __IC3_SYSTEM_PIPE_H__

#include "IOStreamTypes.h"
#include <cppx/immutableString.h>

namespace Ic3::System
{

	Ic3SysDeclareHandle( PipeFactory );

	enum class EPipeType : enum_default_value_t
	{
		Unknown,
		PTWrite,
		PTRead
	};

	struct PipeProperties : public IOStreamProperties
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

		ReadPipeHandle GetReadPipe( const cppx::immutable_string & pPipeName ) const noexcept;

		WritePipeHandle GetWritePipe( const cppx::immutable_string & pPipeName ) const noexcept;

		ReadPipeHandle CreateReadPipe(
			const PipeCreateInfo & pPipeCreateInfo,
			const IOTimeoutSettings & pTimeoutSettings = kIODefaultTimeoutSettings );

		WritePipeHandle CreateWritePipe(
			const PipeCreateInfo & pPipeCreateInfo,
			const IOTimeoutSettings & pTimeoutSettings = kIODefaultTimeoutSettings );

	private:
		virtual ReadPipeHandle _NativeCreateReadPipe(
			const PipeCreateInfo & pPipeCreateInfo,
			const IOTimeoutSettings & pTimeoutSettings ) = 0;

		virtual WritePipeHandle _NativeCreateWritePipe(
			const PipeCreateInfo & pPipeCreateInfo,
			const IOTimeoutSettings & pTimeoutSettings ) = 0;

	private:
		using ReadPipeMap = std::unordered_map<cppx::immutable_string, ReadPipeHandle>;
		using WritePipeMap = std::unordered_map<cppx::immutable_string, WritePipeHandle>;
		ReadPipeMap _readPipeMap;
		WritePipeMap _writePipeMap;
	};

	template <typename TPBaseStream>
	class Pipe : public TPBaseStream
	{
	public:
		EPipeType const mPipeType;
		EPipeDataMode const mPipeDataMode;
		cppx::immutable_string const mFullyQualifiedPipeName;

	public:
		explicit Pipe( SysContextHandle pSysContext, const PipeProperties & pPipeProperties )
		: TPBaseStream( std::move( pSysContext ), pPipeProperties )
		, mPipeType( pPipeProperties.pipeType )
		, mFullyQualifiedPipeName( pPipeProperties.fullyQualifiedPipeName )
		, mPipeDataMode( pPipeProperties.pipeDataMode )
		{}

		virtual ~Pipe() = default;

		CPPX_ATTR_NO_DISCARD virtual bool IsSeekSupported() const noexcept override final
		{
			return false;
		}

		CPPX_ATTR_NO_DISCARD virtual bool IsValid() const noexcept override final
		{
			return _NativePipeIsValid();
		}

		virtual bool Reconnect( const IOTimeoutSettings & pTimeoutSettings ) = 0;

	private:
		virtual bool _NativePipeIsValid() const noexcept = 0;
	};

	class IC3_SYSTEM_CLASS ReadPipe : public Pipe<IOReadOnlyStream>
	{
	public:
		explicit ReadPipe( SysContextHandle pSysContext, const PipeProperties & pPipeProperties );
		virtual ~ReadPipe();

		virtual bool Reconnect( const IOTimeoutSettings & pTimeoutSettings ) override final
		{
			return _NativeReconnectReadPipe( pTimeoutSettings );
		}

		CPPX_ATTR_NO_DISCARD io_size_t GetAvailableDataSize() const override final
		{
			return _NativePipeGetAvailableDataSize();
		}

	private:
		virtual io_size_t ReadImpl( void * pTargetBuffer, io_size_t pReadSize ) override;

		virtual io_size_t _NativePipeReadData( void * pTargetBuffer, io_size_t pReadSize ) = 0;
		virtual bool _NativeReconnectReadPipe( const IOTimeoutSettings & pTimeoutSettings ) = 0;
		virtual io_size_t _NativePipeGetAvailableDataSize() const = 0;
	};

	class IC3_SYSTEM_CLASS WritePipe : public Pipe<IOWriteOnlyStream>
	{
	public:
		explicit WritePipe( SysContextHandle pSysContext, const PipeProperties & pPipeProperties );
		virtual ~WritePipe();

		virtual bool Reconnect( const IOTimeoutSettings & pTimeoutSettings ) override final
		{
			return _NativeReconnectWritePipe( pTimeoutSettings );
		}

	private:
		virtual io_size_t WriteImpl( const void * pData, io_size_t pWriteSize ) override;

		virtual io_size_t _NativePipeWriteData( const void * pData, io_size_t pWriteSize ) = 0;
		virtual bool _NativeReconnectWritePipe( const IOTimeoutSettings & pTimeoutSettings ) = 0;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PIPE_H__
