
#ifndef __CXM_ARRAY_OPS_H__
#define __CXM_ARRAY_OPS_H__

#include "utility.h"

namespace cxm
{

	namespace array_ops
	{

		template <typename TPVal>
		inline void set( array2<TPVal> & pDest, TPVal pX, TPVal pY )
		{
			pDest[0] = pX;
			pDest[1] = pY;
		}

		template <typename TPVal>
		inline void set( array2<TPVal> & pDest, TPVal pValue )
		{
			pDest[0] = pValue;
			pDest[1] = pValue;
		}

		template <typename TPVal>
		inline void set( array2<TPVal> & pDest, const TPVal * pArray )
		{
			pDest[0] = pArray[0];
			pDest[1] = pArray[1];
		}

		template <typename TPVal>
		inline void add( const array2<TPVal> & pFirst, const array2<TPVal> & pSecond, array2<TPVal> & pResult )
		{
			pResult[0] = pFirst[0] + pSecond[0];
			pResult[1] = pFirst[1] + pSecond[1];
		}

		template <typename TPVal>
		inline void sub( const array2<TPVal> & pFirst, const array2<TPVal> & pSecond, array2<TPVal> & pResult )
		{
			pResult[0] = pFirst[0] - pSecond[0];
			pResult[1] = pFirst[1] - pSecond[1];
		}

		template <typename TPVal>
		inline void mul( const array2<TPVal> & pFirst, const array2<TPVal> & pSecond, array2<TPVal> & pResult )
		{
			pResult[0] = pFirst[0] * pSecond[0];
			pResult[1] = pFirst[1] * pSecond[1];
		}

		template <typename TPVal, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		inline void mul( const array2<TPVal> & pData, const TPScalar pScalar, array2<TPVal> & pResult )
		{
			pResult[0] = pData[0] * pScalar;
			pResult[1] = pData[1] * pScalar;
		}

		template <typename TPVal>
		inline void div( const array2<TPVal> & pFirst, const array2<TPVal> & pSecond, array2<TPVal> & pResult )
		{
			pResult[0] = pFirst[0] / pSecond[0];
			pResult[1] = pFirst[1] / pSecond[1];
		}

		template <typename TPVal, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		inline void div( const array2<TPVal> & pData, const TPScalar pScalar, array2<TPVal> & pResult )
		{
			pResult[0] = pData[0] / pScalar;
			pResult[1] = pData[1] / pScalar;
		}

		template <typename TPVal, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		inline void div( const TPScalar pScalar, const array2<TPVal> & pData, array2<TPVal> & pResult )
		{
			pResult[0] = pScalar / pData[0];
			pResult[1] = pScalar / pData[1];
		}

		template <typename TPVal>
		inline void neg( const array2<TPVal> & pData, array2<TPVal> & pResult )
		{
			pResult[0] = -pData[0];
			pResult[1] = -pData[1];
		}

		template <typename TPVal>
		inline TPVal dot( const array2<TPVal> & pFirst, const array2<TPVal> & pSecond )
		{
			return pFirst[0] * pSecond[0] + pFirst[1] * pSecond[1];
		}

		template <typename TPVal>
		inline TPVal length( const array2<TPVal> & pData )
		{
			return static_cast<TPVal>( sqrt( dot( pData, pData ) ) );
		}

		template <typename TPVal>
		inline void normalize( const array2<TPVal> & pData, array2<TPVal> & pResult )
		{
			const auto dotInvSqrt = inv_sqrt( dot( pData, pData ) );
			pResult[0] = pData[0] * dotInvSqrt;
			pResult[1] = pData[1] * dotInvSqrt;
		}

		template <typename TPVal>
		inline void normalize_inplace( array2<TPVal> & pData )
		{
			normalize( pData, pData );
		}

		template <typename TPVal>
		inline bool cmp_equal( const array2<TPVal> & pFirst, const array2<TPVal> & pSecond )
		{
			return ( pFirst[0] == pSecond[0] ) && ( pFirst[1] == pSecond[1] );
		}

		template <typename TPVal>
		inline bool cmp_not_equal( const array2<TPVal> & pFirst, const array2<TPVal> & pSecond )
		{
			return ( pFirst[0] != pSecond[0] ) || ( pFirst[1] != pSecond[1] );
		}

		template <typename TPVal>
		inline bool cmp_less( const array2<TPVal> & pFirst, const array2<TPVal> & pSecond )
		{
			return  ( pFirst[0] < pSecond[0] ) || ( ( pFirst[0] == pSecond[0] ) && ( pFirst[1] < pSecond[1] ) );
		}



		template <typename TPVal>
		inline void set( array3<TPVal> & pDest, float pX, float pY, float pZ )
		{
			pDest[0] = pX;
			pDest[1] = pY;
			pDest[2] = pZ;
		}

		template <typename TPVal>
		inline void set( array3<TPVal> & pDest, TPVal pValue )
		{
			pDest[0] = pValue;
			pDest[1] = pValue;
			pDest[2] = pValue;
		}

		template <typename TPVal>
		inline void set( array3<TPVal> & pDest, const float * pArray )
		{
			pDest[0] = pArray[0];
			pDest[1] = pArray[1];
			pDest[2] = pArray[2];
		}

		template <typename TPVal>
		inline void add( const array3<TPVal> & pFirst, const array3<TPVal> & pSecond, array3<TPVal> & pResult )
		{
			pResult[0] = pFirst[0] + pSecond[0];
			pResult[1] = pFirst[1] + pSecond[1];
			pResult[2] = pFirst[2] + pSecond[2];
		}

		template <typename TPVal>
		inline void sub( const array3<TPVal> & pFirst, const array3<TPVal> & pSecond, array3<TPVal> & pResult )
		{
			pResult[0] = pFirst[0] - pSecond[0];
			pResult[1] = pFirst[1] - pSecond[1];
			pResult[2] = pFirst[2] - pSecond[2];
		}

		template <typename TPVal>
		inline void mul( const array3<TPVal> & pFirst, const array3<TPVal> & pSecond, array3<TPVal> & pResult )
		{
			pResult[0] = pFirst[0] * pSecond[0];
			pResult[1] = pFirst[1] * pSecond[1];
			pResult[2] = pFirst[2] * pSecond[2];
		}

		template <typename TPVal, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		inline void mul( const array3<TPVal> & pData, const TPScalar pScalar, array3<TPVal> & pResult )
		{
			pResult[0] = pData[0] * pScalar;
			pResult[1] = pData[1] * pScalar;
			pResult[2] = pData[2] * pScalar;
		}

		template <typename TPVal>
		inline void div( const array3<TPVal> & pFirst, const array3<TPVal> & pSecond, array3<TPVal> & pResult )
		{
			pResult[0] = pFirst[0] / pSecond[0];
			pResult[1] = pFirst[1] / pSecond[1];
			pResult[2] = pFirst[2] / pSecond[2];
		}

		template <typename TPVal, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		inline void div( const array3<TPVal> & pData, const TPScalar pScalar, array3<TPVal> & pResult )
		{
			pResult[0] = pData[0] / pScalar;
			pResult[1] = pData[1] / pScalar;
			pResult[2] = pData[2] / pScalar;
		}

		template <typename TPVal, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		inline void div( const TPScalar pScalar, const array3<TPVal> & pData, array3<TPVal> & pResult )
		{
			pResult[0] = pScalar / pData[0];
			pResult[1] = pScalar / pData[1];
			pResult[2] = pScalar / pData[2];
		}

		template <typename TPVal>
		inline void neg( const array3<TPVal> & pData, array3<TPVal> & pResult )
		{
			pResult[0] = ( -pData[0] );
			pResult[1] = ( -pData[1] );
			pResult[2] = ( -pData[2] );
		}

		template <typename TPVal>
		inline void cross( const array3<TPVal> & pFirst, const array3<TPVal> & pSecond, array3<TPVal> & pResult )
		{
			pResult[0] = pFirst[1] * pSecond[2] - pFirst[2] * pSecond[1];
			pResult[1] = pFirst[2] * pSecond[0] - pFirst[0] * pSecond[2];
			pResult[2] = pFirst[0] * pSecond[1] - pFirst[1] * pSecond[0];
		}

		template <typename TPVal>
		inline TPVal dot( const array3<TPVal> & pFirst, const array3<TPVal> & pSecond )
		{
			return pFirst[0] * pSecond[0] + pFirst[1] * pSecond[1] + pFirst[2] * pSecond[2];
		}

		template <typename TPVal>
		static double length( const array3<TPVal> & pData )
		{
			return sqrt( dot( pData, pData ) );
		}

		template <typename TPVal>
		inline void normalize( const array3<TPVal> & pData, array3<TPVal> & pResult )
		{
			const auto dotInvSqrt = inv_sqrt( dot( pData, pData ) );
			pResult[0] = pData[0] * dotInvSqrt;
			pResult[1] = pData[1] * dotInvSqrt;
			pResult[2] = pData[2] * dotInvSqrt;
		}

		template <typename TPVal>
		inline void normalize_inplace( array3<TPVal> & pData )
		{
			normalize( pData, pData );
		}

		template <typename TPVal>
		inline bool cmp_equal( const array3<TPVal> & pFirst, const array3<TPVal> & pSecond )
		{
			return ( pFirst[0] == pSecond[0] ) && ( pFirst[1] == pSecond[1] ) && ( pFirst[2] == pSecond[2] );
		}

		template <typename TPVal>
		inline bool cmp_not_equal( const array3<TPVal> & pFirst, const array3<TPVal> & pSecond )
		{
			return ( pFirst[0] != pSecond[0] ) || ( pFirst[1] != pSecond[1] ) || ( pFirst[2] != pSecond[2] );
		}

		template <typename TPVal>
		inline bool cmp_less( const array3<TPVal> & pFirst, const array3<TPVal> & pSecond )
		{
			return ( pFirst[0] < pSecond[0] ) ||
			       ( ( pFirst[0] == pSecond[0] ) && ( pFirst[1] < pSecond[1] ) ) ||
			       ( ( pFirst[0] == pSecond[0] ) && ( pFirst[1] == pSecond[1] ) && ( pFirst[2] < pSecond[2] ) );
		}


		template <typename TPVal>
		inline void set( array4<TPVal> & pDest, float pX, float pY, float pZ, float pW )
		{
			pDest[0] = pX;
			pDest[1] = pY;
			pDest[2] = pZ;
			pDest[3] = pW;
		}

		template <typename TPVal>
		inline void set( array4<TPVal> & pDest, TPVal pValue )
		{
			pDest[0] = pValue;
			pDest[1] = pValue;
			pDest[2] = pValue;
			pDest[3] = pValue;
		}

		template <typename TPVal>
		inline void set( array4<TPVal> & pDest, const float * pArray )
		{
			pDest[0] = pArray[0];
			pDest[1] = pArray[1];
			pDest[2] = pArray[2];
			pDest[3] = pArray[3];
		}

		template <typename TPVal>
		inline void add( const array4<TPVal> & pFirst, const array4<TPVal> & pSecond, array4<TPVal> & pResult )
		{
			pResult[0] = pFirst[0] + pSecond[0];
			pResult[1] = pFirst[1] + pSecond[1];
			pResult[2] = pFirst[2] + pSecond[2];
			pResult[3] = pFirst[3] + pSecond[3];
		}

		template <typename TPVal>
		inline void sub( const array4<TPVal> & pFirst, const array4<TPVal> & pSecond, array4<TPVal> & pResult )
		{
			pResult[0] = pFirst[0] - pSecond[0];
			pResult[1] = pFirst[1] - pSecond[1];
			pResult[2] = pFirst[2] - pSecond[2];
			pResult[3] = pFirst[3] - pSecond[3];
		}

		template <typename TPVal>
		inline void mul( const array4<TPVal> & pFirst, const array4<TPVal> & pSecond, array4<TPVal> & pResult )
		{
			pResult[0] = pFirst[0] * pSecond[0];
			pResult[1] = pFirst[1] * pSecond[1];
			pResult[2] = pFirst[2] * pSecond[2];
			pResult[3] = pFirst[3] * pSecond[3];
		}

		template <typename TPVal, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		inline void mul( const array4<TPVal> & pData, const TPScalar pScalar, array4<TPVal> & pResult )
		{
			pResult[0] = pData[0] * pScalar;
			pResult[1] = pData[1] * pScalar;
			pResult[2] = pData[2] * pScalar;
			pResult[3] = pData[3] * pScalar;
		}

		template <typename TPVal>
		inline void div( const array4<TPVal> & pFirst, const array4<TPVal> & pSecond, array4<TPVal> & pResult )
		{
			pResult[0] = pFirst[0] / pSecond[0];
			pResult[1] = pFirst[1] / pSecond[1];
			pResult[2] = pFirst[2] / pSecond[2];
			pResult[3] = pFirst[3] / pSecond[3];
		}

		template <typename TPVal, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		inline void div( const array4<TPVal> & pData, const TPScalar pScalar, array4<TPVal> & pResult )
		{
			pResult[0] = pData[0] / pScalar;
			pResult[1] = pData[1] / pScalar;
			pResult[2] = pData[2] / pScalar;
			pResult[3] = pData[3] / pScalar;
		}

		template <typename TPVal, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		inline void div( const TPScalar pScalar, const array4<TPVal> & pData, array4<TPVal> & pResult )
		{
			pResult[0] = pScalar / pData[0];
			pResult[1] = pScalar / pData[1];
			pResult[2] = pScalar / pData[2];
			pResult[3] = pScalar / pData[3];
		}

		template <typename TPVal>
		inline void neg( const array4<TPVal> & pData, array4<TPVal> & pResult )
		{
			pResult[0] = ( -pData[0] );
			pResult[1] = ( -pData[1] );
			pResult[2] = ( -pData[2] );
			pResult[3] = ( -pData[3] );
		}

		template <typename TPVal>
		inline TPVal dot( const array4<TPVal> & pFirst, const array4<TPVal> & pSecond )
		{
			return pFirst[0] * pSecond[0] + pFirst[1] * pSecond[1] + pFirst[2] * pSecond[2] + pFirst[3] * pSecond[3];
		}

		template <typename TPVal>
		static double length( const array4<TPVal> & pData )
		{
			return sqrt( dot( pData, pData ) );
		}

		template <typename TPVal>
		inline void normalize( const array4<TPVal> & pData, array4<TPVal> & pResult )
		{
			const auto dotInvSqrt = inv_sqrt( dot( pData, pData ) );
			pResult[0] = pData[0] * dotInvSqrt;
			pResult[1] = pData[1] * dotInvSqrt;
			pResult[2] = pData[2] * dotInvSqrt;
			pResult[3] = pData[3] * dotInvSqrt;
		}

		template <typename TPVal>
		inline void normalize_inplace( array4<TPVal> & pData )
		{
			normalize( pData, pData );
		}

		template <typename TPVal>
		inline bool cmp_equal( const array4<TPVal> & pFirst, const array4<TPVal> & pSecond )
		{
			return ( pFirst[0] == pSecond[0] ) && ( pFirst[1] == pSecond[1] ) && ( pFirst[2] == pSecond[2] ) && ( pFirst[3] == pSecond[3] );
		}

		template <typename TPVal>
		inline bool cmp_not_equal( const array4<TPVal> & pFirst, const array4<TPVal> & pSecond )
		{
			return ( pFirst[0] != pSecond[0] ) || ( pFirst[1] != pSecond[1] ) || ( pFirst[2] != pSecond[2] ) || ( pFirst[3] != pSecond[3] );
		}

		template <typename TPVal>
		inline bool cmp_less( const array4<TPVal> & pFirst, const array4<TPVal> & pSecond )
		{
			return ( pFirst[0] < pSecond[0] ) ||
			       ( ( pFirst[0] == pSecond[0] ) && ( pFirst[1] < pSecond[1] ) ) ||
			       ( ( pFirst[0] == pSecond[0] ) && ( pFirst[1] == pSecond[1] ) && ( pFirst[2] < pSecond[2] ) ) ||
			       ( ( pFirst[0] == pSecond[0] ) && ( pFirst[1] == pSecond[1] ) && ( pFirst[2] == pSecond[2] ) && ( pFirst[3] < pSecond[3] ) );
		}

	} // namespace array_ops

} // namespace cxm

#if( CXM_SIMD_ENABLE )
#  include "arrayOpsSIMD.inl"
#endif

#endif // __CXM_ARRAY_OPS_H__
