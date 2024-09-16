
#ifndef __IC3_MATH_VECTOR_OPS_H__
#define __IC3_MATH_VECTOR_OPS_H__

#include "ArrayOps.h"
#include "Vector.h"

namespace Ic3::Math
{

	template <typename TPValue, size_t tpSize>
	inline void add( const Vector<TPValue, tpSize> & pFirst, const Vector<TPValue, tpSize> & pSecond, Vector<TPValue, tpSize> & pResult )
	{
		ArrayOps::add( pFirst.values, pSecond.values, pResult.values );
	}

	template <typename TPValue, size_t tpSize>
	inline Vector<TPValue, tpSize> add( const Vector<TPValue, tpSize> & pFirst, const Vector<TPValue, tpSize> & pSecond )
	{
		Vector<TPValue, tpSize> result;
		ArrayOps::add( pFirst.values, pSecond.values, result.values );
		return result;
	}

	template <typename TPValue, size_t tpSize>
	inline void sub( const Vector<TPValue, tpSize> & pFirst, const Vector<TPValue, tpSize> & pSecond, Vector<TPValue, tpSize> & pResult )
	{
		ArrayOps::add( pFirst.values, pSecond.values, pResult.values );
	}

	template <typename TPValue, size_t tpSize>
	inline Vector<TPValue, tpSize> sub( const Vector<TPValue, tpSize> & pFirst, const Vector<TPValue, tpSize> & pSecond )
	{
		Vector<TPValue, tpSize> result;
		ArrayOps::sub( pFirst.values, pSecond.values, result.values );
		return result;
	}

	template <typename TPValue, size_t tpSize>
	inline void mul( const Vector<TPValue, tpSize> & pFirst, const Vector<TPValue, tpSize> & pSecond, Vector<TPValue, tpSize> & pResult )
	{
		ArrayOps::mul( pFirst.values, pSecond.values, pResult.values );
	}

	template <typename TPValue, size_t tpSize, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
	inline void mul( const Vector<TPValue, tpSize> & pVector, const TPScalar pScalar, Vector<TPValue, tpSize> & pResult )
	{
		ArrayOps::mul( pVector.values, pScalar, pResult.values );
	}

	template <typename TPValue, size_t tpSize>
	inline Vector<TPValue, tpSize> mul( const Vector<TPValue, tpSize> & pFirst, const Vector<TPValue, tpSize> & pSecond )
	{
		Vector<TPValue, tpSize> result;
		ArrayOps::mul( pFirst.values, pSecond.values, result.values );
		return result;
	}

	template <typename TPValue, size_t tpSize, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
	inline Vector<TPValue, tpSize> mul( const Vector<TPValue, tpSize> & pVector, const TPScalar pScalar )
	{
		Vector<TPValue, tpSize> result;
		ArrayOps::mul( pVector.values, pScalar, result.values );
		return result;
	}

	template <typename TPValue, size_t tpSize>
	inline void div( const Vector<TPValue, tpSize> & pFirst, const Vector<TPValue, tpSize> & pSecond, Vector<TPValue, tpSize> & pResult )
	{
		ArrayOps::div( pFirst.values, pSecond.values, pResult.values );
	}

	template <typename TPValue, size_t tpSize, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
	inline void div( const Vector<TPValue, tpSize> & pVector, const TPScalar pScalar, Vector<TPValue, tpSize> & pResult )
	{
		ArrayOps::div( pVector.values, pScalar, pResult.values );
	}

	template <typename TPValue, size_t tpSize, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
	inline void div( const TPScalar pScalar, const Vector<TPValue, tpSize> & pVector, Vector<TPValue, tpSize> & pResult )
	{
		ArrayOps::div( pScalar, pVector.values, pResult.values );
	}

	template <typename TPValue, size_t tpSize>
	inline Vector<TPValue, tpSize> div( const Vector<TPValue, tpSize> & pFirst, const Vector<TPValue, tpSize> & pSecond )
	{
		Vector<TPValue, tpSize> result;
		ArrayOps::div( pFirst.values, pSecond.values, result.values );
		return result;
	}

	template <typename TPValue, size_t tpSize, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
	inline Vector<TPValue, tpSize> div( const Vector<TPValue, tpSize> & pVector, const TPScalar pScalar )
	{
		Vector<TPValue, tpSize> result;
		ArrayOps::div( pVector.values, pScalar, result.values );
		return result;
	}

	template <typename TPValue, size_t tpSize, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
	inline Vector<TPValue, tpSize> div( const TPScalar pScalar, const Vector<TPValue, tpSize> & pVector )
	{
		Vector<TPValue, tpSize> result;
		ArrayOps::div( pScalar, pVector.values, result.values );
		return result;
	}

	template <typename TPValue, size_t tpSize>
	inline void neg( const Vector<TPValue, tpSize> & pVector, Vector<TPValue, tpSize> & pResult )
	{
		ArrayOps::neg( pVector.values, pResult.values );
	}

	template <typename TPValue, size_t tpSize>
	inline Vector<TPValue, tpSize> neg( const Vector<TPValue, tpSize> & pVector )
	{
		Vector<TPValue, tpSize> result;
		ArrayOps::neg( pVector.values, result.values );
		return result;
	}

	template <typename TPValue>
	inline void cross( const Vector3<TPValue> & pFirst, const Vector3<TPValue> & pSecond, Vector3<TPValue> & pResult )
	{
		ArrayOps::cross( pFirst.values, pSecond.values, pResult.values );
	}

	template <typename TPValue>
	inline Vector3<TPValue> cross( const Vector3<TPValue> & pFirst, const Vector3<TPValue> & pSecond )
	{
		Vector3<TPValue> result;
		ArrayOps::cross( pFirst.values, pSecond.values, result.values );
		return result;
	}

	template <typename TPValue, size_t tpSize>
	inline TPValue dot( const Vector<TPValue, tpSize> & pFirst, const Vector<TPValue, tpSize> & pSecond )
	{
		return ArrayOps::dot( pFirst.values, pSecond.values );
	}

	template <typename TPValue, size_t tpSize>
	inline void normalize( const Vector<TPValue, tpSize> & pVector, Vector<TPValue, tpSize> & pResult )
	{
		ArrayOps::normalize( pVector.values, pResult.values );
	}

	template <typename TPValue, size_t tpSize>
	inline Vector<TPValue, tpSize> normalize( const Vector<TPValue, tpSize> & pVector )
	{
		Vector<TPValue, tpSize> result;
		ArrayOps::normalize( pVector.values, result.values );
		return result;
	}

	template <typename TPValue, size_t tpSize>
	inline void normalizeInplace( Vector<TPValue, tpSize> & pVector )
	{
		ArrayOps::normalizeInplace( pVector.values );
	}

	template <typename TPValue, size_t tpSize>
	inline TPValue length( const Vector<TPValue, tpSize> & pVector )
	{
		return static_cast<TPValue>( ArrayOps::length( pVector.values ) );
	}

	template <typename TPResult, typename TPValue, size_t tpSize>
	inline TPResult lengthAs( const Vector<TPValue, tpSize> & pVector )
	{
		return static_cast<TPResult>( length( pVector ) );
	}

	template <typename TPValue, size_t tpSize>
	inline bool cmpEqual( const Vector<TPValue, tpSize> & pFirst, const Vector<TPValue, tpSize> & pSecond )
	{
		return ArrayOps::cmpEqual( pFirst.values, pSecond.values );
	}

	template <typename TPValue, size_t tpSize>
	inline bool cmpNotEqual( const Vector<TPValue, tpSize> & pFirst, const Vector<TPValue, tpSize> & pSecond )
	{
		return ArrayOps::cmpNotEqual( pFirst.values, pSecond.values );
	}

	template <typename TPValue, size_t tpSize>
	inline bool cmpLess( const Vector<TPValue, tpSize> & pFirst, const Vector<TPValue, tpSize> & pSecond )
	{
		return ArrayOps::cmpLess( pFirst.values, pSecond.values );
	}


	template <typename TPValue, size_t tpSize>
	inline bool operator==( const Vector<TPValue, tpSize> & pLhs, const Vector<TPValue, tpSize> & pRhs ) noexcept
	{
		return cmpEqual( pLhs, pRhs );
	}

	template <typename TPValue, size_t tpSize>
	inline bool operator!=( const Vector<TPValue, tpSize> & pLhs, const Vector<TPValue, tpSize> & pRhs ) noexcept
	{
		return cmpNotEqual( pLhs, pRhs );
	}

	template <typename TPValue, size_t tpSize>
	inline Vector<TPValue, tpSize> operator+( const Vector<TPValue, tpSize> & pLhs, const Vector<TPValue, tpSize> & pRhs ) noexcept
	{
		return add( pLhs, pRhs );
	}

	template <typename TPValue, size_t tpSize>
	inline Vector<TPValue, tpSize> operator-( const Vector<TPValue, tpSize> & pLhs, const Vector<TPValue, tpSize> & pRhs ) noexcept
	{
		return sub( pLhs, pRhs );
	}

	template <typename TPValue, size_t tpSize>
	inline Vector<TPValue, tpSize> operator*( const Vector<TPValue, tpSize> & pLhs, const Vector<TPValue, tpSize> & pRhs ) noexcept
	{
		return mul( pLhs, pRhs );
	}

	template <typename TPValue, size_t tpSize, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
	inline Vector<TPValue, tpSize> operator*( const Vector<TPValue, tpSize> & pLhs, const TPScalar pRhs ) noexcept
	{
		return mul( pLhs, pRhs );
	}

	template <typename TPValue, size_t tpSize, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
	inline Vector<TPValue, tpSize> operator*( const TPScalar pLhs, const Vector<TPValue, tpSize> & pRhs ) noexcept
	{
		return mul( pRhs, pLhs );
	}

	template <typename TPValue, size_t tpSize>
	inline Vector<TPValue, tpSize> operator/( const Vector<TPValue, tpSize> & pLhs, const Vector<TPValue, tpSize> & pRhs ) noexcept
	{
		return div( pLhs, pRhs );
	}

	template <typename TPValue, size_t tpSize, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
	inline Vector<TPValue, tpSize> operator/( const Vector<TPValue, tpSize> & pLhs, const TPScalar pRhs ) noexcept
	{
		return div( pLhs, pRhs );
	}

	template <typename TPValue, size_t tpSize, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
	inline Vector<TPValue, tpSize> operator/( const TPScalar pLhs, const Vector<TPValue, tpSize> & pRhs ) noexcept
	{
		return div( pLhs, pRhs );
	}

	template <typename TPValue, size_t tpSize>
	inline Vector<TPValue, tpSize> operator-( const Vector<TPValue, tpSize> & pRhs ) noexcept
	{
		return neg( pRhs );
	}


	template <typename TPValue, size_t tpSize>
	inline Vector<TPValue, tpSize> & operator+=( Vector<TPValue, tpSize> & pLhs, const Vector<TPValue, tpSize> & pRhs ) noexcept
	{
		pLhs = add( pLhs, pRhs );
		return pLhs;
	}

	template <typename TPValue, size_t tpSize>
	inline Vector<TPValue, tpSize> & operator-=( Vector<TPValue, tpSize> & pLhs, const Vector<TPValue, tpSize> & pRhs ) noexcept
	{
		pLhs = sub( pLhs, pRhs );
		return pLhs;
	}

	template <typename TPValue, size_t tpSize, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
	inline Vector<TPValue, tpSize> & operator*=( Vector<TPValue, tpSize> & pLhs, const TPScalar pRhs ) noexcept
	{
		pLhs = mul( pLhs, pRhs );
		return pLhs;
	}

	template <typename TPValue, size_t tpSize, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
	inline Vector<TPValue, tpSize> & operator/=( Vector<TPValue, tpSize> & pLhs, const TPScalar pRhs ) noexcept
	{
		pLhs = div( pLhs, pRhs );
		return pLhs;
	}

} // namespace Ic3::Math

#endif // __IC3_MATH_VECTOR_OPS_H__
