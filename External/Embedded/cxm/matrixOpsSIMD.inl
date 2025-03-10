
#if !defined( __CXM_MATRIX_OPS_H__ )
#  error ""
#endif

namespace cxm
{

#if( CXM_SIMD_USE_VX128F )

	PCL_ATTR_ALWAYS_INLINE __m128 _mm_mul_mat4_vec4_128f( __m128 pM0, __m128 pM1, __m128 pM2, __m128 pM3, __m128 pV0 )
	{
		const __m128 mul0 = _mm_mul_ps( pM0, pV0 );
		const __m128 mul1 = _mm_mul_ps( pM1, pV0 );
		const __m128 mul2 = _mm_mul_ps( pM2, pV0 );
		const __m128 mul3 = _mm_mul_ps( pM3, pV0 );

		const __m128 taccum01 = _mm_add_ps( mul0, mul1 );
		const __m128 taccum23 = _mm_add_ps( mul2, mul3 );
		const __m128 result = _mm_add_ps( taccum01, taccum23 );

		return result;
	}

	inline vector4<float> mul( const matrix4x4<float> & pMatrix, const vector4<float> & pVector )
	{
		vector4<float> result;
		result.mmv = _mm_mul_mat4_vec4_128f( pMatrix.mm0, pMatrix.mm1, pMatrix.mm2, pMatrix.mm3, pVector.mmv );
		return result;
	}

	inline void mul( const matrix4x4<float> & pMatrix, const vector4<float> & pVector, vector4<float> & pResult )
	{
		pResult.mmv = _mm_mul_mat4_vec4_128f( pMatrix.mm0, pMatrix.mm1, pMatrix.mm2, pMatrix.mm3, pVector.mmv );
	}

#endif // CXM_SIMD_USE_VX128F

} // namespace cxm
