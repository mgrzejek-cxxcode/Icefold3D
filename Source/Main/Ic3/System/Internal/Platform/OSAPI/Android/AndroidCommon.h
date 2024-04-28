
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_COMMON_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_COMMON_H__

#include <Ic3/Platform/Android/AndroidNativeAppProxy.h>
#include <Ic3/System/Prerequisites.h>
#include <Ic3/System/Internal/Platform/Shared/Java/JavaCommon.h>
#include <Ic3/Math/Vector.h>

namespace Ic3::System
{

	class SysContext;
	class EventController;

	namespace Platform
	{

		enum : uint32
		{
			E_ANDROID_APP_STATE_USER_DATA_INDEX_SYS_CONTEXT,
			E_ANDROID_APP_STATE_USER_DATA_INDEX_EVENT_CONTROLLER
		};

		struct ASessionData
		{
			AndroidAppState * aCommonAppState = nullptr;
			ANativeWindow * aNativeWindow = nullptr;
		};

		struct AndroidNativeDataCommon
		{
		public:
			void setSessionData( ASessionData & pSessionData )
			{
				ic3DebugAssert( aSessionDataPtr == nullptr );
				aSessionDataPtr = &pSessionData;
			}

			void resetSessionData()
			{
				ic3DebugAssert( aSessionDataPtr != nullptr );
				aSessionDataPtr = nullptr;
			}

			ASessionData & getSessionData() const
			{
				ic3DebugAssert( aSessionDataPtr != nullptr );
				return *aSessionDataPtr;
			}

		private:
			ASessionData * aSessionDataPtr = nullptr;
		};

		IC3_SYSTEM_API_NODISCARD ASessionData & androidGetASessionData( SysContext & pSysContext );

		IC3_SYSTEM_API_NODISCARD inline ASessionData & androidGetASessionData( const AndroidNativeDataCommon & pNativeData )
		{
			return pNativeData.getSessionData();
		}

		template <typename TNativeData>
		IC3_SYSTEM_API_NODISCARD inline ASessionData & androidGetASessionData( const NativeObject<TNativeData> & pNativeObject )
		{
			return androidGetASessionData( static_cast<AndroidNativeDataCommon>( pNativeObject.mNativeData ) );
		}
		
	}

	template <typename TBaseType, typename TNativeData>
	class AndroidNativeObject : public TBaseType, public NativeObject<TNativeData>
	{
	public:
		template <typename... TBaseTypeArgs>
		explicit AndroidNativeObject( SysContextHandle pSysContext, TBaseTypeArgs && ...pBaseTypeArgs )
		: TBaseType( pSysContext, std::forward<TBaseTypeArgs>( pBaseTypeArgs )... )
		{
			this->mNativeData.setSessionData( Platform::androidGetASessionData( *pSysContext ) );
		}

		template <typename TParentSysObject, typename... TBaseTypeArgs>
		explicit AndroidNativeObject( TParentSysObject & pParentSysObject, TBaseTypeArgs && ...pBaseTypeArgs )
		: TBaseType( pParentSysObject, std::forward<TBaseTypeArgs>( pBaseTypeArgs )... )
		{
			this->mNativeData.setSessionData( Platform::androidGetASessionData( pParentSysObject ) );
		}

		template <typename TParentSysObject, typename... TBaseTypeArgs>
		explicit AndroidNativeObject( SysHandle<TParentSysObject> pParentSysObject, TBaseTypeArgs && ...pBaseTypeArgs )
		: TBaseType( pParentSysObject, std::forward<TBaseTypeArgs>( pBaseTypeArgs )... )
		{
			this->mNativeData.setSessionData( Platform::androidGetASessionData( *pParentSysObject ) );
		}

		virtual ~AndroidNativeObject()
		{
			this->mNativeData.resetSessionData();
		}
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_COMMON_H__
