
#ifndef __IC3_CPPX_PREREQUISITES_H__
#define __IC3_CPPX_PREREQUISITES_H__

#include <Ic3/Platform/Debug.h>

#include <algorithm>
#include <limits>
#include <type_traits>
#include <utility>

namespace Ic3
{

	/// @brief Represents max value which can be stored in the size_t type.
	inline constexpr size_t cxMaxSize = static_cast<size_t>( -1 );

	/// @brief
	inline constexpr size_t cxInvalidLength = static_cast<size_t>( -1 );

	/// @brief Represents and invalid position (e.g. in a container or a continuous range).
	inline constexpr size_t cxInvalidPosition = static_cast<size_t>( -1 );

	template <typename TPValue>
	struct ArrayDelete
	{
		void operator()( TPValue * pInputPtr ) const
		{
			delete[] pInputPtr;
		}
	};

	template <typename TPValue>
	struct DefaultDelete
	{
		void operator()( TPValue * pInputPtr ) const
		{
			delete pInputPtr;
		}
	};

	template <typename TPValue>
	struct EmptyDelete
	{
		void operator()( TPValue * ) const
		{}
	};

	template <typename TPResult, typename TPInput>
	inline TPResult numeric_cast( TPInput pInput )
	{
		ic3DebugAssert( pInput <= std::numeric_limits<TPResult>::max() );
		return static_cast<TPResult>( pInput );
	}

	template <typename TPResult, typename TPInput>
	inline constexpr TPResult static_numeric_cast( TPInput pInput )
	{
		return static_cast<TPResult>( pInput );
	}

}

#endif // __IC3_CPPX_PREREQUISITES_H__
