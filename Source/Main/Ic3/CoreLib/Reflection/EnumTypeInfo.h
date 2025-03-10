
#pragma once

#ifndef __IC3_CORELIB_ENUM_TYPE_INFO_H__
#define __IC3_CORELIB_ENUM_TYPE_INFO_H__

#include "../Prerequisites.h"

#include <deque>
#include <unordered_map>

namespace Ic3
{

	/// @brief Basic properties of an enum to be registered.
	struct EnumProperties
	{
		/// @brief Name of an enum type which will be stored within its info object.
		std::string enumName;
	};


	template <typename TPEnum>
	struct EnumConstant
	{
		std::string name;
		
		TPEnum value;

		explicit operator TPEnum() const
		{
			return value;
		}
	};

	template <typename TPEnum>
	using EnumConstantList = std::deque< EnumConstant<TPEnum> >;


	template <typename TPEnum>
	class EnumConstantMap
	{
		friend class EnumTypeInfo<TPEnum>;

	public:
		constexpr EnumConstantMap() = default;

		const EnumConstant<TPEnum> & operator[]( const std::string & pName ) const
		{
			return GetByName( pName );
		}

		const EnumConstant<TPEnum> & operator[]( TPEnum pValue ) const
		{
			return GetByValue( pValue );
		}

		const EnumConstant<TPEnum> * FindByName( const std::string & pName ) const
		{
			auto constantIter = _constantsByName.find( pName );
			if( constantIter != _constantsByName.end() )
			{
				auto & constant = _constants[constantIter->second];
				return &constant;
			}
			return nullptr;
		}

		const EnumConstant<TPEnum> * FindByValue( TPEnum pValue ) const
		{
			auto constantIter = _constantsByValue.find( pValue );
			if( constantIter != _constantsByValue.end() )
			{
				auto & constant = _constants[constantIter->second];
				return &constant;
			}
			return nullptr;
		}

		const EnumConstant<TPEnum> & GetByName( const std::string & pName ) const
		{
			auto constantIndex = _constantsByName.at( pName );
			return _constants[constantIndex];
		}

		const EnumConstant<TPEnum> & GetByValue( TPEnum pValue ) const
		{
			auto constantIndex = _constantsByValue.at( pValue );
			return _constants[constantIndex];
		}

		const EnumConstantList<TPEnum> & List() const
		{
			return _constants;
		}

		CPPX_ATTR_NO_DISCARD size_t GetSize() const
		{
			return _constants.size();
		}

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const
		{
			return _constants.empty();
		}

	private:
		EnumConstantMap & Add( TPEnum pValue, std::string pName )
		{
			auto constantIndex = _constants.size();
			auto & constantDef = _constants.emplace_back();
			constantDef.name = std::move( pName );
			constantDef.value = pValue;

			_constantsByName[constantDef.name] = constantIndex;
			_constantsByValue[constantDef.value] = constantIndex;

			return *this;
		}

	private:
		//
		EnumConstantList<TPEnum> _constants;

		//
		std::unordered_map<std::string_view , size_t> _constantsByName;

		//
		std::unordered_map<TPEnum, size_t> _constantsByValue;
	};


	template <typename TPEnum>
	inline typename EnumConstantList<TPEnum>::const_iterator begin( const EnumConstantMap<TPEnum> & pEnumConstantMap )
	{
		return pEnumConstantMap.List().begin();
	}

	template <typename TPEnum>
	inline typename EnumConstantList<TPEnum>::const_iterator end( const EnumConstantMap<TPEnum> & pEnumConstantMap )
	{
		return pEnumConstantMap.List().end();
	}


	template <typename TPEnum>
	class EnumTypeInfoInitializer;


	template <typename TPEnum>
	class EnumTypeInfo
	{
		 friend class EnumTypeInfoInitializer<TPEnum>;

	public:
		static inline const std::string EMPTY_CONSTANT_NAME = "";

	public:
		template <typename TPInitFunction>
		explicit EnumTypeInfo( TPInitFunction pInitFunction )
		{
			pInitFunction( *this );
		}

		CPPX_ATTR_NO_DISCARD const EnumConstantMap<TPEnum> & GetConstantMap() const
		{
			return _constantMap;
		}

		CPPX_ATTR_NO_DISCARD const std::string & GetConstantName( TPEnum pValue ) const
		{
			if( auto * constantDef = _constantMap.FindByValue( pValue ) )
			{
				return constantDef->name;
			}
			return EMPTY_CONSTANT_NAME;
		}

		CPPX_ATTR_NO_DISCARD TPEnum GetConstantValue( const std::string & pName ) const
		{
			if( auto * constantDef = _constantMap.FindByName( pName ) )
			{
				return constantDef->value;
			}

			return static_cast<TPEnum>( 0u );
		}

		CPPX_ATTR_NO_DISCARD const std::string & GetConstantNameOrDefault( TPEnum pValue, const std::string & pDefault = "" ) const
		{
			if( auto * constantDef = _constantMap.FindByValue( pValue ) )
			{
				return constantDef->name;
			}
			return pDefault;
		}

		CPPX_ATTR_NO_DISCARD TPEnum GetConstantValueOrDefault( const std::string & pName, TPEnum pDefault = static_cast<TPEnum>( 0u ) ) const
		{
			if( auto * constantDef = _constantMap.FindByName( pName ) )
			{
				return constantDef->value;
			}
			return pDefault;
		}

		CPPX_ATTR_NO_DISCARD bool IsValid() const
		{
			return !_constantMap.empty();
		}

	private:
		EnumTypeInfo & Initialize( EnumProperties pProperties )
		{
			_properties = std::move( pProperties );
			return *this;
		}

		EnumTypeInfo & RegisterConstant( TPEnum pValue, std::string pName )
		{
			_constantMap.Add( pValue, std::move( pName ) );
			return *this;
		}

	private:
		//
		EnumProperties _properties;
		//
		EnumConstantMap<TPEnum> _constantMap;
	};


	template <typename TPEnum>
	class EnumTypeInfoInitializer
	{
	public:
		static EnumTypeInfo<TPEnum> & Initialize( EnumTypeInfo<TPEnum> & pEnumTypeInfo, EnumProperties pProperties )
		{
			return pEnumTypeInfo.Initialize( std::move( pProperties ) );
		}

		static EnumTypeInfo<TPEnum> & RegisterConstant( EnumTypeInfo<TPEnum> & pEnumTypeInfo, TPEnum pValue, std::string pName )
		{
			return pEnumTypeInfo.RegisterConstant( pValue, std::move( pName ) );
		}
	};


	/// @brief Implements an enum's definition/initialization procedure.
	/// Usage and example: see below.
	#define Ic3TypeInfoEnumDefine( TPEnum ) \
		/* Forward declaration of an enum-specific init method which registers all constants. */ \
		void InitializeEnumTypeInfo##TPEnum( ::Ic3::EnumTypeInfo<TPEnum> & ); \
		/* Here, we implement the method declared with Ic3TypeInfoEnumDeclare. */ \
		::Ic3::EnumTypeInfo<TPEnum> & _typeinfo::QueryEnumTypeInfo##TPEnum() \
		{ \
			/* Create static EnumTypeInfo object and pass InitializeEnumTypeInfoXXX function declared above. */ \
			/* EnumTypeInfo will call this function in its ctor and pass *this as an argument for initialization. */ \
			static Ic3::EnumTypeInfo<TPEnum> enumTypeInfo{ InitializeEnumTypeInfo##TPEnum }; \
			return enumTypeInfo; \
		} \
		const std::string & _typeinfo::ToString##TPEnum( TPEnum pValue ) \
		{ \
			return QueryEnumTypeInfo##TPEnum().GetConstantName( pValue ); \
		} \
		void InitializeEnumTypeInfo##TPEnum( Ic3::EnumTypeInfo<TPEnum> & pEnumTypeInfo )

	/// @brief This is a basic initialization of the EnumTypeInfo object (pEnumTypeInfo).
	/// This is a continuation of InitializeEnumTypeInfo##TPEnum function.
	/// Usage and example: see below.
	#define Ic3TypeInfoEnumBegin( TPEnum ) \
        using EnumInitializer = Ic3::EnumTypeInfoInitializer<TPEnum>; \
		/* Create basic definition of an enum - currently only its name. */ \
		Ic3::EnumProperties enumProperties; \
		enumProperties.enumName = #TPEnum; \
		/* This is used for scoped enum constants (e.g. Color::Red). Prefix is the length of enum type name plus '::'. */ \
		const size_t enumNamePrefixLength = enumProperties.enumName.length() + 2; ( enumNamePrefixLength ); \
		/* Initialize the EnumTypeInfo object. */ \
		EnumInitializer::Initialize( pEnumTypeInfo, std::move( enumProperties ) )

	/// @brief Registers a single enumerator of a scoped enum (enum class/struct) within the EnumTypeInfo object.
	#define Ic3TypeInfoEnumRegisterClassConstant( pConstant ) \
		{ \
			/* Preprocessor does the job for us here. */ \
			const char * constantName = #pConstant; \
			/* Register the constant, but do not include the prefix (e.g. 'Color::' in case of 'Color::RED'). */ \
			EnumInitializer::RegisterConstant( pEnumTypeInfo, pConstant, constantName + enumNamePrefixLength ); \
		}

	/// @brief Registers a single enumerator of an unscoped enum within the EnumTypeInfo object.
	#define Ic3TypeInfoEnumRegisterUnscopedConstant( pConstant ) \
		EnumInitializer::RegisterConstant( pEnumTypeInfo, pConstant, #pConstant )

	/*
	 * Above macros should be used together to form a definition of an EnumTypeInfo.
	 * Example usage:
	 *
	 * MyHeaderFile.h:
	 *
	 * enum class EColor : uint32 {
	 *   RED, GREEN, BLUE
	 * };
	 * enum EDeviceFlags : uint32 {
	 *   E_DEVICE_FLAG_ACTIVE_BIT = 1, E_DEVICE_FLAG_PRIMARY_BIT = 2
	 * };
	 * Ic3TypeInfoEnumDeclare( EColor );
	 * Ic3TypeInfoEnumDeclare( EDeviceFlags );
	 *
	 * MySourceFile.cpp:
	 *
	 * Ic3TypeInfoEnumDefine( EColor )
	 * {
	 *   Ic3TypeInfoEnumBegin( EColor );
	 *   Ic3TypeInfoEnumRegisterClassConstant( EColor::RED );
	 *   Ic3TypeInfoEnumRegisterClassConstant( EColor::GREEN );
	 *   Ic3TypeInfoEnumRegisterClassConstant( EColor::BLUE );
	 * }
	 * Ic3TypeInfoEnumDefine( EDeviceFlags )
	 * {
	 *   Ic3TypeInfoEnumBegin( EDeviceFlags );
	 *   Ic3TypeInfoEnumRegisterUnscopedConstant( E_DEVICE_FLAG_ACTIVE_BIT );
	 *   Ic3TypeInfoEnumRegisterUnscopedConstant( E_DEVICE_FLAG_PRIMARY_BIT );
	 * }
	 *
	 */

} // namespace Ic3

#endif // __IC3_CORELIB_ENUM_TYPE_INFO_H__
