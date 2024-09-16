
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_X11_COMMON_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_X11_COMMON_H__

#include <Ic3/System/Prerequisites.h>
#include <X11/X.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>

namespace Ic3::System
{

	class SysContext;

	namespace Platform
	{

		using XDisplay = Display *;
		using XWindow = XID;
		using XColormap = XID;

		// Having these pretty common words replaced with a numbers tends
		// to be an issue, so we use some explicit enum constants instead
		// and remove them from the global scope.
		enum : XID
		{
			eXIDAlways = Always,
			eXIDNone = None,
			eXIDSuccess = Success,
		};

		#undef Always
		#undef None
		#undef Success

		inline constexpr auto cxX11DisplayNull = static_cast<XDisplay>( nullptr );

		struct X11SessionInfo
		{
			int mConnectionNumber;
			std::string mVendorName;
			std::string mDisplayString;
		};

		struct X11SessionData
		{
			struct AtomCache
			{
				Atom wmProtocol = 0;
				Atom wmProtocolDelete = 0;
				Atom wmProtocolDestroy = 0;
				Atom wmState = 0;
				Atom wmStateFullscreen = 0;
			};

			XDisplay mDisplay = cxX11DisplayNull;
			XWindow mRootWindowXID = eXIDNone;
			int mScreenIndex = -1;
			AtomCache mAtomCache;
			X11SessionInfo mSessionInfo;
		};

		struct X11NativeDataCommon
		{
		public:
			void setSessionData( X11SessionData & pSessionData )
			{
				ic3DebugAssert( _xSessionDataPtr == nullptr );
				_xSessionDataPtr = &pSessionData;
			}

			void resetSessionData()
			{
				ic3DebugAssert( _xSessionDataPtr != nullptr );
				_xSessionDataPtr = nullptr;
			}

			IC3_ATTR_NO_DISCARD X11SessionData & getSessionData() const
			{
				ic3DebugAssert( _xSessionDataPtr != nullptr );
				return *_xSessionDataPtr;
			}

		private:
			X11SessionData * _xSessionDataPtr = nullptr;
		};

		IC3_SYSTEM_API_NODISCARD X11SessionData & x11GetXSessionData( SysContext & pSysContext );

		IC3_SYSTEM_API_NODISCARD inline X11SessionData & x11GetXSessionData( const X11NativeDataCommon & pNativeData )
		{
			return pNativeData.getSessionData();
		}

		template <typename TBaseType, typename TNativeData>
		IC3_SYSTEM_API_NODISCARD inline X11SessionData & x11GetXSessionData( const NativeObject<TBaseType, TNativeData> & pNativeObject )
		{
			return x11GetXSessionData( static_cast<X11NativeDataCommon>( pNativeObject.mNativeData ) );
		}

	}

	template <typename TBaseType, typename TNativeData>
	class X11NativeObject : public NativeObject<TBaseType, TNativeData>
	{
	public:
		template <typename... TBaseTypeArgs>
		explicit X11NativeObject( SysContextHandle pSysContext, TBaseTypeArgs && ...pBaseTypeArgs )
		: NativeObject<TBaseType, TNativeData>( pSysContext, std::forward<TBaseTypeArgs>( pBaseTypeArgs )... )
		{
			this->mNativeData.setSessionData( Platform::x11GetXSessionData( *pSysContext ) );
		}

		template <typename TParentSysObject, typename... TBaseTypeArgs>
		explicit X11NativeObject( TParentSysObject & pParentSysObject, TBaseTypeArgs && ...pBaseTypeArgs )
		: NativeObject<TBaseType, TNativeData>( pParentSysObject, std::forward<TBaseTypeArgs>( pBaseTypeArgs )... )
		{
			this->mNativeData.setSessionData( Platform::x11GetXSessionData( pParentSysObject ) );
		}

		template <typename TParentSysObject, typename... TBaseTypeArgs>
		explicit X11NativeObject( TSysHandle<TParentSysObject> pParentSysObject, TBaseTypeArgs && ...pBaseTypeArgs )
		: NativeObject<TBaseType, TNativeData>( pParentSysObject, std::forward<TBaseTypeArgs>( pBaseTypeArgs )... )
		{
			this->mNativeData.setSessionData( Platform::x11GetXSessionData( *pParentSysObject ) );
		}

		virtual ~X11NativeObject()
		{
			this->mNativeData.resetSessionData();
		}
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_X11_COMMON_H__
