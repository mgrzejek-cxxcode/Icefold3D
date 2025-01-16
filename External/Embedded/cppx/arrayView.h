
#ifndef __CPPX_ARRAY_VIEW_H__
#define __CPPX_ARRAY_VIEW_H__

#include "range.h"
#include "utilities.h"

namespace cppx
{

	template <bool tpIsConst>
	struct array_view_byte_type;

	template <>
	struct array_view_byte_type<false>
	{
		using type = byte;
	};

	template <>
	struct array_view_byte_type<true>
	{
		using type = const byte;
	};

	/// @brief Simple struct used to wrap and reference continuous blocks of memory.
	/// @tparam TPValue type of the data the view references.
	template <typename TPValue>
	struct array_view
	{
		static_assert( !std::is_void<TPValue>::value, "Cannot create an ArrayView for void data. Use ArrayView<byte> for that." );

	public:
		using byte_type = typename array_view_byte_type<std::is_const<TPValue>::value>::type;
		
		array_view()
		: _dataPtr( nullptr )
		, _size( 0 )
		{}

		array_view( TPValue * pMemory, size_t pSize )
		: _dataPtr( pMemory )
		, _size( pSize )
		{}

		template <typename TPOther>
		array_view( const array_view<TPOther> & pOther )
		: _dataPtr( pOther.data() )
		, _size( pOther.size() )
		{}

		template <size_t tpSize>
		explicit array_view( TPValue( &pArray )[tpSize] )
		: array_view( &( pArray[0] ), tpSize )
		{}

		explicit array_view( std::initializer_list<TPValue> pInitList )
		: array_view( pInitList.data(), pInitList.size() )
		{}

		explicit operator bool() const
		{
			return !empty();
		}

		CPPX_ATTR_NO_DISCARD TPValue & operator[]( size_t pIndex ) const
		{
			cppx_debug_assert( pIndex < _size );
			return _dataPtr[pIndex];
		}

		CPPX_ATTR_NO_DISCARD array_view<byte_type> asByteView() const
		{
			return array_view<byte_type>{ reinterpret_cast<byte_type *>( _dataPtr ), _size * sizeof( TPValue ) };
		}

		CPPX_ATTR_NO_DISCARD TPValue * data() const
		{
			return _dataPtr;
		}

		CPPX_ATTR_NO_DISCARD size_t size() const
		{
			return _size;
		}

		CPPX_ATTR_NO_DISCARD bool empty() const
		{
			return !_dataPtr || ( _size == 0 );
		}

		void swap( array_view & pOther )
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
	inline void swap( array_view<TPValue> & pFirst, array_view<TPValue> & pSecond )
	{
		pFirst.swap( pSecond );
	}

	template <typename TPValue>
	CPPX_ATTR_NO_DISCARD inline TPValue * begin( const array_view<TPValue> & pArrayView )
	{
		return pArrayView.data();
	}

	template <typename TPValue>
	CPPX_ATTR_NO_DISCARD inline TPValue * end( const array_view<TPValue> & pArrayView )
	{
		return pArrayView.data() + pArrayView.size();
	}

	/// @brief Creates ArrayView that wraps specified memory.
	/// @tparam TPValue type of the data referenced by the view.
	/// @param pArrayMemory Pointer to the beginning of the array.
	/// @param pArraySize Size of the array, in number of elements of type TPValue.
	/// @return ArrayView referencing specified data.
	template <typename TPValue>
	CPPX_ATTR_NO_DISCARD inline array_view<TPValue> bind_array_view( TPValue * pMemory, size_t pSize )
	{
		return array_view<TPValue>( pMemory, pSize );
	}

	template <typename TPValue, typename TPOffset, typename TPSize>
	CPPX_ATTR_NO_DISCARD inline array_view<TPValue> bind_array_view( TPValue * pMemory, const region<TPSize, TPOffset> & pRegion )
	{
		return array_view<TPValue>( pMemory + pRegion.offset, pRegion.size );
	}

	/// @brief Creates ArrayView that wraps specified compile time-sized array.
	/// @tparam TPValue type of the data referenced by the view.
	/// @tparam tpSize Size of the array.
	/// @param pArray Array to create view for.
	/// @return ArrayView referencing specified data.
	template <typename TPValue, size_t tpSize>
	CPPX_ATTR_NO_DISCARD inline array_view<TPValue> bind_array_view( TPValue( &pArray )[tpSize] )
	{
		return array_view<TPValue>( pArray );
	}

	template <typename TPValue, size_t tpSize, typename TPOffset, typename TPSize>
	CPPX_ATTR_NO_DISCARD inline array_view<TPValue> bind_array_view( TPValue( &pArray )[tpSize], const region<TPSize, TPOffset> & pRegion )
	{
		const auto validRegion = get_valid_region( pRegion, tpSize );
		return array_view<TPValue>( &pArray[0] + validRegion.offset, validRegion.size );
	}

	/// @brief Creates ArrayView that wraps specified compile time-sized array.
	/// @tparam TPValue type of the data referenced by the view.
	/// @tparam tpSize Size of the array.
	/// @param pArray Array to create view for.
	/// @return ArrayView referencing specified data.
	template <typename TPValue, size_t tpSize>
	CPPX_ATTR_NO_DISCARD inline array_view<TPValue> bind_array_view( std::array<TPValue, tpSize> & pArray )
	{
		return array_view<TPValue>( pArray.data(), pArray.size() );
	}

	template <typename TPValue, size_t tpSize, typename TPOffset, typename TPSize>
	CPPX_ATTR_NO_DISCARD inline array_view<TPValue> bind_array_view( std::array<TPValue, tpSize> & pArray, const region<TPSize, TPOffset> & pRegion )
	{
		const auto validRegion = get_valid_region( pRegion, tpSize );
		return array_view<TPValue>( pArray.data() + validRegion.offset, validRegion.size );
	}

	/// @brief Creates ArrayView that wraps specified compile time-sized array.
	/// @tparam TPValue type of the data referenced by the view.
	/// @tparam tpSize Size of the array.
	/// @param pArray Array to create view for.
	/// @return ArrayView referencing specified data.
	template <typename TPValue, size_t tpSize>
	CPPX_ATTR_NO_DISCARD inline array_view<const TPValue> bind_array_view( const std::array<TPValue, tpSize> & pArray )
	{
		return array_view<const TPValue>( pArray.data(), pArray.size() );
	}

	template <typename TPValue, size_t tpSize, typename TPOffset, typename TPSize>
	CPPX_ATTR_NO_DISCARD inline array_view<const TPValue> bind_array_view( const std::array<TPValue, tpSize> & pArray, const region<TPSize, TPOffset> & pRegion )
	{
		const auto validRegion = get_valid_region( pRegion, tpSize );
		return array_view<const TPValue>( pArray.data() + validRegion.offset, validRegion.size );
	}

	/// @brief Creates ArrayView that wraps
	template <typename TPValue, typename TPAllocator>
	CPPX_ATTR_NO_DISCARD inline array_view<TPValue> bind_array_view( std::vector<TPValue, TPAllocator> & pVector )
	{
		return array_view<TPValue>( pVector.data(), pVector.size() );
	}

	template <typename TPValue, typename TPAllocator, typename TPOffset, typename TPSize>
	CPPX_ATTR_NO_DISCARD inline array_view<TPValue> bind_array_view( std::vector<TPValue, TPAllocator> & pVector, const region<TPSize, TPOffset> & pRegion )
	{
		const auto validRegion = get_valid_region( pRegion, pVector.size() );
		return array_view<TPValue>( pVector.data() + validRegion.offset, validRegion.size );
	}

	/// @brief Creates ArrayView that wraps
	template <typename TPValue, typename TPAllocator>
	CPPX_ATTR_NO_DISCARD inline array_view<const TPValue> bind_array_view( const std::vector<TPValue, TPAllocator> & pVector )
	{
		return array_view<const TPValue>( pVector.data(), pVector.size() );
	}

	template <typename TPValue, typename TPAllocator, typename TPOffset, typename TPSize>
	CPPX_ATTR_NO_DISCARD inline array_view<const TPValue> bind_array_view( const std::vector<TPValue, TPAllocator> & pVector, const region<TPSize, TPOffset> & pRegion )
	{
		const auto validRegion = get_valid_region( pRegion, pVector.size() );
		return array_view<const TPValue>( pVector.data() + validRegion.offset, validRegion.size );
	}

	template <bool tpIsConst>
	struct value_byte_type_proxy;

	template <>
	struct value_byte_type_proxy<false>
	{
		using type = byte;
	};

	template <>
	struct value_byte_type_proxy<true>
	{
		using type = const byte;
	};

	template <typename TPValue>
	struct value_byte_type
	{
		using type = typename value_byte_type_proxy<std::is_const<TPValue>::value>::type;
	};

	using read_only_memory_view = array_view<const byte>;
	using read_write_memory_view = array_view<byte>;

	template <typename TPValue, std::enable_if_t<std::is_const<TPValue>::value && !std::is_void<TPValue>::value, int> = 0>
	CPPX_ATTR_NO_DISCARD inline read_only_memory_view bind_memory_view( TPValue * pMemory, size_t pSize )
	{
		return { reinterpret_cast<const byte *>( pMemory ), pSize * sizeof( TPValue ) };
	}

	template <typename TPValue, std::enable_if_t<!std::is_const<TPValue>::value && !std::is_void<TPValue>::value, int> = 0>
	CPPX_ATTR_NO_DISCARD inline read_write_memory_view bind_memory_view( TPValue * pMemory, size_t pSize )
	{
		return { reinterpret_cast<byte *>( pMemory ), pSize * sizeof( TPValue ) };
	}

	CPPX_ATTR_NO_DISCARD inline read_only_memory_view bind_memory_view( const void * pMemory, size_t pByteLength )
	{
		return { reinterpret_cast<const byte *>( pMemory ), pByteLength };
	}

	CPPX_ATTR_NO_DISCARD inline read_write_memory_view bind_memory_view( void * pMemory, size_t pByteLength )
	{
		return { reinterpret_cast<byte *>( pMemory ), pByteLength };
	}

}

#endif // __CPPX_ARRAY_VIEW_H__
