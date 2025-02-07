
#pragma once

#include "range.h"
#include "utilities.h"
#include <functional>

#define CPPX_USE_RUNTIME_CHECKED_MEMORY_ROUTINES 0

namespace cppx
{

	using MemoryAllocCallback = std::function<void *( size_t )>;
	using MemoryFreeCallback = std::function<void( void * )>;
	using MemoryReallocCallback = std::function<void *( void *, size_t )>;

	struct memory_routines_proxy
	{
	public:
		uint32 mem_alignment = PCL_MEMORY_BASE_ALIGNMENT;
		MemoryAllocCallback fn_alloc;
		MemoryFreeCallback fn_free;
		MemoryReallocCallback fn_realloc;

	public:
		explicit operator bool() const
		{
			return fn_alloc && fn_free && fn_realloc;
		}

		void swap( memory_routines_proxy & pOther )
		{
			std::swap( mem_alignment, pOther.mem_alignment );
			std::swap( fn_alloc, pOther.fn_alloc );
			std::swap( fn_free, pOther.fn_free );
			std::swap( fn_realloc, pOther.fn_realloc );
		}
	};

	inline void swap( memory_routines_proxy & pFirst, memory_routines_proxy & pSecond )
	{
		pFirst.swap( pSecond );
	}

	namespace cve
	{

		extern const memory_routines_proxy default_memory_routines;

	}

	template <typename TPValue>
	struct memory_base_size
	{
		static constexpr size_t size = sizeof( TPValue );
	};

	template <>
	struct memory_base_size<void>
	{
		static constexpr size_t size = 1;
	};

	template <typename TSize, typename TOffset = TSize>
	struct aligned_memory_alloc_info
	{
		region<TSize, TOffset> accessible_region;
		region<TSize, TOffset> reserved_region;
	};

	inline constexpr uint64 mem_check_requested_copy_size( uint64 pBufferSize, uint64 pCopySize, uint64 pCopyOffset )
	{
		const auto copyOffset = get_min_of( pCopyOffset, pBufferSize );
		const auto maxCopySize = pBufferSize - copyOffset;
		const auto copySize = get_min_of( pCopySize, maxCopySize );
		return copySize;
	}

	///
	/// @tparam TPValue
	/// @param pValue
	/// @param pAlignment
	/// @return
	template <typename TPValue>
	inline constexpr TPValue mem_get_aligned_power_of_2( TPValue pValue, uint32 pAlignment )
	{
		return static_cast<TPValue>( ( static_cast<uint64>( pValue ) + pAlignment ) & ( ~( static_cast<uint64>( pAlignment ) - 1 ) ) );
	}

	///
	/// @tparam TPValue
	/// @param pValue
	/// @param pAlignment
	/// @return
	template <typename TPValue>
	inline TPValue mem_get_aligned_value( TPValue pValue, uint32 pAlignment )
	{
		const auto valueAlignmentMod = pValue % pAlignment;
		return ( valueAlignmentMod != 0 ) ? ( pValue + pAlignment - valueAlignmentMod ) : pValue;
	}

	template <typename TSize, typename TOffset = TSize, typename std::enable_if<std::is_integral<TSize>::value, int>::type = 0>
	inline aligned_memory_alloc_info<TSize, TOffset> mem_compute_aligned_allocation_info( TOffset pBaseAddress, TSize pAllocationSize, uint32 pAlignment )
	{
		const auto alignedOffset = mem_get_aligned_value( pBaseAddress, pAlignment );

		aligned_memory_alloc_info<TSize, TOffset> allocInfo;
		allocInfo.accessible_region.offset = alignedOffset;
		allocInfo.accessible_region.size = pAllocationSize;
		allocInfo.reserved_region.offset = pBaseAddress;
		allocInfo.reserved_region.size = ( alignedOffset - pBaseAddress ) + pAllocationSize;

		return allocInfo;
	}

	template <typename TPValue>
	bool mem_cmp_equal( const TPValue & pFirst, const TPValue & pSecond )
	{
		return memcmp( &pFirst, &pSecond, sizeof( TPValue ) ) == 0;
	}

	template <typename TPValue>
	bool mem_cmp_not_equal( const TPValue & pFirst, const TPValue & pSecond )
	{
		return memcmp( &pFirst, &pSecond, sizeof( TPValue ) ) != 0;
	}

	template <typename TPValue>
	bool mem_cmp_less( const TPValue & pFirst, const TPValue & pSecond )
	{
		return memcmp( &pFirst, &pSecond, sizeof( TPValue ) ) < 0;
	}

	template <typename TPValue>
	bool mem_cmp_less_equal( const TPValue & pFirst, const TPValue & pSecond )
	{
		return memcmp( &pFirst, &pSecond, sizeof( TPValue ) ) <= 0;
	}

	template <typename TPValue>
	bool mem_cmp_greater( const TPValue & pFirst, const TPValue & pSecond )
	{
		return memcmp( &pFirst, &pSecond, sizeof( TPValue ) ) > 0;
	}

	template <typename TPValue>
	bool mem_cmp_greater_equal( const TPValue & pFirst, const TPValue & pSecond )
	{
		return memcmp( &pFirst, &pSecond, sizeof( TPValue ) ) >= 0;
	}

	///
	/// @param pMemory1
	/// @param pSize1
	/// @param pMemory2
	/// @param pSize2
	/// @return
	bool mem_check_region_overlap( const void * pMemory1, size_t pSize1, const void * pMemory2, size_t pSize2 );

	///
	/// @param pDst
	/// @param pDstpSize
	/// @param pSrc
	/// @param pCopySize
	void mem_copy_checked( void * pDst, size_t pDstpSize, const void * pSrc, size_t pCopySize );

	///
	/// @param pDst
	/// @param pDstpSize
	/// @param pSrc
	/// @param pCopySize
	void mem_copy_unchecked( void * pDst, size_t pDstpSize, const void * pSrc, size_t pCopySize );

	/// @brief
	void mem_set_fill_checked( void * pMemoryPtr, size_t pMemorySize, byte pFillValue, size_t pFillSize );

	/// @brief
	void mem_set_fill_unchecked( void * pMemoryPtr, size_t pMemorySize, byte pFillValue, size_t pFillSize );

	/// @brief
	/// @param pMemoryPtr
	/// @param pMemorySize
	/// @param pBaseOffset
	/// @param pCount
	/// @param pMoveOffset
	bool mem_move_checked( void * pMemoryPtr, size_t pMemorySize, size_t pBaseOffset, size_t pMoveSize, ptrdiff_t pMoveOffset );

	/// @brief
	/// @param pMemoryPtr
	/// @param pMemorySize
	/// @param pBaseOffset
	/// @param pCount
	/// @param pMoveOffset
	void mem_move_unchecked( void * pMemoryPtr, size_t pMemorySize, size_t pBaseOffset, size_t pMoveSize, ptrdiff_t pMoveOffset );

	/// @brief
	void mem_set_zero_checked( void * pMemoryPtr, size_t pMemorySize, size_t pZeroSize );

	/// @brief
	void mem_set_zero_unchecked( void * pMemoryPtr, size_t pMemorySize, size_t pZeroSize );

	/// @brief
	template <typename TPDst, typename TPSrc>
	inline void mem_copy( TPDst * pDst, size_t pCapacity, const TPSrc * pSrc, size_t pCopyCount )
	{
		const auto dstByteCapacity = memory_base_size<TPDst>::size * pCapacity;
		const auto copyByteSize = memory_base_size<TPSrc>::size * pCopyCount;
	#if( CPPX_USE_RUNTIME_CHECKED_MEMORY_ROUTINES )
		mem_copy_checked( pDst, dstByteCapacity, pSrc, copyByteSize );
	#else
		mem_copy_unchecked( pDst, dstByteCapacity, pSrc, copyByteSize );
	#endif
	}

	/// @brief
	template <typename TPDst, typename TPSrc>
	inline void mem_copy( TPDst & pDst, const TPSrc & pSrc )
	{
		static_assert( sizeof( TPDst ) >= sizeof( TPSrc ) );
		mem_copy( &pDst, 1, &pSrc, 1 );
	}

	/// @brief
	template <typename TPData>
	inline void mem_set_fill( TPData * pMemory, size_t pCapacity, byte pFillValue, size_t pFillCount )
	{
		const auto memoryByteCapacity = memory_base_size<TPData>::size * pCapacity;
		const auto fillByteSize = memory_base_size<TPData>::size * pFillCount;
	#if( CPPX_USE_RUNTIME_CHECKED_MEMORY_ROUTINES )
		mem_set_fill_checked( pMemory, memoryByteCapacity, pFillValue, fillByteSize );
	#else
		mem_set_fill_unchecked( pMemory, memoryByteCapacity, pFillValue, fillByteSize );
	#endif
	}

	/// @brief
	template <typename TPData>
	inline void mem_move( TPData * pMemory, size_t pCapacity, size_t pBaseOffset, size_t pCount, ptrdiff_t pMoveOffset )
	{
		constexpr auto elemSize = memory_base_size<TPData>::sSize;
	#if( CPPX_USE_RUNTIME_CHECKED_MEMORY_ROUTINES )
		mem_move_checked( pMemory, pCapacity * elemSize, pBaseOffset * elemSize, pCount * elemSize, pMoveOffset * elemSize );
	#else
		mem_move_unchecked( pMemory, pCapacity * elemSize, pBaseOffset * elemSize, pCount * elemSize, pMoveOffset * elemSize );
	#endif
	}

	/// @brief
	template <typename TPData>
	inline void mem_set_zero( TPData * pMemory, size_t pCapacity, size_t pZeroCount )
	{
		const auto memoryByteCapacity = memory_base_size<TPData>::sSize * pCapacity;
		const auto zeroByteSize = memory_base_size<TPData>::sSize * pZeroCount;
	#if( CPPX_USE_RUNTIME_CHECKED_MEMORY_ROUTINES )
		mem_set_zero_checked( pMemory, memoryByteCapacity, zeroByteSize );
	#else
		mem_set_zero_unchecked( pMemory, memoryByteCapacity, zeroByteSize );
	#endif
	}

	template <typename TPValue, size_t tpSize>
	inline void mem_set_zero( TPValue ( &pArray )[tpSize], size_t pZeroSize = sizeof( TPValue ) * tpSize )
	{
		pZeroSize = get_min_of( pZeroSize, sizeof( TPValue ) * tpSize );
		mem_set_zero_unchecked( &( pArray[0] ), sizeof( TPValue ) * tpSize, pZeroSize );
	}

	template <typename TPValue, size_t tpSize>
	inline void mem_set_zero( std::array<TPValue, tpSize> & pArray, size_t pZeroSize = sizeof( TPValue ) * tpSize )
	{
		pZeroSize = get_min_of( pZeroSize, sizeof( TPValue ) * tpSize );
		mem_set_zero_unchecked( &pArray, sizeof( TPValue ) * tpSize, pZeroSize );
	}

	template <typename TPValue>
	inline void mem_set_zero( std::vector<TPValue> & pVector, size_t pZeroOffset = 0, size_t pZeroCount = cve::max_size )
	{
		const auto vectorSize = pVector.size();
		pZeroCount = get_min_of( pZeroCount, vectorSize );
		pZeroOffset = get_min_of( pZeroOffset, vectorSize - pZeroCount );
		mem_set_zero_unchecked( pVector.data() + pZeroOffset, sizeof( TPValue ) * vectorSize, sizeof( TPValue ) * pZeroCount );
	}

	template <typename TPValue>
	inline void mem_set_zero( TPValue & pObject, size_t pZeroSize = sizeof( TPValue ) )
	{
		pZeroSize = get_min_of( pZeroSize, sizeof( TPValue ) );
		mem_set_zero_unchecked( &pObject, sizeof( TPValue ), pZeroSize );
	}

}
