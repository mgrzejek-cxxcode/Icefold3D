#pragma once

#ifndef __IC3_CORELIB_DYNAMIC_INTERFACE_H__
#define __IC3_CORELIB_DYNAMIC_INTERFACE_H__

#include <memory>

namespace Ic3
{

	template <typename TPClass>
	using TSharedHandle = std::shared_ptr<TPClass>;

	template <typename TPClass>
	using TWeakHandle = std::weak_ptr<TPClass>;

	constexpr std::nullptr_t cvNullHandle = nullptr;

	class IDynamicObject : public std::enable_shared_from_this<IDynamicObject>
	{
	public:
		virtual ~IDynamicObject() = default;

		template <typename TPInterfaceSubClass>
		CPPX_ATTR_NO_DISCARD TPInterfaceSubClass * GetInterface()
		{
		#if( IC3_DEBUG )
			return dynamic_cast_check<TPInterfaceSubClass *>( this );
		#else
			return static_cast<TPInterfaceSubClass *>( this );
		#endif
		}

		template <typename TPInterfaceSubClass>
		CPPX_ATTR_NO_DISCARD const TPInterfaceSubClass * GetInterface() const
		{
		#if( IC3_DEBUG )
		    return dynamic_cast_check<const TPInterfaceSubClass *>( this );
		#else
		    return static_cast<const TPInterfaceSubClass *>( this );
		#endif
		}

		template <typename TPInterfaceSubClass>
		CPPX_ATTR_NO_DISCARD TPInterfaceSubClass * QueryInterface()
		{
		#if( IC3_DEBUG )
			return dynamic_cast_check<TPInterfaceSubClass *>( this );
		#else
			return dynamic_cast_throw<TPInterfaceSubClass *>( this );
		#endif
		}

		template <typename TPInterfaceSubClass>
		CPPX_ATTR_NO_DISCARD const TPInterfaceSubClass * QueryInterface() const
		{
		#if( IC3_DEBUG )
			return dynamic_cast_check<const TPInterfaceSubClass *>( this );
		#else
			return dynamic_cast_throw<const TPInterfaceSubClass *>( this );
		#endif
		}

		template <typename TPInterfaceSubClass>
		CPPX_ATTR_NO_DISCARD TSharedHandle<TPInterfaceSubClass> GetHandle()
		{
		#if( IC3_DEBUG )
			return dynamic_ptr_cast_check<TPInterfaceSubClass>( shared_from_this() );
		#else
			return std::static_pointer_cast<TPInterfaceSubClass>( shared_from_this() );
		#endif
		}

		template <typename TPInterfaceSubClass>
		CPPX_ATTR_NO_DISCARD TSharedHandle<const TPInterfaceSubClass> GetHandle() const
		{
		#if( IC3_DEBUG )
			return dynamic_ptr_cast_check<const TPInterfaceSubClass>( shared_from_this() );
		#else
			return std::static_pointer_cast<const TPInterfaceSubClass>( shared_from_this() );
		#endif
		}

		template <typename TPInterfaceSubClass>
		CPPX_ATTR_NO_DISCARD TSharedHandle<TPInterfaceSubClass> QueryHandle()
		{
		#if( IC3_DEBUG )
			return dynamic_ptr_cast_throw<TPInterfaceSubClass>( shared_from_this() );
		#else
			return std::static_pointer_cast<TPInterfaceSubClass>( shared_from_this() );
		#endif
		}

		template <typename TPInterfaceSubClass>
		CPPX_ATTR_NO_DISCARD TSharedHandle<const TPInterfaceSubClass> QueryHandle() const
		{
		#if( IC3_DEBUG )
			return dynamic_ptr_cast_throw<const TPInterfaceSubClass>( shared_from_this() );
		#else
			return std::static_pointer_cast<const TPInterfaceSubClass>( shared_from_this() );
		#endif
		}

	private:
		using std::enable_shared_from_this<IDynamicObject>::shared_from_this;
		using std::enable_shared_from_this<IDynamicObject>::weak_from_this;
	};

	template <typename TPClass, typename... TPArgs>
	CPPX_ATTR_NO_DISCARD inline TSharedHandle<TPClass> CreateDynamicObject( TPArgs && ...pArgs )
	{
		auto objectHandle = std::make_shared<TPClass>( std::forward<TPArgs>( pArgs )... );
	#if( IC3_DEBUG )
		// This will trigger a compile-time error if TPClass is not a subclass of IDynamicObject.
		static_cast<IDynamicObject *>( objectHandle.get() );
	#endif
		return objectHandle;
	}

    template <typename TPClass, typename TPDeleter, typename... TPArgs>
    CPPX_ATTR_NO_DISCARD inline TSharedHandle<TPClass> CreateDynamicObjectWithDeleter( TPDeleter pDeleter, TPArgs && ...pArgs )
    {
        auto objectHandle = std::shared_ptr<TPClass>{ new TPClass( std::forward<TPArgs>( pArgs )... ), std::forward<TPDeleter>( pDeleter ) };
    #if( IC3_DEBUG )
        // This will trigger a compile-time error if TPClass is not a subclass of IDynamicObject.
        static_cast<IDynamicObject *>( objectHandle.get() );
    #endif
        return objectHandle;
    }

	template <typename TPResultType, typename TPInputType>
	CPPX_ATTR_NO_DISCARD inline std::unique_ptr<TPResultType> MoveInterfaceUniquePtr( std::unique_ptr<TPInputType> pUPtr )
	{
		std::unique_ptr<TPResultType> result;
		if( pUPtr )
		{
			if( TPResultType * targetPtr = pUPtr->template QueryInterface<TPResultType>() )
			{
				result = std::unique_ptr<TPResultType>{ targetPtr };
				pUPtr.release();
			}
		}
		return result;
	}

	template <typename TPInterfaceSubClass, typename TPInputType>
	CPPX_ATTR_NO_DISCARD inline TPInterfaceSubClass * dynamic_interface_cast_get( TPInputType * pInterfacePtr )
	{
		return reinterpret_cast<IDynamicObject *>( pInterfacePtr )->template GetInterface<TPInterfaceSubClass>();
	}

	template <typename TPInterfaceSubClass, typename TPInputType>
	CPPX_ATTR_NO_DISCARD inline const TPInterfaceSubClass * dynamic_interface_cast_get( const TPInputType * pInterfacePtr )
	{
		return reinterpret_cast<const IDynamicObject *>( pInterfacePtr )->template GetInterface<TPInterfaceSubClass>();
	}

	template <typename TPInterfaceSubClass, typename TPInputType>
	CPPX_ATTR_NO_DISCARD inline TPInterfaceSubClass * dynamic_interface_cast_query( TPInputType * pInterfacePtr )
	{
		return reinterpret_cast<IDynamicObject *>( pInterfacePtr )->template QueryInterface<TPInterfaceSubClass>();
	}

	template <typename TPInterfaceSubClass, typename TPInputType>
	CPPX_ATTR_NO_DISCARD inline const TPInterfaceSubClass * dynamic_interface_cast_query( const TPInputType * pInterfacePtr )
	{
		return reinterpret_cast<const IDynamicObject *>( pInterfacePtr )->template QueryInterface<TPInterfaceSubClass>();
	}

#define Ic3DeclareClassHandle( pClassName ) \
    class pClassName; \
    using pClassName##Handle = TSharedHandle<pClassName>; \
    using pClassName##TWeakHandle = TWeakHandle<pClassName>; \
    using Const##pClassName##Handle = TSharedHandle<const pClassName>; \
    using Const##pClassName##WeakHandle = TWeakHandle<const pClassName>

#define Ic3DeclareInterfaceHandle( pInterfaceName ) \
    class pInterfaceName; \
    using pInterfaceName##Handle = TSharedHandle<pInterfaceName>; \
    using pInterfaceName##WeakHandle = TWeakHandle<pInterfaceName>; \
    using ConstpInterfaceName##Handle = TSharedHandle<const pInterfaceName>; \
    using ConstpInterfaceName##WeakHandle = TWeakHandle<const pInterfaceName>

#define Ic3DeclareTypedefHandle( pAliasName, pTypeName ) \
    using pAliasName = pTypeName; \
    using pAliasName##Handle = TSharedHandle<pAliasName>; \
    using pAliasName##WeakHandle = TWeakHandle<pAliasName>; \
    using Const##pAliasName##Handle = TSharedHandle<const pAliasName>; \
    using Const##pAliasName##WeakHandle = TWeakHandle<const pAliasName>

}

#endif // __IC3_CORELIB_DYNAMIC_INTERFACE_H__
