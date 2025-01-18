
#include "fsUtils.h"
#include "stringUtils.h"

namespace cppx
{

	std::string fs_normalize_path( const std::string & pPath )
	{
		auto localPathString = pPath;
		std::replace(
			localPathString.begin(),
			localPathString.end(),
			PCL_ENV_NON_STANDARD_PATH_DELIMITER,
			PCL_ENV_DEFAULT_PATH_DELIMITER );

		using StringVec = std::vector<std::string>;
		auto pathComponents = strutil::split_string_ex<StringVec>(
			localPathString, PCL_ENV_DEFAULT_PATH_DELIMITER,
			[]( StringVec & pVec, const char * pStr, size_t pLength ) -> void {
				if( pLength > 0 )
				{
					pVec.push_back( std::string( pStr, pLength ) );
				}
			});

		auto resultLength = localPathString.length();

		for( size_t pathComponentIndex = 0; pathComponentIndex < pathComponents.size();  )
		{
			auto & component = pathComponents[pathComponentIndex];

			if( component == "." )
			{
				pathComponents.erase( pathComponents.begin() + pathComponentIndex );
				resultLength -= 2;
			}
			else if( component == ".." )
			{
				if( pathComponentIndex == 0 )
				{
					return {};
				}

				auto & prevComponent = pathComponents[pathComponentIndex - 1];
				auto removalSize = 3 + prevComponent.length() + 1;

				pathComponents.erase( pathComponents.begin() + pathComponentIndex );
				pathComponents.erase( pathComponents.begin() + pathComponentIndex - 1 );

				resultLength -= removalSize;
				pathComponentIndex -= 1;
			}
			else
			{
				pathComponentIndex += 1;
			}
		}

		std::string result;
		result.reserve( resultLength );

		if( localPathString[0] == PCL_ENV_DEFAULT_PATH_DELIMITER )
		{
			result.append( 1, PCL_ENV_DEFAULT_PATH_DELIMITER );
		}

		for( size_t pathComponentIndex = 0; pathComponentIndex < pathComponents.size(); ++pathComponentIndex )
		{
			if( pathComponentIndex > 0 )
			{
				result.append( 1, PCL_ENV_DEFAULT_PATH_DELIMITER );
			}
			result.append( pathComponents[pathComponentIndex] );
		}

		return result;
	}

	file_path_info fs_split_path( std::string pFilePath, bitmask<fs_api_flags> pFlags )
	{
		file_path_info pathInfo;

		if( !pFilePath.empty() )
		{
			if( !pFlags.is_set_any_of( fs_api_flag_split_assume_directory_bit | fs_api_flag_split_assume_file_bit ) )
			{
				pFlags.set( fs_api_flag_split_assume_directory_bit );
			}
			else if( pFlags.is_set( fs_api_flag_split_assume_directory_bit | fs_api_flag_split_assume_file_bit ) )
			{
				pFlags.unset( fs_api_flag_split_assume_file_bit );
			}

			auto lastSeparatorPos = pFilePath.find_last_of( PCL_ENV_DEFAULT_PATH_DELIMITER );
			if( lastSeparatorPos == std::string::npos )
			{
				if( fs_is_filename_with_extension( pFilePath ) || pFlags.is_set( fs_api_flag_split_assume_file_bit ) )
				{
					pathInfo.file_name = pFilePath;
				}
			}
			else
			{
				auto directoryPart = pFilePath.substr( 0, lastSeparatorPos );
				auto filenamePath = pFilePath.substr( lastSeparatorPos + 1 );

				if( fs_is_filename_with_extension( filenamePath ) || pFlags.is_set( fs_api_flag_split_assume_file_bit ) )
				{
					pathInfo.directory = std::move( directoryPart );
					pathInfo.file_name = std::move( filenamePath );
				}
			}

			if( pathInfo.directory.empty() && pathInfo.file_name.empty() )
			{
				pathInfo.directory = std::move( pFilePath );
			}
		}

		return pathInfo;
	}

	bool fs_is_filename_with_extension( const std::string & pFilename )
	{
		if( !pFilename.empty() )
		{
			auto dotPos = pFilename.find_last_of( '.' );
			if( ( dotPos != std::string::npos ) && ( dotPos != 0 ) && ( dotPos != ( pFilename.length() - 1 ) ) )
			{
				return true;
			}
		}
		return false;
	}

}
