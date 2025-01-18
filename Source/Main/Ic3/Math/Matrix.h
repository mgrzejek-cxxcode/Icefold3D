
#ifndef __IC3_MATH_MATRIX_BASE_H__
#define __IC3_MATH_MATRIX_BASE_H__

#include "Vector.h"

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

	// Traits container struct for common properties of matrix types.
	template <typename TPValue, size_t tpRows, size_t tpColumns>
	struct MatrixTraits
	{
		static const uint32 sRowLength = tpColumns;
		static const uint32 sRowsNum = tpRows;
		static const uint32 sColumnsNumNum = tpColumns;
		static const uint32 sLength = sRowsNum * sColumnsNumNum;
		static const uint32 sByteSize = sizeof( TPValue ) * sLength;
		static const uint32 sRowBytePitch = sizeof( TPValue ) * sRowLength;
	};

	template <typename TPValue, size_t /* tpRows */, size_t tpColumns>
	struct MatrixRowSIMDData
	{
		using Type = TPValue[tpColumns];
	};

	template <typename TPValue, size_t tpRows>
	struct MatrixRowSIMDData<TPValue, tpRows, 4>
	{
		using Type = typename Vector4SIMDData<TPValue>::Type;
	};

	// Common class for all matrix types. The default implementation is empty, because we need
	// more fine-grained control over members definition.
	// Internally, matrix values are represented as arrays of rows (similar to DXM library),
	// so there is a specialized definition for MatrixBase<T, 2, R>, MatrixBase<T, 3, R> and
	// MatrixBase<T, 4, R> for 2xR, 3xR and 4xR matrices of type T, respectively.
	template <typename TPValue, size_t tpRows, size_t tpColumns>
	struct Matrix;


	template <typename TPValue, size_t tpColumns>
	struct Matrix<TPValue, 2, tpColumns>
	{
	public:
		static constexpr size_t sRowsNum = 2;
		static constexpr size_t sColumnsNum = tpColumns;
		static constexpr size_t sMatrixSize = sRowsNum * sColumnsNum;

		using Traits = MatrixTraits<TPValue, 2, tpColumns>;
		using ColumnVector = Vector2<TPValue>;
		using RowVector = Vector<TPValue, tpColumns>;
		using RowSIMDDataType = typename MatrixRowSIMDData<TPValue, sRowsNum, tpColumns>::Type;

		union
		{
			struct
			{
				RowSIMDDataType mm0, mm1;
			};

			struct
			{
				RowVector row0, row1;
			};

			struct
			{
				TPValue values[sMatrixSize];
			};
		};

	public:
		constexpr Matrix( const Matrix & ) = default;
		constexpr Matrix & operator=( const Matrix & ) = default;

		constexpr Matrix() noexcept = default;

		template <typename T0, typename T1>
		constexpr Matrix( const Vector<T0, tpColumns> & pRow0,
						  const Vector<T1, tpColumns> & pRow1 ) noexcept
		: row0( pRow0 )
		, row1( pRow1 )
		{}

		template <typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		constexpr explicit Matrix( TPScalar pScalar ) noexcept
		: row0( pScalar )
		, row1( pScalar )
		{}

		Matrix( std::initializer_list<TPValue> pData ) noexcept
		: row0( pData.begin() )
		, row1( pData.begin() + tpColumns )
		{
			Ic3DebugAssert( pData.size() == sMatrixSize );
		}

		CPPX_ATTR_NO_DISCARD Vector<TPValue, tpColumns> & operator[]( size_t pIndex ) noexcept
		{
			Ic3DebugAssert( pIndex < sRowsNum );
			return static_cast<RowVector *>( &row0 )[pIndex];
		}

		CPPX_ATTR_NO_DISCARD const Vector<TPValue, tpColumns> & operator[]( size_t pIndex ) const noexcept
		{
			Ic3DebugAssert( pIndex < sRowsNum );
			return static_cast<const RowVector *>( &row0 )[pIndex];
		}

		CPPX_ATTR_NO_DISCARD ColumnVector column( size_t pColumnIndex ) const noexcept
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

		CPPX_ATTR_NO_DISCARD RowVector * rowPtr( size_t pIndex = 0 ) noexcept
		{
			Ic3DebugAssert( pIndex < sRowsNum );
			return static_cast<RowVector *>( &row0 ) + pIndex;
		}

		CPPX_ATTR_NO_DISCARD const RowVector * rowPtr( size_t pIndex = 0 ) const noexcept
		{
			Ic3DebugAssert( pIndex < sRowsNum );
			return static_cast<const RowVector *>( &row0 ) + pIndex;
		}

		CPPX_ATTR_NO_DISCARD RowSIMDDataType * simdPtr( size_t pIndex = 0 ) noexcept
		{
			Ic3DebugAssert( pIndex < sRowsNum );
			return static_cast<RowSIMDDataType *>( &mm0 ) + pIndex;
		}

		CPPX_ATTR_NO_DISCARD const RowSIMDDataType * simdPtr( size_t pIndex = 0 ) const noexcept
		{
			Ic3DebugAssert( pIndex < sRowsNum );
			return static_cast<const RowSIMDDataType *>( &mm0 ) + pIndex;
		}
	};


	template <typename TPValue, size_t tpColumns>
	struct Matrix<TPValue, 3, tpColumns>
	{
	public:
		static constexpr size_t sRowsNum = 3;
		static constexpr size_t sColumnsNum = tpColumns;
		static constexpr size_t sMatrixSize = sRowsNum * sColumnsNum;
		using RowSIMDDataType = typename MatrixRowSIMDData<TPValue, sRowsNum, tpColumns>::Type;

		using Traits = MatrixTraits<TPValue, 3, tpColumns>;
		using ColumnVector = Vector<TPValue, 3>;
		using RowVector = Vector<TPValue, tpColumns>;

		union
		{
			struct
			{
				RowSIMDDataType mm0, mm1, mm2;
			};

			struct
			{
				RowVector row0, row1, row2;
			};

			struct
			{
				TPValue values[sMatrixSize];
			};
		};

	public:
		constexpr Matrix( const Matrix & ) = default;
		constexpr Matrix & operator=( const Matrix & ) = default;

		constexpr Matrix() noexcept = default;

		template <typename T0, typename T1, typename T2>
		constexpr Matrix( const Vector<T0, tpColumns> & pRow0,
						  const Vector<T1, tpColumns> & pRow1,
						  const Vector<T2, tpColumns> & pRow2 ) noexcept
		: row0( pRow0 )
		, row1( pRow1 )
		, row2( pRow2 )
		{}

		template <typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		constexpr explicit Matrix( TPScalar pScalar ) noexcept
		: row0( pScalar )
		, row1( pScalar )
		, row2( pScalar )
		{}

		Matrix( std::initializer_list<TPValue> pData ) noexcept
		: row0( pData.begin() )
		, row1( pData.begin() + tpColumns )
		, row2( pData.begin() + 2*tpColumns )
		{
			Ic3DebugAssert( pData.size() == sMatrixSize );
		}

		CPPX_ATTR_NO_DISCARD Vector<TPValue, tpColumns> & operator[]( size_t pIndex ) noexcept
		{
			Ic3DebugAssert( pIndex < sRowsNum );
			return static_cast<RowVector *>( &row0 )[pIndex];
		}

		CPPX_ATTR_NO_DISCARD const Vector<TPValue, tpColumns> & operator[]( size_t pIndex ) const noexcept
		{
			Ic3DebugAssert( pIndex < sRowsNum );
			return static_cast<const RowVector *>( &row0 )[pIndex];
		}

		CPPX_ATTR_NO_DISCARD ColumnVector column( size_t pColumnIndex ) const noexcept
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

		CPPX_ATTR_NO_DISCARD RowVector * rowPtr( size_t pIndex = 0 ) noexcept
		{
			Ic3DebugAssert( pIndex < sRowsNum );
			return static_cast<RowVector *>( &row0 ) + pIndex;
		}

		CPPX_ATTR_NO_DISCARD const RowVector * rowPtr( size_t pIndex = 0 ) const noexcept
		{
			Ic3DebugAssert( pIndex < sRowsNum );
			return static_cast<const RowVector *>( &row0 ) + pIndex;
		}

		CPPX_ATTR_NO_DISCARD RowSIMDDataType * simdPtr( size_t pIndex = 0 ) noexcept
		{
			Ic3DebugAssert( pIndex < sRowsNum );
			return static_cast<RowSIMDDataType *>( &mm0 ) + pIndex;
		}

		CPPX_ATTR_NO_DISCARD const RowSIMDDataType * simdPtr( size_t pIndex = 0 ) const noexcept
		{
			Ic3DebugAssert( pIndex < sRowsNum );
			return static_cast<const RowSIMDDataType *>( &mm0 ) + pIndex;
		}
	};


	template <typename TPValue, size_t tpColumns>
	struct Matrix<TPValue, 4, tpColumns>
	{
	public:
		static constexpr size_t sRowsNum = 4;
		static constexpr size_t sColumnsNum = tpColumns;
		static constexpr size_t sMatrixSize = sRowsNum * tpColumns;
		static constexpr size_t sAlignment = alignof( TPValue );
		
		using RowSIMDDataType = typename MatrixRowSIMDData<TPValue, sRowsNum, tpColumns>::Type;

		using Traits = MatrixTraits<TPValue, sRowsNum, tpColumns>;
		using ColumnVector = Vector<TPValue, sRowsNum>;
		using RowVector = Vector<TPValue, tpColumns>;

		union
		{
			struct
			{
				RowSIMDDataType mm0, mm1, mm2, mm3;
			};

			struct
			{
				RowVector row0, row1, row2, row3;
			};

			struct
			{
				TPValue values[sMatrixSize];
			};
		};

	public:
		constexpr Matrix( const Matrix & ) = default;
		constexpr Matrix & operator=( const Matrix & ) = default;

		constexpr Matrix() noexcept = default;

		template <typename T0, typename T1, typename T2, typename T3>
		constexpr Matrix( const Vector<T0, tpColumns> & pRow0,
						  const Vector<T1, tpColumns> & pRow1,
						  const Vector<T2, tpColumns> & pRow2,
						  const Vector<T3, tpColumns> & pRow3 ) noexcept
		: row0( pRow0 )
		, row1( pRow1 )
		, row2( pRow2 )
		, row3( pRow3 )
		{}

		template <typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		constexpr explicit Matrix( TPScalar pScalar ) noexcept
		: row0( pScalar )
		, row1( pScalar )
		, row2( pScalar )
		, row3( pScalar )
		{}

		Matrix( std::initializer_list<TPValue> pData ) noexcept
		: row0( pData.begin() )
		, row1( pData.begin() + tpColumns )
		, row2( pData.begin() + 2*tpColumns )
		, row3( pData.begin() + 3*tpColumns )
		{
			Ic3DebugAssert( pData.size() == sMatrixSize );
		}

		CPPX_ATTR_NO_DISCARD Vector<TPValue, tpColumns> & operator[]( size_t pIndex ) noexcept
		{
			Ic3DebugAssert( pIndex < sRowsNum );
			return static_cast<RowVector *>( &row0 )[pIndex];
		}

		CPPX_ATTR_NO_DISCARD const Vector<TPValue, tpColumns> & operator[]( size_t pIndex ) const noexcept
		{
			Ic3DebugAssert( pIndex < sRowsNum );
			return static_cast<const RowVector *>( &row0 )[pIndex];
		}

		CPPX_ATTR_NO_DISCARD ColumnVector column( size_t pColumnIndex ) const noexcept
		{
			return { row0[pColumnIndex], row1[pColumnIndex], row2[pColumnIndex], row3[pColumnIndex] };
		}

		CPPX_ATTR_NO_DISCARD Matrix<TPValue, 2, tpColumns> asMat2() const noexcept
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

		CPPX_ATTR_NO_DISCARD RowVector * rowPtr( size_t pIndex = 0 ) noexcept
		{
			Ic3DebugAssert( pIndex < sRowsNum );
			return static_cast<RowVector *>( &row0 ) + pIndex;
		}

		CPPX_ATTR_NO_DISCARD const RowVector * rowPtr( size_t pIndex = 0 ) const noexcept
		{
			Ic3DebugAssert( pIndex < sRowsNum );
			return static_cast<const RowVector *>( &row0 ) + pIndex;
		}

		CPPX_ATTR_NO_DISCARD RowSIMDDataType * simdPtr( size_t pIndex = 0 ) noexcept
		{
			Ic3DebugAssert( pIndex < sRowsNum );
			return static_cast<RowSIMDDataType *>( &mm0 ) + pIndex;
		}

		CPPX_ATTR_NO_DISCARD const RowSIMDDataType * simdPtr( size_t pIndex = 0 ) const noexcept
		{
			Ic3DebugAssert( pIndex < sRowsNum );
			return static_cast<const RowSIMDDataType *>( &mm0 ) + pIndex;
		}
	};

	template <typename TPValue, size_t tpColumns>
	using Matrix2xC = Matrix<TPValue, 2, tpColumns>;

	template <typename TPValue, size_t tpColumns>
	using Matrix3xC = Matrix<TPValue, 3, tpColumns>;

	template <typename TPValue, size_t tpColumns>
	using Matrix4xC = Matrix<TPValue, 4, tpColumns>;

	template <typename TPValue, size_t tpRows>
	using MatrixRx2 = Matrix<TPValue, tpRows, 2>;

	template <typename TPValue, size_t tpRows>
	using MatrixRx3 = Matrix<TPValue, tpRows, 3>;

	template <typename TPValue, size_t tpRows>
	using MatrixRx4 = Matrix<TPValue, tpRows, 4>;

	template <typename TPValue>
	using Mat2x2 = Matrix<TPValue, 2, 2>;

	template <typename TPValue>
	using Mat3x3 = Matrix<TPValue, 3, 3>;

	template <typename TPValue>
	using Mat4x4 = Matrix<TPValue, 4, 4>;

	template <typename TPValue>
	using Matrix2x2 = Matrix<TPValue, 2, 2>;

	template <typename TPValue>
	using Matrix2x3 = Matrix<TPValue, 2, 3>;

	template <typename TPValue>
	using Matrix2x4 = Matrix<TPValue, 2, 4>;

	template <typename TPValue>
	using Matrix3x2 = Matrix<TPValue, 3, 2>;

	template <typename TPValue>
	using Matrix3x3 = Matrix<TPValue, 3, 3>;

	template <typename TPValue>
	using Matrix3x4 = Matrix<TPValue, 3, 4>;

	template <typename TPValue>
	using Matrix4x2 = Matrix<TPValue, 4, 2>;

	template <typename TPValue>
	using Matrix4x3 = Matrix<TPValue, 4, 3>;

	template <typename TPValue>
	using Matrix4x4 = Matrix<TPValue, 4, 4>;

	using Mat2x2i   = Matrix2x2<int32>;
	using Mat2x2u   = Matrix2x2<uint32>;
	using Mat2x2i64 = Matrix2x2<int64>;
	using Mat2x2u64 = Matrix2x2<uint64>;
	using Mat2x2f   = Matrix2x2<float>;
	using Mat2x2d   = Matrix2x2<double>;

	using Mat3x3i   = Matrix3x3<int32>;
	using Mat3x3u   = Matrix3x3<uint32>;
	using Mat3x3i64 = Matrix3x3<int64>;
	using Mat3x3u64 = Matrix3x3<uint64>;
	using Mat3x3f   = Matrix3x3<float>;
	using Mat3x3d   = Matrix3x3<double>;

	using Mat4x4i   = Matrix4x4<int32>;
	using Mat4x4u   = Matrix4x4<uint32>;
	using Mat4x4i64 = Matrix4x4<int64>;
	using Mat4x4u64 = Matrix4x4<uint64>;
	using Mat4x4f   = Matrix4x4<float>;
	using Mat4x4d   = Matrix4x4<double>;

	using Mat2i   = Mat2x2i;
	using Mat2u   = Mat2x2u;
	using Mat2i64 = Mat2x2i64;
	using Mat2u64 = Mat2x2u64;
	using Mat2f   = Mat2x2f;

	using Mat3i   = Mat3x3i;
	using Mat3u   = Mat3x3u;
	using Mat3i64 = Mat3x3i64;
	using Mat3u64 = Mat3x3u64;
	using Mat3f   = Mat3x3f;

	using Mat4i   = Mat4x4i;
	using Mat4u   = Mat4x4u;
	using Mat4i64 = Mat4x4i64;
	using Mat4u64 = Mat4x4u64;
	using Mat4f   = Mat4x4f;


	template <typename TPValue>
	inline Matrix3x3<TPValue> matrixTrim3( const Matrix4x4<TPValue> & pMatrix )
	{
		return Matrix3x3<TPValue> {
			pMatrix[0][0], pMatrix[0][1], pMatrix[0][2],
			pMatrix[1][0], pMatrix[1][1], pMatrix[1][2],
			pMatrix[2][0], pMatrix[2][1], pMatrix[2][2],
		};
	}

} // namespace Ic3::Math

#if( PCL_COMPILER & PCL_COMPILER_CLANG )
#  pragma clang diagnostic pop
#elif( PCL_COMPILER & PCL_COMPILER_GCC )
#  pragma GCC diagnostic pop
#elif( PCL_COMPILER & PCL_COMPILER_MSVC )
#  pragma warning( pop )
#endif

#endif // __IC3_MATH_MATRIX_BASE_H__
