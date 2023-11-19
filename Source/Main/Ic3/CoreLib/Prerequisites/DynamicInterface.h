#pragma once

#ifndef __IC3_CORELIB_DYNAMIC_INTERFACE_H__
#define __IC3_CORELIB_DYNAMIC_INTERFACE_H__

#include <memory>

namespace Ic3
{

	template <typename TClass>
	using SharedHandle = std::shared_ptr<TClass>;

	template <typename TClass>
	using WeakHandle = std::weak_ptr<TClass>;

	inline constexpr std::nullptr_t cvNullHandle { nullptr };

	class DynamicInterface : public std::enable_shared_from_this<DynamicInterface>
	{
	public:
		virtual ~DynamicInterface() = default;

		template <typename TInterfaceSubClass>
		IC3_ATTR_NO_DISCARD TInterfaceSubClass * getInterface()
		{
		#if( IC3_DEBUG )
			return dynamic_cast_check<TInterfaceSubClass *>( this );
		#else
			return static_cast<TInterfaceSubClass *>( this );
		#endif
		}

		template <typename TInterfaceSubClass>
		IC3_ATTR_NO_DISCARD const TInterfaceSubClass * getInterface() const
		{
		#if( IC3_DEBUG )
		    return dynamic_cast_check<const TInterfaceSubClass *>( this );
		#else
		    return static_cast<const TInterfaceSubClass *>( this );
		#endif
		}

		template <typename TInterfaceSubClass>
		IC3_ATTR_NO_DISCARD TInterfaceSubClass * queryInterface()
		{
		#if( IC3_DEBUG )
			return dynamic_cast_check<TInterfaceSubClass *>( this );
		#else
			return dynamic_cast_throw<TInterfaceSubClass *>( this );
		#endif
		}

		template <typename TInterfaceSubClass>
		IC3_ATTR_NO_DISCARD const TInterfaceSubClass * queryInterface() const
		{
		#if( IC3_DEBUG )
			return dynamic_cast_check<const TInterfaceSubClass *>( this );
		#else
			return dynamic_cast_throw<const TInterfaceSubClass *>( this );
		#endif
		}

		template <typename TInterfaceSubClass>
		IC3_ATTR_NO_DISCARD SharedHandle<TInterfaceSubClass> getHandle()
		{
		#if( IC3_DEBUG )
			return dynamic_ptr_cast_check<TInterfaceSubClass>( shared_from_this() );
		#else
			return std::static_pointer_cast<TInterfaceSubClass>( shared_from_this() );
		#endif
		}

		template <typename TInterfaceSubClass>
		IC3_ATTR_NO_DISCARD SharedHandle<TInterfaceSubClass> queryHandle()
		{
		#if( IC3_DEBUG )
			return dynamic_ptr_cast_throw<TInterfaceSubClass>( shared_from_this() );
		#else
			return std::static_pointer_cast<TInterfaceSubClass>( shared_from_this() );
		#endif
		}

	private:
		using std::enable_shared_from_this<DynamicInterface>::shared_from_this;
		using std::enable_shared_from_this<DynamicInterface>::weak_from_this;
	};

	template <typename TClass, typename... TArgs>
	IC3_ATTR_NO_DISCARD inline SharedHandle<TClass> createDynamicInterfaceObject( TArgs && ...pArgs )
	{
		auto objectHandle = std::make_shared<TClass>( std::forward<TArgs>( pArgs )... );
	#if( IC3_DEBUG )
		// This will trigger a compile-time error if TClass is not a subclass of DynamicInterface.
		static_cast<DynamicInterface *>( objectHandle.get() );
	#endif
		return objectHandle;
	}

    template <typename TClass, typename TDeleter, typename... TArgs>
    IC3_ATTR_NO_DISCARD inline SharedHandle<TClass> createDynamicInterfaceObjectWithDeleter( TDeleter pDeleter, TArgs && ...pArgs )
    {
        auto objectHandle = std::shared_ptr<TClass>{ new TClass( std::forward<TArgs>( pArgs )... ), std::forward<TDeleter>( pDeleter ) };
    #if( IC3_DEBUG )
        // This will trigger a compile-time error if TClass is not a subclass of DynamicInterface.
        static_cast<DynamicInterface *>( objectHandle.get() );
    #endif
        return objectHandle;
    }

	template <typename TResultType, typename TInputType>
	IC3_ATTR_NO_DISCARD inline std::unique_ptr<TResultType> moveInterfaceUniquePtr( std::unique_ptr<TInputType> pUPtr )
	{
		std::unique_ptr<TResultType> result;
		if( pUPtr )
		{
			if( TResultType * targetPtr = pUPtr->template queryInterface<TResultType>() )
			{
				result = std::unique_ptr<TResultType>{ targetPtr };
				pUPtr.release();
			}
		}
		return result;
	}

	template <typename TInterfaceSubClass, typename TInputType>
	IC3_ATTR_NO_DISCARD inline TInterfaceSubClass * dynamic_interface_cast_get( TInputType * pInterfacePtr )
	{
		return reinterpret_cast<DynamicInterface *>( pInterfacePtr )->template getInterface<TInterfaceSubClass>();
	}

	template <typename TInterfaceSubClass, typename TInputType>
	IC3_ATTR_NO_DISCARD inline const TInterfaceSubClass * dynamic_interface_cast_get( const TInputType * pInterfacePtr )
	{
		return reinterpret_cast<const DynamicInterface *>( pInterfacePtr )->template getInterface<TInterfaceSubClass>();
	}

	template <typename TInterfaceSubClass, typename TInputType>
	IC3_ATTR_NO_DISCARD inline TInterfaceSubClass * dynamic_interface_cast_query( TInputType * pInterfacePtr )
	{
		return reinterpret_cast<DynamicInterface *>( pInterfacePtr )->template queryInterface<TInterfaceSubClass>();
	}

	template <typename TInterfaceSubClass, typename TInputType>
	IC3_ATTR_NO_DISCARD inline const TInterfaceSubClass * dynamic_interface_cast_query( const TInputType * pInterfacePtr )
	{
		return reinterpret_cast<const DynamicInterface *>( pInterfacePtr )->template queryInterface<TInterfaceSubClass>();
	}

#define ic3DeclareClassHandle( pClassName ) \
    class pClassName; \
    using pClassName##Handle = SharedHandle<pClassName>; \
    using pClassName##WeakHandle = WeakHandle<pClassName>

#define ic3DeclareInterfaceHandle( pInterfaceName ) \
    class pInterfaceName; \
    using pInterfaceName##Handle = SharedHandle<pInterfaceName>; \
    using pInterfaceName##WeakHandle = WeakHandle<pInterfaceName>

#define ic3DeclareTypedefHandle( pAliasName, pTypeName ) \
    using pAliasName = pTypeName; \
    using pAliasName##Handle = SharedHandle<pAliasName>; \
    using pAliasName##WeakHandle = WeakHandle<pAliasName>

}

#endif // __IC3_CORELIB_DYNAMIC_INTERFACE_H__
