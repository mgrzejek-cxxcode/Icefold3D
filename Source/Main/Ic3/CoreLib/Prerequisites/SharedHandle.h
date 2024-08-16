
#pragma once

#ifndef __IC3_CORELIB_SHARED_HANDLE_H__
#define __IC3_CORELIB_SHARED_HANDLE_H__

#include <Ic3/Cppx/Aligned.h>
#include <Ic3/Cppx/RefCounter.h>

namespace Ic3
{

	struct SharedHandleRefCounterData
	{
	public:
		using ref_counter_value_t = Cppx::ref_counter_value_t;

		Cppx::AtomicRefCounter activeRefs;
		Cppx::AtomicRefCounter weakRefs;

	public:
		ref_counter_value_t addActiveRef();
		ref_counter_value_t releaseActiveRef();
	};

	template <typename T>
	struct SharedHandleControlBlock : public SharedHandleRefCounterData
	{
	public:
		T * ptr;

	public:
		explicit SharedHandleControlBlock( T * pPtr = nullptr )
		: ptr( pPtr )
		{}
	};

	template <typename T>
	struct SharedHandleControlBlockWithStorage : public SharedHandleControlBlock<T>
	{
	public:
		using Storage = typename Cppx::AlignedStorage<T>;
		Storage storage;

	public:
		SharedHandleControlBlockWithStorage()
		: SharedHandleControlBlock<T>( reinterpret_cast<T *>( storage.ptr() ) )
		{}
	};

	template <typename T>
	class SharedHandle
	{
	private:
		T * _objectPtr;
		SharedHandleRefCounterData * _cblock;
	};

}

#endif // __IC3_CORELIB_SHARED_HANDLE_H__
