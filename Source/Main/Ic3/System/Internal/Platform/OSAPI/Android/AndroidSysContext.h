
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

		/// @copybrief SysContext::createAssetLoader
		virtual AssetLoaderHandle createAssetLoader( const AssetLoaderCreateInfo & pCreateInfo ) override final;

		/// @copybrief SysContext::createDisplayManager
		virtual DisplayManagerHandle createDisplayManager() override final;

		/// @copybrief SysContext::createEventController
		virtual EventControllerHandle createEventController() override final;

		/// @copybrief SysContext::createFileManager
		virtual FileManagerHandle createFileManager() override final;

		/// @copybrief SysContext::createOpenGLSystemDriver
		virtual OpenGLSystemDriverHandle createOpenGLSystemDriver( DisplayManagerHandle pDisplayManager ) override final;

		/// @copybrief SysContext::createWindowManager
		virtual WindowManagerHandle createWindowManager( DisplayManagerHandle pDisplayManager ) override final;

		/// @copybrief SysContext::queryCurrentProcessExecutableFilePath
		virtual std::string queryCurrentProcessExecutableFilePath() const override final;

		void updateANativeWindowReference( ANativeWindow * pANativeWindow );

		Platform::ASessionData & getASessionData();

		const Platform::ASessionData & getASessionData() const;

	private:
		void _initializeAndroidContextState( AndroidAppState * pAppState );
		void _releaseAndroidContextState();
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_CORE_SESSION_CONTEXT_H__
