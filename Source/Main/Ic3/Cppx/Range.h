
#ifndef __IC3_CPPX_RANGE_H__
#define __IC3_CPPX_RANGE_H__

#include "TypeTraits.h"
#include "Utilities.h"

namespace Ic3::Cppx
{

	template <typename TPValue>
	struct TRange
	{
		using SelfType = TRange<TPValue>;

		TPValue mBegin;
		TPValue mEnd;

		constexpr TRange() = default;

		template <typename TPBegin, typename TPEnd, typename std::enable_if_t<std::is_arithmetic_v<TPBegin> && std::is_arithmetic_v<TPEnd>, int> = 0>
		constexpr TRange( TPBegin pBegin, TPEnd pEnd )
		: mBegin( static_cast<TPValue>( pBegin ) )
		, mEnd( static_cast<TPValue>( pEnd ) )
		{}

		IC3_ATTR_NO_DISCARD typename QUnsignedTypeEquivalent<TPValue>::Type length() const noexcept
		{
			return static_cast<typename QUnsignedTypeEquivalent<TPValue>::Type>( ( mEnd - mBegin ) + static_cast<TPValue>( 1 ) );
		}

		template <typename TPOther>
		IC3_ATTR_NO_DISCARD bool contains( TPOther pValue ) const noexcept
		{
			return ( pValue >= mBegin ) && ( pValue <= mEnd );
		}

		template <typename TPOther>
		IC3_ATTR_NO_DISCARD bool contains( const TRange<TPOther> & pOther ) const noexcept
		{
			return contains( pOther.mBegin ) && contains( pOther.mEnd );
		}

		template <typename TPOther>
		IC3_ATTR_NO_DISCARD bool overlapsWith( const TRange<TPOther> & pOther ) const noexcept
		{
			return contains( pOther.mBegin ) || contains( pOther.mEnd );
		}

		template <typename TPOther>
		IC3_ATTR_NO_DISCARD bool isSubRangeOf( const TRange<TPOther> & pOther ) const noexcept
		{
			return pOther.contains( *this );
		}

		IC3_ATTR_NO_DISCARD static constexpr TRange emptyRange() noexcept
		{
			return TRange{ static_cast<TPValue>( 0 ), static_cast<TPValue>( 0 ) };
		}

		IC3_ATTR_NO_DISCARD static constexpr TRange invalidRange() noexcept
		{
			return TRange{ QLimits<TPValue>::sMaxValue, QLimits<TPValue>::sMinValue };
		}

		IC3_ATTR_NO_DISCARD static constexpr TRange maxRange() noexcept
		{
			return TRange{ QLimits<TPValue>::sMinValue, QLimits<TPValue>::sMaxValue };
		}

		template <typename TPOther>
		IC3_ATTR_NO_DISCARD TRange & add( const TRange<TPOther> & pOther ) noexcept
		{
			if( pOther.mBegin < mBegin )
			{
				mBegin = pOther.mBegin;
			}
			if( pOther.mEnd > mEnd )
			{
				mEnd = pOther.mEnd;
			}
			return *this;
		}

		void setEmpty()
		{
			mBegin = static_cast<TPValue>( 0 );
			mEnd = static_cast<TPValue>( 0 );
		}

		void setInvalid()
		{
			mBegin = QLimits<TPValue>::sMaxValue;
			mEnd = QLimits<TPValue>::sMinValue;
		}

		void setMax()
		{
			mBegin = QLimits<TPValue>::sMinValue;
			mEnd = QLimits<TPValue>::sMaxValue;
		}
	};

	template <typename TPValue>
	inline bool rangeContains( const TRange<TPValue> & pRange, TPValue pValue ) noexcept
	{
		return pRange.contains( pValue );
	}

	template <typename TPValue>
	inline bool rangeContains( const TRange<TPValue> & pFirst, const TRange<TPValue> & pSecond ) noexcept
	{
		return pFirst.contains( pSecond );
	}

	template <typename TPValue>
	inline bool rangeOverlapsWith( const TRange<TPValue> & pFirst, const TRange<TPValue> & pSecond ) noexcept
	{
		return pFirst.overlapsWith( pSecond );
	}

	template <typename TPValue>
	inline bool rangeIsSubRangeOf( const TRange<TPValue> & pFirst, const TRange<TPValue> & pSecond ) noexcept
	{
		return pFirst.isSubRangeOf( pSecond );
	}

	template <typename TPSize, typename TPOffset = TPSize>
	struct TRegion
	{
		static_assert( !std::is_signed_v<TPSize>, "The region mSize cannot be negative, hence usage of signed types is forbidden." );

		using SelfType = TRegion<TPSize, TPOffset>;
		using RangeType = TRange<TPSize>;

		TPOffset mOffset = 0;
		TPSize mSize = 0;

		IC3_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return !empty();
		}

		IC3_ATTR_NO_DISCARD RangeType asRange() const noexcept
		{
			return { mOffset, mOffset + mSize };
		}

		IC3_ATTR_NO_DISCARD bool empty() const noexcept
		{
			return mSize == 0;
		}

		IC3_ATTR_NO_DISCARD bool contains( TPOffset pOffset ) const noexcept
		{
			return rangeContains( asRange(), pOffset );
		}

		IC3_ATTR_NO_DISCARD bool contains( const TRegion<TPSize, TPOffset> & pOther ) const noexcept
		{
			return rangeContains( asRange(), pOther.asRange() );
		}

		IC3_ATTR_NO_DISCARD bool overlapsWith( const TRegion<TPSize, TPOffset> & pOther ) const noexcept
		{
			return rangeOverlapsWith( asRange(), pOther.asRange() );
		}

		IC3_ATTR_NO_DISCARD bool isSubRegionOf( const TRegion<TPSize, TPOffset> & pOther ) const noexcept
		{
			return rangeIsSubRangeOf( asRange(), pOther.asRange() );
		}

		void setEmpty()
		{
			mOffset = static_cast<TPOffset>( 0 );
			mSize = static_cast<TPSize>( 0 );
		}

		void setMax()
		{
			mOffset = static_cast<TPOffset>( 0 );
			mSize = QLimits<TPSize>::sMaxValue;
		}
	};

	template <typename TPSize, typename TPOffset>
	inline bool operator==( const TRegion<TPSize, TPOffset> & pLhs, const TRegion<TPSize, TPOffset> & pRhs )
	{
		return ( pLhs.mOffset == pRhs.mOffset ) && ( pLhs.mSize == pRhs.mSize );
	}

	template <typename TPSize, typename TPOffset>
	inline bool operator!=( const TRegion<TPSize, TPOffset> & pLhs, const TRegion<TPSize, TPOffset> & pRhs )
	{
		return ( pLhs.mOffset != pRhs.mOffset ) || ( pLhs.mSize != pRhs.mSize );
	}

	template <typename TPOffset, typename TPSize, typename TPLimit>
	inline TRegion<TPSize, TPOffset> getValidRegion( TPOffset pOffset, TPSize pSize, TPLimit pSizeLimit )
	{
		const auto validOffset = getMinOf( pOffset, pSizeLimit );
		const auto maxRegionSize = pSizeLimit - validOffset;
		const auto regionSize = getMinOf( pSize, maxRegionSize );

		return { numeric_cast<TPOffset>( validOffset ), numeric_cast<TPSize>( regionSize ) };
	}

	template <typename TPOffset, typename TPSize, typename TPLimit>
	inline TRegion<TPSize, TPOffset> getValidRegion( const TRegion<TPSize, TPOffset> & pRegion, TPLimit pSizeLimit )
	{
		getValidRegion( pRegion.mOffset, pRegion.mSize, pSizeLimit );
	}

}

#endif // __IC3_CPPX_RANGE_H__
