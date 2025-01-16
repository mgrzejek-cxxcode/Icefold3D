
#include "OSXDisplaySystem.h"
#include <CoreGraphics/CGDisplayConfiguration.h>

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_OSX )
namespace Ic3::System
{

	OSXDisplayManager::OSXDisplayManager( SysContextHandle pSysContext )
	: OSXNativeObject( std::move( pSysContext ) )
	{
		_InitializeOSXDisplayManagerState();
	}

	OSXDisplayManager::~OSXDisplayManager() noexcept
	{
		_ReleaseOSXDisplayManagerState();
	}

	void OSXDisplayManager::_InitializeOSXDisplayManagerState()
	{
		CGDisplayCount activeDisplaysNum = 0u;
		auto cgResult = ::CGGetActiveDisplayList( 0, nullptr, &activeDisplaysNum );

		if( cgResult != kCGErrorSuccess )
		{
			ic3DebugInterrupt();
			return;
		}

		if( activeDisplaysNum == 0 )
		{
			return;
		}

		auto activeDisplayList = std::make_unique<CGDirectDisplayID[]>( activeDisplaysNum );
		cgResult = ::CGGetActiveDisplayList( activeDisplaysNum, &( activeDisplayList[0] ), &activeDisplaysNum );

		if( cgResult != kCGErrorSuccess )
		{
			ic3DebugInterrupt();
			return;
		}

		CGDirectDisplayID mainDisplayID = kCGNullDirectDisplay;
		for( CGDisplayCount displayIndex = 0; displayIndex < activeDisplaysNum; ++displayIndex )
		{
			const auto displayID = activeDisplayList[displayIndex];
			if( ::CGDisplayIsMain( displayID ) )
			{
				mainDisplayID = displayID;
				break;
			}
		}

		if( mainDisplayID == kCGNullDirectDisplay )
		{
			mainDisplayID = activeDisplayList[0];
		}

		mNativeData.mCGActiveDisplayList = std::move( activeDisplayList );
		mNativeData.mCGActiveDisplaysNum = activeDisplaysNum;
		mNativeData.mCGMainDisplayID = mainDisplayID;
	}
	
	void OSXDisplayManager::_ReleaseOSXDisplayManagerState()
	{}

	DisplayDriverHandle OSXDisplayManager::_NativeCreateDisplayDriver()
	{
		return CreateSysObject<OSXDisplayDriver>( GetHandle<OSXDisplayManager>() );
	}

	void OSXDisplayManager::_NativeQueryDefaultDisplayOffset( DisplayOffset & pOutOffset ) const
	{
		if( mNativeData.mCGMainDisplayID == kCGNullDirectDisplay )
		{
			throw 0;
		}

		const auto screenBounds = Platform::OSXQueryDisplayRect( mNativeData.mCGMainDisplayID );

		pOutOffset = screenBounds.offset;
	}

	void OSXDisplayManager::_NativeQueryDefaultDisplaySize( DisplaySize & pOutSize ) const
	{
		if( mNativeData.mCGMainDisplayID == kCGNullDirectDisplay )
		{
			throw 0;
		}

		pOutSize = Platform::OSXQueryDisplaySize( mNativeData.mCGMainDisplayID );
	}

	void OSXDisplayManager::_NativeQueryMinWindowSize( DisplaySize & pOutSize ) const
	{
		pOutSize.x = 1;
		pOutSize.y = 1;
	}


	OSXDisplayDriver::OSXDisplayDriver( OSXDisplayManagerHandle pDisplayManager )
	: OSXNativeObject( std::move( pDisplayManager ), EDisplayDriverType::Generic )
	{}

	OSXDisplayDriver::~OSXDisplayDriver() noexcept = default;

	void OSXDisplayDriver::_NativeEnumDisplayDevices()
	{}

	void OSXDisplayDriver::_NativeEnumVideoModes( DisplayOutput & pOutput, EColorFormat pColorFormat )
	{}

	EColorFormat OSXDisplayDriver::_NativeQueryDefaultSystemColorFormat() const
	{
		return EColorFormat::B8G8R8A8;
	}


	namespace Platform
	{

		ScreenRect OSXQueryDisplayRect( CGDirectDisplayID pCGDisplayID )
		{
			ic3DebugAssert( pCGDisplayID != kCGNullDirectDisplay );

			const auto cgDisplayRect = ::CGDisplayBounds( pCGDisplayID );

			ScreenRect displayRect{};
			displayRect.offset.x = static_cast<int32>( cgDisplayRect.origin.x );
			displayRect.offset.y = static_cast<int32>( cgDisplayRect.origin.y );
			displayRect.size.x = static_cast<int32>( cgDisplayRect.size.width );
			displayRect.size.y = static_cast<int32>( cgDisplayRect.size.height );

			return displayRect;
		}

		DisplaySize OSXQueryDisplaySize( CGDirectDisplayID pCGDisplayID )
		{
			ic3DebugAssert( pCGDisplayID != kCGNullDirectDisplay );

			const auto cgDisplayRect = ::CGDisplayBounds( pCGDisplayID );

			DisplaySize displaySize{};
			displaySize.x = static_cast<int32>( cgDisplayRect.size.width );
			displaySize.y = static_cast<int32>( cgDisplayRect.size.height );

			return displaySize;
		}

	}

} // namespace Ic3::System
#endif // PCL_TARGET_SYSAPI_OSX
