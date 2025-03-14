
#ifndef __CXM_MATRIX_OPS_H__
#define __CXM_MATRIX_OPS_H__

#include "matrix.h"
#include "vectorOps.h"

namespace cxm
{

	template <typename TPValue, size_t tpC>
	inline void add( const matrix<TPValue, 2, tpC> & pFirst, const matrix<TPValue, 2, tpC> & pSecond, matrix<TPValue, 2, tpC> & pResult )
	{
		pResult.row0 = pFirst.row0 + pSecond.row0;
		pResult.row1 = pFirst.row1 + pSecond.row1;
	}

	template <typename TPValue, size_t tpC>
	inline void add( const matrix<TPValue, 3, tpC> & pFirst, const matrix<TPValue, 3, tpC> & pSecond, matrix<TPValue, 3, tpC> & pResult )
	{
		pResult.row0 = pFirst.row0 + pSecond.row0;
		pResult.row1 = pFirst.row1 + pSecond.row1;
		pResult.row2 = pFirst.row2 + pSecond.row2;
	}

	template <typename TPValue, size_t tpC>
	inline void add( const matrix<TPValue, 4, tpC> & pFirst, const matrix<TPValue, 4, tpC> & pSecond, matrix<TPValue, 4, tpC> & pResult )
	{
		pResult.row0 = pFirst.row0 + pSecond.row0;
		pResult.row1 = pFirst.row1 + pSecond.row1;
		pResult.row2 = pFirst.row2 + pSecond.row2;
		pResult.row3 = pFirst.row3 + pSecond.row3;
	}

	template <typename TMatrix>
	inline TMatrix add( const TMatrix & pFirst, const TMatrix & pSecond )
	{
		TMatrix result;
		add( pFirst, pSecond, result );
		return result;
	}

	template <typename TPValue, size_t tpC>
	inline void sub( const matrix<TPValue, 2, tpC> & pFirst, const matrix<TPValue, 2, tpC> & pSecond, matrix<TPValue, 2, tpC> & pResult )
	{
		pResult.row0 = pFirst.row0 - pSecond.row0;
		pResult.row1 = pFirst.row1 - pSecond.row1;
	}

	template <typename TPValue, size_t tpC>
	inline void sub( const matrix<TPValue, 3, tpC> & pFirst, const matrix<TPValue, 3, tpC> & pSecond, matrix<TPValue, 3, tpC> & pResult )
	{
		pResult.row0 = pFirst.row0 - pSecond.row0;
		pResult.row1 = pFirst.row1 - pSecond.row1;
		pResult.row2 = pFirst.row2 - pSecond.row2;
	}

	template <typename TPValue, size_t tpC>
	inline void sub( const matrix<TPValue, 4, tpC> & pFirst, const matrix<TPValue, 4, tpC> & pSecond, matrix<TPValue, 4, tpC> & pResult )
	{
		pResult.row0 = pFirst.row0 - pSecond.row0;
		pResult.row1 = pFirst.row1 - pSecond.row1;
		pResult.row2 = pFirst.row2 - pSecond.row2;
		pResult.row3 = pFirst.row3 - pSecond.row3;
	}

	template <typename TMatrix>
	inline TMatrix sub( const TMatrix & pFirst, const TMatrix & pSecond )
	{
		TMatrix result;
		sub( pFirst, pSecond, result );
		return result;
	}

	template <typename TPValue, size_t tpR, size_t tpN, size_t tpC>
	inline void mul( const matrix<TPValue, tpR, tpN> & pFirst, const matrix<TPValue, tpN, tpC> & pSecond, matrix<TPValue, tpR, tpC> & pResult )
	{
		for( size_t fRowIndex = 0; fRowIndex < tpR; ++fRowIndex )
		{
			for( size_t sColumnIndex = 0; sColumnIndex < tpC; ++sColumnIndex )
			{
				TPValue currentElemValue = static_cast<TPValue>( 0 );
				for( size_t elemIndex = 0; elemIndex < tpN; ++elemIndex )
				{
					currentElemValue += ( pFirst[fRowIndex][elemIndex] * pSecond[elemIndex][sColumnIndex] );
				}
				pResult[fRowIndex][sColumnIndex] = currentElemValue;
			}
		}
	}

	template <typename TPValue>
	inline void mul( const matrix2x2<TPValue> & pFirst, const matrix2x2<TPValue> & pSecond, matrix2x2<TPValue> & pResult )
	{
		const auto firstE00MulSecondR0 = pFirst.row0[0] * pSecond.row0;
		const auto firstE01MulSecondR1 = pFirst.row0[1] * pSecond.row1;
		pResult.row0 = firstE00MulSecondR0 + firstE01MulSecondR1;

		const auto firstE10MulSecondR0 = pFirst.row1[0] * pSecond.row0;
		const auto firstE11MulSecondR1 = pFirst.row1[1] * pSecond.row1;
		pResult.row1 = firstE10MulSecondR0 + firstE11MulSecondR1;
	}

	template <typename TPValue>
	inline void mul( const matrix3x3<TPValue> & pFirst, const matrix3x3<TPValue> & pSecond, matrix3x3<TPValue> & pResult )
	{
		const auto firstE00MulSecondR0 = pFirst.row0[0] * pSecond.row0;
		const auto firstE01MulSecondR1 = pFirst.row0[1] * pSecond.row1;
		const auto firstE02MulSecondR2 = pFirst.row0[2] * pSecond.row2;
		pResult.row0 = firstE00MulSecondR0 + firstE01MulSecondR1 + firstE02MulSecondR2;

		const auto firstE10MulSecondR0 = pFirst.row1[0] * pSecond.row0;
		const auto firstE11MulSecondR1 = pFirst.row1[1] * pSecond.row1;
		const auto firstE12MulSecondR2 = pFirst.row1[2] * pSecond.row2;
		pResult.row1 = firstE10MulSecondR0 + firstE11MulSecondR1 + firstE12MulSecondR2;

		const auto firstE20MulSecondR0 = pFirst.row2[0] * pSecond.row0;
		const auto firstE21MulSecondR1 = pFirst.row2[1] * pSecond.row1;
		const auto firstE22MulSecondR2 = pFirst.row2[2] * pSecond.row2;
		pResult.row2 = firstE20MulSecondR0 + firstE21MulSecondR1 + firstE22MulSecondR2;
	}

	template <typename TPValue>
	inline void mul( const matrix4x4<TPValue> & pFirst, const matrix4x4<TPValue> & pSecond, matrix4x4<TPValue> & pResult )
	{
		const auto firstE00MulSecondR0 = pFirst.row0[0] * pSecond.row0;
		const auto firstE01MulSecondR1 = pFirst.row0[1] * pSecond.row1;
		const auto firstE02MulSecondR2 = pFirst.row0[2] * pSecond.row2;
		const auto firstE03MulSecondR3 = pFirst.row0[3] * pSecond.row3;
		pResult.row0 = firstE00MulSecondR0 + firstE01MulSecondR1 + firstE02MulSecondR2 + firstE03MulSecondR3;

		const auto firstE10MulSecondR0 = pFirst.row1[0] * pSecond.row0;
		const auto firstE11MulSecondR1 = pFirst.row1[1] * pSecond.row1;
		const auto firstE12MulSecondR2 = pFirst.row1[2] * pSecond.row2;
		const auto firstE13MulSecondR3 = pFirst.row1[3] * pSecond.row3;
		pResult.row1 = firstE10MulSecondR0 + firstE11MulSecondR1 + firstE12MulSecondR2 + firstE13MulSecondR3;

		const auto firstE20MulSecondR0 = pFirst.row2[0] * pSecond.row0;
		const auto firstE21MulSecondR1 = pFirst.row2[1] * pSecond.row1;
		const auto firstE22MulSecondR2 = pFirst.row2[2] * pSecond.row2;
		const auto firstE23MulSecondR3 = pFirst.row2[3] * pSecond.row3;
		pResult.row2 = firstE20MulSecondR0 + firstE21MulSecondR1 + firstE22MulSecondR2 + firstE23MulSecondR3;

		const auto firstE30MulSecondR0 = pFirst.row3[0] * pSecond.row0;
		const auto firstE31MulSecondR1 = pFirst.row3[1] * pSecond.row1;
		const auto firstE32MulSecondR2 = pFirst.row3[2] * pSecond.row2;
		const auto firstE33MulSecondR3 = pFirst.row3[3] * pSecond.row3;
		pResult.row3 = firstE30MulSecondR0 + firstE31MulSecondR1 + firstE32MulSecondR2 + firstE33MulSecondR3;
	}

	template <typename TPValue>
	inline void mul( const matrix2x2<TPValue> & pMatrix, const vector2<TPValue> & pVector, vector2<TPValue> & pResult )
	{
		pResult[0] = pMatrix.row0 * pVector;
		pResult[1] = pMatrix.row1 * pVector;
	}

	template <typename TPValue>
	inline void mul( const vector2<TPValue> & pVector, const matrix2x2<TPValue> & pMatrix, vector2<TPValue> & pResult )
	{
		pResult[0] = dot( pVector, pMatrix.column( 0 ) );
		pResult[1] = dot( pVector, pMatrix.column( 1 ) );
	}

	template <typename TPValue>
	inline void mul( const matrix3x3<TPValue> & pMatrix, const vector3<TPValue> & pVector, vector3<TPValue> & pResult )
	{
		pResult.x = pMatrix.row0 * pVector;
		pResult.y = pMatrix.row1 * pVector;
		pResult.z = pMatrix.row2 * pVector;
	}

	template <typename TPValue>
	inline void mul( const vector3<TPValue> & pVector, const matrix3x3<TPValue> & pMatrix, vector3<TPValue> & pResult )
	{
		pResult[0] = dot( pVector, pMatrix.column( 0 ) );
		pResult[1] = dot( pVector, pMatrix.column( 1 ) );
		pResult[2] = dot( pVector, pMatrix.column( 2 ) );
	}

	template <typename TPValue>
	inline void mul( const matrix4x4<TPValue> & pMatrix, const vector4<TPValue> & pVector, vector4<TPValue> & pResult )
	{
		pResult.x = dot( pMatrix.row0, pVector );
		pResult.y = dot( pMatrix.row1, pVector );
		pResult.z = dot( pMatrix.row2, pVector );
		pResult.w = dot( pMatrix.row3, pVector );
	}

	template <typename TPValue>
	inline void mul( const vector4<TPValue> & pVector, const matrix4x4<TPValue> & pMatrix, vector4<TPValue> & pResult )
	{
		pResult[0] = dot( pVector, pMatrix.column( 0 ) );
		pResult[1] = dot( pVector, pMatrix.column( 1 ) );
		pResult[2] = dot( pVector, pMatrix.column( 2 ) );
		pResult[3] = dot( pVector, pMatrix.column( 3 ) );
	}

	template <typename TPValue, size_t tpN>
	inline matrix<TPValue, tpN, tpN> mul( const matrix<TPValue, tpN, tpN> & pFirst, const matrix<TPValue, tpN, tpN> & pSecond )
	{
		matrix<TPValue, tpN, tpN> result;
		mul( pFirst, pSecond, result );
		return result;
	}

	template <typename TPValue, size_t tpN>
	inline vector<TPValue, tpN> mul( const matrix<TPValue, tpN, tpN> & pMatrix, const vector<TPValue, tpN> & pVector )
	{
		vector<TPValue, tpN> result;
		mul( pMatrix, pVector, result );
		return result;
	}

	template <typename TPValue, size_t tpN>
	inline vector<TPValue, tpN> mul( const vector<TPValue, tpN> & pVector, const matrix<TPValue, tpN, tpN> & pMatrix )
	{
		vector<TPValue, tpN> result;
		mul( pVector, pMatrix, result );
		return result;
	}

} // namespace cxm

#if( CXM_SIMD_ENABLE )
#  include "matrixOpsSIMD.inl"
#endif

#endif //TESSLINE_3DX_MATRIXOPS_H
