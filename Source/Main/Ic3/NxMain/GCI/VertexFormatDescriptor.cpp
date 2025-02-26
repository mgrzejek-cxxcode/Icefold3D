
#include "VertexFormatDescriptor.h"

namespace Ic3
{

	VertexFormatDescriptor::VertexFormatDescriptor()
	: mIndexDataFormat( _indexDataFormat )
	, mPrimitiveTopology( _primitiveTopology )
	, mInputAttributeArrayLayout( _inputAttributeArrayLayout )
	, mInputStreamArrayConfig( _inputStreamArrayConfig )
	{}

	VertexFormatDescriptor::VertexFormatDescriptor( VertexFormatDescriptor && pSource ) noexcept
	: mIndexDataFormat( _indexDataFormat )
	, mPrimitiveTopology( _primitiveTopology )
	, mInputAttributeArrayLayout( _inputAttributeArrayLayout )
	, mInputStreamArrayConfig( _inputStreamArrayConfig )
	{}

	VertexFormatDescriptor & VertexFormatDescriptor::operator=( VertexFormatDescriptor && pRhs ) noexcept
	{
		_indexDataFormat = pRhs._indexDataFormat;
		_primitiveTopology = pRhs._primitiveTopology;
		_inputAttributeArrayLayout = std::move( pRhs._inputAttributeArrayLayout );
		_inputStreamArrayConfig = std::move( pRhs._inputStreamArrayConfig );
		return *this;
	}

	VertexFormatDescriptor::VertexFormatDescriptor( const VertexFormatDescriptor & pSource )
	: mIndexDataFormat( _indexDataFormat )
	, mPrimitiveTopology( _primitiveTopology )
	, mInputAttributeArrayLayout( _inputAttributeArrayLayout )
	, mInputStreamArrayConfig( _inputStreamArrayConfig )
	, _indexDataFormat( pSource._indexDataFormat )
	, _primitiveTopology( pSource._primitiveTopology )
	, _inputAttributeArrayLayout( pSource._inputAttributeArrayLayout )
	, _inputStreamArrayConfig( pSource._inputStreamArrayConfig )
	{}

	VertexFormatDescriptor & VertexFormatDescriptor::operator=( const VertexFormatDescriptor & pRhs )
	{
		_indexDataFormat = pRhs._indexDataFormat;
		_primitiveTopology = pRhs._primitiveTopology;
		_inputAttributeArrayLayout = pRhs._inputAttributeArrayLayout;
		_inputStreamArrayConfig = pRhs._inputStreamArrayConfig;
		return *this;
	}

	VertexFormatDescriptor::~VertexFormatDescriptor() = default;

	bool VertexFormatDescriptor::Equals( const VertexFormatDescriptor & pOther ) const noexcept
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

	uint32 VertexFormatDescriptor::GetElementStrideForAttribute( native_uint pAttributeSlot ) const noexcept
	{
		const auto * genericAttribute = GetAttribute( pAttributeSlot );
		if( genericAttribute && genericAttribute->IsActive() )
		{
			const auto * attributeStream = GetStream( genericAttribute->streamSlot );
			if( attributeStream && attributeStream->IsActive() )
			{
				return attributeStream->dataStrideInBytes;
			}
		}

		return 0;
	}

	uint32 VertexFormatDescriptor::GetElementStrideForAttribute( cppx::string_view pSemanticName ) const noexcept
	{
		const auto attributeSlot = _ResolveAttributeRefImpl( pSemanticName, 0u );
		return GetElementStrideForAttribute( attributeSlot );
	}

	uint32 VertexFormatDescriptor::GetElementStrideForAttribute( VertexAttributeKey pAttributeKey ) const noexcept
	{
		const auto attributeSlot = _ResolveAttributeRefImpl( pAttributeKey.GetSystemSemanticFlags(), 0u );
		return GetElementStrideForAttribute( attributeSlot );
	}

	uint32 VertexFormatDescriptor::GetElementStrideForAttribute( cppx::bitmask<ESystemAttributeSemanticFlags> pSystemSemanticFlags ) const noexcept
	{
		const auto attributeSlot = _ResolveAttributeRefImpl( pSystemSemanticFlags, 0u );
		return GetElementStrideForAttribute( attributeSlot );
	}

	uint32 VertexFormatDescriptor::GetElementStrideForAttributeUnchecked( native_uint pAttributeSlot ) const noexcept
	{
		const auto & genericAttribute = _inputAttributeArrayLayout[pAttributeSlot];
		const auto & attributeStream = _inputStreamArrayConfig[genericAttribute.streamSlot];
		return attributeStream.dataStrideInBytes;
	}

	uint32 VertexFormatDescriptor::GetElementStrideForAttributeUnchecked( VertexAttributeKey pAttributeKey ) const noexcept
	{
		const auto & genericAttribute = _inputAttributeArrayLayout[pAttributeKey.uBaseSlot];
		const auto & attributeStream = _inputStreamArrayConfig[genericAttribute.streamSlot];
		return attributeStream.dataStrideInBytes;
	}

	uint32 VertexFormatDescriptor::ResolveAttributeRef( const ShaderSemantics & pSemantics ) const noexcept
	{
		const auto semanticName = pSemantics.IsEmpty() ? pSemantics.semanticName.strView() : cppx::string_view{};
		return _ResolveAttributeRefImpl( semanticName, pSemantics.semanticIndex );
	}

	uint32 VertexFormatDescriptor::ResolveAttributeRef( cppx::string_view pSemanticName, uint32 pSemanticIndex ) const noexcept
	{
		return _ResolveAttributeRefImpl( pSemanticName, pSemanticIndex );
	}

	uint32 VertexFormatDescriptor::ResolveAttributeRef( VertexAttributeKey pAttributeKey, uint32 pSemanticIndex ) const noexcept
	{
		const auto sysSmtFlags = pAttributeKey.GetSystemSemanticFlags();
		return _ResolveAttributeRefImpl( sysSmtFlags, pSemanticIndex );
	}

	uint32 VertexFormatDescriptor::ResolveAttributeRef( cppx::bitmask<ESystemAttributeSemanticFlags> pSystemSemanticFlags, uint32 pSemanticIndex ) const noexcept
	{
		return _ResolveAttributeRefImpl( pSystemSemanticFlags, pSemanticIndex );
	}

	std::string VertexFormatDescriptor::GenerateVertexFormatStringID() const noexcept
	{
		return GCU::GenerateVertexFormatLayoutString( _inputAttributeArrayLayout, _inputStreamArrayConfig );
	}

	std::vector<VertexInputAttributeDefinition> VertexFormatDescriptor::GenerateAttributeDefinitionArray() const noexcept
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
					attributeDefinition.streamSlot = attribute.streamSlot;
					attributeDefinition.dataFormat = attribute.dataFormat;
					attributeDefinition.dataPadding = attribute.dataPadding;
					attributeDefinition.semanticGroupSize = attribute.semanticGroupSize;
					attributeDefinition.vertexStreamRelativeOffset = attribute.vertexStreamRelativeOffset;
					attributeDefinition.shaderSemantics = attribute.shaderSemantics;

					const auto & attributeStream = _inputStreamArrayConfig[attribute.streamSlot];
					attributeDefinition.dataRate = attributeStream.streamDataRate;
				}
			}
		}

		return resultDefinitionArray;
	}

	uint32 VertexFormatDescriptor::_ResolveAttributeRefImpl( cppx::string_view pSemanticName, uint32 pSemanticIndex ) const noexcept
	{
		if( !pSemanticName.empty() && ( pSemanticIndex < GCM::kIAMaxVertexAttributeSemanticGroupSize ) )
		{
			// Query the internal AttributeLayout object for the index of a base attribute with the specified semantics.
			// If such attribute does not exist, an invalid index is returned instead (cxGCIVertexAttributeIndexUndefined).
			const auto baseAttributeSlot = _inputAttributeArrayLayout.QueryBaseAttributeBySemantics( pSemanticName );

			if( baseAttributeSlot != kGCIVertexInputAttributeSlotUndefined )
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
						Ic3DebugAssert( requestedAttributeComponent->shaderSemantics.semanticIndex == pSemanticIndex );
						return requestedAttributeIASlot;
					}
				}
			}
		}

		return kGCIVertexInputAttributeSlotUndefined;
	}

	uint32 VertexFormatDescriptor::_ResolveAttributeRefImpl( cppx::bitmask<ESystemAttributeSemanticFlags> pSystemSemanticFlags, uint32 pSemanticIndex ) const noexcept
	{
		if( GCU::IsStandardShaderInputAttribute( pSystemSemanticFlags ) )
		{
			const auto semanticName = GCU::GetStandardSemanticNameFromSystemFlags( pSystemSemanticFlags );
			return _ResolveAttributeRefImpl( semanticName, pSemanticIndex );
		}
		return kGCIVertexInputAttributeSlotUndefined;
	}


	namespace GCU
	{

		std::string GenerateVertexFormatLayoutString(
				const VertexInputAttributeArrayConfig & pAttributeLayout,
				const VertexInputStreamArrayConfig & pStreamConfig )
		{
			std::string vertexStreamStrings[GCM::kIAMaxDataStreamVertexBuffersNum];

			const auto & attributeArray = pAttributeLayout.GetAttributeArray();
			for( const auto & genericAttribute : attributeArray )
			{
				if( genericAttribute.IsActive() )
				{
					if( genericAttribute.IsBaseAttribute() )
					{
						const auto attributeFormatStr = GenerateVertexAttributeFormatString( genericAttribute );
						if( !vertexStreamStrings[genericAttribute.streamSlot].empty() )
						{
							vertexStreamStrings[genericAttribute.streamSlot].append( 1, '|' );
						}
						vertexStreamStrings[genericAttribute.streamSlot].append( attributeFormatStr );
					}
				}
			}

			std::string resultStringID;

			const auto & streamArray = pStreamConfig.GetStreamArray();
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
						resultStringID.append( 1, ( vertexStream.streamDataRate == EVertexDataRate::PerInstance ) ? 'I' : 'V' );
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

	}

}
