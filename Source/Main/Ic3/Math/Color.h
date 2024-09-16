
#ifndef __IC3_MATH_COLOR_H__
#define __IC3_MATH_COLOR_H__

#include "Vector.h"
#include <Ic3/Cppx/StaticLimits.h>

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

	union RGBAColor
	{
	public:
		struct
		{
			uint8 mU8Red;
			uint8 mU8Green;
			uint8 mU8Blue;
			uint8 mU8Alpha;
		};

		uint32 mU32Code = 0xFF;

	public:
		constexpr RGBAColor() noexcept
		: mU8Red( 0u )
		, mU8Green( 0u )
		, mU8Blue( 0u )
		, mU8Alpha( 255u )
		{}

		constexpr RGBAColor( uint32 pU32Code ) noexcept
		: mU32Code( pU32Code )
		{}

		constexpr RGBAColor( uint8 pRed, uint8 pGreen, uint8 pBlue, uint8 pAlpha = 0xFF ) noexcept
		: mU8Red( pRed )
		, mU8Green( pGreen )
		, mU8Blue( pBlue )
		, mU8Alpha( pAlpha )
		{}
	};

	template <typename TReal>
	union RGBAColorNorm
	{
	public:
		using ValueType = TReal;

		struct
		{
			TReal mFpRed;
			TReal mFpGreen;
			TReal mFpBlue;
			TReal mFpAlpha;
		};

		TReal mRGBA[4];

	public:
		constexpr RGBAColorNorm() noexcept
		: mFpRed( static_cast<TReal>( 0 ) )
		, mFpGreen( static_cast<TReal>( 0 ) )
		, mFpBlue( static_cast<TReal>( 0 ) )
		, mFpAlpha( static_cast<TReal>( 1 ) )
		{}

		constexpr RGBAColorNorm( TReal pRed, TReal pGreen, TReal pBlue, TReal pAlpha = static_cast<TReal>( 1 ) ) noexcept
		: mFpRed( pRed )
		, mFpGreen( pGreen )
		, mFpBlue( pBlue )
		, mFpAlpha( pAlpha )
		{}

		constexpr RGBAColorNorm( RGBAColor pColor )
		: mFpRed( static_cast<TReal>( pColor.mU8Red ) / static_cast<TReal>( Cppx::QLimits<uint8>::sMaxValue ) )
		, mFpGreen( static_cast<TReal>( pColor.mU8Green ) / static_cast<TReal>( Cppx::QLimits<uint8>::sMaxValue ) )
		, mFpBlue( static_cast<TReal>( pColor.mU8Blue ) / static_cast<TReal>( Cppx::QLimits<uint8>::sMaxValue ) )
		, mFpAlpha( static_cast<TReal>( pColor.mU8Alpha ) / static_cast<TReal>( Cppx::QLimits<uint8>::sMaxValue ) )
		{}

		constexpr RGBAColor toU8Color() const
		{
			return RGBAColor(
				static_cast<uint8>( mFpRed * Cppx::QLimits<uint8>::sMaxValue  ),
				static_cast<uint8>( mFpGreen * Cppx::QLimits<uint8>::sMaxValue  ),
				static_cast<uint8>( mFpBlue * Cppx::QLimits<uint8>::sMaxValue  ),
				static_cast<uint8>( mFpAlpha * Cppx::QLimits<uint8>::sMaxValue  ) );
		}
	};

	using RGBAColorU8 = RGBAColor;
	using RGBAColorR32Norm = RGBAColorNorm<float>;
	using RGBAColorR64Norm = RGBAColorNorm<double>;

	inline bool operator==( const RGBAColor & pLhs, const RGBAColor & pRhs )
	{
		return pLhs.mU32Code == pRhs.mU32Code;
	}

	inline bool operator!=( const RGBAColor & pLhs, const RGBAColor & pRhs )
	{
		return pLhs.mU32Code != pRhs.mU32Code;
	}

	template <typename TReal>
	inline bool operator==( const RGBAColorNorm<TReal> & pLhs, const RGBAColorNorm<TReal> & pRhs )
	{
		return ( pLhs.mFpRed == pRhs.mFpRed ) && ( pLhs.mFpGreen == pRhs.mFpGreen ) && ( pLhs.mFpBlue == pRhs.mFpBlue ) && ( pLhs.mFpAlpha == pRhs.mFpAlpha );
	}

	template <typename TReal>
	inline bool operator!=( const RGBAColorNorm<TReal> & pLhs, const RGBAColorNorm<TReal> & pRhs )
	{
		return ( pLhs.mFpRed != pRhs.mFpRed ) || ( pLhs.mFpGreen != pRhs.mFpGreen ) || ( pLhs.mFpBlue != pRhs.mFpBlue ) || ( pLhs.mFpAlpha != pRhs.mFpAlpha );
	}

	template <typename TReal>
	inline Vec4f colorVector( const RGBAColorNorm<TReal> & pColor )
	{
		return { pColor.mFpRed, pColor.mFpGreen, pColor.mFpBlue, pColor.mFpAlpha };
	}

	inline Vec4f colorVector( const RGBAColorU8 & pColor )
	{
		return colorVector( RGBAColorR32Norm( pColor ) );
	}

	RGBAColorU8 generateRandomColor();

	namespace colors
	{

		constexpr RGBAColorU8 cxColorBlueDeepSky{ 0x00, 0xB2, 0xEE };
		constexpr RGBAColorU8 cxColorBlueFacebookDark{ 0x3b, 0x59, 0x98 };
		constexpr RGBAColorU8 cxColorBlueFacebookLight{ 0x8b, 0x9d, 0xc3 };
		constexpr RGBAColorU8 cxColorBlueNavyDark{ 0x00, 0x00, 0x80 };
		constexpr RGBAColorU8 cxColorBlueSignatory{ 0x00, 0x3F, 0x77 };
		constexpr RGBAColorU8 cxColorPeachPuff{ 0xFF, 0xDA, 0xB9 };
		constexpr RGBAColorU8 cxColorTeal{ 0x00, 0x80, 0x80 };
		constexpr RGBAColorU8 cxColorTopaz{ 0x01, 0x98, 0xE1 };

		constexpr RGBAColorU8 CX_COLOR_BLACK_OPAQUE{ 0x00, 0x00, 0x00, 0xFF };
		constexpr RGBAColorU8 CX_COLOR_BLACK_TRANSPARENT{ 0x00, 0x00, 0x00, 0x00 };
		constexpr RGBAColorU8 CX_COLOR_WHITE_OPAQUE{ 0xFF, 0xFF, 0xFF, 0xFF };

	}

} // namespace Ic3::Math

#if( IC3_PCL_COMPILER & IC3_PCL_COMPILER_CLANG )
#  pragma clang diagnostic pop
#elif( IC3_PCL_COMPILER & IC3_PCL_COMPILER_GCC )
#  pragma GCC diagnostic pop
#elif( IC3_PCL_COMPILER & IC3_PCL_COMPILER_MSVC )
#  pragma warning( pop )
#endif

#endif // __IC3_MATH_COLOR_H__
