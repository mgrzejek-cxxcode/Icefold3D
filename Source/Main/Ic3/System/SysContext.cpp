
#include <Ic3/System/SysContext.h>

namespace Ic3::System
{

	SysContext::SysContext() = default;

	SysContext::~SysContext() noexcept = default;

	MetalSystemDriverHandle SysContext::createMetalSystemDriver( DisplayManagerHandle /* pDisplayManager */,
	                                                             const MetalSystemDriverCreateInfo & /* pCreateInfo */ )
	{
		ic3ThrowDesc( eEXCSystemInterfaceNotSupported, "Metal is not supported on the current operating system." );
	}

	OpenGLSystemDriverHandle SysContext::createOpenGLSystemDriver( DisplayManagerHandle /* pDisplayManager */ )
	{
		ic3ThrowDesc( eEXCSystemInterfaceNotSupported, "OpenGL is not supported on the current operating system." );
	}

	std::string SysContext::queryCurrentProcessExecutableDirectory() const
	{
		auto executableFileDirectory = queryCurrentProcessExecutableFilePath();

		if( !executableFileDirectory.empty() )
		{
			std::replace( executableFileDirectory.begin(),
			              executableFileDirectory.end(),
			              IC3_PCL_ENV_NON_STANDARD_PATH_DELIMITER,
			              IC3_PCL_ENV_DEFAULT_PATH_DELIMITER );

			const auto lastStringPosition = executableFileDirectory.length() - 1;
			const auto lastSeparatorPos = executableFileDirectory.find_last_of( IC3_PCL_ENV_DEFAULT_PATH_DELIMITER );

			if( ( lastSeparatorPos > 0 ) && ( lastSeparatorPos < lastStringPosition ) )
			{
				executableFileDirectory.resize( lastSeparatorPos );
			}
		}

		return executableFileDirectory;
	}

} // namespace Ic3::System
