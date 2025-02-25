
#ifndef __CPPX_PREREQUISITES_H__
#define __CPPX_PREREQUISITES_H__

#include "platform/platform.h"
#include "platform/fileLocationInfo.h"
#include "platform/debug.h"

#include <algorithm>
#include <limits>
#include <type_traits>
#include <utility>

namespace cppx
{

	namespace cve
	{

		/// @brief Represents max value which can be stored in the size_t type.
		inline constexpr size_t max_size = static_cast<size_t>( -1 );

		/// @brief
		inline constexpr size_t invalid_length = static_cast<size_t>( -1 );

		/// @brief Represents and invalid position (e.g. in a container or a continuous range).
		inline constexpr size_t invalid_position = static_cast<size_t>( -1 );

	}

	template <typename TPValue>
	struct array_delete
	{
		void operator()( TPValue * pInputPtr ) const
		{
			delete[] pInputPtr;
		}
	};

	template <typename TPValue>
	struct default_delete
	{
		void operator()( TPValue * pInputPtr ) const
		{
			delete pInputPtr;
		}
	};

	template <typename TPValue>
	struct empty_delete
	{
		void operator()( TPValue * ) const
		{}
	};

	template <typename TPResult, typename TPInput>
	inline TPResult numeric_cast( TPInput pInput )
	{
		cppx_debug_assert( pInput <= std::numeric_limits<TPResult>::max() );
		return static_cast<TPResult>( pInput );
	}

	template <typename TPResult, typename TPInput>
	inline constexpr TPResult static_numeric_cast( TPInput pInput )
	{
		return static_cast<TPResult>( pInput );
	}

}

#endif // __CPPX_PREREQUISITES_H__
