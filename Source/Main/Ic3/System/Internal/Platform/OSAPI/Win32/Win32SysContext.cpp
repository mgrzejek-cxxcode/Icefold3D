
#include "Win32SysContext.h"
#include "Win32AssetSystem.h"
#include "Win32DisplaySystem.h"
#include "Win32EventCore.h"
#include "Win32FileManager.h"
#include "Win32OpenGLDriver.h"
#include "Win32WindowSystem.h"
#include <Ic3/System/SysContextNative.h>
#include <Ic3/System/AssetSystemNative.h>
#include <process.h>
#include <tlhelp32.h>

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_WIN32 )
namespace Ic3::System
{

    namespace Platform
    {

        SysContextHandle CreateSysContext( const SysContextCreateInfo & pCreateInfo )
        {
            return CreateDynamicObject<Win32SysContext>();
        }

    }


	Win32SysContext::Win32SysContext()
	{
		_InitializeWin32ContextState();
	}

	Win32SysContext::~Win32SysContext() noexcept
	{
		_ReleaseWin32ContextState();
	}
	
	AssetLoaderHandle Win32SysContext::CreateAssetLoader( const AssetLoaderCreateInfo & pCreateInfo )
	{
		return Platform::CreateFileAssetLoader( GetHandle<Win32SysContext>(), *pCreateInfo.nativeParams );
	}

	DisplayManagerHandle Win32SysContext::CreateDisplayManager()
	{
		return CreateSysObject<Win32DisplayManager>( GetHandle<Win32SysContext>() );
	}

	EventControllerHandle Win32SysContext::CreateEventController()
	{
		return CreateSysObject<Win32EventController>( GetHandle<Win32SysContext>() );
	}

	FileManagerHandle Win32SysContext::CreateFileManager()
	{
		return CreateSysObject<Win32FileManager>( GetHandle<Win32SysContext>() );
	}

	OpenGLSystemDriverHandle Win32SysContext::CreateOpenGLSystemDriver( DisplayManagerHandle pDisplayManager )
	{
		if( !pDisplayManager )
		{
			pDisplayManager = CreateDisplayManager();
		}

		return CreateSysObject<Win32OpenGLSystemDriver>( pDisplayManager->GetHandle<Win32DisplayManager>() );
	}

	WindowManagerHandle Win32SysContext::CreateWindowManager( DisplayManagerHandle pDisplayManager )
	{
		if( !pDisplayManager )
		{
			pDisplayManager = CreateDisplayManager();
		}

		return CreateSysObject<Win32WindowManager>( pDisplayManager->GetHandle<Win32DisplayManager>() );
	}

	std::string Win32SysContext::QueryCurrentProcessWorkingDirectory() const
	{
		char workingDirStrBuffer[2048];
		_getcwd( workingDirStrBuffer, 2048 );
		return std::string( workingDirStrBuffer );
	}

	std::string Win32SysContext::QueryCurrentProcessExecutableFilePath() const
	{
		std::string executableFilePath;

		DWORD validProcessID = 0;
		DWORD currentProcessID = getpid();
		HANDLE moduleSnapshotHandle = ::CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, currentProcessID );

		MODULEENTRY32 moduleEntryInfo;
		moduleEntryInfo.dwSize = sizeof( MODULEENTRY32 );

		if( ::Module32First( moduleSnapshotHandle, &moduleEntryInfo ) )
		{
			do
			{
				if( moduleEntryInfo.th32ProcessID == currentProcessID )
				{
					validProcessID = currentProcessID;
					break;
				}
			}
			while( ::Module32Next( moduleSnapshotHandle, &moduleEntryInfo ) );
		}

		::CloseHandle( moduleSnapshotHandle );

		if( validProcessID != 0 )
		{
			executableFilePath = moduleEntryInfo.szExePath;
		}

		return executableFilePath;
	}

	void Win32SysContext::_InitializeWin32ContextState()
	{
		mNativeData.appExecModuleHandle = ::GetModuleHandleA( nullptr );
	}

	void Win32SysContext::_ReleaseWin32ContextState()
	{
		mNativeData.appExecModuleHandle = nullptr;
	}

} // namespace Ic3::System
#endif // PCL_TARGET_SYSAPI_WIN32
