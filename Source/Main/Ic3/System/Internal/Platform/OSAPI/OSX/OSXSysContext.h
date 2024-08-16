
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_OSX_SYS_CONTEXT_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_OSX_SYS_CONTEXT_H__

#include "OSXCommon.h"
#include <Ic3/System/SysContext.h>
#include <Ic3/Cppx/BitmaskAtomic.h>

namespace Ic3::System
{

	namespace Platform
	{

		struct OSXSysContextNativeData
		{
			id nsApplicationProxy = nil;

			id nsApplicationDelegate = nil;

			OSXSharedData osxSharedData;
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

		/// @copybrief SysContext::createAssetLoader
		virtual AssetLoaderHandle createAssetLoader( const AssetLoaderCreateInfo & pCreateInfo ) override final;

		/// @copybrief SysContext::createDisplayManager
		virtual DisplayManagerHandle createDisplayManager() override final;

		/// @copybrief SysContext::createEventController
		virtual EventControllerHandle createEventController() override final;

		/// @copybrief SysContext::createFileManager
		virtual FileManagerHandle createFileManager() override final;

		virtual MetalSystemDriverHandle createMetalSystemDriver( DisplayManagerHandle pDisplayManager,
		                                                         const MetalSystemDriverCreateInfo & pCreateInfo ) override final;

		/// @copybrief SysContext::createOpenGLSystemDriver
		virtual OpenGLSystemDriverHandle createOpenGLSystemDriver( DisplayManagerHandle pDisplayManager ) override final;

		/// @copybrief SysContext::createWindowManager
		virtual WindowManagerHandle createWindowManager( DisplayManagerHandle pDisplayManager ) override final;

		/// @copybrief SysContext::queryCurrentProcessWorkingDirectory
		virtual std::string queryCurrentProcessWorkingDirectory() const override final;

		/// @copybrief SysContext::queryCurrentProcessExecutableFilePath
		virtual std::string queryCurrentProcessExecutableFilePath() const override final;

		bool isNSAppProxyRegistered() const;

	private:
		void _initializeOSXContextState();
		void _releaseOSXContextState();

		void _registerNSAppProxy();

	private:
		enum : uint32
		{
			E_STATE_NS_APP_PROXY_REGISTERED = 0x1000,
		};

		Cppx::AtomicBitmask<uint32> _stateMask;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_OSX_SYS_CONTEXT_H__
