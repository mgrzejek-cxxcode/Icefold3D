
#pragma once

#ifndef __IC3_NXMAIN_GEOMETRY_VERTEX_FORMAT_H__
#define __IC3_NXMAIN_GEOMETRY_VERTEX_FORMAT_H__

#include "GeometryCommonDefs.h"

namespace Ic3
{

	enum class EVertexDataRate : enum_default_value_t
	{
		Undefined,
		PerInstance,
		PerVertex
	};

	struct VertexAttributeSemantics
	{
		EVertexAttributeSemanticsID semID;
		std::string semName;

		VertexAttributeSemantics()
		: semID( EVertexAttributeSemanticsID::Undefined )
		{}

		VertexAttributeSemantics( EVertexAttributeSemanticsID pSemanticsID )
		: semID( pSemanticsID )
		{}

		VertexAttributeSemantics( std::string pSemanticsName )
		: semID( EVertexAttributeSemanticsID::Custom )
		, semName( std::move( pSemanticsName ) )
		{}
	};

	struct VertexAttributeFormat
	{
		VertexAttributeFormat * nextComponent = nullptr;
		GCI::EVertexAttribFormat componentFormat = GCI::EVertexAttribFormat::Undefined;
		VertexAttributeSemantics semantics;
		uint16 componentsNum;
		uint16 componentSizeInBytes;
		uint16 attributeTotalSizeInBytes;
		uint16 streamIndex;
		uint16 streamElementRelativeOffset;
		uint16 instanceRate;

		bool active() const noexcept
		{
			return componentFormat != GCI::EVertexAttribFormat::Undefined;
		}

		bool isBaseAttribute() const noexcept
		{
			return active() && !semantics.semName.empty();
		}

		EVertexDataRate getAttributeDataRate() const noexcept
		{
			return ( instanceRate != 0 ) ? EVertexDataRate::PerInstance : EVertexDataRate::PerVertex;
		}
	};

	struct VertexStreamFormat
	{
		EVertexDataRate streamDataRate = EVertexDataRate::Undefined;

		Bitmask<GCI::EIAVertexAttributeFlags> activeAttributesMask;

		uint16 activeAttributesNum = 0;

		uint16 elementSizeInBytes = 0;

		uint32 strideInBytes = 0;

		bool active() const noexcept
		{
			return elementSizeInBytes > 0;
		}
	};

	struct VertexAttributeArrayLayoutBase
	{
		using AttributeSemanticsMap = std::unordered_map<std::string_view, uint32>;

		uint32 attributeArraySize = 0;

		VertexAttributeFormat * attributeArrayPtr = nullptr;

		uint32 activeAttributesNum = 0;

		uint32 activeAttributeSlotsNum = 0;

		Bitmask<GCI::EIAVertexAttributeFlags> activeAttributesMask;

		Bitmask<EVertexAttributeSemanticsFlags> activeAttributeSemanticsMask;

		AttributeSemanticsMap semanticsMap;

		VertexAttributeFormat & attributeFormat( size_t pAttributeIndex ) noexcept
		{
			ic3DebugAssert( attributeArrayPtr && ( attributeArraySize > 0 ) );
			ic3DebugAssert( pAttributeIndex < attributeArraySize );
			return attributeArrayPtr[pAttributeIndex];
		}

		const VertexAttributeFormat & attributeFormat( size_t pAttributeIndex ) const noexcept
		{
			ic3DebugAssert( attributeArrayPtr && ( attributeArraySize > 0 ) );
			ic3DebugAssert( pAttributeIndex < attributeArraySize );
			return attributeArrayPtr[pAttributeIndex];
		}
	};

	template <size_t tVertexAttributeArraySize = 0>
	struct VertexAttributeArrayLayout : public VertexAttributeArrayLayoutBase
	{
		using VertexAttributeFormatArray = std::array<VertexAttributeFormat, tVertexAttributeArraySize>;

		VertexAttributeFormatArray attributeArray;

		VertexAttributeArrayLayout()
		{
			attributeArrayPtr = attributeArray.data();
			attributeArraySize = tVertexAttributeArraySize;
		}
	};

	template <>
	struct VertexAttributeArrayLayout<0> : public VertexAttributeArrayLayoutBase
	{
		using VertexAttributeFormatArray = std::vector<VertexAttributeFormat>;

		VertexAttributeFormatArray attributeArray;

		VertexAttributeArrayLayout( size_t pVertexAttributeArraySize )
		{
			ic3DebugAssert( pVertexAttributeArraySize > 0 );
			ic3DebugAssert( pVertexAttributeArraySize <= gpa::MAX_GEOMETRY_VERTEX_ATTRIBUTES_NUM );
			attributeArray.resize( pVertexAttributeArraySize );
			attributeArrayPtr = attributeArray.data();
			attributeArraySize = pVertexAttributeArraySize;
		}
	};

	struct VertexStreamArrayLayoutBase
	{
		using VertexStreamIndexArray = std::vector<uint16>;

		uint32 vertexStreamArraySize = 0;

		VertexStreamFormat * vertexStreamArrayPtr = nullptr;

		uint32 activeVertexStreamsNum = 0;

		uint16 * activeVertexStreamArrayPtr = nullptr;

		Bitmask<GCI::EIAVertexStreamBindingFlags> activeVertexStreamsMask;

		VertexStreamFormat & streamFormat( size_t pStreamIndex ) noexcept
		{
			ic3DebugAssert( vertexStreamArrayPtr && ( vertexStreamArraySize > 0 ) );
			ic3DebugAssert( pStreamIndex < vertexStreamArraySize );
			return vertexStreamArrayPtr[pStreamIndex];
		}

		const VertexStreamFormat & streamFormat( size_t pStreamIndex ) const noexcept
		{
			ic3DebugAssert( vertexStreamArrayPtr && ( vertexStreamArraySize > 0 ) );
			ic3DebugAssert( pStreamIndex < vertexStreamArraySize );
			return vertexStreamArrayPtr[pStreamIndex];
		}
	};

	template <size_t tVertexStreamArraySize = 0>
	struct VertexStreamArrayLayout : public VertexStreamArrayLayoutBase
	{
		using VertexStreamFormatArray = std::array<VertexStreamFormat, tVertexStreamArraySize>;
		using ActiveVertexStreamArray = std::array<uint16, tVertexStreamArraySize>;

		VertexStreamFormatArray vertexStreamArray;

		ActiveVertexStreamArray activeVertexStreamArray;

		VertexStreamArrayLayout()
		{
			vertexStreamArrayPtr = vertexStreamArray.data();
			vertexStreamArraySize = tVertexStreamArraySize;
			activeVertexStreamArrayPtr = activeVertexStreamArray.data();
		}
	};

	template <>
	struct VertexStreamArrayLayout<0> : public VertexStreamArrayLayoutBase
	{
		using VertexStreamFormatArray = std::vector<VertexStreamFormat>;
		using ActiveVertexStreamArray = std::vector<uint16>;

		VertexStreamFormatArray vertexStreamArray;

		ActiveVertexStreamArray activeVertexStreamArray;

		VertexStreamArrayLayout( size_t pVertexStreamArraySize )
		{
			ic3DebugAssert( pVertexStreamArraySize > 0 );
			ic3DebugAssert( pVertexStreamArraySize <= gpa::MAX_GEOMETRY_VERTEX_STREAMS_NUM );
			vertexStreamArray.resize( pVertexStreamArraySize );
			vertexStreamArrayPtr = vertexStreamArray.data();
			vertexStreamArraySize = pVertexStreamArraySize;
			activeVertexStreamArray.resize( pVertexStreamArraySize );
			activeVertexStreamArrayPtr = activeVertexStreamArray.data();
		}
	};

	struct P3SVertex
	{
		Math::Vec3f position;
		Math::Vec3f normal;
		Math::Vec4f color;
		Math::Vec2f texcoord0;
	};


} // namespace Ic3

#endif // __IC3_NXMAIN_GEOMETRY_VERTEX_FORMAT_H__
