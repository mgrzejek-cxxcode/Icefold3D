
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
			return getByName( pName );
		}

		const EnumConstant<TPEnum> & operator[]( TPEnum pValue ) const
		{
			return getByValue( pValue );
		}

		const EnumConstant<TPEnum> * findByName( const std::string & pName ) const
		{
			auto constantIter = _constantsByName.find( pName );
			if( constantIter != _constantsByName.end() )
			{
				auto & constant = _constants[constantIter->second];
				return &constant;
			}
			return nullptr;
		}

		const EnumConstant<TPEnum> * findByValue( TPEnum pValue ) const
		{
			auto constantIter = _constantsByValue.find( pValue );
			if( constantIter != _constantsByValue.end() )
			{
				auto & constant = _constants[constantIter->second];
				return &constant;
			}
			return nullptr;
		}

		const EnumConstant<TPEnum> & getByName( const std::string & pName ) const
		{
			auto constantIndex = _constantsByName.at( pName );
			return _constants[constantIndex];
		}

		const EnumConstant<TPEnum> & getByValue( TPEnum pValue ) const
		{
			auto constantIndex = _constantsByValue.at( pValue );
			return _constants[constantIndex];
		}

		const EnumConstantList<TPEnum> & list() const
		{
			return _constants;
		}

		IC3_ATTR_NO_DISCARD size_t size() const
		{
			return _constants.size();
		}

		IC3_ATTR_NO_DISCARD bool empty() const
		{
			return _constants.empty();
		}

	private:
		EnumConstantMap & add( TPEnum pValue, std::string pName )
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
		return pEnumConstantMap.list().begin();
	}

	template <typename TPEnum>
	inline typename EnumConstantList<TPEnum>::const_iterator end( const EnumConstantMap<TPEnum> & pEnumConstantMap )
	{
		return pEnumConstantMap.list().end();
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

		IC3_ATTR_NO_DISCARD const EnumConstantMap<TPEnum> & getConstantMap() const
		{
			return _constantMap;
		}

		IC3_ATTR_NO_DISCARD const std::string & getConstantName( TPEnum pValue ) const
		{
			if( auto * constantDef = _constantMap.findByValue( pValue ) )
			{
				return constantDef->name;
			}
			return EMPTY_CONSTANT_NAME;
		}

		IC3_ATTR_NO_DISCARD TPEnum getConstantValue( const std::string & pName ) const
		{
			if( auto * constantDef = _constantMap.findByName( pName ) )
			{
				return constantDef->value;
			}

			return static_cast<TPEnum>( 0u );
		}

		IC3_ATTR_NO_DISCARD const std::string & getConstantNameOrDefault( TPEnum pValue, const std::string & pDefault = "" ) const
		{
			if( auto * constantDef = _constantMap.findByValue( pValue ) )
			{
				return constantDef->name;
			}
			return pDefault;
		}

		IC3_ATTR_NO_DISCARD TPEnum getConstantValueOrDefault( const std::string & pName, TPEnum pDefault = static_cast<TPEnum>( 0u ) ) const
		{
			if( auto * constantDef = _constantMap.findByName( pName ) )
			{
				return constantDef->value;
			}
			return pDefault;
		}

		IC3_ATTR_NO_DISCARD bool isValid() const
		{
			return !_constantMap.empty();
		}

	private:
		EnumTypeInfo & initialize( EnumProperties pProperties )
		{
			_properties = std::move( pProperties );
			return *this;
		}

		EnumTypeInfo & registerConstant( TPEnum pValue, std::string pName )
		{
			_constantMap.add( pValue, std::move( pName ) );
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
		static EnumTypeInfo<TPEnum> & initialize( EnumTypeInfo<TPEnum> & pEnumTypeInfo, EnumProperties pProperties )
		{
			return pEnumTypeInfo.initialize( std::move( pProperties ) );
		}

		static EnumTypeInfo<TPEnum> & registerConstant( EnumTypeInfo<TPEnum> & pEnumTypeInfo, TPEnum pValue, std::string pName )
		{
			return pEnumTypeInfo.registerConstant( pValue, std::move( pName ) );
		}
	};


	/// @brief Implements an enum's definition/initialization procedure.
	/// Usage and example: see below.
	#define ic3TypeInfoEnumDefine( TPEnum ) \
		/* Forward declaration of an enum-specific init method which registers all constants. */ \
		void initializeEnumTypeInfo##TPEnum( ::Ic3::EnumTypeInfo<TPEnum> & ); \
		/* Here, we implement the method declared with ic3TypeInfoEnumDeclare. */ \
		::Ic3::EnumTypeInfo<TPEnum> & _typeinfo::queryEnumTypeInfo##TPEnum() \
		{ \
			/* Create static EnumTypeInfo object and pass initializeEnumTypeInfoXXX function declared above. */ \
			/* EnumTypeInfo will call this function in its ctor and pass *this as an argument for initialization. */ \
			static Ic3::EnumTypeInfo<TPEnum> enumTypeInfo{ initializeEnumTypeInfo##TPEnum }; \
			return enumTypeInfo; \
		} \
		const std::string & _typeinfo::toString##TPEnum( TPEnum pValue ) \
		{ \
			return queryEnumTypeInfo##TPEnum().getConstantName( pValue ); \
		} \
		void initializeEnumTypeInfo##TPEnum( Ic3::EnumTypeInfo<TPEnum> & pEnumTypeInfo )

	/// @brief This is a basic initialization of the EnumTypeInfo object (pEnumTypeInfo).
	/// This is a continuation of initializeEnumTypeInfo##TPEnum function.
	/// Usage and example: see below.
	#define ic3TypeInfoEnumBegin( TPEnum ) \
        using EnumInitializer = Ic3::EnumTypeInfoInitializer<TPEnum>; \
		/* Create basic definition of an enum - currently only its name. */ \
		Ic3::EnumProperties enumProperties; \
		enumProperties.enumName = #TPEnum; \
		/* This is used for scoped enum constants (e.g. Color::Red). Prefix is the length of enum type name plus '::'. */ \
		const size_t enumNamePrefixLength = enumProperties.enumName.length() + 2; ( enumNamePrefixLength ); \
		/* Initialize the EnumTypeInfo object. */ \
		EnumInitializer::initialize( pEnumTypeInfo, std::move( enumProperties ) )

	/// @brief Registers a single enumerator of a scoped enum (enum class/struct) within the EnumTypeInfo object.
	#define ic3TypeInfoEnumRegisterClassConstant( pConstant ) \
		{ \
			/* Preprocessor does the job for us here. */ \
			const char * constantName = #pConstant; \
			/* Register the constant, but do not include the prefix (e.g. 'Color::' in case of 'Color::RED'). */ \
			EnumInitializer::registerConstant( pEnumTypeInfo, pConstant, constantName + enumNamePrefixLength ); \
		}

	/// @brief Registers a single enumerator of an unscoped enum within the EnumTypeInfo object.
	#define ic3TypeInfoEnumRegisterUnscopedConstant( pConstant ) \
		EnumInitializer::registerConstant( pEnumTypeInfo, pConstant, #pConstant )

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
	 *   E_DEVICE_FLAG_ACTIVEBit = 1, E_DEVICE_FLAG_PRIMARYBit = 2
	 * };
	 * ic3TypeInfoEnumDeclare( EColor );
	 * ic3TypeInfoEnumDeclare( EDeviceFlags );
	 *
	 * MySourceFile.cpp:
	 *
	 * ic3TypeInfoEnumDefine( EColor )
	 * {
	 *   ic3TypeInfoEnumBegin( EColor );
	 *   ic3TypeInfoEnumRegisterClassConstant( EColor::RED );
	 *   ic3TypeInfoEnumRegisterClassConstant( EColor::GREEN );
	 *   ic3TypeInfoEnumRegisterClassConstant( EColor::BLUE );
	 * }
	 * ic3TypeInfoEnumDefine( EDeviceFlags )
	 * {
	 *   ic3TypeInfoEnumBegin( EDeviceFlags );
	 *   ic3TypeInfoEnumRegisterUnscopedConstant( E_DEVICE_FLAG_ACTIVEBit );
	 *   ic3TypeInfoEnumRegisterUnscopedConstant( E_DEVICE_FLAG_PRIMARYBit );
	 * }
	 *
	 */

} // namespace Ic3

#endif // __IC3_CORELIB_ENUM_TYPE_INFO_H__
