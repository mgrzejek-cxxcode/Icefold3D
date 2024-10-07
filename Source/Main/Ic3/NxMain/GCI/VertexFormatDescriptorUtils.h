
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
		VertexFormatDescriptorBuilder & addAttribute( VertexAttributeDefinition pAttributeDefinition );

		VertexFormatDescriptorBuilder & addStandardAttribute(
				VertexAttributeKey pAttributeKey,
				uint32 pStreamIndex,
				uint32 pStreamRelativeOffset = GCI::cxIAVertexAttributeOffsetAppend,
				uint32 pExtraComponentPadding = 0 );

		VertexFormatDescriptorBuilder & addAttribute(
				uint32 pBaseIndex,
				ShaderSemantics pSemantics,
				GCI::EVertexAttribFormat pComponentFormat,
				uint32 pComponentsNum,
				uint32 pStreamIndex,
				uint32 pStreamRelativeOffset = GCI::cxIAVertexAttributeOffsetAppend,
				EVertexDataRate pDataRate = EVertexDataRate::PerVertex );

		template <typename TPVertex, typename TPAttribute>
		VertexFormatDescriptorBuilder & addAttribute(
				TPAttribute TPVertex::* pAttributePtr,
				uint32 pBaseIndex,
				ShaderSemantics pSemantics,
				uint32 pStreamIndex,
				uint32 pStreamRelativeOffset = GCI::cxIAVertexAttributeOffsetAppend,
				EVertexDataRate pDataRate = EVertexDataRate::PerVertex );

		template <typename TPVertex, typename TPAttribute>
		VertexFormatDescriptorBuilder & addStandardAttribute(
				TPAttribute TPVertex::* pAttributePtr,
				VertexAttributeKey pAttributeKey,
				uint32 pStreamIndex,
				uint32 pStreamRelativeOffset = GCI::cxIAVertexAttributeOffsetAppend,
				uint32 pDataStride = 0 );

		TSharedHandle<VertexFormatDescriptor> createVertexFormatDescriptor(
				const GCI::EPrimitiveTopology pPrimitiveTopology,
				const GCI::EIndexDataFormat pIndexDataFormat );

		static TSharedHandle<VertexFormatDescriptor> createVertexFormatDescriptor(
				const GCI::EPrimitiveTopology pPrimitiveTopology,
				const GCI::EIndexDataFormat pIndexDataFormat,
				const TArrayView<VertexAttributeDefinition> & pAttributeDefinitions );

		static TSharedHandle<VertexFormatDescriptor> createVertexFormatDescriptor(
				const GCI::EPrimitiveTopology pPrimitiveTopology,
				const GCI::EIndexDataFormat pIndexDataFormat,
				const std::string_view & pVertexFormatString );

		static std::vector<VertexAttributeDefinition> parseVertexFormatString( const std::string_view & pVertexFormatString );

	private:
		template <typename TPAttributeTraits>
		IC3_ATTR_NO_DISCARD static bool checkAttributeAutoDataFormat(
				GCI::EVertexAttribFormat pAttributeBaseFormat,
				uint32 pAttributeComponentsNum = 0 ) noexcept;

	private:
		IC3_NXMAIN_API_NO_DISCARD static bool adjustAttributeDefinition(
				VertexAttributeDefinition & pAttributeDefinition,
				const VertexAttributeArrayLayout & pAttribArrayLayout,
				const VertexStreamArrayConfig & pStreamArrayConfig ) noexcept;

	private:
		struct VertexAttributeDefinitionCmp
		{
			bool operator()( const VertexAttributeDefinition & pLhs, const VertexAttributeDefinition & pRhs ) const noexcept
			{
				return pLhs.attributeIASlot < pRhs.attributeIASlot;
			}
		};

		Cppx::TSortedArray<VertexAttributeDefinition, VertexAttributeDefinitionCmp> _attributeDefs;
	};

	template <typename TPVertex, typename TPAttribute>
	inline VertexFormatDescriptorBuilder & VertexFormatDescriptorBuilder::addAttribute(
			TPAttribute TPVertex:: * pAttributePtr,
			uint32 pBaseIndex,
			ShaderSemantics pSemantics,
			uint32 pStreamIndex,
			uint32 pStreamRelativeOffset,
			EVertexDataRate pDataRate )
	{
		using Traits = GCI::QVertexAttribFormatDataTypeTraits<TPAttribute>;

		VertexAttributeDefinition attributeDefinition{};
		attributeDefinition.baseFormat = Traits::sBaseAttribFormat;
		attributeDefinition.attributeIASlot = pBaseIndex;
		attributeDefinition.semanticComponentsNum = Traits::sAttribComponentsNum;
		attributeDefinition.componentPadding = 0;
		attributeDefinition.vertexStreamIASlot = numeric_cast<uint16>( pStreamIndex );
		attributeDefinition.vertexStreamRelativeOffset = pStreamRelativeOffset;
		attributeDefinition.dataRate = pDataRate;
		attributeDefinition.shaderSemantics = std::move( pSemantics );

		return addAttribute( std::move( attributeDefinition ) );
	}

	template <typename TPVertex, typename TPAttribute>
	inline VertexFormatDescriptorBuilder & VertexFormatDescriptorBuilder::addStandardAttribute(
			TPAttribute TPVertex:: * pAttributePtr,
			VertexAttributeKey pAttributeKey,
			uint32 pStreamIndex,
			uint32 pStreamRelativeOffset,
			uint32 pDataStride )
	{
		using Traits = GCI::QVertexAttribFormatDataTypeTraits<TPAttribute>;

		const auto fixedAttributeFormat = CxDef::getVertexAttributeKeyBaseFormat( pAttributeKey );
		const auto fixedAttributeComponentsNum = CxDef::getVertexAttributeKeyComponentsNum( pAttributeKey );

		if( !GCU::checkAttributeAutoDataFormat<Traits>( fixedAttributeFormat, fixedAttributeComponentsNum ) )
		{
			ic3DebugOutput( "" );
		}
		else
		{
			VertexAttributeDefinition attributeDefinition{};
			attributeDefinition.attributeIASlot = CxDef::getVertexAttributeKeyBaseIndex( pAttributeKey );
			attributeDefinition.baseFormat = Traits::sBaseAttribFormat;
			attributeDefinition.vertexStreamIASlot = numeric_cast<uint16>( pStreamIndex );
			attributeDefinition.vertexStreamRelativeOffset = pStreamRelativeOffset;
			attributeDefinition.semanticComponentsNum = 0;
			attributeDefinition.dataRate = EVertexDataRate::PerVertex;
			const auto attributeSemanticsID = CxDef::getVertexAttributeKeySemanticID( pAttributeKey );
			attributeDefinition.shaderSemantics = ShaderSemantics{ attributeSemanticsID };
			attributeDefinition.componentPadding = Traits::sAttribComponentsNum;

			addAttribute( std::move( attributeDefinition ) );
		}

		return *this;
	}

	template <typename TPAttributeTraits>
	inline bool VertexFormatDescriptorBuilder::checkAttributeAutoDataFormat(
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

		const auto specifiedComponentByteSize = GCI::CxDef::getVertexAttribFormatByteSize( pAttributeBaseFormat );
		const auto specifiedAttributeByteSize = specifiedComponentByteSize * pAttributeComponentsNum;

		if( specifiedAttributeByteSize != TPAttributeTraits::sSizeInBytes )
		{
			return false;
		}

		return true;
	}

}

#endif // __IC3_NXMAIN_VERTEX_INPUT_DESCRIPTOR_BUILDER_H__