
#ifndef __IC3_CPPX_HASH_OBJECT_H__
#define __IC3_CPPX_HASH_OBJECT_H__

#include "ArrayView.h"
#include <string>

namespace Ic3
{

	enum class EHashAlgo : enum_default_value_t
	{
		Adler32,
		CRC32,
		DJB2,
		FNV1A32,
		FNV1A64,
		SDBM
	};

	struct SHashInput
	{
		const void * data;
		size_t dataSize;

		template <typename TChar>
		explicit SHashInput( const std::basic_string<TChar> & pString )
		: data( pString.data() )
		, dataSize( pString.length() * sizeof( TChar ) )
		{}

		explicit SHashInput( const char * pCStr )
		: data( pCStr )
		, dataSize( std::strlen( pCStr ) )
		{}

		explicit SHashInput( const wchar_t * pWCStr )
		: data( pWCStr )
		, dataSize( std::wcslen( pWCStr ) )
		{}

		template <typename TValue>
		explicit SHashInput( const ArrayView<TValue> & pArrayView )
		: data( pArrayView.data() )
		, dataSize( pArrayView.size() * sizeof( TValue ) )
		{}

		template <typename TValue>
		explicit SHashInput( const TValue & pObject )
		: data( &pObject )
		, dataSize( sizeof( TValue ) )
		{}

		template <typename TInput>
		explicit SHashInput( const TInput * pData, size_t pCount )
		: data( pData )
		, dataSize( pCount * sizeof( TInput ) )
		{}
	};

	template <EHashAlgo tHashAlgo>
	struct QHashTraits;
	
	template <EHashAlgo tHashAlgo>
	struct SHashObject
	{
	public:
		using ValueType = typename QHashTraits<tHashAlgo>::ValueType;

		static constexpr EHashAlgo sHashAlgo = tHashAlgo;

		ValueType hashValue;

	public:
		SHashObject( const SHashObject & ) = default;
		SHashObject & operator=( const SHashObject & ) = default;
		
		constexpr SHashObject() noexcept
		: hashValue( QHashTraits<tHashAlgo>::sHashInitValue )
		{}

		constexpr explicit SHashObject( ValueType pValue ) noexcept
		: hashValue( pValue )
		{}

		constexpr explicit operator ValueType() const noexcept
		{
			return hashValue;
		}

		void swap( SHashObject & pOther ) noexcept
		{
			std::swap( hashValue, pOther.hashValue );
		}

		IC3_ATTR_NO_DISCARD bool equals( const SHashObject & pOther ) const noexcept
		{
			return hashValue == pOther.hashValue;
		}

		IC3_ATTR_NO_DISCARD int32 compare( const SHashObject & pOther ) const noexcept
		{
			return equals( pOther ) ? 0 : ( hashValue > pOther.hashValue ? 1 : -1 );
		}

		IC3_ATTR_NO_DISCARD constexpr bool operator==( const SHashObject<tHashAlgo> & pRhs ) const noexcept
		{
			return hashValue == pRhs.hashValue;
		}

		IC3_ATTR_NO_DISCARD constexpr bool operator!=( const SHashObject<tHashAlgo> & pRhs ) const noexcept
		{
			return hashValue != pRhs.hashValue;
		}

		IC3_ATTR_NO_DISCARD constexpr bool operator<( const SHashObject<tHashAlgo> & pRhs ) const noexcept
		{
			return hashValue < pRhs.hashValue;
		}

		IC3_ATTR_NO_DISCARD constexpr bool operator<=( const SHashObject<tHashAlgo> & pRhs ) const noexcept
		{
			return hashValue <= pRhs.hashValue;
		}

		IC3_ATTR_NO_DISCARD constexpr bool operator>( const SHashObject<tHashAlgo> & pRhs ) const noexcept
		{
			return hashValue > pRhs.hashValue;
		}

		IC3_ATTR_NO_DISCARD constexpr bool operator>=( const SHashObject<tHashAlgo> & pRhs ) const noexcept
		{
			return hashValue >= pRhs.hashValue;
		}
	};

	template <EHashAlgo tHashAlgo>
	inline constexpr SHashObject<tHashAlgo> cvHashEmpty{ 0u };

	template <EHashAlgo tHashAlgo>
	inline constexpr SHashObject<tHashAlgo> cvHashInit{ QHashTraits<tHashAlgo>::sHashInitValue };



	template <EHashAlgo tHashAlgo>
	IC3_ATTR_NO_DISCARD inline SHashObject<tHashAlgo> hashCompute( const void * pData, size_t pDataLength )
	{
		return SHashObject<tHashAlgo>{ QHashTraits<tHashAlgo>::compute( pData, pDataLength ) };
	}

	template <EHashAlgo tHashAlgo>
	IC3_ATTR_NO_DISCARD inline SHashObject<tHashAlgo> hashCompute( const SHashInput & pInput )
	{
		return hashCompute<tHashAlgo>( pInput.data, pInput.dataSize );
	}

	template <EHashAlgo tHashAlgo, typename TValue>
	IC3_ATTR_NO_DISCARD inline SHashObject<tHashAlgo> hashCompute( const TValue & pValue )
	{
		return hashCompute<tHashAlgo>( SHashInput{ pValue } );
	}

	template <EHashAlgo tHashAlgo>
	IC3_ATTR_NO_DISCARD inline SHashObject<tHashAlgo> hashComputeEx( const SHashObject<tHashAlgo> & pInitHash, const void * pData, size_t pDataLength )
	{
		return SHashObject<tHashAlgo>{ QHashTraits<tHashAlgo>::update( pInitHash.hashValue, pData, pDataLength ) };
	}

	template <EHashAlgo tHashAlgo>
	IC3_ATTR_NO_DISCARD inline SHashObject<tHashAlgo> hashComputeEx( const SHashObject<tHashAlgo> & pInitHash, const SHashInput & pInput )
	{
		return hashComputeEx<tHashAlgo>( pInitHash, pInput.data, pInput.dataSize );
	}

	template <EHashAlgo tHashAlgo, typename TValue>
	IC3_ATTR_NO_DISCARD inline SHashObject<tHashAlgo> hashComputeEx( const SHashObject<tHashAlgo> & pInitHash, const TValue & pValue )
	{
		return hashComputeEx<tHashAlgo>( pInitHash, SHashInput{ pValue } );
	}

	template <EHashAlgo tHashAlgo, typename TValue, typename... TRest>
	IC3_ATTR_NO_DISCARD inline SHashObject<tHashAlgo> hashCompute( const TValue & pValue, TRest && ...pRest )
	{
		return hashComputeEx<tHashAlgo>( hashCompute<tHashAlgo>( pValue ), std::forward<TRest>( pRest )... );
	}

	template <EHashAlgo tHashAlgo, typename TValue, typename... TRest>
	IC3_ATTR_NO_DISCARD inline SHashObject<tHashAlgo> hashComputeEx( const SHashObject<tHashAlgo> & pInitHash, const TValue & pValue, TRest && ...pRest )
	{
		return hashComputeEx<tHashAlgo>( hashComputeEx<tHashAlgo>( pInitHash, pValue ), std::forward<TRest>( pRest )... );
	}


	template <EHashAlgo tHashAlgo>
	inline void hashComputeInplace( SHashObject<tHashAlgo> & pOutput, const void * pData, size_t pDataLength )
	{
		pOutput.hashValue = QHashTraits<tHashAlgo>::compute( pData, pDataLength );
	}

	template <EHashAlgo tHashAlgo>
	inline void hashComputeInplace( SHashObject<tHashAlgo> & pOutput, const SHashInput & pInput )
	{
		hashComputeInplace<tHashAlgo>( pOutput, pInput.data, pInput.dataSize );
	}

	template <EHashAlgo tHashAlgo, typename TValue>
	inline void hashComputeInplace( SHashObject<tHashAlgo> & pOutput, const ArrayView<TValue> & pInput )
	{
		hashComputeInplace<tHashAlgo>( pOutput, pInput.data(), pInput.size() * sizeof( TValue ) );
	}

	template <EHashAlgo tHashAlgo>
	inline void hashComputeInplaceEx( SHashObject<tHashAlgo> & pOutput, const SHashObject<tHashAlgo> & pInitHash, const void * pData, size_t pDataLength )
	{
		pOutput.hashValue = QHashTraits<tHashAlgo>::update( pInitHash.hashValue, pData, pDataLength );
	}

	template <EHashAlgo tHashAlgo>
	inline void hashComputeInplaceEx( SHashObject<tHashAlgo> & pOutput, const SHashObject<tHashAlgo> & pInitHash, const SHashInput & pInput )
	{
		hashComputeInplaceEx<tHashAlgo>( pOutput, pInitHash, pInput.data, pInput.dataSize );
	}

	template <EHashAlgo tHashAlgo, typename TValue>
	inline void hashComputeInplaceEx( SHashObject<tHashAlgo> & pOutput, const SHashObject<tHashAlgo> & pInitHash, const TValue & pValue )
	{
		hashComputeInplaceEx<tHashAlgo>( pOutput, pInitHash, SHashInput{ pValue } );
	}

	template <EHashAlgo tHashAlgo, typename TValue, typename... TRest>
	inline void hashComputeInplace( SHashObject<tHashAlgo> & pOutput, const TValue & pValue, TRest && ...pRest )
	{
		hashComputeInplaceEx<tHashAlgo>( pOutput, hashCompute<tHashAlgo>( pValue ), std::forward<TRest>( pRest )... );
	}

	template <EHashAlgo tHashAlgo, typename TValue, typename... TRest>
	inline void hashComputeInplaceEx( SHashObject<tHashAlgo> & pOutput, const SHashObject<tHashAlgo> & pInitHash, const TValue & pValue, TRest && ...pRest )
	{
		hashComputeInplaceEx<tHashAlgo>( pOutput, hashComputeEx<tHashAlgo>( pInitHash, pValue ), std::forward<TRest>( pRest )... );
	}

	template <typename THashValue>
	struct QHashCommonTraits
	{
		/// Underlying type used to store hash value.
		using ValueType = THashValue;

		/// Size of the hash value, in bytes.
		static constexpr size_t sByteSize = sizeof( THashValue );

		/// Length of the hexadecimal string representation of a hash.
		static constexpr size_t sHexLength = sByteSize * 8 / 4;
	};

	template <>
	struct QHashTraits<EHashAlgo::Adler32> : public QHashCommonTraits<uint32>
	{
		static constexpr uint32 sHashInitValue = 1u;
		static uint32 compute( const void * pInput, size_t pInputSize );
		static uint32 update( uint32 pHash, const void * pInput, size_t pInputSize );
	};

	template <>
	struct QHashTraits<EHashAlgo::CRC32> : public QHashCommonTraits<uint32>
	{
		static constexpr uint32 sHashInitValue = 0xFFFFFFFF;
		static uint32 compute( const void * pInput, size_t pInputSize );
		static uint32 update( uint32 pHash, const void * pInput, size_t pInputSize );
	};

	template <>
	struct QHashTraits<EHashAlgo::DJB2> : public QHashCommonTraits<uint32>
	{
		static constexpr uint32 sHashInitValue = 0x1505;
		static uint32 compute( const void * pInput, size_t pInputSize );
		static uint32 update( uint32 pHash, const void * pInput, size_t pInputSize );
	};

	template <>
	struct QHashTraits<EHashAlgo::FNV1A32> : public QHashCommonTraits<uint32>
	{
		static constexpr uint32 sHashInitValue = 0x811C9DC5;
		static uint32 compute( const void * pInput, size_t pInputSize );
		static uint32 update( uint32 pHash, const void * pInput, size_t pInputSize );
	};

	template <>
	struct QHashTraits<EHashAlgo::FNV1A64> : public QHashCommonTraits<uint64>
	{
		static constexpr uint64 sHashInitValue = 0xCBF29CE484222325;
		static uint64 compute( const void * pInput, size_t pInputSize );
		static uint64 update( uint64 pHash, const void * pInput, size_t pInputSize );
	};

	template <>
	struct QHashTraits<EHashAlgo::SDBM> : public QHashCommonTraits<uint32>
	{
		static constexpr uint32 sHashInitValue = 0u;
		static uint32 compute( const void * pInput, size_t pInputSize );
		static uint32 update( uint32 pHash, const void * pInput, size_t pInputSize );
	};

}

namespace std
{

	template <Ic3::EHashAlgo tHashAlgo>
	struct hash< Ic3::SHashObject<tHashAlgo> >
	{
		size_t operator()( const Ic3::SHashObject<tHashAlgo> & pHashObject ) const noexcept
		{
			return Ic3::numeric_cast<size_t>( pHashObject.hashValue );
		}
	};

}

#endif // __IC3_CPPX_HASH_OBJECT_H__
