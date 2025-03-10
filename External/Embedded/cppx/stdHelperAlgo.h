
#ifndef __CPPX_STD_HELPER_ALGO_H__
#define __CPPX_STD_HELPER_ALGO_H__

#include "range.h"

namespace cppx
{

	template <typename TPValue>
	size_t copy_array(
			cppx::array_view<const TPValue> pSource,
			cppx::array_view<TPValue> pTarget,
			size_t pElemCount = cve::max_size )
	{
		const auto sourceRegion = get_valid_region( 0, pElemCount, pSource.size() );
		const auto targetRegion = get_valid_region( 0, pElemCount, pTarget.size() );
		const auto copySize = get_min_of( sourceRegion.size, targetRegion.size );
		if( copySize > 0 )
		{
			for( size_t iElement = 0; iElement < copySize; ++iElement )
			{
				pSource[targetRegion.offset + iElement] = pTarget[sourceRegion.offset + iElement];
			}
		}
		return copySize;
	}

	template <typename TPValue, size_t tpSourceSize, size_t tpTargetSize>
	size_t copy_array(
			const TPValue ( & pSourceArray )[tpSourceSize],
			TPValue ( & pTargetArray )[tpTargetSize],
			size_t pSourceOffset = 0,
			size_t pTargetOffset = 0,
			size_t pElemCount = cve::max_size )
	{
		const auto sourceRegion = get_valid_region( pSourceOffset, pElemCount, tpSourceSize );
		const auto targetRegion = get_valid_region( pTargetOffset, pElemCount, tpTargetSize );
		const auto copySize = get_min_of( sourceRegion.size, targetRegion.size );
		if( copySize > 0 )
		{
			for( size_t iElement = 0; iElement < copySize; ++iElement )
			{
				pTargetArray[targetRegion.offset + iElement] = pSourceArray[sourceRegion.offset + iElement];
			}
		}
		return copySize;
	}

	template <typename TPValue, size_t tpSourceSize, size_t tpTargetSize>
	size_t copy_array( const TPValue ( & pSourceArray )[tpSourceSize], TPValue ( & pTargetArray )[tpTargetSize] )
	{
		const auto copySize = get_min_of( tpSourceSize, tpTargetSize );
		for( size_t iElement = 0; iElement < copySize; ++iElement )
		{
			pTargetArray[iElement] = pSourceArray[iElement];
		}
		return copySize;
	}

	/// @brief Finds a value in the specified range which equals to the ref value. Returns the pointer to the element or nullptr.
	/// @param pRange A range to search withing. Must support begin()/end().
	/// @param pRef A reference value to compare against.
	template <typename TPRange, typename TPValue>
	typename TPRange::value_type * find_or_get_null( TPRange & pRange, const TPValue & pRef )
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
	const typename TPRange::value_type * find_or_get_null( const TPRange & pRange, const TPValue & pRef )
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
	typename TPRange::value_type * find_if_or_get_null( TPRange & pRange, TPPredicate pPredicate )
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
	const typename TPRange::value_type * find_if_or_get_null( const TPRange & pRange, TPPredicate pPredicate )
	{
		const auto elementIter = std::find_if( begin( pRange ), end( pRange ), std::forward<TPPredicate>( pPredicate ) );
		if( elementIter != end( pRange ) )
		{
			return &( *elementIter );
		}
		return nullptr;
	}

	template <typename TPMap, typename TPKey = typename TPMap::key_type>
	typename TPMap::mapped_type * get_map_value_ptr_or_null( TPMap & pMap, const TPKey & pKey )
	{
		const auto valueIter = pMap.find( pKey );
		if( valueIter != pMap.end() )
		{
			return &( valueIter->second );
		}
		return nullptr;
	}

	template <typename TPMap, typename TPKey = typename TPMap::key_type>
	const typename TPMap::mapped_type * get_map_value_ptr_or_null( const TPMap & pMap, const TPKey & pKey )
	{
		const auto valueIter = pMap.find( pKey );
		if( valueIter != pMap.end() )
		{
			return &( valueIter->second );
		}
		return nullptr;
	}

	template <typename TPMap, typename TPKey = typename TPMap::key_type>
	typename TPMap::mapped_type & get_map_value_ref_or_default( const TPMap & pMap, const TPKey & pKey, typename TPMap::mapped_type & pDefault )
	{
		const auto valueIter = pMap.find( pKey );
		if( valueIter != pMap.end() )
		{
			return valueIter->second;
		}
		return pDefault;
	}

	template <typename TPMap, typename TPKey = typename TPMap::key_type>
	const typename TPMap::mapped_type & get_map_value_ref_or_default( const TPMap & pMap, const TPKey & pKey, const typename TPMap::mapped_type & pDefault )
	{
		const auto valueIter = pMap.find( pKey );
		if( valueIter != pMap.end() )
		{
			return valueIter->second;
		}
		return pDefault;
	}

} // namespace Ic3

#endif // __CPPX_STD_HELPER_ALGO_H__
