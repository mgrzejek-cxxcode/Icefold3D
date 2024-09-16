
#ifndef __IC3_CPPX_ARRAY_VIEW_H__
#define __IC3_CPPX_ARRAY_VIEW_H__

#include "Range.h"
#include "Utilities.h"

namespace Ic3::Cppx
{

	template <bool tpIsConst>
	struct ArrayViewByteType;

	template <>
	struct ArrayViewByteType<false>
	{
		using Type = byte;
	};

	template <>
	struct ArrayViewByteType<true>
	{
		using Type = const byte;
	};

	/// @brief Simple struct used to wrap and reference continuous blocks of memory.
	/// @tparam TPValue Type of the data the view references.
	template <typename TPValue>
	struct TArrayView
	{
		static_assert( !std::is_void<TPValue>::value, "Cannot create an ArrayView for void data. Use ArrayView<byte> for that." );

	public:
		using ByteType = typename ArrayViewByteType<std::is_const<TPValue>::value>::Type;
		
		TArrayView()
		: _dataPtr( nullptr )
		, _size( 0 )
		{}

		TArrayView( TPValue * pMemory, size_t pSize )
		: _dataPtr( pMemory )
		, _size( pSize )
		{}

		template <typename TPOther>
		TArrayView( const TArrayView<TPOther> & pOther )
		: _dataPtr( pOther.data() )
		, _size( pOther.size() )
		{}

		template <size_t tpSize>
		explicit TArrayView( TPValue( &pArray )[tpSize] )
		: TArrayView( &( pArray[0] ), tpSize )
		{}

		explicit TArrayView( std::initializer_list<TPValue> pInitList )
		: TArrayView( pInitList.data(), pInitList.size() )
		{}

		explicit operator bool() const
		{
			return !empty();
		}

		IC3_ATTR_NO_DISCARD TPValue & operator[]( size_t pIndex ) const
		{
			ic3DebugAssert( pIndex < _size );
			return _dataPtr[pIndex];
		}

		IC3_ATTR_NO_DISCARD TArrayView<ByteType> asByteView() const
		{
			return TArrayView<ByteType>{ reinterpret_cast<ByteType *>( _dataPtr ), _size * sizeof( TPValue ) };
		}

		IC3_ATTR_NO_DISCARD TPValue * data() const
		{
			return _dataPtr;
		}

		IC3_ATTR_NO_DISCARD size_t size() const
		{
			return _size;
		}

		IC3_ATTR_NO_DISCARD bool empty() const
		{
			return !_dataPtr || ( _size == 0 );
		}

		void swap( TArrayView & pOther )
		{
			std::swap( _dataPtr, pOther._dataPtr );
			std::swap( _size, pOther._size );
		}

	private:
		// Pointer to the beginning of the data.
		TPValue * _dataPtr;
		// Size of the data, in number of elements of the underlying type (TPValue).
		size_t _size;
	};

	template <typename TPValue>
	inline void swap( TArrayView<TPValue> & pFirst, TArrayView<TPValue> & pSecond )
	{
		pFirst.swap( pSecond );
	}

	template <typename TPValue>
	IC3_ATTR_NO_DISCARD inline TPValue * begin( const TArrayView<TPValue> & pArrayView )
	{
		return pArrayView.data();
	}

	template <typename TPValue>
	IC3_ATTR_NO_DISCARD inline TPValue * end( const TArrayView<TPValue> & pArrayView )
	{
		return pArrayView.data() + pArrayView.size();
	}

	/// @brief Creates ArrayView that wraps specified memory.
	/// @tparam TPValue Type of the data referenced by the view.
	/// @param pArrayMemory Pointer to the beginning of the array.
	/// @param pArraySize Size of the array, in number of elements of type TPValue.
	/// @return ArrayView referencing specified data.
	template <typename TPValue>
	IC3_ATTR_NO_DISCARD inline TArrayView<TPValue> bindArrayView( TPValue * pMemory, size_t pSize )
	{
		return TArrayView<TPValue>( pMemory, pSize );
	}

	template <typename TPValue, typename TOffset, typename TSize>
	IC3_ATTR_NO_DISCARD inline TArrayView<TPValue> bindArrayView( TPValue * pMemory, const TRegion<TSize, TOffset> & pRegion )
	{
		return TArrayView<TPValue>( pMemory + pRegion.mOffset, pRegion.mSize );
	}

	/// @brief Creates ArrayView that wraps specified compile time-sized array.
	/// @tparam TPValue Type of the data referenced by the view.
	/// @tparam tpSize Size of the array.
	/// @param pArray Array to create view for.
	/// @return ArrayView referencing specified data.
	template <typename TPValue, size_t tpSize>
	IC3_ATTR_NO_DISCARD inline TArrayView<TPValue> bindArrayView( TPValue( &pArray )[tpSize] )
	{
		return TArrayView<TPValue>( pArray );
	}

	template <typename TPValue, size_t tpSize, typename TOffset, typename TSize>
	IC3_ATTR_NO_DISCARD inline TArrayView<TPValue> bindArrayView( TPValue( &pArray )[tpSize], const TRegion<TSize, TOffset> & pRegion )
	{
		const auto validRegion = getValidRegion( pRegion, tpSize );
		return TArrayView<TPValue>( &pArray[0] + validRegion.mOffset, validRegion.mSize );
	}

	/// @brief Creates ArrayView that wraps specified compile time-sized array.
	/// @tparam TPValue Type of the data referenced by the view.
	/// @tparam tpSize Size of the array.
	/// @param pArray Array to create view for.
	/// @return ArrayView referencing specified data.
	template <typename TPValue, size_t tpSize>
	IC3_ATTR_NO_DISCARD inline TArrayView<TPValue> bindArrayView( std::array<TPValue, tpSize> & pArray )
	{
		return TArrayView<TPValue>( pArray.data(), pArray.size() );
	}

	template <typename TPValue, size_t tpSize, typename TOffset, typename TSize>
	IC3_ATTR_NO_DISCARD inline TArrayView<TPValue> bindArrayView( std::array<TPValue, tpSize> & pArray, const TRegion<TSize, TOffset> & pRegion )
	{
		const auto validRegion = getValidRegion( pRegion, tpSize );
		return TArrayView<TPValue>( pArray.data() + validRegion.mOffset, validRegion.mSize );
	}

	/// @brief Creates ArrayView that wraps specified compile time-sized array.
	/// @tparam TPValue Type of the data referenced by the view.
	/// @tparam tpSize Size of the array.
	/// @param pArray Array to create view for.
	/// @return ArrayView referencing specified data.
	template <typename TPValue, size_t tpSize>
	IC3_ATTR_NO_DISCARD inline TArrayView<const TPValue> bindArrayView( const std::array<TPValue, tpSize> & pArray )
	{
		return TArrayView<const TPValue>( pArray.data(), pArray.size() );
	}

	template <typename TPValue, size_t tpSize, typename TOffset, typename TSize>
	IC3_ATTR_NO_DISCARD inline TArrayView<const TPValue> bindArrayView( const std::array<TPValue, tpSize> & pArray, const TRegion<TSize, TOffset> & pRegion )
	{
		const auto validRegion = getValidRegion( pRegion, tpSize );
		return TArrayView<const TPValue>( pArray.data() + validRegion.mOffset, validRegion.mSize );
	}

	template <bool tpIsConst>
	struct ValueByteTypeProxy;

	template <>
	struct ValueByteTypeProxy<false>
	{
		using Type = byte;
	};

	template <>
	struct ValueByteTypeProxy<true>
	{
		using Type = const byte;
	};

	template <typename TPValue>
	struct ValueByteType
	{
		using Type = typename ValueByteTypeProxy<std::is_const<TPValue>::value>::Type;
	};

	using ReadOnlyMemoryView = TArrayView<const byte>;
	using ReadWriteMemoryView = TArrayView<byte>;

	template <typename TPValue, std::enable_if_t<std::is_const<TPValue>::value && !std::is_void<TPValue>::value, int> = 0>
	IC3_ATTR_NO_DISCARD inline ReadOnlyMemoryView bindMemoryView( TPValue * pMemory, size_t pSize )
	{
		return { reinterpret_cast<const byte *>( pMemory ), pSize * sizeof( TPValue ) };
	}

	template <typename TPValue, std::enable_if_t<!std::is_const<TPValue>::value && !std::is_void<TPValue>::value, int> = 0>
	IC3_ATTR_NO_DISCARD inline ReadWriteMemoryView bindMemoryView( TPValue * pMemory, size_t pSize )
	{
		return { reinterpret_cast<byte *>( pMemory ), pSize * sizeof( TPValue ) };
	}

	IC3_ATTR_NO_DISCARD inline ReadOnlyMemoryView bindMemoryView( const void * pMemory, size_t pByteLength )
	{
		return { reinterpret_cast<const byte *>( pMemory ), pByteLength };
	}

	IC3_ATTR_NO_DISCARD inline ReadWriteMemoryView bindMemoryView( void * pMemory, size_t pByteLength )
	{
		return { reinterpret_cast<byte *>( pMemory ), pByteLength };
	}

}

#endif // __IC3_CPPX_ARRAY_VIEW_H__
