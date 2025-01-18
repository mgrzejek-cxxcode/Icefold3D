
#ifndef __IC3_SCRIPT_SCRIPT_MODULE_H__
#define __IC3_SCRIPT_SCRIPT_MODULE_H__

#include "Prerequisites.h"
#include <cppx/hash.h>
#include <cppx/sortedArray.h>
#include <cppx/stringUtils.h>

namespace Ic3::Script
{

	struct SmEntryPtrCompare;

	class SmClass;
	class SmEntry;
	class SmNamespace;
	class ScriptModule;
	class ScriptModuleBuilder;

	using SmEntryPtr = std::unique_ptr<SmEntry>;
	using SmEntryPtrArray = cppx::sorted_array<SmEntryPtr, SmEntryPtrCompare>;
	using sm_namespace_id = uint32;
	using sm_type_id = cppx::hash_object<cppx::hash_algo::fnv1a64>::value_type;

	enum class SmEntryType
	{
		Undefined,

		Namespace,
		BuiltinType,
		Class,
		Enum,

		Function,
		Constant,
		Variable,
	};

	class SmEntry
	{
		friend class SmAPI;

	public:
		SmEntryType const mEntryType = SmEntryType::Undefined;

		SmEntry * const mParent = nullptr;

		const std::string mName;

		const std::string mScopedName;

	public:
		SmEntry( SmEntryType pType, SmEntry * pParent, std::string pName );
		virtual ~SmEntry() = default;

	protected:
		void addChild( SmEntryPtr pChildEntry );

	protected:
		SmEntryPtrArray _childEntries;
	};

	class SmNamespace : public SmEntry
	{
	public:
		SmNamespace( SmEntry * pParent, std::string pName );
		virtual ~SmNamespace() = default;
	};

	class SmType : public SmEntry
	{
	public:
		SmType( SmEntryType pType, SmEntry * pParent, std::string pName );
		virtual ~SmType() = default;
	};

	struct SmTypeInfo
	{
		SmType * baseType = nullptr;
		bool isPointer;
		bool isReference;
		bool isConst;
	};

	struct SmValueInfo
	{
		SmTypeInfo tpi;
		bool isConst;
	};

	struct SmInstanceInfo : public SmValueInfo
	{
		SmTypeInfo tpi;
		bool isClassMember;
	};

	template <size_t tValuesNum>
	using SmValueInfoArray = std::array<SmValueInfo, tValuesNum>;

	class SmBuiltinType : public SmType
	{
	public:
		SmBuiltinType( SmEntry * pParent, std::string pName );
		virtual ~SmBuiltinType() = default;
	};

	class SmClass : public SmType
	{
	public:
		const cppx::array_view<SmClass> mBaseTypes;

	public:
		SmClass( SmEntry * pParent, std::string pName );
		virtual ~SmClass() = default;
	};

	class SmEnum : public SmType
	{
	public:
		SmEnum( SmEntry * pParent, std::string pName );
		virtual ~SmEnum() = default;
	};

	class SmFunction : public SmEntry
	{
	public:
		const SmValueInfo mReturnValue;

		const cppx::array_view<const SmValueInfo> mArguments;

	public:
		SmFunction(
			SmEntry * pParent,
			std::string pName,
			SmValueInfo pReturnValue,
			const SmValueInfo * pArguments );

		virtual ~SmFunction() = default;
	};

	template <size_t tArgumentsNum>
	class SmFunctionWithArgs : public SmFunction
	{
	public:
		using ArgumentArray = SmValueInfoArray<tArgumentsNum>;

		const ArgumentArray mArguments;

	public:
		SmFunctionWithArgs(
			SmEntry * pParent,
			std::string pName,
			SmValueInfo pReturnValue,
			std::initializer_list<SmValueInfo> pArguments )
		: SmFunction( pParent, std::move( pName ), std::move( pReturnValue ), mArguments.data() )
		, mArguments( pArguments )
		{}
	};

	template <>
	class SmFunctionWithArgs<0> : public SmFunction
	{
	public:
		SmFunctionWithArgs( SmEntry * pParent, std::string pName, SmValueInfo pReturnValue )
		: SmFunction( pParent, std::move( pName ), std::move( pReturnValue ), nullptr )
		{}
	};

	class SmValue : public SmEntry
	{
	public:
		const SmValueInfo mValueInfo;

	public:
		SmValue( SmEntry * pParent, std::string pName, SmValueInfo pValueInfo )
		: SmEntry( pValueInfo.isConst ? SmEntryType::Constant : SmEntryType::Variable, pParent, std::move( pName ) )
		, mValueInfo( std::move( pValueInfo ) )
		{}
	};

	template <typename TPValue>
	class SmConstant : public SmValue
	{
	public:
		using ValueGetCallback = std::function<const TPValue &()>;

		ValueGetCallback const mGet;

	public:
		SmConstant( SmEntry * pParent, std::string pName, SmType * pBaseType, const TPValue * pValuePtr )
		: SmValue( pParent, std::move( pName ), { pBaseType, true, false }, pValuePtr )
		, mGet( [pValuePtr]() -> const TPValue & { return *pValuePtr; } )
		{}
	};

	template <typename TClass, typename TPValue>
	class SmClassConstant : public SmValue
	{
	public:
		using ValueGetCallback = std::function<const TPValue &( TClass * )>;

		ValueGetCallback const mGet;

	public:
		SmClassConstant( SmEntry * pParent, std::string pName, SmType * pBaseType, const TPValue TClass::* pValuePtr )
		: SmValue( pParent, std::move( pName ), { pBaseType, true, true }, pValuePtr )
		{}
	};

	template <typename TPValue>
	class SmVariable : public SmValue
	{
	public:
		using ValueGetCallback = std::function<TPValue &()>;
		using ValueSetCallback = std::function<void( const TPValue & )>;

		ValueGetCallback const mGet;
		ValueSetCallback const mSet;

	public:
		SmVariable( SmEntry * pParent, std::string pName, SmType * pBaseType, TPValue * pValuePtr )
		: SmValue( pParent, std::move( pName ), { pBaseType, false, false } )
		, mGet( [pValuePtr]() -> const TPValue & { return *pValuePtr; } )
		, mSet( [pValuePtr]( const TPValue & pValue ) -> void { *pValuePtr = pValue; } )
		{}
	};

	template <typename TClass, typename TPValue>
	class SmClassVariable : public SmValue
	{
	public:
		using ValueGetCallback = std::function<TPValue &( TClass * )>;
		using ValueSetCallback = std::function<void( TClass *, const TPValue & )>;

		ValueGetCallback const mGet;
		ValueSetCallback const mSet;

	public:
		SmClassVariable( SmEntry * pParent, std::string pName, SmType * pBaseType, TPValue TClass::* pValuePtr )
		: SmValue( pParent, std::move( pName ), { pBaseType, false, true } )
		, mGet( [pValuePtr]( TClass * pClassInstance ) -> const TPValue & { return pClassInstance->*( pValuePtr ); } )
		, mSet( [pValuePtr]( TClass * pClassInstance, const TPValue & pValue ) -> void { pClassInstance->*( pValuePtr ) = pValue; } )
		{}
	};

	template <typename TPValue>
	class SmVariable<const TPValue>;

	class ScriptModule
	{
	public:
		using SmEntryMap = std::unordered_map<std::string_view, SmEntry *>;

	public:
	};

	class ScriptModuleBuilder
	{
	};

	struct SmEntryPtrCompare
	{
		bool operator()( const SmEntryPtr & pLhs, const SmEntryPtr & pRhs ) const
		{
			return ( pLhs->mEntryType < pRhs->mEntryType ) ||
			       ( ( pLhs->mEntryType == pRhs->mEntryType ) && ( pLhs->mName < pRhs->mName ) );
		}
	};

	template <const std::string_view & tParentScopeName, const std::string_view & tNestedScopeName>
	struct SmJoinScopedNames
	{
		static constexpr auto joinImpl() noexcept
		{
			constexpr size_t joinedLength = ( tParentScopeName.size() + 1 + tNestedScopeName.size() );
			std::array<char, joinedLength + 1> joinedData{};

			auto staticAppend = [joinedPos = 0, &joinedData]( const auto & pStrView ) mutable {
				for( auto ch : pStrView )
				{
					joinedData[joinedPos++] = ch;
				}
			};

			staticAppend( tParentScopeName );
			staticAppend( std::string_view( "." ) );
			staticAppend( tNestedScopeName );

			joinedData[joinedLength] = 0;

			return joinedData;
		}

		static constexpr auto _sJoinedStrData = joinImpl();

		static constexpr auto get() noexcept
		{
			return std::string_view{ _sJoinedStrData.data(), _sJoinedStrData.size() };
		}
	};

	template <typename T, SmEntryType>
	struct SmTypeTraits;

	template <typename T>
	struct SmTypeTraits<T, SmEntryType::Class>
	{
		static constexpr auto sEntryType = SmEntryType::Class;
	};

	template <sm_namespace_id>
	struct SmNamespaceMetaInfo;

#define Ic3DefineNamespaceMeta( pParentNamespaceID, pNamespaceID, pName )                          \
	template <> struct SmNamespaceMetaInfo<pNamespaceID> {                                         \
		static constexpr sm_namespace_id sID = pNamespaceID;                                       \
		static constexpr std::string_view sName = pName;                                           \
		static constexpr std::string_view sScopedName =                                            \
			SmJoinScopedNames<SmNamespaceMetaInfo<pParentNamespaceID>::sScopedName, sName>::get(); \
	};

#define Ic3DefineNamespaceMetaUnscoped( pNamespaceID, pName )                          \
	template <> struct SmNamespaceMetaInfo<pNamespaceID> {                                         \
		static constexpr sm_namespace_id sID = pNamespaceID;                                       \
		static constexpr std::string_view sName = pName;                                           \
		static constexpr std::string_view sScopedName = sName;                                     \
	};

	template <typename T>
	struct SmTypeMetaInfo;

#define Ic3SmDefineTypeMetaAutoNested( pParentType, pType, ... )                       \
	template <> struct SmTypeMetaInfo<pParentType::pType> {                            \
		static constexpr std::string_view sName = #pType;                              \
		static constexpr std::string_view sScopedName =                                \
			SmJoinScopedNames<SmTypeMetaInfo<pParentType>::sScopedName, sName>::get(); \
	};

#define Ic3SmDefineTypeMetaAutoUnscoped( pType, ... )          \
	template <> struct SmTypeMetaInfo<pType> {                 \
		static constexpr std::string_view sName = #pType;      \
		static constexpr std::string_view sScopedName = sName; \
	};

	template <typename T>
	SmTypeMetaInfo<T> smGetTypeMetaInfo()
	{
		return SmTypeMetaInfo<T>{};
	}

	class SomeInterface
	{
	public:
		class ExampleClass
		{
		};
	};

	Ic3SmDefineTypeMetaAutoUnscoped( SomeInterface );
	Ic3SmDefineTypeMetaAutoNested( SomeInterface, ExampleClass );

	class SmAPI
	{
	public:
		IC3_SCRIPT_API SmNamespace & defineNamespace( SmNamespace * pParent, std::string pName );

		template <typename TParent, typename TClass, typename... TBaseTypes>
		SmClass * defineClass( SmEntry * pParent, std::string pName )
		{
			auto classPtr = std::make_unique<SmClass>( nullptr, "" );
			return nullptr;
		}
	};

} // namespace Ic3::Script

#endif //__IC3_SCRIPT_SCRIPT_MODULE_H__
