
#include "X11SysContext.h"
#include "X11AssetSystem.h"
#include "X11DisplaySystem.h"
#include "X11EventCore.h"
#include "X11FileManager.h"
#include "X11OpenGLDriver.h"
#include "X11WindowSystem.h"
#include <Ic3/System/SysContextNative.h>
#include <Ic3/System/AssetSystemNative.h>

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_X11 )
namespace Ic3::System
{

	namespace Platform
	{

		SysContextHandle CreateSysContext( const SysContextCreateInfo & pCreateInfo )
		{
			return CreateDynamicObject<X11SysContext>();
		}

	}


	X11SysContext::X11SysContext()
	{
		_InitializeX11ContextState();
	}

	X11SysContext::~X11SysContext() noexcept
	{
		_ReleaseX11ContextState();
	}

	AssetLoaderHandle X11SysContext::CreateAssetLoader( const AssetLoaderCreateInfo & pCreateInfo )
	{
		return Platform::CreateFileAssetLoader( GetHandle<X11SysContext>(), *pCreateInfo.nativeParams );
	}

	DisplayManagerHandle X11SysContext::CreateDisplayManager()
	{
		return CreateSysObject<X11DisplayManager>( GetHandle<X11SysContext>() );
	}

	EventControllerHandle X11SysContext::CreateEventController()
	{
		return CreateSysObject<X11EventController>( GetHandle<X11SysContext>() );
	}

	FileManagerHandle X11SysContext::CreateFileManager()
	{
		return CreateSysObject<PosixFileManager>( GetHandle<X11SysContext>() );
	}

	OpenGLSystemDriverHandle X11SysContext::CreateOpenGLSystemDriver( DisplayManagerHandle pDisplayManager )
	{
		if( !pDisplayManager )
		{
			pDisplayManager = CreateDisplayManager();
		}

		return CreateSysObject<X11OpenGLSystemDriver>( pDisplayManager->GetHandle<X11DisplayManager>() );
	}

	WindowManagerHandle X11SysContext::CreateWindowManager( DisplayManagerHandle pDisplayManager )
	{
		if( !pDisplayManager )
		{
			pDisplayManager = CreateDisplayManager();
		}

		return CreateSysObject<X11WindowManager>( pDisplayManager->GetHandle<X11DisplayManager>() );
	}

	void X11SysContext::_InitializeX11ContextState()
	{
		// This has completely crashed the whole X11 subsystem when ran on a VM-ed ARM-based Ubuntu 21.
		// The environment: MacBook Pro 16.2 with M1 Max, Ubuntu 21.04 (ARM version), Parallels Pro.
		// Most X11 started giving infinite freezes diagnosed as deadlocks on a system-level mutexes.
		//
		// int thrInitStatus = ::XInitThreads();
		// if( thrInitStatus == False )
		// {
		// 	Ic3Throw( eExcCodeDebugPlaceholder );
		// }

		auto * displayHandle = ::XOpenDisplay( nullptr );
		if( displayHandle == nullptr )
		{
			Ic3Throw( eExcCodeDebugPlaceholder );
		}

		mNativeData.mXSessionData.displayHandle = displayHandle;
		mNativeData.mXSessionData.screenIndex = XDefaultScreen( mNativeData.mXSessionData.displayHandle );
		mNativeData.mXSessionData.rootWindowXID = XRootWindow( mNativeData.mXSessionData.displayHandle, mNativeData.mXSessionData.screenIndex );
		mNativeData.mXSessionData.atomCache.wmProtocol = XInternAtom( mNativeData.mXSessionData.displayHandle, "WM_PROTOCOLS", True );
		mNativeData.mXSessionData.atomCache.wmProtocolDelete = XInternAtom( mNativeData.mXSessionData.displayHandle, "WM_DELETE_WINDOW", True );
		mNativeData.mXSessionData.atomCache.wmProtocolDestroy = XInternAtom( mNativeData.mXSessionData.displayHandle, "WM_DESTROY_WINDOW", True );
		mNativeData.mXSessionData.atomCache.wmState = XInternAtom( mNativeData.mXSessionData.displayHandle, "_NET_WM_STATE", True );
		mNativeData.mXSessionData.atomCache.wmStateFullscreen = XInternAtom( mNativeData.mXSessionData.displayHandle, "_NET_WM_STATE_FULLSCREEN", True );
		mNativeData.mXSessionData.sessionInfo.connectionNumber = XConnectionNumber( displayHandle );
		mNativeData.mXSessionData.sessionInfo.vendorName = XServerVendor( displayHandle );
		mNativeData.mXSessionData.sessionInfo.displayString = XDisplayString( displayHandle );
	}

	void X11SysContext::_ReleaseX11ContextState()
	{
		mNativeData.mXSessionData.displayHandle = nullptr;
		mNativeData.mXSessionData.screenIndex = -1;
		mNativeData.mXSessionData.rootWindowXID = Platform::eXIDNone;
		mNativeData.mXSessionData.atomCache.wmProtocolDelete = 0;
		mNativeData.mXSessionData.atomCache.wmProtocolDestroy = 0;
		mNativeData.mXSessionData.atomCache.wmState = 0;
		mNativeData.mXSessionData.atomCache.wmStateFullscreen = 0;
		mNativeData.mXSessionData.sessionInfo.connectionNumber = -1;
		mNativeData.mXSessionData.sessionInfo.vendorName.clear();
		mNativeData.mXSessionData.sessionInfo.displayString.clear();
	}

	std::string X11SysContext::QueryCurrentProcessWorkingDirectory() const
	{
		char workingDirStrBuffer[2048];
		getcwd( workingDirStrBuffer, 2048 );

		return std::string( workingDirStrBuffer );
	}

	std::string X11SysContext::QueryCurrentProcessExecutableFilePath() const
	{
		pid_t currentProcessID = getpid();

		std::string executableFilePath;
		std::string exeLink = std::string( "/proc/" ) + std::to_string( currentProcessID ) + std::string( "/exe" );

		if( char * bufferPtr = realpath( exeLink.c_str(), nullptr ) )
		{
			executableFilePath = bufferPtr;
			free( bufferPtr );
		}

		return executableFilePath;
	}

} // namespace Ic3::System
#endif // PCL_TARGET_SYSAPI_X11
