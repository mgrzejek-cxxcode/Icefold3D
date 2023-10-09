
#pragma once

#ifndef __IC3_NXMAIN_GEOMETRY_DATA_FORMAT_H__
#define __IC3_NXMAIN_GEOMETRY_DATA_FORMAT_H__

#include "GeometryCommonDefs.h"
#include "GeometryVertexFormat.h"
#include <unordered_map>

namespace Ic3
{
	
	struct GeometryDataFormatBaseProperties
	{
		uint32 activeAttributesNum = 0;
		uint32 activeAttributeSlotsNum = 0;
		uint32 activeVertexStreamsNum = 0;
		uint32 vertexElementSizeInBytes = 0;
		Bitmask<GCI::EIAVertexAttributeFlags> activeAttributesMask;
		Bitmask<EVertexAttributeSemanticsFlags> activeAttributeSemanticsMask;
		Bitmask<GCI::EIAVertexStreamBindingFlags> activeVertexStreamsMask;
	};

	class GeometryDataFormatBase
	{
	public:
		GeometryDataFormatBase(
				VertexAttributeArrayLayoutBase * pAttribLayoutPtr,
				VertexStreamArrayLayoutBase * pStreamLayoutPtr );

		virtual ~GeometryDataFormatBase();

		IC3_ATTR_NO_DISCARD std::string getFormatStringID() const noexcept;

		IC3_ATTR_NO_DISCARD const VertexAttributeFormat & attribute( uint32 pAttributeIndex ) const;

		IC3_ATTR_NO_DISCARD const VertexAttributeFormat & attribute( EVertexAttributeSemanticsID pAttributeSemantics ) const;

		IC3_ATTR_NO_DISCARD const VertexStreamFormat & vertexStream( uint32 pStreamIndex ) const;

		IC3_ATTR_NO_DISCARD ArrayView<const uint16> activeVertexStreams() const noexcept;

		IC3_ATTR_NO_DISCARD uint32 activeVertexStreamsNum() const noexcept;

		IC3_ATTR_NO_DISCARD uint16 firstActiveVertexStream() const noexcept;

		IC3_ATTR_NO_DISCARD uint32 vertexElementSizeInBytes() const noexcept;

		IC3_ATTR_NO_DISCARD uint32 vertexStreamElementSizeInBytes( uint32 pStreamIndex ) const;

		IC3_ATTR_NO_DISCARD GCI::EIndexDataFormat indexDataFormat() const noexcept;

		IC3_ATTR_NO_DISCARD uint32 indexElementSizeInBytes() const noexcept;

		IC3_ATTR_NO_DISCARD bool isAttributeSemanticsActive( EVertexAttributeSemanticsID pAttributeSemantics ) const noexcept;

		IC3_ATTR_NO_DISCARD bool isFixedAttributeActive( EFixedVertexAttributeID pFixedAttribute ) const noexcept;

		IC3_ATTR_NO_DISCARD bool isIndexedGeometry() const noexcept;

		IC3_ATTR_NO_DISCARD bool isAttributeSlotUsed( uint32 pAttributeIndex ) const;

		IC3_ATTR_NO_DISCARD bool isAttributeActive( uint32 pAttributeIndex ) const;

		IC3_ATTR_NO_DISCARD bool isVertexStreamActive( uint32 pStreamIndex ) const;

		IC3_ATTR_NO_DISCARD bool empty() const noexcept;

		void setIndexDataFormat( GCI::EIndexDataFormat pIndexDataFormat );

		void setPrimitiveTopology( GCI::EPrimitiveTopology pTopology );

		bool configureFixedAttribute(
				EFixedVertexAttributeID pFixedAttributeID,
				uint32 pStreamIndex,
				uint16 pStreamElementRelativeOffset = GCI::CxDefs::IA_VERTEX_ATTRIBUTE_OFFSET_APPEND );

		template <typename TVertex, typename TAttribute>
		bool configureFixedAttribute(
				EFixedVertexAttributeID pFixedAttributeID,
				uint32 pStreamIndex,
				TAttribute TVertex::* pAttributePtr );

		bool configureCustomAttribute(
				uint32 pAttributeBaseIndex,
				VertexAttributeSemantics pSemantics,
				GCI::EVertexAttribFormat pAttributeBaseFormat,
				uint32 pAttributeComponentsNum,
				uint32 pStreamIndex,
				uint16 pStreamElementRelativeOffset = GCI::CxDefs::IA_VERTEX_ATTRIBUTE_OFFSET_APPEND,
				EVertexDataRate pAttributeDataRate = EVertexDataRate::PerVertex );

		template <typename TVertex, typename TAttribute>
		bool configureCustomAttribute(
				uint32 pAttributeBaseIndex,
				VertexAttributeSemantics pSemantics,
				GCI::EVertexAttribFormat pAttributeBaseFormat,
				uint32 pAttributeComponentsNum,
				uint32 pStreamIndex,
				TAttribute TVertex::* pAttributePtr,
				EVertexDataRate pAttributeDataRate = EVertexDataRate::PerVertex );

		template <typename TVertex, typename TAttribute>
		bool configureCustomAttributeAuto(
				uint32 pAttributeBaseIndex,
				VertexAttributeSemantics pSemantics,
				uint32 pStreamIndex,
				TAttribute TVertex::* pAttributePtr,
				EVertexDataRate pAttributeDataRate = EVertexDataRate::PerVertex );

		IC3_ATTR_NO_DISCARD GCI::IAInputLayoutDefinition generateGpaInputLayoutDefinition() const noexcept;

	private:
		template <typename TAttribute>
		IC3_ATTR_NO_DISCARD bool checkAttributeAutoDataFormat(
				GCI::EVertexAttribFormat pAttributeComponentFormat,
				uint16 pAttributeComponentsNum ) const noexcept;

		IC3_ATTR_NO_DISCARD bool checkAttributeDefinitionParams(
				uint32 pAttributeBaseIndex,
				uint32 pAttributeComponentsNum,
				uint32 pStreamIndex,
				EVertexDataRate pAttributeDataRate );

		IC3_ATTR_NO_DISCARD bool checkAttributeSlotRangeFree(
				uint32 pAttributeBaseIndex,
				uint32 pAttributeComponentsNum ) const noexcept;

		IC3_ATTR_NO_DISCARD bool checkAttributeVertexStreamCompatibility(
				uint32 pStreamIndex,
				EVertexDataRate pAttributeDataRate ) const noexcept;

		void updateStateWithNewAttribute( uint32 pNewAttributeIndex );

		IC3_ATTR_NO_DISCARD bool resolveAttributeSemanticsDefinition(
				VertexAttributeSemantics & pAttributeSemantics );

	private:
		VertexAttributeArrayLayoutBase * _attribLayoutPtr;
		VertexStreamArrayLayoutBase * _streamLayoutPtr;
		GCI::EIndexDataFormat _indexDataFormat;
		GCI::EPrimitiveTopology _primitiveTopology;
	};

	template <size_t tVertexAttributeArraySize = 0, size_t tVertexStreamArraySize = 0>
	class GeometryDataFormat : public GeometryDataFormatBase
	{
	public:
		using VertexAttributeArrayLayoutType = VertexAttributeArrayLayout<tVertexAttributeArraySize>;
		using VertexStreamArrayLayoutType = VertexStreamArrayLayout<tVertexStreamArraySize>;

		GeometryDataFormat()
		: GeometryDataFormatBase( &_attribLayout, &_streamLayout )
		{}

	private:
		VertexAttributeArrayLayoutType _attribLayout;
		VertexStreamArrayLayoutType _streamLayout;
	};

	template <typename TVertex, typename TAttribute>
	inline bool GeometryDataFormatBase::configureFixedAttribute(
			EFixedVertexAttributeID pFixedAttributeID,
			uint32 pStreamIndex,
			TAttribute TVertex::* pAttributePtr )
	{
		const auto fixedAttributeFormat = CxDefs::getFixedVertexAttributeBaseFormat( pFixedAttributeID );
		const auto fixedAttributeComponentsNum = CxDefs::getFixedVertexAttributeComponentsNum( pFixedAttributeID );
		if( !checkAttributeAutoDataFormat<TAttribute>( fixedAttributeFormat, fixedAttributeComponentsNum ) )
		{
			return false;
		}

		return configureFixedAttribute(
				pFixedAttributeID,
				pStreamIndex,
				memberOffset( pAttributePtr ) );
	}

	template <typename TVertex, typename TAttribute>
	inline bool GeometryDataFormatBase::configureCustomAttribute(
			uint32 pAttributeBaseIndex,
			VertexAttributeSemantics pSemantics,
			GCI::EVertexAttribFormat pAttributeBaseFormat,
			uint32 pAttributeComponentsNum,
			uint32 pStreamIndex,
			TAttribute TVertex::* pAttributePtr,
			EVertexDataRate pAttributeDataRate )
	{
		if( !checkAttributeAutoDataFormat<TAttribute>( pAttributeBaseFormat, pAttributeComponentsNum ) )
		{
			return false;
		}

		return configureCustomAttribute(
				pAttributeBaseIndex,
				std::move( pSemantics ),
				pAttributeBaseFormat,
				pAttributeComponentsNum,
				pStreamIndex,
				memberOffset( pAttributePtr ),
				pAttributeDataRate );
	}

	template <typename TVertex, typename TAttribute>
	inline bool GeometryDataFormatBase::configureCustomAttributeAuto(
			uint32 pAttributeBaseIndex,
			VertexAttributeSemantics pSemantics,
			uint32 pStreamIndex,
			TAttribute TVertex::* pAttributePtr,
			EVertexDataRate pAttributeDataRate )
	{
		using AttributeTraits = GCI::VertexAttribFormatDataTypeTraits<TAttribute>;
		if( !checkAttributeAutoDataFormat<TAttribute>( AttributeTraits::sVertexAttribFormat, AttributeTraits::sVertexAttribComponentsNum ) )
		{
			return false;
		}

		return configureCustomAttribute(
				pAttributeBaseIndex,
				std::move( pSemantics ),
				AttributeTraits::sVertexAttribFormat,
				AttributeTraits::sVertexAttribComponentsNum,
				pStreamIndex,
				memberOffset( pAttributePtr ),
				pAttributeDataRate );
	}

	template <typename TAttribute>
	inline bool GeometryDataFormatBase::checkAttributeAutoDataFormat(
			GCI::EVertexAttribFormat pAttributeComponentFormat,
			uint16 pAttributeComponentsNum ) const noexcept
	{
		const auto componentByteSize = GCI::CxDefs::getVertexAttribFormatByteSize( pAttributeComponentFormat );
		const auto attributeByteSize = componentByteSize * pAttributeComponentsNum;

		return sizeof( TAttribute ) == attributeByteSize;
	}

	namespace gpa
	{

		IC3_ATTR_NO_DISCARD const char * getStandardVertexAttributeSemanticsName( EVertexAttributeSemanticsID pSemanticsID );

		IC3_ATTR_NO_DISCARD std::string generateVertexFormatStringID( const VertexAttributeFormatArray & pAttributeArray );

	}


} // namespace Ic3

#endif // __IC3_NXMAIN_GEOMETRY_DATA_FORMAT_H__
