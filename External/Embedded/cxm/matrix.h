
#ifndef __CXM_MATRIX_BASE_H__
#define __CXM_MATRIX_BASE_H__

#include "vector.h"

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

	// traits_type container struct for common properties of matrix types.
	template <typename TPValue, size_t tpRows, size_t tpColumns>
	struct matrix_traits
	{
		static const uint32 row_length = tpColumns;
		static const uint32 rows_num = tpRows;
		static const uint32 columns_num = tpColumns;
		static const uint32 length = rows_num * columns_num;
		static const uint32 byte_size = sizeof( TPValue ) * length;
		static const uint32 row_byte_pitch = sizeof( TPValue ) * row_length;
	};

	template <typename TPValue, size_t /* tpRows */, size_t tpColumns>
	struct matrix_row_simd_data
	{
		using type = TPValue[tpColumns];
	};

	template <typename TPValue, size_t tpRows>
	struct matrix_row_simd_data<TPValue, tpRows, 4>
	{
		using type = typename vector4_simd_data<TPValue>::type;
	};

	// Common class for all matrix types. The default implementation is empty, because we need
	// more fine-grained control over members definition.
	// Internally, matrix values are represented as arrays of rows (similar to DXM library),
	// so there is a specialized definition for MatrixBase<T, 2, R>, MatrixBase<T, 3, R> and
	// MatrixBase<T, 4, R> for 2xR, 3xR and 4xR matrices of type T, respectively.
	template <typename TPValue, size_t tpRows, size_t tpColumns>
	struct matrix;


	template <typename TPValue, size_t tpColumns>
	struct matrix<TPValue, 2, tpColumns>
	{
	public:
		static constexpr size_t rows_num = 2;
		static constexpr size_t columns_num = tpColumns;
		static constexpr size_t matrix_size = rows_num * columns_num;

		using traits_type = matrix_traits<TPValue, 2, tpColumns>;
		using column_vector = vector2<TPValue>;
		using row_vector = vector<TPValue, tpColumns>;
		using row_simd_data_type = typename matrix_row_simd_data<TPValue, rows_num, tpColumns>::type;

		union
		{
			struct
			{
				row_simd_data_type mm0, mm1;
			};

			struct
			{
				row_vector row0, row1;
			};

			struct
			{
				TPValue values[matrix_size];
			};
		};

	public:
		constexpr matrix( const matrix & ) = default;
		constexpr matrix & operator=( const matrix & ) = default;

		constexpr matrix() noexcept = default;

		template <typename T0, typename T1>
		constexpr matrix(
			const vector<T0, tpColumns> & pRow0,
			const vector<T1, tpColumns> & pRow1 ) noexcept
		: row0( pRow0 )
		, row1( pRow1 )
		{}

		template <typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		constexpr explicit matrix( TPScalar pScalar ) noexcept
		: row0( pScalar )
		, row1( pScalar )
		{}

		matrix( std::initializer_list<TPValue> pData ) noexcept
		: row0( pData.begin() )
		, row1( pData.begin() + tpColumns )
		{
			Ic3DebugAssert( pData.size() == matrix_size );
		}

		CPPX_ATTR_NO_DISCARD vector<TPValue, tpColumns> & operator[]( size_t pIndex ) noexcept
		{
			Ic3DebugAssert( pIndex < rows_num );
			return static_cast<row_vector *>( &row0 )[pIndex];
		}

		CPPX_ATTR_NO_DISCARD const vector<TPValue, tpColumns> & operator[]( size_t pIndex ) const noexcept
		{
			Ic3DebugAssert( pIndex < rows_num );
			return static_cast<const row_vector *>( &row0 )[pIndex];
		}

		CPPX_ATTR_NO_DISCARD column_vector column( size_t pColumnIndex ) const noexcept
		{
			return { row0[pColumnIndex], row1[pColumnIndex] };
		}

		CPPX_ATTR_NO_DISCARD TPValue * data() noexcept
		{
			return &( values[0] );
		}

		CPPX_ATTR_NO_DISCARD const TPValue * data() const noexcept
		{
			return &( values[0] );
		}

		CPPX_ATTR_NO_DISCARD row_vector * row_ptr( size_t pIndex = 0 ) noexcept
		{
			Ic3DebugAssert( pIndex < rows_num );
			return static_cast<row_vector *>( &row0 ) + pIndex;
		}

		CPPX_ATTR_NO_DISCARD const row_vector * row_ptr( size_t pIndex = 0 ) const noexcept
		{
			Ic3DebugAssert( pIndex < rows_num );
			return static_cast<const row_vector *>( &row0 ) + pIndex;
		}

		CPPX_ATTR_NO_DISCARD row_simd_data_type * simd_ptr( size_t pIndex = 0 ) noexcept
		{
			Ic3DebugAssert( pIndex < rows_num );
			return static_cast<row_simd_data_type *>( &mm0 ) + pIndex;
		}

		CPPX_ATTR_NO_DISCARD const row_simd_data_type * simd_ptr( size_t pIndex = 0 ) const noexcept
		{
			Ic3DebugAssert( pIndex < rows_num );
			return static_cast<const row_simd_data_type *>( &mm0 ) + pIndex;
		}
	};


	template <typename TPValue, size_t tpColumns>
	struct matrix<TPValue, 3, tpColumns>
	{
	public:
		static constexpr size_t rows_num = 3;
		static constexpr size_t columns_num = tpColumns;
		static constexpr size_t matrix_size = rows_num * columns_num;
		using row_simd_data_type = typename matrix_row_simd_data<TPValue, rows_num, tpColumns>::type;

		using traits_type = matrix_traits<TPValue, 3, tpColumns>;
		using column_vector = vector<TPValue, 3>;
		using row_vector = vector<TPValue, tpColumns>;

		union
		{
			struct
			{
				row_simd_data_type mm0, mm1, mm2;
			};

			struct
			{
				row_vector row0, row1, row2;
			};

			struct
			{
				TPValue values[matrix_size];
			};
		};

	public:
		constexpr matrix( const matrix & ) = default;
		constexpr matrix & operator=( const matrix & ) = default;

		constexpr matrix() noexcept = default;

		template <typename T0, typename T1, typename T2>
		constexpr matrix(
			const vector<T0, tpColumns> & pRow0,
			const vector<T1, tpColumns> & pRow1,
			const vector<T2, tpColumns> & pRow2 ) noexcept
		: row0( pRow0 )
		, row1( pRow1 )
		, row2( pRow2 )
		{}

		template <typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		constexpr explicit matrix( TPScalar pScalar ) noexcept
		: row0( pScalar )
		, row1( pScalar )
		, row2( pScalar )
		{}

		matrix( std::initializer_list<TPValue> pData ) noexcept
		: row0( pData.begin() )
		, row1( pData.begin() + tpColumns )
		, row2( pData.begin() + 2*tpColumns )
		{
			Ic3DebugAssert( pData.size() == matrix_size );
		}

		CPPX_ATTR_NO_DISCARD vector<TPValue, tpColumns> & operator[]( size_t pIndex ) noexcept
		{
			Ic3DebugAssert( pIndex < rows_num );
			return static_cast<row_vector *>( &row0 )[pIndex];
		}

		CPPX_ATTR_NO_DISCARD const vector<TPValue, tpColumns> & operator[]( size_t pIndex ) const noexcept
		{
			Ic3DebugAssert( pIndex < rows_num );
			return static_cast<const row_vector *>( &row0 )[pIndex];
		}

		CPPX_ATTR_NO_DISCARD column_vector column( size_t pColumnIndex ) const noexcept
		{
			return { row0[pColumnIndex], row1[pColumnIndex], row2[pColumnIndex] };
		}

		CPPX_ATTR_NO_DISCARD TPValue * data() noexcept
		{
			return &( values[0] );
		}

		CPPX_ATTR_NO_DISCARD const TPValue * data() const noexcept
		{
			return &( values[0] );
		}

		CPPX_ATTR_NO_DISCARD row_vector * row_ptr( size_t pIndex = 0 ) noexcept
		{
			Ic3DebugAssert( pIndex < rows_num );
			return static_cast<row_vector *>( &row0 ) + pIndex;
		}

		CPPX_ATTR_NO_DISCARD const row_vector * row_ptr( size_t pIndex = 0 ) const noexcept
		{
			Ic3DebugAssert( pIndex < rows_num );
			return static_cast<const row_vector *>( &row0 ) + pIndex;
		}

		CPPX_ATTR_NO_DISCARD row_simd_data_type * simd_ptr( size_t pIndex = 0 ) noexcept
		{
			Ic3DebugAssert( pIndex < rows_num );
			return static_cast<row_simd_data_type *>( &mm0 ) + pIndex;
		}

		CPPX_ATTR_NO_DISCARD const row_simd_data_type * simd_ptr( size_t pIndex = 0 ) const noexcept
		{
			Ic3DebugAssert( pIndex < rows_num );
			return static_cast<const row_simd_data_type *>( &mm0 ) + pIndex;
		}
	};


	template <typename TPValue, size_t tpColumns>
	struct matrix<TPValue, 4, tpColumns>
	{
	public:
		static constexpr size_t rows_num = 4;
		static constexpr size_t columns_num = tpColumns;
		static constexpr size_t matrix_size = rows_num * tpColumns;
		static constexpr size_t sAlignment = alignof( TPValue );
		
		using row_simd_data_type = typename matrix_row_simd_data<TPValue, rows_num, tpColumns>::type;

		using traits_type = matrix_traits<TPValue, rows_num, tpColumns>;
		using column_vector = vector<TPValue, rows_num>;
		using row_vector = vector<TPValue, tpColumns>;

		union
		{
			struct
			{
				row_simd_data_type mm0, mm1, mm2, mm3;
			};

			struct
			{
				row_vector row0, row1, row2, row3;
			};

			struct
			{
				TPValue values[matrix_size];
			};
		};

	public:
		constexpr matrix( const matrix & ) = default;
		constexpr matrix & operator=( const matrix & ) = default;

		constexpr matrix() noexcept = default;

		template <typename T0, typename T1, typename T2, typename T3>
		constexpr matrix(
			const vector<T0, tpColumns> & pRow0,
			const vector<T1, tpColumns> & pRow1,
			const vector<T2, tpColumns> & pRow2,
			const vector<T3, tpColumns> & pRow3 ) noexcept
		: row0( pRow0 )
		, row1( pRow1 )
		, row2( pRow2 )
		, row3( pRow3 )
		{}

		template <typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		constexpr explicit matrix( TPScalar pScalar ) noexcept
		: row0( pScalar )
		, row1( pScalar )
		, row2( pScalar )
		, row3( pScalar )
		{}

		matrix( std::initializer_list<TPValue> pData ) noexcept
		: row0( pData.begin() )
		, row1( pData.begin() + tpColumns )
		, row2( pData.begin() + 2*tpColumns )
		, row3( pData.begin() + 3*tpColumns )
		{
			Ic3DebugAssert( pData.size() == matrix_size );
		}

		CPPX_ATTR_NO_DISCARD vector<TPValue, tpColumns> & operator[]( size_t pIndex ) noexcept
		{
			Ic3DebugAssert( pIndex < rows_num );
			return static_cast<row_vector *>( &row0 )[pIndex];
		}

		CPPX_ATTR_NO_DISCARD const vector<TPValue, tpColumns> & operator[]( size_t pIndex ) const noexcept
		{
			Ic3DebugAssert( pIndex < rows_num );
			return static_cast<const row_vector *>( &row0 )[pIndex];
		}

		CPPX_ATTR_NO_DISCARD column_vector column( size_t pColumnIndex ) const noexcept
		{
			return { row0[pColumnIndex], row1[pColumnIndex], row2[pColumnIndex], row3[pColumnIndex] };
		}

		CPPX_ATTR_NO_DISCARD matrix<TPValue, 2, tpColumns> asMat2() const noexcept
		{

		}

		CPPX_ATTR_NO_DISCARD TPValue * data() noexcept
		{
			return &( values[0] );
		}

		CPPX_ATTR_NO_DISCARD const TPValue * data() const noexcept
		{
			return &( values[0] );
		}

		CPPX_ATTR_NO_DISCARD row_vector * row_ptr( size_t pIndex = 0 ) noexcept
		{
			Ic3DebugAssert( pIndex < rows_num );
			return static_cast<row_vector *>( &row0 ) + pIndex;
		}

		CPPX_ATTR_NO_DISCARD const row_vector * row_ptr( size_t pIndex = 0 ) const noexcept
		{
			Ic3DebugAssert( pIndex < rows_num );
			return static_cast<const row_vector *>( &row0 ) + pIndex;
		}

		CPPX_ATTR_NO_DISCARD row_simd_data_type * simd_ptr( size_t pIndex = 0 ) noexcept
		{
			Ic3DebugAssert( pIndex < rows_num );
			return static_cast<row_simd_data_type *>( &mm0 ) + pIndex;
		}

		CPPX_ATTR_NO_DISCARD const row_simd_data_type * simd_ptr( size_t pIndex = 0 ) const noexcept
		{
			Ic3DebugAssert( pIndex < rows_num );
			return static_cast<const row_simd_data_type *>( &mm0 ) + pIndex;
		}
	};

	template <typename TPValue, size_t tpColumns>
	using matrix2xC = matrix<TPValue, 2, tpColumns>;

	template <typename TPValue, size_t tpColumns>
	using matrix3xC = matrix<TPValue, 3, tpColumns>;

	template <typename TPValue, size_t tpColumns>
	using matrix4xC = matrix<TPValue, 4, tpColumns>;

	template <typename TPValue, size_t tpRows>
	using matrixRx2 = matrix<TPValue, tpRows, 2>;

	template <typename TPValue, size_t tpRows>
	using matrixRx3 = matrix<TPValue, tpRows, 3>;

	template <typename TPValue, size_t tpRows>
	using matrixRx4 = matrix<TPValue, tpRows, 4>;

	template <typename TPValue>
	using mat2x2 = matrix<TPValue, 2, 2>;

	template <typename TPValue>
	using mat3x3 = matrix<TPValue, 3, 3>;

	template <typename TPValue>
	using mat4x4 = matrix<TPValue, 4, 4>;

	template <typename TPValue>
	using matrix2x2 = matrix<TPValue, 2, 2>;

	template <typename TPValue>
	using matrix2x3 = matrix<TPValue, 2, 3>;

	template <typename TPValue>
	using matrix2x4 = matrix<TPValue, 2, 4>;

	template <typename TPValue>
	using matrix3x2 = matrix<TPValue, 3, 2>;

	template <typename TPValue>
	using matrix3x3 = matrix<TPValue, 3, 3>;

	template <typename TPValue>
	using matrix3x4 = matrix<TPValue, 3, 4>;

	template <typename TPValue>
	using matrix4x2 = matrix<TPValue, 4, 2>;

	template <typename TPValue>
	using matrix4x3 = matrix<TPValue, 4, 3>;

	template <typename TPValue>
	using matrix4x4 = matrix<TPValue, 4, 4>;

	using mat2x2i   = matrix2x2<int32>;
	using mat2x2u   = matrix2x2<uint32>;
	using mat2x2i64 = matrix2x2<int64>;
	using mat2x2u64 = matrix2x2<uint64>;
	using mat2x2f   = matrix2x2<float>;
	using mat2x2d   = matrix2x2<double>;

	using mat3x3i   = matrix3x3<int32>;
	using mat3x3u   = matrix3x3<uint32>;
	using mat3x3i64 = matrix3x3<int64>;
	using mat3x3u64 = matrix3x3<uint64>;
	using mat3x3f   = matrix3x3<float>;
	using mat3x3d   = matrix3x3<double>;

	using mat4x4i   = matrix4x4<int32>;
	using mat4x4u   = matrix4x4<uint32>;
	using mat4x4i64 = matrix4x4<int64>;
	using mat4x4u64 = matrix4x4<uint64>;
	using mat4x4f   = matrix4x4<float>;
	using mat4x4d   = matrix4x4<double>;

	using mat2i   = mat2x2i;
	using mat2u   = mat2x2u;
	using mat2i64 = mat2x2i64;
	using mat2u64 = mat2x2u64;
	using mat2f   = mat2x2f;

	using mat3i   = mat3x3i;
	using mat3u   = mat3x3u;
	using mat3i64 = mat3x3i64;
	using mat3u64 = mat3x3u64;
	using mat3f   = mat3x3f;

	using mat4i   = mat4x4i;
	using mat4u   = mat4x4u;
	using mat4i64 = mat4x4i64;
	using mat4u64 = mat4x4u64;
	using mat4f   = mat4x4f;


	template <typename TPValue>
	inline matrix3x3<TPValue> matrix_trim3( const matrix4x4<TPValue> & pMatrix )
	{
		return matrix3x3<TPValue> {
			pMatrix[0][0], pMatrix[0][1], pMatrix[0][2],
			pMatrix[1][0], pMatrix[1][1], pMatrix[1][2],
			pMatrix[2][0], pMatrix[2][1], pMatrix[2][2],
		};
	}

} // namespace cxm

#if( PCL_COMPILER & PCL_COMPILER_CLANG )
#  pragma clang diagnostic pop
#elif( PCL_COMPILER & PCL_COMPILER_GCC )
#  pragma GCC diagnostic pop
#elif( PCL_COMPILER & PCL_COMPILER_MSVC )
#  pragma warning( pop )
#endif

#endif // __CXM_MATRIX_BASE_H__
