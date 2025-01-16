
#ifndef __CPPX_MEMORY_BUFFER_H__
#define __CPPX_MEMORY_BUFFER_H__

#include "arrayView.h"
#include "memory.h"

namespace cppx
{

	class memory_buffer
	{
	public:
		memory_buffer( const memory_buffer & ) = delete;
		memory_buffer & operator=( const memory_buffer & ) = delete;

		memory_buffer() noexcept
		: _bufferBasePtr( nullptr )
		, _bufferLength( 0u )
		{}

		memory_buffer( memory_buffer && pSrcObject ) noexcept
		: _bufferBasePtr( pSrcObject._bufferBasePtr )
		, _bufferLength( pSrcObject._bufferLength )
		{
			pSrcObject._bufferBasePtr = nullptr;
			pSrcObject._bufferLength = 0u;
		}

		memory_buffer( void * pMemory, size_t pMemorySize ) noexcept
		: _bufferBasePtr( reinterpret_cast<byte *>( pMemory ) )
		, _bufferLength( pMemorySize )
		{}

		CPPX_ATTR_NO_DISCARD byte & operator[]( size_t pOffset ) noexcept
		{
			cppx_debug_assert( pOffset < _bufferLength );
			return _bufferBasePtr[pOffset];
		}

		CPPX_ATTR_NO_DISCARD const byte & operator[]( size_t pOffset ) const noexcept
		{
			cppx_debug_assert( pOffset < _bufferLength );
			return _bufferBasePtr[pOffset];
		}

		CPPX_ATTR_NO_DISCARD read_write_memory_view as_memory_view() noexcept
		{
			return bind_memory_view( _bufferBasePtr, _bufferLength );
		}

		CPPX_ATTR_NO_DISCARD read_only_memory_view as_memory_view() const noexcept
		{
			return bind_memory_view( _bufferBasePtr, _bufferLength );
		}

		CPPX_ATTR_NO_DISCARD byte * data() noexcept
		{
			cppx_debug_assert( _bufferLength > 0 );
			return _bufferBasePtr;
		}

		CPPX_ATTR_NO_DISCARD const byte * data() const noexcept
		{
			cppx_debug_assert( _bufferLength > 0 );
			return _bufferBasePtr;
		}

		CPPX_ATTR_NO_DISCARD byte * data( size_t pOffset ) noexcept
		{
			cppx_debug_assert( ( _bufferLength > 0 ) && ( pOffset < _bufferLength ) );
			return _bufferBasePtr + pOffset;
		}

		CPPX_ATTR_NO_DISCARD const byte * data( size_t pOffset ) const noexcept
		{
			cppx_debug_assert( ( _bufferLength > 0 ) && ( pOffset < _bufferLength ) );
			return _bufferBasePtr + pOffset;
		}

		CPPX_ATTR_NO_DISCARD bool empty() const noexcept
		{
			return ( _bufferBasePtr == nullptr ) && ( _bufferLength == 0 );
		}

		CPPX_ATTR_NO_DISCARD size_t size() const noexcept
		{
			return _bufferLength;
		}

	protected:
		byte * _bufferBasePtr;
		size_t _bufferLength;
	};

	inline read_write_memory_view bind_memory_view( memory_buffer & pMemoryBuffer )
	{
		return read_write_memory_view( pMemoryBuffer.data(), pMemoryBuffer.size() );
	}

	inline read_only_memory_view bind_memory_view( const memory_buffer & pMemoryBuffer )
	{
		return read_only_memory_view( pMemoryBuffer.data(), pMemoryBuffer.size() );
	}

	class dynamic_memory_buffer : public memory_buffer
	{
	public:
		dynamic_memory_buffer( const dynamic_memory_buffer & ) = delete;
		dynamic_memory_buffer & operator=( const dynamic_memory_buffer & ) = delete;

		dynamic_memory_buffer()
		: memory_buffer()
		, _memRoutines( cve::default_memory_routines )
		{}

		dynamic_memory_buffer( std::nullptr_t )
		: memory_buffer()
		, _memRoutines( cve::default_memory_routines )
		{}

		explicit dynamic_memory_buffer( memory_routines_proxy pMemoryRoutines )
		: _memRoutines( pMemoryRoutines ? std::move( pMemoryRoutines ) : cve::default_memory_routines )
		{}

		dynamic_memory_buffer( dynamic_memory_buffer && pSrcObject ) noexcept
		: memory_buffer( std::move( pSrcObject ) )
		, _memRoutines( std::move( pSrcObject._memRoutines ) )
		{}

		dynamic_memory_buffer & operator=( dynamic_memory_buffer && pRhs ) noexcept
		{
			dynamic_memory_buffer( std::move( pRhs ) ).swap( *this );
			return *this;
		}

		explicit operator bool() const
		{
			return _bufferBasePtr && ( _bufferLength > 0 );
		}

		size_t resize( size_t pNewSize )
		{
			pNewSize = mem_get_aligned_value( pNewSize, _memRoutines.mem_alignment );
			if( pNewSize == _bufferLength )
			{
				return _bufferLength;
			}
			else if( pNewSize == 0 )
			{
				release();
				return 0;
			}
			else
			{
				auto * newMemoryPtr = std::realloc( _bufferBasePtr, pNewSize );
				_bufferBasePtr = reinterpret_cast<byte *>( newMemoryPtr );
				_bufferLength = pNewSize;
				return _bufferLength;
			}
		}

		void release()
		{
			if( _bufferBasePtr != nullptr )
			{
				_memRoutines.fn_free( _bufferBasePtr );
				_bufferBasePtr = nullptr;
				_bufferLength = 0;
			}
		}

		void swap( dynamic_memory_buffer & pOther )
		{
			std::swap( _bufferBasePtr, pOther._bufferBasePtr );
			std::swap( _bufferLength, pOther._bufferLength );
			cppx::swap( _memRoutines, pOther._memRoutines );
		}

	private:
		memory_routines_proxy _memRoutines;
	};

	inline void swap( dynamic_memory_buffer & pFirst, dynamic_memory_buffer & pSecond )
	{
		pFirst.swap( pSecond );
	}

	template <size_t tpSize>
	class fixed_memory_buffer : public memory_buffer
	{
	public:
		fixed_memory_buffer( const fixed_memory_buffer & ) = delete;
		fixed_memory_buffer & operator=( const fixed_memory_buffer & ) = delete;

		fixed_memory_buffer()
		: memory_buffer( &( _storage[0] ), tpSize )
		{}

	private:
		byte _storage[tpSize];
	};

}

#endif /* __CPPX_MEMORY_BUFFER_H__ */
