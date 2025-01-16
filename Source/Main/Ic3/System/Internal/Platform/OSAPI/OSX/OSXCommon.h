
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_OSX_COMMON_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_OSX_COMMON_H__

#include <Ic3/System/Prerequisites.h>
#include <cppx/bitmaskAtomic.h>

#import <AppKit/NSApplication.h>
#import <CoreGraphics/CGError.h>

namespace Ic3::System
{

	class OSXSysContext;

	namespace Platform
	{

		enum EOSXCommonStateFlags : uint32
		{
			eOSXCommonStateAppDelegateInitializedBit = 0x0001,
			eOSXCommonStateAppFinishedLaunchingBit = 0x0002
		};

		struct OSXSharedData
		{
			cppx::atomic_bitmask<EOSXCommonStateFlags> stateFlags = 0u;
		};

		struct OSXNativeDataCommon
		{
		public:
			void SetSharedData( OSXSharedData & pSharedData )
			{
				ic3DebugAssert( _osxSharedDataPtr == nullptr );
				_osxSharedDataPtr = &pSharedData;
			}

			void ResetSharedData()
			{
				ic3DebugAssert( _osxSharedDataPtr != nullptr );
				_osxSharedDataPtr = nullptr;
			}

			CPPX_ATTR_NO_DISCARD OSXSharedData & GetSharedData() const
			{
				ic3DebugAssert( _osxSharedDataPtr != nullptr );
				return *_osxSharedDataPtr;
			}

		private:
			OSXSharedData * _osxSharedDataPtr = nullptr;
		};

		IC3_SYSTEM_API_NODISCARD OSXSharedData & OSXGetOSXSharedData( SysContext & pSysContext );

		IC3_SYSTEM_API_NODISCARD OSXSharedData & OSXGetOSXSharedData( OSXSysContext & pSysContext );

		IC3_SYSTEM_API_NODISCARD inline OSXSharedData & OSXGetOSXSharedData( const OSXNativeDataCommon & pNativeData )
		{
			return pNativeData.GetSharedData();
		}

		template <typename TBaseType, typename TNativeData>
		IC3_SYSTEM_API_NODISCARD inline OSXSharedData & OSXGetOSXSharedData( const NativeObject<TBaseType, TNativeData> & pNativeObject )
		{
			return OSXGetOSXSharedData( static_cast<OSXNativeDataCommon>( pNativeObject.mNativeData ) );
		}

		IC3_SYSTEM_API_NODISCARD bool OSXCheckAppKitFrameworkVersion( NSAppKitVersion pRequiredVersion );

		IC3_SYSTEM_API_NODISCARD const char * OSXQueryCGErrorMessage( CGError pCGError );

		bool OSXNibLoadMenuNibFile();

		void OSXNibCreateDefaultApplicationMenu();

	}


	template <typename TBaseType, typename TNativeData>
	class OSXNativeObject : public NativeObject<TBaseType, TNativeData>
	{
	public:
		template <typename... TBaseTypeArgs>
		explicit OSXNativeObject( SysContextHandle pSysContext, TBaseTypeArgs && ...pBaseTypeArgs )
		: NativeObject<TBaseType, TNativeData>( pSysContext, std::forward<TBaseTypeArgs>( pBaseTypeArgs )... )
		{
			this->mNativeData.SetSharedData( Platform::OSXGetOSXSharedData( *pSysContext ) );
		}

		template <typename TParentSysObject, typename... TBaseTypeArgs>
		explicit OSXNativeObject( TParentSysObject & pParentSysObject, TBaseTypeArgs && ...pBaseTypeArgs )
		: NativeObject<TBaseType, TNativeData>( pParentSysObject, std::forward<TBaseTypeArgs>( pBaseTypeArgs )... )
		{
			this->mNativeData.SetSharedData( Platform::OSXGetOSXSharedData( pParentSysObject ) );
		}

		template <typename TParentSysObject, typename... TBaseTypeArgs>
		explicit OSXNativeObject( TSysHandle<TParentSysObject> pParentSysObject, TBaseTypeArgs && ...pBaseTypeArgs )
		: NativeObject<TBaseType, TNativeData>( pParentSysObject, std::forward<TBaseTypeArgs>( pBaseTypeArgs )... )
		{
			this->mNativeData.SetSharedData( Platform::OSXGetOSXSharedData( *pParentSysObject ) );
		}

		virtual ~OSXNativeObject()
		{
			this->mNativeData.ResetSharedData();
		}
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_OSX_COMMON_H__
