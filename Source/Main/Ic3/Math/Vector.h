
#ifndef __IC3_MATH_VECTOR_BASE_H__
#define __IC3_MATH_VECTOR_BASE_H__

#include "Prerequisites.h"

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

	template <typename TPValue>
	struct Vector4SIMDData
	{
		using Type = TPValue[4];
	};

#if( IC3_MATH_SIMD_USE_VX128F )
	template <>
	struct Vector4SIMDData<float>
	{
		using Type = __m128;
	};
#endif

#if( IC3_MATH_SIMD_USE_VX128I )
	template <>
	struct Vector4SIMDData<int32>
	{
		using Type = __m128i;
	};

	template <>
	struct Vector4SIMDData<uint32>
	{
		using Type = __m128i;
	};
#endif

#if( IC3_MATH_SIMD_USE_VX256D )
	template <>
	struct Vector4SIMDData<double>
	{
		using Type = __m256d;
	};
#endif

#if( IC3_MATH_SIMD_USE_VX256I )
	template <>
	struct Vector4SIMDData<int64>
	{
		using Type = __m256i;
	};

	template <>
	struct Vector4SIMDData<uint64>
	{
		using Type = __m256i;
	};
#endif

	template <typename TPValue, size_t tpSize>
	struct VectorTraits
	{
		static const uint32 sLength = tpSize;
		static const uint32 sByteSize = sizeof( TPValue ) * sLength;
	};

	template <typename TPValue, size_t tpSize>
	struct Vector;

	template <typename TPValue>
	struct Vector<TPValue, 2>
	{
	public:
		using Traits = VectorTraits<TPValue, 2>;

		static const uint32 sLength = Traits::sLength;
		static const uint32 sByteSize = Traits::sByteSize;

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
		constexpr Vector( const Vector & ) = default;
		constexpr Vector & operator=( const Vector & ) = default;

		constexpr Vector() = default;

		template <typename TPOther>
		constexpr explicit Vector( const Vector<TPOther, 2> & pVec ) noexcept
		: x( static_cast<TPValue>( pVec.x ) )
		, y( static_cast<TPValue>( pVec.y ) )
		{}

		template <typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		constexpr explicit Vector( TPScalar pScalar ) noexcept
		: x( static_cast<TPValue>( pScalar ) )
		, y( static_cast<TPValue>( pScalar ) )
		{}

		template <typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		constexpr explicit Vector( const TPScalar * pData ) noexcept
		: x( static_cast<TPValue>( pData[0] ) )
		, y( static_cast<TPValue>( pData[1] ) )
		{}

		template <typename TX, typename TY>
		constexpr Vector( TX pX, TY pY ) noexcept
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
			Ic3DebugAssert( pIndex < sLength )
			return values[pIndex];
		}

		const TPValue & operator[]( size_t pIndex ) const noexcept
		{
			Ic3DebugAssert( pIndex < sLength )
			return values[pIndex];
		}
	};

	template <typename TPValue>
	struct Vector<TPValue, 3>
	{
	public:
		using Traits = VectorTraits<TPValue, 3>;

		static const uint32 sLength = Traits::sLength;
		static const uint32 sByteSize = Traits::sByteSize;

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
		constexpr Vector( const Vector & ) = default;
		constexpr Vector & operator=( const Vector & ) = default;

		constexpr Vector() = default;

		template <typename TPOther>
		constexpr explicit Vector( const Vector<TPOther, 3> & pVec ) noexcept
		: x( static_cast<TPValue>( pVec.x ) )
		, y( static_cast<TPValue>( pVec.y ) )
		, z( static_cast<TPValue>( pVec.z ) )
		{}

		template <typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		constexpr explicit Vector( TPScalar pScalar ) noexcept
		: x( static_cast<TPValue>( pScalar ) )
		, y( static_cast<TPValue>( pScalar ) )
		, z( static_cast<TPValue>( pScalar ) )
		{}

		template <typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		constexpr explicit Vector( const TPScalar * pData ) noexcept
		: x( static_cast<TPValue>( pData[0] ) )
		, y( static_cast<TPValue>( pData[1] ) )
		, z( static_cast<TPValue>( pData[2] ) )
		{}

		template <typename TXY, typename TZ>
		constexpr Vector( const Vector<TXY, 2> & pVecXY, TZ pZ ) noexcept
		: x( static_cast<TPValue>( pVecXY.x ) )
		, y( static_cast<TPValue>( pVecXY.y ) )
		, z( static_cast<TPValue>( pZ ) )
		{}

		template <typename TX, typename TYZ>
		constexpr Vector( TX pX, const Vector<TYZ, 2> & pVecYZ ) noexcept
		: x( static_cast<TPValue>( pX ) )
		, y( static_cast<TPValue>( pVecYZ.y ) )
		, z( static_cast<TPValue>( pVecYZ.z ) )
		{}

		template <typename TX, typename TY, typename TZ>
		constexpr Vector( TX pX, TY pY, TZ pZ ) noexcept
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
			Ic3DebugAssert( pIndex < sLength )
			return values[pIndex];
		}

		const TPValue & operator[]( size_t pIndex ) const noexcept
		{
			Ic3DebugAssert( pIndex < sLength )
			return values[pIndex];
		}
	};

	template <typename TPValue>
	struct Vector<TPValue, 4>
	{
	public:
		using Traits = VectorTraits<TPValue, 4>;
		using SIMDDataType = typename Vector4SIMDData<TPValue>::Type;

		static const uint32 sLength = Traits::sLength;
		static const uint32 sByteSize = Traits::sByteSize;

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
		constexpr Vector( const Vector & ) = default;
		constexpr Vector & operator=( const Vector & ) = default;

		constexpr Vector() = default;

		constexpr explicit Vector( SIMDDataType pMMV ) noexcept
		: mmv( pMMV )
		{}

		template <typename TPOther>
		constexpr explicit Vector( const Vector<TPOther, 4> & pVec ) noexcept
		: x( static_cast<TPValue>( pVec.x ) )
		, y( static_cast<TPValue>( pVec.y ) )
		, z( static_cast<TPValue>( pVec.z ) )
		, w( static_cast<TPValue>( pVec.w ) )
		{}

		template <typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		constexpr explicit Vector( TPScalar pScalar ) noexcept
		: x( static_cast<TPValue>( pScalar ) )
		, y( static_cast<TPValue>( pScalar ) )
		, z( static_cast<TPValue>( pScalar ) )
		, w( static_cast<TPValue>( pScalar ) )
		{}

		template <typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		constexpr explicit Vector( const TPScalar * pData ) noexcept
		: x( static_cast<TPValue>( pData[0] ) )
		, y( static_cast<TPValue>( pData[1] ) )
		, z( static_cast<TPValue>( pData[2] ) )
		, w( static_cast<TPValue>( pData[3] ) )
		{}

		template <typename TXY, typename TZ, typename TW>
		constexpr Vector( const Vector<TXY, 2> & pVecXY, TZ pZ, TW pW ) noexcept
		: x( static_cast<TPValue>( pVecXY.x ) )
		, y( static_cast<TPValue>( pVecXY.y ) )
		, z( static_cast<TPValue>( pZ ) )
		, w( static_cast<TPValue>( pW ) )
		{}

		template <typename TX, typename TYZ, typename TW>
		constexpr Vector( TX pX, const Vector<TYZ, 2> & pVecYZ, TW pW ) noexcept
		: x( static_cast<TPValue>( pX ) )
		, y( static_cast<TPValue>( pVecYZ.y ) )
		, z( static_cast<TPValue>( pVecYZ.z ) )
		, w( static_cast<TPValue>( pW ) )
		{}

		template <typename TX, typename TY, typename TZW>
		constexpr Vector( TX pX, TPValue pY, const Vector<TZW, 2> & pVecZW ) noexcept
		: x( static_cast<TPValue>( pX ) )
		, y( static_cast<TPValue>( pY ) )
		, z( static_cast<TPValue>( pVecZW.z ) )
		, w( static_cast<TPValue>( pVecZW.w ) )
		{}

		template <typename TXYZ, typename TW>
		constexpr Vector( const Vector<TXYZ, 3> & pVecXYZ, TW pW ) noexcept
		: x( static_cast<TPValue>( pVecXYZ.x ) )
		, y( static_cast<TPValue>( pVecXYZ.y ) )
		, z( static_cast<TPValue>( pVecXYZ.z ) )
		, w( static_cast<TPValue>( pW ) )
		{}

		template <typename TX, typename TYZW>
		constexpr Vector( TX pX, const Vector<TYZW, 3> & pVecYZW ) noexcept
		: x( static_cast<TPValue>( pX ) )
		, y( static_cast<TPValue>( pVecYZW.y ) )
		, z( static_cast<TPValue>( pVecYZW.z ) )
		, w( static_cast<TPValue>( pVecYZW.w ) )
		{}

		template <typename TX, typename TY, typename TZ, typename TW>
		constexpr Vector( TX pX, TY pY, TZ pZ, TW pW ) noexcept
		: x( static_cast<TPValue>( pX ) )
		, y( static_cast<TPValue>( pY ) )
		, z( static_cast<TPValue>( pZ ) )
		, w( static_cast<TPValue>( pW ) )
		{}

		TPValue & operator[]( size_t pIndex ) noexcept
		{
			Ic3DebugAssert( pIndex < sLength )
			return values[pIndex];
		}

		const TPValue & operator[]( size_t pIndex ) const noexcept
		{
			Ic3DebugAssert( pIndex < sLength )
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

		SIMDDataType * simdPtr() noexcept
		{
			return &mmv;
		}

		const SIMDDataType * simdPtr() const noexcept
		{
			return &mmv;
		}
	};

	template <typename TPValue>
	using Vec2 = Vector<TPValue, 2>;

	template <typename TPValue>
	using Vec3 = Vector<TPValue, 3>;

	template <typename TPValue>
	using Vec4 = Vector<TPValue, 4>;

	template <typename TPValue>
	using Vector2 = Vector<TPValue, 2>;

	template <typename TPValue>
	using Vector3 = Vector<TPValue, 3>;

	template <typename TPValue>
	using Vector4 = Vector<TPValue, 4>;

	using Vec2i16 = Vector2<int16>;
	using Vec2u16 = Vector2<uint16>;
	using Vec2i32 = Vector2<int32>;
	using Vec2u32 = Vector2<uint32>;
	using Vec2i64 = Vector2<int64>;
	using Vec2u64 = Vector2<uint64>;
	using Vec2f   = Vector2<float>;
	using Vec2d   = Vector2<double>;

	using Vec3i32 = Vector3<int32>;
	using Vec3u32 = Vector3<uint32>;
	using Vec3i64 = Vector3<int64>;
	using Vec3u64 = Vector3<uint64>;
	using Vec3f   = Vector3<float>;
	using Vec3d   = Vector3<double>;

	using Vec4i16 = Vector4<int16>;
	using Vec4u16 = Vector4<uint16>;
	using Vec4i32 = Vector4<int32>;
	using Vec4u32 = Vector4<uint32>;
	using Vec4i64 = Vector4<int64>;
	using Vec4u64 = Vector4<uint64>;
	using Vec4u8n = Vector4<uint8>;
	using Vec4f   = Vector4<float>;
	using Vec4d   = Vector4<double>;

	using Pos2f  = Vector2<float>;
	using Pos2i  = Vector2<int32>;
	using Pos3f  = Vector3<float>;
	using Pos3i  = Vector3<int32>;
	using Size2f = Vector2<float>;
	using Size2u = Vector2<uint32>;
	using Size3f = Vector3<float>;
	using Size3u = Vector3<uint32>;

} // namespace Ic3::Math

#if( PCL_COMPILER & PCL_COMPILER_CLANG )
#  pragma clang diagnostic pop
#elif( PCL_COMPILER & PCL_COMPILER_GCC )
#  pragma GCC diagnostic pop
#elif( PCL_COMPILER & PCL_COMPILER_MSVC )
#  pragma warning( pop )
#endif

#endif // __IC3_MATH_VECTOR_BASE_H__
