
#ifndef __IC3_SYSTEM_PREREQUISITES_H__
#define __IC3_SYSTEM_PREREQUISITES_H__

#include <Ic3/CoreLib/Exception.h>
#include <Ic3/Cppx/Bitmask.h>
#include <Ic3/Cppx/Utilities.h>
#include <Ic3/Cppx/Version.h>

#include <memory>
#include <string>
#include <utility>

#if( IC3_BUILD_STATIC )
#  define IC3_SYSTEM_API
#  define IC3_SYSTEM_CLASS
#  define IC3_SYSTEM_OBJ extern __cdecl
#else
#  if( IC3_SYSTEM_BUILD )
#	define IC3_SYSTEM_API   IC3_PCL_ATTR_DLL_EXPORT
#	define IC3_SYSTEM_CLASS IC3_PCL_ATTR_DLL_EXPORT
#	define IC3_SYSTEM_OBJ   IC3_PCL_ATTR_DLL_EXPORT
#  else
#	define IC3_SYSTEM_API   IC3_PCL_ATTR_DLL_IMPORT
#	define IC3_SYSTEM_CLASS IC3_PCL_ATTR_DLL_IMPORT
#	define IC3_SYSTEM_OBJ   IC3_PCL_ATTR_DLL_IMPORT
#  endif
#endif

#define IC3_SYSTEM_API_NODISCARD IC3_SYSTEM_API IC3_ATTR_NO_DISCARD

namespace Ic3::System
{

	ic3EnableCustomExceptionSupport();
	ic3EnableEnumTypeInfoSupport();

	template <typename TPClass>
	using TSysHandle = ::Ic3::TSharedHandle<TPClass>;

	/// @brief Declares the handle type for a given system class.
	/// The handle's type name is created by appending 'Handle' suffix to a given class name.
	/// For example: ic3SysDeclareHandle( MyType ) will produce a declaration for 'MyTypeHandle'.
	#define ic3SysDeclareHandle( pType ) \
		class pType; using pType##Handle = TSysHandle<::Ic3::System::pType>

	// These two types need to be visible everywhere.
	ic3SysDeclareHandle( SysContext );
	ic3SysDeclareHandle( SysObject );

	enum : exception_category_value_t
	{
		eExceptionCategorySystemCore    = CxDef::declareExceptionCategory( EExceptionBaseType::SYSTEM, 0x01 ),
		eExceptionCategorySystemDisplay = CxDef::declareExceptionCategory( EExceptionBaseType::SYSTEM, 0x02 ),
		eExceptionCategorySystemEvent   = CxDef::declareExceptionCategory( EExceptionBaseType::SYSTEM, 0x03 ),
		eExceptionCategorySystemFile    = CxDef::declareExceptionCategory( EExceptionBaseType::SYSTEM, 0x04 ),
		eExceptionCategorySystemMetal   = CxDef::declareExceptionCategory( EExceptionBaseType::SYSTEM, 0x05 ),
		eExceptionCategorySystemOpenGL  = CxDef::declareExceptionCategory( EExceptionBaseType::SYSTEM, 0x07 ),
		eExceptionCategorySystemWindow  = CxDef::declareExceptionCategory( EExceptionBaseType::SYSTEM, 0x09 ),
	};

	enum : exception_code_value_t
	{
		eEXCSystemInterfaceNotSupported = CxDef::declareExceptionCode( eExceptionCategorySystemCore, 0x04 ),
	};

	/// @brief Helper proxy-like base class for platform-specific types.
	/// @tparam TPBaseClass Base class to derive from.
	/// @tparam TPNativeData Native data type with platform-specific state.
	///
	/// Public classes in the System component (like File, Window, GLDriver etc.) are implemented at the platform
	/// level (Win32File, Win32Window, Win32GLDriver). These classes obviously carry some platform-specific data.
	/// This class serves as a helper which adds public mNativeData member of a specified type and inherits from
	/// a given base class. It enables much compact definition of platform-specific types - especially those, that
	/// only require native data (like Adapter/Output/VideoMode from the display system) - they can be defined as a
	/// simple typedef.
	template <typename TPBaseClass, typename TPNativeData>
	class NativeObject : public TPBaseClass
	{
	public:
		TPNativeData mNativeData;

	public:
		template <typename... TBaseArgs>
		explicit NativeObject( TBaseArgs && ...pBaseArgs )
		: TPBaseClass( std::forward<TBaseArgs>( pBaseArgs )... )
		{}

		virtual ~NativeObject() = default;
	};

	template <typename TPBaseClass>
	class NativeObject<TPBaseClass, void> : public TPBaseClass
	{
	public:
		template <typename... TBaseArgs>
		explicit NativeObject( TBaseArgs && ...pBaseArgs )
		: TPBaseClass( std::forward<TBaseArgs>( pBaseArgs )... )
		{}

		virtual ~NativeObject() = default;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PREREQUISITES_H__
