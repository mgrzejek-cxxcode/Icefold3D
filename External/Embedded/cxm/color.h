
#ifndef __CXM_COLOR_H__
#define __CXM_COLOR_H__

#include "vector.h"
#include <cppx/typeLimits.h>

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

namespace cxm
{

	union rgba_color
	{
	public:
		struct
		{
			uint8 u8_red;
			uint8 u8_green;
			uint8 u8_blue;
			uint8 u8_alpha;
		};

		uint32 u32_code = 0xFF;

	public:
		constexpr rgba_color() noexcept
		: u8_red( 0u )
		, u8_green( 0u )
		, u8_blue( 0u )
		, u8_alpha( 255u )
		{}

		constexpr rgba_color( uint32 pU32Code ) noexcept
		: u32_code( pU32Code )
		{}

		constexpr rgba_color( uint8 pRed, uint8 pGreen, uint8 pBlue, uint8 pAlpha = 0xFF ) noexcept
		: u8_red( pRed )
		, u8_green( pGreen )
		, u8_blue( pBlue )
		, u8_alpha( pAlpha )
		{}
	};

	template <typename TPReal>
	union rgba_color_norm
	{
	public:
		using ValueType = TPReal;

		struct
		{
			TPReal ufp_red;
			TPReal ufp_green;
			TPReal ufp_blue;
			TPReal ufp_alpha;
		};

		TPReal uv_rgba[4];

	public:
		constexpr rgba_color_norm() noexcept
		: ufp_red( static_cast<TPReal>( 0 ) )
		, ufp_green( static_cast<TPReal>( 0 ) )
		, ufp_blue( static_cast<TPReal>( 0 ) )
		, ufp_alpha( static_cast<TPReal>( 1 ) )
		{}

		constexpr rgba_color_norm( TPReal pRed, TPReal pGreen, TPReal pBlue, TPReal pAlpha = static_cast<TPReal>( 1 ) ) noexcept
		: ufp_red( pRed )
		, ufp_green( pGreen )
		, ufp_blue( pBlue )
		, ufp_alpha( pAlpha )
		{}

		constexpr rgba_color_norm( rgba_color pColor )
		: ufp_red( static_cast<TPReal>( pColor.u8_red ) / static_cast<TPReal>( cppx::meta::limits<uint8>::max_value ) )
		, ufp_green( static_cast<TPReal>( pColor.u8_green ) / static_cast<TPReal>( cppx::meta::limits<uint8>::max_value ) )
		, ufp_blue( static_cast<TPReal>( pColor.u8_blue ) / static_cast<TPReal>( cppx::meta::limits<uint8>::max_value ) )
		, ufp_alpha( static_cast<TPReal>( pColor.u8_alpha ) / static_cast<TPReal>( cppx::meta::limits<uint8>::max_value ) )
		{}

		constexpr rgba_color ToU8Color() const
		{
			return rgba_color(
				static_cast<uint8>( ufp_red * cppx::meta::limits<uint8>::max_value  ),
				static_cast<uint8>( ufp_green * cppx::meta::limits<uint8>::max_value  ),
				static_cast<uint8>( ufp_blue * cppx::meta::limits<uint8>::max_value  ),
				static_cast<uint8>( ufp_alpha * cppx::meta::limits<uint8>::max_value  ) );
		}
	};

	using rgba_color_u8 = rgba_color;
	using rgba_color_r32_norm = rgba_color_norm<float>;
	using rgba_color_r64_norm = rgba_color_norm<double>;

	inline bool operator==( const rgba_color & pLhs, const rgba_color & pRhs )
	{
		return pLhs.u32_code == pRhs.u32_code;
	}

	inline bool operator!=( const rgba_color & pLhs, const rgba_color & pRhs )
	{
		return pLhs.u32_code != pRhs.u32_code;
	}

	template <typename TPReal>
	inline bool operator==( const rgba_color_norm<TPReal> & pLhs, const rgba_color_norm<TPReal> & pRhs )
	{
		return ( pLhs.ufp_red == pRhs.ufp_red ) && ( pLhs.ufp_green == pRhs.ufp_green ) && ( pLhs.ufp_blue == pRhs.ufp_blue ) && ( pLhs.ufp_alpha == pRhs.ufp_alpha );
	}

	template <typename TPReal>
	inline bool operator!=( const rgba_color_norm<TPReal> & pLhs, const rgba_color_norm<TPReal> & pRhs )
	{
		return ( pLhs.ufp_red != pRhs.ufp_red ) || ( pLhs.ufp_green != pRhs.ufp_green ) || ( pLhs.ufp_blue != pRhs.ufp_blue ) || ( pLhs.ufp_alpha != pRhs.ufp_alpha );
	}

	template <typename TPReal>
	static inline vec4f make_color_vec4f( const rgba_color_norm<TPReal> & pColor )
	{
		return { pColor.ufp_red, pColor.ufp_green, pColor.ufp_blue, pColor.ufp_alpha };
	}

	static inline vec4f make_color_vec4f( const rgba_color_u8 & pColor )
	{
		return make_color_vec4f( rgba_color_r32_norm{ pColor } );
	}

	rgba_color_u8 generate_random_color();

	constexpr rgba_color_u8 kColorBlueDeepSky{0x00, 0xB2, 0xEE};
	constexpr rgba_color_u8 kColorBlueFacebookDark{0x3b, 0x59, 0x98};
	constexpr rgba_color_u8 kColorBlueFacebookLight{0x8b, 0x9d, 0xc3};
	constexpr rgba_color_u8 kColorBlueNavyDark{0x00, 0x00, 0x80};
	constexpr rgba_color_u8 kColorBlueSignatory{0x00, 0x3F, 0x77};
	constexpr rgba_color_u8 kColorPeachPuff{0xFF, 0xDA, 0xB9};
	constexpr rgba_color_u8 kColorTeal{0x00, 0x80, 0x80};
	constexpr rgba_color_u8 kColorTopaz{0x01, 0x98, 0xE1};

	constexpr rgba_color_u8 kColorBlackOpaque{0x00, 0x00, 0x00, 0xFF};
	constexpr rgba_color_u8 kColorBlackTransparent{0x00, 0x00, 0x00, 0x00};
	constexpr rgba_color_u8 kColorWhiteOpaque{0xFF, 0xFF, 0xFF, 0xFF};

} // namespace cxm

#if( PCL_COMPILER & PCL_COMPILER_CLANG )
#  pragma clang diagnostic pop
#elif( PCL_COMPILER & PCL_COMPILER_GCC )
#  pragma GCC diagnostic pop
#elif( PCL_COMPILER & PCL_COMPILER_MSVC )
#  pragma warning( pop )
#endif

#endif // __CXM_COLOR_H__
