
#ifndef __IC3_SYSTEM_IO_DATA_STREAM_H__
#define __IC3_SYSTEM_IO_DATA_STREAM_H__

#include "IOCommonDefs.h"
#include "../SysObject.h"
#include <cppx/byteArray.h>

namespace Ic3::System
{

	struct IODataStreamProperties
	{
		EIOAccessMode accessMode;
	};

	class IC3_SYSTEM_CLASS IODataStream : public SysObject
	{
	public:
		EIOAccessMode const mAccessMode;

	public:
		IODataStream( SysContextHandle pSysContext, const IODataStreamProperties & pIODataStreamProperties );
		virtual ~IODataStream();

		CPPX_ATTR_NO_DISCARD virtual bool IsSeekSupported() const noexcept = 0;

		CPPX_ATTR_NO_DISCARD virtual bool IsValid() const noexcept = 0;

		/**
		 * @brief Returns the number of bytes in the stream that are available for reading.
		 * @return The number of bytes in the stream that are available for reading.
		 *
		 * This function works similarly for all types of IO streams (files, pipes and sockets) with one
		 * exception - message-based pipes (EPipeDataMode::MessageStream). For message-based pipes, if the
		 * next message in the pipe is larger than the size specified, the number of remaining bytes is saved
		 * and will be returned upon subsequent calls to this function (until the whole message is read).
		 * When a new message waits in the stream, the returned value is the size of that message.
		 */
		CPPX_ATTR_NO_DISCARD virtual io_size_t GetAvailableDataSize() const = 0;

		CPPX_ATTR_NO_DISCARD bool IsDataAvailable() const noexcept;

		CPPX_ATTR_NO_DISCARD bool CheckAccess( cppx::bitmask<EIOAccessFlags> pAccessFlags ) const noexcept;

		io_size_t Read( void * pTargetBuffer, io_size_t pTargetBufferSize, io_size_t pReadSize = kIOSizeMax );

		io_size_t Read( const cppx::read_write_memory_view & pTarget, io_size_t pReadSize = kIOSizeMax );

		io_size_t ReadAuto( cppx::dynamic_memory_buffer & pTarget, io_size_t pReadSize = kIOSizeMax );

		io_size_t ReadAuto( cppx::dynamic_byte_array & pTarget, io_size_t pReadSize = kIOSizeMax );

		template <typename TResizableBuffer, std::enable_if_t<!std::is_pointer<TResizableBuffer>::value, int>>
		io_size_t ReadAuto( TResizableBuffer & pTarget, io_size_t pReadSize = kIOSizeMax )
		{
			return _ReadAuto( pTarget, pReadSize );
		}

		io_size_t Write( const void * pData, io_size_t pDataSize, io_size_t pWriteSize = kIOSizeMax );

		io_size_t Write( const cppx::read_only_memory_view & pSource, io_size_t pWriteSize = kIOSizeMax );

		io_size_t Write( const cppx::memory_buffer & pSource, io_size_t pWriteSize = kIOSizeMax );

		io_size_t Write( const cppx::byte_array & pSource, io_size_t pWriteSize = kIOSizeMax );

		template <typename TBuffer, std::enable_if_t<!std::is_pointer<TBuffer>::value, int>>
		io_size_t Write( const TBuffer & pSource, io_size_t pWriteSize )
		{
			return _Write( pSource, pWriteSize );
		}

	protected:
		virtual io_size_t ReadImpl( void * pTargetBuffer, io_size_t pReadSize ) = 0;

		virtual io_size_t WriteImpl( const void * pData , io_size_t pWriteSize ) = 0;

	private:
		template <typename TResizableBuffer>
		io_size_t _ReadAuto( TResizableBuffer & pTarget, io_size_t pReadSize )
		{
			const auto remainingDataSize = GetAvailableDataSize();
			const auto readSize = cppx::get_min_of( pReadSize, remainingDataSize );
			if( pReadSize == 0 )
			{
				return 0;
			}

			pTarget.resize( readSize );

			return ReadImpl( pTarget.data(), readSize );
		}

		template <typename TContinuousBuffer>
		io_size_t _Write( const TContinuousBuffer & pBuffer, io_size_t pWriteSize )
		{
			const auto writeSize = cppx::get_min_of( pBuffer.size(), pWriteSize );
			if( pWriteSize == 0 )
			{
				return 0;
			}

			return WriteImpl( pBuffer.data(), writeSize );
		}
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_IO_DATA_STREAM_H__
