
#pragma once

#ifndef __IC3_NXMAIN_VERTEX_FORMAT_UTILS_H__
#define __IC3_NXMAIN_VERTEX_FORMAT_UTILS_H__

#include "VertexFormatSignature.h"
#include "VertexAttributeKey.h"

namespace Ic3
{

	class VertexFormatBuilderBase
	{
	public:
		VertexFormatBuilderBase & AddAttribute( VertexInputAttributeDefinition pAttributeDefinition );

		VertexFormatBuilderBase & AddAttribute(
				uint32 pVertexStreamSlot,
				VertexAttributeKey pAttributeKey,
				uint32 pDataPadding = 0,
				uint32 pVertexStreamRelativeOffset = GCI::kIAVertexAttributeOffsetAppend );

		VertexFormatBuilderBase & AddAttribute(
				uint32 pVertexStreamSlot,
				VertexAttributeKey pAttributeKey,
				const VertexAttributeShaderSemantics & pAttributeShaderSemantics,
				uint32 pDataPadding = 0,
				uint32 pVertexStreamRelativeOffset = GCI::kIAVertexAttributeOffsetAppend );

		VertexFormatBuilderBase & AddAttribute(
				uint32 pVertexStreamSlot,
				uint32 pAttributeSlot,
				const VertexAttributeShaderSemantics & pAttributeShaderSemantics,
				GCI::EVertexAttribFormat pDataFormat,
				uint32 pDataPadding = 0,
				uint32 pVertexStreamRelativeOffset = GCI::kIAVertexAttributeOffsetAppend,
				GCI::EIAVertexAttributeDataRate pDataRate = GCI::EIAVertexAttributeDataRate::PerVertex );

		VertexFormatBuilderBase & AddAttributeSemanticGroup(
				uint32 pVertexStreamSlot,
				uint32 pBaseAttributeSlot,
				const VertexAttributeShaderSemantics & pAttributeShaderSemantics,
				uint32 pSemanticGroupSize,
				GCI::EVertexAttribFormat pBaseDataFormat,
				uint32 pDataPadding = 0,
				uint32 pVertexStreamRelativeOffset = GCI::kIAVertexAttributeOffsetAppend,
				GCI::EIAVertexAttributeDataRate pDataRate = GCI::EIAVertexAttributeDataRate::PerVertex );

		VertexFormatBuilderBase & AddAttributeList(
				const cppx::array_view<VertexInputAttributeDefinition> & pAttributeDefinitionList );

		VertexFormatBuilderBase & AddAttributeList(
				const cppx::array_view<const VertexInputAttributeDefinition> & pAttributeDefinitionList );

	protected:
		static bool GenerateVertexFormatData(
				VertexInputAttributeArrayConfig & pAttributeArrayConfig,
				VertexInputStreamArrayConfig & pStreamArrayConfig,
				const cppx::array_view<VertexInputAttributeDefinition> & pAttributeDefinitionList );
	private:
		IC3_NXMAIN_API_NO_DISCARD static bool _ValidateAttributeDefinitions(
				const cppx::array_view<VertexInputAttributeDefinition> & pAttributeDefinitionList ) noexcept;

		IC3_NXMAIN_API_NO_DISCARD static bool _AdjustAttributeDefinition(
				VertexInputAttributeDefinition & pAttributeDefinition,
				const VertexInputAttributeArrayConfig & pAttributeArrayConfig,
				const VertexInputStreamArrayConfig & pStreamArrayConfig ) noexcept;

	protected:
		std::vector<VertexInputAttributeDefinition> _attributeDefinitionArray;
	};

	class VertexFormatSignatureBuilder : public VertexFormatBuilderBase
	{
	public:
		TSharedHandle<VertexFormatSignature> CreateVertexFormatSignature(
				const GCI::EPrimitiveTopology pPrimitiveTopology,
				const GCI::EIndexDataFormat pIndexDataFormat );

		bool InitializeVertexFormatSignature(
				VertexFormatSignature & pVertexFormatSignature,
				const GCI::EPrimitiveTopology pPrimitiveTopology,
				const GCI::EIndexDataFormat pIndexDataFormat );

		static TSharedHandle<VertexFormatSignature> CreateVertexFormatSignatureFromDefinitionList(
				const cppx::array_view<VertexInputAttributeDefinition> & pAttributeDefinitionList,
				const GCI::EPrimitiveTopology pPrimitiveTopology,
				const GCI::EIndexDataFormat pIndexDataFormat );

		static TSharedHandle<VertexFormatSignature> CreateVertexFormatSignatureFromSerialString(
				const std::string_view & pVertexFormatSignatureSerialString,
				const GCI::EPrimitiveTopology pPrimitiveTopology,
				const GCI::EIndexDataFormat pIndexDataFormat );

		static bool InitializeVertexFormatSignatureFromDefinitionList(
				VertexFormatSignature & pVertexFormatSignature,
				const cppx::array_view<VertexInputAttributeDefinition> & pAttributeDefinitionList,
				const GCI::EPrimitiveTopology pPrimitiveTopology,
				const GCI::EIndexDataFormat pIndexDataFormat );

		static bool InitializeVertexFormatSignatureFromSerialString(
				VertexFormatSignature & pVertexFormatSignature,
				const std::string_view & pVertexFormatSignatureSerialString,
				const GCI::EPrimitiveTopology pPrimitiveTopology,
				const GCI::EIndexDataFormat pIndexDataFormat );
	};

	class GCIVertexAttributeLayoutBuilder : public VertexFormatBuilderBase
	{
	public:
		GCI::IAVertexAttributeLayoutDefinition CreateGCIVertexAttributeLayoutDefinition(
			const GCI::EPrimitiveTopology pPrimitiveTopology );

		bool InitializeGCIVertexAttributeLayoutDefinition(
		GCI::IAVertexAttributeLayoutDefinition & pLayoutDefinition,
		const GCI::EPrimitiveTopology pPrimitiveTopology );

		static GCI::IAVertexAttributeLayoutDefinition CreateGCIVertexAttributeLayoutDefinition(
		const cppx::array_view<VertexInputAttributeDefinition> & pAttributeDefinitionList,
		const GCI::EPrimitiveTopology pPrimitiveTopology );

		static bool InitializeGCIVertexAttributeLayoutDefinition(
			GCI::IAVertexAttributeLayoutDefinition & pLayoutDefinition,
			const cppx::array_view<VertexInputAttributeDefinition> & pAttributeDefinitionList,
			const GCI::EPrimitiveTopology pPrimitiveTopology );
	};

	namespace GCIUtils
	{

		VertexInputAttributeDefinition MakeVertexInputAttributeDefinition(
				uint32 pVertexStreamSlot,
				VertexAttributeKey pAttributeKey,
				uint32 pDataPadding = 0,
				uint32 pVertexStreamRelativeOffset = GCI::kIAVertexAttributeOffsetAppend );

		VertexInputAttributeDefinition MakeVertexInputAttributeDefinition(
				uint32 pVertexStreamSlot,
				VertexAttributeKey pAttributeKey,
				const VertexAttributeShaderSemantics & pAttributeShaderSemantics,
				uint32 pDataPadding = 0,
				uint32 pVertexStreamRelativeOffset = GCI::kIAVertexAttributeOffsetAppend );

		VertexInputAttributeDefinition MakeVertexInputAttributeSemanticGroupDefinition(
				uint32 pVertexStreamSlot,
				uint32 pBaseAttributeSlot,
				const VertexAttributeShaderSemantics & pAttributeShaderSemantics,
				uint32 pSemanticGroupSize,
				GCI::EVertexAttribFormat pBaseDataFormat,
				uint32 pDataPadding = 0,
				uint32 pVertexStreamRelativeOffset = GCI::kIAVertexAttributeOffsetAppend,
				GCI::EIAVertexAttributeDataRate pDataRate = GCI::EIAVertexAttributeDataRate::PerVertex );


	}

}

#endif // __IC3_NXMAIN_VERTEX_FORMAT_UTILS_H__