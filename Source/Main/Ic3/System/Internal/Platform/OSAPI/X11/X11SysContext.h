
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_X11_CORE_SESSION_CONTEXT_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_X11_CORE_SESSION_CONTEXT_H__

#include "X11Common.h"
#include <Ic3/System/SysContext.h>

namespace Ic3::System
{

	namespace Platform
	{

		struct X11SysContextNativeData
		{
			X11SessionData mXSessionData;
		};

		struct SysContextCreateInfoNativeParams
		{
		};

	}

	class X11SysContext : public NativeObject<SysContext, Platform::X11SysContextNativeData>
	{
	public:
		X11SysContext();
		virtual ~X11SysContext() noexcept;

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
		void _InitializeX11ContextState();
		void _ReleaseX11ContextState();
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_X11_CORE_SESSION_CONTEXT_H__
