
#ifndef __CPPX_STATIC_VECTOR_H__
#define __CPPX_STATIC_VECTOR_H__

#include "arrayIterator.h"
#include "arrayView.h"
#include "memoryBuffer.h"

namespace cppx
{

	template <typename TPValue>
	class static_storage_array_adapter
	{
	public:
		using iterator = array_iterator<TPValue, false>;
		using const_iterator = array_iterator<const TPValue, false>;

		using reverse_iterator = array_iterator<TPValue, true>;
		using const_reverse_iterator = array_iterator<const TPValue, true>;

		static_storage_array_adapter() = default;
		~static_storage_array_adapter() = default;

		CPPX_ATTR_NO_DISCARD TPValue & operator[]( size_t pIndex ) noexcept
		{
			cppx_debug_assert( _mBasePtr && ( pIndex < _mElementsNum ) );
			return _mBasePtr[pIndex];
		}

		CPPX_ATTR_NO_DISCARD const TPValue & operator[]( size_t pIndex ) const noexcept
		{
			cppx_debug_assert( _mBasePtr && ( pIndex < _mElementsNum ) );
			return _mBasePtr[pIndex];
		}

		CPPX_ATTR_NO_DISCARD bool hasFreeSpace( size_t pElementsNum = 1 ) const noexcept
		{
			const auto freeSpaceSize = getFreeSpaceSize();
			return freeSpaceSize >= pElementsNum;
		}

		CPPX_ATTR_NO_DISCARD size_t getFreeSpaceSize() const noexcept
		{
			return ( _mCapacity > _mElementsNum ) ? ( _mCapacity - _mElementsNum ) : 0u;
		}

		CPPX_ATTR_NO_DISCARD size_t size() const noexcept
		{
			return _mElementsNum;
		}

		CPPX_ATTR_NO_DISCARD size_t capacity() const noexcept
		{
			return _mCapacity;
		}

		void push_back( TPValue && pValue )
		{
			_insert( _mElementsNum, std::move( pValue ) );
		}

		void _push_back( const TPValue & pValue )
		{
			_insert( _mElementsNum, pValue );
		}

		void push_front( TPValue && pValue )
		{
			_insert( 0, std::move( pValue ) );
		}

		void push_front( const TPValue & pValue )
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
		TPValue & emplace_back( TPArgs && ...pArgs )
		{
		}

		template <typename... TPArgs>
		TPValue & emplace_front( TPArgs && ...pArgs )
		{
		}

		void set_storage( TPValue * pBasePtr, size_t pCapacity, size_t pElementsNum )
		{
			_mBasePtr = pBasePtr;
			_mCapacity = pCapacity;
			_mElementsNum = pElementsNum;
		}

		void reset_storage()
		{
			_mBasePtr = nullptr;
			_mCapacity = 0;
			_mElementsNum = 0;
		}

	private:
		void _move_elements( size_t pFirstElement, size_t pNumElementsToMove, native_int pMoveOffset )
		{
			cppx_debug_assert( pFirstElement < _mElementsNum );

			if( ( pNumElementsToMove == 0 ) || ( pMoveOffset == 0 ) )
			{
				return;
			}

			cppx_debug_only_code(
			{
				const auto maxMoveSize = _mElementsNum - pFirstElement;
				cppx_debug_assert( pNumElementsToMove <= maxMoveSize );

				if( pMoveOffset > 0 )
				{
					const auto maxMoveOffset = getFreeSpaceSize();
					cppx_debug_assert( pMoveOffset <= maxMoveOffset );
				}
				else
				{
					const auto maxMoveOffset = -pFirstElement;
					cppx_debug_assert( pMoveOffset >= maxMoveOffset );
				}
			});

			const auto memoryCapacity = capacity();
			cppx::mem_move_checked( _mBasePtr,
			                      memoryCapacity * sizeof( TPValue ),
			                      pFirstElement * sizeof( TPValue ),
			                      pNumElementsToMove * sizeof( TPValue ),
			                      pMoveOffset * sizeof( TPValue ) );
		}

		template <typename TPInput>
		TPValue * _insert( size_t pInsertPosition, TPInput pInput )
		{
			if( !hasFreeSpace() )
			{
				cppx_debug_interrupt();
				throw 0;
			}

			if( pInsertPosition < _mElementsNum )
			{
				const auto moveCount = _mElementsNum - pInsertPosition;
				_move_elements( pInsertPosition, moveCount, 1 );
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
		: _currentStorageSize( tpSSOElementsNum )
		{}

		~DynamicArraySSO() = default;

		CPPX_ATTR_NO_DISCARD size_t freeSpace() const noexcept
		{
			return _storageAdapter.getFreeSpaceSize();
		}

		void _push_back( TPValue && pValue )
		{
			;
		}

		void _push_back( const TPValue & pValue )
		{
			;
		}

		void push_front( TPValue && pValue )
		{
			;
		}

		void push_front( const TPValue & pValue )
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
		TPValue & emplace_back( TPArgs && ...pArgs )
		{
		}

		template <typename... TPArgs>
		TPValue & emplace_front( TPArgs && ...pArgs )
		{
		}

	private:
		void _allocateSpace( size_t pElements )
		{
			const auto freeSpace = _storageAdapter.getFreeSpaceSize();

			if( freeSpace < pElements )
			{
				if( _currentStorageSize == sFixedStorageCapacity )
				{
				}
			}
		}

		void _allocateDynamicStorage( size_t pStorageSize )
		{
			if( ( pStorageSize == 0 ) || ( pStorageSize <= _currentStorageSize ) )
			{
				pStorageSize = _currentStorageSize * 2;
			}
		}

	private:
		using FixedStorage = fixed_memory_buffer<sFixedStorageSizeInBytes>;
		using DynamicStorage = dynamic_memory_buffer;

		FixedStorage _fixedStorage;
		DynamicStorage _dynamicStorage;
		static_storage_array_adapter<TPValue> _storageAdapter;
		size_t _currentStorageSize;
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

#endif /* __CPPX_STATIC_VECTOR_H__ */
