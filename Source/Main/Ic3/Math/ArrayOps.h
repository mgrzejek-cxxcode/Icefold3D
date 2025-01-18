
#ifndef __IC3_MATH_ARRAY_OPS_H__
#define __IC3_MATH_ARRAY_OPS_H__

#include "Utility.h"

namespace Ic3::Math
{

	namespace ArrayOps
	{

		template <typename TPVal>
		inline void set( Array2<TPVal> & pDest, TPVal pX, TPVal pY )
		{
			pDest[0] = pX;
			pDest[1] = pY;
		}

		template <typename TPVal>
		inline void set( Array2<TPVal> & pDest, TPVal pValue )
		{
			pDest[0] = pValue;
			pDest[1] = pValue;
		}

		template <typename TPVal>
		inline void set( Array2<TPVal> & pDest, const TPVal * pArray )
		{
			pDest[0] = pArray[0];
			pDest[1] = pArray[1];
		}

		template <typename TPVal>
		inline void add( const Array2<TPVal> & pFirst, const Array2<TPVal> & pSecond, Array2<TPVal> & pResult )
		{
			pResult[0] = pFirst[0] + pSecond[0];
			pResult[1] = pFirst[1] + pSecond[1];
		}

		template <typename TPVal>
		inline void sub( const Array2<TPVal> & pFirst, const Array2<TPVal> & pSecond, Array2<TPVal> & pResult )
		{
			pResult[0] = pFirst[0] - pSecond[0];
			pResult[1] = pFirst[1] - pSecond[1];
		}

		template <typename TPVal>
		inline void mul( const Array2<TPVal> & pFirst, const Array2<TPVal> & pSecond, Array2<TPVal> & pResult )
		{
			pResult[0] = pFirst[0] * pSecond[0];
			pResult[1] = pFirst[1] * pSecond[1];
		}

		template <typename TPVal, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		inline void mul( const Array2<TPVal> & pData, const TPScalar pScalar, Array2<TPVal> & pResult )
		{
			pResult[0] = pData[0] * pScalar;
			pResult[1] = pData[1] * pScalar;
		}

		template <typename TPVal>
		inline void div( const Array2<TPVal> & pFirst, const Array2<TPVal> & pSecond, Array2<TPVal> & pResult )
		{
			pResult[0] = pFirst[0] / pSecond[0];
			pResult[1] = pFirst[1] / pSecond[1];
		}

		template <typename TPVal, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		inline void div( const Array2<TPVal> & pData, const TPScalar pScalar, Array2<TPVal> & pResult )
		{
			pResult[0] = pData[0] / pScalar;
			pResult[1] = pData[1] / pScalar;
		}

		template <typename TPVal, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		inline void div( const TPScalar pScalar, const Array2<TPVal> & pData, Array2<TPVal> & pResult )
		{
			pResult[0] = pScalar / pData[0];
			pResult[1] = pScalar / pData[1];
		}

		template <typename TPVal>
		inline void neg( const Array2<TPVal> & pData, Array2<TPVal> & pResult )
		{
			pResult[0] = -pData[0];
			pResult[1] = -pData[1];
		}

		template <typename TPVal>
		inline TPVal dot( const Array2<TPVal> & pFirst, const Array2<TPVal> & pSecond )
		{
			return pFirst[0] * pSecond[0] + pFirst[1] * pSecond[1];
		}

		template <typename TPVal>
		inline TPVal length( const Array2<TPVal> & pData )
		{
			return static_cast<TPVal>( sqrt( dot( pData, pData ) ) );
		}

		template <typename TPVal>
		inline void normalize( const Array2<TPVal> & pData, Array2<TPVal> & pResult )
		{
			const auto dotInvSqrt = invSqrt( dot( pData, pData ) );
			pResult[0] = pData[0] * dotInvSqrt;
			pResult[1] = pData[1] * dotInvSqrt;
		}

		template <typename TPVal>
		inline void normalizeInplace( Array2<TPVal> & pData )
		{
			normalize( pData, pData );
		}

		template <typename TPVal>
		inline bool cmpEqual( const Array2<TPVal> & pFirst, const Array2<TPVal> & pSecond )
		{
			return ( pFirst[0] == pSecond[0] ) && ( pFirst[1] == pSecond[1] );
		}

		template <typename TPVal>
		inline bool cmpNotEqual( const Array2<TPVal> & pFirst, const Array2<TPVal> & pSecond )
		{
			return ( pFirst[0] != pSecond[0] ) || ( pFirst[1] != pSecond[1] );
		}

		template <typename TPVal>
		inline bool cmpLess( const Array2<TPVal> & pFirst, const Array2<TPVal> & pSecond )
		{
			return  ( pFirst[0] < pSecond[0] ) || ( ( pFirst[0] == pSecond[0] ) && ( pFirst[1] < pSecond[1] ) );
		}



		template <typename TPVal>
		inline void set( Array3<TPVal> & pDest, float pX, float pY, float pZ )
		{
			pDest[0] = pX;
			pDest[1] = pY;
			pDest[2] = pZ;
		}

		template <typename TPVal>
		inline void set( Array3<TPVal> & pDest, TPVal pValue )
		{
			pDest[0] = pValue;
			pDest[1] = pValue;
			pDest[2] = pValue;
		}

		template <typename TPVal>
		inline void set( Array3<TPVal> & pDest, const float * pArray )
		{
			pDest[0] = pArray[0];
			pDest[1] = pArray[1];
			pDest[2] = pArray[2];
		}

		template <typename TPVal>
		inline void add( const Array3<TPVal> & pFirst, const Array3<TPVal> & pSecond, Array3<TPVal> & pResult )
		{
			pResult[0] = pFirst[0] + pSecond[0];
			pResult[1] = pFirst[1] + pSecond[1];
			pResult[2] = pFirst[2] + pSecond[2];
		}

		template <typename TPVal>
		inline void sub( const Array3<TPVal> & pFirst, const Array3<TPVal> & pSecond, Array3<TPVal> & pResult )
		{
			pResult[0] = pFirst[0] - pSecond[0];
			pResult[1] = pFirst[1] - pSecond[1];
			pResult[2] = pFirst[2] - pSecond[2];
		}

		template <typename TPVal>
		inline void mul( const Array3<TPVal> & pFirst, const Array3<TPVal> & pSecond, Array3<TPVal> & pResult )
		{
			pResult[0] = pFirst[0] * pSecond[0];
			pResult[1] = pFirst[1] * pSecond[1];
			pResult[2] = pFirst[2] * pSecond[2];
		}

		template <typename TPVal, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		inline void mul( const Array3<TPVal> & pData, const TPScalar pScalar, Array3<TPVal> & pResult )
		{
			pResult[0] = pData[0] * pScalar;
			pResult[1] = pData[1] * pScalar;
			pResult[2] = pData[2] * pScalar;
		}

		template <typename TPVal>
		inline void div( const Array3<TPVal> & pFirst, const Array3<TPVal> & pSecond, Array3<TPVal> & pResult )
		{
			pResult[0] = pFirst[0] / pSecond[0];
			pResult[1] = pFirst[1] / pSecond[1];
			pResult[2] = pFirst[2] / pSecond[2];
		}

		template <typename TPVal, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		inline void div( const Array3<TPVal> & pData, const TPScalar pScalar, Array3<TPVal> & pResult )
		{
			pResult[0] = pData[0] / pScalar;
			pResult[1] = pData[1] / pScalar;
			pResult[2] = pData[2] / pScalar;
		}

		template <typename TPVal, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		inline void div( const TPScalar pScalar, const Array3<TPVal> & pData, Array3<TPVal> & pResult )
		{
			pResult[0] = pScalar / pData[0];
			pResult[1] = pScalar / pData[1];
			pResult[2] = pScalar / pData[2];
		}

		template <typename TPVal>
		inline void neg( const Array3<TPVal> & pData, Array3<TPVal> & pResult )
		{
			pResult[0] = ( -pData[0] );
			pResult[1] = ( -pData[1] );
			pResult[2] = ( -pData[2] );
		}

		template <typename TPVal>
		inline void cross( const Array3<TPVal> & pFirst, const Array3<TPVal> & pSecond, Array3<TPVal> & pResult )
		{
			pResult[0] = pFirst[1] * pSecond[2] - pFirst[2] * pSecond[1];
			pResult[1] = pFirst[2] * pSecond[0] - pFirst[0] * pSecond[2];
			pResult[2] = pFirst[0] * pSecond[1] - pFirst[1] * pSecond[0];
		}

		template <typename TPVal>
		inline TPVal dot( const Array3<TPVal> & pFirst, const Array3<TPVal> & pSecond )
		{
			return pFirst[0] * pSecond[0] + pFirst[1] * pSecond[1] + pFirst[2] * pSecond[2];
		}

		template <typename TPVal>
		static double length( const Array3<TPVal> & pData )
		{
			return sqrt( dot( pData, pData ) );
		}

		template <typename TPVal>
		inline void normalize( const Array3<TPVal> & pData, Array3<TPVal> & pResult )
		{
			const auto dotInvSqrt = invSqrt( dot( pData, pData ) );
			pResult[0] = pData[0] * dotInvSqrt;
			pResult[1] = pData[1] * dotInvSqrt;
			pResult[2] = pData[2] * dotInvSqrt;
		}

		template <typename TPVal>
		inline void normalizeInplace( Array3<TPVal> & pData )
		{
			normalize( pData, pData );
		}

		template <typename TPVal>
		inline bool cmpEqual( const Array3<TPVal> & pFirst, const Array3<TPVal> & pSecond )
		{
			return ( pFirst[0] == pSecond[0] ) && ( pFirst[1] == pSecond[1] ) && ( pFirst[2] == pSecond[2] );
		}

		template <typename TPVal>
		inline bool cmpNotEqual( const Array3<TPVal> & pFirst, const Array3<TPVal> & pSecond )
		{
			return ( pFirst[0] != pSecond[0] ) || ( pFirst[1] != pSecond[1] ) || ( pFirst[2] != pSecond[2] );
		}

		template <typename TPVal>
		inline bool cmpLess( const Array3<TPVal> & pFirst, const Array3<TPVal> & pSecond )
		{
			return ( pFirst[0] < pSecond[0] ) ||
			       ( ( pFirst[0] == pSecond[0] ) && ( pFirst[1] < pSecond[1] ) ) ||
			       ( ( pFirst[0] == pSecond[0] ) && ( pFirst[1] == pSecond[1] ) && ( pFirst[2] < pSecond[2] ) );
		}


		template <typename TPVal>
		inline void set( Array4<TPVal> & pDest, float pX, float pY, float pZ, float pW )
		{
			pDest[0] = pX;
			pDest[1] = pY;
			pDest[2] = pZ;
			pDest[3] = pW;
		}

		template <typename TPVal>
		inline void set( Array4<TPVal> & pDest, TPVal pValue )
		{
			pDest[0] = pValue;
			pDest[1] = pValue;
			pDest[2] = pValue;
			pDest[3] = pValue;
		}

		template <typename TPVal>
		inline void set( Array4<TPVal> & pDest, const float * pArray )
		{
			pDest[0] = pArray[0];
			pDest[1] = pArray[1];
			pDest[2] = pArray[2];
			pDest[3] = pArray[3];
		}

		template <typename TPVal>
		inline void add( const Array4<TPVal> & pFirst, const Array4<TPVal> & pSecond, Array4<TPVal> & pResult )
		{
			pResult[0] = pFirst[0] + pSecond[0];
			pResult[1] = pFirst[1] + pSecond[1];
			pResult[2] = pFirst[2] + pSecond[2];
			pResult[3] = pFirst[3] + pSecond[3];
		}

		template <typename TPVal>
		inline void sub( const Array4<TPVal> & pFirst, const Array4<TPVal> & pSecond, Array4<TPVal> & pResult )
		{
			pResult[0] = pFirst[0] - pSecond[0];
			pResult[1] = pFirst[1] - pSecond[1];
			pResult[2] = pFirst[2] - pSecond[2];
			pResult[3] = pFirst[3] - pSecond[3];
		}

		template <typename TPVal>
		inline void mul( const Array4<TPVal> & pFirst, const Array4<TPVal> & pSecond, Array4<TPVal> & pResult )
		{
			pResult[0] = pFirst[0] * pSecond[0];
			pResult[1] = pFirst[1] * pSecond[1];
			pResult[2] = pFirst[2] * pSecond[2];
			pResult[3] = pFirst[3] * pSecond[3];
		}

		template <typename TPVal, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		inline void mul( const Array4<TPVal> & pData, const TPScalar pScalar, Array4<TPVal> & pResult )
		{
			pResult[0] = pData[0] * pScalar;
			pResult[1] = pData[1] * pScalar;
			pResult[2] = pData[2] * pScalar;
			pResult[3] = pData[3] * pScalar;
		}

		template <typename TPVal>
		inline void div( const Array4<TPVal> & pFirst, const Array4<TPVal> & pSecond, Array4<TPVal> & pResult )
		{
			pResult[0] = pFirst[0] / pSecond[0];
			pResult[1] = pFirst[1] / pSecond[1];
			pResult[2] = pFirst[2] / pSecond[2];
			pResult[3] = pFirst[3] / pSecond[3];
		}

		template <typename TPVal, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		inline void div( const Array4<TPVal> & pData, const TPScalar pScalar, Array4<TPVal> & pResult )
		{
			pResult[0] = pData[0] / pScalar;
			pResult[1] = pData[1] / pScalar;
			pResult[2] = pData[2] / pScalar;
			pResult[3] = pData[3] / pScalar;
		}

		template <typename TPVal, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
		inline void div( const TPScalar pScalar, const Array4<TPVal> & pData, Array4<TPVal> & pResult )
		{
			pResult[0] = pScalar / pData[0];
			pResult[1] = pScalar / pData[1];
			pResult[2] = pScalar / pData[2];
			pResult[3] = pScalar / pData[3];
		}

		template <typename TPVal>
		inline void neg( const Array4<TPVal> & pData, Array4<TPVal> & pResult )
		{
			pResult[0] = ( -pData[0] );
			pResult[1] = ( -pData[1] );
			pResult[2] = ( -pData[2] );
			pResult[3] = ( -pData[3] );
		}

		template <typename TPVal>
		inline TPVal dot( const Array4<TPVal> & pFirst, const Array4<TPVal> & pSecond )
		{
			return pFirst[0] * pSecond[0] + pFirst[1] * pSecond[1] + pFirst[2] * pSecond[2] + pFirst[3] * pSecond[3];
		}

		template <typename TPVal>
		static double length( const Array4<TPVal> & pData )
		{
			return sqrt( dot( pData, pData ) );
		}

		template <typename TPVal>
		inline void normalize( const Array4<TPVal> & pData, Array4<TPVal> & pResult )
		{
			const auto dotInvSqrt = invSqrt( dot( pData, pData ) );
			pResult[0] = pData[0] * dotInvSqrt;
			pResult[1] = pData[1] * dotInvSqrt;
			pResult[2] = pData[2] * dotInvSqrt;
			pResult[3] = pData[3] * dotInvSqrt;
		}

		template <typename TPVal>
		inline void normalizeInplace( Array4<TPVal> & pData )
		{
			normalize( pData, pData );
		}

		template <typename TPVal>
		inline bool cmpEqual( const Array4<TPVal> & pFirst, const Array4<TPVal> & pSecond )
		{
			return ( pFirst[0] == pSecond[0] ) && ( pFirst[1] == pSecond[1] ) && ( pFirst[2] == pSecond[2] ) && ( pFirst[3] == pSecond[3] );
		}

		template <typename TPVal>
		inline bool cmpNotEqual( const Array4<TPVal> & pFirst, const Array4<TPVal> & pSecond )
		{
			return ( pFirst[0] != pSecond[0] ) || ( pFirst[1] != pSecond[1] ) || ( pFirst[2] != pSecond[2] ) || ( pFirst[3] != pSecond[3] );
		}

		template <typename TPVal>
		inline bool cmpLess( const Array4<TPVal> & pFirst, const Array4<TPVal> & pSecond )
		{
			return ( pFirst[0] < pSecond[0] ) ||
			       ( ( pFirst[0] == pSecond[0] ) && ( pFirst[1] < pSecond[1] ) ) ||
			       ( ( pFirst[0] == pSecond[0] ) && ( pFirst[1] == pSecond[1] ) && ( pFirst[2] < pSecond[2] ) ) ||
			       ( ( pFirst[0] == pSecond[0] ) && ( pFirst[1] == pSecond[1] ) && ( pFirst[2] == pSecond[2] ) && ( pFirst[3] < pSecond[3] ) );
		}

	} // namespace ArrayOps

} // namespace Ic3::Math

#if( IC3_MATH_SIMD_ENABLE )
#  include "ArrayOpsSIMD.inl"
#endif

#endif // __IC3_MATH_ARRAY_OPS_H__
