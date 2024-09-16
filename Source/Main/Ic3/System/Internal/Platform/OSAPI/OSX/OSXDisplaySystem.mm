
#include "OSXDisplaySystem.h"
#include <CoreGraphics/CGDisplayConfiguration.h>

#if( IC3_PCL_TARGET_SYSAPI == IC3_PCL_TARGET_SYSAPI_OSX )
namespace Ic3::System
{

	OSXDisplayManager::OSXDisplayManager( SysContextHandle pSysContext )
	: OSXNativeObject( std::move( pSysContext ) )
	{
		_initializeOSXDisplayManagerState();
	}

	OSXDisplayManager::~OSXDisplayManager() noexcept
	{
		_releaseOSXDisplayManagerState();
	}

	void OSXDisplayManager::_initializeOSXDisplayManagerState()
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
	
	void OSXDisplayManager::_releaseOSXDisplayManagerState()
	{}

	DisplayDriverHandle OSXDisplayManager::_nativeCreateDisplayDriver()
	{
		return createSysObject<OSXDisplayDriver>( getHandle<OSXDisplayManager>() );
	}

	void OSXDisplayManager::_nativeQueryDefaultDisplayOffset( DisplayOffset & pOutOffset ) const
	{
		if( mNativeData.mCGMainDisplayID == kCGNullDirectDisplay )
		{
			throw 0;
		}

		const auto screenBounds = Platform::osxQueryDisplayRect( mNativeData.mCGMainDisplayID );

		pOutOffset = screenBounds.mOffset;
	}

	void OSXDisplayManager::_nativeQueryDefaultDisplaySize( DisplaySize & pOutSize ) const
	{
		if( mNativeData.mCGMainDisplayID == kCGNullDirectDisplay )
		{
			throw 0;
		}

		pOutSize = Platform::osxQueryDisplaySize( mNativeData.mCGMainDisplayID );
	}

	void OSXDisplayManager::_nativeQueryMinWindowSize( DisplaySize & pOutSize ) const
	{
		pOutSize.x = 1;
		pOutSize.y = 1;
	}


	OSXDisplayDriver::OSXDisplayDriver( OSXDisplayManagerHandle pDisplayManager )
	: OSXNativeObject( std::move( pDisplayManager ), EDisplayDriverType::Generic )
	{}

	OSXDisplayDriver::~OSXDisplayDriver() noexcept = default;

	void OSXDisplayDriver::_nativeEnumDisplayDevices()
	{}

	void OSXDisplayDriver::_nativeEnumVideoModes( DisplayOutput & pOutput, EColorFormat pColorFormat )
	{}

	EColorFormat OSXDisplayDriver::_nativeQueryDefaultSystemColorFormat() const
	{
		return EColorFormat::B8G8R8A8;
	}


	namespace Platform
	{

		ScreenRect osxQueryDisplayRect( CGDirectDisplayID pCGDisplayID )
		{
			ic3DebugAssert( pCGDisplayID != kCGNullDirectDisplay );

			const auto cgDisplayRect = ::CGDisplayBounds( pCGDisplayID );

			ScreenRect displayRect{};
			displayRect.mOffset.x = static_cast<int32>( cgDisplayRect.origin.x );
			displayRect.mOffset.y = static_cast<int32>( cgDisplayRect.origin.y );
			displayRect.mSize.x = static_cast<int32>( cgDisplayRect.size.width );
			displayRect.mSize.y = static_cast<int32>( cgDisplayRect.size.height );

			return displayRect;
		}

		DisplaySize osxQueryDisplaySize( CGDirectDisplayID pCGDisplayID )
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
#endif // IC3_PCL_TARGET_SYSAPI_OSX
