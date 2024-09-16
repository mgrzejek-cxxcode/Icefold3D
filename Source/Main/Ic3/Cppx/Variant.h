
#ifndef __IC3_CPPX_VARIANT_H__
#define __IC3_CPPX_VARIANT_H__

#include "StaticAlgo.h"
#include "TypeTraits.h"
#include <typeinfo>

namespace Ic3::Cppx
{

	/// @brief Type for storing the Variant type index.
	using variant_index_t = size_t;

	struct VariantTypeConvTag
	{};

	/// @brief Represents invalid Variant type index. Usually indicates, that incompatible type was used as a target.
	constexpr variant_index_t CX_INVALID_VARIANT_INDEX = 0;

	/// @brief Shared instance of `VariantTypeConvTag`. Use it to force desired overload.
	inline constexpr VariantTypeConvTag CX_VARIANT_TYPE_CONV{};

	template <typename TPValue>
	struct VariantTypeIndex
	{
		static const variant_index_t value;
	};

	template <typename TPValue>
	inline const variant_index_t VariantTypeIndex<TPValue>::value = typeid( TPValue ).hash_code();

	template <typename... TPTypeList>
	struct VariantStorage
	{
		static constexpr size_t SIZE = StaticMaxSizeofT<TPTypeList...>::value;
		static constexpr size_t ALIGNMENT = StaticMaxAlignOfT<TPTypeList...>::value;

		using Type = typename std::aligned_storage<SIZE, ALIGNMENT>::type;
	};


	template <typename TPValue, typename... TPRest>
	struct VariantTypeSelector
	{
	};


	template <bool tpExactType, typename... TPTypeList>
	struct VariantSetProxy;

	template <typename... TPTypeList>
	struct VariantSetProxy<true, TPTypeList...>
	{
		template <typename TArg>
		static variant_index_t construct( void * pStorage, TArg pArg )
		{
			using UnrefArg = std::remove_cv_t<TArg>;
			new ( reinterpret_cast<UnrefArg *>( pStorage ) ) UnrefArg( std::forward<TArg>( pArg ) );
			return VariantTypeIndex<UnrefArg>::value;
		}
	};

	template <typename TPValue, typename... TPTypeList>
	struct VariantSetProxy<false, TPValue, TPTypeList...>
	{
		template <typename... TPArgs, typename std::enable_if<std::is_constructible<TPValue, TPArgs...>::value>>
		static variant_index_t construct( void * pStorage, TPArgs && ...pArgs )
		{
			new ( reinterpret_cast<TPValue *>( pStorage ) ) TPValue( std::forward<TPArgs>( pArgs )... );
			return VariantTypeIndex<TPValue>::value;
		}

		template <typename... TPArgs, typename std::enable_if<!std::is_constructible<TPValue, TPArgs...>::value>>
		static variant_index_t construct( void * pStorage, TPArgs && ...pArgs )
		{
			return VariantSetProxy<false, TPTypeList...>::construct( pStorage, std::forward<TPArgs>( pArgs )... );
		}
	};

	template <>
	struct VariantSetProxy<false>
	{
		template <typename TPInput>
		static variant_index_t construct( void * pStorage, TPInput pInput )
		{
			return CX_INVALID_VARIANT_INDEX;
		}
	};


	// Proxy with the actual implementation of the Variant methods. Implementation is done using the following concept:
	//
	// Variant is a template defined for finite list of types. By using VariantTypeIndex (above) we assign a unique
	// index to every type used with Variant. When an operation is executed for
	// a given type T (e.g. set<int>( 5 ) ), we do as follows:
	// - pass the unique typeindex to the proper method (here: typeindex of <int>)
	// - check if index matches the first type on the Variant's type list
	// - if yes, execute the function assuming int storage and return
	// - if not, drop the first type and call the function recursively
	// Additional empty specialization of VariantProxy is provided with the fallback implementation of each method. When
	// it gets executed, it means, that requested type is not valid for the Variant object (not present on the type list).
	//
	// Performance notice: by using constexpr modifier, the if actually becomes a compile-time check. Since it's a tail
	// recursion, each function should always end up being just a single call (false ifs should be completely removed by
	// every decent compiler).
	template <typename... TPTypeList>
	struct VariantProxy;

	template <typename TPValue, typename... TPRest>
	struct VariantProxy<TPValue, TPRest...>
	{
//		template <typename TArg, std::enable_if_t<QIsTypeOnTypeList<TArg, TPValue, TPRest...>::value, int>>
//		static variant_index_t construct( void * pStorage, TArg pArg )
//		{
//			using Type = std::remove_cv_t<std::remove_reference_t<TArg>>;
//			new ( reinterpret_cast<Type *>( pStorage ) ) TArg( std::forward<TArg>( pArg ) );
//			return VariantTypeIndex<Type>::value;
//		}
//
//		template <typename... TPArgs, std::enable_if<std::is_constructible<TPValue, TPArgs...>::value, int>>
//		static variant_index_t construct( void * pStorage, TPArgs && ...pArgs )
//		{
//			new ( reinterpret_cast<TPValue *>( pStorage ) ) TPValue( std::forward<TPArgs>( pArgs )... );
//			return VariantTypeIndex<TPValue>::value;
//		}
//
//		template <typename... TPArgs, std::enable_if<!std::is_constructible<TPValue, TPArgs...>::value, int>>
//		static variant_index_t construct( void * pStorage, TPArgs && ...pArgs )
//		{
//			return VariantProxy<TPRest...>::construct( pStorage, std::forward<TPArgs>( pArgs )... );
//		}

		template <typename... TPArgs>
		static variant_index_t construct( void * pStorage, TPArgs && ...pArgs )
		{
			return VariantSetProxy<false, TPValue, TPRest...>::template construct<TPArgs...>( pStorage, std::forward<TPArgs>( pArgs )... );
		}

		template <typename... TPArgs>
		static variant_index_t construct( const variant_index_t pTypeIndex, void * pStorage, TPArgs && ...pArgs )
		{
			if( pTypeIndex == VariantTypeIndex<TPValue>::value )
			{
				new ( reinterpret_cast< TPValue * >( pStorage ) ) TPValue( std::forward<TPArgs>( pArgs )... );
				return VariantTypeIndex<TPValue>::value;
			}
			else
			{
				return VariantProxy<TPRest...>::construct( pTypeIndex, pStorage, std::forward<TPArgs>( pArgs )... );
			}
		}

		template <typename TPInput>
		static variant_index_t constructConv( void * pStorage, TPInput pValue )
		{
			if( std::is_convertible<TPInput, TPValue>::value )
			{
				new ( reinterpret_cast< TPValue * >( pStorage ) ) TPValue( std::forward<TPInput>( pValue ) );
				return VariantTypeIndex<TPValue>::value;
			}
			else
			{
				return VariantProxy<TPRest...>::constructConv( std::forward<TPInput>( pValue ) );
			}
		}

		static variant_index_t constructDefault( void * pStorage )
		{
			if( std::is_default_constructible<TPValue>::value )
			{
				new ( reinterpret_cast< TPValue * >( pStorage ) ) TPValue();
				return VariantTypeIndex<TPValue>::value;
			}
			else
			{
				return VariantProxy<TPRest...>::constructDefault( pStorage );
			}
		}

		static variant_index_t copy( const variant_index_t pTypeIndex, void * pStorage, void * pSource )
		{
			if( pTypeIndex == VariantTypeIndex<TPValue>::value )
			{
				new ( reinterpret_cast<TPValue *>( pStorage ) ) TPValue( *( reinterpret_cast< TPValue * >( pSource ) ) );
				return VariantTypeIndex<TPValue>::value;
			}
			else
			{
				return VariantProxy<TPRest...>::copy( pTypeIndex, pSource, pStorage );
			}
		}

		static variant_index_t move( const variant_index_t pTypeIndex, void * pStorage, void * pSource )
		{
			if( pTypeIndex == VariantTypeIndex<TPValue>::value )
			{
				new ( reinterpret_cast< TPValue * >( pStorage ) ) TPValue( std::move( *( reinterpret_cast< TPValue * >( pSource ) ) ) );
				return VariantTypeIndex<TPValue>::value;
			}
			else
			{
				return VariantProxy<TPRest...>::move( pTypeIndex, pSource, pStorage );
			}
		}

		static variant_index_t destroy( const variant_index_t pTypeIndex, void * pStorage )
		{
			if( pTypeIndex == VariantTypeIndex<TPValue>::value )
			{
				reinterpret_cast< TPValue * >( pStorage )->~TPValue();
				return VariantTypeIndex<TPValue>::value;
			}
			else
			{
				return VariantProxy<TPRest...>::destroy( pTypeIndex, pStorage );
			}
		}

		static bool validateType( const variant_index_t pTypeIndex )
		{
			if( pTypeIndex == VariantTypeIndex<TPValue>::value )
			{
				return true;
			}
			else
			{
				return VariantProxy<TPRest...>::validateType( pTypeIndex );
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
	struct VariantProxy<>
	{
		template <typename... TPArgs>
		static variant_index_t construct( void *, TPArgs && ... )
		{
			return CX_INVALID_VARIANT_INDEX;
		}

		template <typename TPValue>
		static variant_index_t constructConv( variant_index_t, void *, TPValue && )
		{
			return CX_INVALID_VARIANT_INDEX;
		}

		static variant_index_t constructDefault( void * )
		{
			return CX_INVALID_VARIANT_INDEX;
		}

		static variant_index_t copy( variant_index_t, void *, void * )
		{
			return CX_INVALID_VARIANT_INDEX;
		}

		static variant_index_t move( variant_index_t, void *, void * )
		{
			return CX_INVALID_VARIANT_INDEX;
		}

		static variant_index_t destroy( variant_index_t, void * )
		{
			return CX_INVALID_VARIANT_INDEX;
		}

		static bool validateType( variant_index_t )
		{
			return false;
		}
	};


	template <typename... TPTypeList>
	class Variant
	{
		static_assert( std::is_void<typename QFirstMatchingType<std::is_reference, TPTypeList...>::Type>::value, "No references allowed" );

	public:
		static constexpr size_t STORAGE_SIZE = VariantStorage<TPTypeList...>::SIZE;
		static constexpr size_t STORAGE_ALIGNMENT = VariantStorage<TPTypeList...>::ALIGNMENT;

		using MyType = Variant<TPTypeList...>;
		using StorageType = typename VariantStorage<TPTypeList...>::Type;

	public:
		Variant()
		{
			_typeIndex = VariantProxy<TPTypeList...>::constructDefault( &( _storage ) );
		}

		Variant( Variant && pSrcObject ) noexcept
		{
			_typeIndex = VariantProxy<TPTypeList...>::move( _typeIndex, &( _storage ), &( pSrcObject._pStorage ) );
		}

		Variant( const Variant & pSrcObject )
		{
			_typeIndex = VariantProxy<TPTypeList...>::copy( _typeIndex, &( _storage ), &( pSrcObject._pStorage ) );
		}

		template <typename TPValue> //, typename std::enable_if_t<!std::is_same<TPValue, MyType>::value, int>>
		Variant( TPValue && pValue ) noexcept
		{
			_typeIndex = VariantProxy<TPTypeList...>::construct( &( _storage ), std::forward<TPValue>( pValue ) );
		}

		~Variant()
		{
			_release();
		}

		Variant & operator=( Variant && pRhs ) noexcept
		{
			if( this != &pRhs )
			{
				Variant( std::move( pRhs ) ).swap( *this );
			}

			return *this;
		}

		Variant & operator=( const Variant & pRhs )
		{
			if( this != &pRhs )
			{
				Variant( pRhs ).swap( *this );
			}

			return *this;
		}

		template <typename TPTarget>
		Variant & operator=( TPTarget && pRhs )
		{
			_reInit<TPTarget>( std::forward<TPTarget>( pRhs ) );
			return *this;
		}

		template <typename TPTarget>
		void set( TPTarget && pValue )
		{
			_reInit<TPTarget>( std::forward<TPTarget>( pValue ) );
		}

		template <typename TPTarget>
		void setConv( TPTarget && pValue )
		{
			_reInitConv<TPTarget>( std::forward<TPTarget>( pValue ) );
		}

		template <typename TPTarget, typename... TPArgs>
		void emplace( TPArgs &&... pArgs )
		{
			_reInit<TPTarget>( std::forward<TPArgs>( pArgs )... );
		}

		void swap( Variant & pOther )
		{
			VariantProxy<TPTypeList...>::template swap<StorageType>( _typeIndex, &( _storage ), pOther._typeIndex, &( pOther._storage ) );
			std::swap( _typeIndex, pOther._typeIndex );
		}

		template <typename TPTarget>
		IC3_ATTR_NO_DISCARD bool checkType() const
		{
			return _typeIndex == VariantTypeIndex<TPTarget>::value;
		}

		template <typename TPTarget>
		IC3_ATTR_NO_DISCARD TPTarget & get() const
		{
			_validate<TPTarget>();
			return *( reinterpret_cast<TPTarget *>( &( _storage ) ) );
		}

	private:
		template <typename TPTarget, typename... TPArgs>
		void _reInit( TPArgs && ...pArgs )
		{
			VariantProxy<TPTypeList...>::destroy( _typeIndex, &( _storage ) );
			_typeIndex = VariantProxy<TPTypeList...>::construct( VariantTypeIndex<TPTarget>::value, &( _storage ), std::forward<TPArgs>( pArgs )... );
		}

		template <typename TP>
		void _reInitConv( TP pValue )
		{
			VariantProxy<TPTypeList...>::destroy( _typeIndex, &( _storage ) );
			_typeIndex = VariantProxy<TPTypeList...>::constructConv( &( _storage ), std::forward<TP>( pValue ) );
		}

		template <typename TP>
		void _validate() const
		{
			if( _typeIndex != VariantTypeIndex<TP>::value )
			{
				throw 0;
			}
		}

		void _release()
		{
			VariantProxy<TPTypeList...>::destroy( _typeIndex, &( _storage ) );
			_typeIndex = CX_INVALID_VARIANT_INDEX;
		}

	private:
		StorageType _storage;
		variant_index_t _typeIndex;
	};

}

#endif /* __IC3_CPPX_VARIANT_H__ */
