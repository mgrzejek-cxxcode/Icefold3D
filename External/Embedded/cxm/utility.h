
#ifndef __CXM_UTILITY_H__
#define __CXM_UTILITY_H__

#include "prerequisites.h"

namespace cxm
{

	inline float sqrt( float pValue )
	{
		return std::sqrt( pValue );
	}

	inline double sqrt( double pValue )
	{
		return std::sqrt( pValue );
	}

	inline long double sqrt( long double pValue )
	{
		return std::sqrt( pValue );
	}

	template <typename TVal>
	inline double sqrt( TVal pValue )
	{
		return std::sqrt( static_cast<double>( pValue ) );
	}

	inline float inv_sqrt( float pValue )
	{
		return 1.0f / sqrt( pValue );
	}

	inline double inv_sqrt( double pValue )
	{
		return 1.0 / sqrt( pValue );
	}

	inline long double inv_sqrt( long double pValue )
	{
		return 1.0L / sqrt( pValue );
	}

	template <typename TVal>
	inline double inv_sqrt( TVal pValue )
	{
		return 1.0 / sqrt( pValue );
	}

	inline std::pair<float, float> sin_cos( float pValue )
	{
		return { std::sin( pValue ), std::cos( pValue ) };
	}

} // namespace cxm

#endif // __CXM_VECTOR_OPS_H__
