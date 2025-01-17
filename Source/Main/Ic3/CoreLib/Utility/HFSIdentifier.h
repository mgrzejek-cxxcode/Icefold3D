
#ifndef __IC3_CORELIB_HFS_IDENTIFIER_H__
#define __IC3_CORELIB_HFS_IDENTIFIER_H__

#include "../Prerequisites.h"
#include <cppx/hash.h>

namespace Ic3
{

	/// @brief [H]ybrid [F]NV1A32 + [S]DBM identifier for string-based IDs.
	struct HfsIdentifier
	{
	public:
		static constexpr auto hashAlgo1 = cppx::hash_algo::fnv1a32;
		static constexpr auto hashAlgo2 = cppx::hash_algo::sdbm;

		using Hash1 = cppx::hash_object<hashAlgo1>;
		using Hash2 = cppx::hash_object<hashAlgo2>;
		using ValueType = uint64;

		uint64 idValue;
		
	public:
		HfsIdentifier() = default;

		HfsIdentifier( const HfsIdentifier & ) = default;
		HfsIdentifier & operator=( const HfsIdentifier & ) = default;

		explicit constexpr HfsIdentifier( uint64 p64Value ) noexcept
		: idValue( p64Value )
		{}

		constexpr HfsIdentifier( uint32 pHashValue1, uint32 pHashValue2 ) noexcept
		: idValue( MakeU64Value( pHashValue1, pHashValue2 ) )
		{}

		constexpr HfsIdentifier( const Hash1 & pHash1, const Hash2 & pHash2 ) noexcept
		: idValue( MakeU64Value( pHash1.value, pHash2.value ) )
		{}

		constexpr operator uint64() const noexcept
		{
			return idValue;
		}

		explicit constexpr operator bool() const noexcept
		{
			return !IsEmpty();
		}

		constexpr bool IsEmpty() const noexcept
		{
			return idValue == 0;
		}

		constexpr ValueType AsValue() const noexcept
		{
			return idValue;
		}

		constexpr void Set( uint32 pHashValue1, uint32 pHashValue2 ) noexcept
		{
			idValue = MakeU64Value( pHashValue1, pHashValue2 );
		}

		constexpr void Set( const Hash1 & pHash1, const Hash2 & pHash2 ) noexcept
		{
			Set( pHash1.value, pHash2.value );
		}

		static constexpr uint64 MakeU64Value( uint32 pHashValue1, uint32 pHashValue2 ) noexcept
		{
			return ( static_cast<uint64>( pHashValue1 ) << 32 ) | pHashValue2;
		}
	};

	template <typename TPInput, typename... TRest>
	inline HfsIdentifier GenerateHfsIdentifier( const TPInput & pInput, const TRest & ...pRest )
	{
		const auto hash1 = cppx::hash_compute<cppx::hash_algo::fnv1a32>( pInput, pRest... );
		const auto hash2 = cppx::hash_compute<cppx::hash_algo::sdbm>( pInput, pRest... );
		return HfsIdentifier{ hash1, hash2 };
	}

} // namespace Ic3

template<>
struct std::hash<Ic3::HfsIdentifier>
{
	std::size_t operator()( const Ic3::HfsIdentifier & pHfsIdentifier ) const noexcept
	{
		return pHfsIdentifier.AsValue();
	}
};

#endif // __IC3_CORELIB_HFS_IDENTIFIER_H__
