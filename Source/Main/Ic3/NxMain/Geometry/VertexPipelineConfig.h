
#pragma once

#ifndef __IC3_NXMAIN_VERTEX_INPUT_LAYOUT_H__
#define __IC3_NXMAIN_VERTEX_INPUT_LAYOUT_H__

#include "GeometryCommonDefs.h"
#include "../GCI/VertexPipelineLayouts.h"

namespace Ic3
{

	class CVertexPipelineConfig
	{
		friend class CVertexPipelineConfigBuilder;

	public:
		const GCI::EPrimitiveTopology & mPrimitiveTopology;
		const GCI::EIndexDataFormat & mIndexDataFormat;
		const CVertexAttributeArrayLayout & mAttributeLayout;
		const CVertexStreamArrayLayout & mStreamLayout;

	public:
		CVertexPipelineConfig();
		~CVertexPipelineConfig();

		IC3_ATTR_NO_DISCARD bool empty() const noexcept;

		IC3_ATTR_NO_DISCARD bool isIndexedGeometry() const noexcept;
		IC3_ATTR_NO_DISCARD bool isAttributeActive( uint32 pAttributeIndex ) const;
		IC3_ATTR_NO_DISCARD bool isVertexStreamActive( uint32 pStreamIndex ) const;

		IC3_ATTR_NO_DISCARD uint32 getIndexDataSize() const noexcept;

		IC3_ATTR_NO_DISCARD uint32 resolveAttributeRef( std::string_view pSemanticName, uint32 pSubIndex = 0 ) const noexcept;
		IC3_ATTR_NO_DISCARD uint32 resolveAttributeRef( EShaderInputSemanticID pSemanticID ) const noexcept;
		IC3_ATTR_NO_DISCARD uint32 resolveAttributeRef( EStandardVertexAttributeID pAttributeID ) const noexcept;
		IC3_ATTR_NO_DISCARD uint32 resolveAttributeRef( const SShaderSemantics & pSemantics ) const noexcept;

		IC3_ATTR_NO_DISCARD const SGenericVertexAttributeInfo * getAttribute( uint32 pAttributeIndex ) const noexcept;
		IC3_ATTR_NO_DISCARD const SGenericVertexAttributeInfo * getAttribute( std::string_view pSemanticName, uint32 pSubIndex = 0 ) const noexcept;
		IC3_ATTR_NO_DISCARD const SGenericVertexAttributeInfo * getAttribute( EShaderInputSemanticID pSemanticID ) const noexcept;
		IC3_ATTR_NO_DISCARD const SGenericVertexAttributeInfo * getAttribute( EStandardVertexAttributeID pAttributeID ) const noexcept;
		IC3_ATTR_NO_DISCARD const SGenericVertexAttributeInfo * getAttribute( const SShaderSemantics & pSemantics ) const noexcept;

		IC3_ATTR_NO_DISCARD const SVertexStreamInfo * getStream( uint32 pStreamIndex ) const noexcept;

		IC3_ATTR_NO_DISCARD const SVertexStreamInfo * getStreamForAttribute( uint32 pAttributeIndex ) const noexcept;
		IC3_ATTR_NO_DISCARD const SVertexStreamInfo * getStreamForAttribute( std::string_view pSemanticName, uint32 pSubIndex = 0 ) const noexcept;
		IC3_ATTR_NO_DISCARD const SVertexStreamInfo * getStreamForAttribute( EShaderInputSemanticID pSemanticID ) const noexcept;
		IC3_ATTR_NO_DISCARD const SVertexStreamInfo * getStreamForAttribute( EStandardVertexAttributeID pAttributeID ) const noexcept;
		IC3_ATTR_NO_DISCARD const SVertexStreamInfo * getStreamForAttribute( const SShaderSemantics & pSemantics ) const noexcept;

	private:
		IC3_ATTR_NO_DISCARD const SVertexStreamInfo * _getVertexStreamForAttribute(
				const SGenericVertexAttributeInfo * pAttributeInfo ) const noexcept;

	protected:
		GCI::EPrimitiveTopology _primitiveTopology = GCI::EPrimitiveTopology::Undefined;
		GCI::EIndexDataFormat _indexDataFormat = GCI::EIndexDataFormat::Undefined;
		CVertexAttributeArrayLayout _attribLayout;
		CVertexStreamArrayLayout _streamLayout;
		GCI::IAInputLayoutImmutableStateHandle _gciInputLayoutDefault;
		GCI::IAInputLayoutImmutableStateHandle _gciInputLayoutPosition;
		GCI::IAInputLayoutImmutableStateHandle _gciInputLayoutPositionNormal;
	};

	inline uint32 CVertexPipelineConfig::resolveAttributeRef( std::string_view pSemanticName, uint32 pSubIndex ) const noexcept
	{
		return mAttributeLayout.resolveAttributeRef( pSemanticName, pSubIndex );
	}

	inline uint32 CVertexPipelineConfig::resolveAttributeRef( EShaderInputSemanticID pSemanticID ) const noexcept
	{
		return mAttributeLayout.resolveAttributeRef( pSemanticID );
	}

	inline uint32 CVertexPipelineConfig::resolveAttributeRef( EStandardVertexAttributeID pAttributeID ) const noexcept
	{
		return mAttributeLayout.resolveAttributeRef( pAttributeID );
	}

	inline uint32 CVertexPipelineConfig::resolveAttributeRef( const SShaderSemantics & pSemantics ) const noexcept
	{
		return mAttributeLayout.resolveAttributeRef( pSemantics );
	}

	inline const SGenericVertexAttributeInfo * CVertexPipelineConfig::getAttribute( uint32 pAttributeIndex ) const noexcept
	{
		return mAttributeLayout.getAttribute( pAttributeIndex );
	}

	inline const SGenericVertexAttributeInfo * CVertexPipelineConfig::getAttribute( std::string_view pSemanticName, uint32 pSubIndex ) const noexcept
	{
		return mAttributeLayout.getAttribute( pSemanticName, pSubIndex );
	}

	inline const SGenericVertexAttributeInfo * CVertexPipelineConfig::getAttribute( EShaderInputSemanticID pSemanticID ) const noexcept
	{
		return mAttributeLayout.getAttribute( pSemanticID );
	}

	inline const SGenericVertexAttributeInfo * CVertexPipelineConfig::getAttribute( EStandardVertexAttributeID pAttributeID ) const noexcept
	{
		return mAttributeLayout.getAttribute( pAttributeID );
	}

	inline const SGenericVertexAttributeInfo * CVertexPipelineConfig::getAttribute( const SShaderSemantics & pSemantics ) const noexcept
	{
		return mAttributeLayout.getAttribute( pSemantics );
	}

	inline const SVertexStreamInfo * CVertexPipelineConfig::getStream( uint32 pStreamIndex ) const noexcept
	{
		return mStreamLayout.getStream( pStreamIndex );
	}


	class CVertexPipelineConfigBuilder
	{
	public:
		CVertexPipelineConfigBuilder();
		virtual ~CVertexPipelineConfigBuilder();

		IC3_ATTR_NO_DISCARD bool checkAttributeSlotRangeFree(
				uint32 pAttributeBaseIndex,
				uint32 pAttributeComponentsNum ) const noexcept;

		IC3_ATTR_NO_DISCARD bool validateAttributeDefinition(
				SVertexAttributeDefinition & pAttributeDefinition,
				uint32 pComponentStrideInBytes = 0,
				uint32 pCombinedAttributeDataStride = 0 ) const noexcept;

		IC3_ATTR_NO_DISCARD SharedHandle<CVertexPipelineConfig> getPipelineConfig() const noexcept;

		void setPrimitiveTopology( GCI::EPrimitiveTopology pTopology );

		void setIndexDataFormat( GCI::EIndexDataFormat pIndexDataFormat );

		SGenericVertexAttributeInfo * addAttribute( SVertexAttributeDefinition pAttributeDefinition );

		SGenericVertexAttributeInfo * addStandardAttribute(
				EStandardVertexAttributeID pAttributeID,
				uint32 pStreamIndex,
				uint32 pStreamRelativeOffset,
				uint32 pDataStride = 0 );

		template <typename TVertex, typename TAttribute>
		bool addAttribute(
				uint32 pBaseIndex,
				TAttribute TVertex::* pAttributePtr,
				SShaderSemantics pSemantics,
				uint32 pStreamIndex,
				uint32 pStreamRelativeOffset = GCI::CxDef::IA_VERTEX_ATTRIBUTE_OFFSET_APPEND,
				uint32 pInstanceRate = 0 );

		template <typename TVertex, typename TAttribute>
		bool addStandardAttribute(
				EStandardVertexAttributeID pAttributeID,
				TAttribute TVertex::* pAttributePtr,
				uint32 pStreamIndex,
				uint32 pStreamRelativeOffset = GCI::CxDef::IA_VERTEX_ATTRIBUTE_OFFSET_APPEND );

		SharedHandle<CVertexPipelineConfig> reset( SharedHandle<CVertexPipelineConfig> pPipelineConfig = nullptr );

	private:
		IC3_ATTR_NO_DISCARD bool validateAttributeDefinition(
				SVertexAttributeDefinition & pAttributeDefinition,
				uint32 pComponentStrideInBytes );

		IC3_ATTR_NO_DISCARD bool resolveAttributeSemanticsDefinition(
				SShaderSemantics & pAttributeSemantics );

		SGenericVertexAttributeInfo & updateAttribute(
				const SVertexAttributeDefinition & pAttributeDefinition,
				uint32 pComponentStrideInBytes );

		void updateAttributeSubComponents(
				SGenericVertexAttributeInfo & pBaseAttribute,
				uint32 pAttributeComponentsNum,
				uint32 pComponentStrideInBytes );

		void updateVertexStreamState(
				SGenericVertexAttributeInfo & pBaseAttribute );;

	private:
		SharedHandle<CVertexPipelineConfig> _pipelineConfig;
	};

	template <typename TVertex, typename TAttribute>
	inline bool CVertexPipelineConfigBuilder::addAttribute(
			uint32 pBaseIndex,
			TAttribute TVertex::* pAttributePtr,
			SShaderSemantics pSemantics,
			uint32 pVertexBufferIndex,
			uint32 pVertexBufferRelativeOffset,
			uint32 pInstanceRate )
	{
		using AttributeFormatTraits = GCI::VertexAttribFormatDataTypeTraits<TAttribute>;

		SVertexAttributeDefinition attributeDefinition{};
		attributeDefinition.baseAttributeIndex = pBaseIndex;
		attributeDefinition.baseFormat = AttributeFormatTraits::sBaseAttribFormat;
		attributeDefinition.instanceRate = pInstanceRate;
		attributeDefinition.dataStride = sizeof( TVertex );
		attributeDefinition.vertexStreamIndex = pVertexBufferIndex;
		attributeDefinition.vertexStreamRelativeOffset = pVertexBufferRelativeOffset + memberOffset( pAttributePtr );
		attributeDefinition.semantics = std::move( pSemantics );
		attributeDefinition.componentsNum = AttributeFormatTraits::sAttribComponentsNum;
		attributeDefinition.subComponentPadding = 0;

		return addAttribute( std::move( attributeDefinition ) );
	}

	template <typename TVertex, typename TAttribute>
	inline bool CVertexPipelineConfigBuilder::addStandardAttribute(
			EStandardVertexAttributeID pFixedAttributeID,
			TAttribute TVertex::* pAttributePtr,
			uint32 pVertexBufferIndex,
			uint32 pVertexBufferRelativeOffset )
	{
		const auto fixedAttributeFormat = CxDef::getStandardVertexAttributeBaseFormat( pFixedAttributeID );
		const auto fixedAttributeComponentsNum = CxDef::getStandardVertexAttributeComponentsNum( pFixedAttributeID );
		if( !GCIUtils::checkAttributeAutoDataFormat<TAttribute>( fixedAttributeFormat, fixedAttributeComponentsNum ) )
		{
			return false;
		}

		return setStandardAttribute( pFixedAttributeID, pVertexBufferIndex, pVertexBufferRelativeOffset + memberOffset( pAttributePtr ) );
	}

	//	template <typename TVertex, typename TAttribute>
	//	inline bool CVertexPipelineConfigBuilder::setFixedAttribute(
	//			EStandardVertexAttributeID pFixedAttributeID,
	//			uint32 pStreamIndex,
	//			TAttribute TVertex::* pAttributePtr )
	//	{
	//		const auto fixedAttributeFormat = CxDef::getStandardVertexAttributeBaseFormat( pFixedAttributeID );
	//		const auto fixedAttributeComponentsNum = CxDef::getStandardVertexAttributeComponentsNum( pFixedAttributeID );
	//		if( !checkAttributeAutoDataFormat<TAttribute>( fixedAttributeFormat, fixedAttributeComponentsNum ) )
	//		{
	//			return false;
	//		}
	//
	//		return setFixedAttribute(
	//				pFixedAttributeID,
	//				pStreamIndex,
	//				memberOffset( pAttributePtr ) );
	//	}
	//
	//	template <typename TVertex, typename TAttribute>
	//	inline bool CVertexPipelineConfigBuilder::setCustomAttribute(
	//			uint32 pAttributeBaseIndex,
	//			SShaderSemantics pSemantics,
	//			GCI::EVertexAttribFormat pAttributeBaseFormat,
	//			uint32 pAttributeComponentsNum,
	//			uint32 pStreamIndex,
	//			TAttribute TVertex::* pAttributePtr,
	//			EVertexDataRate pAttributeDataRate )
	//	{
	//		if( !checkAttributeAutoDataFormat<TAttribute>( pAttributeBaseFormat, pAttributeComponentsNum ) )
	//		{
	//			return false;
	//		}
	//
	//		return setCustomAttribute(
	//				pAttributeBaseIndex,
	//				std::move( pSemantics ),
	//				pAttributeBaseFormat,
	//				pAttributeComponentsNum,
	//				pStreamIndex,
	//				memberOffset( pAttributePtr ),
	//				pAttributeDataRate );
	//	}
	//
	//	template <typename TVertex, typename TAttribute>
	//	inline bool CVertexPipelineConfigBuilder::setCustomAttributeAuto(
	//			uint32 pAttributeBaseIndex,
	//			SShaderSemantics pSemantics,
	//			uint32 pStreamIndex,
	//			TAttribute TVertex::* pAttributePtr,
	//			EVertexDataRate pAttributeDataRate )
	//	{
	//		using AttributeTraits = GCI::VertexAttribFormatDataTypeTraits<TAttribute>;
	//		if( !checkAttributeAutoDataFormat<TAttribute>( AttributeTraits::sVertexAttribFormat, AttributeTraits::sVertexAttribComponentsNum ) )
	//		{
	//			return false;
	//		}
	//
	//		return setCustomAttribute(
	//				pAttributeBaseIndex,
	//				std::move( pSemantics ),
	//				AttributeTraits::sVertexAttribFormat,
	//				AttributeTraits::sVertexAttribComponentsNum,
	//				pStreamIndex,
	//				memberOffset( pAttributePtr ),
	//				pAttributeDataRate );
	//	}
	//
	//	template <typename TAttribute>
	//	inline bool CVertexPipelineConfigBuilder::checkAttributeAutoDataFormat(
	//			GCI::EVertexAttribFormat pAttributeComponentFormat,
	//			uint16 pAttributeComponentsNum ) const noexcept
	//	{
	//		const auto componentByteSize = GCI::CxDef::getVertexAttribFormatByteSize( pAttributeComponentFormat );
	//		const auto attributeByteSize = componentByteSize * pAttributeComponentsNum;
	//
	//		return sizeof( TAttribute ) == attributeByteSize;
	//	}

} // namespace Ic3

#endif // __IC3_NXMAIN_VERTEX_INPUT_LAYOUT_H__
