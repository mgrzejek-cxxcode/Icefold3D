
#ifndef __CXM_VECTOR_BASE_H__
#define __CXM_VECTOR_BASE_H__

#include "prerequisites.h"

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

	template <typename TPValue>
	struct vector4_simd_data
	{
		using type = TPValue[4];
	};

#if( CXM_SIMD_USE_VX128F )
	template <>
	struct vector4_simd_data<float>
	{
		using type = __m128;
	};
#endif

#if( CXM_SIMD_USE_VX128I )
	template <>
	struct vector4_simd_data<int32>
	{
		using type = __m128i;
	};

	template <>
	struct vector4_simd_data<uint32>
	{
		using type = __m128i;
	};
#endif

#if( CXM_SIMD_USE_VX256D )
	template <>
	struct vector4_simd_data<double>
	{
		using type = __m256d;
	};
#endif

#if( CXM_SIMD_USE_VX256I )
	template <>
	struct vector4_simd_data<int64>
	{
		using type = __m256i;
	};

	template <>
	struct vector4_simd_data<uint64>
	{
		using type = __m256i;
	};
#endif

	template <typename TPValue, size_t tpSize>
	struct vector_traits
	{
		static const uint32 length = tpSize;
		static const uint32 byte_size = sizeof( TPValue ) * length;
	};

	template <typename TPValue, size_t tpSize>
	struct vector;

	template <typename TPValue>
	struct vector<TPValue, 2>
	{
	public:
		using traits_type = vector_traits<TPValue, 2>;

		static const uint32 length = traits_type::length;
		static const uint32 byte_size = traits_type::byte_size;

		union
		{
			struct
			{
				// Explicit values for member access.
				TPValue x, y;
			};

			struct
			{
				TPValue values[2];
			};
		};

	public:
		constexpr vector( const vector & ) = default;
		constexpr vector & operator=( const vector & ) = default;

		constexpr vector() = default;

		template <typename TPOther>
		constexpr explicit vector( const vector<TPOther, 2> & pVec ) noexcept
		: x( static_cast<TPValue>( pVec.x ) )
		, y( static_cast<TPValue>( pVec.y ) )
		{}

		template <typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		constexpr explicit vector( TPScalar pScalar ) noexcept
		: x( static_cast<TPValue>( pScalar ) )
		, y( static_cast<TPValue>( pScalar ) )
		{}

		template <typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		constexpr explicit vector( const TPScalar * pData ) noexcept
		: x( static_cast<TPValue>( pData[0] ) )
		, y( static_cast<TPValue>( pData[1] ) )
		{}

		template <typename TX, typename TY>
		constexpr vector( TX pX, TY pY ) noexcept
		: x( static_cast<TPValue>( pX ) )
		, y( static_cast<TPValue>( pY ) )
		{}

		TPValue * data() noexcept
		{
			return &( values[0] );
		}

		const TPValue * data() const noexcept
		{
			return &( values[0] );
		}

		TPValue & operator[]( size_t pIndex ) noexcept
		{
			cppx_debug_assert( pIndex < length )
			return values[pIndex];
		}

		const TPValue & operator[]( size_t pIndex ) const noexcept
		{
			cppx_debug_assert( pIndex < length )
			return values[pIndex];
		}
	};

	template <typename TPValue>
	struct vector<TPValue, 3>
	{
	public:
		using traits_type = vector_traits<TPValue, 3>;

		static const uint32 length = traits_type::length;
		static const uint32 byte_size = traits_type::byte_size;

		union
		{
			struct
			{
				// Explicit values for member access.
				TPValue x, y, z;
			};

			struct
			{
				TPValue values[3];
			};
		};

	public:
		constexpr vector( const vector & ) = default;
		constexpr vector & operator=( const vector & ) = default;

		constexpr vector() = default;

		template <typename TPOther>
		constexpr explicit vector( const vector<TPOther, 3> & pVec ) noexcept
		: x( static_cast<TPValue>( pVec.x ) )
		, y( static_cast<TPValue>( pVec.y ) )
		, z( static_cast<TPValue>( pVec.z ) )
		{}

		template <typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		constexpr explicit vector( TPScalar pScalar ) noexcept
		: x( static_cast<TPValue>( pScalar ) )
		, y( static_cast<TPValue>( pScalar ) )
		, z( static_cast<TPValue>( pScalar ) )
		{}

		template <typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		constexpr explicit vector( const TPScalar * pData ) noexcept
		: x( static_cast<TPValue>( pData[0] ) )
		, y( static_cast<TPValue>( pData[1] ) )
		, z( static_cast<TPValue>( pData[2] ) )
		{}

		template <typename TXY, typename TZ>
		constexpr vector( const vector<TXY, 2> & pVecXY, TZ pZ ) noexcept
		: x( static_cast<TPValue>( pVecXY.x ) )
		, y( static_cast<TPValue>( pVecXY.y ) )
		, z( static_cast<TPValue>( pZ ) )
		{}

		template <typename TX, typename TYZ>
		constexpr vector( TX pX, const vector<TYZ, 2> & pVecYZ ) noexcept
		: x( static_cast<TPValue>( pX ) )
		, y( static_cast<TPValue>( pVecYZ.y ) )
		, z( static_cast<TPValue>( pVecYZ.z ) )
		{}

		template <typename TX, typename TY, typename TZ>
		constexpr vector( TX pX, TY pY, TZ pZ ) noexcept
		: x( static_cast<TPValue>( pX ) )
		, y( static_cast<TPValue>( pY ) )
		, z( static_cast<TPValue>( pZ ) )
		{}

		TPValue * data() noexcept
		{
			return &( values[0] );
		}

		const TPValue * data() const noexcept
		{
			return &( values[0] );
		}

		TPValue & operator[]( size_t pIndex ) noexcept
		{
			cppx_debug_assert( pIndex < length )
			return values[pIndex];
		}

		const TPValue & operator[]( size_t pIndex ) const noexcept
		{
			cppx_debug_assert( pIndex < length )
			return values[pIndex];
		}
	};

	template <typename TPValue>
	struct vector<TPValue, 4>
	{
	public:
		using traits_type = vector_traits<TPValue, 4>;
		using SIMDDataType = typename vector4_simd_data<TPValue>::type;

		static const uint32 length = traits_type::length;
		static const uint32 byte_size = traits_type::byte_size;

		union
		{
			struct
			{
				// Explicit values for member access.
				TPValue x, y, z, w;
			};

			struct
			{
				// Either generic array of values or SSE/AVX SIMD type (depends on support and build config).
				SIMDDataType mmv;
			};

			struct
			{
				TPValue values[4];
			};
		};

	public:
		constexpr vector( const vector & ) = default;
		constexpr vector & operator=( const vector & ) = default;

		constexpr vector() = default;

		constexpr explicit vector( SIMDDataType pMMV ) noexcept
		: mmv( pMMV )
		{}

		template <typename TPOther>
		constexpr explicit vector( const vector<TPOther, 4> & pVec ) noexcept
		: x( static_cast<TPValue>( pVec.x ) )
		, y( static_cast<TPValue>( pVec.y ) )
		, z( static_cast<TPValue>( pVec.z ) )
		, w( static_cast<TPValue>( pVec.w ) )
		{}

		template <typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		constexpr explicit vector( TPScalar pScalar ) noexcept
		: x( static_cast<TPValue>( pScalar ) )
		, y( static_cast<TPValue>( pScalar ) )
		, z( static_cast<TPValue>( pScalar ) )
		, w( static_cast<TPValue>( pScalar ) )
		{}

		template <typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		constexpr explicit vector( const TPScalar * pData ) noexcept
		: x( static_cast<TPValue>( pData[0] ) )
		, y( static_cast<TPValue>( pData[1] ) )
		, z( static_cast<TPValue>( pData[2] ) )
		, w( static_cast<TPValue>( pData[3] ) )
		{}

		template <typename TXY, typename TZ, typename TW>
		constexpr vector( const vector<TXY, 2> & pVecXY, TZ pZ, TW pW ) noexcept
		: x( static_cast<TPValue>( pVecXY.x ) )
		, y( static_cast<TPValue>( pVecXY.y ) )
		, z( static_cast<TPValue>( pZ ) )
		, w( static_cast<TPValue>( pW ) )
		{}

		template <typename TX, typename TYZ, typename TW>
		constexpr vector( TX pX, const vector<TYZ, 2> & pVecYZ, TW pW ) noexcept
		: x( static_cast<TPValue>( pX ) )
		, y( static_cast<TPValue>( pVecYZ.y ) )
		, z( static_cast<TPValue>( pVecYZ.z ) )
		, w( static_cast<TPValue>( pW ) )
		{}

		template <typename TX, typename TY, typename TZW>
		constexpr vector( TX pX, TPValue pY, const vector<TZW, 2> & pVecZW ) noexcept
		: x( static_cast<TPValue>( pX ) )
		, y( static_cast<TPValue>( pY ) )
		, z( static_cast<TPValue>( pVecZW.z ) )
		, w( static_cast<TPValue>( pVecZW.w ) )
		{}

		template <typename TXYZ, typename TW>
		constexpr vector( const vector<TXYZ, 3> & pVecXYZ, TW pW ) noexcept
		: x( static_cast<TPValue>( pVecXYZ.x ) )
		, y( static_cast<TPValue>( pVecXYZ.y ) )
		, z( static_cast<TPValue>( pVecXYZ.z ) )
		, w( static_cast<TPValue>( pW ) )
		{}

		template <typename TX, typename TYZW>
		constexpr vector( TX pX, const vector<TYZW, 3> & pVecYZW ) noexcept
		: x( static_cast<TPValue>( pX ) )
		, y( static_cast<TPValue>( pVecYZW.y ) )
		, z( static_cast<TPValue>( pVecYZW.z ) )
		, w( static_cast<TPValue>( pVecYZW.w ) )
		{}

		template <typename TX, typename TY, typename TZ, typename TW>
		constexpr vector( TX pX, TY pY, TZ pZ, TW pW ) noexcept
		: x( static_cast<TPValue>( pX ) )
		, y( static_cast<TPValue>( pY ) )
		, z( static_cast<TPValue>( pZ ) )
		, w( static_cast<TPValue>( pW ) )
		{}

		TPValue & operator[]( size_t pIndex ) noexcept
		{
			cppx_debug_assert( pIndex < length )
			return values[pIndex];
		}

		const TPValue & operator[]( size_t pIndex ) const noexcept
		{
			cppx_debug_assert( pIndex < length )
			return values[pIndex];
		}

		TPValue * data() noexcept
		{
			return &( values[0] );
		}

		const TPValue * data() const noexcept
		{
			return &( values[0] );
		}

		SIMDDataType * simd_ptr() noexcept
		{
			return &mmv;
		}

		const SIMDDataType * simd_ptr() const noexcept
		{
			return &mmv;
		}
	};

	template <typename TPValue>
	using vec2 = vector<TPValue, 2>;

	template <typename TPValue>
	using vec3 = vector<TPValue, 3>;

	template <typename TPValue>
	using vec4 = vector<TPValue, 4>;

	template <typename TPValue>
	using vector2 = vector<TPValue, 2>;

	template <typename TPValue>
	using vector3 = vector<TPValue, 3>;

	template <typename TPValue>
	using vector4 = vector<TPValue, 4>;

	using vec2i16 = vector2<int16>;
	using vec2u16 = vector2<uint16>;
	using vec2i32 = vector2<int32>;
	using vec2u32 = vector2<uint32>;
	using vec2i64 = vector2<int64>;
	using vec2u64 = vector2<uint64>;
	using vec2f   = vector2<float>;
	using vec2d   = vector2<double>;

	using vec3i32 = vector3<int32>;
	using vec3u32 = vector3<uint32>;
	using vec3i64 = vector3<int64>;
	using vec3u64 = vector3<uint64>;
	using vec3f   = vector3<float>;
	using vec3d   = vector3<double>;

	using vec4i16 = vector4<int16>;
	using vec4u16 = vector4<uint16>;
	using vec4i32 = vector4<int32>;
	using vec4u32 = vector4<uint32>;
	using vec4i64 = vector4<int64>;
	using vec4u64 = vector4<uint64>;
	using vec4u8n = vector4<uint8>;
	using vec4f   = vector4<float>;
	using vec4d   = vector4<double>;

	using pos2f  = vector2<float>;
	using pos2i  = vector2<int32>;
	using pos3f  = vector3<float>;
	using pos3i  = vector3<int32>;
	using size2f = vector2<float>;
	using size2u = vector2<uint32>;
	using size3f = vector3<float>;
	using size3u = vector3<uint32>;

} // namespace cxm

#if( PCL_COMPILER & PCL_COMPILER_CLANG )
#  pragma clang diagnostic pop
#elif( PCL_COMPILER & PCL_COMPILER_GCC )
#  pragma GCC diagnostic pop
#elif( PCL_COMPILER & PCL_COMPILER_MSVC )
#  pragma warning( pop )
#endif

#endif // __CXM_VECTOR_BASE_H__
