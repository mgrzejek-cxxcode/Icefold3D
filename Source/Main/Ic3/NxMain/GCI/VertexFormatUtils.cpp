
#include "VertexFormatUtils.h"
#include <cppx/stringUtils.h>
#include <regex>

namespace Ic3
{

	namespace GCIUtils
	{

		VertexInputAttributeDefinition MakeVertexInputAttributeDefinition(
			uint32 pVertexStreamSlot,
			VertexAttributeKey pAttributeKey,
			uint32 pDataPadding,
			uint32 pVertexStreamRelativeOffset );

		VertexInputAttributeDefinition MakeVertexInputAttributeDefinition(
			uint32 pVertexStreamSlot,
			VertexAttributeKey pAttributeKey,
			const VertexAttributeShaderSemantics & pAttributeShaderSemantics,
			uint32 pDataPadding,
			uint32 pVertexStreamRelativeOffset );

		VertexInputAttributeDefinition MakeVertexInputAttributeSemanticGroupDefinition(
			uint32 pVertexStreamSlot,
			uint32 pBaseAttributeSlot,
			const VertexAttributeShaderSemantics & pAttributeShaderSemantics,
			uint32 pSemanticGroupSize,
			GCI::EVertexAttribFormat pBaseDataFormat,
			uint32 pDataPadding,
			uint32 pVertexStreamRelativeOffset,
			GCI::EIAVertexAttributeDataRate pDataRate );

		GCI::IAVertexAttributeSemantics IAProcessAttributeSemantics( const GCI::IAVertexAttributeSemantics & pInputSemantics );

		GCI::IAVertexAttributeSemantics IAProcessAttributeSemantics( const cppx::immutable_string & pSemanticName, native_uint pSemanticIndex );

	}


	VertexFormatBuilderBase & VertexFormatBuilderBase::AddAttribute( VertexInputAttributeDefinition pAttributeDefinition )
	{
		if( !pAttributeDefinition.IsValid() )
		{
			Ic3DebugOutput( "" );
		}
		else
		{
			_attributeDefinitionArray.push_back( std::move( pAttributeDefinition ) );
		}

		return *this;
	}

	VertexFormatBuilderBase & VertexFormatBuilderBase::AddAttribute(
			uint32 pVertexStreamSlot,
			VertexAttributeKey pAttributeKey,
			uint32 pDataPadding,
			uint32 pVertexStreamRelativeOffset )
	{
		VertexInputAttributeDefinition attributeDefinition{};
		attributeDefinition.attributeSlot = pAttributeKey.GetBaseSlot();
		attributeDefinition.semanticGroupSize = pAttributeKey.GetSemanticGroupSize();
		attributeDefinition.baseDataFormat = pAttributeKey.GetBaseDataFormat();
		attributeDefinition.dataPadding = cppx::numeric_cast<decltype( attributeDefinition.dataPadding )>( pDataPadding );
		attributeDefinition.semanticName = GCIUtils::GetSemanticNameFromAttributeFlags( pAttributeKey.GetSemanticFlags() );
		attributeDefinition.semanticFlags = pAttributeKey.GetSemanticFlags();
		attributeDefinition.dataRate = pAttributeKey.GetDataRate();
		attributeDefinition.vertexStreamSlot = cppx::numeric_cast<decltype( attributeDefinition.vertexStreamSlot )>( pVertexStreamSlot );
		attributeDefinition.vertexStreamRelativeOffset = pVertexStreamRelativeOffset;

		return AddAttribute( std::move( attributeDefinition ) );
	}

	VertexFormatBuilderBase & VertexFormatBuilderBase::AddAttribute(
			uint32 pVertexStreamSlot,
			VertexAttributeKey pAttributeKey,
			const VertexAttributeShaderSemantics & pAttributeShaderSemantics,
			uint32 pDataPadding,
			uint32 pVertexStreamRelativeOffset )
	{
		VertexInputAttributeDefinition attributeDefinition{};
		attributeDefinition.attributeSlot = pAttributeKey.GetBaseSlot();
		attributeDefinition.semanticGroupSize = pAttributeKey.GetSemanticGroupSize();
		attributeDefinition.baseDataFormat = pAttributeKey.GetBaseDataFormat();
		attributeDefinition.dataPadding = cppx::numeric_cast<decltype( attributeDefinition.dataPadding )>( pDataPadding );
		attributeDefinition.semanticName = pAttributeShaderSemantics.semanticName;
		attributeDefinition.semanticFlags = pAttributeShaderSemantics.semanticFlags;
		attributeDefinition.dataRate = pAttributeKey.GetDataRate();
		attributeDefinition.vertexStreamSlot = cppx::numeric_cast<decltype( attributeDefinition.vertexStreamSlot )>( pVertexStreamSlot );
		attributeDefinition.vertexStreamRelativeOffset = pVertexStreamRelativeOffset;

		return AddAttribute( std::move( attributeDefinition ) );
	}

	VertexFormatBuilderBase & VertexFormatBuilderBase::AddAttribute(
			uint32 pVertexStreamSlot,
			uint32 pAttributeSlot,
			const VertexAttributeShaderSemantics & pAttributeShaderSemantics,
			GCI::EVertexAttribFormat pDataFormat,
			uint32 pDataPadding,
			uint32 pVertexStreamRelativeOffset,
			GCI::EIAVertexAttributeDataRate pDataRate )
	{
		VertexInputAttributeDefinition attributeDefinition{};
		attributeDefinition.attributeSlot = cppx::numeric_cast<decltype( attributeDefinition.attributeSlot )>( pAttributeSlot );
		attributeDefinition.semanticGroupSize = 1;
		attributeDefinition.baseDataFormat = pDataFormat;
		attributeDefinition.dataPadding = cppx::numeric_cast<decltype( attributeDefinition.dataPadding )>( pDataPadding );
		attributeDefinition.semanticName = pAttributeShaderSemantics.semanticName;
		attributeDefinition.semanticFlags = pAttributeShaderSemantics.semanticFlags;
		attributeDefinition.dataRate = pDataRate;
		attributeDefinition.vertexStreamSlot = cppx::numeric_cast<decltype( attributeDefinition.vertexStreamSlot )>( pVertexStreamSlot );
		attributeDefinition.vertexStreamRelativeOffset = pVertexStreamRelativeOffset;

		return AddAttribute( std::move( attributeDefinition ) );
	}

	VertexFormatBuilderBase & VertexFormatBuilderBase::AddAttributeSemanticGroup(
			uint32 pVertexStreamSlot,
			uint32 pBaseAttributeSlot,
			const VertexAttributeShaderSemantics & pAttributeShaderSemantics,
			uint32 pSemanticGroupSize,
			GCI::EVertexAttribFormat pBaseDataFormat,
			uint32 pDataPadding,
			uint32 pVertexStreamRelativeOffset,
			GCI::EIAVertexAttributeDataRate pDataRate )
	{
		VertexInputAttributeDefinition attributeDefinition{};
		attributeDefinition.attributeSlot = cppx::numeric_cast<decltype( attributeDefinition.attributeSlot )>( pBaseAttributeSlot );
		attributeDefinition.semanticGroupSize = cppx::numeric_cast<decltype( attributeDefinition.semanticGroupSize )>( pSemanticGroupSize );
		attributeDefinition.baseDataFormat = pBaseDataFormat;
		attributeDefinition.dataPadding = cppx::numeric_cast<decltype( attributeDefinition.dataPadding )>( pDataPadding );
		attributeDefinition.semanticName = pAttributeShaderSemantics.semanticName;
		attributeDefinition.semanticFlags = pAttributeShaderSemantics.semanticFlags;
		attributeDefinition.dataRate = pDataRate;
		attributeDefinition.vertexStreamSlot = cppx::numeric_cast<decltype( attributeDefinition.vertexStreamSlot )>( pVertexStreamSlot );
		attributeDefinition.vertexStreamRelativeOffset = pVertexStreamRelativeOffset;

		return AddAttribute( std::move( attributeDefinition ) );
	}

	VertexFormatBuilderBase & VertexFormatBuilderBase::AddAttributeList(
			const cppx::array_view<VertexInputAttributeDefinition> & pAttributeDefinitionList )
	{
		for( auto & attributeDefinition : pAttributeDefinitionList )
		{
			if( attributeDefinition.IsValid() )
			{
				_attributeDefinitionArray.push_back( std::move( attributeDefinition ) );
			}
		}

		return *this;
	}

	VertexFormatBuilderBase & VertexFormatBuilderBase::AddAttributeList(
			const cppx::array_view<const VertexInputAttributeDefinition> & pAttributeDefinitionList )
	{
		for( const auto & attributeDefinition : pAttributeDefinitionList )
		{
			if( attributeDefinition.IsValid() )
			{
				_attributeDefinitionArray.push_back( attributeDefinition );
			}
		}

		return *this;
	}

	bool VertexFormatBuilderBase::GenerateVertexFormatData(
			VertexInputAttributeArrayConfig & pAttributeArrayConfig,
			VertexInputStreamArrayConfig & pStreamArrayConfig,
			const cppx::array_view<VertexInputAttributeDefinition> & pAttributeDefinitionList )
	{
		if( !_ValidateAttributeDefinitions( pAttributeDefinitionList ) )
		{
			return false;
		}

		for( auto & attributeDefinition : pAttributeDefinitionList )
		{
			if( !pStreamArrayConfig.AddActiveStream( attributeDefinition.vertexStreamSlot, attributeDefinition.dataRate ) )
			{
				return false;
			}

			if( !_AdjustAttributeDefinition( attributeDefinition, pAttributeArrayConfig, pStreamArrayConfig ) )
			{
				return false;
			}

			auto * baseAttribute = pAttributeArrayConfig.AddAttribute( attributeDefinition );
			if( !pStreamArrayConfig.AppendAttribute( attributeDefinition.vertexStreamSlot, *baseAttribute ) )
			{
				return false;
			}
		}

		return true;
	}

	bool VertexFormatBuilderBase::_ValidateAttributeDefinitions(
			const cppx::array_view<VertexInputAttributeDefinition> & pAttributeDefinitionList ) noexcept
	{
		bool activeAttributesFlagArray[GCM::kIAMaxVertexAttributesNum]{};
		bool activeStreamsFlagArray[GCM::kIAMaxDataStreamVertexBuffersNum]{};

		uint32 activeAttributesNum = 0;
		uint32 activeStreamsNum = 0;

		for( const auto & attributeDefinition : pAttributeDefinitionList )
		{
			if( attributeDefinition.IsValid() )
			{
				if( !activeAttributesFlagArray[attributeDefinition.attributeSlot] )
				{
					activeAttributesFlagArray[attributeDefinition.attributeSlot] = true;
					activeAttributesNum += attributeDefinition.semanticGroupSize;
				}

				if( !activeStreamsFlagArray[attributeDefinition.vertexStreamSlot] )
				{
					activeStreamsFlagArray[attributeDefinition.vertexStreamSlot] = true;
					activeStreamsNum += 1;
				}
			}
		}

		return ( activeAttributesNum > 0 ) && ( activeStreamsNum > 0 );
	}

	bool VertexFormatBuilderBase::_AdjustAttributeDefinition(
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
			if( const auto * vertexInputStream = pInputStreamArrayConfig.StreamPtr( pAttributeDefinition.vertexStreamSlot ) )
			{
				if( !vertexInputStream->IsInitialized() )
				{
					return false;
				}

				// If the stream is already defined in the stream array config, replace the "append" offsets with
				// the current data size defined for this stream (potentially aligned, if requested).
				if( pAttributeDefinition.HasAppendAsRelativeOffset() )
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

		if( pAttributeDefinition.dataPadding > 0 )
		{
			const auto attributeSizeInBytes = pAttributeDefinition.GetDataSizeInBytes();
			Ic3DebugAssert( attributeSizeInBytes <= 16 );

			const auto maxAllowedPaddingToFit = 16 - attributeSizeInBytes;
			const auto correctPaddingValue = cppx::get_min_of( pAttributeDefinition.dataPadding, maxAllowedPaddingToFit );

			pAttributeDefinition.dataPadding = cppx::numeric_cast<decltype( pAttributeDefinition.dataPadding )>( correctPaddingValue );
		}

		return true;
	}


	TSharedHandle<VertexFormatSignature> VertexFormatSignatureBuilder::CreateVertexFormatSignature(
			const GCI::EPrimitiveTopology pPrimitiveTopology,
			const GCI::EIndexDataFormat pIndexDataFormat )
	{
		auto vertexFormatSignature = CreateDynamicObject<VertexFormatSignature>();
		if( !InitializeVertexFormatSignatureFromDefinitionList(
				*vertexFormatSignature,
				cppx::bind_array_view( _attributeDefinitionArray ),
				pPrimitiveTopology,
				pIndexDataFormat ) )
		{
			vertexFormatSignature.reset();
		}
		return vertexFormatSignature;
	}

	bool VertexFormatSignatureBuilder::InitializeVertexFormatSignature(
			VertexFormatSignature & pVertexFormatSignature,
			const GCI::EPrimitiveTopology pPrimitiveTopology,
			const GCI::EIndexDataFormat pIndexDataFormat )
	{
		return InitializeVertexFormatSignatureFromDefinitionList(
				pVertexFormatSignature,
				cppx::bind_array_view( _attributeDefinitionArray ),
				pPrimitiveTopology,
				pIndexDataFormat );
	}

	TSharedHandle<VertexFormatSignature> VertexFormatSignatureBuilder::CreateVertexFormatSignatureFromDefinitionList(
			const cppx::array_view<VertexInputAttributeDefinition> & pAttributeDefinitionList,
			const GCI::EPrimitiveTopology pPrimitiveTopology,
			const GCI::EIndexDataFormat pIndexDataFormat )
	{
		auto vertexFormatSignature = CreateDynamicObject<VertexFormatSignature>();
		if( !InitializeVertexFormatSignatureFromDefinitionList(
				*vertexFormatSignature,
				pAttributeDefinitionList,
				pPrimitiveTopology,
				pIndexDataFormat ) )
		{
			vertexFormatSignature.reset();
		}
		return vertexFormatSignature;
	}

	TSharedHandle<VertexFormatSignature> VertexFormatSignatureBuilder::CreateVertexFormatSignatureFromSerialString(
			const std::string_view & pVertexFormatSignatureSerialString,
			const GCI::EPrimitiveTopology pPrimitiveTopology,
			const GCI::EIndexDataFormat pIndexDataFormat )
	{
		auto vertexFormatSignature = CreateDynamicObject<VertexFormatSignature>();
		if( !InitializeVertexFormatSignatureFromSerialString(
				*vertexFormatSignature,
				pVertexFormatSignatureSerialString,
				pPrimitiveTopology,
				pIndexDataFormat ) )
		{
			vertexFormatSignature.reset();
		}
		return vertexFormatSignature;
	}

	bool VertexFormatSignatureBuilder::InitializeVertexFormatSignatureFromDefinitionList(
			VertexFormatSignature & pVertexFormatSignature,
			const cppx::array_view<VertexInputAttributeDefinition> & pAttributeDefinitionList,
			const GCI::EPrimitiveTopology pPrimitiveTopology,
			const GCI::EIndexDataFormat pIndexDataFormat )
	{
		if( ( pIndexDataFormat == GCI::EIndexDataFormat::Undefined ) || ( pPrimitiveTopology == GCI::EPrimitiveTopology::Undefined ) )
		{
			return false;
		}

		auto & attributeArrayConfig = pVertexFormatSignature._inputAttributeArrayLayout;
		auto & streamArrayConfig = pVertexFormatSignature._inputStreamArrayConfig;

		if( !GenerateVertexFormatData( attributeArrayConfig, streamArrayConfig, pAttributeDefinitionList ) )
		{
			return false;
		}

		pVertexFormatSignature._primitiveTopology = pPrimitiveTopology;
		pVertexFormatSignature._indexDataFormat = pIndexDataFormat;

		return true;
	}

	bool VertexFormatSignatureBuilder::InitializeVertexFormatSignatureFromSerialString(
			VertexFormatSignature & pVertexFormatSignature,
			const std::string_view & pVertexFormatSignatureSerialString,
			const GCI::EPrimitiveTopology pPrimitiveTopology,
			const GCI::EIndexDataFormat pIndexDataFormat )
	{
		auto attributeDefinitionList = GCIUtils::ParseVertexFormatSignatureSerialString(
				pVertexFormatSignatureSerialString );

		return InitializeVertexFormatSignatureFromDefinitionList(
			pVertexFormatSignature,
			cppx::bind_array_view( attributeDefinitionList ),
			pPrimitiveTopology,
			pIndexDataFormat );
	}


	GCI::IAVertexAttributeLayoutDefinition GCIVertexAttributeLayoutBuilder::CreateGCIVertexAttributeLayoutDefinition(
			const GCI::EPrimitiveTopology pPrimitiveTopology )
	{
		GCI::IAVertexAttributeLayoutDefinition gciAttributeLayoutDefinition{};
		InitializeGCIVertexAttributeLayoutDefinition( gciAttributeLayoutDefinition, pPrimitiveTopology );
		return gciAttributeLayoutDefinition;
	}

	bool GCIVertexAttributeLayoutBuilder::InitializeGCIVertexAttributeLayoutDefinition(
	GCI::IAVertexAttributeLayoutDefinition & pLayoutDefinition,
	const GCI::EPrimitiveTopology pPrimitiveTopology )
	{
		return InitializeGCIVertexAttributeLayoutDefinition(
				pLayoutDefinition,
				cppx::bind_array_view( _attributeDefinitionArray ),
				pPrimitiveTopology );
	}

	GCI::IAVertexAttributeLayoutDefinition GCIVertexAttributeLayoutBuilder::CreateGCIVertexAttributeLayoutDefinition(
	const cppx::array_view<VertexInputAttributeDefinition> & pAttributeDefinitionList,
	const GCI::EPrimitiveTopology pPrimitiveTopology )
	{
		GCI::IAVertexAttributeLayoutDefinition gciAttributeLayoutDefinition{};

		InitializeGCIVertexAttributeLayoutDefinition(
				gciAttributeLayoutDefinition,
				pAttributeDefinitionList,
				pPrimitiveTopology );

		return gciAttributeLayoutDefinition;
	}

	bool GCIVertexAttributeLayoutBuilder::InitializeGCIVertexAttributeLayoutDefinition(
			GCI::IAVertexAttributeLayoutDefinition & pLayoutDefinition,
			const cppx::array_view<VertexInputAttributeDefinition> & pAttributeDefinitionList,
			const GCI::EPrimitiveTopology pPrimitiveTopology )
	{
		VertexInputAttributeArrayConfig attributeArrayConfig;
		VertexInputStreamArrayConfig streamArrayConfig;

		if( !GenerateVertexFormatData( attributeArrayConfig, streamArrayConfig, pAttributeDefinitionList ) )
		{
			return false;
		}

		for( uint32 iAttributeIndex = 0; GCIUtils::IAIsVertexAttributeSlotValid( iAttributeIndex ); ++iAttributeIndex )
		{
			if( const auto & genericAttribute = attributeArrayConfig.AttributeAt( iAttributeIndex ) )
			{
				Ic3DebugAssert( genericAttribute.attributeSlot == iAttributeIndex );

				auto & gciAttributeDesc = pLayoutDefinition.attributeArray[iAttributeIndex];
				gciAttributeDesc.attribInfo.attributeSlot = genericAttribute.attributeSlot;
				gciAttributeDesc.attribInfo.dataFormat = genericAttribute.dataFormat;
				gciAttributeDesc.attribInfo.dataRate = genericAttribute.dataRate;
				gciAttributeDesc.semantics = GCIUtils::IAProcessAttributeSemantics( genericAttribute.semanticName, genericAttribute.semanticIndex );
				gciAttributeDesc.streamBinding.streamSlot = genericAttribute.vertexStreamSlot;
				gciAttributeDesc.streamBinding.streamRelativeOffset = genericAttribute.vertexStreamRelativeOffset;
			}
		}

		pLayoutDefinition.activeAttributesMask = attributeArrayConfig.GetActiveAttributesMask();
		pLayoutDefinition.primitiveTopology = pPrimitiveTopology;

		return true;
	}


	namespace GCIUtils
	{

		VertexInputAttributeDefinition MakeVertexInputAttributeDefinition(
				uint32 pVertexStreamSlot,
				VertexAttributeKey pAttributeKey,
				uint32 pDataPadding,
				uint32 pVertexStreamRelativeOffset );

		VertexInputAttributeDefinition MakeVertexInputAttributeDefinition(
				uint32 pVertexStreamSlot,
				VertexAttributeKey pAttributeKey,
				const VertexAttributeShaderSemantics & pAttributeShaderSemantics,
				uint32 pDataPadding,
				uint32 pVertexStreamRelativeOffset );

		VertexInputAttributeDefinition MakeVertexInputAttributeSemanticGroupDefinition(
				uint32 pVertexStreamSlot,
				uint32 pBaseAttributeSlot,
				const VertexAttributeShaderSemantics & pAttributeShaderSemantics,
				uint32 pSemanticGroupSize,
				GCI::EVertexAttribFormat pBaseDataFormat,
				uint32 pDataPadding,
				uint32 pVertexStreamRelativeOffset,
				GCI::EIAVertexAttributeDataRate pDataRate);

		GCI::IAVertexAttributeSemantics IAProcessAttributeSemantics( const GCI::IAVertexAttributeSemantics & pInputSemantics )
		{
			return IAProcessAttributeSemantics( pInputSemantics.semanticName, pInputSemantics.semanticIndex );
		}

		GCI::IAVertexAttributeSemantics IAProcessAttributeSemantics( const cppx::immutable_string & pSemanticName, native_uint pSemanticIndex )
		{
			static const auto reStrDX11AttrNameSemanticsCombined = R"(^[A-Za-z]+([0-9]*)$)";
			static const std::regex reDX11AttrNameSemanticsCombined{ reStrDX11AttrNameSemanticsCombined };

			static const auto reStrDX11AttrNameSemanticIndexOnly = R"(([0-9]*)$)";
			static const std::regex reDX11AttrNameSemanticIndexOnly{ reStrDX11AttrNameSemanticIndexOnly };

			GCI::IAVertexAttributeSemantics resultSemantics{};
			resultSemantics.semanticName = pSemanticName;
			resultSemantics.semanticIndex = pSemanticIndex;

			std::smatch regexMatch;
			if( std::regex_search( resultSemantics.semanticName.str(), regexMatch, reDX11AttrNameSemanticsCombined ) )
			{
				const auto attributeSemanticIndexStr = regexMatch[1].str();
				if( !attributeSemanticIndexStr.empty() )
				{
					const auto attributeSemanticIndex = cppx::from_string<uint32>( attributeSemanticIndexStr );
					Ic3DebugAssert( ( resultSemantics.semanticIndex == 0 ) || ( attributeSemanticIndex == resultSemantics.semanticIndex ) );

					if( resultSemantics.semanticIndex == 0 )
					{
						resultSemantics.semanticIndex = cppx::numeric_cast< uint8 >( attributeSemanticIndex );
					}

					resultSemantics.semanticName = std::regex_replace( resultSemantics.semanticName.str(), reDX11AttrNameSemanticIndexOnly, "" );
				}
			}

			return resultSemantics;
		}

	}

}
