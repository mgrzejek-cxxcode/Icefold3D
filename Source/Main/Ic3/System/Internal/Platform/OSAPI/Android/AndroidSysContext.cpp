
#include "AndroidSysContext.h"
#include "AndroidAssetSystem.h"
#include "AndroidDisplaySystem.h"
#include "AndroidEventCore.h"
#include "AndroidFileManager.h"
#include "AndroidOpenGLDriver.h"
#include "AndroidWindowSystem.h"
#include <Ic3/System/SysContextNative.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_ANDROID )
namespace Ic3::System
{

	SysContextHandle CreateSysContext( const SysContextCreateInfo & pCreateInfo )
	{
		if( !pCreateInfo.nativeParams.aCommonAppState )
		{
			return nullptr;
		}
		return CreateDynamicObject<AndroidSysContext>( pCreateInfo.nativeParams.aCommonAppState );
	}


	AndroidSysContext::AndroidSysContext( AndroidAppState * pAppState )
	: mJVMInstance( std::make_unique<JavaVMInstance>( pAppState->activity->vm ) )
	, mSysThreadJNIObject( mJVMInstance->AcquireJNIForCurrentThread() )
	{
		_InitializeAndroidContextState( pAppState );
	}

	AndroidSysContext::~AndroidSysContext() noexcept
	{
		_ReleaseAndroidContextState();
	}

	AssetLoaderHandle AndroidSysContext::CreateAssetLoader( const AssetLoaderCreateInfo & pCreateInfo )
	{
		return CreateSysObject<AndroidAssetLoader>( GetHandle<AndroidSysContext>() );
	}

	DisplayManagerHandle AndroidSysContext::CreateDisplayManager()
	{
		return CreateSysObject<AndroidDisplayManager>( GetHandle<AndroidSysContext>() );
	}

	EventControllerHandle AndroidSysContext::CreateEventController()
	{
		return CreateSysObject<AndroidEventController>( GetHandle<AndroidSysContext>() );
	}

	FileManagerHandle AndroidSysContext::CreateFileManager()
	{
		return CreateSysObject<PosixFileManager>( GetHandle<AndroidSysContext>() );
	}

	OpenGLSystemDriverHandle AndroidSysContext::CreateOpenGLSystemDriver( DisplayManagerHandle pDisplayManager )
	{
		if( !pDisplayManager )
		{
			pDisplayManager = CreateDisplayManager();
		}

		auto androidDisplayManager = pDisplayManager->QueryHandle<AndroidDisplayManager>();
		return CreateSysObject<AndroidOpenGLSystemDriver>( androidDisplayManager );
	}

	WindowManagerHandle AndroidSysContext::CreateWindowManager( DisplayManagerHandle pDisplayManager )
	{
		if( !pDisplayManager )
		{
			pDisplayManager = CreateDisplayManager();
		}

		return CreateSysObject<AndroidWindowManager>( pDisplayManager );
	}

	std::string AndroidSysContext::QueryCurrentProcessExecutableFilePath() const
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

	void AndroidSysContext::_InitializeAndroidContextState( AndroidAppState * pAppState )
	{
		mNativeData.aSessionData.aCommonAppState = pAppState;
		mNativeData.aSessionData.aCommonAppState->Ic3SetUserData(
			Platform::eAndroidAppStateUserDataIndexSysContext, this );
	}

	void AndroidSysContext::_ReleaseAndroidContextState()
	{
		mNativeData.aSessionData.aCommonAppState->Ic3SetUserData(
			Platform::eAndroidAppStateUserDataIndexSysContext, nullptr );
		mNativeData.aSessionData.aCommonAppState = nullptr;
	}

	void AndroidSysContext::UpdateANativeWindowReference( ANativeWindow * pANativeWindow )
	{
		if( pANativeWindow != mNativeData.aSessionData.aNativeWindow )
		{
			ANativeWindow_acquire( pANativeWindow );
			if( mNativeData.aSessionData.aNativeWindow )
			{
				ANativeWindow_Release( mNativeData.aSessionData.aNativeWindow );
			}
			mNativeData.aSessionData.aNativeWindow = pANativeWindow;
		}
	}

	Platform::ASessionData & AndroidSysContext::GetASessionData()
	{
		return mNativeData.aSessionData;
	}

	const Platform::ASessionData & AndroidSysContext::GetASessionData() const
	{
		return mNativeData.aSessionData;
	}

} // namespace Ic3::System
#endif // PCL_TARGET_SYSAPI_ANDROID
