
#pragma once

#ifndef __IC3_NXMAIN_VERTEX_INPUT_DESCRIPTOR_BUILDER_H__
#define __IC3_NXMAIN_VERTEX_INPUT_DESCRIPTOR_BUILDER_H__

#include "VertexFormatDescriptor.h"
#include "VertexAttributeCommonDefs.h"

namespace Ic3
{

	class VertexFormatDescriptorBuilder
	{
	public:
		VertexFormatDescriptorBuilder & AddAttribute( VertexInputAttributeDefinition pAttributeDefinition );

		VertexFormatDescriptorBuilder & AddAttribute(
				uint32 pStreamSlot,
				VertexAttributeKey pAttributeKey,
				cppx::immutable_string pSemanticName = {},
				uint32 pDataPadding = 0,
				uint32 pStreamRelativeOffset = GCI::kIAVertexAttributeOffsetAppend );

		VertexFormatDescriptorBuilder & AddAttribute(
				uint32 pStreamSlot,
				uint32 pBaseAttributeIASlot,
				ShaderSemantics pSemantics,
				GCI::EVertexAttribFormat pBaseFormat,
				uint32 pDataPadding = 0,
				uint32 pStreamRelativeOffset = GCI::kIAVertexAttributeOffsetAppend,
				EVertexDataRate pDataRate = EVertexDataRate::PerVertex );

		VertexFormatDescriptorBuilder & AddAttributeSemanticGroup(
				uint32 pStreamSlot,
				uint32 pBaseAttributeIASlot,
				ShaderSemantics pSemantics,
				GCI::EVertexAttribFormat pBaseFormat,
				uint32 pSemanticGroupSize,
				uint32 pDataPadding = 0,
				uint32 pStreamRelativeOffset = GCI::kIAVertexAttributeOffsetAppend,
				EVertexDataRate pDataRate = EVertexDataRate::PerVertex );

		VertexFormatDescriptorBuilder & AddAttributeList(
				const cppx::array_view<VertexInputAttributeDefinition> & pAttributeList );

		VertexFormatDescriptorBuilder & AddAttributeList(
				const cppx::array_view<const VertexInputAttributeDefinition> & pAttributeList );

		TSharedHandle<VertexFormatDescriptor> CreateVertexFormatDescriptor(
				const GCI::EPrimitiveTopology pPrimitiveTopology,
				const GCI::EIndexDataFormat pIndexDataFormat );

		static TSharedHandle<VertexFormatDescriptor> CreateVertexFormatDescriptor(
				const GCI::EPrimitiveTopology pPrimitiveTopology,
				const GCI::EIndexDataFormat pIndexDataFormat,
				const cppx::array_view<VertexInputAttributeDefinition> & pAttributeDefinitions );

		static TSharedHandle<VertexFormatDescriptor> CreateVertexFormatDescriptor(
				const GCI::EPrimitiveTopology pPrimitiveTopology,
				const GCI::EIndexDataFormat pIndexDataFormat,
				const std::string_view & pVertexFormatString );

		static std::vector<VertexInputAttributeDefinition> ParseVertexFormatString( const std::string_view & pVertexFormatString );

	private:
		IC3_NXMAIN_API_NO_DISCARD static bool ValidateAttributeDefinitions(
				const cppx::array_view<VertexInputAttributeDefinition> & pAttributeDefinitionArray ) noexcept;

		IC3_NXMAIN_API_NO_DISCARD static bool AdjustAttributeDefinition(
				VertexInputAttributeDefinition & pAttributeDefinition,
				const VertexInputAttributeArrayConfig & pInputAttributeArrayLayout,
				const VertexInputStreamArrayConfig & pInputStreamArrayConfig ) noexcept;

	private:
		std::vector<VertexInputAttributeDefinition> _attributeDefs;
	};

	/*
	template <typename TPVertex, typename TPAttribute>
	inline VertexFormatDescriptorBuilder & VertexFormatDescriptorBuilder::AddAttribute(
			TPAttribute TPVertex:: * pAttributePtr,
			uint32 pBaseIndex,
			ShaderSemantics pSemantics,
			uint32 streamSlot,
			uint32 pStreamRelativeOffset,
			EVertexDataRate pDataRate )
	{
		using Traits = GCI::QVertexAttribFormatDataTypeTraits<TPAttribute>;

		VertexAttributeDefinition attributeDefinition{};
		attributeDefinition.baseFormat = Traits::sBaseAttribFormat;
		attributeDefinition.attributeSlot = pBaseIndex;
		attributeDefinition.semanticComponentsNum = Traits::sAttribComponentsNum;
		attributeDefinition.componentPadding = 0;
		attributeDefinition.vertexStreamIASlot = cppx::numeric_cast<uint16>( streamSlot );
		attributeDefinition.vertexStreamRelativeOffset = pStreamRelativeOffset;
		attributeDefinition.dataRate = pDataRate;
		attributeDefinition.shaderSemantics = std::move( pSemantics );

		return AddAttribute( std::move( attributeDefinition ) );
	}

	template <typename TPVertex, typename TPAttribute>
	inline VertexFormatDescriptorBuilder & VertexFormatDescriptorBuilder::addStandardAttribute(
			TPAttribute TPVertex:: * pAttributePtr,
			VertexAttributeKey pAttributeKey,
			uint32 streamSlot,
			uint32 pStreamRelativeOffset,
			uint32 pDataStride )
	{
		using Traits = GCI::QVertexAttribFormatDataTypeTraits<TPAttribute>;

		const auto fixedAttributeFormat = CXU::getVertexAttributeKeyBaseFormat( pAttributeKey );
		const auto fixedAttributeComponentsNum = CXU::getVertexAttributeKeyComponentsNum( pAttributeKey );

		if( !GCU::CheckAttributeAutoDataFormat<Traits>( fixedAttributeFormat, fixedAttributeComponentsNum ) )
		{
			Ic3DebugOutput( "" );
		}
		else
		{
			VertexAttributeDefinition attributeDefinition{};
			attributeDefinition.attributeSlot = CXU::getVertexAttributeKeyBaseIndex( pAttributeKey );
			attributeDefinition.baseFormat = Traits::sBaseAttribFormat;
			attributeDefinition.vertexStreamIASlot = cppx::numeric_cast<uint16>( streamSlot );
			attributeDefinition.vertexStreamRelativeOffset = pStreamRelativeOffset;
			attributeDefinition.semanticComponentsNum = 0;
			attributeDefinition.dataRate = EVertexDataRate::PerVertex;
			const auto attributeSemanticsID = CXU::getVertexAttributeKeySystemSemanticFlags( pAttributeKey );
			attributeDefinition.shaderSemantics = ShaderSemantics{ attributeSemanticsID };
			attributeDefinition.componentPadding = Traits::sAttribComponentsNum;

			AddAttribute( std::move( attributeDefinition ) );
		}

		return *this;
	}

	template <typename TPAttributeTraits>
	inline bool VertexFormatDescriptorBuilder::CheckAttributeAutoDataFormat(
			GCI::EVertexAttribFormat pAttributeBaseFormat,
			uint32 pAttributeComponentsNum ) noexcept
	{
		if( pAttributeBaseFormat != TPAttributeTraits::sBaseAttribFormat )
		{
			return false;
		}

		if( ( pAttributeComponentsNum != 0 ) && ( TPAttributeTraits::sAttribComponentsNum != pAttributeComponentsNum ) )
		{
			return false;
		}

		const auto specifiedComponentByteSize = GCI::CXU::getVertexAttribFormatByteSize( pAttributeBaseFormat );
		const auto specifiedAttributeByteSize = specifiedComponentByteSize * pAttributeComponentsNum;

		if( specifiedAttributeByteSize != TPAttributeTraits::sSizeInBytes )
		{
			return false;
		}

		return true;
	}
	 */

}

#endif // __IC3_NXMAIN_VERTEX_INPUT_DESCRIPTOR_BUILDER_H__