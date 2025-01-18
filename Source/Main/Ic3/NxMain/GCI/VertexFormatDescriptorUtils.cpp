
#include "VertexFormatDescriptorUtils.h"
#include <cppx/stringUtils.h>
#include <regex>

namespace Ic3
{

	VertexFormatDescriptorBuilder & VertexFormatDescriptorBuilder::addAttribute( VertexAttributeDefinition pAttributeDefinition )
	{
		if( !pAttributeDefinition.IsValid() )
		{
			Ic3DebugOutput( "" );
		}
		else
		{
			_attributeDefs.push_back( std::move( pAttributeDefinition ) );
		}

		return *this;
	}

	VertexFormatDescriptorBuilder & VertexFormatDescriptorBuilder::addAttribute(
			uint32 pStreamIASlot,
			VertexAttributeKey pAttributeKey,
			cppx::ImmutableString pSemanticName,
			uint32 pDataPadding,
			uint32 pStreamRelativeOffset )
	{
		if( pSemanticName.empty() )
		{
			auto autoSemanticName = GCU::getStandardSemanticNameFromSystemFlags( pAttributeKey.getSystemSemanticFlags() );
			if( autoSemanticName.empty() )
			{
				Ic3DebugOutput( "" );
				return *this;
			}

			pSemanticName = autoSemanticName;
		}

		VertexAttributeDefinition attributeDefinition;
		attributeDefinition.attributeIASlot = pAttributeKey.uBaseSlot;
		attributeDefinition.streamIASlot = pStreamIASlot;
		attributeDefinition.dataFormat = pAttributeKey.uBaseFormat;
		attributeDefinition.dataPadding = pDataPadding;
		attributeDefinition.semanticGroupSize = pAttributeKey.uSemanticGroupSize;
		attributeDefinition.vertexStreamRelativeOffset = pStreamRelativeOffset;
		attributeDefinition.dataRate = pAttributeKey.getVertexDataRate();
		attributeDefinition.shaderSemantics.systemSemanticFlags = pAttributeKey.getSystemSemanticFlags();
		attributeDefinition.shaderSemantics.semanticName = pSemanticName;

		return addAttribute( std::move( attributeDefinition ) );
	}

	VertexFormatDescriptorBuilder & VertexFormatDescriptorBuilder::addAttribute(
			uint32 pStreamIASlot,
			uint32 pBaseAttributeIASlot,
			ShaderSemantics pSemantics,
			GCI::EVertexAttribFormat pBaseFormat,
			uint32 pDataPadding,
			uint32 pStreamRelativeOffset,
			EVertexDataRate pDataRate )
	{
		VertexAttributeDefinition attributeDefinition;
		attributeDefinition.attributeIASlot = pBaseAttributeIASlot;
		attributeDefinition.streamIASlot = pStreamIASlot;
		attributeDefinition.dataFormat = pBaseFormat;
		attributeDefinition.dataPadding = pDataPadding;
		attributeDefinition.semanticGroupSize = 1;
		attributeDefinition.vertexStreamRelativeOffset = pStreamRelativeOffset;
		attributeDefinition.dataRate = pDataRate;
		attributeDefinition.shaderSemantics = std::move( pSemantics );

		return addAttribute( std::move( attributeDefinition ) );
	}

	VertexFormatDescriptorBuilder & VertexFormatDescriptorBuilder::addAttributeSemanticGroup(
			uint32 pStreamIASlot,
			uint32 pBaseAttributeIASlot,
			ShaderSemantics pSemantics,
			GCI::EVertexAttribFormat pBaseFormat,
			uint32 pSemanticGroupSize,
			uint32 pDataPadding,
			uint32 pStreamRelativeOffset,
			EVertexDataRate pDataRate )
	{
		VertexAttributeDefinition attributeDefinition;
		attributeDefinition.attributeIASlot = pBaseAttributeIASlot;
		attributeDefinition.streamIASlot = pStreamIASlot;
		attributeDefinition.dataFormat = pBaseFormat;
		attributeDefinition.dataPadding = pDataPadding;
		attributeDefinition.semanticGroupSize = pSemanticGroupSize;
		attributeDefinition.vertexStreamRelativeOffset = pStreamRelativeOffset;
		attributeDefinition.dataRate = pDataRate;
		attributeDefinition.shaderSemantics = std::move( pSemantics );

		return addAttribute( std::move( attributeDefinition ) );
	}

	VertexFormatDescriptorBuilder & VertexFormatDescriptorBuilder::addAttributeList(
			const cppx::array_view<VertexAttributeDefinition> & pAttributeList )
	{
		for( auto & attributeDefinition : pAttributeList )
		{
			if( attributeDefinition.IsValid() )
			{
				_attributeDefs.push_back( std::move( attributeDefinition ) );
			}
		}

		return *this;
	}

	VertexFormatDescriptorBuilder & VertexFormatDescriptorBuilder::addAttributeList(
			const cppx::array_view<const VertexAttributeDefinition> & pAttributeList )
	{
		for( const auto & attributeDefinition : pAttributeList )
		{
			if( attributeDefinition.IsValid() )
			{
				_attributeDefs.push_back( attributeDefinition );
			}
		}

		return *this;
	}

	TSharedHandle<VertexFormatDescriptor> VertexFormatDescriptorBuilder::createVertexFormatDescriptor(
			const GCI::EPrimitiveTopology pPrimitiveTopology,
			const GCI::EIndexDataFormat pIndexDataFormat )
	{
		auto vertexFormatDescriptor = createVertexFormatDescriptor(
				pPrimitiveTopology,
				pIndexDataFormat,
				cppx::bind_array_view( _attributeDefs ) );

		_attributeDefs.clear();

		return vertexFormatDescriptor;
	}

	TSharedHandle<VertexFormatDescriptor> VertexFormatDescriptorBuilder::createVertexFormatDescriptor(
			const GCI::EPrimitiveTopology pPrimitiveTopology,
			const GCI::EIndexDataFormat pIndexDataFormat,
			const cppx::array_view<VertexAttributeDefinition> & pAttributeDefinitions )
	{
		if( ( pIndexDataFormat == GCI::EIndexDataFormat::Undefined ) || ( pPrimitiveTopology == GCI::EPrimitiveTopology::Undefined ) )
		{
			return nullptr;
		}

		auto vertexFormatDescriptor = CreateDynamicObject<VertexFormatDescriptor>();

		auto & attribArrayLayout = vertexFormatDescriptor->_attribArrayLayout;
		auto & streamArrayConfig = vertexFormatDescriptor->_streamArrayConfig;

		for( auto & attributeDefinition : pAttributeDefinitions )
		{
			if( !adjustAttributeDefinition( attributeDefinition, attribArrayLayout, streamArrayConfig ) )
			{
				vertexFormatDescriptor.reset();
				break;
			}

			const auto streamIASlot = attributeDefinition.streamIASlot;
			const auto attributeDataRate = attributeDefinition.dataRate;

			auto * baseAttributeComponent = attribArrayLayout.AddActiveAttribute( std::move( attributeDefinition ) );

			streamArrayConfig.AppendAttributeAuto( streamIASlot, attributeDataRate, *baseAttributeComponent );
		}

		if( vertexFormatDescriptor )
		{
			vertexFormatDescriptor->_primitiveTopology = pPrimitiveTopology;
			vertexFormatDescriptor->_indexDataFormat = pIndexDataFormat;
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
	static const std::string cvReStrAttribute{ R"(A([0-9]+)([A-Z][A-Za-z0-9_]+)\:([0-9]+)\:([A-Z0-9]+)(\+([0-9]+))?(\[([0-9])\])?)" };
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
		auto vertexStreamStrings = cppx::strutil::split_string_ex<StrArray>(
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
				const auto & StreamAttributesCombinedStr = regexMatch[3].str();

				if( streamIASlotStr.empty() || streamDataRateStr.empty() || StreamAttributesCombinedStr.empty() )
				{
					continue;
				}

				const auto streamIASlot = cppx::fromStringOrDefault<uint32>( streamIASlotStr, cxGCIVertexStreamIndexUndefined );
				if( !cxGCIValidInputAssemblerSlotIndexRange.contains( streamIASlot ) )
				{
					continue;
				}

				const auto streamDataRate = ( streamDataRateStr == "I" ) ? EVertexDataRate::PerInstance : EVertexDataRate::PerVertex;

				cppx::strutil::split_string( StreamAttributesCombinedStr, '|',
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
						const auto & dataPaddingValueStr = regexMatch[6].str();

						// [7/Optional]: Array suffix for multi-slot/-component attribs (example: "[4]"). Not used in parsing.
						// [8/Optional]: Size of the array for multi-slot/-component attributes (example: "4").
						const auto & semanticGroupSizeValueStr = regexMatch[8].str();

						if( attribIASlotStr.empty() || attribSemanticsStr.empty() || attribBaseFormatStr.empty() )
						{
							continue;
						}

						const auto attribIASlot = cppx::fromStringOrDefault<uint32>( attribIASlotStr, cxGCIVertexAttributeIndexUndefined );
						const auto attribRelativeOffset = cppx::fromStringOrDefault<uint32>( attribIASlotStr, cxGCIVertexAttributeIndexUndefined );
						const auto semanticGroupSize = cppx::fromStringOrDefault<uint32>( semanticGroupSizeValueStr, 1u );
						const auto dataPadding = cppx::fromStringOrDefault<uint32>( dataPaddingValueStr, 0u );

						if( !GCU::IsAttributeLocationAndSizeValid( attribIASlot, semanticGroupSize ) ||
						    ( attribRelativeOffset == cxGCIVertexAttributeOffsetInvalid ) )
						{
							continue;
						}

						auto & attributeDefinition = vertexAttribsDefinitions.emplace_back();
						attributeDefinition.attributeIASlot = attribIASlot;
						attributeDefinition.streamIASlot = attribIASlot;
						attributeDefinition.dataFormat = GCI::EVertexAttribFormat::Undefined;
						attributeDefinition.semanticGroupSize = semanticGroupSize;
						attributeDefinition.dataPadding = dataPadding;
						attributeDefinition.vertexStreamRelativeOffset = attribRelativeOffset;
						attributeDefinition.dataRate = streamDataRate;

						if( const auto resolvedSemantics = GCU::ResolveShaderSemanticShortName( attribSemanticsStr ) )
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

	bool VertexFormatDescriptorBuilder::validateAttributeDefinitions(
			const cppx::array_view<VertexAttributeDefinition> & pAttributeDefinitionArray ) noexcept
	{
		bool activeAttributesFlagArray[GCM::cxIAMaxVertexAttributesNum]{};
		bool activeStreamsFlagArray[GCM::cxIAMaxVertexStreamsNum]{};

		uint32 activeAttributesNum = 0;
		uint32 activeStreamsNum = 0;

		for( auto & attributeDefinition : pAttributeDefinitionArray )
		{
			if( !activeAttributesFlagArray[attributeDefinition.attributeIASlot] )
			{
				activeAttributesFlagArray[attributeDefinition.attributeIASlot] = true;
				activeAttributesNum += attributeDefinition.semanticGroupSize;
			}

			if( !activeStreamsFlagArray[attributeDefinition.streamIASlot] )
			{
				activeStreamsFlagArray[attributeDefinition.streamIASlot] = true;
				activeStreamsNum += 1;
			}
		}

		return ( activeAttributesNum > 0 ) && ( activeStreamsNum > 0 );
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

		if( pAttributeDefinition.HasAppendAsRelativeOffset() )
		{
			// Get the vertex stream this attribute is defined for.
			if( const auto * streamComponent =  pStreamArrayConfig.StreamPtr( pAttributeDefinition.streamIASlot ) )
			{
				// If the stream is already defined in the stream array config, replace the "append" offsets with
				// the current data size defined for this stream (potentially aligned, if requested).
				if( pAttributeDefinition.vertexStreamRelativeOffset == GCI::cxIAVertexAttributeOffsetAppend16 )
				{
					pAttributeDefinition.vertexStreamRelativeOffset = cppx::mem_get_aligned_value( streamComponent->dataStrideInBytes, 16 );
				}
				else if( pAttributeDefinition.vertexStreamRelativeOffset == GCI::cxIAVertexAttributeOffsetAppend )
				{
					pAttributeDefinition.vertexStreamRelativeOffset = streamComponent->dataStrideInBytes;
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
