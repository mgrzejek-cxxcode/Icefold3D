
#ifndef __IC3_SYSTEM_DATA_PIPE_H__
#define __IC3_SYSTEM_DATA_PIPE_H__

#include "IODataStream.h"
#include <cppx/immutableString.h>

namespace Ic3::System
{

	Ic3SysDeclareHandle( Pipe );
	Ic3SysDeclareHandle( PipeFactory );

	class PipeFactory : public SysObject
	{
	public:
		explicit PipeFactory( SysContextHandle pSysContext );
		virtual ~PipeFactory();

		PipeHandle GetPipe(  const cppx::immutable_string & pPipeName  ) const noexcept;

		PipeHandle CreateNamedPipeObject( const cppx::immutable_string & pPipeName );

	private:
		virtual PipeHandle _NativeCreateNamedPipe( const cppx::immutable_string & pPipeName ) = 0;

	private:
		using PipeMap = std::unordered_map<cppx::immutable_string, PipeHandle>;
		PipeMap _pipeMap;
	};

	class Pipe : public IODataStream
	{
	public:
		explicit Pipe( SysContextHandle pSysContext );
		virtual ~Pipe();

		CPPX_ATTR_NO_DISCARD virtual io_size_t GetRemainingBytes() const noexcept = 0;

		CPPX_ATTR_NO_DISCARD virtual bool IsSeekSupported() const noexcept = 0;

	protected:
		virtual io_size_t ReadImpl( void * pTargetBuffer, io_size_t pReadSize ) override;

		virtual io_size_t WriteImpl( const void * pData , io_size_t pWriteSize ) override;

	private:
		virtual io_size_t _NativeReadData( void * pTargetBuffer, io_size_t pReadSize ) = 0;
		virtual io_size_t _NativeWriteData( const void * pData, io_size_t pWriteSize ) = 0;
		virtual io_size_t _NativeGetRemainingBytes() const = 0;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_DATA_PIPE_H__
