
#pragma once

#ifndef __IC3_CORELIB_ENUM_TYPE_INFO_H__
#define __IC3_CORELIB_ENUM_TYPE_INFO_H__

#include "../Prerequisites.h"
#include <unordered_map>

namespace Ic3
{

	/// @brief Basic properties of an enum to be registered.
	struct EnumProperties
	{
		/// @brief Name of an enum type which will be stored within its info object.
		std::string enumName;
	};


	template <typename TEnum>
	struct EnumConstant
	{
		std::string name;
		TEnum value;

		explicit operator TEnum() const
		{
			return value;
		}
	};

	template <typename TEnum>
	using EnumConstantList = std::vector< EnumConstant<TEnum> >;


	template <typename TEnum>
	class EnumConstantMap
	{
		friend class EnumTypeInfo<TEnum>;

	public:
		constexpr EnumConstantMap() = default;

		const EnumConstant<TEnum> & operator[]( const std::string & pName ) const
		{
			return getByName( pName );
		}

		const EnumConstant<TEnum> & operator[]( TEnum pValue ) const
		{
			return getByValue( pValue );
		}

		const EnumConstant<TEnum> * findByName( const std::string & pName ) const
		{
			auto constantIter = _constantsByName.find( pName );
			if( constantIter != _constantsByName.end() )
			{
				auto & constant = _constants[constantIter->second];
				return &constant;
			}
			return nullptr;
		}

		const EnumConstant<TEnum> * findByValue( TEnum pValue ) const
		{
			auto constantIter = _constantsByValue.find( pValue );
			if( constantIter != _constantsByValue.end() )
			{
				auto & constant = _constants[constantIter->second];
				return &constant;
			}
			return nullptr;
		}

		const EnumConstant<TEnum> & getByName( const std::string & pName ) const
		{
			auto constantIndex = _constantsByName.at( pName );
			return _constants[constantIndex];
		}

		const EnumConstant<TEnum> & getByValue( TEnum pValue ) const
		{
			auto constantIndex = _constantsByValue.at( pValue );
			return _constants[constantIndex];
		}

		const EnumConstantList<TEnum> & list() const
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
		EnumConstantMap & add( TEnum pValue, std::string pName )
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
		EnumConstantList<TEnum> _constants;
		//
		std::unordered_map<std::string, size_t> _constantsByName;
		//
		std::unordered_map<TEnum, size_t> _constantsByValue;
	};


	template <typename TEnum>
	inline typename EnumConstantList<TEnum>::const_iterator begin( const EnumConstantMap<TEnum> & pEnumConstantMap )
	{
		return pEnumConstantMap.list().begin();
	}

	template <typename TEnum>
	inline typename EnumConstantList<TEnum>::const_iterator end( const EnumConstantMap<TEnum> & pEnumConstantMap )
	{
		return pEnumConstantMap.list().end();
	}


	template <typename TEnum>
	class EnumTypeInfoInitializer;


	template <typename TEnum>
	class EnumTypeInfo
	{
		 friend class EnumTypeInfoInitializer<TEnum>;

	public:
		static inline const std::string sEmptyConstantName = "";

	public:
		template <typename TInitFunction>
		explicit EnumTypeInfo( TInitFunction pInitFunction )
		{
			pInitFunction( *this );
		}

		IC3_ATTR_NO_DISCARD const EnumConstantMap<TEnum> & getConstantMap() const
		{
			return _constantMap;
		}

		IC3_ATTR_NO_DISCARD const std::string & getConstantName( TEnum pValue ) const
		{
			if( auto * constantDef = _constantMap.findByValue( pValue ) )
			{
				return constantDef->name;
			}
			return sEmptyConstantName;
		}

		IC3_ATTR_NO_DISCARD TEnum getConstantValue( const std::string & pName ) const
		{
			if( auto * constantDef = _constantMap.findByName( pName ) )
			{
				return constantDef->value;
			}

			return static_cast<TEnum>( 0u );
		}

		IC3_ATTR_NO_DISCARD const std::string & getConstantNameOrDefault( TEnum pValue, const std::string & pDefault = "" ) const
		{
			if( auto * constantDef = _constantMap.findByValue( pValue ) )
			{
				return constantDef->name;
			}
			return pDefault;
		}

		IC3_ATTR_NO_DISCARD TEnum getConstantValueOrDefault( const std::string & pName, TEnum pDefault = static_cast<TEnum>( 0u ) ) const
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

		EnumTypeInfo & registerConstant( TEnum pValue, std::string pName )
		{
			_constantMap.add( pValue, std::move( pName ) );
			return *this;
		}

	private:
		//
		EnumProperties _properties;
		//
		EnumConstantMap<TEnum> _constantMap;
	};


	template <typename TEnum>
	class EnumTypeInfoInitializer
	{
	public:
		static EnumTypeInfo<TEnum> & initialize( EnumTypeInfo<TEnum> & pEnumTypeInfo, EnumProperties pProperties )
		{
			return pEnumTypeInfo.initialize( std::move( pProperties ) );
		}

		static EnumTypeInfo<TEnum> & registerConstant( EnumTypeInfo<TEnum> & pEnumTypeInfo, TEnum pValue, std::string pName )
		{
			return pEnumTypeInfo.registerConstant( pValue, std::move( pName ) );
		}
	};


	/// @brief Implements an enum's definition/initialization procedure.
	/// Usage and example: see below.
	#define ic3TypeInfoEnumDefine( TEnum ) \
		/* Forward declaration of an enum-specific init method which registers all constants. */ \
		void initializeEnumTypeInfo##TEnum( ::Ic3::EnumTypeInfo<TEnum> & ); \
		/* Here, we implement the method declared with ic3TypeInfoEnumDeclare. */ \
		::Ic3::EnumTypeInfo<TEnum> & _typeinfo::queryEnumTypeInfo##TEnum() \
		{ \
			/* Create static EnumTypeInfo object and pass initializeEnumTypeInfoXXX function declared above. */ \
			/* EnumTypeInfo will call this function in its ctor and pass *this as an argument for initialization. */ \
			static Ic3::EnumTypeInfo<TEnum> enumTypeInfo{ initializeEnumTypeInfo##TEnum }; \
			return enumTypeInfo; \
		} \
		const std::string & _typeinfo::toString##TEnum( TEnum pValue ) \
		{ \
			return queryEnumTypeInfo##TEnum().getConstantName( pValue ); \
		} \
		void initializeEnumTypeInfo##TEnum( Ic3::EnumTypeInfo<TEnum> & pEnumTypeInfo ) \

	/// @brief This is a basic initialization of the EnumTypeInfo object (pEnumTypeInfo).
	/// This is a continuation of initializeEnumTypeInfo##TEnum function.
	/// Usage and example: see below.
	#define ic3TypeInfoEnumBegin( TEnum ) \
        using EnumInitializer = Ic3::EnumTypeInfoInitializer<TEnum>; \
		/* Create basic definition of an enum - currently only its name. */ \
		Ic3::EnumProperties enumProperties; \
		enumProperties.enumName = #TEnum; \
		/* This is used for scoped enum constants (e.g. Color::Red). Prefix is the length of enum type name plus '::'. */ \
		const size_t enumNamePrefixLength = enumProperties.enumName.length() + 2; ( enumNamePrefixLength ); \
		/* Initialize the EnumTypeInfo object. */ \
		EnumInitializer::initialize( pEnumTypeInfo, std::move( enumProperties ) )

	/// @brief Registers a single enumerator of a scoped enum (enum class/struct) within the EnumTypeInfo object.
	#define ic3TypeInfoEnumRegisterClassConstant( pConstant ) \
		{ \
			/* Preprocessor does the job for us here. */ \
			const char * constantName = #pConstant; \
			/* Register the constant, but do not include the prefix (e.g. 'Color::' in case of 'Color::Red'). */ \
			EnumInitializer::registerConstant( pEnumTypeInfo, pConstant, constantName + enumNamePrefixLength ); \
		}

	/// @brief Registers a single enumerator of an unscoped enum within the EnumTypeInfo object.
	#define ic3TypeInfoEnumRegisterUnscopedConstant( pConstant ) \
		EnumInitializer::registerConstant( pEnumTypeInfo, pConstant, #pConstant )

	/*
	 * Basically, all above macros should be used together to form a definition of an EnumTypeInfo.
	 * Example usage:
	 *
	 * MyHeaderFile.h:
	 *
	 * enum class Color : uint32 {
	 *   Red, Green, Blue
	 * };
	 * enum DeviceFlags : uint32 {
	 *   DEVICE_FLAG_ACTIVE_BIT = 1, DEVICE_FLAG_PRIMARY_BIT = 2
	 * };
	 * ic3TypeInfoEnumDeclare( Color );
	 * ic3TypeInfoEnumDeclare( DeviceFlags );
	 *
	 * MySourceFile.cpp:
	 *
	 * ic3TypeInfoEnumDefine( Color )
	 * {
	 *   ic3TypeInfoEnumBegin( Color );
	 *   ic3TypeInfoEnumRegisterClassConstant( Color::Red );
	 *   ic3TypeInfoEnumRegisterClassConstant( Color::Green );
	 *   ic3TypeInfoEnumRegisterClassConstant( Color::Blue );
	 * }
	 * ic3TypeInfoEnumDefine( DeviceFlags )
	 * {
	 *   ic3TypeInfoEnumBegin( DeviceFlags );
	 *   ic3TypeInfoEnumRegisterUnscopedConstant( DEVICE_FLAG_ACTIVE_BIT );
	 *   ic3TypeInfoEnumRegisterUnscopedConstant( DEVICE_FLAG_PRIMARY_BIT );
	 * }
	 *
	 */

} // namespace Ic3

#endif // __IC3_CORELIB_ENUM_TYPE_INFO_H__
