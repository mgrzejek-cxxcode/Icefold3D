
#pragma once

#ifndef __IC3_CORELIB_CORE_ENUMS_H__
#define __IC3_CORELIB_CORE_ENUMS_H__

namespace Ic3
{

	enum class EActiveState : uint16
	{
		Disabled = 0,
		Enabled = 1,
		Unknown = 2
	};

	enum EAccessModeFlags : uint32
	{
		eAccessModeFlagReadBit = 0x0001,
		eAccessModeFlagWriteBit = 0x0002,
		eAccessModeFlagsFullAccess = eAccessModeFlagReadBit | eAccessModeFlagWriteBit,
		eAccessModeFlagsNone = 0
	};

	/// @brief Declares a namespace-scope function template for querying Ic3::EnumTypeInfo for enum types.
	#define Ic3EnableEnumTypeInfoSupport() \
        /* Template function used to retrieve an EnumTypeInfo object for an enum type. */ \
        /* Not implemented, specialized for every enum using Ic3TypeInfoEnumDeclare. */  \
        template <typename TPEnum> const ::Ic3::EnumTypeInfo<TPEnum> & QueryEnumTypeInfo();

	/// @brief Declares an enum for which an Ic3::EnumTypeInfo object can be queried.
	/// Extra arguments are used as a function specifier. You can pass e.g. declspec(dllexport/import) if the
	/// client library which provides the implementation is a dynamic library.
	/// Usage: call it once after an enum definition (in a header file), passing enum type as an argument.
	/// Example:
	/// enum class Color { Red, Green, Blue, Unknown };
	/// Ic3TypeInfoEnumDeclare( Color );
	#define Ic3TypeInfoEnumDeclare( TPEnum ) \
        namespace _typeinfo {                                                                              \
            /* Forward declaration of an enum-specific query function, e.g. queryEnumTypeInfoColor() */    \
            ::Ic3::EnumTypeInfo<TPEnum> & QueryEnumTypeInfo##TPEnum();                                       \
            const std::string & ToString##TPEnum( TPEnum );                                                  \
        }                                                                                                  \
        /* Implementation of QueryEnumTypeInfo<TPEnum> template function specialization. */                 \
        template <> inline const ::Ic3::EnumTypeInfo<TPEnum> & QueryEnumTypeInfo<TPEnum>()                   \
        {                                                                                                  \
            /* Just call the enum-specific function. This allows moving the definition code to .cpp. */    \
            return _typeinfo::QueryEnumTypeInfo##TPEnum();                                                  \
        }                                                                                                  \
        CPPX_ATTR_NO_DISCARD inline const std::string & ToString( TPEnum pValue )                           \
        {                                                                                                  \
        	/* Just call the enum-specific function. This allows moving the definition code to .cpp. */    \
        	return _typeinfo::ToString##TPEnum( pValue );                                                   \
        }

	#define Ic3TypeInfoEnumDeclareAPI( TPEnum, pAPISpec ) \
        namespace _typeinfo {                                                                              \
            /* Forward declaration of an enum-specific query function, e.g. queryEnumTypeInfoColor() */    \
            pAPISpec ::Ic3::EnumTypeInfo<TPEnum> & QueryEnumTypeInfo##TPEnum();                                       \
            pAPISpec const std::string & ToString##TPEnum( TPEnum );                                                  \
        }                                                                                                  \
        /* Implementation of QueryEnumTypeInfo<TPEnum> template function specialization. */                 \
        template <> inline const ::Ic3::EnumTypeInfo<TPEnum> & QueryEnumTypeInfo<TPEnum>()                   \
        {                                                                                                  \
            /* Just call the enum-specific function. This allows moving the definition code to .cpp. */    \
            return _typeinfo::QueryEnumTypeInfo##TPEnum();                                                  \
        }                                                                                                  \
        CPPX_ATTR_NO_DISCARD inline const std::string & ToString( TPEnum pValue )                           \
        {                                                                                                  \
        	/* Just call the enum-specific function. This allows moving the definition code to .cpp. */    \
        	return _typeinfo::ToString##TPEnum( pValue );                                                   \
        }

    ///
    #define Ic3TypeInfoEnumDeclareCoreLib( TPEnum ) Ic3TypeInfoEnumDeclareAPI( TPEnum, IC3_CORELIB_API )

	// Enable support for enum type info for the whole Ic3:: namespace.
	Ic3EnableEnumTypeInfoSupport();

    Ic3TypeInfoEnumDeclareCoreLib( EActiveState );
    Ic3TypeInfoEnumDeclareCoreLib( EAccessModeFlags );

}

#endif // __IC3_CORELIB_CORE_ENUMS_H__
