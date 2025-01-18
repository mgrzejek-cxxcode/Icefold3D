
#ifndef __CPPX_VARIANT_H__
#define __CPPX_VARIANT_H__

#include "staticAlgo.h"
#include "typeTraits.h"
#include <typeinfo>

namespace cppx
{

	/// @brief Type for storing the variant type index.
	using variant_index_t = size_t;

	struct variant_type_conv_tag
	{};

	namespace cve
	{

		/// @brief Represents invalid variant type index. Usually indicates, that incompatible type was used as a target.
		constexpr variant_index_t invalid_variant_index = 0;

		/// @brief Shared instance of `variant_type_conv_tag`. Use it to force desired overload.
		inline constexpr variant_type_conv_tag variant_type_conv{};
		
	}

	template <typename TPValue>
	struct variant_type_index
	{
		static const variant_index_t value;
	};

	template <typename TPValue>
	inline const variant_index_t variant_type_index<TPValue>::value = typeid( TPValue ).hash_code();

	template <typename... TPTypeList>
	struct variant_storage
	{
		static constexpr size_t size = static_sizeof_type<TPTypeList...>::value;
		static constexpr size_t alignment = static_alignof_type<TPTypeList...>::value;

		using type = typename std::aligned_storage<size, alignment>::type;
	};


	template <typename TPValue, typename... TPRest>
	struct variant_type_selector
	{
	};


	template <bool tpExactType, typename... TPTypeList>
	struct variant_set_proxy;

	template <typename... TPTypeList>
	struct variant_set_proxy<true, TPTypeList...>
	{
		template <typename TArg>
		static variant_index_t construct( void * pStorage, TArg pArg )
		{
			using UnrefArg = std::remove_cv_t<TArg>;
			new ( reinterpret_cast<UnrefArg *>( pStorage ) ) UnrefArg( std::forward<TArg>( pArg ) );
			return variant_type_index<UnrefArg>::value;
		}
	};

	template <typename TPValue, typename... TPTypeList>
	struct variant_set_proxy<false, TPValue, TPTypeList...>
	{
		template <typename... TPArgs, typename std::enable_if<std::is_constructible<TPValue, TPArgs...>::value>>
		static variant_index_t construct( void * pStorage, TPArgs && ...pArgs )
		{
			new ( reinterpret_cast<TPValue *>( pStorage ) ) TPValue( std::forward<TPArgs>( pArgs )... );
			return variant_type_index<TPValue>::value;
		}

		template <typename... TPArgs, typename std::enable_if<!std::is_constructible<TPValue, TPArgs...>::value>>
		static variant_index_t construct( void * pStorage, TPArgs && ...pArgs )
		{
			return variant_set_proxy<false, TPTypeList...>::construct( pStorage, std::forward<TPArgs>( pArgs )... );
		}
	};

	template <>
	struct variant_set_proxy<false>
	{
		template <typename TPInput>
		static variant_index_t construct( void * pStorage, TPInput pInput )
		{
			return cve::invalid_variant_index;
		}
	};


	// Proxy with the actual implementation of the variant methods. Implementation is done using the following concept:
	//
	// variant is a template defined for finite list of types. By using variant_type_index (above) we assign a unique
	// index to every type used with variant. When an operation is executed for
	// a given type T (e.g. set<int>( 5 ) ), we do as follows:
	// - pass the unique typeindex to the proper method (here: typeindex of <int>)
	// - check if index matches the first type on the variant's type list
	// - if yes, execute the function assuming int storage and return
	// - if not, drop the first type and call the function recursively
	// Additional empty specialization of variant_proxy is provided with the fallback implementation of each method. When
	// it gets executed, it means, that requested type is not valid for the variant object (not present on the type list).
	//
	// Performance notice: by using constexpr modifier, the if actually becomes a compile-time check. Since it's a tail
	// recursion, each function should always end up being just a single call (false ifs should be completely removed by
	// every decent compiler).
	template <typename... TPTypeList>
	struct variant_proxy;

	template <typename TPValue, typename... TPRest>
	struct variant_proxy<TPValue, TPRest...>
	{
//		template <typename TArg, std::enable_if_t<is_type_on_type_list<TArg, TPValue, TPRest...>::value, int>>
//		static variant_index_t construct( void * pStorage, TArg pArg )
//		{
//			using type = std::remove_cv_t<std::remove_reference_t<TArg>>;
//			new ( reinterpret_cast<type *>( pStorage ) ) TArg( std::forward<TArg>( pArg ) );
//			return variant_type_index<type>::value;
//		}
//
//		template <typename... TPArgs, std::enable_if<std::is_constructible<TPValue, TPArgs...>::value, int>>
//		static variant_index_t construct( void * pStorage, TPArgs && ...pArgs )
//		{
//			new ( reinterpret_cast<TPValue *>( pStorage ) ) TPValue( std::forward<TPArgs>( pArgs )... );
//			return variant_type_index<TPValue>::value;
//		}
//
//		template <typename... TPArgs, std::enable_if<!std::is_constructible<TPValue, TPArgs...>::value, int>>
//		static variant_index_t construct( void * pStorage, TPArgs && ...pArgs )
//		{
//			return variant_proxy<TPRest...>::construct( pStorage, std::forward<TPArgs>( pArgs )... );
//		}

		template <typename... TPArgs>
		static variant_index_t construct( void * pStorage, TPArgs && ...pArgs )
		{
			return variant_set_proxy<false, TPValue, TPRest...>::template construct<TPArgs...>( pStorage, std::forward<TPArgs>( pArgs )... );
		}

		template <typename... TPArgs>
		static variant_index_t construct( const variant_index_t pTypeIndex, void * pStorage, TPArgs && ...pArgs )
		{
			if( pTypeIndex == variant_type_index<TPValue>::value )
			{
				new ( reinterpret_cast< TPValue * >( pStorage ) ) TPValue( std::forward<TPArgs>( pArgs )... );
				return variant_type_index<TPValue>::value;
			}
			else
			{
				return variant_proxy<TPRest...>::construct( pTypeIndex, pStorage, std::forward<TPArgs>( pArgs )... );
			}
		}

		template <typename TPInput>
		static variant_index_t construct_conv( void * pStorage, TPInput pValue )
		{
			if( std::is_convertible<TPInput, TPValue>::value )
			{
				new ( reinterpret_cast< TPValue * >( pStorage ) ) TPValue( std::forward<TPInput>( pValue ) );
				return variant_type_index<TPValue>::value;
			}
			else
			{
				return variant_proxy<TPRest...>::construct_conv( std::forward<TPInput>( pValue ) );
			}
		}

		static variant_index_t construct_default( void * pStorage )
		{
			if( std::is_default_constructible<TPValue>::value )
			{
				new ( reinterpret_cast< TPValue * >( pStorage ) ) TPValue();
				return variant_type_index<TPValue>::value;
			}
			else
			{
				return variant_proxy<TPRest...>::construct_default( pStorage );
			}
		}

		static variant_index_t copy( const variant_index_t pTypeIndex, void * pStorage, void * pSource )
		{
			if( pTypeIndex == variant_type_index<TPValue>::value )
			{
				new ( reinterpret_cast<TPValue *>( pStorage ) ) TPValue( *( reinterpret_cast< TPValue * >( pSource ) ) );
				return variant_type_index<TPValue>::value;
			}
			else
			{
				return variant_proxy<TPRest...>::copy( pTypeIndex, pSource, pStorage );
			}
		}

		static variant_index_t move( const variant_index_t pTypeIndex, void * pStorage, void * pSource )
		{
			if( pTypeIndex == variant_type_index<TPValue>::value )
			{
				new ( reinterpret_cast< TPValue * >( pStorage ) ) TPValue( std::move( *( reinterpret_cast< TPValue * >( pSource ) ) ) );
				return variant_type_index<TPValue>::value;
			}
			else
			{
				return variant_proxy<TPRest...>::move( pTypeIndex, pSource, pStorage );
			}
		}

		static variant_index_t destroy( const variant_index_t pTypeIndex, void * pStorage )
		{
			if( pTypeIndex == variant_type_index<TPValue>::value )
			{
				reinterpret_cast< TPValue * >( pStorage )->~TPValue();
				return variant_type_index<TPValue>::value;
			}
			else
			{
				return variant_proxy<TPRest...>::destroy( pTypeIndex, pStorage );
			}
		}

		static bool validate_type( const variant_index_t pTypeIndex )
		{
			if( pTypeIndex == variant_type_index<TPValue>::value )
			{
				return true;
			}
			else
			{
				return variant_proxy<TPRest...>::validate_type( pTypeIndex );
			}
		}

		template <typename TStorage>
		static void swap( const variant_index_t pFirstIndex, void * pFirstStorage, const variant_index_t pSecondIndex, void * pSecondStorage )
		{
			TStorage temporaryStorage;

			move( pFirstIndex, &temporaryStorage, pFirstStorage );
			move( pSecondIndex, pFirstStorage, pSecondStorage );
			move( pFirstIndex, pSecondStorage, &temporaryStorage );
		}
	};

	template <>
	struct variant_proxy<>
	{
		template <typename... TPArgs>
		static variant_index_t construct( void *, TPArgs && ... )
		{
			return cve::invalid_variant_index;
		}

		template <typename TPValue>
		static variant_index_t construct_conv( variant_index_t, void *, TPValue && )
		{
			return cve::invalid_variant_index;
		}

		static variant_index_t construct_default( void * )
		{
			return cve::invalid_variant_index;
		}

		static variant_index_t copy( variant_index_t, void *, void * )
		{
			return cve::invalid_variant_index;
		}

		static variant_index_t move( variant_index_t, void *, void * )
		{
			return cve::invalid_variant_index;
		}

		static variant_index_t destroy( variant_index_t, void * )
		{
			return cve::invalid_variant_index;
		}

		static bool validate_type( variant_index_t )
		{
			return false;
		}
	};


	template <typename... TPTypeList>
	class variant
	{
		static_assert( std::is_void<typename meta::first_matching_type<std::is_reference, TPTypeList...>::type>::value, "No references allowed" );

	public:
		static constexpr size_t storage_size = variant_storage<TPTypeList...>::size;
		static constexpr size_t storage_alignment = variant_storage<TPTypeList...>::alignment;

		using self_type = variant<TPTypeList...>;
		using storage_type = typename variant_storage<TPTypeList...>::type;

	public:
		variant()
		{
			_typeIndex = variant_proxy<TPTypeList...>::construct_default( &( _storage ) );
		}

		variant( variant && pSrcObject ) noexcept
		{
			_typeIndex = variant_proxy<TPTypeList...>::move( _typeIndex, &( _storage ), &( pSrcObject._storage ) );
		}

		variant( const variant & pSrcObject )
		{
			_typeIndex = variant_proxy<TPTypeList...>::copy( _typeIndex, &( _storage ), &( pSrcObject._storage ) );
		}

		template <typename TPValue> //, typename std::enable_if_t<!std::is_same<TPValue, self_type>::value, int>>
		variant( TPValue && pValue ) noexcept
		{
			_typeIndex = variant_proxy<TPTypeList...>::construct( &( _storage ), std::forward<TPValue>( pValue ) );
		}

		~variant()
		{
			_release();
		}

		variant & operator=( variant && pRhs ) noexcept
		{
			if( this != &pRhs )
			{
				variant( std::move( pRhs ) ).swap( *this );
			}

			return *this;
		}

		variant & operator=( const variant & pRhs )
		{
			if( this != &pRhs )
			{
				variant( pRhs ).swap( *this );
			}

			return *this;
		}

		template <typename TPTarget>
		variant & operator=( TPTarget && pRhs )
		{
			_re_init<TPTarget>( std::forward<TPTarget>( pRhs ) );
			return *this;
		}

		template <typename TPTarget>
		void set( TPTarget && pValue )
		{
			_re_init<TPTarget>( std::forward<TPTarget>( pValue ) );
		}

		template <typename TPTarget>
		void set_conv( TPTarget && pValue )
		{
			_re_init_conv<TPTarget>( std::forward<TPTarget>( pValue ) );
		}

		template <typename TPTarget, typename... TPArgs>
		void emplace( TPArgs &&... pArgs )
		{
			_re_init<TPTarget>( std::forward<TPArgs>( pArgs )... );
		}

		void swap( variant & pOther )
		{
			variant_proxy<TPTypeList...>::template swap<storage_type>( _typeIndex, &( _storage ), pOther._typeIndex, &( pOther._storage ) );
			std::swap( _typeIndex, pOther._typeIndex );
		}

		template <typename TPTarget>
		CPPX_ATTR_NO_DISCARD bool check_type() const
		{
			return _typeIndex == variant_type_index<TPTarget>::value;
		}

		template <typename TPTarget>
		CPPX_ATTR_NO_DISCARD TPTarget & get() const
		{
			_validate<TPTarget>();
			return *( reinterpret_cast<TPTarget *>( &( _storage ) ) );
		}

	private:
		template <typename TPTarget, typename... TPArgs>
		void _re_init( TPArgs && ...pArgs )
		{
			variant_proxy<TPTypeList...>::destroy( _typeIndex, &( _storage ) );
			_typeIndex = variant_proxy<TPTypeList...>::construct( variant_type_index<TPTarget>::value, &( _storage ), std::forward<TPArgs>( pArgs )... );
		}

		template <typename TP>
		void _re_init_conv( TP pValue )
		{
			variant_proxy<TPTypeList...>::destroy( _typeIndex, &( _storage ) );
			_typeIndex = variant_proxy<TPTypeList...>::construct_conv( &( _storage ), std::forward<TP>( pValue ) );
		}

		template <typename TP>
		void _validate() const
		{
			if( _typeIndex != variant_type_index<TP>::value )
			{
				throw 0;
			}
		}

		void _release()
		{
			variant_proxy<TPTypeList...>::destroy( _typeIndex, &( _storage ) );
			_typeIndex = cve::invalid_variant_index;
		}

	private:
		storage_type _storage;
		variant_index_t _typeIndex;
	};

}

#endif /* __CPPX_VARIANT_H__ */
