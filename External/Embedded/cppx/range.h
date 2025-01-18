
#ifndef __CPPX_RANGE_H__
#define __CPPX_RANGE_H__

#include "typeTraits.h"
#include "utilities.h"

namespace cppx
{

	/**
	 * An inclusive range type. Represents a range expressed as a pair of begin/end values. The range is
	 * left- and right-inclusive, that is: for a given X it belongs to the range R if, and only if, it fulfills
	 * the condition X >= R.begin && X <= R.end.
	 * @tparam TPValue Value type for the range boundaries.
	 */
	template <typename TPValue>
	struct range
	{
		using self_type = range<TPValue>;
		using signed_value_type = typename meta::signed_type_equivalent<TPValue>::type;

		TPValue begin;

		TPValue end;

		constexpr range() = default;

		constexpr range( TPValue pBegin, TPValue pEnd )
		: begin( pBegin )
		, end( pEnd )
		{}

		template <typename TPBegin, typename TPEnd, typename std::enable_if_t<std::is_arithmetic_v<TPBegin> && std::is_arithmetic_v<TPEnd>, int> = 0>
		constexpr range( TPBegin pBegin, TPEnd pEnd )
		: begin( static_cast<TPValue>( pBegin ) )
		, end( static_cast<TPValue>( pEnd ) )
		{}

		CPPX_ATTR_NO_DISCARD typename meta::unsigned_type_equivalent<TPValue>::type length() const noexcept
		{
			const auto range_diff = static_cast<signed_value_type>( end ) - static_cast<signed_value_type>( begin );
			return static_cast<typename meta::unsigned_type_equivalent<TPValue>::type>( range_diff + static_cast<TPValue>( 1 ) );
		}

		CPPX_ATTR_NO_DISCARD bool empty() const noexcept
		{
			return length() == 0;
		}

		CPPX_ATTR_NO_DISCARD bool valid() const noexcept
		{
			return end >= begin;
		}

		template <typename TPOther>
		CPPX_ATTR_NO_DISCARD bool contains( TPOther pValue ) const noexcept
		{
			return ( pValue >= begin ) && ( pValue <= end );
		}

		template <typename TPOther>
		CPPX_ATTR_NO_DISCARD bool contains( const range<TPOther> & pOther ) const noexcept
		{
			return contains( pOther.begin ) && contains( pOther.end );
		}

		template <typename TPOther>
		CPPX_ATTR_NO_DISCARD bool overlaps_with( const range<TPOther> & pOther ) const noexcept
		{
			return contains( pOther.begin ) || contains( pOther.end );
		}

		template <typename TPOther>
		CPPX_ATTR_NO_DISCARD bool is_sub_range_of( const range<TPOther> & pOther ) const noexcept
		{
			return pOther.contains( *this );
		}

		CPPX_ATTR_NO_DISCARD static constexpr range empty_range() noexcept
		{
			return range{ static_cast<TPValue>( 1 ), static_cast<TPValue>( 0 ) };
		}

		CPPX_ATTR_NO_DISCARD static constexpr range invalid_range() noexcept
		{
			return range{ meta::limits<TPValue>::max_value, meta::limits<TPValue>::min_value };
		}

		CPPX_ATTR_NO_DISCARD static constexpr range max_range() noexcept
		{
			return range{ meta::limits<TPValue>::min_value, meta::limits<TPValue>::max_value };
		}

		template <typename TPOther>
		CPPX_ATTR_NO_DISCARD range & add( const range<TPOther> & pOther ) noexcept
		{
			if( pOther.valid() )
			{
				if( empty() )
				{
					*this = pOther;
				}
				else
				{
					if( pOther.begin < begin )
					{
						begin = pOther.begin;
					}
					if( pOther.end > end )
					{
						end = pOther.end;
					}
				}
			}
			return *this;
		}

		void set_empty()
		{
			begin = static_cast<TPValue>( 1 );
			end = static_cast<TPValue>( 0 );
		}

		void set_invalid()
		{
			begin = meta::limits<TPValue>::max_value;
			end = meta::limits<TPValue>::min_value;
		}

		void set_max()
		{
			begin = meta::limits<TPValue>::min_value;
			end = meta::limits<TPValue>::max_value;
		}
	};

	template <typename TPValue, typename TPBegin, typename TPEnd>
	inline range<TPValue> make_range( TPBegin pBegin, TPEnd pEnd )
	{
		const auto begin = numeric_cast<TPValue>( pBegin );
		const auto end = numeric_cast<TPValue>( pEnd );
		return range<TPValue>{ begin, end };
	}

	template <typename TPValue>
	inline bool range_contains( const range<TPValue> & pRange, TPValue pValue ) noexcept
	{
		return pRange.contains( pValue );
	}

	template <typename TPValue>
	inline bool range_contains( const range<TPValue> & pFirst, const range<TPValue> & pSecond ) noexcept
	{
		return pFirst.contains( pSecond );
	}

	template <typename TPValue>
	inline bool range_overlaps_with( const range<TPValue> & pFirst, const range<TPValue> & pSecond ) noexcept
	{
		return pFirst.overlaps_with( pSecond );
	}

	template <typename TPValue>
	inline bool range_is_sub_range_of( const range<TPValue> & pFirst, const range<TPValue> & pSecond ) noexcept
	{
		return pFirst.is_sub_range_of( pSecond );
	}

	template <typename TPSize, typename TPOffset = TPSize>
	struct region
	{
		static_assert( !std::is_signed_v<TPSize>, "The region size cannot be negative, hence usage of signed types is forbidden." );

		using self_type = region<TPSize, TPOffset>;
		using range_type = range<TPSize>;

		TPOffset offset = 0;

		TPSize size = 0;

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return !empty();
		}

		CPPX_ATTR_NO_DISCARD range_type as_range() const noexcept
		{
			return { offset, offset + size };
		}

		CPPX_ATTR_NO_DISCARD bool empty() const noexcept
		{
			return size == 0;
		}

		CPPX_ATTR_NO_DISCARD bool contains( TPOffset pOffset ) const noexcept
		{
			return range_contains( as_range(), pOffset );
		}

		CPPX_ATTR_NO_DISCARD bool contains( const region<TPSize, TPOffset> & pOther ) const noexcept
		{
			return range_contains( as_range(), pOther.as_range() );
		}

		CPPX_ATTR_NO_DISCARD bool overlaps_with( const region<TPSize, TPOffset> & pOther ) const noexcept
		{
			return range_overlaps_with( as_range(), pOther.as_range() );
		}

		CPPX_ATTR_NO_DISCARD bool is_sub_region_of( const region<TPSize, TPOffset> & pOther ) const noexcept
		{
			return range_is_sub_range_of( as_range(), pOther.as_range() );
		}

		void set_empty()
		{
			offset = static_cast<TPOffset>( 0 );
			size = static_cast<TPSize>( 0 );
		}

		void set_max()
		{
			offset = static_cast<TPOffset>( 0 );
			size = meta::limits<TPSize>::max_value;
		}
	};

	template <typename TPSize, typename TPOffset>
	inline bool operator==( const region<TPSize, TPOffset> & pLhs, const region<TPSize, TPOffset> & pRhs )
	{
		return ( pLhs.offset == pRhs.offset ) && ( pLhs.size == pRhs.size );
	}

	template <typename TPSize, typename TPOffset>
	inline bool operator!=( const region<TPSize, TPOffset> & pLhs, const region<TPSize, TPOffset> & pRhs )
	{
		return ( pLhs.offset != pRhs.offset ) || ( pLhs.size != pRhs.size );
	}

	template <typename TPOffset, typename TPSize, typename TPLimit>
	inline region<TPSize, TPOffset> get_valid_region( TPOffset pOffset, TPSize pSize, TPLimit pSizeLimit )
	{
		const auto validOffset = get_min_of( pOffset, pSizeLimit );
		const auto maxRegionSize = pSizeLimit - validOffset;
		const auto regionSize = get_min_of( pSize, maxRegionSize );

		return { numeric_cast<TPOffset>( validOffset ), numeric_cast<TPSize>( regionSize ) };
	}

	template <typename TPOffset, typename TPSize, typename TPLimit>
	inline region<TPSize, TPOffset> get_valid_region( const region<TPSize, TPOffset> & pRegion, TPLimit pSizeLimit )
	{
		get_valid_region( pRegion.offset, pRegion.size, pSizeLimit );
	}

}

#endif // __CPPX_RANGE_H__
