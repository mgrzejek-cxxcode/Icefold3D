
#ifndef __IC3_SYSTEM_CORE_SYS_CONTEXT_H__
#define __IC3_SYSTEM_CORE_SYS_CONTEXT_H__

#include "Prerequisites.h"

namespace Ic3::System
{

	namespace Platform
	{

		struct AssetLoaderCreateInfoNativeParams;
		struct SysContextCreateInfoNativeParams;

	}

	struct MetalSystemDriverCreateInfo;

	Ic3SysDeclareHandle( SysContext );
	Ic3SysDeclareHandle( AssetLoader );
	Ic3SysDeclareHandle( DisplayManager );
	Ic3SysDeclareHandle( EventController );
	Ic3SysDeclareHandle( FileManager );
	Ic3SysDeclareHandle( MetalSystemDriver );
	Ic3SysDeclareHandle( OpenGLSystemDriver );
	Ic3SysDeclareHandle( PipeFactory );
	Ic3SysDeclareHandle( WindowManager );

	struct AssetLoaderCreateInfo
	{
		Platform::AssetLoaderCreateInfoNativeParams * nativeParams = nullptr;
	};

	struct SysContextCreateInfo
	{
		Platform::SysContextCreateInfoNativeParams * nativeParams = nullptr;
	};

	/// @brief
	class IC3_SYSTEM_CLASS SysContext : public IDynamicObject
	{
	public:
		SysContext();
		virtual ~SysContext() noexcept;

		virtual AssetLoaderHandle CreateAssetLoader( const AssetLoaderCreateInfo & pCreateInfo ) = 0;

		virtual DisplayManagerHandle CreateDisplayManager() = 0;

		virtual EventControllerHandle CreateEventController() = 0;

		virtual FileManagerHandle CreateFileManager() = 0;

		virtual MetalSystemDriverHandle CreateMetalSystemDriver(
				DisplayManagerHandle pDisplayManager,
				const MetalSystemDriverCreateInfo & pCreateInfo );

		virtual OpenGLSystemDriverHandle CreateOpenGLSystemDriver( DisplayManagerHandle pDisplayManager );

		virtual PipeFactoryHandle CreatePipeFactory() = 0;

		virtual WindowManagerHandle CreateWindowManager( DisplayManagerHandle pDisplayManager ) = 0;

		virtual std::string QueryCurrentProcessWorkingDirectory() const = 0;

		virtual std::string QueryCurrentProcessExecutableFilePath() const = 0;

		std::string QueryCurrentProcessExecutableDirectory() const;
	};

	namespace Platform
	{

		/// @brief Creates a new SysContext instance and returns the handle.
		/// @param pCreateInfo
		IC3_SYSTEM_API_NODISCARD SysContextHandle CreateSysContext( const SysContextCreateInfo & pCreateInfo );

	}

} // namespace Ic3::System

#endif // __IC3_SYSTEM_CORE_SYS_CONTEXT_H__
