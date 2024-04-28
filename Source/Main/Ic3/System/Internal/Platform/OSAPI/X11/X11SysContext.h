
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
			X11SessionData xSessionData;
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
		void _initializeX11ContextState();
		void _releaseX11ContextState();
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_X11_CORE_SESSION_CONTEXT_H__
