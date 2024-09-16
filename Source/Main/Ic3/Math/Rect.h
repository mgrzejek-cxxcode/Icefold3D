
#ifndef __IC3_MATH_RECT_H__
#define __IC3_MATH_RECT_H__

#include "Vector.h"

#if( IC3_PCL_COMPILER & IC3_PCL_COMPILER_CLANG )
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#  pragma clang diagnostic ignored "-Wnested-anon-types"
#elif( IC3_PCL_COMPILER & IC3_PCL_COMPILER_GCC )
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wpedantic"
#elif( IC3_PCL_COMPILER & IC3_PCL_COMPILER_MSVC )
#  pragma warning( push )
#  pragma warning( disable: 4201 )  // 'Nonstandard extension used: nameless struct/union'
#endif

namespace Ic3::Math
{

	template <typename TPOffset, typename TPSize>
	struct Rect
	{
	public:
		union
		{
			struct
			{
				Vector2<TPOffset> mOffset;
				Vector2<TPSize> mSize;
			};

			struct
			{
				TPOffset mOffX;
				TPOffset mOffY;
				TPSize mSzWidth;
				TPSize mSzHeight;
			};
		};

	public:
		constexpr Rect() = default;

		constexpr Rect( const Rect & ) = default;
		constexpr Rect & operator=( const Rect & ) = default;

		template <typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		constexpr explicit Rect( TPScalar pScalar ) noexcept
		: mOffset( pScalar, pScalar )
		, mSize( pScalar, pScalar )
		{}

		template <typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		constexpr Rect( TPScalar pWidth, TPScalar pHeight ) noexcept
		: mOffset( 0, 0 )
		, mSize( pWidth, pHeight )
		{}

		template <typename TX, typename TY, typename TWidth, typename THeight>
		constexpr Rect( TX pX, TY pY, TWidth pWidth, THeight pHeight = 0 ) noexcept
		: mOffset( pX, pY )
		, mSize( pWidth, ( pHeight == 0 ) ? pWidth : pHeight )
		{}

		template <typename T1, typename T2>
		constexpr Rect( const Vector2<T1> & pOffset, const Vector2<T2> & pSize ) noexcept
		: mOffset( pOffset )
		, mSize( pSize )
		{}

		IC3_ATTR_NO_DISCARD TPSize area() const
		{
			return mSzWidth * mSzHeight;
		}

		IC3_ATTR_NO_DISCARD bool isNonZero() const
		{
			return ( mSzWidth != 0 ) && ( mSzHeight != 0 );
		}

		IC3_ATTR_NO_DISCARD bool isZero() const
		{
			return ( mSzWidth == 0 ) || ( mSzHeight == 0 );
		}
	};

	using Recti32 = Rect<int32, uint32>;
	using Rectu32 = Rect<uint32, uint32>;
	using Recti64 = Rect<int64, uint64>;
	using Rectu64 = Rect<uint64, uint64>;
	using Rectf   = Rect<float, float>;

} // namespace Ic3::Math

#if( IC3_PCL_COMPILER & IC3_PCL_COMPILER_CLANG )
#  pragma clang diagnostic pop
#elif( IC3_PCL_COMPILER & IC3_PCL_COMPILER_GCC )
#  pragma GCC diagnostic pop
#elif( IC3_PCL_COMPILER & IC3_PCL_COMPILER_MSVC )
#  pragma warning( pop )
#endif

#endif // __IC3_MATH_RECT_H__
