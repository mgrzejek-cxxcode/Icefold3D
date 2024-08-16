
#ifndef __IC3_CPPX_RANGE_H__
#define __IC3_CPPX_RANGE_H__

#include "TypeTraits.h"
#include "Utilities.h"

namespace Ic3::Cppx
{

	template <typename TVal>
	struct Range
	{
		using SelfType = Range<TVal>;

		TVal begin;
		TVal end;

		constexpr Range() = default;

		template <typename TBegin, typename TEnd, typename std::enable_if_t<std::is_arithmetic_v<TBegin> && std::is_arithmetic_v<TEnd>, int> = 0>
		constexpr Range( TBegin pBegin, TEnd pEnd )
		: begin( static_cast<TVal>( pBegin ) )
		, end( static_cast<TVal>( pEnd ) )
		{}

		IC3_ATTR_NO_DISCARD typename QUnsignedTypeEquivalent<TVal>::Type length() const noexcept
		{
			return static_cast<typename QUnsignedTypeEquivalent<TVal>::Type>( ( end - begin ) + static_cast<TVal>( 1 ) );
		}

		template <typename TOther>
		IC3_ATTR_NO_DISCARD bool contains( TOther pValue ) const noexcept
		{
			return ( pValue >= begin ) && ( pValue <= end );
		}

		template <typename TOther>
		IC3_ATTR_NO_DISCARD bool contains( const Range<TOther> & pOther ) const noexcept
		{
			return contains( pOther.begin ) && contains( pOther.end );
		}

		template <typename TOther>
		IC3_ATTR_NO_DISCARD bool overlapsWith( const Range<TOther> & pOther ) const noexcept
		{
			return contains( pOther.begin ) || contains( pOther.end );
		}

		template <typename TOther>
		IC3_ATTR_NO_DISCARD bool isSubRangeOf( const Range<TOther> & pOther ) const noexcept
		{
			return pOther.contains( *this );
		}

		IC3_ATTR_NO_DISCARD static constexpr Range maxRange() noexcept
		{
			return Range{ QLimits<TVal>::minValue, QLimits<TVal>::maxValue };
		}

		template <typename TOther>
		IC3_ATTR_NO_DISCARD Range & add( const Range<TOther> & pOther ) noexcept
		{
			if( pOther.begin < begin )
			{
				begin = pOther.begin;
			}
			if( pOther.end > end )
			{
				end = pOther.end;
			}
			return *this;
		}

		void setEmpty()
		{
			begin = static_cast<TVal>( 0 );
			end = static_cast<TVal>( 0 );
		}

		void setInvalid()
		{
			begin = QLimits<TVal>::maxValue;
			end = QLimits<TVal>::minValue;
		}

		void setMax()
		{
			begin = QLimits<TVal>::minValue;
			end = QLimits<TVal>::maxValue;
		}
	};

	template <typename TVal>
	inline bool rangeContains( const Range<TVal> & pRange, TVal pValue ) noexcept
	{
		return pRange.contains( pValue );
	}

	template <typename TVal>
	inline bool rangeContains( const Range<TVal> & pFirst, const Range<TVal> & pSecond ) noexcept
	{
		return pFirst.contains( pSecond );
	}

	template <typename TVal>
	inline bool rangeOverlapsWith( const Range<TVal> & pFirst, const Range<TVal> & pSecond ) noexcept
	{
		return pFirst.overlapsWith( pSecond );
	}

	template <typename TVal>
	inline bool rangeIsSubRangeOf( const Range<TVal> & pFirst, const Range<TVal> & pSecond ) noexcept
	{
		return pFirst.isSubRangeOf( pSecond );
	}

	template <typename TSize, typename TOffset = TSize>
	struct SRegion
	{
		static_assert( !std::is_signed_v<TSize>, "The region size cannot be negative, hence usage of signed types is forbidden." );

		using SelfType = SRegion<TSize, TOffset>;
		using RangeType = Range<TSize>;

		TOffset offset = 0;
		TSize size = 0;

		IC3_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return !empty();
		}

		IC3_ATTR_NO_DISCARD RangeType asRange() const noexcept
		{
			return { offset, offset + size };
		}

		IC3_ATTR_NO_DISCARD bool empty() const noexcept
		{
			return size == 0;
		}

		IC3_ATTR_NO_DISCARD bool contains( TOffset pOffset ) const noexcept
		{
			return rangeContains( asRange(), pOffset );
		}

		IC3_ATTR_NO_DISCARD bool contains( const SRegion<TSize, TOffset> & pOther ) const noexcept
		{
			return rangeContains( asRange(), pOther.asRange() );
		}

		IC3_ATTR_NO_DISCARD bool overlapsWith( const SRegion<TSize, TOffset> & pOther ) const noexcept
		{
			return rangeOverlapsWith( asRange(), pOther.asRange() );
		}

		IC3_ATTR_NO_DISCARD bool isSubRegionOf( const SRegion<TSize, TOffset> & pOther ) const noexcept
		{
			return rangeIsSubRangeOf( asRange(), pOther.asRange() );
		}

		void setEmpty()
		{
			offset = static_cast<TOffset>( 0 );
			size = static_cast<TSize>( 0 );
		}

		void setMax()
		{
			offset = static_cast<TOffset>( 0 );
			size = QLimits<TSize>::maxValue;
		}
	};

	template <typename TSize, typename TOffset>
	inline bool operator==( const SRegion<TSize, TOffset> & pLhs, const SRegion<TSize, TOffset> & pRhs )
	{
		return ( pLhs.offset == pRhs.offset ) && ( pLhs.size == pRhs.size );
	}

	template <typename TSize, typename TOffset>
	inline bool operator!=( const SRegion<TSize, TOffset> & pLhs, const SRegion<TSize, TOffset> & pRhs )
	{
		return ( pLhs.offset != pRhs.offset ) || ( pLhs.size != pRhs.size );
	}

	template <typename TOffset, typename TSize, typename TLimit>
	inline SRegion<TSize, TOffset> getValidRegion( TOffset pOffset, TSize pSize, TLimit pSizeLimit )
	{
		const auto validOffset = getMinOf( pOffset, pSizeLimit );
		const auto maxRegionSize = pSizeLimit - validOffset;
		const auto regionSize = getMinOf( pSize, maxRegionSize );

		return { numeric_cast<TOffset>( validOffset ), numeric_cast<TSize>( regionSize ) };
	}

	template <typename TOffset, typename TSize, typename TLimit>
	inline SRegion<TSize, TOffset> getValidRegion( const SRegion<TSize, TOffset> & pRegion, TLimit pSizeLimit )
	{
		getValidRegion( pRegion.offset, pRegion.size, pSizeLimit );
	}

}

#endif // __IC3_CPPX_RANGE_H__
