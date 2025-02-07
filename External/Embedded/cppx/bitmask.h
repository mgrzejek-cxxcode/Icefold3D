
#ifndef __CPPX_BITMASK_H__
#define __CPPX_BITMASK_H__

#include "bitUtils.h"
#include "typeTraits.h"

namespace cppx
{


	template <typename TPValue>
	struct bitmask_value_type_check
	{
		static constexpr auto value = ( std::is_integral<TPValue>::value || std::is_enum<TPValue>::value ) && !std::is_same<TPValue, bool>::value;
	};

	template <typename TPBaseType, typename TPStoreType = typename meta::uint_type_by_size<sizeof( TPBaseType )>::type>
	class bitmask
	{
		static_assert( bitmask_value_type_check<TPBaseType>::value,
				"Atomic masks are only valid for integer and enum types (but not a bool type)!" );

	public:
		using self_type = bitmask<TPBaseType>;
		using value_type = TPStoreType;

	public:
		constexpr bitmask() noexcept
		: _value( 0 )
		{}

		template <typename TPBits, typename = std::enable_if_t<bitmask_value_type_check<TPBits>::value, int>>
		constexpr bitmask( TPBits pValue ) noexcept
		: _value( static_cast<value_type>( pValue ) )
		{}

		template <typename TPOtherValue>
		constexpr bitmask( bitmask<TPOtherValue> pOther ) noexcept
		: _value( static_cast<value_type>( pOther.get() ) )
		{}

		template <typename TPOtherValue>
		self_type & operator=( bitmask<TPOtherValue> pOther )
		{
			_value = static_cast<value_type>( pOther.get() );
			return *this;
		}

		template <typename TPBits, typename = std::enable_if_t<bitmask_value_type_check<TPBits>::value, int>>
		self_type & operator=( TPBits pValue )
		{
			_value = static_cast<value_type>( pValue );
			return *this;
		}

		constexpr operator TPBaseType() const
		{
			return static_cast<TPBaseType>( _value );
		}

		CPPX_ATTR_NO_DISCARD constexpr value_type get() const
		{
			return _value;
		}

		template <typename TPBits>
		CPPX_ATTR_NO_DISCARD constexpr value_type test( TPBits pBits ) const
		{
			return _value & static_cast<value_type>( pBits );
		}

		template <typename TPBits>
		CPPX_ATTR_NO_DISCARD constexpr bool is_set( TPBits pBits ) const
		{
			return ( static_cast<value_type>( pBits ) != 0 ) && ( ( _value & static_cast<value_type>( pBits ) ) == static_cast<value_type>( pBits ) );
		}

		template <typename TPBits>
		CPPX_ATTR_NO_DISCARD constexpr bool is_set_any_of( TPBits pBits ) const
		{
			return ( _value & static_cast<value_type>( pBits ) ) != static_cast<value_type>( 0 );
		}

		CPPX_ATTR_NO_DISCARD constexpr uint16 count_bits() const
		{
			return pop_count( _value );;
		}

		CPPX_ATTR_NO_DISCARD constexpr bool empty() const
		{
			return _value == 0;
		}

		CPPX_ATTR_NO_DISCARD static constexpr uint16 count_bits( value_type pValue )
		{
			return pop_count( pValue );
		}

		template <typename TPBits>
		self_type & store( TPBits pBits )
		{
			_value = static_cast<value_type>( pBits );
			return *this;
		}

		template <typename TPBits>
		self_type & set( TPBits pBits )
		{
			_value |= static_cast<value_type>( pBits );
			return *this;
		}

		template <typename TPBits>
		self_type & toggle( TPBits pBits )
		{
			_value ^= static_cast<value_type>( pBits );
			return *this;
		}

		template <typename TPBits>
		self_type & unset( TPBits pBits )
		{
			_value &= ~static_cast<value_type>( pBits );
			return *this;
		}

		self_type & invert()
		{
			_value = ~( _value );
			return *this;
		}

		template <typename TPBits>
		self_type & set_or_unset( TPBits pBits, bool pSet )
		{
			pSet ? set( pBits ) : unset( pBits );
			return *this;
		}

		template <typename TPBits>
		bool test_and_set( TPBits pBits )
		{
			if( ( _value & static_cast<value_type>( pBits ) ) != 0 )
			{
				return false;
			}

			_value |= static_cast<value_type>( pBits );
			return true;
		}

		template <typename TPBits>
		bool test_and_unset( TPBits pBits )
		{
			if( ( _value & static_cast<value_type>( pBits ) ) != static_cast<value_type>( pBits ) )
			{
				return false;
			}

			_value &= ~static_cast<value_type>( pBits );
			return true;
		}

		template <typename TPBits>
		self_type & apply_and( TPBits pBits )
		{
			_value &= pBits;
			return *this;
		}

		template <typename TPBits>
		self_type & apply_or( TPBits pBits )
		{
			_value |= pBits;
			return *this;
		}

		template <typename TPBits>
		self_type & apply_xor( TPBits pBits )
		{
			_value ^= pBits;
			return *this;
		}

		void clear()
		{
			_value = 0;
		}

		void swap( self_type & pOther )
		{
			std::swap( _value, pOther._value );
		}

		template <typename TPBits>
		constexpr void operator|=( TPBits pBits )
		{
			_value |= static_cast<value_type>( pBits );
		}

		template <typename TPBits>
		constexpr void operator&=( TPBits pBits )
		{
			_value &= static_cast<value_type>( pBits );
		}

		template <typename TPBits>
		constexpr void operator^=( TPBits pBits )
		{
			_value ^= static_cast<value_type>( pBits );
		}

		constexpr void operator<<=( size_t pShift )
		{
			_value <<= pShift;
		}

		constexpr void operator>>=( size_t pShift )
		{
			_value <<= pShift;
		}

		constexpr self_type operator~() const
		{
			return ~( _value );
		}

		template <typename TPBits>
		constexpr self_type operator|( TPBits pBits ) const
		{
			return _value | static_cast<value_type>( pBits );
		}

		template <typename TPBits>
		constexpr self_type operator&( TPBits pBits ) const
		{
			return _value & static_cast<value_type>( pBits );
		}

		template <typename TPBits>
		constexpr self_type operator^( TPBits pBits ) const
		{
			return _value ^ static_cast<value_type>( pBits );
		}

		constexpr self_type operator<<( size_t pShift ) const
		{
			return _value << pShift;
		}

		constexpr self_type operator>>( size_t pShift ) const
		{
			return _value >> pShift;
		}

	private:
		value_type  _value;
	};

	template <typename TPBaseType>
	inline void swap( bitmask<TPBaseType> & pFirst, bitmask<TPBaseType> & pSecond )
	{
		pFirst.swap( pSecond );
	}

	template <typename TEnum, typename std::enable_if<std::is_enum<TEnum>::value, int>::type = 0>
	CPPX_ATTR_NO_DISCARD inline constexpr bitmask<TEnum> make_bitmask( TEnum pValue ) noexcept
	{
		return bitmask<TEnum>( pValue );
	}

	template <typename TIntegral, typename std::enable_if<std::is_integral<TIntegral>::value, int>::type = 0>
	CPPX_ATTR_NO_DISCARD inline constexpr bitmask<TIntegral> make_bitmask( TIntegral pValue ) noexcept
	{
		return bitmask<TIntegral>( pValue );
	}

	template <typename TPBaseType, typename TPInput = TPBaseType>
	CPPX_ATTR_NO_DISCARD inline constexpr bitmask<TPBaseType> make_bitmask_tp( TPInput pValue = static_cast<TPInput>( 0 ) ) noexcept
	{
		return bitmask<TPBaseType>( pValue );
	}

	template <typename TPStoreType, typename TEnum, typename std::enable_if<std::is_enum<TEnum>::value, int>::type = 0>
	CPPX_ATTR_NO_DISCARD inline constexpr bitmask<TEnum> make_bitmask_ex( TEnum pValue ) noexcept
	{
		static_assert( pValue <= meta::limits<TPStoreType>::maxValue );
		return bitmask<TEnum>( pValue );
	}

	template <typename TPStoreType, typename TIntegral, typename std::enable_if<std::is_integral<TIntegral>::value, int>::type = 0>
	CPPX_ATTR_NO_DISCARD inline constexpr bitmask<TIntegral> make_bitmask_ex( TIntegral pValue ) noexcept
	{
		static_assert( pValue <= meta::limits<TPStoreType>::maxValue );
		return bitmask<TIntegral>( pValue );
	}

	template <typename TPStoreType, typename TPBaseType, typename TPInput = TPBaseType>
	CPPX_ATTR_NO_DISCARD inline constexpr bitmask<TPBaseType> make_bitmask_ex_tp( TPInput pValue = static_cast<TPInput>( 0 ) ) noexcept
	{
		static_assert( pValue <= meta::limits<TPStoreType>::maxValue );
		return bitmask<TPBaseType>( pValue );
	}

}

#endif // __CPPX_BITMASK_H__
