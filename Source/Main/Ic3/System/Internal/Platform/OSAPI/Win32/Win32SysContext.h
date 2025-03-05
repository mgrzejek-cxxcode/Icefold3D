
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
			HINSTANCE appExecModuleHandle;
		};

		struct SysContextCreateInfoNativeParams
		{
			HINSTANCE appExecModuleHandle;
		};

	}

	class IC3_SYSTEM_CLASS Win32SysContext : public NativeObject<SysContext, Platform::Win32SysContextNativeData>
	{
	public:
		Win32SysContext();
		virtual ~Win32SysContext() noexcept;

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

		/// @copybrief SysContext::QueryCurrentProcessWorkingDirectory
		virtual std::string QueryCurrentProcessWorkingDirectory() const override final;

		/// @copybrief SysContext::QueryCurrentProcessExecutableFilePath
		virtual std::string QueryCurrentProcessExecutableFilePath() const override final;

	private:
		void _InitializeWin32ContextState();
		void _ReleaseWin32ContextState();
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_SYS_CONTEXT_H__
