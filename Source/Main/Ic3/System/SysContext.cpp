
#include <Ic3/System/SysContext.h>

namespace Ic3::System
{

	SysContext::SysContext() = default;

	SysContext::~SysContext() noexcept = default;

	MetalSystemDriverHandle SysContext::CreateMetalSystemDriver(
			DisplayManagerHandle /* pDisplayManager */,
			const MetalSystemDriverCreateInfo & /* pCreateInfo */ )
	{
		Ic3ThrowDesc( eEXCSystemInterfaceNotSupported, "Metal is not supported on the current operating system." );
	}

	OpenGLSystemDriverHandle SysContext::CreateOpenGLSystemDriver( DisplayManagerHandle /* pDisplayManager */ )
	{
		Ic3ThrowDesc( eEXCSystemInterfaceNotSupported, "OpenGL is not supported on the current operating system." );
	}

	std::string SysContext::QueryCurrentProcessExecutableDirectory() const
	{
		auto executableFileDirectory = QueryCurrentProcessExecutableFilePath();

		if( !executableFileDirectory.empty() )
		{
			std::replace( executableFileDirectory.begin(),
			              executableFileDirectory.end(),
			              PCL_ENV_NON_STANDARD_PATH_DELIMITER,
			              PCL_ENV_DEFAULT_PATH_DELIMITER );

			const auto lastStringPosition = executableFileDirectory.length() - 1;
			const auto lastSeparatorPos = executableFileDirectory.find_last_of( PCL_ENV_DEFAULT_PATH_DELIMITER );

			if( ( lastSeparatorPos > 0 ) && ( lastSeparatorPos < lastStringPosition ) )
			{
				executableFileDirectory.resize( lastSeparatorPos );
			}
		}

		return executableFileDirectory;
	}

} // namespace Ic3::System
