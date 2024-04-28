
#include "OSXSysContext.h"
#include "OSXAssetSystem.h"
#include "OSXDisplaySystem.h"
#include "OSXFileManager.h"
#include "OSXMetalDriver.h"
#include "OSXOpenGLDriver.h"
#include "OSXWindowSystem.h"
#include <Ic3/System/SysContextNative.h>
#include <Ic3/System/AssetSystemNative.h>
#include "NSIApplicationProxy.h"

#include <unistd.h>

#if( IC3_PCL_TARGET_SYSAPI == IC3_PCL_TARGET_SYSAPI_OSX )
namespace Ic3::System
{

    namespace Platform
    {

        SysContextHandle createSysContext( const SysContextCreateInfo & pCreateInfo )
        {
            return createDynamicInterfaceObject<OSXSysContext>();
        }

    }


    OSXSysContext::OSXSysContext()
    {
        _initializeOSXContextState();
    }

    OSXSysContext::~OSXSysContext() noexcept
    {
        _releaseOSXContextState();
    }
    
    AssetLoaderHandle OSXSysContext::createAssetLoader( const AssetLoaderCreateInfo & pCreateInfo )
    {
		ic3DebugAssert( pCreateInfo.nativeParams );
        return Platform::createFileAssetLoader( getHandle<SysContext>(), *pCreateInfo.nativeParams );
    }

    DisplayManagerHandle OSXSysContext::createDisplayManager()
    {
        return createSysObject<OSXDisplayManager>( getHandle<OSXSysContext>() );
    }

    EventControllerHandle OSXSysContext::createEventController()
    {
        return createSysObject<OSXEventController>( getHandle<OSXSysContext>() );
    }

    FileManagerHandle OSXSysContext::createFileManager()
    {
        return createSysObject<PosixFileManager>( getHandle<OSXSysContext>() );
    }

	MetalSystemDriverHandle OSXSysContext::createMetalSystemDriver( DisplayManagerHandle pDisplayManager,
	                                                                const MetalSystemDriverCreateInfo & pCreateInfo )
	{
		if( !pDisplayManager )
		{
			pDisplayManager = createDisplayManager();
		}

		return createSysObject<OSXMetalSystemDriver>( pDisplayManager->getHandle<OSXDisplayManager>() );
	}

    OpenGLSystemDriverHandle OSXSysContext::createOpenGLSystemDriver( DisplayManagerHandle pDisplayManager )
    {
        if( !pDisplayManager )
        {
            pDisplayManager = createDisplayManager();
        }

        return createSysObject<OSXOpenGLSystemDriver>( pDisplayManager->getHandle<OSXDisplayManager>() );
    }

    WindowManagerHandle OSXSysContext::createWindowManager( DisplayManagerHandle pDisplayManager )
    {
        if( !pDisplayManager )
        {
            pDisplayManager = createDisplayManager();
        }

        return createSysObject<OSXWindowManager>( pDisplayManager->getHandle<OSXDisplayManager>() );
    }

	std::string OSXSysContext::queryCurrentProcessWorkingDirectory() const
	{
		char workingDirStrBuffer[2048];
		getcwd( workingDirStrBuffer, 2048 );

		return std::string( workingDirStrBuffer );
	}

    std::string OSXSysContext::queryCurrentProcessExecutableFilePath() const
    {
        std::string executableFilePath;

		auto * executablePath = [[NSBundle mainBundle] bundlePath];
	    executableFilePath.assign( [executablePath cStringUsingEncoding: NSUTF8StringEncoding],
	                               [executablePath lengthOfBytesUsingEncoding: NSUTF8StringEncoding] );

        return executableFilePath;
    }

	bool OSXSysContext::isNSAppProxyRegistered() const
	{
		return _stateMask.isSet( E_STATE_NS_APP_PROXY_REGISTERED, std::memory_order_acquire );
	}

    void OSXSysContext::_initializeOSXContextState()
    {
		_registerNSAppProxy();
    }

    void OSXSysContext::_releaseOSXContextState()
    {
    }

	void OSXSysContext::_registerNSAppProxy()
	{
	@autoreleasepool
	{
		auto & osxSharedData = Platform::osxGetOSXSharedData( *this );

		if( !NSApp )
		{
			[NSOSXApplicationProxy sharedApplication];
			ic3DebugAssert( NSApp );

			if( ![NSApp delegate] )
			{
				auto * nsAppDelegate = [[NSOSXApplicationDelegate alloc] initWithSysContext:this];
				[NSApp setDelegate:nsAppDelegate];

				mNativeData.nsApplicationDelegate = nsAppDelegate;
				mNativeData.osxSharedData.stateFlags.isSet( Platform::E_OSX_COMMON_STATE_APP_DELEGATE_INITIALIZED_BIT );
			}

			if( !mNativeData.osxSharedData.stateFlags.isSet( Platform::E_OSX_COMMON_STATE_APP_FINISHED_LAUNCHING_BIT ) )
			{
				[NSApp finishLaunching];

				// Note: E_OSX_COMMON_STATE_APP_FINISHED_LAUNCHING_BIT is usually (better: almost certainly) not set at
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
#endif // IC3_PCL_TARGET_SYSAPI_OSX
