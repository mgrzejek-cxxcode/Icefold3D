
#ifndef __CXM_VECTOR_OPS_H__
#define __CXM_VECTOR_OPS_H__

#include "arrayOps.h"
#include "vector.h"

namespace cxm
{

	template <typename TPValue, size_t tpSize>
	inline void add( const vector<TPValue, tpSize> & pFirst, const vector<TPValue, tpSize> & pSecond, vector<TPValue, tpSize> & pResult )
	{
		array_ops::add( pFirst.values, pSecond.values, pResult.values );
	}

	template <typename TPValue, size_t tpSize>
	inline vector<TPValue, tpSize> add( const vector<TPValue, tpSize> & pFirst, const vector<TPValue, tpSize> & pSecond )
	{
		vector<TPValue, tpSize> result;
		array_ops::add( pFirst.values, pSecond.values, result.values );
		return result;
	}

	template <typename TPValue, size_t tpSize>
	inline void sub( const vector<TPValue, tpSize> & pFirst, const vector<TPValue, tpSize> & pSecond, vector<TPValue, tpSize> & pResult )
	{
		array_ops::add( pFirst.values, pSecond.values, pResult.values );
	}

	template <typename TPValue, size_t tpSize>
	inline vector<TPValue, tpSize> sub( const vector<TPValue, tpSize> & pFirst, const vector<TPValue, tpSize> & pSecond )
	{
		vector<TPValue, tpSize> result;
		array_ops::sub( pFirst.values, pSecond.values, result.values );
		return result;
	}

	template <typename TPValue, size_t tpSize>
	inline void mul( const vector<TPValue, tpSize> & pFirst, const vector<TPValue, tpSize> & pSecond, vector<TPValue, tpSize> & pResult )
	{
		array_ops::mul( pFirst.values, pSecond.values, pResult.values );
	}

	template <typename TPValue, size_t tpSize, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
	inline void mul( const vector<TPValue, tpSize> & pVector, const TPScalar pScalar, vector<TPValue, tpSize> & pResult )
	{
		array_ops::mul( pVector.values, pScalar, pResult.values );
	}

	template <typename TPValue, size_t tpSize>
	inline vector<TPValue, tpSize> mul( const vector<TPValue, tpSize> & pFirst, const vector<TPValue, tpSize> & pSecond )
	{
		vector<TPValue, tpSize> result;
		array_ops::mul( pFirst.values, pSecond.values, result.values );
		return result;
	}

	template <typename TPValue, size_t tpSize, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
	inline vector<TPValue, tpSize> mul( const vector<TPValue, tpSize> & pVector, const TPScalar pScalar )
	{
		vector<TPValue, tpSize> result;
		array_ops::mul( pVector.values, pScalar, result.values );
		return result;
	}

	template <typename TPValue, size_t tpSize>
	inline void div( const vector<TPValue, tpSize> & pFirst, const vector<TPValue, tpSize> & pSecond, vector<TPValue, tpSize> & pResult )
	{
		array_ops::div( pFirst.values, pSecond.values, pResult.values );
	}

	template <typename TPValue, size_t tpSize, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
	inline void div( const vector<TPValue, tpSize> & pVector, const TPScalar pScalar, vector<TPValue, tpSize> & pResult )
	{
		array_ops::div( pVector.values, pScalar, pResult.values );
	}

	template <typename TPValue, size_t tpSize, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
	inline void div( const TPScalar pScalar, const vector<TPValue, tpSize> & pVector, vector<TPValue, tpSize> & pResult )
	{
		array_ops::div( pScalar, pVector.values, pResult.values );
	}

	template <typename TPValue, size_t tpSize>
	inline vector<TPValue, tpSize> div( const vector<TPValue, tpSize> & pFirst, const vector<TPValue, tpSize> & pSecond )
	{
		vector<TPValue, tpSize> result;
		array_ops::div( pFirst.values, pSecond.values, result.values );
		return result;
	}

	template <typename TPValue, size_t tpSize, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
	inline vector<TPValue, tpSize> div( const vector<TPValue, tpSize> & pVector, const TPScalar pScalar )
	{
		vector<TPValue, tpSize> result;
		array_ops::div( pVector.values, pScalar, result.values );
		return result;
	}

	template <typename TPValue, size_t tpSize, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
	inline vector<TPValue, tpSize> div( const TPScalar pScalar, const vector<TPValue, tpSize> & pVector )
	{
		vector<TPValue, tpSize> result;
		array_ops::div( pScalar, pVector.values, result.values );
		return result;
	}

	template <typename TPValue, size_t tpSize>
	inline void neg( const vector<TPValue, tpSize> & pVector, vector<TPValue, tpSize> & pResult )
	{
		array_ops::neg( pVector.values, pResult.values );
	}

	template <typename TPValue, size_t tpSize>
	inline vector<TPValue, tpSize> neg( const vector<TPValue, tpSize> & pVector )
	{
		vector<TPValue, tpSize> result;
		array_ops::neg( pVector.values, result.values );
		return result;
	}

	template <typename TPValue>
	inline void cross( const vector3<TPValue> & pFirst, const vector3<TPValue> & pSecond, vector3<TPValue> & pResult )
	{
		array_ops::cross( pFirst.values, pSecond.values, pResult.values );
	}

	template <typename TPValue>
	inline vector3<TPValue> cross( const vector3<TPValue> & pFirst, const vector3<TPValue> & pSecond )
	{
		vector3<TPValue> result;
		array_ops::cross( pFirst.values, pSecond.values, result.values );
		return result;
	}

	template <typename TPValue, size_t tpSize>
	inline TPValue dot( const vector<TPValue, tpSize> & pFirst, const vector<TPValue, tpSize> & pSecond )
	{
		return array_ops::dot( pFirst.values, pSecond.values );
	}

	template <typename TPValue, size_t tpSize>
	inline void normalize( const vector<TPValue, tpSize> & pVector, vector<TPValue, tpSize> & pResult )
	{
		array_ops::normalize( pVector.values, pResult.values );
	}

	template <typename TPValue, size_t tpSize>
	inline vector<TPValue, tpSize> normalize( const vector<TPValue, tpSize> & pVector )
	{
		vector<TPValue, tpSize> result;
		array_ops::normalize( pVector.values, result.values );
		return result;
	}

	template <typename TPValue, size_t tpSize>
	inline void normalize_inplace( vector<TPValue, tpSize> & pVector )
	{
		array_ops::normalize_inplace( pVector.values );
	}

	template <typename TPValue, size_t tpSize>
	inline TPValue length( const vector<TPValue, tpSize> & pVector )
	{
		return static_cast<TPValue>( array_ops::length( pVector.values ) );
	}

	template <typename TPResult, typename TPValue, size_t tpSize>
	inline TPResult length_as( const vector<TPValue, tpSize> & pVector )
	{
		return static_cast<TPResult>( length( pVector ) );
	}

	template <typename TPValue, size_t tpSize>
	inline bool cmp_equal( const vector<TPValue, tpSize> & pFirst, const vector<TPValue, tpSize> & pSecond )
	{
		return array_ops::cmp_equal( pFirst.values, pSecond.values );
	}

	template <typename TPValue, size_t tpSize>
	inline bool cmp_not_equal( const vector<TPValue, tpSize> & pFirst, const vector<TPValue, tpSize> & pSecond )
	{
		return array_ops::cmp_not_equal( pFirst.values, pSecond.values );
	}

	template <typename TPValue, size_t tpSize>
	inline bool cmp_less( const vector<TPValue, tpSize> & pFirst, const vector<TPValue, tpSize> & pSecond )
	{
		return array_ops::cmp_less( pFirst.values, pSecond.values );
	}


	template <typename TPValue, size_t tpSize>
	inline bool operator==( const vector<TPValue, tpSize> & pLhs, const vector<TPValue, tpSize> & pRhs ) noexcept
	{
		return cmp_equal( pLhs, pRhs );
	}

	template <typename TPValue, size_t tpSize>
	inline bool operator!=( const vector<TPValue, tpSize> & pLhs, const vector<TPValue, tpSize> & pRhs ) noexcept
	{
		return cmp_not_equal( pLhs, pRhs );
	}

	template <typename TPValue, size_t tpSize>
	inline vector<TPValue, tpSize> operator+( const vector<TPValue, tpSize> & pLhs, const vector<TPValue, tpSize> & pRhs ) noexcept
	{
		return add( pLhs, pRhs );
	}

	template <typename TPValue, size_t tpSize>
	inline vector<TPValue, tpSize> operator-( const vector<TPValue, tpSize> & pLhs, const vector<TPValue, tpSize> & pRhs ) noexcept
	{
		return sub( pLhs, pRhs );
	}

	template <typename TPValue, size_t tpSize>
	inline vector<TPValue, tpSize> operator*( const vector<TPValue, tpSize> & pLhs, const vector<TPValue, tpSize> & pRhs ) noexcept
	{
		return mul( pLhs, pRhs );
	}

	template <typename TPValue, size_t tpSize, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
	inline vector<TPValue, tpSize> operator*( const vector<TPValue, tpSize> & pLhs, const TPScalar pRhs ) noexcept
	{
		return mul( pLhs, pRhs );
	}

	template <typename TPValue, size_t tpSize, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
	inline vector<TPValue, tpSize> operator*( const TPScalar pLhs, const vector<TPValue, tpSize> & pRhs ) noexcept
	{
		return mul( pRhs, pLhs );
	}

	template <typename TPValue, size_t tpSize>
	inline vector<TPValue, tpSize> operator/( const vector<TPValue, tpSize> & pLhs, const vector<TPValue, tpSize> & pRhs ) noexcept
	{
		return div( pLhs, pRhs );
	}

	template <typename TPValue, size_t tpSize, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
	inline vector<TPValue, tpSize> operator/( const vector<TPValue, tpSize> & pLhs, const TPScalar pRhs ) noexcept
	{
		return div( pLhs, pRhs );
	}

	template <typename TPValue, size_t tpSize, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
	inline vector<TPValue, tpSize> operator/( const TPScalar pLhs, const vector<TPValue, tpSize> & pRhs ) noexcept
	{
		return div( pLhs, pRhs );
	}

	template <typename TPValue, size_t tpSize>
	inline vector<TPValue, tpSize> operator-( const vector<TPValue, tpSize> & pRhs ) noexcept
	{
		return neg( pRhs );
	}


	template <typename TPValue, size_t tpSize>
	inline vector<TPValue, tpSize> & operator+=( vector<TPValue, tpSize> & pLhs, const vector<TPValue, tpSize> & pRhs ) noexcept
	{
		pLhs = add( pLhs, pRhs );
		return pLhs;
	}

	template <typename TPValue, size_t tpSize>
	inline vector<TPValue, tpSize> & operator-=( vector<TPValue, tpSize> & pLhs, const vector<TPValue, tpSize> & pRhs ) noexcept
	{
		pLhs = sub( pLhs, pRhs );
		return pLhs;
	}

	template <typename TPValue, size_t tpSize, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
	inline vector<TPValue, tpSize> & operator*=( vector<TPValue, tpSize> & pLhs, const TPScalar pRhs ) noexcept
	{
		pLhs = mul( pLhs, pRhs );
		return pLhs;
	}

	template <typename TPValue, size_t tpSize, typename TPScalar, enable_if_scalar_t<TPScalar> = true>
	inline vector<TPValue, tpSize> & operator/=( vector<TPValue, tpSize> & pLhs, const TPScalar pRhs ) noexcept
	{
		pLhs = div( pLhs, pRhs );
		return pLhs;
	}

} // namespace cxm

#endif // __CXM_VECTOR_OPS_H__
