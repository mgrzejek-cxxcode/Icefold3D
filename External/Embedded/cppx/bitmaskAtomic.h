
#ifndef __CPPX_BITMASK_ATOMIC_H__
#define __CPPX_BITMASK_ATOMIC_H__

#include "prerequisites.h"
#include <atomic>

namespace cppx
{

	template <typename TPValue>
	class atomic_bitmask
	{
		static_assert(
			( std::is_integral<TPValue>::value || std::is_enum<TPValue>::value ) && !std::is_same<TPValue, bool>::value,
			"Atomic Masks are only valid for integer and enum types (but not a bool type)!" );

	public:
		using MyType = atomic_bitmask<TPValue>;
		using ValueType = typename meta::uint_type_by_size<sizeof( TPValue )>::type;

	public:
		atomic_bitmask( const atomic_bitmask & ) = delete;
		atomic_bitmask & operator=( const atomic_bitmask & ) = delete;

		atomic_bitmask()  noexcept
		: _value( 0 )
		{}

		template <typename TPBits>
		atomic_bitmask( TPBits pBits ) noexcept
		: _value( static_cast<ValueType>( pBits ) )
		{}

		template <typename TPBits>
		MyType & operator=( TPBits pBits )
		{
			_value.store( static_cast<ValueType>( pBits ), std::memory_order_relaxed );
			return *this;
		}

		operator TPValue() const
		{
			return static_cast<TPValue>( get() );
		}

		CPPX_ATTR_NO_DISCARD ValueType get( std::memory_order pOrder = std::memory_order_relaxed ) const
		{
			return _value.load( pOrder );
		}

		template <typename TPBits>
		CPPX_ATTR_NO_DISCARD ValueType test( TPBits pBits, std::memory_order pOrder = std::memory_order_relaxed ) const
		{
			return get( pOrder ) & static_cast<ValueType>( pBits );
		}

		template <typename TPBits>
		CPPX_ATTR_NO_DISCARD bool is_set( TPBits pBits, std::memory_order pOrder = std::memory_order_relaxed ) const
		{
			return ( pBits != 0 ) && ( ( get( pOrder ) & static_cast<ValueType>( pBits ) ) == static_cast<ValueType>( pBits ) );
		}

		template <typename TPBits>
		CPPX_ATTR_NO_DISCARD bool is_set_any_of( TPBits pBits, std::memory_order pOrder = std::memory_order_relaxed ) const
		{
			return ( get( pOrder ) & static_cast<ValueType>( pBits ) ) != static_cast<ValueType>( 0 );
		}

		CPPX_ATTR_NO_DISCARD bool empty( std::memory_order pOrder = std::memory_order_relaxed ) const
		{
			return get( pOrder ) == 0;
		}

		template <typename TPBits>
		void store( TPBits pBits, std::memory_order pOrder = std::memory_order_relaxed )
		{
			_value.store( static_cast<ValueType>( pBits ), pOrder );
		}

		template <typename TPBits>
		void set( TPBits pBits, std::memory_order pOrder = std::memory_order_relaxed )
		{
			_value.fetch_or( static_cast<ValueType>( pBits ), pOrder );
		}

		template <typename TPBits>
		void toggle( TPBits pBits, std::memory_order pOrder = std::memory_order_relaxed )
		{
			_value.fetch_xor( static_cast<ValueType>( pBits ), pOrder );
		}

		template <typename TPBits>
		void unset( TPBits pBits, std::memory_order pOrder = std::memory_order_relaxed )
		{
			_value.fetch_and( ~static_cast<ValueType>( pBits ), pOrder );
		}

		template <typename TPBits>
		void set_or_unset( TPBits pBits, bool pSet, std::memory_order pOrder = std::memory_order_relaxed )
		{
			if( pSet )
			{
				set( pBits, pOrder );
			}
			else
			{
				unset( pBits, pOrder );
			}
		}

		template <typename TPBits>
		bool test_and_set( TPBits pBits )
		{
			ValueType current = _value.load( std::memory_order_relaxed );

			while( true )
			{
				if( ( current & static_cast<ValueType>( pBits ) ) != 0 )
				{
					return false;
				}

				if( _value.compare_exchange_weak( current,
				                                  current | static_cast<ValueType>( pBits ),
				                                  std::memory_order_acq_rel,
				                                  std::memory_order_relaxed ) )
				{
					return true;
				}
			}
		}

		template <typename TPBits>
		bool test_and_unset( TPBits pBits )
		{
			ValueType current = _value.load( std::memory_order_relaxed );

			while( true )
			{
				if( ( current & static_cast<ValueType>( pBits ) ) != static_cast<ValueType>( pBits ) )
				{
					return false;
				}

				if( _value.compare_exchange_weak( current,
				                                  current & ~static_cast<ValueType>( pBits ),
				                                  std::memory_order_acq_rel,
				                                  std::memory_order_relaxed ) )
				{
					return true;
				}
			}
		}

		void clear( std::memory_order pOrder = std::memory_order_relaxed )
		{
			_value.store( 0, pOrder );
		}

		template <typename TPBits>
		void operator|=( TPBits pBits )
		{
			_value.fetch_or( static_cast<ValueType>( pBits ), std::memory_order_relaxed );
		}

		template <typename TPBits>
		void operator&=( TPBits pBits )
		{
			_value.fetch_and( static_cast<ValueType>( pBits ), std::memory_order_relaxed );
		}

		template <typename TPBits>
		void operator^=( TPBits pBits )
		{
			_value.fetch_xor( static_cast<ValueType>( pBits ), std::memory_order_relaxed );
		}

		ValueType operator~() const
		{
			return ~( get() );
		}

		template <typename TPBits>
		ValueType operator|( TPBits pBits ) const
		{
			return get() | static_cast<ValueType>( pBits );
		}

		template <typename TPBits>
		ValueType operator&( TPBits pBits ) const
		{
			return get() & static_cast<ValueType>( pBits );
		}

		template <typename TPBits>
		ValueType operator^( TPBits pBits ) const
		{
			return get() ^ static_cast<ValueType>( pBits );
		}

		ValueType operator<<( size_t pShift ) const
		{
			return get() << pShift;
		}

		ValueType operator>>( size_t pShift ) const
		{
			return get() >> pShift;
		}

	private:
		std::atomic<ValueType> _value;
	};

}

#endif // __CPPX_BITMASK_ATOMIC_H__
