
#ifndef __IC3_CPPX_STATIC_VECTOR_H__
#define __IC3_CPPX_STATIC_VECTOR_H__

#include "ArrayIterator.h"
#include "ArrayView.h"
#include "MemoryBuffer.h"
#include <Ic3/Cppx/Memory.h>

namespace Ic3::Cppx
{

	template <typename TPValue>
	class StaticStorageArrayAdapter
	{
	public:
		using iterator = ArrayIterator<TPValue, false>;
		using const_iterator = ArrayIterator<const TPValue, false>;

		using reverse_iterator = ArrayIterator<TPValue, true>;
		using const_reverse_iterator = ArrayIterator<const TPValue, true>;

		StaticStorageArrayAdapter() = default;
		~StaticStorageArrayAdapter() = default;

		IC3_ATTR_NO_DISCARD TPValue & operator[]( size_t pIndex ) noexcept
		{
			ic3DebugAssert( _mBasePtr && ( pIndex < _mElementsNum ) );
			return _mBasePtr[pIndex];
		}

		IC3_ATTR_NO_DISCARD const TPValue & operator[]( size_t pIndex ) const noexcept
		{
			ic3DebugAssert( _mBasePtr && ( pIndex < _mElementsNum ) );
			return _mBasePtr[pIndex];
		}

		IC3_ATTR_NO_DISCARD bool hasFreeSpace( size_t pElementsNum = 1 ) const noexcept
		{
			const auto freeSpaceSize = getFreeSpaceSize();
			return freeSpaceSize >= pElementsNum;
		}

		IC3_ATTR_NO_DISCARD size_t getFreeSpaceSize() const noexcept
		{
			return ( _mCapacity > _mElementsNum ) ? ( _mCapacity - _mElementsNum ) : 0u;
		}

		IC3_ATTR_NO_DISCARD size_t size() const noexcept
		{
			return _mElementsNum;
		}

		IC3_ATTR_NO_DISCARD size_t capacity() const noexcept
		{
			return _mCapacity;
		}

		void pushBack( TPValue && pValue )
		{
			_insert( _mElementsNum, std::move( pValue ) );
		}

		void pushBack( const TPValue & pValue )
		{
			_insert( _mElementsNum, pValue );
		}

		void pushFront( TPValue && pValue )
		{
			_insert( 0, std::move( pValue ) );
		}

		void pushFront( const TPValue & pValue )
		{
			_insert( _mElementsNum, pValue );
		}

		void insert( size_t pInsertPosition, TPValue && pValue )
		{
			_insert( pInsertPosition, std::move( pValue ) );
		}

		void insert( size_t pInsertPosition, const TPValue & pValue )
		{
			_insert( pInsertPosition, pValue );
		}

		template <typename... TPArgs>
		TPValue & emplaceBack( TPArgs && ...pArgs )
		{
		}

		template <typename... TPArgs>
		TPValue & emplaceFront( TPArgs && ...pArgs )
		{
		}

		void setStorage( TPValue * pBasePtr, size_t pCapacity, size_t pElementsNum )
		{
			_mBasePtr = pBasePtr;
			_mCapacity = pCapacity;
			_mElementsNum = pElementsNum;
		}

		void resetStorage()
		{
			_mBasePtr = nullptr;
			_mCapacity = 0;
			_mElementsNum = 0;
		}

	private:
		void _moveElements( size_t pFirstElement, size_t pNumElementsToMove, native_int pMoveOffset )
		{
			ic3DebugAssert( pFirstElement < _mElementsNum );

			if( ( pNumElementsToMove == 0 ) || ( pMoveOffset == 0 ) )
			{
				return;
			}

			ic3DebugOnlyCode(
			{
				const auto maxMoveSize = _mElementsNum - pFirstElement;
				ic3DebugAssert( pNumElementsToMove <= maxMoveSize );

				if( pMoveOffset > 0 )
				{
					const auto maxMoveOffset = getFreeSpaceSize();
					ic3DebugAssert( pMoveOffset <= maxMoveOffset );
				}
				else
				{
					const auto maxMoveOffset = -pFirstElement;
					ic3DebugAssert( pMoveOffset >= maxMoveOffset );
				}
			});

			const auto memoryCapacity = capacity();
			Cppx::memMoveChecked( _mBasePtr, memoryCapacity, pFirstElement, pNumElementsToMove, pMoveOffset );
		}

		template <typename TPInput>
		TPValue * _insert( size_t pInsertPosition, TPInput pInput )
		{
			if( !hasFreeSpace() )
			{
				ic3DebugInterrupt();
				throw 0;
			}

			if( pInsertPosition < _mElementsNum )
			{
				const auto moveCount = _mElementsNum - pInsertPosition;
				_moveElements( pInsertPosition, moveCount, 1 );
			}

			auto * newElementMemoryPtr = _mBasePtr + pInsertPosition;
			new ( newElementMemoryPtr ) TPValue( std::forward<TPInput>( pInput ) );

			++_mElementsNum;

			return newElementMemoryPtr;
		}

	private:
		TPValue * _mBasePtr = nullptr;
		size_t _mCapacity = 0;
		size_t _mElementsNum = 0;
	};

	template <typename TPValue, size_t tpSSOElementsNum>
	class DynamicArraySSO
	{
	public:
		static constexpr size_t sElementSize = sizeof( TPValue );

		static constexpr size_t sFixedStorageCapacity = tpSSOElementsNum;

		static constexpr size_t sFixedStorageSizeInBytes = sElementSize * sFixedStorageCapacity;

		DynamicArraySSO()
		: _mCurrentStorageSize( tpSSOElementsNum )
		{}

		~DynamicArraySSO() = default;

		IC3_ATTR_NO_DISCARD size_t freeSpace() const noexcept
		{
			return _mStorageAdapter.getFreeSpaceSize();
		}

		void pushBack( TPValue && pValue )
		{
			;
		}

		void pushBack( const TPValue & pValue )
		{
			;
		}

		void pushFront( TPValue && pValue )
		{
			;
		}

		void pushFront( const TPValue & pValue )
		{
			;
		}

		void insert( size_t pInsertPosition, TPValue && pValue )
		{
			;
		}

		void insert( size_t pInsertPosition, const TPValue & pValue )
		{
			;
		}

		template <typename... TPArgs>
		TPValue & emplaceBack( TPArgs && ...pArgs )
		{
		}

		template <typename... TPArgs>
		TPValue & emplaceFront( TPArgs && ...pArgs )
		{
		}

	private:
		void _allocateSpace( size_t pElements )
		{
			const auto freeSpace = _mStorageAdapter.getFreeSpaceSize();

			if( freeSpace < pElements )
			{
				if( _mCurrentStorageSize == sFixedStorageCapacity )
				{
				}
			}
		}

		void _allocateDynamicStorage( size_t pStorageSize )
		{
			if( ( pStorageSize == 0 ) || ( pStorageSize <= _mCurrentStorageSize ) )
			{
				pStorageSize = _mCurrentStorageSize * 2;
			}
		}

	private:
		using FixedStorage = FixedMemoryBuffer<sFixedStorageSizeInBytes>;
		using DynamicStorage = DynamicMemoryBuffer;

		FixedStorage _mFixedStorage;
		DynamicStorage _mDynamicStorage;
		StaticStorageArrayAdapter<TPValue> _mStorageAdapter;
		size_t _mCurrentStorageSize;
	};
	
	struct STest
	{
		DynamicArraySSO<int, 0> S;
	};

	template <typename TPValue, size_t tpMaxCapacity>
	class StaticVector
	{
	public:
		StaticVector() = default;

		StaticVector( StaticVector && pSrcObject )
		{}

		StaticVector( const StaticVector & pSrcObject )
		{}

		StaticVector & operator=( StaticVector && pRhs )
		{}

		StaticVector & operator=( const StaticVector & pRhs )
		{}

	private:
		using Storage = typename std::aligned_storage<sizeof( TPValue ), alignof( TPValue )>::type;
		Storage _data[tpMaxCapacity];
		size_t _beginIndex = tpMaxCapacity;
		size_t _endIndex = tpMaxCapacity;
	};

}

#endif /* __IC3_CPPX_STATIC_VECTOR_H__ */
