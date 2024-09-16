
#ifndef __IC3_CPPX_BYTE_ARRAY_H__
#define __IC3_CPPX_BYTE_ARRAY_H__

#include "MemoryBuffer.h"

namespace Ic3::Cppx
{

	class ByteArray
	{
	public:
		ByteArray( const ByteArray & ) = delete;
		ByteArray & operator=( const ByteArray & ) = delete;

		ByteArray() noexcept
		: _storageMemoryPtr( nullptr )
		, _dataSize( 0u )
		{}

		ByteArray( ByteArray && pSrcObject ) noexcept
		: _storageMemoryPtr( pSrcObject._storageMemoryPtr )
		, _dataSize( pSrcObject._dataSize )
		{
			pSrcObject._storageMemoryPtr = nullptr;
			pSrcObject._dataSize = 0u;
		}

		ByteArray( void * pBufferBasePtr, size_t pBufferLength ) noexcept
		: _storageMemoryPtr( reinterpret_cast<byte *>( pBufferBasePtr ) )
		, _dataSize( pBufferLength )
		{}

		explicit operator bool() const
		{
			return _storageMemoryPtr && ( _dataSize > 0 );
		}

		IC3_ATTR_NO_DISCARD ReadWriteMemoryView asMemoryView() noexcept
		{
			return bindMemoryView( _storageMemoryPtr, _dataSize );
		}

		IC3_ATTR_NO_DISCARD ReadOnlyMemoryView asMemoryView() const noexcept
		{
			return bindMemoryView( _storageMemoryPtr, _dataSize );
		}

		IC3_ATTR_NO_DISCARD byte & operator[]( size_t pOffset ) noexcept
		{
			ic3DebugAssert( pOffset < _dataSize );
			return _storageMemoryPtr[pOffset];
		}

		IC3_ATTR_NO_DISCARD const byte & operator[]( size_t pOffset ) const noexcept
		{
			ic3DebugAssert( pOffset < _dataSize );
			return _storageMemoryPtr[pOffset];
		}

		IC3_ATTR_NO_DISCARD byte * data() noexcept
		{
			return _storageMemoryPtr;
		}

		IC3_ATTR_NO_DISCARD const byte * data() const noexcept
		{
			return _storageMemoryPtr;
		}

		IC3_ATTR_NO_DISCARD byte * dataOffset( size_t pOffset ) noexcept
		{
			ic3DebugAssert( pOffset < _dataSize );
			return _storageMemoryPtr + pOffset;
		}

		IC3_ATTR_NO_DISCARD const byte * dataOffset( size_t pOffset ) const noexcept
		{
			ic3DebugAssert( pOffset < _dataSize );
			return _storageMemoryPtr + pOffset;
		}

		template <typename TPResult>
		IC3_ATTR_NO_DISCARD TPResult * dataAs() noexcept
		{
			ic3DebugAssert( _dataSize % sizeof( TPResult ) == 0 );
			return reinterpret_cast<TPResult *>( data() );
		}

		template <typename TPResult>
		IC3_ATTR_NO_DISCARD const TPResult * dataAs() const noexcept
		{
			ic3DebugAssert( _dataSize % sizeof( TPResult ) == 0 );
			return reinterpret_cast<TPResult *>( data() );
		}

		template <typename TPResult>
		IC3_ATTR_NO_DISCARD TPResult * dataOffsetAs( size_t pOffset ) noexcept
		{
			ic3DebugAssert( ( _dataSize - pOffset ) % sizeof( TPResult ) == 0 );
			return reinterpret_cast<TPResult *>( dataOffset( pOffset ) );
		}

		template <typename TPResult>
		IC3_ATTR_NO_DISCARD const TPResult * dataOffsetAs( size_t pOffset ) const noexcept
		{
			ic3DebugAssert( ( _dataSize - pOffset ) % sizeof( TPResult ) == 0 );
			return reinterpret_cast<TPResult *>( dataOffset( pOffset ) );
		}

		IC3_ATTR_NO_DISCARD bool empty() const noexcept
		{
			return !_storageMemoryPtr || ( _dataSize == 0 );
		}

		IC3_ATTR_NO_DISCARD size_t size() const noexcept
		{
			return _dataSize;
		}

		size_t fill( byte pValue, size_t pFillCount = cxMaxSize, size_t pFillOffset = 0 )
		{
			if( pFillOffset >= _dataSize )
			{
				return 0;
			}

			const auto bufferCapacity = _dataSize - pFillOffset;
			pFillCount = getMinOf( pFillCount, bufferCapacity );
			memFillChecked( _storageMemoryPtr + pFillOffset, bufferCapacity, pValue, pFillCount );
			_dataSize = pFillCount;

			return pFillCount;
		}

		size_t setData( const void * pData, size_t pDataSize, size_t pSetOffset = 0 )
		{
			if( !pData || ( pDataSize == 0 ) || ( pSetOffset >= _dataSize ) )
			{
				return 0;
			}

			const auto bufferCapacity = _dataSize - pSetOffset;
			pSetOffset = getMinOf( pSetOffset, bufferCapacity );
			memCopyUnchecked( _storageMemoryPtr + pSetOffset, bufferCapacity, pData, pDataSize );
			_dataSize = pDataSize;

			return pDataSize;
		}

		size_t copy( void * pBuffer, size_t pBufferSize, size_t pCopySize, size_t pCopyOffset = 0 )
		{
			if( !pBuffer || ( pBufferSize == 0 ) || ( pCopyOffset >= _dataSize ) )
			{
				return 0;
			}

			pCopySize = getMinOf( pCopySize, pBufferSize );
			pCopySize = getMinOf( pCopySize, _dataSize - pCopyOffset );

			if( pCopySize > 0 )
			{
				memCopyUnchecked( pBuffer, pBufferSize, _storageMemoryPtr + pCopyOffset, pCopySize );
			}

			return pCopySize;
		}

	protected:
		void updateStorage( void * pBufferBasePtr )
		{
			_storageMemoryPtr = reinterpret_cast<byte *>( pBufferBasePtr );
		}

	protected:
		byte * _storageMemoryPtr;
		size_t _dataSize;
	};

	inline ReadWriteMemoryView bindMemoryView( ByteArray & pByteArray )
	{
		return { pByteArray.data(), pByteArray.size() };
	}

	inline ReadOnlyMemoryView bindMemoryView( const ByteArray & pByteArray )
	{
		return { pByteArray.data(), pByteArray.size() };
	}

	class DynamicByteArray : public ByteArray
	{
	public:
		DynamicByteArray() = default;

		DynamicByteArray( DynamicByteArray && pSrcObject ) noexcept
		: ByteArray( std::move( pSrcObject ) )
		, _internalBuffer( std::move( pSrcObject._internalBuffer ) )
		{}

		DynamicByteArray( DynamicMemoryBuffer pInternalBuffer, size_t pSize = cxMaxSize )
		: _internalBuffer( std::move( pInternalBuffer ) )
		{
			_storageMemoryPtr = _internalBuffer.data();
			_dataSize = getMinOf( _internalBuffer.size(), pSize );
		}

		explicit DynamicByteArray( size_t pSize )
		{
			resize( pSize );
		}

		DynamicByteArray & operator=( DynamicByteArray && pRhs ) noexcept
		{
			DynamicByteArray( std::move( pRhs ) ).swap( *this );
			return *this;
		}

		IC3_ATTR_NO_DISCARD DynamicByteArray clone() const
		{
			DynamicByteArray newArray;
			newArray.assign( *this );
			return newArray;
		}

		DynamicByteArray & assign( const DynamicByteArray & pOther )
		{
			resize( pOther.size() );
			setData( pOther.data(), pOther.size(), 0 );
			return *this;
		}

		void setSize( size_t pNewSize )
		{
			pNewSize = getMinOf( pNewSize, _internalBuffer.size() );
			_dataSize = pNewSize;
		}

		void resize( size_t pNewSize )
		{
			if( pNewSize > _internalBuffer.size() )
			{
				resizeStorage( pNewSize );
			}
			_dataSize = pNewSize;
		}

		void shrinkToFit()
		{
			if( _internalBuffer.size() > _dataSize )
			{
				resizeStorage( _dataSize );
			}
		}

		bool optimizeStorage( float pAllowedFactor = 0.5f )
		{
			const auto storageCapacity = _internalBuffer.size();
			const auto exceedFactor = static_cast<float>( storageCapacity - _dataSize ) / _dataSize;

			if( exceedFactor > pAllowedFactor )
			{
				resizeStorage( _dataSize );
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

		void swap( DynamicByteArray & pOther )
		{
			Ic3::Cppx::swap( _internalBuffer, pOther._internalBuffer );
			std::swap( _storageMemoryPtr, pOther._storageMemoryPtr );
			std::swap( _dataSize, pOther._dataSize );
		}

	private:
		void resizeStorage( size_t pCapacity )
		{
			_internalBuffer.resize( pCapacity );
			updateStorage( _internalBuffer.data() );
		}

	private:
		DynamicMemoryBuffer _internalBuffer;
	};

	inline void swap( DynamicByteArray & pFirst, DynamicByteArray & pSecond )
	{
		pFirst.swap( pSecond );
	}

	template <size_t tSize>
	class FixedByteArray : public ByteArray
	{
	public:
		FixedByteArray( const FixedByteArray & ) = delete;
		FixedByteArray & operator=( const FixedByteArray & ) = delete;

		FixedByteArray()
		{
			_storageMemoryPtr = _internalBuffer.data();
			_dataSize = _internalBuffer.size();
		}

	private:
		FixedMemoryBuffer<tSize> _internalBuffer;
	};

}

#endif /* __IC3_CPPX_BYTE_ARRAY_H__ */
