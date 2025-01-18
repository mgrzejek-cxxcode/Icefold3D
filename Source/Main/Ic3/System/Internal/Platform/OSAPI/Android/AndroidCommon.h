
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
			eAndroidAppStateUserDataIndexSysContext,
			eAndroidAppStateUserDataIndexEventController
		};

		struct ASessionData
		{
			AndroidAppState * aCommonAppState = nullptr;
			ANativeWindow * aNativeWindow = nullptr;
		};

		struct AndroidNativeDataCommon
		{
		public:
			void SetSessionData( ASessionData & pSessionData )
			{
				Ic3DebugAssert( _aSessionDataPtr == nullptr );
				_aSessionDataPtr = &pSessionData;
			}

			void ResetSessionData()
			{
				Ic3DebugAssert( _aSessionDataPtr != nullptr );
				_aSessionDataPtr = nullptr;
			}

			ASessionData & GetSessionData() const
			{
				Ic3DebugAssert( _aSessionDataPtr != nullptr );
				return *_aSessionDataPtr;
			}

		private:
			ASessionData * _aSessionDataPtr = nullptr;
		};

		IC3_SYSTEM_API_NODISCARD ASessionData & AndroidGetASessionData( SysContext & pSysContext );

		IC3_SYSTEM_API_NODISCARD inline ASessionData & AndroidGetASessionData( const AndroidNativeDataCommon & pNativeData )
		{
			return pNativeData.GetSessionData();
		}

		template <typename TNativeData>
		IC3_SYSTEM_API_NODISCARD inline ASessionData & AndroidGetASessionData( const NativeObject<TNativeData> & pNativeObject )
		{
			return AndroidGetASessionData( static_cast<AndroidNativeDataCommon>( pNativeObject.mNativeData ) );
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
			this->mNativeData.SetSessionData( Platform::AndroidGetASessionData( *pSysContext ) );
		}

		template <typename TParentSysObject, typename... TBaseTypeArgs>
		explicit AndroidNativeObject( TParentSysObject & pParentSysObject, TBaseTypeArgs && ...pBaseTypeArgs )
		: TBaseType( pParentSysObject, std::forward<TBaseTypeArgs>( pBaseTypeArgs )... )
		{
			this->mNativeData.SetSessionData( Platform::AndroidGetASessionData( pParentSysObject ) );
		}

		template <typename TParentSysObject, typename... TBaseTypeArgs>
		explicit AndroidNativeObject( TSysHandle<TParentSysObject> pParentSysObject, TBaseTypeArgs && ...pBaseTypeArgs )
		: TBaseType( pParentSysObject, std::forward<TBaseTypeArgs>( pBaseTypeArgs )... )
		{
			this->mNativeData.SetSessionData( Platform::AndroidGetASessionData( *pParentSysObject ) );
		}

		virtual ~AndroidNativeObject()
		{
			this->mNativeData.ResetSessionData();
		}
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_COMMON_H__
