
#ifndef __IC3_CORELIB_HFS_IDENTIFIER_H__
#define __IC3_CORELIB_HFS_IDENTIFIER_H__

#include "../Prerequisites.h"
#include <Ic3/Cppx/Hash.h>

namespace Ic3
{

	/// @brief Hybrid FNV1A32 + SDBM identifier for string-based IDs.
	struct HFSIdentifier
	{
	public:
		static constexpr auto sHashAlgo1 = Cppx::EHashAlgo::FNV1A32;
		static constexpr auto sHashAlgo2 = Cppx::EHashAlgo::SDBM;

		using Hash1 = Cppx::SHashObject<sHashAlgo1>;
		using Hash2 = Cppx::SHashObject<sHashAlgo2>;
		using ValueType = uint64;

		uint64 idValue;
		
	public:
		HFSIdentifier() = default;

		HFSIdentifier( const HFSIdentifier & ) = default;
		HFSIdentifier & operator=( const HFSIdentifier & ) = default;

		explicit constexpr HFSIdentifier( uint64 p64Value ) noexcept
		: idValue( p64Value )
		{}

		constexpr HFSIdentifier( uint32 pHashValue1, uint32 pHashValue2 ) noexcept
		: idValue( makeU64Value( pHashValue1, pHashValue2 ) )
		{}

		constexpr HFSIdentifier( const Hash1 & pHash1, const Hash2 & pHash2 ) noexcept
		: idValue( makeU64Value( pHash1.hashValue, pHash2.hashValue ) )
		{}

		constexpr operator uint64() const noexcept
		{
			return idValue;
		}

		explicit constexpr operator bool() const noexcept
		{
			return !empty();
		}

		constexpr bool empty() const noexcept
		{
			return idValue == 0;
		}

		constexpr ValueType asValue() const noexcept
		{
			return idValue;
		}

		constexpr void set( uint32 pHashValue1, uint32 pHashValue2 ) noexcept
		{
			idValue = makeU64Value( pHashValue1, pHashValue2 );
		}

		constexpr void set( const Hash1 & pHash1, const Hash2 & pHash2 ) noexcept
		{
			set( pHash1.hashValue, pHash2.hashValue );
		}

		static constexpr uint64 makeU64Value( uint32 pHashValue1, uint32 pHashValue2 ) noexcept
		{
			return ( static_cast<uint64>( pHashValue1 ) << 32 ) | pHashValue2;
		}
	};

	template <typename TInput, typename... TRest>
	inline HFSIdentifier generateHFSIdentifier( const TInput & pInput, const TRest & ...pRest )
	{
		const auto hash1 = Cppx::hashCompute<Cppx::EHashAlgo::FNV1A32>( pInput, pRest... );
		const auto hash2 = Cppx::hashCompute<Cppx::EHashAlgo::SDBM>( pInput, pRest... );
		return HFSIdentifier{ hash1, hash2 };
	}

} // namespace Ic3

template<>
struct std::hash<Ic3::HFSIdentifier>
{
	std::size_t operator()( const Ic3::HFSIdentifier & pHFSIdentifier ) const noexcept
	{
		return pHFSIdentifier.asValue();
	}
};

#endif // __IC3_CORELIB_HFS_IDENTIFIER_H__
