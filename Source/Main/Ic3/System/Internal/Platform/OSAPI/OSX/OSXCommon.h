
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_OSX_COMMON_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_OSX_COMMON_H__

#include <Ic3/System/Prerequisites.h>
#include <Ic3/Cppx/BitmaskAtomic.h>

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
			Cppx::TAtomicBitmask<EOSXCommonStateFlags> mStateFlags = 0u;
		};

		struct OSXNativeDataCommon
		{
		public:
			void setSharedData( OSXSharedData & pSharedData )
			{
				ic3DebugAssert( _osxSharedDataPtr == nullptr );
				_osxSharedDataPtr = &pSharedData;
			}

			void resetSharedData()
			{
				ic3DebugAssert( _osxSharedDataPtr != nullptr );
				_osxSharedDataPtr = nullptr;
			}

			IC3_ATTR_NO_DISCARD OSXSharedData & getSharedData() const
			{
				ic3DebugAssert( _osxSharedDataPtr != nullptr );
				return *_osxSharedDataPtr;
			}

		private:
			OSXSharedData * _osxSharedDataPtr = nullptr;
		};

		IC3_SYSTEM_API_NODISCARD OSXSharedData & osxGetOSXSharedData( SysContext & pSysContext );

		IC3_SYSTEM_API_NODISCARD OSXSharedData & osxGetOSXSharedData( OSXSysContext & pSysContext );

		IC3_SYSTEM_API_NODISCARD inline OSXSharedData & osxGetOSXSharedData( const OSXNativeDataCommon & pNativeData )
		{
			return pNativeData.getSharedData();
		}

		template <typename TBaseType, typename TNativeData>
		IC3_SYSTEM_API_NODISCARD inline OSXSharedData & osxGetOSXSharedData( const NativeObject<TBaseType, TNativeData> & pNativeObject )
		{
			return osxGetOSXSharedData( static_cast<OSXNativeDataCommon>( pNativeObject.mNativeData ) );
		}

		IC3_SYSTEM_API_NODISCARD bool osxCheckAppKitFrameworkVersion( NSAppKitVersion pRequiredVersion );

		IC3_SYSTEM_API_NODISCARD const char * osxQueryCGErrorMessage( CGError pCGError );

		bool osxNibLoadMenuNibFile();

		void osxNibCreateDefaultApplicationMenu();

	}


	template <typename TBaseType, typename TNativeData>
	class OSXNativeObject : public NativeObject<TBaseType, TNativeData>
	{
	public:
		template <typename... TBaseTypeArgs>
		explicit OSXNativeObject( SysContextHandle pSysContext, TBaseTypeArgs && ...pBaseTypeArgs )
		: NativeObject<TBaseType, TNativeData>( pSysContext, std::forward<TBaseTypeArgs>( pBaseTypeArgs )... )
		{
			this->mNativeData.setSharedData( Platform::osxGetOSXSharedData( *pSysContext ) );
		}

		template <typename TParentSysObject, typename... TBaseTypeArgs>
		explicit OSXNativeObject( TParentSysObject & pParentSysObject, TBaseTypeArgs && ...pBaseTypeArgs )
		: NativeObject<TBaseType, TNativeData>( pParentSysObject, std::forward<TBaseTypeArgs>( pBaseTypeArgs )... )
		{
			this->mNativeData.setSharedData( Platform::osxGetOSXSharedData( pParentSysObject ) );
		}

		template <typename TParentSysObject, typename... TBaseTypeArgs>
		explicit OSXNativeObject( TSysHandle<TParentSysObject> pParentSysObject, TBaseTypeArgs && ...pBaseTypeArgs )
		: NativeObject<TBaseType, TNativeData>( pParentSysObject, std::forward<TBaseTypeArgs>( pBaseTypeArgs )... )
		{
			this->mNativeData.setSharedData( Platform::osxGetOSXSharedData( *pParentSysObject ) );
		}

		virtual ~OSXNativeObject()
		{
			this->mNativeData.resetSharedData();
		}
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_OSX_COMMON_H__
