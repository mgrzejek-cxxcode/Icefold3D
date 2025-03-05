
#ifndef __IC3_SYSTEM_PREREQUISITES_H__
#define __IC3_SYSTEM_PREREQUISITES_H__

#include <Ic3/CoreLib/Exception.h>

#include <cppx/bitmask.h>
#include <cppx/utilities.h>
#include <cppx/version.h>

#include <memory>
#include <string>
#include <utility>

#if( IC3_BUILD_STATIC )
#  define IC3_SYSTEM_API
#  define IC3_SYSTEM_CLASS
#  define IC3_SYSTEM_OBJ extern
#else
#  if( IC3_SYSTEM_BUILD )
#	define IC3_SYSTEM_API   PCL_ATTR_DLL_EXPORT
#	define IC3_SYSTEM_CLASS PCL_ATTR_DLL_EXPORT
#	define IC3_SYSTEM_OBJ   PCL_ATTR_DLL_EXPORT extern
#  else
#	define IC3_SYSTEM_API   PCL_ATTR_DLL_IMPORT
#	define IC3_SYSTEM_CLASS PCL_ATTR_DLL_IMPORT
#	define IC3_SYSTEM_OBJ   PCL_ATTR_DLL_IMPORT extern
#  endif
#endif

#define IC3_SYSTEM_API_NODISCARD  CPPX_ATTR_NO_DISCARD IC3_SYSTEM_API

namespace Ic3::System
{

	Ic3EnableCustomExceptionSupport();
	Ic3EnableEnumTypeInfoSupport();

	///
	#define Ic3TypeInfoEnumDeclareSystem( TPEnum ) Ic3TypeInfoEnumDeclareAPI( TPEnum, IC3_SYSTEM_API )

	template <typename TPClass>
	using TSysHandle = ::Ic3::TSharedHandle<TPClass>;

	/// @brief Declares the handle type for a given system class.
	/// The handle's type name is created by appending 'Handle' suffix to a given class name.
	/// For example: Ic3SysDeclareHandle( MyType ) will produce a declaration for 'MyTypeHandle'.
	#define Ic3SysDeclareHandle( pType ) \
		class pType; using pType##Handle = TSysHandle<::Ic3::System::pType>

	// These two types need to be visible everywhere.
	Ic3SysDeclareHandle( SysContext );
	Ic3SysDeclareHandle( SysObject );

	enum : exception_category_value_t
	{
		eExceptionCategorySystemCore    = Ic3::CXU::DeclareExceptionCategory( EExceptionBaseType::System, 0x01 ),
		eExceptionCategorySystemDisplay = Ic3::CXU::DeclareExceptionCategory( EExceptionBaseType::System, 0x02 ),
		eExceptionCategorySystemEvent   = Ic3::CXU::DeclareExceptionCategory( EExceptionBaseType::System, 0x03 ),
		eExceptionCategorySystemFile    = Ic3::CXU::DeclareExceptionCategory( EExceptionBaseType::System, 0x04 ),
		eExceptionCategorySystemMetal   = Ic3::CXU::DeclareExceptionCategory( EExceptionBaseType::System, 0x05 ),
		eExceptionCategorySystemOpenGL  = Ic3::CXU::DeclareExceptionCategory( EExceptionBaseType::System, 0x07 ),
		eExceptionCategorySystemWindow  = Ic3::CXU::DeclareExceptionCategory( EExceptionBaseType::System, 0x09 ),
	};

	enum : exception_code_value_t
	{
		eEXCSystemInterfaceNotSupported = Ic3::CXU::DeclareExceptionCode( eExceptionCategorySystemCore, 0x04 ),
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
