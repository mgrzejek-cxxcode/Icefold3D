
#pragma once

#ifndef __IC3_NXMAIN_VERTEX_PIPELINE_LAYOUTS_H__
#define __IC3_NXMAIN_VERTEX_PIPELINE_LAYOUTS_H__

#include "VertexPipelineDefs.h"

namespace Ic3
{

	/// @brief
	class IC3_NXMAIN_CLASS CVertexAttributeArrayLayout
	{
	public:
		const SVertexAttributeArrayLayoutData & mLayoutData;

	public:
		CVertexAttributeArrayLayout();

		CVertexAttributeArrayLayout( CVertexAttributeArrayLayout && pSource ) noexcept;
		CVertexAttributeArrayLayout & operator=( CVertexAttributeArrayLayout && pRhs ) noexcept;

		CVertexAttributeArrayLayout( const CVertexAttributeArrayLayout & pSource );
		CVertexAttributeArrayLayout & operator=( const CVertexAttributeArrayLayout & pRhs );

		~CVertexAttributeArrayLayout();

		SGenericVertexAttributeInfo & operator[]( size_t pAttributeIndex ) noexcept;
		IC3_ATTR_NO_DISCARD const SGenericVertexAttributeInfo & operator[]( size_t pAttributeIndex ) const noexcept;

		IC3_ATTR_NO_DISCARD explicit operator bool() const noexcept;

		IC3_ATTR_NO_DISCARD bool empty() const noexcept;

		IC3_ATTR_NO_DISCARD SVertexAttributeArrayLayoutData & getLayoutData() noexcept;
		IC3_ATTR_NO_DISCARD const SVertexAttributeArrayLayoutData & getLayoutData() const noexcept;

		IC3_ATTR_NO_DISCARD uint32 resolveAttributeRef( std::string_view pSemanticName, uint32 pSubIndex = 0 ) const noexcept;
		IC3_ATTR_NO_DISCARD uint32 resolveAttributeRef( EShaderInputSemanticID pSemanticID ) const noexcept;
		IC3_ATTR_NO_DISCARD uint32 resolveAttributeRef( EStandardVertexAttributeID pAttributeID ) const noexcept;
		IC3_ATTR_NO_DISCARD uint32 resolveAttributeRef( const SShaderSemantics & pSemantics ) const noexcept;

		IC3_ATTR_NO_DISCARD const SGenericVertexAttributeInfo * getAttribute( uint32 pAttributeIndex ) const noexcept;
		IC3_ATTR_NO_DISCARD const SGenericVertexAttributeInfo * getAttribute( std::string_view pSemanticName, uint32 pSubIndex = 0 ) const noexcept;
		IC3_ATTR_NO_DISCARD const SGenericVertexAttributeInfo * getAttribute( EShaderInputSemanticID pSemanticID ) const noexcept;
		IC3_ATTR_NO_DISCARD const SGenericVertexAttributeInfo * getAttribute( EStandardVertexAttributeID pAttributeID ) const noexcept;
		IC3_ATTR_NO_DISCARD const SGenericVertexAttributeInfo * getAttribute( const SShaderSemantics & pSemantics ) const noexcept;

		IC3_ATTR_NO_DISCARD ArrayView<const SGenericVertexAttributeInfo> getActiveAttributesRange() const noexcept;

		IC3_ATTR_NO_DISCARD std::string generateVertexFormatStringID() const noexcept;

		IC3_ATTR_NO_DISCARD std::vector<SVertexAttributeDefinition> generateAttributeDefinitionArray() const noexcept;

		IC3_ATTR_NO_DISCARD bool checkAttributeSlotRangeFree(
				uint32 pAttributeBaseIndex,
				uint32 pAttributeComponentsNum ) const noexcept;

		IC3_ATTR_NO_DISCARD bool validateAttributeDefinition(
				SVertexAttributeDefinition & pAttributeDefinition,
				uint32 pComponentStrideInBytes = 0,
				uint32 pCombinedAttributeDataStride = 0 ) const noexcept;

		SGenericVertexAttributeInfo * setAttribute( SVertexAttributeDefinition pAttributeDefinition );

		SGenericVertexAttributeInfo * setAttributeUnchecked( SVertexAttributeDefinition pAttributeDefinition );

		SGenericVertexAttributeInfo * setStandardAttribute(
				EStandardVertexAttributeID pAttributeID,
				uint32 pStreamIndex,
				uint32 pStreamRelativeOffset,
				uint32 pDataStride = 0 );

		void reset();

	private:
		SGenericVertexAttributeInfo & _updateBaseAttribute(
				SVertexAttributeDefinition & pAttributeDefinition,
				uint32 pComponentStrideInBytes );

		void _updateSubComponents(
				SGenericVertexAttributeInfo & pBaseAttribute,
				uint32 pAttributeComponentsNum,
				uint32 pComponentStrideInBytes );

	private:
		SVertexAttributeArrayLayoutData _layoutData;
	};

	/// @brief
	class IC3_NXMAIN_CLASS CVertexStreamArrayLayout
	{
	public:
		const SVertexStreamArrayLayoutData & mLayoutData;

	public:
		CVertexStreamArrayLayout();

		CVertexStreamArrayLayout( CVertexStreamArrayLayout && pSource ) noexcept;
		CVertexStreamArrayLayout & operator=( CVertexStreamArrayLayout && pRhs ) noexcept;

		CVertexStreamArrayLayout( const CVertexStreamArrayLayout & pSource );
		CVertexStreamArrayLayout & operator=( const CVertexStreamArrayLayout & pRhs );

		~CVertexStreamArrayLayout();

		SVertexStreamInfo & operator[]( size_t pStreamIndex ) noexcept;
		IC3_ATTR_NO_DISCARD const SVertexStreamInfo & operator[]( size_t pStreamIndex ) const noexcept;

		IC3_ATTR_NO_DISCARD explicit operator bool() const noexcept;

		IC3_ATTR_NO_DISCARD SVertexStreamArrayLayoutData & getLayoutData() noexcept;
		IC3_ATTR_NO_DISCARD const SVertexStreamArrayLayoutData & getLayoutData() const noexcept;

		IC3_ATTR_NO_DISCARD bool empty() const noexcept;

		IC3_ATTR_NO_DISCARD const SVertexStreamInfo * getStream( uint32 pStreamIndex ) const noexcept;

		IC3_ATTR_NO_DISCARD ArrayView<const SVertexStreamInfo> getActiveStreamsRange() const noexcept;

		bool reset();

	private:
		SVertexStreamArrayLayoutData _layoutData;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_VERTEX_PIPELINE_LAYOUTS_H__
