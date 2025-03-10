
#include "GLShaderCommon.h"
#include <cppx/stringUtils.h>
#include <regex>

#define RE_STR_BASE_SEMANTICS_ONLY         R"((\s*\,\s*semantic\s*\=\s*([a-zA-Z0-9_]+)\s*))"
#define RE_STR_BASE_BINDING_ONLY           R"(\s*binding\s*\=\s*([0-9]+)\s*\,?)"
#define RE_STR_BASE_LOCATION_ONLY          R"(\s*location\s*\=\s*([0-9]+)\s*\,?)"
#define RE_STR_BASE_LAYOUT_EMPTY           R"(layout\s*\(\s*\)\s*)"
#define RE_STR_BASE_LAYOUT_BINDING         R"(layout\s*\()" RE_STR_BASE_BINDING_ONLY  R"(([a-zA-Z0-9_\=]*\,?\s*)*\))"
#define RE_STR_BASE_LAYOUT_LOCATION        R"(layout\s*\()" RE_STR_BASE_LOCATION_ONLY R"(([a-zA-Z0-9_\=]*\,?\s*)*\))"
#define RE_STR_BASE_LAYOUT_LOCATION_SEM    R"(layout\s*\()" RE_STR_BASE_LOCATION_ONLY RE_STR_BASE_SEMANTICS_ONLY R"(\))"

#define RE_STR_BASE_VAR_ATTRIBUTE          R"([a-zA-Z0-9_]*\s*in\s+([a-zA-Z0-9_]+)\s*([a-zA-Z0-9_]+))"
#define RE_STR_BASE_VAR_OUTPUT_VARIABLE    R"([a-zA-Z0-9_]*\s*out\s+([a-zA-Z0-9_]+)\s*([a-zA-Z0-9_]+))"
#define RE_STR_BASE_VAR_UNIFORM_BLOCK      R"(uniform\s+([a-zA-Z0-9_]+))"
#define RE_STR_BASE_VAR_UNIFORM_SAMPLER    R"(uniform\s+sampler[a-zA-Z0-9]+\s+([a-zA-Z0-9_]+))"

#define RE_STR_FULL_VAR_ATTRIBUTE          RE_STR_BASE_LAYOUT_LOCATION     R"(\s*)" RE_STR_BASE_VAR_ATTRIBUTE       R"(\s*)"
#define RE_STR_FULL_VAR_ATTRIBUTE_SEM      RE_STR_BASE_LAYOUT_LOCATION_SEM R"(\s*)" RE_STR_BASE_VAR_ATTRIBUTE       R"(\s*)"
#define RE_STR_FULL_VAR_OUTPUT_VARIABLE    RE_STR_BASE_LAYOUT_LOCATION     R"(\s*)" RE_STR_BASE_VAR_OUTPUT_VARIABLE R"(\s*)"
#define RE_STR_FULL_VAR_UNIFORM_BLOCK      RE_STR_BASE_LAYOUT_BINDING      R"(\s*)" RE_STR_BASE_VAR_UNIFORM_BLOCK   R"(\s*)"
#define RE_STR_FULL_VAR_UNIFORM_SAMPLER    RE_STR_BASE_LAYOUT_BINDING      R"(\s*)" RE_STR_BASE_VAR_UNIFORM_SAMPLER R"(\s*)"

#define RE_STR_ATTRIBUTE_FORMAT_BASE       R"(bool|int|uint|float|double)"
#define RE_STR_ATTRIBUTE_FORMAT_VEC        R"((b|i|u|d)?vec(2|3|4))"
#define RE_STR_ATTRIBUTE_FORMAT_MAT        R"(mat(2|3|4)(x(2|3|4))?)"

#define RE_STR_UTIL_LAYOUT_WORD_ONLY       R"(layout)"
#define RE_STR_UTIL_VERSION_NUMBER         R"(#version\s+[3-4][0-6]0)"
#define RE_STR_UTIL_LEADING_SPACES_IN      R"(^\s*in)"
#define RE_STR_UTIL_LEADING_SPACES_OUT     R"(^\s*out)"
#define RE_STR_UTIL_LEADING_SPACES_UNIFORM R"(^\s*uniform)"

static const std::regex reBaseSemanticsOnly{ RE_STR_BASE_SEMANTICS_ONLY };
static const std::regex reBaseBindingOnly{ RE_STR_BASE_BINDING_ONLY };
static const std::regex reBaseLocationOnly{ RE_STR_BASE_LOCATION_ONLY };
static const std::regex reBaseLayoutEmpty{ RE_STR_BASE_LAYOUT_EMPTY };
static const std::regex reFullVarAttribute{ RE_STR_FULL_VAR_ATTRIBUTE };
static const std::regex reFullVarAttributeSem{ RE_STR_FULL_VAR_ATTRIBUTE_SEM };
static const std::regex reFullVarOutputVariable{ RE_STR_FULL_VAR_OUTPUT_VARIABLE };
static const std::regex reFullVarUniformBlock{ RE_STR_FULL_VAR_UNIFORM_BLOCK };
static const std::regex reFullVarUniformSampler{ RE_STR_FULL_VAR_UNIFORM_SAMPLER };

static const std::regex reUtilLayoutWordOnly{ RE_STR_UTIL_LAYOUT_WORD_ONLY };
static const std::regex reUtilVersionNumber{ RE_STR_UTIL_VERSION_NUMBER };
static const std::regex reUtilLeadingSpacesIn{ RE_STR_UTIL_LEADING_SPACES_IN };
static const std::regex reUtilLeadingSpacesOut{ RE_STR_UTIL_LEADING_SPACES_OUT };
static const std::regex reUtilLeadingSpacesUniform{ RE_STR_UTIL_LEADING_SPACES_UNIFORM };

static const std::regex reAttribFormatBase{ RE_STR_ATTRIBUTE_FORMAT_BASE };
static const std::regex reAttribFormatVec{ RE_STR_ATTRIBUTE_FORMAT_VEC };
static const std::regex reAttribFormatMat{ RE_STR_ATTRIBUTE_FORMAT_MAT };

namespace Ic3::Graphics::GCI
{

	namespace RCU
	{

		ShaderAttributeFormatDesc GetAttributeFormatDesc( const std::string & pAttributeFormatStr )
		{
			ShaderAttributeFormatDesc attributeFormatDesc{};

			std::smatch regexMatch;
			if( std::regex_search( pAttributeFormatStr, regexMatch, reAttribFormatBase ) )
			{
				const auto baseTypeStr = regexMatch[0].str();

				if( baseTypeStr == "bool" )
				{
					attributeFormatDesc.baseType = EBaseDataType::Bool;
				}
				else if( baseTypeStr == "int" )
				{
					attributeFormatDesc.baseType = EBaseDataType::Int32;
				}
				else if( baseTypeStr == "uint" )
				{
					attributeFormatDesc.baseType = EBaseDataType::Uint32;
				}
				else if( baseTypeStr == "float" )
				{
					attributeFormatDesc.baseType = EBaseDataType::Float32;
				}
				else if( baseTypeStr == "double" )
				{
					attributeFormatDesc.baseType = EBaseDataType::Double;
				}
				attributeFormatDesc.baseComponentsNum = 1;
				attributeFormatDesc.semanticGroupSize = 1;
			}
			else if( std::regex_search( pAttributeFormatStr, regexMatch, reAttribFormatVec ) )
			{
				const auto baseTypePrefixStr = regexMatch[1].str();
				const auto vectorSizeStr = regexMatch[2].str();
				const auto vectorSize = cppx::from_string<uint32>( vectorSizeStr );

				if( baseTypePrefixStr == "b" )
				{
					attributeFormatDesc.baseType = EBaseDataType::Bool;
				}
				else if( baseTypePrefixStr == "i" )
				{
					attributeFormatDesc.baseType = EBaseDataType::Int32;
				}
				else if( baseTypePrefixStr == "u" )
				{
					attributeFormatDesc.baseType = EBaseDataType::Uint32;
				}
				else if( baseTypePrefixStr == "d" )
				{
					attributeFormatDesc.baseType = EBaseDataType::Double;
				}
				else // Float is the default type for vectors in GLSL, no prefix in this case.
				{
					attributeFormatDesc.baseType = EBaseDataType::Float32;
				}

				attributeFormatDesc.baseComponentsNum = cppx::from_string<uint8>( vectorSizeStr );
				attributeFormatDesc.semanticGroupSize = 1;
			}
			else if( std::regex_search( pAttributeFormatStr, regexMatch, reAttribFormatMat ) )
			{
				const auto matrixBaseSizeStr = regexMatch[1].str();
				const auto matrixExtSizeStr = regexMatch[3].str();

				attributeFormatDesc.baseType = EBaseDataType::Float32;

				if( matrixExtSizeStr.empty() )
				{
					const auto matrixSize = cppx::from_string<uint8>( matrixBaseSizeStr );
					attributeFormatDesc.baseComponentsNum = matrixSize;
					attributeFormatDesc.semanticGroupSize = matrixSize;
				}
				else
				{
					const auto matrixColumnsNum = cppx::from_string<uint8>( matrixBaseSizeStr );
					const auto matrixBaseElementsNum = cppx::from_string<uint8>( matrixExtSizeStr );
					attributeFormatDesc.baseComponentsNum = matrixBaseElementsNum;
					attributeFormatDesc.semanticGroupSize = matrixColumnsNum;
				}
			}

			return attributeFormatDesc;
		}

		void GenerateShaderInputSignatureAndRemoveSemanticsInfo(
				const cppx::array_view<std::string> & pShaderSourceLines,
				ShaderInputSignature & pInputSignature )
		{
			pInputSignature.activeAttributesMask.clear();

			for( auto & sourceLine : pShaderSourceLines )
			{
				std::smatch regexMatch;
				if( std::regex_search( sourceLine, regexMatch, reUtilLayoutWordOnly ) )
				{
					if( std::regex_search( sourceLine, regexMatch, reFullVarAttributeSem ) )
					{
					#if( PCL_BUILD_TYPE == PCL_BUILD_TYPE_DEBUG )
						const auto debugViewStr = regexMatch[0].str();
					#endif

						const auto attributeLocationStr = regexMatch[1].str();
						// const auto attributeSemanticSectionStr = regexMatch[2].str();
						const auto attributeSemanticNameStr = regexMatch[3].str();
						const auto attributeFormatStr = regexMatch[4].str();
						// const auto attributeNameStr = regexMatch[5].str();

						const auto attributeLocation = cppx::from_string<uint32>( attributeLocationStr );
						const auto attributeFormatDesc = GetAttributeFormatDesc( attributeFormatStr );
						const auto attributeFormatValue = CXU::MakeVertexAttribFormatEnumValue(
								attributeFormatDesc.baseComponentsNum,
								attributeFormatDesc.baseType );

						for( uint32 iSemanticIndex = 0; iSemanticIndex < attributeFormatDesc.semanticGroupSize; ++iSemanticIndex )
						{
							const auto attributeIndex = attributeLocation + iSemanticIndex;

							auto & attributeSignature = pInputSignature.attributeSignatures[attributeIndex];
							attributeSignature.format = static_cast<EVertexAttribFormat>( attributeFormatValue );
							attributeSignature.semanticName = attributeSemanticNameStr;
							attributeSignature.semanticIndex = cppx::numeric_cast<uint8>( iSemanticIndex );

							pInputSignature.activeAttributesMask.set( GCI::CXU::IAMakeVertexAttributeFlag( attributeIndex ) );
						}

						sourceLine = std::regex_replace( sourceLine, reBaseSemanticsOnly, " " );
						sourceLine = std::regex_replace( sourceLine, reBaseLayoutEmpty, "" );
					}
				}
			}
		}

		void GenerateGLShaderBindingLayoutAndRemoveExplicitLayoutSpecifiers(
				const cppx::array_view<std::string> & pShaderSourceLines,
				GLShaderBindingLayout & pBindingLayout )
		{
			for( auto & sourceLine : pShaderSourceLines )
			{
				std::smatch regexMatch;
				if( std::regex_search( sourceLine, regexMatch, reUtilLayoutWordOnly ) )
				{
					if( std::regex_search( sourceLine, regexMatch, reFullVarAttribute ) )
					{
					#if( PCL_BUILD_TYPE == PCL_BUILD_TYPE_DEBUG )
						const auto debugView = regexMatch[0].str();
					#endif
						const auto attributeLocation = regexMatch[1].str();
						// regexMatch[2] -> optional additional layout qualifiers
						// regexMatch[3] -> attribute format (GLSL type)
						const auto attributeName = regexMatch[4].str();
						pBindingLayout.attributeLocations[attributeName] = cppx::from_string<uint32>( attributeLocation );
						sourceLine = std::regex_replace( sourceLine, reBaseLocationOnly, "" );
						sourceLine = std::regex_replace( sourceLine, reBaseLayoutEmpty, "" );
						sourceLine = std::regex_replace( sourceLine, reUtilLeadingSpacesIn, "in" );
					}
					else if( std::regex_search( sourceLine, regexMatch, reFullVarOutputVariable ) )
					{
					#if( PCL_BUILD_TYPE == PCL_BUILD_TYPE_DEBUG )
						const auto debugView = regexMatch[0].str();
					#endif
						const auto outputVarLocation = regexMatch[1].str();
						// regexMatch[2] -> optional additional layout qualifiers
						// regexMatch[3] -> variable format (GLSL type)
						const auto outputVarName = regexMatch[4].str();
						pBindingLayout.fragDataLocations[outputVarName] = cppx::from_string<uint32>( outputVarLocation );
						sourceLine = std::regex_replace( sourceLine, reBaseLocationOnly, "" );
						sourceLine = std::regex_replace( sourceLine, reBaseLayoutEmpty, "" );
						sourceLine = std::regex_replace( sourceLine, reUtilLeadingSpacesOut, "out" );
					}
					else if( std::regex_search( sourceLine, regexMatch, reFullVarUniformBlock ) )
					{
					#if( PCL_BUILD_TYPE == PCL_BUILD_TYPE_DEBUG )
						const auto debugView = regexMatch[0].str();
					#endif
						const auto uniformBlockBinding = regexMatch[1].str();
						// regexMatch[2] -> optional additional layout qualifiers
						const auto uniformBlockName = regexMatch[3].str();
						pBindingLayout.uniformBlockBindings[uniformBlockName] = cppx::from_string<uint32>( uniformBlockBinding );
						sourceLine = std::regex_replace( sourceLine, reBaseBindingOnly, "" );
						sourceLine = std::regex_replace( sourceLine, reBaseLayoutEmpty, "" );
						sourceLine = std::regex_replace( sourceLine, reUtilLeadingSpacesUniform, "uniform" );
					}
					else if( std::regex_search( sourceLine, regexMatch, reFullVarUniformSampler ) )
					{
						Ic3DebugInterrupt(); /// @TODO: Not tested, check regexMatch[N]
					#if( PCL_BUILD_TYPE == PCL_BUILD_TYPE_DEBUG )
						const auto debugView = regexMatch[0].str();
					#endif
						const auto samplerBinding = regexMatch[1].str();
						const auto samplerName = regexMatch[4].str();
						pBindingLayout.samplerBindings[samplerName] = cppx::from_string<uint32>( samplerBinding );
						sourceLine = std::regex_replace( sourceLine, reBaseBindingOnly, "" );
						sourceLine = std::regex_replace( sourceLine, reBaseLayoutEmpty, "" );
						sourceLine = std::regex_replace( sourceLine, reUtilLeadingSpacesUniform, "uniform" );
					}
				}
			}
		}

		bool ProcessGLShaderSource(
				const cppx::version & pSupportedAPIVersion,
				std::string & pShaderSource,
				ShaderInputSignature & pInputSignature,
				GLShaderBindingLayout * pBindingLayout )
		{

			using StrLines = std::vector<std::string>;
			auto sourceLines = cppx::strutil::split_string_ex<StrLines>( pShaderSource, '\n', []( auto & pStrLines, auto * pStr, auto pLength ) {
				pStrLines.push_back( std::string( pStr, pLength ) );
			} );

			GenerateShaderInputSignatureAndRemoveSemanticsInfo( cppx::bind_array_view( sourceLines ), pInputSignature );

			if( pBindingLayout )
			{
				GenerateGLShaderBindingLayoutAndRemoveExplicitLayoutSpecifiers( cppx::bind_array_view( sourceLines ), *pBindingLayout );
			}

			pShaderSource.clear();
			for( auto & sourceLine : sourceLines )
			{
				pShaderSource.append( sourceLine );
				pShaderSource.append( 1, '\n' );
			}

			std::string versionStr{ "#version " };
			versionStr.append( cppx::to_string<char>( pSupportedAPIVersion.num_major ) );
			versionStr.append( cppx::to_string<char>( pSupportedAPIVersion.num_minor ) );
			versionStr.append( 1, '0' );
			pShaderSource = std::regex_replace( pShaderSource, reUtilVersionNumber, versionStr );

			return true;
		}

	}

}
