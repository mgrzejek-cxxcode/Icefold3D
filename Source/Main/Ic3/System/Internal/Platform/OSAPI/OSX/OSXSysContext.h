
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_OSX_SYS_CONTEXT_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_OSX_SYS_CONTEXT_H__

#include "OSXCommon.h"
#include <Ic3/System/SysContext.h>
#include <cppx/bitmaskAtomic.h>

namespace Ic3::System
{

	namespace Platform
	{

		struct OSXSysContextNativeData
		{
			id mNSApplicationProxy = nil;

			id mNSApplicationDelegate = nil;

			OSXSharedData mOSXSharedData;
		};

		struct SysContextCreateInfoNativeParams
		{
		};

	}

	class OSXSysContext : public NativeObject<SysContext, Platform::OSXSysContextNativeData>
	{
	public:
		OSXSysContext();
		virtual ~OSXSysContext() noexcept;

		/// @copybrief SysContext::CreateAssetLoader
		virtual AssetLoaderHandle CreateAssetLoader( const AssetLoaderCreateInfo & pCreateInfo ) override final;

		/// @copybrief SysContext::CreateDisplayManager
		virtual DisplayManagerHandle CreateDisplayManager() override final;

		/// @copybrief SysContext::CreateEventController
		virtual EventControllerHandle CreateEventController() override final;

		/// @copybrief SysContext::CreateFileManager
		virtual FileManagerHandle CreateFileManager() override final;

		virtual MetalSystemDriverHandle CreateMetalSystemDriver(
				DisplayManagerHandle pDisplayManager,
				const MetalSystemDriverCreateInfo & pCreateInfo ) override final;

		/// @copybrief SysContext::CreateOpenGLSystemDriver
		virtual OpenGLSystemDriverHandle CreateOpenGLSystemDriver( DisplayManagerHandle pDisplayManager ) override final;

		/// @copybrief SysContext::CreateWindowManager
		virtual WindowManagerHandle CreateWindowManager( DisplayManagerHandle pDisplayManager ) override final;

		/// @copybrief SysContext::QueryCurrentProcessWorkingDirectory
		virtual std::string QueryCurrentProcessWorkingDirectory() const override final;

		/// @copybrief SysContext::QueryCurrentProcessExecutableFilePath
		virtual std::string QueryCurrentProcessExecutableFilePath() const override final;

		bool IsNSAppProxyRegistered() const;

	private:
		void _InitializeOSXContextState();
		void _ReleaseOSXContextState();

		void _registerNSAppProxy();

	private:
		enum : uint32
		{
			E_STATE_NS_APP_PROXY_REGISTERED = 0x1000,
		};

		cppx::atomic_bitmask<uint32> _stateMask;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_OSX_SYS_CONTEXT_H__
