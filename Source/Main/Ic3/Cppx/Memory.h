
#pragma once

#include "Range.h"
#include "Utilities.h"
#include <functional>

#define IC3_USE_RUNTIME_CHECKED_MEMORY_ROUTINES 0

namespace Ic3::Cppx
{

	using MemoryAllocCallback = std::function<void *( size_t )>;
	using MemoryFreeCallback = std::function<void( void * )>;
	using MemoryReallocCallback = std::function<void *( void *, size_t )>;

	struct MemoryAllocationProxy
	{
	public:
		uint32 mMemoryAlignment = cxMemoryCPUDefaultAlignment;
		MemoryAllocCallback mFnAlloc;
		MemoryFreeCallback mFnFree;
		MemoryReallocCallback mFnRealloc;

	public:
		explicit operator bool() const
		{
			return mFnAlloc && mFnFree && mFnRealloc;
		}

		void swap( MemoryAllocationProxy & pOther )
		{
			std::swap( mMemoryAlignment, pOther.mMemoryAlignment );
			std::swap( mFnAlloc, pOther.mFnAlloc );
			std::swap( mFnFree, pOther.mFnFree );
			std::swap( mFnRealloc, pOther.mFnRealloc );
		}
	};

	inline void swap( MemoryAllocationProxy & pFirst, MemoryAllocationProxy & pSecond )
	{
		pFirst.swap( pSecond );
	}

	extern const MemoryAllocationProxy cvDefaultMemoryAllocationProxy;

	template <typename TPValue>
	struct MemoryBaseSize
	{
		static constexpr size_t sSize = sizeof( TPValue );
	};

	template <>
	struct MemoryBaseSize<void>
	{
		static constexpr size_t sSize = 1;
	};

	template <typename TSize, typename TOffset = TSize>
	struct AlignedMemoryAllocInfo
	{
		TRegion<TSize, TOffset> mAccessibleRegion;
		TRegion<TSize, TOffset> mReservedRegion;
	};

	inline constexpr uint64 memCheckRequestedCopySize( uint64 pBufferSize, uint64 pCopySize, uint64 pCopyOffset )
	{
		const auto copyOffset = getMinOf( pCopyOffset, pBufferSize );
		const auto maxCopySize = pBufferSize - copyOffset;
		const auto copySize = getMinOf( pCopySize, maxCopySize );
		return copySize;
	}

	///
	/// @tparam TPValue
	/// @param pValue
	/// @param pAlignment
	/// @return
	template <typename TPValue>
	inline constexpr TPValue memGetAlignedPowerOf2( TPValue pValue, uint32 pAlignment )
	{
		return static_cast<TPValue>( ( static_cast<uint64>( pValue ) + pAlignment ) & ( ~( static_cast<uint64>( pAlignment ) - 1 ) ) );
	}

	///
	/// @tparam TPValue
	/// @param pValue
	/// @param pAlignment
	/// @return
	template <typename TPValue>
	inline TPValue memGetAlignedValue( TPValue pValue, uint32 pAlignment )
	{
		const auto valueAlignmentMod = pValue % pAlignment;
		return ( valueAlignmentMod != 0 ) ? ( pValue + pAlignment - valueAlignmentMod ) : pValue;
	}

	template <typename TSize, typename TOffset = TSize, typename std::enable_if<std::is_integral<TSize>::value, int>::type = 0>
	inline AlignedMemoryAllocInfo<TSize, TOffset> memComputeAlignedAllocationInfo( TOffset pBaseAddress, TSize pAllocationSize, uint32 pAlignment )
	{
		const auto alignedOffset = memGetAlignedValue( pBaseAddress, pAlignment );

		AlignedMemoryAllocInfo<TSize, TOffset> allocInfo;
		allocInfo.mAccessibleRegion.offset = alignedOffset;
		allocInfo.mAccessibleRegion.size = pAllocationSize;
		allocInfo.mReservedRegion.offset = pBaseAddress;
		allocInfo.mReservedRegion.size = ( alignedOffset - pBaseAddress ) + pAllocationSize;

		return allocInfo;
	}

	template <typename TPValue>
	bool memCompareEqual( const TPValue & pFirst, const TPValue & pSecond )
	{
		return memcmp( &pFirst, &pSecond, sizeof( TPValue ) ) == 0;
	}

	template <typename TPValue>
	bool memCompareNotEqual( const TPValue & pFirst, const TPValue & pSecond )
	{
		return memcmp( &pFirst, &pSecond, sizeof( TPValue ) ) != 0;
	}

	///
	/// @param pMemory1
	/// @param pSize1
	/// @param pMemory2
	/// @param pSize2
	/// @return
	bool memCheckMemoryOverlap( const void * pMemory1, size_t pSize1, const void * pMemory2, size_t pSize2 );

	///
	/// @param pDst
	/// @param pDstpSize
	/// @param pSrc
	/// @param pCopySize
	void memCopyChecked( void * pDst, size_t pDstpSize, const void * pSrc, size_t pCopySize );

	///
	/// @param pDst
	/// @param pDstpSize
	/// @param pSrc
	/// @param pCopySize
	void memCopyUnchecked( void * pDst, size_t pDstpSize, const void * pSrc, size_t pCopySize );

	/// @brief
	void memFillChecked( void * pMemoryPtr, size_t pMemorySize, byte pFillValue, size_t pFillSize );

	/// @brief
	void memFillUnchecked( void * pMemoryPtr, size_t pMemorySize, byte pFillValue, size_t pFillSize );

	/// @brief
	/// @param pMemoryPtr
	/// @param pMemorySize
	/// @param pBaseOffset
	/// @param pCount
	/// @param pMoveOffset
	void memMoveChecked( void * pMemoryPtr, size_t pMemorySize, size_t pBaseOffset, size_t pMoveSize, ptrdiff_t pMoveOffset );

	/// @brief
	/// @param pMemoryPtr
	/// @param pMemorySize
	/// @param pBaseOffset
	/// @param pCount
	/// @param pMoveOffset
	void memMoveUnchecked( void * pMemoryPtr, size_t pMemorySize, size_t pBaseOffset, size_t pMoveSize, ptrdiff_t pMoveOffset );

	/// @brief
	void memZeroChecked( void * pMemoryPtr, size_t pMemorySize, size_t pZeroSize );

	/// @brief
	void memZeroUnchecked( void * pMemoryPtr, size_t pMemorySize, size_t pZeroSize );

	/// @brief
	template <typename TPDst, typename TPSrc>
	inline void memCopy( TPDst * pDst, size_t pCapacity, const TPSrc * pSrc, size_t pCopyCount )
	{
		const auto dstByteCapacity = MemoryBaseSize<TPDst>::size * pCapacity;
		const auto copyByteSize = MemoryBaseSize<TPSrc>::size * pCopyCount;
	#if( IC3_USE_RUNTIME_CHECKED_MEMORY_ROUTINES )
		memCopyChecked( pDst, dstByteCapacity, pSrc, copyByteSize );
	#else
		memCopyUnchecked( pDst, dstByteCapacity, pSrc, copyByteSize );
	#endif
	}

	/// @brief
	template <typename TPDst, typename TPSrc>
	inline void memCopy( TPDst & pDst, const TPSrc & pSrc )
	{
		static_assert( sizeof( TPDst ) >= sizeof( TPSrc ) );
		memCopy( &pDst, 1, &pSrc, 1 );
	}

	/// @brief
	template <typename TPData>
	inline void memFill( TPData * pMemory, size_t pCapacity, byte pFillValue, size_t pFillCount )
	{
		const auto memoryByteCapacity = MemoryBaseSize<TPData>::size * pCapacity;
		const auto fillByteSize = MemoryBaseSize<TPData>::size * pFillCount;
	#if( IC3_USE_RUNTIME_CHECKED_MEMORY_ROUTINES )
		memFillChecked( pMemory, memoryByteCapacity, pFillValue, fillByteSize );
	#else
		memFillUnchecked( pMemory, memoryByteCapacity, pFillValue, fillByteSize );
	#endif
	}

	/// @brief
	template <typename TPData>
	inline void memMove( TPData * pMemory, size_t pCapacity, size_t pBaseOffset, size_t pCount, ptrdiff_t pMoveOffset )
	{
		constexpr auto elemSize = MemoryBaseSize<TPData>::size;
	#if( IC3_USE_RUNTIME_CHECKED_MEMORY_ROUTINES )
		memMoveChecked( pMemory, pCapacity * elemSize, pBaseOffset * elemSize, pCount * elemSize, pMoveOffset * elemSize );
	#else
		memMoveUnchecked( pMemory, pCapacity * elemSize, pBaseOffset * elemSize, pCount * elemSize, pMoveOffset * elemSize );
	#endif
	}

	/// @brief
	template <typename TPData>
	inline void memZero( TPData * pMemory, size_t pCapacity, size_t pZeroCount )
	{
		const auto memoryByteCapacity = MemoryBaseSize<TPData>::size * pCapacity;
		const auto zeroByteSize = MemoryBaseSize<TPData>::size * pZeroCount;
	#if( IC3_USE_RUNTIME_CHECKED_MEMORY_ROUTINES )
		memZeroChecked( pMemory, memoryByteCapacity, zeroByteSize );
	#else
		memZeroUnchecked( pMemory, memoryByteCapacity, zeroByteSize );
	#endif
	}

	template <typename TPValue, size_t tpSize>
	inline void memZero( TPValue ( &pArray )[tpSize], size_t pZeroSize = sizeof( TPValue ) * tpSize )
	{
		pZeroSize = getMinOf( pZeroSize, sizeof( TPValue ) * tpSize );
		memZeroUnchecked( &( pArray[0] ), sizeof( TPValue ) * tpSize, pZeroSize );
	}

	template <typename TPValue, size_t tpSize>
	inline void memZero( std::array<TPValue, tpSize> & pArray, size_t pZeroSize = sizeof( TPValue ) * tpSize )
	{
		pZeroSize = getMinOf( pZeroSize, sizeof( TPValue ) * tpSize );
		memZeroUnchecked( &pArray, sizeof( TPValue ) * tpSize, pZeroSize );
	}

	template <typename TPValue>
	inline void memZero( std::vector<TPValue> & pVector, size_t pZeroOffset = 0, size_t pZeroCount = cxMaxSize )
	{
		const auto vectorSize = pVector.size();
		pZeroCount = getMinOf( pZeroCount, vectorSize );
		pZeroOffset = getMinOf( pZeroOffset, vectorSize - pZeroCount );
		memZeroUnchecked( pVector.data() + pZeroOffset, sizeof( TPValue ) * vectorSize, sizeof( TPValue ) * pZeroCount );
	}

	template <typename TPValue>
	inline void memZero( TPValue & pObject, size_t pZeroSize = sizeof( TPValue ) )
	{
		pZeroSize = getMinOf( pZeroSize, sizeof( TPValue ) );
		memZeroUnchecked( &pObject, sizeof( TPValue ), pZeroSize );
	}

}
