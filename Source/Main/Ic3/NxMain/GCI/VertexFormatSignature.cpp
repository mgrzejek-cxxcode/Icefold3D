
#include "VertexFormatSignature.h"
#include <regex>

namespace Ic3
{

	VertexFormatSignature::VertexFormatSignature()
	: mIndexDataFormat( _indexDataFormat )
	, mPrimitiveTopology( _primitiveTopology )
	, mInputAttributeArrayLayout( _inputAttributeArrayLayout )
	, mInputStreamArrayConfig( _inputStreamArrayConfig )
	{}

	VertexFormatSignature::VertexFormatSignature( VertexFormatSignature && pSource ) noexcept
	: mIndexDataFormat( _indexDataFormat )
	, mPrimitiveTopology( _primitiveTopology )
	, mInputAttributeArrayLayout( _inputAttributeArrayLayout )
	, mInputStreamArrayConfig( _inputStreamArrayConfig )
	{}

	VertexFormatSignature & VertexFormatSignature::operator=( VertexFormatSignature && pRhs ) noexcept
	{
		_indexDataFormat = pRhs._indexDataFormat;
		_primitiveTopology = pRhs._primitiveTopology;
		_inputAttributeArrayLayout = std::move( pRhs._inputAttributeArrayLayout );
		_inputStreamArrayConfig = std::move( pRhs._inputStreamArrayConfig );
		return *this;
	}

	VertexFormatSignature::VertexFormatSignature( const VertexFormatSignature & pSource )
	: mIndexDataFormat( _indexDataFormat )
	, mPrimitiveTopology( _primitiveTopology )
	, mInputAttributeArrayLayout( _inputAttributeArrayLayout )
	, mInputStreamArrayConfig( _inputStreamArrayConfig )
	, _indexDataFormat( pSource._indexDataFormat )
	, _primitiveTopology( pSource._primitiveTopology )
	, _inputAttributeArrayLayout( pSource._inputAttributeArrayLayout )
	, _inputStreamArrayConfig( pSource._inputStreamArrayConfig )
	{}

	VertexFormatSignature & VertexFormatSignature::operator=( const VertexFormatSignature & pRhs )
	{
		_indexDataFormat = pRhs._indexDataFormat;
		_primitiveTopology = pRhs._primitiveTopology;
		_inputAttributeArrayLayout = pRhs._inputAttributeArrayLayout;
		_inputStreamArrayConfig = pRhs._inputStreamArrayConfig;
		return *this;
	}

	VertexFormatSignature::~VertexFormatSignature() = default;

	bool VertexFormatSignature::Equals( const VertexFormatSignature & pOther ) const noexcept
	{
		for( uint32 nAttribute = 0; nAttribute < GCM::kIAMaxVertexAttributesNum; ++nAttribute )
		{
			const auto & attribute = _inputAttributeArrayLayout[nAttribute];
			const auto & otherAttribute = pOther._inputAttributeArrayLayout[nAttribute];

			if( !attribute.IsSameAs( otherAttribute ) )
			{
				return false;
			}
		}

		return true;
	}

	uint32 VertexFormatSignature::GetElementStrideForAttribute( native_uint pAttributeSlot ) const noexcept
	{
		const auto * genericAttribute = GetAttribute( pAttributeSlot );
		if( genericAttribute && genericAttribute->IsActive() )
		{
			const auto * attributeStream = GetStream( genericAttribute->vertexStreamSlot );
			if( attributeStream && attributeStream->IsActive() )
			{
				return attributeStream->dataStrideInBytes;
			}
		}

		return 0;
	}

	uint32 VertexFormatSignature::GetElementStrideForAttribute( cppx::string_view pSemanticName ) const noexcept
	{
		const auto attributeIndex = _ResolveAttributeRefImpl( pSemanticName, 0u );
		return GetElementStrideForAttribute( attributeIndex );
	}

	uint32 VertexFormatSignature::GetElementStrideForAttribute( VertexAttributeKey pAttributeKey ) const noexcept
	{
		const auto attributeIndex = _ResolveAttributeRefImpl( pAttributeKey.GetSemanticFlags(), 0u );
		return GetElementStrideForAttribute( attributeIndex );
	}

	uint32 VertexFormatSignature::GetElementStrideForAttribute( cppx::bitmask<EVertexAttributeSemanticFlags> pAttributeSemanticFlags ) const noexcept
	{
		const auto attributeIndex = _ResolveAttributeRefImpl( pAttributeSemanticFlags, 0u );
		return GetElementStrideForAttribute( attributeIndex );
	}

	uint32 VertexFormatSignature::GetElementStrideForAttributeUnchecked( native_uint pAttributeSlot ) const noexcept
	{
		const auto & genericAttribute = _inputAttributeArrayLayout[pAttributeSlot];
		const auto & attributeStream = _inputStreamArrayConfig[genericAttribute.vertexStreamSlot];
		return attributeStream.dataStrideInBytes;
	}

	uint32 VertexFormatSignature::GetElementStrideForAttributeUnchecked( VertexAttributeKey pAttributeKey ) const noexcept
	{
		const auto & genericAttribute = _inputAttributeArrayLayout[pAttributeKey.GetBaseSlot()];
		const auto & attributeStream = _inputStreamArrayConfig[genericAttribute.vertexStreamSlot];
		return attributeStream.dataStrideInBytes;
	}

	uint32 VertexFormatSignature::ResolveAttributeRef( const VertexAttributeShaderSemantics & pSemantics ) const noexcept
	{
		const auto semanticName = pSemantics.IsEmpty() ? pSemantics.semanticName.str_view() : cppx::string_view{};
		return _ResolveAttributeRefImpl( semanticName, pSemantics.semanticIndex );
	}

	uint32 VertexFormatSignature::ResolveAttributeRef( cppx::string_view pSemanticName, uint32 pSemanticIndex ) const noexcept
	{
		return _ResolveAttributeRefImpl( pSemanticName, pSemanticIndex );
	}

	uint32 VertexFormatSignature::ResolveAttributeRef( VertexAttributeKey pAttributeKey, uint32 pSemanticIndex ) const noexcept
	{
		const auto sysSmtFlags = pAttributeKey.GetSemanticFlags();
		return _ResolveAttributeRefImpl( sysSmtFlags, pSemanticIndex );
	}

	uint32 VertexFormatSignature::ResolveAttributeRef( cppx::bitmask<EVertexAttributeSemanticFlags> pAttributeSemanticFlags, uint32 pSemanticIndex ) const noexcept
	{
		return _ResolveAttributeRefImpl( pAttributeSemanticFlags, pSemanticIndex );
	}

	std::string VertexFormatSignature::GenerateSerialString() const noexcept
	{
		return GCIUtils::GenerateVertexFormatSignatureSerialString( _inputAttributeArrayLayout, _inputStreamArrayConfig );
	}

	std::vector<VertexInputAttributeDefinition> VertexFormatSignature::GenerateAttributeDefinitionArray() const noexcept
	{
		std::vector<VertexInputAttributeDefinition> resultDefinitionArray;

		if( !_inputAttributeArrayLayout.IsEmpty() )
		{
			const auto & attributeArray = _inputAttributeArrayLayout.GetAttributeArray();
			resultDefinitionArray.reserve( _inputAttributeArrayLayout.GetActiveBaseAttributesNum() );

			for( const auto & attribute : attributeArray )
			{
				if( attribute.IsBaseAttribute() )
				{
					auto & attributeDefinition = resultDefinitionArray.emplace_back();
					attributeDefinition.attributeSlot = attribute.attributeSlot;
					attributeDefinition.semanticGroupSize = attribute.semanticGroupSize;
					attributeDefinition.baseDataFormat = attribute.dataFormat;
					attributeDefinition.dataPadding = attribute.dataPadding;
					attributeDefinition.semanticName = attribute.semanticName;
					attributeDefinition.semanticFlags = attribute.semanticFlags;
					attributeDefinition.vertexStreamSlot = attribute.vertexStreamSlot;
					attributeDefinition.vertexStreamRelativeOffset = attribute.vertexStreamRelativeOffset;

					const auto & attributeStream = _inputStreamArrayConfig[attribute.vertexStreamSlot];
					attributeDefinition.dataRate = attributeStream.streamDataRate;
				}
			}
		}

		return resultDefinitionArray;
	}

	uint32 VertexFormatSignature::_ResolveAttributeRefImpl( cppx::string_view pSemanticName, uint32 pSemanticIndex ) const noexcept
	{
		if( !pSemanticName.empty() && ( pSemanticIndex < GCM::kIAMaxVertexAttributeSemanticGroupSize ) )
		{
			// Query the internal AttributeLayout object for the index of a base attribute with the specified semantics.
			// If such attribute does not exist, an invalid index is returned instead (cxGCIVertexAttributeSlotUndefined).
			const auto baseAttributeSlot = _inputAttributeArrayLayout.QueryBaseAttributeBySemantics( pSemanticName );

			if( baseAttributeSlot != GCI::kIAVertexAttributeSlotUndefined )
			{
				const auto & baseAttribute = _inputAttributeArrayLayout[baseAttributeSlot];

				// Attributes can be part of semantic groups, i.e. related attributes with the same semantic name,
				// occupying adjacent range of IA slots. Calculate the slot index of the requested attribute.
				const auto requestedAttributeIASlot = baseAttributeSlot + pSemanticIndex;

				// Fetch the requested attribute. In case it is not a valid attribute, a nullptr is returned here.
				if( const auto * requestedAttributeComponent = _inputAttributeArrayLayout.AttributePtr( requestedAttributeIASlot ) )
				{
					if( requestedAttributeComponent->HasSameSemanticsAs( baseAttribute ) )
					{
						Ic3DebugAssert( requestedAttributeComponent->semanticIndex == pSemanticIndex );
						return requestedAttributeIASlot;
					}
				}
			}
		}

		return GCI::kIAVertexAttributeSlotUndefined;
	}

	uint32 VertexFormatSignature::_ResolveAttributeRefImpl( cppx::bitmask<EVertexAttributeSemanticFlags> pAttributeSemanticFlags, uint32 pSemanticIndex ) const noexcept
	{
		if( GCIUtils::IsStandardSystemAttribute( pAttributeSemanticFlags ) )
		{
			const auto semanticName = GCIUtils::GetSemanticNameFromAttributeFlags( pAttributeSemanticFlags );
			return _ResolveAttributeRefImpl( semanticName, pSemanticIndex );
		}
		return GCI::kIAVertexAttributeSlotUndefined;
	}


	namespace GCIUtils
	{

		std::string GenerateVertexFormatSignatureSerialString(
				const VertexInputAttributeArrayConfig & pAttributeArrayConfig,
				const VertexInputStreamArrayConfig & pStreamArrayConfig )
		{
			std::string vertexStreamStrings[GCM::kIAMaxDataStreamVertexBuffersNum];

			const auto & attributeArray = pAttributeArrayConfig.GetAttributeArray();
			for( const auto & genericAttribute : attributeArray )
			{
				if( genericAttribute.IsActive() )
				{
					if( genericAttribute.IsBaseAttribute() )
					{
						const auto attributeFormatStr = GenerateVertexAttributeFormatString( genericAttribute );
						if( !vertexStreamStrings[genericAttribute.vertexStreamSlot].empty() )
						{
							vertexStreamStrings[genericAttribute.vertexStreamSlot].append( 1, '|' );
						}
						vertexStreamStrings[genericAttribute.vertexStreamSlot].append( attributeFormatStr );
					}
				}
			}

			std::string resultStringID;

			const auto & streamArray = pStreamArrayConfig.GetStreamArray();
			for( const auto & vertexStream : streamArray )
			{
				if( vertexStream.IsActive() )
				{
					if( !vertexStreamStrings[vertexStream.streamSlot].empty() )
					{
						resultStringID.append( 1, '#' );
						resultStringID.append( 1, 'S' );
						resultStringID.append( std::to_string( vertexStream.streamSlot ) );
						resultStringID.append( 1, '(' );
						resultStringID.append( 1, ( vertexStream.streamDataRate == GCI::EIAVertexAttributeDataRate::PerInstance ) ? 'I' : 'V' );
						resultStringID.append( 1, ')' );
						resultStringID.append( 1, '=' );
						resultStringID.append( std::to_string( vertexStream.dataStrideInBytes ) );
						resultStringID.append( 1, '<' );
						resultStringID.append( vertexStreamStrings[vertexStream.streamSlot] );
						resultStringID.append( 1, '>' );
					}
				}
			}

			return resultStringID;
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
		#define RE_STR_VFS_ATTRIBUTE R"(A([0-9]+)([A-Z][A-Za-z0-9_]+)\:([0-9]+)\:([A-Z0-9]+)(\+([0-9]+))?(\[([0-9])\])?)"

		// Stream format: S{1}({2})<{3}>
		// {1} -> Input Assembler slot
		// {2} -> Data rate: "V" means per-vertex, "I" means per-instance.
		// {3} -> List of attributes separated with "|" (OPTIONAL).
		#define RE_STR_VFS_STREAM R"(S([0-9]+)\((V|I)\)\=([0-9]+)<([a-zA-Z0-9_:|\+\[\]]+)?>)"

		static const std::regex cvReVfsAttribute{ RE_STR_VFS_ATTRIBUTE };
		static const std::regex cvReVfsStream{ RE_STR_VFS_STREAM };

		std::vector<VertexInputAttributeDefinition> ParseVertexFormatSignatureSerialString(
				const std::string_view & pVertexFormatString )
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
				if( std::regex_search( vertexStreamStr, regexMatch, cvReVfsStream ) )
				{
					if( regexMatch.size() < 3 )
					{
						continue;
					}

					// [0]: The whole matched expression.
					const auto & debugView = regexMatch[0].str();
					// [1]: Stream slot.
					const auto & streamIndexStr = regexMatch[1].str();
					// [2]: Stream data rate (V - per-vertex, I - per-instance).
					const auto & streamDataRateStr = regexMatch[2].str();
					// [3]: Stream data stride (in bytes).
					const auto & streamDataStrideStr = regexMatch[3].str();
					// [4]: String with attributes contained within this stream.
					const auto & streamAttributesCombinedStr = regexMatch[4].str();

					if( streamIndexStr.empty() || streamDataRateStr.empty() || streamAttributesCombinedStr.empty() )
					{
						continue;
					}

					const auto streamIndex = cppx::from_string_or_default<uint8>( streamIndexStr, GCI::kIAVertexStreamSlotUndefined );
					if( !IAIsDataStreamVertexBufferSlotValid( streamIndex ) )
					{
						continue;
					}

					const auto streamDataRate = ( streamDataRateStr == "I" ) ? GCI::EIAVertexAttributeDataRate::PerInstance : GCI::EIAVertexAttributeDataRate::PerVertex;

					cppx::strutil::split_string(
							streamAttributesCombinedStr, '|',
							[&vsAttributesStrings]( auto * pStr, auto pLength ) {
								vsAttributesStrings.push_back( std::string( pStr, pLength ) );
							} );

					for( const auto & attributeStr : vsAttributesStrings )
					{
						if( std::regex_search( attributeStr, regexMatch, cvReVfsAttribute ) )
						{
							const auto regexSize= regexMatch.size();

							// Let's assume the following attribute string: A12INSTANCE_MATRIX:64:3F32+4[4].
							// (Per-instance matrix, located at slot 12 (12 is the base slot, i.e. slot of the first component),
							// consisted of four components of type Vec3f (3F32), each with 4 bytes of added padding)

							// [0]: The whole matched expression.
							const auto & attribDebugView = regexMatch[0].str();
							// [1]: Attribute slot (example: "12").
							const auto & attributeSlotStr = regexMatch[1].str();
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

							if( attributeSlotStr.empty() || attribSemanticsStr.empty() || attribBaseFormatStr.empty() )
							{
								continue;
							}

							const auto attributeSlot =
								cppx::from_string_or_default<uint8>( attributeSlotStr, GCI::kIAVertexAttributeSlotUndefined );
							
							const auto attribRelativeOffset =
								cppx::from_string_or_default<uint32>( attribRelativeOffsetStr, GCI::kIAVertexAttributeOffsetInvalid );
							
							const auto semanticGroupSize =
								cppx::from_string_or_default<uint8>( semanticGroupSizeValueStr, 1u );
							
							const auto dataPadding =
								cppx::from_string_or_default<uint8>( dataPaddingValueStr, 0u );

							if( !IsAttributeLocationAndSizeValid( attributeSlot, semanticGroupSize ) || ( attribRelativeOffset == GCI::kIAVertexAttributeOffsetInvalid ) )
							{
								continue;
							}

							auto & attributeDefinition = vertexAttribsDefinitions.emplace_back();
							attributeDefinition.attributeSlot = attributeSlot;
							attributeDefinition.vertexStreamSlot = streamIndex;
							attributeDefinition.baseDataFormat = GetAttributeFormatFromStringIdentifier( attribBaseFormatStr );
							attributeDefinition.semanticGroupSize = semanticGroupSize;
							attributeDefinition.dataPadding = dataPadding;
							attributeDefinition.vertexStreamRelativeOffset = attribRelativeOffset;
							attributeDefinition.dataRate = streamDataRate;

							if( const auto resolvedSemanticName = ResolveShaderSemanticShortName( attribSemanticsStr ) )
							{
								attributeDefinition.semanticName = resolvedSemanticName;
							}
							else
							{
								attributeDefinition.semanticName = attribSemanticsStr;
							}

							attributeDefinition.semanticFlags = GetSemanticFlagsFromAttributeName( attributeDefinition.semanticName );
						}
					}

					vsAttributesStrings.clear();
				}
			}

			return vertexAttribsDefinitions;
		}

	}

}
