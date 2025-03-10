
#ifndef __CPPX_BYTE_ARRAY_H__
#define __CPPX_BYTE_ARRAY_H__

#include "memoryBuffer.h"

namespace cppx
{

	class byte_array
	{
	public:
		byte_array( const byte_array & ) = delete;
		byte_array & operator=( const byte_array & ) = delete;

		byte_array() noexcept
		: _storageMemoryPtr( nullptr )
		, _dataSize( 0u )
		{}

		byte_array( byte_array && pSrcObject ) noexcept
		: _storageMemoryPtr( pSrcObject._storageMemoryPtr )
		, _dataSize( pSrcObject._dataSize )
		{
			pSrcObject._storageMemoryPtr = nullptr;
			pSrcObject._dataSize = 0u;
		}

		byte_array( void * pBufferBasePtr, size_t pBufferLength ) noexcept
		: _storageMemoryPtr( reinterpret_cast<byte *>( pBufferBasePtr ) )
		, _dataSize( pBufferLength )
		{}

		explicit operator bool() const
		{
			return _storageMemoryPtr && ( _dataSize > 0 );
		}

		CPPX_ATTR_NO_DISCARD read_write_memory_view as_memory_view() noexcept
		{
			return bind_memory_view( _storageMemoryPtr, _dataSize );
		}

		CPPX_ATTR_NO_DISCARD read_only_memory_view as_memory_view() const noexcept
		{
			return bind_memory_view( _storageMemoryPtr, _dataSize );
		}

		CPPX_ATTR_NO_DISCARD byte & operator[]( size_t pOffset ) noexcept
		{
			cppx_debug_assert( pOffset < _dataSize );
			return _storageMemoryPtr[pOffset];
		}

		CPPX_ATTR_NO_DISCARD const byte & operator[]( size_t pOffset ) const noexcept
		{
			cppx_debug_assert( pOffset < _dataSize );
			return _storageMemoryPtr[pOffset];
		}

		CPPX_ATTR_NO_DISCARD byte * data() noexcept
		{
			return _storageMemoryPtr;
		}

		CPPX_ATTR_NO_DISCARD const byte * data() const noexcept
		{
			return _storageMemoryPtr;
		}

		CPPX_ATTR_NO_DISCARD byte * data_offset( size_t pOffset ) noexcept
		{
			cppx_debug_assert( pOffset < _dataSize );
			return _storageMemoryPtr + pOffset;
		}

		CPPX_ATTR_NO_DISCARD const byte * data_offset( size_t pOffset ) const noexcept
		{
			cppx_debug_assert( pOffset < _dataSize );
			return _storageMemoryPtr + pOffset;
		}

		template <typename TPResult>
		CPPX_ATTR_NO_DISCARD TPResult * data_as() noexcept
		{
			cppx_debug_assert( _dataSize % sizeof( TPResult ) == 0 );
			return reinterpret_cast<TPResult *>( data() );
		}

		template <typename TPResult>
		CPPX_ATTR_NO_DISCARD const TPResult * data_as() const noexcept
		{
			cppx_debug_assert( _dataSize % sizeof( TPResult ) == 0 );
			return reinterpret_cast<TPResult *>( data() );
		}

		template <typename TPResult>
		CPPX_ATTR_NO_DISCARD TPResult * data_offset_as( size_t pOffset ) noexcept
		{
			cppx_debug_assert( ( _dataSize - pOffset ) % sizeof( TPResult ) == 0 );
			return reinterpret_cast<TPResult *>( data_offset( pOffset ) );
		}

		template <typename TPResult>
		CPPX_ATTR_NO_DISCARD const TPResult * data_offset_as( size_t pOffset ) const noexcept
		{
			cppx_debug_assert( ( _dataSize - pOffset ) % sizeof( TPResult ) == 0 );
			return reinterpret_cast<TPResult *>( data_offset( pOffset ) );
		}

		CPPX_ATTR_NO_DISCARD bool empty() const noexcept
		{
			return !_storageMemoryPtr || ( _dataSize == 0 );
		}

		CPPX_ATTR_NO_DISCARD size_t size() const noexcept
		{
			return _dataSize;
		}

		size_t fill( byte pValue, size_t pFillCount = cve::max_size, size_t pFillOffset = 0 )
		{
			if( pFillOffset >= _dataSize )
			{
				return 0;
			}

			const auto bufferCapacity = _dataSize - pFillOffset;
			pFillCount = get_min_of( pFillCount, bufferCapacity );
			mem_set_fill_checked( _storageMemoryPtr + pFillOffset, bufferCapacity, pValue, pFillCount );
			_dataSize = pFillCount;

			return pFillCount;
		}

		size_t set_data( const void * pData, size_t pDataSize, size_t pSetOffset = 0 )
		{
			if( !pData || ( pDataSize == 0 ) || ( pSetOffset >= _dataSize ) )
			{
				return 0;
			}

			const auto bufferCapacity = _dataSize - pSetOffset;
			pSetOffset = get_min_of( pSetOffset, bufferCapacity );
			mem_copy_unchecked( _storageMemoryPtr + pSetOffset, bufferCapacity, pData, pDataSize );
			_dataSize = pDataSize;

			return pDataSize;
		}

		size_t copy( void * pBuffer, size_t pBufferSize, size_t pCopySize, size_t pCopyOffset = 0 )
		{
			if( !pBuffer || ( pBufferSize == 0 ) || ( pCopyOffset >= _dataSize ) )
			{
				return 0;
			}

			pCopySize = get_min_of( pCopySize, pBufferSize );
			pCopySize = get_min_of( pCopySize, _dataSize - pCopyOffset );

			if( pCopySize > 0 )
			{
				mem_copy_unchecked( pBuffer, pBufferSize, _storageMemoryPtr + pCopyOffset, pCopySize );
			}

			return pCopySize;
		}

	protected:
		void update_storage( void * pBufferBasePtr )
		{
			_storageMemoryPtr = reinterpret_cast<byte *>( pBufferBasePtr );
		}

	protected:
		byte * _storageMemoryPtr;
		size_t _dataSize;
	};

	inline read_write_memory_view bind_memory_view( byte_array & pByteArray )
	{
		return { pByteArray.data(), pByteArray.size() };
	}

	inline read_only_memory_view bind_memory_view( const byte_array & pByteArray )
	{
		return { pByteArray.data(), pByteArray.size() };
	}

	class dynamic_byte_array : public byte_array
	{
	public:
		dynamic_byte_array() = default;

		dynamic_byte_array( dynamic_byte_array && pSrcObject ) noexcept
		: byte_array( std::move( pSrcObject ) )
		, _internalBuffer( std::move( pSrcObject._internalBuffer ) )
		{}

		dynamic_byte_array( dynamic_memory_buffer pInternalBuffer, size_t pSize = cve::max_size )
		: _internalBuffer( std::move( pInternalBuffer ) )
		{
			_storageMemoryPtr = _internalBuffer.data();
			_dataSize = get_min_of( _internalBuffer.size(), pSize );
		}

		explicit dynamic_byte_array( size_t pSize )
		{
			resize( pSize );
		}

		dynamic_byte_array & operator=( dynamic_byte_array && pRhs ) noexcept
		{
			dynamic_byte_array( std::move( pRhs ) ).swap( *this );
			return *this;
		}

		CPPX_ATTR_NO_DISCARD dynamic_byte_array clone() const
		{
			dynamic_byte_array newArray;
			newArray.assign( *this );
			return newArray;
		}

		dynamic_byte_array & assign( const dynamic_byte_array & pOther )
		{
			resize( pOther.size() );
			set_data( pOther.data(), pOther.size(), 0 );
			return *this;
		}

		void set_size( size_t pNewSize )
		{
			pNewSize = get_min_of( pNewSize, _internalBuffer.size() );
			_dataSize = pNewSize;
		}

		void resize( size_t pNewSize )
		{
			if( pNewSize > _internalBuffer.size() )
			{
				_resize_storage( pNewSize );
			}
			_dataSize = pNewSize;
		}

		void shrink_to_fit()
		{
			if( _internalBuffer.size() > _dataSize )
			{
				_resize_storage( _dataSize );
			}
		}

		bool optimize_storage( float pAllowedFactor = 0.5f )
		{
			const auto storageCapacity = _internalBuffer.size();
			const auto exceedFactor = static_cast<float>( storageCapacity - _dataSize ) / _dataSize;

			if( exceedFactor > pAllowedFactor )
			{
				_resize_storage( _dataSize );
				return true;
			}

			return false;
		}

		void clear()
		{
			_dataSize = 0;
		}

		void release()
		{
			_internalBuffer.release();
			_dataSize = 0;
		}

		void swap( dynamic_byte_array & pOther )
		{
			cppx::swap( _internalBuffer, pOther._internalBuffer );
			std::swap( _storageMemoryPtr, pOther._storageMemoryPtr );
			std::swap( _dataSize, pOther._dataSize );
		}

	private:
		void _resize_storage( size_t pCapacity )
		{
			_internalBuffer.resize( pCapacity );
			update_storage( _internalBuffer.data());
		}

	private:
		dynamic_memory_buffer _internalBuffer;
	};

	inline void swap( dynamic_byte_array & pFirst, dynamic_byte_array & pSecond )
	{
		pFirst.swap( pSecond );
	}

	template <size_t tpSize>
	class fixed_byte_array : public byte_array
	{
	public:
		fixed_byte_array( const fixed_byte_array & ) = delete;
		fixed_byte_array & operator=( const fixed_byte_array & ) = delete;

		fixed_byte_array()
		{
			_storageMemoryPtr = _internalBuffer.data();
			_dataSize = _internalBuffer.size();
		}

	private:
		fixed_memory_buffer<tpSize> _internalBuffer;
	};

}

#endif /* __CPPX_BYTE_ARRAY_H__ */
