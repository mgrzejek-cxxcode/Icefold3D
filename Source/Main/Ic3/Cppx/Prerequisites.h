
#ifndef __IC3_CPPX_PREREQUISITES_H__
#define __IC3_CPPX_PREREQUISITES_H__

#include <Ic3/Platform/Debug.h>

#include <algorithm>
#include <limits>
#include <type_traits>
#include <utility>

namespace Ic3
{

	namespace CxDef
	{

		/// @brief Represents max value which can be stored in the size_t type.
		inline constexpr size_t MAX_SIZE = static_cast<size_t>( -1 );

		/// @brief
		inline constexpr size_t INVALID_LENGTH = static_cast<size_t>( -1 );

		/// @brief Represents and invalid position (e.g. in a container or a continuous range).
		inline constexpr size_t INVALID_POSITION = static_cast<size_t>( -1 );

	}

	template <typename TType>
	struct ArrayDelete
	{
		void operator()( TType * pInputPtr ) const
		{
			delete[] pInputPtr;
		}
	};

	template <typename TType>
	struct DefaultDelete
	{
		void operator()( TType * pInputPtr ) const
		{
			delete pInputPtr;
		}
	};

	template <typename TType>
	struct EmptyDelete
	{
		void operator()( TType * ) const
		{}
	};

	template <typename TResult, typename TInput>
	inline TResult numeric_cast( TInput pInput )
	{
		ic3DebugAssert( pInput <= std::numeric_limits<TResult>::max() );
		return static_cast<TResult>( pInput );
	}

	template <typename TResult, typename TInput>
	inline constexpr TResult static_numeric_cast( TInput pInput )
	{
		return static_cast<TResult>( pInput );
	}

}

#endif // __IC3_CPPX_PREREQUISITES_H__
