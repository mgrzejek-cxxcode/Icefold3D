
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_CORE_SESSION_CONTEXT_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_CORE_SESSION_CONTEXT_H__

#include "AndroidCommon.h"
#include <Ic3/System/SysContext.h>
#include <Ic3/System/Internal/Platform/Shared/Java/JavaVM.h>
#include <Ic3/System/Internal/Platform/Shared/Java/JavaNativeInterface.h>
#include <thread>

namespace Ic3::System
{

	namespace Platform
	{

		struct AndroidSysContextNativeData
		{
			ASessionData aSessionData;
		};

		struct SysContextCreateInfoNativeParams
		{
			AndroidAppState * aCommonAppState = nullptr;
		};

	}

	class AndroidSysContext : public SysContext, public NativeObject<Platform::AndroidSysContextNativeData>
	{
	public:
		std::unique_ptr<JavaVMInstance> const mJVMInstance;
		JavaNativeInterfacePtr const mSysThreadJNIObject;

	public:
		AndroidSysContext( AndroidAppState * pAppState );
		virtual ~AndroidSysContext() noexcept;

		/// @copybrief SysContext::CreateAssetLoader
		virtual AssetLoaderHandle CreateAssetLoader( const AssetLoaderCreateInfo & pCreateInfo ) override final;

		/// @copybrief SysContext::CreateDisplayManager
		virtual DisplayManagerHandle CreateDisplayManager() override final;

		/// @copybrief SysContext::CreateEventController
		virtual EventControllerHandle CreateEventController() override final;

		/// @copybrief SysContext::CreateFileManager
		virtual FileManagerHandle CreateFileManager() override final;

		/// @copybrief SysContext::CreateOpenGLSystemDriver
		virtual OpenGLSystemDriverHandle CreateOpenGLSystemDriver( DisplayManagerHandle pDisplayManager ) override final;

		/// @copybrief SysContext::CreateWindowManager
		virtual WindowManagerHandle CreateWindowManager( DisplayManagerHandle pDisplayManager ) override final;

		/// @copybrief SysContext::QueryCurrentProcessExecutableFilePath
		virtual std::string QueryCurrentProcessExecutableFilePath() const override final;

		void UpdateANativeWindowReference( ANativeWindow * pANativeWindow );

		Platform::ASessionData & GetASessionData();

		const Platform::ASessionData & GetASessionData() const;

	private:
		void _InitializeAndroidContextState( AndroidAppState * pAppState );
		void _ReleaseAndroidContextState();
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_CORE_SESSION_CONTEXT_H__
