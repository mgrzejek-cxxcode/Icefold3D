
#include "VertexFormatDescriptorUtils.h"
#include <cppx/stringUtils.h>
#include <regex>

namespace Ic3
{

	VertexFormatDescriptorBuilder & VertexFormatDescriptorBuilder::AddAttribute( VertexInputAttributeDefinition pAttributeDefinition )
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

	VertexFormatDescriptorBuilder & VertexFormatDescriptorBuilder::AddAttribute(
			uint32 pStreamSlot,
			VertexAttributeKey pAttributeKey,
			cppx::immutable_string pSemanticName,
			uint32 pDataPadding,
			uint32 pStreamRelativeOffset )
	{
		if( pSemanticName.empty() )
		{
			auto autoSemanticName = GCU::GetStandardSemanticNameFromSystemFlags( pAttributeKey.GetSystemSemanticFlags() );
			if( autoSemanticName.empty() )
			{
				Ic3DebugOutput( "" );
				return *this;
			}

			pSemanticName = autoSemanticName;
		}

		VertexInputAttributeDefinition attributeDefinition;
		attributeDefinition.attributeSlot = pAttributeKey.uBaseSlot;
		attributeDefinition.streamSlot = pStreamSlot;
		attributeDefinition.dataFormat = pAttributeKey.uBaseFormat;
		attributeDefinition.dataPadding = pDataPadding;
		attributeDefinition.semanticGroupSize = pAttributeKey.uSemanticGroupSize;
		attributeDefinition.vertexStreamRelativeOffset = pStreamRelativeOffset;
		attributeDefinition.dataRate = pAttributeKey.GetVertexDataRate();
		attributeDefinition.shaderSemantics.systemSemanticFlags = pAttributeKey.GetSystemSemanticFlags();
		attributeDefinition.shaderSemantics.semanticName = pSemanticName;

		return AddAttribute( std::move( attributeDefinition ) );
	}

	VertexFormatDescriptorBuilder & VertexFormatDescriptorBuilder::AddAttribute(
			uint32 pStreamSlot,
			uint32 pBaseAttributeIASlot,
			ShaderSemantics pSemantics,
			GCI::EVertexAttribFormat pBaseFormat,
			uint32 pDataPadding,
			uint32 pStreamRelativeOffset,
			EVertexDataRate pDataRate )
	{
		VertexInputAttributeDefinition attributeDefinition;
		attributeDefinition.attributeSlot = pBaseAttributeIASlot;
		attributeDefinition.streamSlot = pStreamSlot;
		attributeDefinition.dataFormat = pBaseFormat;
		attributeDefinition.dataPadding = pDataPadding;
		attributeDefinition.semanticGroupSize = 1;
		attributeDefinition.vertexStreamRelativeOffset = pStreamRelativeOffset;
		attributeDefinition.dataRate = pDataRate;
		attributeDefinition.shaderSemantics = std::move( pSemantics );

		return AddAttribute( std::move( attributeDefinition ) );
	}

	VertexFormatDescriptorBuilder & VertexFormatDescriptorBuilder::AddAttributeSemanticGroup(
			uint32 pStreamSlot,
			uint32 pBaseAttributeIASlot,
			ShaderSemantics pSemantics,
			GCI::EVertexAttribFormat pBaseFormat,
			uint32 pSemanticGroupSize,
			uint32 pDataPadding,
			uint32 pStreamRelativeOffset,
			EVertexDataRate pDataRate )
	{
		VertexInputAttributeDefinition attributeDefinition;
		attributeDefinition.attributeSlot = pBaseAttributeIASlot;
		attributeDefinition.streamSlot = pStreamSlot;
		attributeDefinition.dataFormat = pBaseFormat;
		attributeDefinition.dataPadding = pDataPadding;
		attributeDefinition.semanticGroupSize = pSemanticGroupSize;
		attributeDefinition.vertexStreamRelativeOffset = pStreamRelativeOffset;
		attributeDefinition.dataRate = pDataRate;
		attributeDefinition.shaderSemantics = std::move( pSemantics );

		return AddAttribute( std::move( attributeDefinition ) );
	}

	VertexFormatDescriptorBuilder & VertexFormatDescriptorBuilder::AddAttributeList(
			const cppx::array_view<VertexInputAttributeDefinition> & pAttributeList )
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

	VertexFormatDescriptorBuilder & VertexFormatDescriptorBuilder::AddAttributeList(
			const cppx::array_view<const VertexInputAttributeDefinition> & pAttributeList )
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

	TSharedHandle<VertexFormatDescriptor> VertexFormatDescriptorBuilder::CreateVertexFormatDescriptor(
			const GCI::EPrimitiveTopology pPrimitiveTopology,
			const GCI::EIndexDataFormat pIndexDataFormat )
	{
		auto vertexFormatDescriptor = CreateVertexFormatDescriptor(
				pPrimitiveTopology,
				pIndexDataFormat,
				cppx::bind_array_view( _attributeDefs ) );

		_attributeDefs.clear();

		return vertexFormatDescriptor;
	}

	TSharedHandle<VertexFormatDescriptor> VertexFormatDescriptorBuilder::CreateVertexFormatDescriptor(
			const GCI::EPrimitiveTopology pPrimitiveTopology,
			const GCI::EIndexDataFormat pIndexDataFormat,
			const cppx::array_view<VertexInputAttributeDefinition> & pAttributeDefinitions )
	{
		if( ( pIndexDataFormat == GCI::EIndexDataFormat::Undefined ) || ( pPrimitiveTopology == GCI::EPrimitiveTopology::Undefined ) )
		{
			return nullptr;
		}

		auto vertexFormatDescriptor = CreateDynamicObject<VertexFormatDescriptor>();

		auto & inputAttributeArrayLayout = vertexFormatDescriptor->_inputAttributeArrayLayout;
		auto & inputStreamArrayConfig = vertexFormatDescriptor->_inputStreamArrayConfig;

		for( auto & attributeDefinition : pAttributeDefinitions )
		{
			if( !inputStreamArrayConfig.AddActiveStream( attributeDefinition.streamSlot, attributeDefinition.dataRate ) )
			{
				vertexFormatDescriptor.reset();
				break;
			}

			if( !AdjustAttributeDefinition( attributeDefinition, inputAttributeArrayLayout, inputStreamArrayConfig ) )
			{
				vertexFormatDescriptor.reset();
				break;
			}

			const auto streamSlot = attributeDefinition.streamSlot;
			const auto attributeDataRate = attributeDefinition.dataRate;

			auto * baseAttribute = inputAttributeArrayLayout.AddActiveAttribute( attributeDefinition );

			if( !inputStreamArrayConfig.AppendAttribute( streamSlot, *baseAttribute ) )
			{
				vertexFormatDescriptor.reset();
				break;
			}
		}

		if( vertexFormatDescriptor )
		{
			vertexFormatDescriptor->_primitiveTopology = pPrimitiveTopology;
			vertexFormatDescriptor->_indexDataFormat = pIndexDataFormat;
		}

		return vertexFormatDescriptor;
	}

	TSharedHandle<VertexFormatDescriptor> VertexFormatDescriptorBuilder::CreateVertexFormatDescriptor(
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
	static const std::string cvReStrStream{ R"(S([0-9]+)\((V|I)\)\=([0-9]+)<([a-zA-Z0-9_:|\+\[\]]+)?>)" };

	// Stream format: S{1}({2})<{3}>
	// {1} -> Input Assembler slot
	// {2} -> Data rate: "V" means per-vertex, "I" means per-instance.
	// {3} -> List of attributes separated with "|" (OPTIONAL).
	static const std::regex cvRegexAttribute{ cvReStrAttribute };
	static const std::regex cvRegexStream{ cvReStrStream };

	std::vector<VertexInputAttributeDefinition> VertexFormatDescriptorBuilder::ParseVertexFormatString( const std::string_view & pVertexFormatString )
	{
		std::vector<VertexInputAttributeDefinition> vertexAttribsDefinitions{};

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
				const auto & streamSlotStr = regexMatch[1].str();
				// [2]: Stream data rate (V - per-vertex, I - per-instance).
				const auto & streamDataRateStr = regexMatch[2].str();
				// [3]: Stream data stride (in bytes).
				const auto & streamDataStrideStr = regexMatch[3].str();
				// [4]: String with attributes contained within this stream.
				const auto & streamAttributesCombinedStr = regexMatch[4].str();

				if( streamSlotStr.empty() || streamDataRateStr.empty() || streamAttributesCombinedStr.empty() )
				{
					continue;
				}

				const auto streamSlot = cppx::fromStringOrDefault<uint32>( streamSlotStr, kGCIVertexInputStreamSlotUndefined );
				if( !GCI::CXU::IAIsDataStreamVertexBufferIndexValid( streamSlot ) )
				{
					continue;
				}

				const auto streamDataRate = ( streamDataRateStr == "I" ) ? EVertexDataRate::PerInstance : EVertexDataRate::PerVertex;

				cppx::strutil::split_string(
						streamAttributesCombinedStr, '|',
						[&vsAttributesStrings]( auto * pStr, auto pLength ) {
							vsAttributesStrings.push_back( std::string( pStr, pLength ) );
						} );

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

						const auto attribIASlot = cppx::fromStringOrDefault<uint32>( attribIASlotStr, kGCIVertexInputAttributeSlotUndefined );
						const auto attribRelativeOffset = cppx::fromStringOrDefault<uint32>( attribIASlotStr, kGCIVertexInputAttributeSlotUndefined );
						const auto semanticGroupSize = cppx::fromStringOrDefault<uint32>( semanticGroupSizeValueStr, 1u );
						const auto dataPadding = cppx::fromStringOrDefault<uint32>( dataPaddingValueStr, 0u );

						if( !GCU::IsAttributeLocationAndSizeValid( attribIASlot, semanticGroupSize ) ||
						    ( attribRelativeOffset == kGCIVertexAttributeOffsetInvalid ) )
						{
							continue;
						}

						auto & attributeDefinition = vertexAttribsDefinitions.emplace_back();
						attributeDefinition.attributeSlot = attribIASlot;
						attributeDefinition.streamSlot = attribIASlot;
						attributeDefinition.dataFormat = GCU::GetAttributeFormatFromStringIdentifier( attribBaseFormatStr );
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

	bool VertexFormatDescriptorBuilder::ValidateAttributeDefinitions(
			const cppx::array_view<VertexInputAttributeDefinition> & pAttributeDefinitionArray ) noexcept
	{
		bool activeAttributesFlagArray[GCM::kIAMaxVertexAttributesNum]{};
		bool activeStreamsFlagArray[GCM::kIAMaxDataStreamVertexBuffersNum]{};

		uint32 activeAttributesNum = 0;
		uint32 activeStreamsNum = 0;

		for( const auto & attributeDefinition : pAttributeDefinitionArray )
		{
			if( !activeAttributesFlagArray[attributeDefinition.attributeSlot] )
			{
				activeAttributesFlagArray[attributeDefinition.attributeSlot] = true;
				activeAttributesNum += attributeDefinition.semanticGroupSize;
			}

			if( !activeStreamsFlagArray[attributeDefinition.streamSlot] )
			{
				activeStreamsFlagArray[attributeDefinition.streamSlot] = true;
				activeStreamsNum += 1;
			}
		}

		return ( activeAttributesNum > 0 ) && ( activeStreamsNum > 0 );
	}

	bool VertexFormatDescriptorBuilder::AdjustAttributeDefinition(
			VertexInputAttributeDefinition & pAttributeDefinition,
			const VertexInputAttributeArrayConfig & pInputAttributeArrayLayout,
			const VertexInputStreamArrayConfig & pInputStreamArrayConfig ) noexcept
	{
		// This method updates some of the "auto" values that can be specified for attributes with correct ones, based
		// on the current state of the vertex stream configuration. Currently, the following values are supported:
		// - vertexStreamRelativeOffset: possible values are:
		//   > kIAVertexAttributeOffsetAppend, meaning that such attribute should follow immediately
		//     after the previous one in the vertex stream.

		if( pAttributeDefinition.HasAppendAsRelativeOffset() )
		{
			// Get the vertex stream this attribute is defined for.
			if( const auto * vertexInputStream =  pInputStreamArrayConfig.StreamPtr( pAttributeDefinition.streamSlot ) )
			{
				if( !vertexInputStream->IsInitialized() )
				{
					return false;
				}
				
				// If the stream is already defined in the stream array config, replace the "append" offsets with
				// the current data size defined for this stream (potentially aligned, if requested).
				if( pAttributeDefinition.vertexStreamRelativeOffset == GCI::kIAVertexAttributeOffsetAppend )
				{
					pAttributeDefinition.vertexStreamRelativeOffset = vertexInputStream->dataStrideInBytes;
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
