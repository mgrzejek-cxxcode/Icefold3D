
#include "VertexPipelineLayouts.h"

namespace Ic3
{

	namespace GCIUtils
	{

		bool checkAttributeLayoutArraySlotsFree(
				const SVertexAttributeArrayLayoutData & pAttributeLayoutData,
				uint32 pAttributeBaseIndex,
				uint32 pAttributeComponentsNum )
		{
			for( uint32 nComponent = 0; nComponent < pAttributeComponentsNum; ++nComponent )
			{
				const auto componentIndex = pAttributeBaseIndex + nComponent;
				const auto & attributeInfo = pAttributeLayoutData[componentIndex];

				if( attributeInfo.active() )
				{
					return false;
				}
			}

			return true;
		}

		bool validateVertexAttributeDefinition(
				const SVertexAttributeDefinition & pAttributeDefinition,
				const SVertexAttributeArrayLayoutData & pAttributeLayoutData,
				const SVertexStreamArrayLayoutData * pStreamLayoutData )
		{
			if( !pAttributeDefinition.valid() )
			{
				return false;
			}

			if( !checkAttributeLayoutArraySlotsFree( pAttributeLayoutData, pAttributeDefinition.baseAttributeIndex, pAttributeDefinition.componentsNum ) )
			{
				return false;
			}

			if( pStreamLayoutData )
			{
				const auto & attributeStream = pStreamLayoutData->streamAt( pAttributeDefinition.vertexStreamIndex );
				if( !attributeStream.checkAttributeCompatibility( pAttributeDefinition ) )
				{
					return false;
				}
			}

			return true;
		}

		bool validateVertexAttributeDefinitionWithUpdate(
				SVertexAttributeDefinition & pAttributeDefinition,
				const SVertexAttributeArrayLayoutData & pAttributeLayoutData,
				const SVertexStreamArrayLayoutData * pStreamLayoutData )
		{
			if( !validateVertexAttributeDefinition( pAttributeDefinition, pAttributeLayoutData, pStreamLayoutData ) )
			{
				return false;
			}

			if( pAttributeDefinition.hasAppendAsRelativeOffset() )
			{
				const auto & attributeStream = pStreamLayoutData->streamAt( pAttributeDefinition.vertexStreamIndex );
				if( pAttributeDefinition.vertexStreamRelativeOffset == GCI::CxDef::IA_VERTEX_ATTRIBUTE_OFFSET_APPEND16 )
				{
					pAttributeDefinition.vertexStreamRelativeOffset = memGetAlignedValue( attributeStream.elementStrideInBytes, 16 );
				}
				else if( pAttributeDefinition.vertexStreamRelativeOffset == GCI::CxDef::IA_VERTEX_ATTRIBUTE_OFFSET_APPEND )
				{
					pAttributeDefinition.vertexStreamRelativeOffset = attributeStream.elementStrideInBytes;
				}
			}

			const auto attributeBaseSize = pAttributeDefinition.baseSize();
			if( pAttributeDefinition.componentStride < attributeBaseSize )
			{
				pAttributeDefinition.componentStride = attributeBaseSize + pAttributeDefinition.componentPadding;
			}

			return true;
		}

		SGenericVertexAttributeInfo * appendVertexLayoutAttribute(
				SVertexAttributeDefinition pAttributeDefinition,
				SVertexAttributeArrayLayoutData & pAttributeLayoutData,
				SVertexStreamArrayLayoutData * pStreamLayoutData )
		{
			if( !GCIUtils::validateVertexAttributeDefinitionWithUpdate( pAttributeDefinition, pAttributeLayoutData, pStreamLayoutData ) )
			{
				return nullptr;
			}

			const auto baseAttributeIndex = pAttributeDefinition.baseAttributeIndex;
			const auto vertexStreamIndex = pAttributeDefinition.vertexStreamIndex;
			const auto attributeComponentsNum = pAttributeDefinition.componentsNum;

			auto & baseAttributeInfo = pAttributeLayoutData[baseAttributeIndex];
			baseAttributeInfo.initBaseAttributeFromDefinition( std::move( pAttributeDefinition ) );

			pAttributeLayoutData.activeBaseAttributesNum += 1;
			pAttributeLayoutData.activeGenericAttributesNum += 1;
			pAttributeLayoutData.activeAttributesMask.set( GCI::CxDef::makeIAVertexAttributeFlag( baseAttributeIndex ) );
			pAttributeLayoutData.activeAttributeSemanticsMask.set( static_cast<uint32>( baseAttributeInfo.semantics.smtID ) );

			pAttributeLayoutData.semanticIDMap[baseAttributeInfo.semantics.smtID] = baseAttributeIndex;
			pAttributeLayoutData.semanticNameMap[baseAttributeInfo.semantics.smtName] = baseAttributeIndex;

			for( uint32 nComponent = 1; nComponent < attributeComponentsNum; ++nComponent )
			{
				const auto subComponentIndex = baseAttributeIndex + nComponent;

				auto & subComponentInfo = pAttributeLayoutData.attributeAt( subComponentIndex );
				subComponentInfo.initSubComponentFromBaseAttribute( baseAttributeInfo, nComponent );

				pAttributeLayoutData.activeGenericAttributesNum += 1;
				pAttributeLayoutData.activeAttributesMask.set( GCI::CxDef::makeIAVertexAttributeFlag( subComponentIndex ) );
			}

			const auto lastComponentIndex = baseAttributeIndex + attributeComponentsNum - 1;
			pAttributeLayoutData.activeAttributesRange.add( SVertexAttributeIndexRange{ baseAttributeIndex, lastComponentIndex } );

			if( pStreamLayoutData )
			{
				appendVertexStreamAttribute( *pStreamLayoutData, baseAttributeInfo );
			}

			return &baseAttributeInfo;
		}

		bool appendVertexStreamAttribute(
				SVertexStreamArrayLayoutData & pStreamLayoutData,
				const SGenericVertexAttributeInfo & pBaseAttribute )
		{
			auto & attributeStream = pStreamLayoutData.streamAt( pBaseAttribute.vertexStreamIndex );

			if( !attributeStream )
			{
				attributeStream.initWithFirstAttribute( pBaseAttribute );

				pStreamLayoutData.activeStreamsNum += 1;
				pStreamLayoutData.activeStreamsRange.add( SVertexAttributeIndexRange{ pBaseAttribute.vertexStreamIndex, pBaseAttribute.vertexStreamIndex  } );
				pStreamLayoutData.activeStreamsMask.set( GCI::CxDef::makeIAVertexBufferFlag( pBaseAttribute.vertexStreamIndex ) );
				pStreamLayoutData.activeStreams.insert( pBaseAttribute.vertexStreamIndex );
			}

			for( uint32 nComponent = 0; nComponent < pBaseAttribute.componentsNum; ++nComponent )
			{
				const auto attributeIndex = pBaseAttribute.baseAttributeIndex + nComponent;

				attributeStream.activeAttributesNum += 1;
				attributeStream.activeAttributesMask.set( GCI::CxDef::makeIAVertexAttributeFlag( attributeIndex ) );
				attributeStream.elementStrideInBytes += pBaseAttribute.elementStride;
			}
		}

		bool createVertexStreamLayoutFromAttributeLayout(
				const SVertexAttributeArrayLayoutData & pAttributeLayoutData,
				SVertexStreamArrayLayoutData & pStreamLayoutData )
		{
			pStreamLayoutData.reset();

			for( const auto & attributeInfo : pAttributeLayoutData.attributesArray )
			{
				if( attributeInfo && attributeInfo.isBaseAttribute() )
				{
					auto & attributeStream = pStreamLayoutData.streamAt( attributeInfo.vertexStreamIndex );

					if( !attributeStream )
					{
						attributeStream.initWithFirstAttribute( attributeInfo );
					}

					for( uint32 nComponent = 0; nComponent < attributeInfo.componentsNum; ++nComponent )
					{
						const auto attributeIndex = attributeInfo.baseAttributeIndex + nComponent;

						attributeStream.activeAttributesNum += 1;
						attributeStream.activeAttributesMask.set( GCI::CxDef::makeIAVertexAttributeFlag( attributeIndex ) );
						attributeStream.elementStrideInBytes += attributeInfo.elementStride;
					}
				}
			}
		}

		bool createVertexStreamLayoutFromAttributeLayoutWithUpdate(
				SVertexAttributeArrayLayoutData & pAttributeLayoutData,
				SVertexStreamArrayLayoutData & pStreamLayoutData )
		{
		}

		const char * getAttributeFormatStringIdentifier( GCI::EVertexAttribFormat pGCIAttributeFormat )
		{
			switch( pGCIAttributeFormat )
			{
				ic3CaseReturn( GCI::EVertexAttribFormat::F16      , "1F16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::F32      , "1F32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::I8       , "1I8"   );
				ic3CaseReturn( GCI::EVertexAttribFormat::I16      , "1I16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::I32      , "1I32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::U8       , "1U8"   );
				ic3CaseReturn( GCI::EVertexAttribFormat::U16      , "1U16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::U32      , "1U32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::I8N      , "1I8N"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::I16N     , "1I16N" );
				ic3CaseReturn( GCI::EVertexAttribFormat::U8N      , "1U8N"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::U16N     , "1U16N" );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2F16  , "2F16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2F32  , "2F32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2I8   , "2I8"   );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2I16  , "2I16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2I32  , "2I32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2U8   , "2U8"   );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2U16  , "2U16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2U32  , "2U32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2I8N  , "2I8N"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2I16N , "2I16N" );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2U8N  , "2U8N"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec2U16N , "2U16N" );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec3F32  , "3F32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec3I32  , "3I32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec3U32  , "3U32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4F16  , "4F16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4F32  , "4F32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4I8   , "4I8"   );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4I16  , "4I16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4I32  , "4I32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4U8   , "4U8"   );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4U16  , "4U16"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4U32  , "4U32"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4I8N  , "4I8N"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4I16N , "4I16N" );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4U8N  , "4U8N"  );
				ic3CaseReturn( GCI::EVertexAttribFormat::Vec4U16N , "4U16N" );
				ic3CaseDefaultBreak();
			}

			return "#";
		}

		const char * getShaderSemanticShortName( const std::string_view & pSemanticName )
		{
			if( pSemanticName == "POSITION"  ) return "P";
			if( pSemanticName == "NORMAL"    ) return "N";
			if( pSemanticName == "TANGENT"   ) return "T";
			if( pSemanticName == "BITANGENT" ) return "B";
			if( pSemanticName == "COLOR"     ) return "C";
			if( pSemanticName == "TEXCOORD0" ) return "UV0";
			if( pSemanticName == "TEXCOORD1" ) return "UV1";
			if( pSemanticName == "TEXCOORD2" ) return "UV2";
			if( pSemanticName == "TEXCOORD3" ) return "UV3";
			if( pSemanticName == "TEXCOORD4" ) return "UV4";
			if( pSemanticName == "TEXCOORD5" ) return "UV5";
			if( pSemanticName == "TEXCOORD6" ) return "UV6";
			if( pSemanticName == "TEXCOORD7" ) return "UV7";
			if( pSemanticName == "INSTANCE0" ) return "I0";
			if( pSemanticName == "INSTANCE1" ) return "I1";

			return nullptr;
		}

		std::string generateVertexAttributeFormatString( const SGenericVertexAttributeInfo & pAttributeInfo )
		{
			std::string formatStr;
			formatStr.reserve( 16 );
			formatStr.append( 1, 'a' );
			formatStr.append( std::to_string( pAttributeInfo.baseAttributeIndex ) );

			if( const auto * semanticsIDCStr = getShaderSemanticShortName( pAttributeInfo.semantics.smtName ) )
			{
				formatStr.append( semanticsIDCStr );
			}
			else
			{
				formatStr.append( pAttributeInfo.semantics.smtName );
			}

			const auto baseFormatStr = getAttributeFormatStringIdentifier( pAttributeInfo.baseFormat );
			formatStr.append( 1, ':' );
			formatStr.append( baseFormatStr );

			if( pAttributeInfo.componentsNum > 1 )
			{
				formatStr.append( 1, '[' );
				formatStr.append( std::to_string( pAttributeInfo.componentsNum ) );
				formatStr.append( 1, ']' );
			}

			return formatStr;
		}

		std::string generateVertexAttributeLayoutString( const SVertexAttributeArrayLayoutData & pLayoutData )
		{
			std::string vertexStreamStrings[GCM::IA_MAX_VERTEX_STREAMS_NUM];

			for( uint32 iAttribute = 0; iAttribute < GCM::IA_MAX_VERTEX_ATTRIBUTES_NUM; ++iAttribute )
			{
				const auto & attributeInfo = pLayoutData[iAttribute];
				if( attributeInfo.isBaseAttribute() )
				{
					const auto attributeFormatStr = generateVertexAttributeFormatString( attributeInfo );
					if( !vertexStreamStrings[attributeInfo.vertexStreamIndex].empty() )
					{
						vertexStreamStrings[attributeInfo.vertexStreamIndex].append( 1, '|' );
					}
					vertexStreamStrings[attributeInfo.vertexStreamIndex].append( attributeFormatStr );
				}
			}

			std::string resultStringID;

			for( uint32 iStream = 0; iStream < GCM::IA_MAX_VERTEX_STREAMS_NUM; ++iStream )
			{
				if( !vertexStreamStrings[iStream].empty() )
				{
					if( !resultStringID.empty() )
					{
						resultStringID.append( 1, '_' );
					}
					resultStringID.append( 1, 's' );
					resultStringID.append( std::to_string( iStream ) );
					resultStringID.append( 1, '<' );
					resultStringID.append( vertexStreamStrings[iStream] );
					resultStringID.append( 1, '>' );
				}
			}

			return resultStringID;
		}

	}

}
