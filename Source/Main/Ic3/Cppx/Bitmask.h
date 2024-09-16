
#ifndef __IC3_CPPXBitMASK_H__
#define __IC3_CPPXBitMASK_H__

#include "BitUtils.h"
#include "TypeTraits.h"
#include <Ic3/Platform/GDS.h>

namespace Ic3::Cppx
{

	template <typename TPValue>
	class TBitmask
	{
		static_assert(
			( std::is_integral<TPValue>::value || std::is_enum<TPValue>::value ) && !std::is_same<TPValue, bool>::value,
			"Atomic masks are only valid for integer and enum types (but not a bool type)!" );

	public:
		using MyType = TBitmask<TPValue>;
		using ValueType = typename QUintTypeBySize<sizeof( TPValue )>::Type;

	public:
		constexpr TBitmask() noexcept
		: _value( 0 )
		{}

		template <typename TPBits>
		constexpr TBitmask( TPBits pValue ) noexcept
		: _value( static_cast<ValueType>( pValue ) )
		{}

		template <typename TPBits>
		MyType & operator=( TPBits pValue )
		{
			_value = static_cast<ValueType>( pValue );
			return *this;
		}

		constexpr operator TPValue() const
		{
			return static_cast<TPValue>( _value );
		}

		template <typename TPBits>
		MyType & store( TPBits pBits )
		{
			_value = static_cast<ValueType>( pBits );
			return *this;
		}

		template <typename TPBits>
		MyType & set( TPBits pBits )
		{
			_value |= static_cast<ValueType>( pBits );
			return *this;
		}

		template <typename TPBits>
		MyType & toggle( TPBits pBits )
		{
			_value ^= static_cast<ValueType>( pBits );
			return *this;
		}

		template <typename TPBits>
		MyType & unset( TPBits pBits )
		{
			_value &= ~static_cast<ValueType>( pBits );
			return *this;
		}

		MyType & invert()
		{
			_value = ~( _value );
			return *this;
		}

		template <typename TPBits>
		MyType & setOrUnset( TPBits pBits, bool pSet )
		{
			pSet ? set( pBits ) : unset( pBits );
			return *this;
		}

		template <typename TPBits>
		bool testAndSet( TPBits pBits )
		{
			if( ( _value & static_cast<ValueType>( pBits ) ) != 0 )
			{
				return false;
			}

			_value |= static_cast<ValueType>( pBits );
			return true;
		}

		template <typename TPBits>
		bool testAndUnset( TPBits pBits )
		{
			if( ( _value & static_cast<ValueType>( pBits ) ) != static_cast<ValueType>( pBits ) )
			{
				return false;
			}

			_value &= ~static_cast<ValueType>( pBits );
			return true;
		}

		template <typename TPBits>
		MyType & applyAnd( TPBits pBits )
		{
			_value &= pBits;
			return *this;
		}

		template <typename TPBits>
		MyType & applyOr( TPBits pBits )
		{
			_value |= pBits;
			return *this;
		}

		template <typename TPBits>
		MyType & applyXor( TPBits pBits )
		{
			_value ^= pBits;
			return *this;
		}

		void clear()
		{
			_value = 0;
		}

		void swap( MyType & pOther )
		{
			std::swap( _value, pOther._value );
		}

		IC3_ATTR_NO_DISCARD ValueType get() const
		{
			return _value;
		}

		template <typename TPBits>
		IC3_ATTR_NO_DISCARD ValueType test( TPBits pBits ) const
		{
			return _value & static_cast<ValueType>( pBits );
		}

		template <typename TPBits>
		IC3_ATTR_NO_DISCARD constexpr bool isSet( TPBits pBits ) const
		{
			return ( static_cast<ValueType>( pBits ) != 0 ) && ( ( _value & static_cast<ValueType>( pBits ) ) == static_cast<ValueType>( pBits ) );
		}

		template <typename TPBits>
		IC3_ATTR_NO_DISCARD constexpr bool isSetAnyOf( TPBits pBits ) const
		{
			return ( _value & static_cast<ValueType>( pBits ) ) != static_cast<ValueType>( 0 );
		}

		IC3_ATTR_NO_DISCARD constexpr uint16 countBits() const
		{
			return popCount( _value );;
		}

		IC3_ATTR_NO_DISCARD constexpr bool empty() const
		{
			return _value == 0;
		}

		IC3_ATTR_NO_DISCARD static constexpr uint16 countBits( ValueType pValue )
		{
			return popCount( pValue );
		}

		template <typename TPBits>
		constexpr void operator|=( TPBits pBits )
		{
			_value |= static_cast<ValueType>( pBits );
		}

		template <typename TPBits>
		constexpr void operator&=( TPBits pBits )
		{
			_value &= static_cast<ValueType>( pBits );
		}

		template <typename TPBits>
		constexpr void operator^=( TPBits pBits )
		{
			_value ^= static_cast<ValueType>( pBits );
		}

		constexpr void operator<<=( size_t pShift )
		{
			_value <<= pShift;
		}

		constexpr void operator>>=( size_t pShift )
		{
			_value <<= pShift;
		}

		constexpr MyType operator~() const
		{
			return ~( _value );
		}

		template <typename TPBits>
		constexpr MyType operator|( TPBits pBits ) const
		{
			return _value | static_cast<ValueType>( pBits );
		}

		template <typename TPBits>
		constexpr MyType operator&( TPBits pBits ) const
		{
			return _value & static_cast<ValueType>( pBits );
		}

		template <typename TPBits>
		constexpr MyType operator^( TPBits pBits ) const
		{
			return _value ^ static_cast<ValueType>( pBits );
		}

		constexpr MyType operator<<( size_t pShift ) const
		{
			return _value << pShift;
		}

		constexpr MyType operator>>( size_t pShift ) const
		{
			return _value >> pShift;
		}

	private:
		ValueType  _value;
	};

	template <typename TPValue>
	inline void swap( TBitmask<TPValue> & pFirst, TBitmask<TPValue> & pSecond )
	{
		pFirst.swap( pSecond );
	}

	template <typename TEnum, typename std::enable_if<std::is_enum<TEnum>::value, int>::type = 0>
	IC3_ATTR_NO_DISCARD inline constexpr TBitmask<typename std::underlying_type<TEnum>::type> makeBitmask( TEnum pValue = static_cast<TEnum>( 0 ) ) noexcept
	{
		return TBitmask<typename std::underlying_type<TEnum>::type>( pValue );
	}

	template <typename TIntegral, typename std::enable_if<std::is_integral<TIntegral>::value, int>::type = 0>
	IC3_ATTR_NO_DISCARD inline constexpr TBitmask<TIntegral> makeBitmask( TIntegral pValue ) noexcept
	{
		return TBitmask<TIntegral>( pValue );
	}

	template <typename TPValue, typename TPInput>
	IC3_ATTR_NO_DISCARD inline constexpr TBitmask<TPValue> makeBitmaskEx( TPInput pValue = static_cast<TPInput>( 0 ) ) noexcept
	{
		return TBitmask<TPValue>( pValue );
	}

}

#endif // __IC3_CPPXBitMASK_H__
