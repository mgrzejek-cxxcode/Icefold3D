
#include "OSXSysContext.h"
#include "OSXAssetSystem.h"
#include "OSXDisplaySystem.h"
#include "OSXFileSystem.h"
#include "OSXMetalDriver.h"
#include "OSXOpenGLDriver.h"
#include "OSXWindowSystem.h"
#include <Ic3/System/SysContextNative.h>
#include <Ic3/System/IO/AssetSystemNative.h>
#include "NSIApplicationProxy.h"

#include <unistd.h>

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_OSX )
namespace Ic3::System
{

    namespace Platform
    {

        SysContextHandle CreateSysContext( const SysContextCreateInfo & pCreateInfo )
        {
            return CreateDynamicObject<OSXSysContext>();
        }

    }


    OSXSysContext::OSXSysContext()
    {
        _InitializeOSXContextState();
    }

    OSXSysContext::~OSXSysContext() noexcept
    {
        _ReleaseOSXContextState();
    }
    
    AssetLoaderHandle OSXSysContext::CreateAssetLoader( const AssetLoaderCreateInfo & pCreateInfo )
    {
		Ic3DebugAssert( pCreateInfo.nativeParams );
        return Platform::CreateFileAssetLoader( GetHandle<SysContext>(), *pCreateInfo.nativeParams );
    }

    DisplayManagerHandle OSXSysContext::CreateDisplayManager()
    {
        return CreateSysObject<OSXDisplayManager>( GetHandle<OSXSysContext>() );
    }

    EventControllerHandle OSXSysContext::CreateEventController()
    {
        return CreateSysObject<OSXEventController>( GetHandle<OSXSysContext>() );
    }

    FileManagerHandle OSXSysContext::CreateFileManager()
    {
        return CreateSysObject<PosixFileManager>( GetHandle<OSXSysContext>() );
    }

	MetalSystemDriverHandle OSXSysContext::CreateMetalSystemDriver(
			DisplayManagerHandle pDisplayManager,
			const MetalSystemDriverCreateInfo & pCreateInfo )
	{
		if( !pDisplayManager )
		{
			pDisplayManager = CreateDisplayManager();
		}

		return CreateSysObject<OSXMetalSystemDriver>( pDisplayManager->GetHandle<OSXDisplayManager>() );
	}

    OpenGLSystemDriverHandle OSXSysContext::CreateOpenGLSystemDriver( DisplayManagerHandle pDisplayManager )
    {
        if( !pDisplayManager )
        {
            pDisplayManager = CreateDisplayManager();
        }

        return CreateSysObject<OSXOpenGLSystemDriver>( pDisplayManager->GetHandle<OSXDisplayManager>() );
    }

    WindowManagerHandle OSXSysContext::CreateWindowManager( DisplayManagerHandle pDisplayManager )
    {
        if( !pDisplayManager )
        {
            pDisplayManager = CreateDisplayManager();
        }

        return CreateSysObject<OSXWindowManager>( pDisplayManager->GetHandle<OSXDisplayManager>() );
    }

	std::string OSXSysContext::QueryCurrentProcessWorkingDirectory() const
	{
		char workingDirStrBuffer[2048];
		getcwd( workingDirStrBuffer, 2048 );

		return std::string( workingDirStrBuffer );
	}

    std::string OSXSysContext::QueryCurrentProcessExecutableFilePath() const
    {
        std::string executableFilePath;

		auto * executablePath = [[NSBundle mainBundle] bundlePath];
	    executableFilePath.assign( [executablePath cStringUsingEncoding: NSUTF8StringEncoding],
	                               [executablePath lengthOfBytesUsingEncoding: NSUTF8StringEncoding] );

        return executableFilePath;
    }

	bool OSXSysContext::IsNSAppProxyRegistered() const
	{
		return _stateMask.is_set( E_STATE_NS_APP_PROXY_REGISTERED, std::memory_order_acquire );
	}

    void OSXSysContext::_InitializeOSXContextState()
    {
		_registerNSAppProxy();
    }

    void OSXSysContext::_ReleaseOSXContextState()
    {
    }

	void OSXSysContext::_registerNSAppProxy()
	{
	@autoreleasepool
	{
		auto & osxSharedData = Platform::OSXGetOSXSharedData( *this );

		if( !NSApp )
		{
			[NSOSXApplicationProxy sharedApplication];
			Ic3DebugAssert( NSApp );

			if( ![NSApp delegate] )
			{
				auto * nsAppDelegate = [[NSOSXApplicationDelegate alloc] initWithSysContext:this];
				[NSApp setDelegate:nsAppDelegate];

				mNativeData.mNSApplicationDelegate = nsAppDelegate;
				mNativeData.mOSXSharedData.stateFlags.is_set( Platform::eOSXCommonStateAppDelegateInitializedBit );
			}

			if( !mNativeData.mOSXSharedData.stateFlags.is_set( Platform::eOSXCommonStateAppFinishedLaunchingBit ) )
			{
				[NSApp finishLaunching];

				// Note: eOSXCommonStateAppFinishedLaunchingBit is usually (better: almost certainly) not set at
				// this point. The reason is that [finishLaunching] posts a message which is caught by the AppDelegate
				// and processed during event loop. That means, that applicationDidFinishLaunching() may still not be
				// called when SysContext finishes its initialization process.

				if( [NSApp delegate] )
				{
					[NSOSXApplicationProxy registerUserDefaults];
				}
			}
		}
	}
	}

} // namespace Ic3::System
#endif // PCL_TARGET_SYSAPI_OSX
