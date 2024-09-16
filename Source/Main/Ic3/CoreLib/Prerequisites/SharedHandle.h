
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

	template <typename TPClass>
	struct SharedHandleControlBlock : public SharedHandleRefCounterData
	{
	public:
		TPClass * ptr;

	public:
		explicit SharedHandleControlBlock( TPClass * pPtr = nullptr )
		: ptr( pPtr )
		{}
	};

	template <typename TPClass>
	struct SharedHandleControlBlockWithStorage : public SharedHandleControlBlock<TPClass>
	{
	public:
		using Storage = typename Cppx::TAlignedStorage<TPClass>;
		Storage storage;

	public:
		SharedHandleControlBlockWithStorage()
		: SharedHandleControlBlock<TPClass>( reinterpret_cast<TPClass *>( storage.ptr() ) )
		{}
	};

	template <typename TPClass>
	class SharedHandle
	{
	private:
		TPClass * _objectPtr;
		SharedHandleControlBlock<TPClass> * _cblock;
	};

}

#endif // __IC3_CORELIB_SHARED_HANDLE_H__
