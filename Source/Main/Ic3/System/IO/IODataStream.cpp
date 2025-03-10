
#include "IODataStream.h"

namespace Ic3::System
{

    IODataStream::IODataStream( SysContextHandle pSysContext, const IODataStreamProperties & pIODataStreamProperties )
    : SysObject( pSysContext )
    , mAccessMode( pIODataStreamProperties.accessMode )
    {}

    IODataStream::~IODataStream() = default;

    bool IODataStream::IsDataAvailable() const noexcept
    {
        return GetAvailableDataSize() > 0;
    }

    bool IODataStream::CheckAccess( cppx::bitmask<EIOAccessFlags> pAccessFlags ) const noexcept
    {
        return cppx::make_bitmask( mAccessMode ).is_set( pAccessFlags );
    }

    io_size_t IODataStream::Read( void * pTargetBuffer, io_size_t pTargetBufferSize, io_size_t pReadSize )
    {
		const auto readSize = cppx::get_min_of( pReadSize, pTargetBufferSize );
        if( readSize == 0 )
        {
            return 0;
        }

        return ReadImpl( pTargetBuffer, readSize );
    }

    io_size_t IODataStream::Read( const cppx::read_write_memory_view & pTarget, io_size_t pReadSize )
    {
        return Read( pTarget.data(), pTarget.size(), pReadSize );
    }

    io_size_t IODataStream::ReadAuto( cppx::dynamic_memory_buffer & pTarget, io_size_t pReadSize )
    {
        return _ReadAuto( pTarget, pReadSize );
    }

    io_size_t IODataStream::ReadAuto( cppx::dynamic_byte_array & pTarget, io_size_t pReadSize )
    {
        return _ReadAuto( pTarget, pReadSize );
    }

	io_size_t IODataStream::Write( const void * pData, io_size_t pDataSize, io_size_t pWriteSize )
    {
        const auto writeSize = cppx::get_min_of( pWriteSize, pDataSize );
        if( writeSize == 0 )
        {
            return 0;
        }

        return WriteImpl( pData, writeSize );
    }

    io_size_t IODataStream::Write( const cppx::read_only_memory_view & pSource, io_size_t pWriteSize )
    {
        return _Write( pSource, pWriteSize );
    }

    io_size_t IODataStream::Write( const cppx::memory_buffer & pSource, io_size_t pWriteSize )
    {
        return _Write( pSource, pWriteSize );
    }

    io_size_t IODataStream::Write( const cppx::byte_array & pSource, io_size_t pWriteSize )
    {
        return _Write( pSource, pWriteSize );
    }

} // namespace Ic3::System
