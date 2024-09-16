
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_SYS_CONTEXT_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_SYS_CONTEXT_H__

#include <Ic3/System/SysContext.h>
#include "Win32Common.h"

namespace Ic3::System
{

	namespace Platform
	{

		struct Win32SysContextNativeData
		{
			HINSTANCE mAppExecModuleHandle;
		};

		struct SysContextCreateInfoNativeParams
		{
			HINSTANCE mAppExecModuleHandle;
		};

	}

	class Win32SysContext : public NativeObject<SysContext, Platform::Win32SysContextNativeData>
	{
	public:
		Win32SysContext();
		virtual ~Win32SysContext() noexcept;

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

		/// @copybrief SysContext::queryCurrentProcessWorkingDirectory
		virtual std::string queryCurrentProcessWorkingDirectory() const override final;

		/// @copybrief SysContext::queryCurrentProcessExecutableFilePath
		virtual std::string queryCurrentProcessExecutableFilePath() const override final;

	private:
		void _initializeWin32ContextState();
		void _releaseWin32ContextState();
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_SYS_CONTEXT_H__
