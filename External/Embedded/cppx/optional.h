
#ifndef __CPPX_OPTIONAL_H__
#define __CPPX_OPTIONAL_H__

#include "staticAlgo.h"

namespace cppx
{

	template <typename TPValue>
	struct OptionalStorage
	{
		static constexpr size_t sSize = sizeof( TPValue );
		static constexpr size_t sAlignment = alignof( TPValue );

		using Type = typename std::aligned_storage<sSize, sAlignment>::type;
	};

	template <typename TPValue>
	class Optional
	{
	public:
		static constexpr size_t sStorageSize = OptionalStorage<TPValue>::sSize;
		static constexpr size_t sStorageAlignment = OptionalStorage<TPValue>::sAlignment;

		using ValueType = TPValue;
		using MyType = Optional<TPValue>;
		using StorageType = typename OptionalStorage<TPValue>::Type;

	public:
		Optional()
		: _valuePtr( nullptr )
		{}

		Optional( TPValue && pInput )
		{
			_init( std::move( pInput ) );
		}

		Optional( const TPValue & pInput )
		{
			_init( pInput );
		}

		~Optional()
		{
			_release();
		}

		Optional & operator=( Optional && pRhs )
		{
			Optional( std::move( pRhs ) ).swap( *this );
			return *this;
		}

		Optional & operator=( const Optional & pRhs )
		{
			Optional( pRhs ).swap( *this );
			return *this;
		}

		Optional & operator=( TPValue && pRhs )
		{
			_reinit( std::move( pRhs ) );
			return *this;
		}

		Optional & operator=( const TPValue & pRhs )
		{
			_reinit( pRhs );
			return *this;
		}

		operator bool() const
		{
			return _check();
		}

		TPValue& operator*() const
		{
			_validate();
			return *(_valuePtr);
		}

		TPValue* operator->() const
		{
			_validate();
			return _valuePtr;
		}

		void set( TPValue && value )
		{
			_reinit( std::move( value ) );
		}

		void set( const TPValue & value )
		{
			_reinit( value );
		}

		template <typename... TPArgs>
		void emplace( TPArgs && ...pArgs )
		{
			_reinit( std::forward<TPArgs>( pArgs )... );
		}

		void reset()
		{
			_release();
		}

		bool empty() const
		{
			return !_check();
		}

		const TPValue & value() const
		{
			_validate();
			return *(_valuePtr);
		}

		const TPValue & valueOrDefault( const TPValue & pDefault ) const
		{
			return _check() ? *(_valuePtr) : pDefault;
		}

		void swap( MyType & other )
		{
			std::swap( *(_valuePtr), *( other._valuePtr ) );
		}

	private:
		template <typename... TPArgs>
		void _init( TPArgs && ...pArgs )
		{
			_valuePtr = reinterpret_cast<TPValue*>( &_storage );
			new (_valuePtr) TPValue( std::forward<TPArgs>( pArgs )... );
		}

		template <typename... TPArgs>
		void _reinit( TPArgs && ...pArgs )
		{
			if(_valuePtr != nullptr)
			{
				_valuePtr->~TPValue();
			}
			else
			{
				_valuePtr = reinterpret_cast<TPValue*>( &_storage );
			}

			new (_valuePtr) TPValue( std::forward<TPArgs>( pArgs )... );
		}

		bool _check() const
		{
			return _valuePtr != nullptr;
		}

		void _validate() const
		{
			if(_valuePtr == nullptr)
			{
				throw 0;
			}
		}

		void _Release()
		{
			if(_valuePtr != nullptr)
			{
				_valuePtr->~TPValue();
				_valuePtr = nullptr;
			}
		}

	private:
		StorageType _storage;
		TPValue * _valuePtr;
	};

}

#endif /* __CPPX_OPTIONAL_H__ */
