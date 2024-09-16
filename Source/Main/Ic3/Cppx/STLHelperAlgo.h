
#ifndef __IC3_CPPX_STL_HELPER_ALGO_H__
#define __IC3_CPPX_STL_HELPER_ALGO_H__

#include "Prerequisites.h"

namespace Ic3::Cppx
{

	/// @brief Finds a value in the specified range which equals to the ref value. Returns the pointer to the element or nullptr.
	/// @param pRange A range to search withing. Must support begin()/end().
	/// @param pRef A reference value to compare against.
	template <typename TPRange, typename TPValue>
	typename TPRange::value_type * findByRefValueOrGetNull( TPRange & pRange, const TPValue & pRef )
	{
		const auto elementIter = std::find_if( begin( pRange ), end( pRange ), [&pRef]( const auto & pElement ) -> bool {
			return pRef == pElement;
		});
		if( elementIter != end( pRange ) )
		{
			return &( *elementIter );
		}
		return nullptr;
	}

	/// @brief Finds a value in the specified range which equals to the ref value. Returns the pointer to the element or nullptr.
	/// @param pRange A range to search withing. Must support begin()/end().
	/// @param pRef A reference value to compare against.
	template <typename TPRange, typename TPValue>
	const typename TPRange::value_type * findByRefValueOrGetNull( const TPRange & pRange, const TPValue & pRef )
	{
		const auto elementIter = std::find_if( begin( pRange ), end( pRange ), [&pRef]( const auto & pElement ) -> bool {
			return pRef == pElement;
		});
		if( elementIter != end( pRange ) )
		{
			return &( *elementIter );
		}
		return nullptr;
	}

	/// @brief Finds a value in the specified range which fulfills the specified predicate. Returns the pointer to the element or nullptr.
	/// @param pRange A range to search withing. Must support begin()/end().
	/// @param pPredicate A predicate to evaluate on each element. Must be a callable, accepting a single range's value-type param.
	template <typename TPRange, typename TPPredicate>
	typename TPRange::value_type * findIfOrGetNull( TPRange & pRange, TPPredicate pPredicate )
	{
		const auto elementIter = std::find_if( begin( pRange ), end( pRange ), std::forward<TPPredicate>( pPredicate ) );
		if( elementIter != end( pRange ) )
		{
			return &( *elementIter );
		}
		return nullptr;
	}

	/// @brief Finds a value in the specified range which fulfills the specified predicate. Returns the pointer to the element or nullptr.
	/// @param pRange A range to search withing. Must support begin()/end().
	/// @param pPredicate A predicate to evaluate on each element. Must be a callable, accepting a single range's value-type param.
	template <typename TPRange, typename TPPredicate>
	const typename TPRange::value_type * findIfOrGetNull( const TPRange & pRange, TPPredicate pPredicate )
	{
		const auto elementIter = std::find_if( begin( pRange ), end( pRange ), std::forward<TPPredicate>( pPredicate ) );
		if( elementIter != end( pRange ) )
		{
			return &( *elementIter );
		}
		return nullptr;
	}

	template <typename TPMap, typename TPKey = typename TPMap::key_type>
	typename TPMap::mapped_type * getMapValuePtrOrNull( TPMap & pMap, const TPKey & pKey )
	{
		const auto valueIter = pMap.find( pKey );
		if( valueIter != pMap.end() )
		{
			return &( valueIter->second );
		}
		return nullptr;
	}

	template <typename TPMap, typename TPKey = typename TPMap::key_type>
	const typename TPMap::mapped_type * getMapValuePtrOrNull( const TPMap & pMap, const TPKey & pKey )
	{
		const auto valueIter = pMap.find( pKey );
		if( valueIter != pMap.end() )
		{
			return &( valueIter->second );
		}
		return nullptr;
	}

	template <typename TPMap, typename TPKey = typename TPMap::key_type>
	typename TPMap::mapped_type & getMapValueRefOrDefault( const TPMap & pMap, const TPKey & pKey, typename TPMap::mapped_type & pDefault )
	{
		const auto valueIter = pMap.find( pKey );
		if( valueIter != pMap.end() )
		{
			return valueIter->second;
		}
		return pDefault;
	}

	template <typename TPMap, typename TPKey = typename TPMap::key_type>
	const typename TPMap::mapped_type & getMapValueRefOrDefault( const TPMap & pMap, const TPKey & pKey, const typename TPMap::mapped_type & pDefault )
	{
		const auto valueIter = pMap.find( pKey );
		if( valueIter != pMap.end() )
		{
			return valueIter->second;
		}
		return pDefault;
	}

} // namespace Ic3

#endif // __IC3_CPPX_STL_HELPER_ALGO_H__
