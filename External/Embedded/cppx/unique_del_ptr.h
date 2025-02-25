
#ifndef __CPPX_UNIQUE_DEL_PTR_H__
#define __CPPX_UNIQUE_DEL_PTR_H__

#include "memory.h"

namespace cppx
{

	template <typename TPData>
	struct unique_del_ptr_traits
	{
		using value_type = TPData;
		using pointer_type = TPData *;
		using del_callback_type = std::function<void( TPData * )>;

		static void del_proxy_func( pointer_type pPointer, const del_callback_type & pDelCallback )
		{
			if( pDelCallback )
			{
				pDelCallback( pPointer );
			}
			else
			{
				delete pPointer;
			}
		}
	};

	template <>
	struct unique_del_ptr_traits<void>
	{
		using value_type = void;
		using pointer_type = void *;
		using del_callback_type = std::function<void( void * )>;

		static void del_proxy_func( pointer_type pPointer, const del_callback_type & pDelCallback )
		{
			if( pDelCallback )
			{
				pDelCallback( pPointer );
			}
		}
	};

	template <typename TPData>
	class unique_del_ptr
	{
	public:
		using traits_type = unique_del_ptr_traits<TPData>;
		using value_type = typename traits_type::value_type;
		using pointer_type = typename traits_type::pointer_type;
		using del_callback_type = typename traits_type::del_callback_type;

		unique_del_ptr( const unique_del_ptr & ) = delete;
		unique_del_ptr & operator=( const unique_del_ptr & ) = delete;

		unique_del_ptr() noexcept
		: _dataPtr( nullptr )
		{}

		unique_del_ptr( std::nullptr_t ) noexcept
		: _dataPtr( nullptr )
		{}

		unique_del_ptr( pointer_type pData, del_callback_type pDelCallback = {} ) noexcept
		: _dataPtr( pData )
		{}

		pointer_type get() const noexcept
		{
			return _dataPtr;
		}

		~unique_del_ptr() noexcept
		{
			if( _dataPtr )
			{
				traits_type::del_proxy_func( _dataPtr, _delCallback );
				_dataPtr = nullptr;
			}
		}

	private:
		pointer_type _dataPtr;
		del_callback_type _delCallback;
	};

}

#endif // __CPPX_UNIQUE_DEL_PTR_H__
