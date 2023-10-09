
#ifndef __IC3_SYSTEM_CORE_SYS_CONTEXT_H__
#define __IC3_SYSTEM_CORE_SYS_CONTEXT_H__

#include "Prerequisites.h"

namespace Ic3::System
{

	namespace platform
	{

		struct AssetLoaderCreateInfoNativeParams;
		struct SysContextCreateInfoNativeParams;

	}

	struct MetalSystemDriverCreateInfo;

	ic3SysDeclareHandle( SysContext );
	ic3SysDeclareHandle( AssetLoader );
	ic3SysDeclareHandle( DisplayManager );
	ic3SysDeclareHandle( EventController );
	ic3SysDeclareHandle( FileManager );
	ic3SysDeclareHandle( MetalSystemDriver );
	ic3SysDeclareHandle( OpenGLSystemDriver );
	ic3SysDeclareHandle( WindowManager );

	struct AssetLoaderCreateInfo
	{
		platform::AssetLoaderCreateInfoNativeParams * nativeParams = nullptr;
	};

	struct SysContextCreateInfo
	{
		platform::SysContextCreateInfoNativeParams * nativeParams = nullptr;
	};

	/// @brief
	class SysContext : public DynamicInterface
	{
	public:
		SysContext();
		virtual ~SysContext() noexcept;

		virtual AssetLoaderHandle createAssetLoader( const AssetLoaderCreateInfo & pCreateInfo ) = 0;

		virtual DisplayManagerHandle createDisplayManager() = 0;

		virtual EventControllerHandle createEventController() = 0;

		virtual FileManagerHandle createFileManager() = 0;

		virtual MetalSystemDriverHandle createMetalSystemDriver( DisplayManagerHandle pDisplayManager,
																 const MetalSystemDriverCreateInfo & pCreateInfo );

		virtual OpenGLSystemDriverHandle createOpenGLSystemDriver( DisplayManagerHandle pDisplayManager );

		virtual WindowManagerHandle createWindowManager( DisplayManagerHandle pDisplayManager ) = 0;

		virtual std::string queryCurrentProcessWorkingDirectory() const = 0;

		virtual std::string queryCurrentProcessExecutableFilePath() const = 0;

		std::string queryCurrentProcessExecutableDirectory() const;
	};

	namespace platform
	{

		/// @brief Creates a new SysContext instance and returns the handle.
		/// @param pCreateInfo
		IC3_SYSTEM_API_NODISCARD SysContextHandle createSysContext( const SysContextCreateInfo & pCreateInfo );

	}

} // namespace Ic3::System

#endif // __IC3_SYSTEM_CORE_SYS_CONTEXT_H__
