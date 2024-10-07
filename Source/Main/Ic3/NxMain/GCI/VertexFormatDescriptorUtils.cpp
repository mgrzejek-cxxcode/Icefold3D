
#include "VertexFormatDescriptorUtils.h"
#include <Ic3/Cppx/StringUtils.h>
#include <regex>

namespace Ic3
{

	VertexFormatDescriptorBuilder & VertexFormatDescriptorBuilder::addAttribute( VertexAttributeDefinition pAttributeDefinition )
	{
		if( !pAttributeDefinition.valid() )
		{
			ic3DebugOutput( "" );
		}
		else
		{
			_attributeDefs.insert( std::move( pAttributeDefinition ) );
		}

		return *this;
	}

	VertexFormatDescriptorBuilder & VertexFormatDescriptorBuilder::addStandardAttribute(
			VertexAttributeKey pAttributeKey,
			uint32 pStreamIndex,
			uint32 pStreamRelativeOffset,
			uint32 pExtraComponentPadding )
	{
		VertexAttributeDefinition attributeDefinition;
		attributeDefinition.attributeIASlot = CxDef::getVertexAttributeKeyBaseIndex( pAttributeKey );
		attributeDefinition.baseFormat = CxDef::getVertexAttributeKeyBaseFormat( pAttributeKey );
		attributeDefinition.vertexStreamIASlot = numeric_cast<uint16>( pStreamIndex );
		attributeDefinition.vertexStreamRelativeOffset = numeric_cast<uint32>( pStreamRelativeOffset );
		attributeDefinition.componentPadding = numeric_cast<uint16>( pExtraComponentPadding );
		const auto isPerInstance = CxDef::isVertexAttributeKeyPerInstance( pAttributeKey );
		attributeDefinition.dataRate = isPerInstance ? EVertexDataRate::PerInstance : EVertexDataRate::PerVertex;
		const auto attributeSemanticsID = CxDef::getVertexAttributeKeySemanticID( pAttributeKey );
		attributeDefinition.shaderSemantics = ShaderSemantics{ attributeSemanticsID };
		attributeDefinition.semanticComponentsNum = CxDef::getVertexAttributeKeyComponentsNum( pAttributeKey );

		return addAttribute( std::move( attributeDefinition ) );
	}

	TSharedHandle<VertexFormatDescriptor> VertexFormatDescriptorBuilder::createVertexFormatDescriptor(
			const GCI::EPrimitiveTopology pPrimitiveTopology,
			const GCI::EIndexDataFormat pIndexDataFormat )
	{
		auto vertexFormatDescriptor = createVertexFormatDescriptor(
				pPrimitiveTopology,
				pIndexDataFormat,
				Cppx::bindArrayView( _attributeDefs ) );

		_attributeDefs.clear();

		return vertexFormatDescriptor;
	}

	TSharedHandle<VertexFormatDescriptor> VertexFormatDescriptorBuilder::createVertexFormatDescriptor(
			const GCI::EPrimitiveTopology pPrimitiveTopology,
			const GCI::EIndexDataFormat pIndexDataFormat,
			const TArrayView<VertexAttributeDefinition> & pAttributeDefinitions )
	{
		if( ( pIndexDataFormat == GCI::EIndexDataFormat::Undefined ) || ( pPrimitiveTopology == GCI::EPrimitiveTopology::Undefined ) )
		{
			return nullptr;
		}

		auto vertexFormatDescriptor = createDynamicObject<VertexFormatDescriptor>();

		auto & attribArrayLayout = vertexFormatDescriptor->_attribArrayLayout;
		auto & streamArrayConfig = vertexFormatDescriptor->_streamArrayConfig;

		bool activeAttributesFlagArray[GCM::cxIAMaxVertexAttributesNum]{};
		bool activeStreamsFlagArray[GCM::cxIAMaxVertexStreamsNum]{};

		uint32 activeAttributesNum = 0;
		uint32 activeStreamsNum = 0;

		for( auto & attributeDefinition : pAttributeDefinitions )
		{
			if( !attribArrayLayout.checkAttributeDefinitionCompatibility( attributeDefinition ) )
			{
				activeAttributesNum = 0;
				break;
			}

			if( !streamArrayConfig.checkAttributeDefinitionCompatibility( attributeDefinition ) )
			{
				activeAttributesNum = 0;
				break;
			}

			if( !activeAttributesFlagArray[attributeDefinition.attributeIASlot] )
			{
				activeAttributesFlagArray[attributeDefinition.attributeIASlot] = true;
				activeAttributesNum += attributeDefinition.semanticComponentsNum;
			}

			if( !activeStreamsFlagArray[attributeDefinition.vertexStreamIASlot] )
			{
				activeStreamsFlagArray[attributeDefinition.vertexStreamIASlot] = true;
				activeStreamsNum += 1;
			}
		}

		if( activeAttributesNum == 0 )
		{
			return nullptr;
		}

		attribArrayLayout.reserveAttributeArraySpace( activeAttributesNum );
		streamArrayConfig.reserveAttributeArraySpace( activeStreamsNum );

		for( auto & attributeDefinition : pAttributeDefinitions )
		{
			if( !adjustAttributeDefinition( attributeDefinition, attribArrayLayout, streamArrayConfig ) )
			{
				vertexFormatDescriptor.reset();
				break;
			}

			const auto attributeDataRate = attributeDefinition.dataRate;

			auto * baseAttributeComponent = attribArrayLayout.addActiveAttribute( std::move( attributeDefinition ) );

			streamArrayConfig.appendAttributeAuto( attributeDefinition.vertexStreamIASlot, attributeDataRate, *baseAttributeComponent );
		}

		return vertexFormatDescriptor;
	}

	TSharedHandle<VertexFormatDescriptor> VertexFormatDescriptorBuilder::createVertexFormatDescriptor(
			const GCI::EPrimitiveTopology pPrimitiveTopology,
			const GCI::EIndexDataFormat pIndexDataFormat,
			const std::string_view & pVertexFormatString )
	{
		return {};
	}

	// Example VFS (second stream S1 set intentionally as empty):
	// "#S0(V)<A0POSITION:0:3F32|A1NORMAL:12:3F32|A2TANGENT:24:3F32|A3BITANGENT:36:3F32|A5TEXCOORD0:48:2F32>#S1(I)<>#S2(I)<A14INSTANCE_UDATA:0:2F32[2]>"

	// Attribute format: A{1}{2}:{3}:{4}[+{5}][[{6}]]
	// {1} -> Input Assembler slot
	// {2} -> Semantic name
	// {3} -> Relative offset (from start of the Vertex Stream)
	// {4} -> Base format (format of a single component)
	// {5} -> Additional per-component padding (OPTIONAL)
	// {6} -> Array size (i.e. number of components, i.e. number of occupied slots) (OPTIONAL)
	// Example: A14INSTANCE_DATA:0:2F32[2]
	// Meaning: Attribute "INSTANCE_DATA" starts at slot 14 and has 2 components (occupies 2 IA slots).
	//          Each component is a 2-component vector of 32-bit floats. Attribute's data is located at the
	//          beginning of its vertex stream (relative offset = 0).
	static const std::string cvReStrAttribute{ R"(A([0-9]+)([A-Z0-9_]+)\:([0-9]+)\:([A-Z0-9]+)(\+([0-9]+))?(\[([0-9])\])?)" };
	static const std::string cvReStrStream{ R"(S([0-9]+)\((V|I)\)<([a-zA-Z0-9_:|\+\[\]]+)?>)" };

	// Stream format: S{1}({2})<{3}>
	// {1} -> Input Assembler slot
	// {2} -> Data rate: "V" means per-vertex, "I" means per-instance.
	// {3} -> List of attributes separated with "|" (OPTIONAL).
	static const std::regex cvRegexAttribute{ cvReStrAttribute };
	static const std::regex cvRegexStream{ cvReStrStream };

	std::vector<VertexAttributeDefinition> VertexFormatDescriptorBuilder::parseVertexFormatString( const std::string_view & pVertexFormatString )
	{
		std::vector<VertexAttributeDefinition> vertexAttribsDefinitions{};

		using StrArray = std::vector<std::string>;
		auto vertexStreamStrings = Cppx::StrUtils::splitStringEx<StrArray>(
				pVertexFormatString, '#', []( auto & pResult, auto * pStr, auto pLength ) {
					pResult.push_back( std::string( pStr, pLength ) );
				} );

		StrArray vsAttributesStrings{};
		for( const auto & vertexStreamStr : vertexStreamStrings )
		{
			std::smatch regexMatch;
			if( std::regex_search( vertexStreamStr, regexMatch, cvRegexStream ) )
			{
				if( regexMatch.size() < 3 )
				{
					continue;
				}

				// [0]: The whole matched expression.
				const auto & debugView = regexMatch[0].str();
				// [1]: Stream slot.
				const auto & streamIASlotStr = regexMatch[1].str();
				// [2]: Stream data rate (V - per-vertex, I - per-instance).
				const auto & streamDataRateStr = regexMatch[2].str();
				// [3]: String with attributes contained within this stream.
				const auto & streamAttributesCombinedStr = regexMatch[3].str();

				if( streamIASlotStr.empty() || streamDataRateStr.empty() || streamAttributesCombinedStr.empty() )
				{
					continue;
				}

				const auto streamIASlot = Cppx::fromStringOrDefault<uint32>( streamIASlotStr, cxGCIVertexStreamSlotUndefined );
				if( !cxGCIValidInputAssemblerSlotRange.contains( streamIASlot ) )
				{
					continue;
				}

				const auto streamDataRate = ( streamDataRateStr == "I" ) ? EVertexDataRate::PerInstance : EVertexDataRate::PerVertex;

				Cppx::StrUtils::splitString( streamAttributesCombinedStr, '|',
				                             [&vsAttributesStrings]( auto * pStr, auto pLength ) {
				                                vsAttributesStrings.push_back( std::string( pStr, pLength ) );
				                             });

				for( const auto & attributeStr : vsAttributesStrings )
				{
					if( std::regex_search( attributeStr, regexMatch, cvRegexAttribute ) )
					{
						const auto regexSize= regexMatch.size();

						// Let's assume the following attribute string: A12INSTANCE_MATRIX:64:3F32+4[4].
						// (Per-instance matrix, located at slot 12 (12 is the base slot, i.e. slot of the first component),
						// consisted of four components of type Vec3f (3F32), each with 4 bytes of added padding)

						// [0]: The whole matched expression.
						const auto & attribDebugView = regexMatch[0].str();
						// [1]: Attribute slot (example: "12").
						const auto & attribIASlotStr = regexMatch[1].str();
						// [2]: The semantic name of the attribute (example: "INSTANCE_MATRIX").
						const auto & attribSemanticsStr = regexMatch[2].str();
						// [3]: Attribute offset relative to the beginning of its vertex stream (example: "64").
						const auto & attribRelativeOffsetStr = regexMatch[3].str();
						// [4]: The type of single component (example: "3F32").
						const auto & attribBaseFormatStr = regexMatch[4].str();

						// [5/Optional]: Per-component padding specification (example: "+4"). Not used in parsing.
						// [6/Optional]: Value of the per-component padding (example: "4").
						const auto & attribComponentPaddingValueStr = regexMatch[6].str();

						// [7/Optional]: Array suffix for multi-slot/-component attribs (example: "[4]"). Not used in parsing.
						// [8/Optional]: Size of the array for multi-slot/-component attributes (example: "4").
						const auto & attribArraySizeStr = regexMatch[8].str();

						if( attribIASlotStr.empty() || attribSemanticsStr.empty() || attribBaseFormatStr.empty() )
						{
							continue;
						}

						const auto attribIASlot = Cppx::fromStringOrDefault<uint32>( attribIASlotStr, cxGCIVertexAttributeSlotUndefined );
						const auto attribRelativeOffset = Cppx::fromStringOrDefault<uint32>( attribIASlotStr, cxGCIVertexAttributeSlotUndefined );
						const auto attribComponentsNum = Cppx::fromStringOrDefault<uint32>( attribArraySizeStr, 1u );
						const auto attribComponentPadding = Cppx::fromStringOrDefault<uint32>( attribComponentPaddingValueStr, 0u );

						if( !GCU::isAttributeLocationAndSizeValid( attribIASlot, attribComponentsNum ) ||
						    ( attribRelativeOffset == cxGCIVertexAttributeRelativeOffsetInvalid ) )
						{
							continue;
						}

						auto & attributeDefinition = vertexAttribsDefinitions.emplace_back();
						attributeDefinition.baseFormat = GCI::EVertexAttribFormat::Unknown;
						attributeDefinition.attributeIASlot = attribIASlot;
						attributeDefinition.semanticComponentsNum = attribComponentsNum;
						attributeDefinition.semanticIndex = 0;
						attributeDefinition.componentPadding = attribComponentPadding;
						attributeDefinition.vertexStreamIASlot = streamIASlot;
						attributeDefinition.vertexStreamRelativeOffset = attribRelativeOffset;
						attributeDefinition.dataRate = streamDataRate;

						if( const auto resolvedSemantics = GCU::resolveShaderSemanticShortName( attribSemanticsStr ) )
						{
							attributeDefinition.shaderSemantics = ShaderSemantics( resolvedSemantics );
						}
						else
						{
							attributeDefinition.shaderSemantics = ShaderSemantics( attribSemanticsStr );
						}
					}
				}

				vsAttributesStrings.clear();
			}
		}

		return vertexAttribsDefinitions;
	}

	bool VertexFormatDescriptorBuilder::adjustAttributeDefinition(
			VertexAttributeDefinition & pAttributeDefinition,
			const VertexAttributeArrayLayout & pAttribArrayLayout,
			const VertexStreamArrayConfig & pStreamArrayConfig ) noexcept
	{
		// This method updates some of the "auto" values that can be specified for attributes with correct ones, based
		// on the current state of the vertex stream configuration. Currently, the following values are supported:
		// - vertexStreamRelativeOffset: possible values are cxIAVertexAttributeOffsetAppend and
		//   cxIAVertexAttributeOffsetAppend16, meaning that such attribute should follow immediately
		//   after the previous data in the vertex stream.

		if( pAttributeDefinition.hasAppendAsRelativeOffset() )
		{
			// Get the vertex stream this attribute is defined for.
			if( const auto * streamComponent =  pStreamArrayConfig.streamPtr( pAttributeDefinition.vertexStreamIASlot ) )
			{
				// If the stream is already defined in the stream array config, replace the "append" offsets with
				// the current data size defined for this stream (potentially aligned, if requested).
				if( pAttributeDefinition.vertexStreamRelativeOffset == GCI::cxIAVertexAttributeOffsetAppend16 )
				{
					pAttributeDefinition.vertexStreamRelativeOffset = Cppx::memGetAlignedValue( streamComponent->elementStrideInBytes, 16 );
				}
				else if( pAttributeDefinition.vertexStreamRelativeOffset == GCI::cxIAVertexAttributeOffsetAppend )
				{
					pAttributeDefinition.vertexStreamRelativeOffset = streamComponent->elementStrideInBytes;
				}
			}
			else
			{
				// If the stream for the specified attribute does not exist yet, it means there will be no data
				// in this stream before that attribute. In this case, offset is replaced with 0.
				pAttributeDefinition.vertexStreamRelativeOffset = 0u;
			}
		}

		return true;
	}

}
