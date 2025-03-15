
#pragma once

#ifndef __IC3_NXMAIN_VERTEX_FORMAT_PARSING_H__
#define __IC3_NXMAIN_VERTEX_FORMAT_PARSING_H__

#include "VertexFormatCommon.h"
#include "VertexAttributeKey.h"

namespace Ic3
{

	struct VertexFormatCombinedLayoutBase;

	namespace VertexFormat
	{

		IC3_NXMAIN_API_NO_DISCARD cppx::string_view GetAttributeFormatStringIdentifier( GCI::EVertexAttribFormat pAttributeFormat );

		IC3_NXMAIN_API_NO_DISCARD GCI::EVertexAttribFormat ParseAttributeFormatSerialString( const std::string & pAttributeFormatSerialString );

		IC3_NXMAIN_API_NO_DISCARD cppx::string_view GetAttributeFormatStringIdentifier( GCI::EVertexAttribFormat pGCIAttributeFormat );

		IC3_NXMAIN_API_NO_DISCARD cppx::string_view GetShaderSemanticShortName( const cppx::string_view & pSemanticName );

		IC3_NXMAIN_API_NO_DISCARD cppx::string_view ResolveShaderSemanticShortName( const cppx::string_view & pSemanticName );

		IC3_NXMAIN_API_NO_DISCARD std::string GenerateAttributeSerialString( const GenericVertexInputAttribute & pAttribute );

		IC3_NXMAIN_API_NO_DISCARD std::string GenerateVertexFormatSignatureSerialString( const VertexFormatCombinedLayoutBase & pVertexFormatCombinedLayout );

		IC3_NXMAIN_API_NO_DISCARD std::vector<VertexInputAttributeDefinition> ParseVertexFormatSignatureSerialString(
				const std::string_view & pVertexFormatString );

		IC3_NXMAIN_API_NO_DISCARD std::string GenerateVertexFormatSignatureSerialString(
				const VertexFormatCombinedLayoutBase & pVertexFormatCombinedLayout );

	}

} // namespace Ic3

#endif // __IC3_NXMAIN_VERTEX_FORMAT_PARSING_H__
