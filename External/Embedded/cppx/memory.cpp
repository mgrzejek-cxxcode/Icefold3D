
#include "Memory.h"

namespace cppx
{

	namespace cve
	{

		static memory_routines_proxy get_default_memory_routines() noexcept
		{
			memory_routines_proxy defaultMemoryRoutines;
			try
			{
				defaultMemoryRoutines.fn_alloc = []( size_t pSize ) -> void * {
					return std::malloc( pSize );
				};
				defaultMemoryRoutines.fn_free = []( void * pMemory ) -> void {
					return std::free( pMemory );
				};
				defaultMemoryRoutines.fn_realloc = []( void * pMemory, size_t pNewSize ) -> void * {
					return std::realloc( pMemory, pNewSize );
				};
				defaultMemoryRoutines.mem_alignment = PCL_MEMORY_BASE_ALIGNMENT;
			}
			catch( ... )
			{
				cppx_debug_interrupt();
			}
			return defaultMemoryRoutines;
		}

		const memory_routines_proxy default_memory_routines = get_default_memory_routines();

	}


	bool mem_check_region_overlap( const void * pMemory1, size_t pSize1, const void * pMemory2, size_t pSize2 )
	{
		// For two different regions with at least one being of size 0, there can be no overlap.
		if( ( pSize1 == 0 ) || ( pSize2 == 0 ) )
		{
			return false;
		}
		// If pointers are the same, we treat it as an overlap, no matter the size.
		if( pMemory1 == pMemory2 )
		{
			return true;
		}
		// pMemory2 is within <pMemory1Start, pMemory1End> -> overlap.
		if( ( pMemory2 > pMemory1 ) && ( pMemory2 <= reinterpret_cast<const char *>( pMemory1 ) + pSize1 ) )
		{
			return true;
		}
		// pMemory1 is within <pMemory2Start, pMemory2End> -> overlap.
		if( ( pMemory1 > pMemory2 ) && ( pMemory1 <= reinterpret_cast<const char *>( pMemory2 ) + pSize2 ) )
		{
			return true;
		}

		return false;
	}

	void mem_copy_checked( void * pDst, size_t pDstSize, const void * pSrc, size_t pCopySize )
	{
		// For zero copy size, do nothing and just return.
		if( pCopySize == 0 )
		{
			return;
		}
		// Target must not be NULL and have a size greater than zero.
		if( ( pDst == nullptr ) || ( pDstSize == 0u ) )
		{
			throw 0;
		}
		// Check if the data to be copied fits into buffer to prevent writing in someone else's memory.
		if( pCopySize > pDstSize )
		{
			throw 0;
		}
		// For memory copying, regions may not overlap.
		if( mem_check_region_overlap( pDst, pCopySize, pSrc, pCopySize ) )
		{
			throw 0;
		}
		PCL_COPY_MEMORY( pDst, pSrc, pCopySize );
	}

	void mem_copy_unchecked( void * pDst, size_t pDstSize, const void * pSrc, size_t pCopySize )
	{
		cppx_debug_assert( ( pDst != nullptr ) && ( pDstSize > 0 ) );
		cppx_debug_assert( pCopySize <= pDstSize );
		cppx_debug_assert( !mem_check_region_overlap( pDst, pCopySize, pSrc, pCopySize ) );
		PCL_COPY_MEMORY( pDst, pSrc, pCopySize );
	}

	void mem_set_fill_checked( void * pMemoryPtr, size_t pMemorySize, byte pFillValue, size_t pFillSize )
	{
		if( ( pMemoryPtr == nullptr ) && ( pMemorySize == 0 ) )
		{
			throw 0;
		}
		if( pFillSize > pMemorySize )
		{
			throw 0;
		}
		PCL_FILL_MEMORY( pMemoryPtr, pFillSize, pFillValue );
	}

	void mem_set_fill_unchecked( void * pMemoryPtr, size_t pMemorySize, byte pFillValue, size_t pFillSize )
	{
		cppx_debug_assert( ( pMemoryPtr != nullptr ) && ( pMemorySize > 0 ) );
		cppx_debug_assert( pFillSize <= pMemorySize );
		PCL_FILL_MEMORY( pMemoryPtr, pFillSize, pFillValue );
	}

	bool mem_move_checked( void * pMemoryPtr, size_t pMemorySize, size_t pBaseOffset, size_t pMoveSize, ptrdiff_t pMoveOffset )
	{
		// Memory is assumed to be a non-null pointer with a pointed area of size greater than zero.
		if( ( pMemoryPtr == nullptr ) && ( pMemorySize == 0 ) )
		{
			return false;
		}
		// We are moving pCount elements from pBaseOffset by pMoveOffset. Check if the memory block is large enough.
		if( pBaseOffset + pMoveOffset + pMoveSize > pMemorySize )
		{
			return false;
		}
		// Source pointer: base memory pointer + base offset
		auto * const srcMemoryPtr = reinterpret_cast<char *>( pMemoryPtr ) + pBaseOffset;
		// Destination pointer: source pointer + number of elements to move the block by.
		auto * const dstMemoryPtr = srcMemoryPtr + pMoveOffset;
		// Do the actual move.
		PCL_MOVE_MEMORY( dstMemoryPtr, srcMemoryPtr, pMoveSize );

		return true;
	}

	void mem_move_unchecked( void * pMemoryPtr, size_t pMemorySize, size_t pBaseOffset, size_t pMoveSize, ptrdiff_t pMoveOffset )
	{
		cppx_debug_assert( ( pMemoryPtr != nullptr ) && ( pMemorySize > 0 ) );
		cppx_debug_assert( pBaseOffset + pMoveOffset + pMoveSize <= pMemorySize );
		auto * const srcMemoryPtr = reinterpret_cast<char *>( pMemoryPtr ) + pBaseOffset;
		auto * const dstMemoryPtr = srcMemoryPtr + pMoveOffset;
		PCL_MOVE_MEMORY( dstMemoryPtr, srcMemoryPtr, pMoveSize );
	}

	void mem_set_zero_checked( void * pMemoryPtr, size_t pMemorySize, size_t pZeroSize )
	{
		if( ( pMemoryPtr == nullptr ) && ( pMemorySize == 0 ) )
		{
			throw 0;
		}
		if( pZeroSize > pMemorySize )
		{
			throw 0;
		}
		PCL_ZERO_MEMORY( pMemoryPtr, pZeroSize );
	}

	void mem_set_zero_unchecked( void * pMemoryPtr, size_t pMemorySize, size_t pZeroSize )
	{
		cppx_debug_assert( ( pMemoryPtr != nullptr ) && ( pMemorySize > 0 ) );
		cppx_debug_assert( pZeroSize <= pMemorySize );
		PCL_ZERO_MEMORY( pMemoryPtr, pZeroSize );
	}

}
