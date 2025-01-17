
#include "GLShaderCommon.h"
#include <cppx/stringUtils.h>
#include <regex>

namespace Ic3::Graphics::GCI
{
	namespace RCU
	{

		bool ProcessGLShaderSourceExplicitLayoutQualifiers(
				const cppx::version & pSupportedAPIVersion,
				std::string & pShaderSource,
				GLShaderDataLayoutMap & pOutputLayoutMap )
		{
			static const std::string cvReStrBaseLayoutBinding{ R"(layout\s*\([\s\,\=a-zA-Z0-9]*binding\s*\=\s*([0-9]+r)[\s\,\=a-zA-Z0-9]*\))" };
			static const std::string cvReStrBaseLayoutLocation{ R"(layout\s*\([\s\,\=a-zA-Z0-9]+location\s*\=\s*([0-9]+)[\s\,\=a-zA-Z0-9]*\))" };
			static const std::string cvReStrBaseInputAttribute{ R"(in\s+[a-zA-Z0-9]+\s*([a-zA-Z0-9]+))" };
			static const std::string cvReStrBaseOutputVariable{ R"(out\s+[a-zA-Z0-9]+\s*([a-zA-Z0-9]+))" };
			static const std::string cvReStrBaseUniformBlock{ R"(uniform\s+([a-zA-Z0-9]+))" };
			static const std::string cvReStrBaseUniformSampler{ R"(uniform\s+sampler[a-zA-Z0-9]+\s+([a-zA-Z0-9]+))" };

			static const std::string cvReStrFullInputAttribute{ cvReStrBaseLayoutLocation + R"(\s*)" + cvReStrBaseInputAttribute + R"(\s*;)" };
			static const std::string cvReStrFullOutputVariable{ cvReStrBaseLayoutLocation + R"(\s*)" + cvReStrBaseOutputVariable + R"(\s*;)" };
			static const std::string cvReStrFullUniformBlock{ cvReStrBaseLayoutBinding + R"(\s*)" + cvReStrBaseUniformBlock };
			static const std::string cvReStrFullUniformSampler{ cvReStrBaseLayoutBinding + R"(\s*)" + cvReStrBaseUniformSampler };

			static const std::regex cvRegexBaseLayoutBinding{ cvReStrBaseLayoutBinding };
			static const std::regex cvRegexBaseLayoutLocation{ cvReStrBaseLayoutLocation };

			static const std::regex cvRegexFullInputAttribute{ cvReStrFullInputAttribute };
			static const std::regex cvRegexFullOutputVariable{ cvReStrFullOutputVariable };
			static const std::regex cvRegexFullUniformBlock{ cvReStrFullUniformBlock };
			static const std::regex cvRegexFullUniformSampler{ cvReStrFullUniformSampler };

			static const std::regex cvRegexUtilLayout{ "layout" };
			static const std::regex cvRegexUtilVersion{ R"(#version\s+[3-4][0-6]0)" };
			static const std::regex cvRegexUtilLeadingSpacesIn{ R"(^\s*in)" };
			static const std::regex cvRegexUtilLeadingSpacesOut{ R"(^\s*out)" };
			static const std::regex cvRegexUtilLeadingSpacesUniform{ R"(^\s*uniform)" };

			using StrLines = std::vector<std::string>;
			auto sourceLines = cppx::strutil::split_string_ex<StrLines>( pShaderSource, '\n', []( auto & pStrLines, auto * pStr, auto pLength ) {
				pStrLines.push_back( std::string( pStr, pLength ) );
			} );

			uint32 layouts = 0;

			for( auto & sourceLine : sourceLines )
			{
				std::smatch regexMatch;
				if( std::regex_search( sourceLine, regexMatch, cvRegexUtilLayout ) )
				{
					if( std::regex_search( sourceLine, regexMatch, cvRegexFullInputAttribute ) )
					{
						const auto & debugView = regexMatch[0].str();
						const auto & attributeLocation = regexMatch[1].str();
						const auto & attributeName = regexMatch[2].str();
						pOutputLayoutMap.attributeLocations[attributeName] = cppx::from_string<uint32>( attributeLocation );
						sourceLine = std::regex_replace( sourceLine, cvRegexBaseLayoutLocation, "" );
						sourceLine = std::regex_replace( sourceLine, cvRegexUtilLeadingSpacesIn, "in" );
						continue;
					}
					if( std::regex_search( sourceLine, regexMatch, cvRegexFullOutputVariable ) )
					{
						const auto & debugView = regexMatch[0].str();
						const auto & outputVarLocation = regexMatch[1].str();
						const auto & outputVarName = regexMatch[2].str();
						pOutputLayoutMap.fragDataLocations[outputVarName] = cppx::from_string<uint32>( outputVarLocation );
						sourceLine = std::regex_replace( sourceLine, cvRegexBaseLayoutLocation, "" );
						sourceLine = std::regex_replace( sourceLine, cvRegexUtilLeadingSpacesOut, "out" );
						continue;
					}
					if( std::regex_search( sourceLine, regexMatch, cvRegexFullUniformSampler ) )
					{
						const auto & debugView = regexMatch[0].str();
						const auto & samplerBinding = regexMatch[1].str();
						const auto & samplerName = regexMatch[2].str();
						pOutputLayoutMap.samplerBindings[samplerName] = cppx::from_string<uint32>( samplerBinding );
						sourceLine = std::regex_replace( sourceLine, cvRegexBaseLayoutBinding, "" );
						sourceLine = std::regex_replace( sourceLine, cvRegexUtilLeadingSpacesUniform, "uniform" );
						continue;
					}
					if( std::regex_search( sourceLine, regexMatch, cvRegexFullUniformBlock ) )
					{
						const auto & debugView = regexMatch[0].str();
						const auto & uniformBlockBinding = regexMatch[1].str();
						const auto & uniformBlockName = regexMatch[2].str();
						pOutputLayoutMap.uniformBlockBindings[uniformBlockName] = cppx::from_string<uint32>( uniformBlockBinding );
						sourceLine = std::regex_replace( sourceLine, cvRegexBaseLayoutBinding, "" );
						sourceLine = std::regex_replace( sourceLine, cvRegexUtilLeadingSpacesUniform, "uniform" );
						continue;
					}

					++layouts;
				}
			}

			pShaderSource.clear();
			for( auto & sourceLine : sourceLines )
			{
				pShaderSource.append( sourceLine );
				pShaderSource.append( 1, '\n' );
			}

			std::string versionStr{ "#version " };
			versionStr.append( cppx::toString<char>( pSupportedAPIVersion.num_major ) );
			versionStr.append( cppx::toString<char>( pSupportedAPIVersion.num_minor ) );
			versionStr.append( 1, '0' );
			pShaderSource = std::regex_replace( pShaderSource, cvRegexUtilVersion, versionStr );

			return !pOutputLayoutMap.IsEmpty();
		}

	}
}
