
#pragma once

#ifndef __IC3_GRAPHICS_COMMON_GRAPHICS_CORE_DEFS_H__
#define __IC3_GRAPHICS_COMMON_GRAPHICS_CORE_DEFS_H__

#include <Ic3/CoreLib/Utility/HFSIdentifier.h>
#include <cppx/immutableString.h>

namespace Ic3::Graphics
{

	using GfxObjectID = uint64;
	using GfxObjectName = cppx::immutable_string;

	template <typename TP>
	using TGfxHandle = TSharedHandle<TP>;

	///
	inline constexpr GfxObjectID kGfxObjectIDAuto = cppx::meta::limits<uint64>::max_value;

	///
	inline constexpr GfxObjectID kGfxObjectIDEmpty = 0u;

	constexpr inline bool IsGfxObjectIDValid( GfxObjectID pObjectID ) noexcept
	{
		return ( pObjectID != kGfxObjectIDAuto ) && ( pObjectID != kGfxObjectIDEmpty );
	}

	template <typename... TPArgs>
	inline GfxObjectName MakeGfxObjectName( TPArgs && ...pArgs )
	{
		return GfxObjectName{ std::forward<TPArgs>( pArgs )... };
	}

	template <typename TPInput>
	inline GfxObjectID GenerateGfxObjectID( const TPInput & pInput )
	{
		return GenerateHfsIdentifier( pInput );
	}

	template <typename TPInput>
	inline GfxObjectID MakeGfxObjectIDFromName( const cppx::string_view & pGfxObjectName )
	{
		return GenerateHfsIdentifier( pGfxObjectName );
	}

	template <typename TPInput>
	inline GfxObjectID MakeGfxObjectIDFromName( const cppx::immutable_string & pGfxObjectName )
	{
		return GenerateHfsIdentifier( pGfxObjectName );
	}

	/**
	 *
	 */
	struct GfxObjectUID
	{
	public:
		GfxObjectID id = kGfxObjectIDEmpty;

		cppx::string_view name;

	public:
		constexpr GfxObjectUID() = default;

		GfxObjectUID( const GfxObjectUID & ) = default;
		GfxObjectUID & operator=( const GfxObjectUID & ) = default;

		GfxObjectUID( GfxObjectID pObjectID )
		: id( pObjectID )
		{}

		GfxObjectUID( cppx::string_view pObjectName )
		: id( pObjectName ? GenerateGfxObjectID( pObjectName ) : kGfxObjectIDEmpty )
		, name( pObjectName )
		{}

		GfxObjectUID( cppx::string_view pObjectName, GfxObjectID pObjectID )
		: id( pObjectID )
		, name( pObjectName )
		{}

		GfxObjectUID & operator=( GfxObjectID pObjectID )
		{
			id = pObjectID;
			return *this;
		}

		GfxObjectUID & operator=( cppx::string_view pObjectName )
		{
			name = pObjectName;
			id = pObjectName ? GenerateGfxObjectID( pObjectName ) : kGfxObjectIDEmpty;
			return *this;
		}

		operator GfxObjectID() const noexcept
		{
			return id;
		}

		operator cppx::string_view () const noexcept
		{
			return name;
		}

		explicit operator bool() const noexcept
		{
			return !empty();
		}

		bool empty() const noexcept
		{
			return ( id != kGfxObjectIDEmpty ) && !name.empty();
		}

		bool operator==( const GfxObjectUID & pRhs ) const noexcept
		{
			return ( id == pRhs.id ) && ( name == pRhs.name );
		}

		bool operator!=( const GfxObjectUID & pRhs ) const noexcept
		{
			return ( id != pRhs.id ) || ( name != pRhs.name );
		}
	};

} // namespace Ic3::Graphics

#endif // __IC3_GRAPHICS_COMMON_GRAPHICS_CORE_DEFS_H__
