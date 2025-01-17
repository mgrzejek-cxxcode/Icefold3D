
#ifndef __IC3_MATH_COLOR_H__
#define __IC3_MATH_COLOR_H__

#include "Vector.h"
#include <cppx/staticLimits.h>

#if( PCL_COMPILER & PCL_COMPILER_CLANG )
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#  pragma clang diagnostic ignored "-Wnested-anon-types"
#elif( PCL_COMPILER & PCL_COMPILER_GCC )
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wpedantic"
#elif( PCL_COMPILER & PCL_COMPILER_MSVC )
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
			uint8 u8Red;
			uint8 u8Green;
			uint8 u8Blue;
			uint8 u8Alpha;
		};

		uint32 u32Code = 0xFF;

	public:
		constexpr RGBAColor() noexcept
		: u8Red( 0u )
		, u8Green( 0u )
		, u8Blue( 0u )
		, u8Alpha( 255u )
		{}

		constexpr RGBAColor( uint32 pU32Code ) noexcept
		: u32Code( pU32Code )
		{}

		constexpr RGBAColor( uint8 pRed, uint8 pGreen, uint8 pBlue, uint8 pAlpha = 0xFF ) noexcept
		: u8Red( pRed )
		, u8Green( pGreen )
		, u8Blue( pBlue )
		, u8Alpha( pAlpha )
		{}
	};

	template <typename TReal>
	union RGBAColorNorm
	{
	public:
		using ValueType = TReal;

		struct
		{
			TReal fpRed;
			TReal fpGreen;
			TReal fpBlue;
			TReal fpAlpha;
		};

		TReal mRGBA[4];

	public:
		constexpr RGBAColorNorm() noexcept
		: fpRed( static_cast<TReal>( 0 ) )
		, fpGreen( static_cast<TReal>( 0 ) )
		, fpBlue( static_cast<TReal>( 0 ) )
		, fpAlpha( static_cast<TReal>( 1 ) )
		{}

		constexpr RGBAColorNorm( TReal pRed, TReal pGreen, TReal pBlue, TReal pAlpha = static_cast<TReal>( 1 ) ) noexcept
		: fpRed( pRed )
		, fpGreen( pGreen )
		, fpBlue( pBlue )
		, fpAlpha( pAlpha )
		{}

		constexpr RGBAColorNorm( RGBAColor pColor )
		: fpRed( static_cast<TReal>( pColor.u8Red ) / static_cast<TReal>( cppx::meta::limits<uint8>::max_value ) )
		, fpGreen( static_cast<TReal>( pColor.u8Green ) / static_cast<TReal>( cppx::meta::limits<uint8>::max_value ) )
		, fpBlue( static_cast<TReal>( pColor.u8Blue ) / static_cast<TReal>( cppx::meta::limits<uint8>::max_value ) )
		, fpAlpha( static_cast<TReal>( pColor.u8Alpha ) / static_cast<TReal>( cppx::meta::limits<uint8>::max_value ) )
		{}

		constexpr RGBAColor toU8Color() const
		{
			return RGBAColor(
				static_cast<uint8>( fpRed * cppx::meta::limits<uint8>::max_value  ),
				static_cast<uint8>( fpGreen * cppx::meta::limits<uint8>::max_value  ),
				static_cast<uint8>( fpBlue * cppx::meta::limits<uint8>::max_value  ),
				static_cast<uint8>( fpAlpha * cppx::meta::limits<uint8>::max_value  ) );
		}
	};

	using RGBAColorU8 = RGBAColor;
	using RGBAColorR32Norm = RGBAColorNorm<float>;
	using RGBAColorR64Norm = RGBAColorNorm<double>;

	inline bool operator==( const RGBAColor & pLhs, const RGBAColor & pRhs )
	{
		return pLhs.u32Code == pRhs.u32Code;
	}

	inline bool operator!=( const RGBAColor & pLhs, const RGBAColor & pRhs )
	{
		return pLhs.u32Code != pRhs.u32Code;
	}

	template <typename TReal>
	inline bool operator==( const RGBAColorNorm<TReal> & pLhs, const RGBAColorNorm<TReal> & pRhs )
	{
		return ( pLhs.fpRed == pRhs.fpRed ) && ( pLhs.fpGreen == pRhs.fpGreen ) && ( pLhs.fpBlue == pRhs.fpBlue ) && ( pLhs.fpAlpha == pRhs.fpAlpha );
	}

	template <typename TReal>
	inline bool operator!=( const RGBAColorNorm<TReal> & pLhs, const RGBAColorNorm<TReal> & pRhs )
	{
		return ( pLhs.fpRed != pRhs.fpRed ) || ( pLhs.fpGreen != pRhs.fpGreen ) || ( pLhs.fpBlue != pRhs.fpBlue ) || ( pLhs.fpAlpha != pRhs.fpAlpha );
	}

	template <typename TReal>
	inline Vec4f colorVector( const RGBAColorNorm<TReal> & pColor )
	{
		return { pColor.fpRed, pColor.fpGreen, pColor.fpBlue, pColor.fpAlpha };
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

#if( PCL_COMPILER & PCL_COMPILER_CLANG )
#  pragma clang diagnostic pop
#elif( PCL_COMPILER & PCL_COMPILER_GCC )
#  pragma GCC diagnostic pop
#elif( PCL_COMPILER & PCL_COMPILER_MSVC )
#  pragma warning( pop )
#endif

#endif // __IC3_MATH_COLOR_H__
