
#pragma once

#ifndef __IC3_NXMAIN_VERTEX_FORMAT_SIGNATURE_H__
#define __IC3_NXMAIN_VERTEX_FORMAT_SIGNATURE_H__

#include "VertexFormatCombinedLayout.h"
#include <cppx/hash.h>
#include <map>

namespace Ic3
{

	Ic3DeclareClassHandle( VertexFormatSignatureBase );
	
	/**
	 * @brief 
	 * This is a high-level equivalent of the Input Assembler (IA) primitives from the GCI with some additional functionalities that allow
	 * easy access to attribute/stream properties, their layout and dependencies.
	 * Each instance of this class defines the following properties of the IA stage:
	 * - layout/configuration of active vertex attributes,
	 * - layout/configuration of active vertex streams,
	 * - format of the index data (GCI::EIndexDataFormat),
	 * - primitive topology (GCI::EPrimitiveTopology).
	 * Note, that VertexFormatSignature does not describe the location of the vertex data - it only refers to how the data is laid out in
	 * memory, which attributes are fetched from which streams, what are their offsets, strides, rates at which they are read by the input
	 * pipeline and so on. Consequently, the vertex stream signature describes the usage of each stream, but not their data directly.
	 *
	 * One VertexFormatSignature can be used with multiple geometry buffer sets, as long as their internal layout matches the one described
	 * by the signature.
	 * @see VertexFormatAttributeArrayLayoutBase
	 * @see VertexFormatStreamArrayLayoutBase
	 * @see GCI::EIndexDataFormat
	 * @see GCI::EPrimitiveTopology
	 */
	class IC3_NXMAIN_CLASS VertexFormatSignatureBase : public IDynamicObject
	{
	public:
		/// Read-only reference to the format of the index data.
		const GCI::EIndexDataFormat & mIndexDataFormat;

		///
		const GCI::EPrimitiveTopology & mPrimitiveTopology;

		/// Read-only reference signature's layout object.
		const VertexFormatCombinedLayoutBase & mInternalLayout;

	public:
		/**
		 * @brief 
		 * @param pAttributeArrayLayout 
		 * @param pStreamArrayLayout 
		 * @param pIndexDataFormat 
		 * @param pPrimitiveTopology 
		 */
		VertexFormatSignatureBase( const VertexFormatCombinedLayoutBase & pInternalLayout );
		virtual ~VertexFormatSignatureBase();

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept;

		/**
		 * @brief Returns true if the layout is empty, i.e. contains no active attribute definition, or false otherwise.
		 * @return 
		 */
		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept;

		//
		CPPX_ATTR_NO_DISCARD bool IsValid() const noexcept;

		/**
		 * @brief Returns true if the layout describes an indexed geometry, or false otherwise.
		 * @return 
		 */
		CPPX_ATTR_NO_DISCARD bool IsIndexedVertexFormat() const noexcept;

		CPPX_ATTR_NO_DISCARD uint32 GetIndexDataSize() const noexcept;

		/**
		 * @brief 
		 * @param pAttributeSlot 
		 * @return 
		 */
		CPPX_ATTR_NO_DISCARD uint32 GetElementStrideForAttribute( native_uint pAttributeSlot ) const noexcept;
		CPPX_ATTR_NO_DISCARD uint32 GetElementStrideForAttribute( EVertexAttributeSemanticID pAttributeSemanticID ) const noexcept;
		CPPX_ATTR_NO_DISCARD uint32 GetElementStrideForAttribute( cppx::string_view pSemanticName ) const noexcept;

		/**
		 * @brief 
		 * @param pAttributeSlot 
		 * @return 
		 */
		CPPX_ATTR_NO_DISCARD uint32 GetElementStrideForAttributeUnchecked( native_uint pAttributeSlot ) const noexcept;
		CPPX_ATTR_NO_DISCARD uint32 GetElementStrideForAttributeUnchecked( EVertexAttributeSemanticID pAttributeSemanticID ) const noexcept;
		CPPX_ATTR_NO_DISCARD uint32 GetElementStrideForAttributeUnchecked( cppx::string_view pSemanticName ) const noexcept;

		/**
		 * @brief 
		 * @param pAttributeSemanticID 
		 * @return 
		 */
		CPPX_ATTR_NO_DISCARD bool HasAttributeWithSemantics( EVertexAttributeSemanticID pAttributeSemanticID ) const noexcept;
		CPPX_ATTR_NO_DISCARD bool HasAttributeWithSemantics( cppx::string_view pSemanticName ) const noexcept;

		/**
		 * @brief 
		 * @param pAttributeSlot 
		 * @return 
		 */
		CPPX_ATTR_NO_DISCARD const GenericVertexInputAttribute * GetAttribute( native_uint pAttributeSlot ) const noexcept;
		CPPX_ATTR_NO_DISCARD const GenericVertexInputAttribute * GetAttribute( EVertexAttributeSemanticID pAttributeSemanticID ) const noexcept;
		CPPX_ATTR_NO_DISCARD const GenericVertexInputAttribute * GetAttribute( cppx::string_view pSemanticName ) const noexcept;

		/**
		 * @brief 
		 * @param pAttributeSlot 
		 * @return 
		 */
		CPPX_ATTR_NO_DISCARD const VertexInputStream * GetStreamForAttribute( native_uint pAttributeSlot ) const noexcept;
		CPPX_ATTR_NO_DISCARD const VertexInputStream * GetStreamForAttribute( EVertexAttributeSemanticID pAttributeSemanticID ) const noexcept;
		CPPX_ATTR_NO_DISCARD const VertexInputStream * GetStreamForAttribute( cppx::string_view pSemanticName ) const noexcept;

		/**
		 * @brief 
		 * @param pStreamSlot 
		 * @return 
		 */
		CPPX_ATTR_NO_DISCARD const VertexInputStream * GetStream( native_uint pStreamSlot ) const noexcept;

		/**
		 * @brief 
		 * @return 
		 */
		CPPX_ATTR_NO_DISCARD cppx::array_view<input_assembler_slot_t> GetActiveAttributeSlots() const noexcept;

		/**
		 * @brief 
		 * @return 
		 */
		CPPX_ATTR_NO_DISCARD cppx::array_view<input_assembler_slot_t> GetActiveStreamSlots() const noexcept;

		/**
		 * @brief Returns true if the specified index is an index of an active vertex attribute, or false otherwise.
		 * @param pAttributeSlot 
		 * @return 
		 */
		CPPX_ATTR_NO_DISCARD bool IsAttributeActive( native_uint pAttributeSlot ) const noexcept;

		/**
		 * @brief Returns true if the specified index is an index of an active vertex stream, or false otherwise.
		 * @param pStreamSlot 
		 * @return 
		 */
		CPPX_ATTR_NO_DISCARD bool IsStreamActive( native_uint pStreamSlot ) const noexcept;

		CPPX_ATTR_NO_DISCARD bool Equals( const VertexFormatSignatureBase & pOther ) const noexcept;

		/**
		 * @brief Returns the string representation of this VertexFormatSignatureBase.
		 * @return 
		 */
		CPPX_ATTR_NO_DISCARD std::string GenerateSerialString() const noexcept;
		
		/**
		 * @brief  Returns an array of VertexAttributeDefinition values which can be used to re-create this signature.
		 * @return 
		 */
		CPPX_ATTR_NO_DISCARD std::vector<VertexInputAttributeDefinition> GenerateAttributeDefinitionArray() const noexcept;

		CPPX_ATTR_NO_DISCARD TSharedHandle<VertexFormatSignatureBase> MakeCopy() const;

		void SetIndexDataFormat( GCI::EIndexDataFormat pIndexDataFormat );

		void SetPrimitiveTopology( GCI::EPrimitiveTopology pPrimitiveTopology );

	protected:
		void UpdateSemanticMap();

		static void InitializeSignature(
				VertexFormatSignatureBase & pVertexFormatSignature,
				VertexFormatCombinedLayoutBase & pVertexFormatCombinedLayout,
				const cppx::array_view<VertexInputAttributeDefinition> & pAttributeDefinitions );

	private:
		// uint32 _ResolveAttributeRefImpl( EVertexAttributeSemanticID pAttributeSemanticID ) const noexcept;
		// uint32 _ResolveAttributeRefImpl( cppx::string_view pSemanticName ) const noexcept;

	private:
		using SemanticNameMap = std::map<cppx::string_view, uint32>;

		/// AAA
		GCI::EIndexDataFormat _indexDataFormat = GCI::EIndexDataFormat::Undefined;

		/// BBB
		GCI::EPrimitiveTopology _primitiveTopology = GCI::EPrimitiveTopology::Undefined;

		/// CCC
		SemanticNameMap _semanticNameMap;
	};


	template <native_uint tpAttributeArraySize, native_uint tpStreamArraySize>
	class VertexFormatSignature : public VertexFormatSignatureBase
	{
	public:
		using InternalLayoutType = VertexFormatCombinedLayout<tpAttributeArraySize, tpStreamArraySize>;

		VertexFormatSignature()
		: VertexFormatSignatureBase( _internalLayout )
		{}

		static  TSharedHandle<VertexFormatSignatureBase> CreateInstance(
				const cppx::array_view<VertexInputAttributeDefinition> & pAttributeDefinitions )
		{
			auto vertexFormatSignature = CreateDynamicObject<VertexFormatSignature<tpAttributeArraySize, tpStreamArraySize>>();
			InitializeSignature( *vertexFormatSignature, vertexFormatSignature->_internalLayout, pAttributeDefinitions );
			vertexFormatSignature->UpdateSemanticMap();
			return vertexFormatSignature;
		}

	private:
		InternalLayoutType _internalLayout;
	};


	namespace VertexFormat
	{

		template <native_uint tpAttributeArraySize>
		CPPX_ATTR_NO_DISCARD inline TSharedHandle<VertexFormatSignatureBase> CreateVertexFormatSignature(
				native_uint pStreamArraySize,
				const cppx::array_view<VertexInputAttributeDefinition> & pAttributeDefinitions )
		{
			switch( pStreamArraySize )
			{
				case 0:  return VertexFormatSignature<tpAttributeArraySize,  0>::CreateInstance( pAttributeDefinitions );
				case 1:  return VertexFormatSignature<tpAttributeArraySize,  1>::CreateInstance( pAttributeDefinitions );
				case 2:  return VertexFormatSignature<tpAttributeArraySize,  2>::CreateInstance( pAttributeDefinitions );
				case 3:  return VertexFormatSignature<tpAttributeArraySize,  3>::CreateInstance( pAttributeDefinitions );
				case 4:  return VertexFormatSignature<tpAttributeArraySize,  4>::CreateInstance( pAttributeDefinitions );
				case 5:  return VertexFormatSignature<tpAttributeArraySize,  5>::CreateInstance( pAttributeDefinitions );
				case 6:  return VertexFormatSignature<tpAttributeArraySize,  6>::CreateInstance( pAttributeDefinitions );
				case 7:  return VertexFormatSignature<tpAttributeArraySize,  7>::CreateInstance( pAttributeDefinitions );
				case 8:  return VertexFormatSignature<tpAttributeArraySize,  8>::CreateInstance( pAttributeDefinitions );
				case 9:  return VertexFormatSignature<tpAttributeArraySize,  9>::CreateInstance( pAttributeDefinitions );
				case 10: return VertexFormatSignature<tpAttributeArraySize, 10>::CreateInstance( pAttributeDefinitions );
				case 11: return VertexFormatSignature<tpAttributeArraySize, 11>::CreateInstance( pAttributeDefinitions );
				case 12: return VertexFormatSignature<tpAttributeArraySize, 12>::CreateInstance( pAttributeDefinitions );
				case 13: return VertexFormatSignature<tpAttributeArraySize, 13>::CreateInstance( pAttributeDefinitions );
				case 14: return VertexFormatSignature<tpAttributeArraySize, 14>::CreateInstance( pAttributeDefinitions );
				case 15: return VertexFormatSignature<tpAttributeArraySize, 15>::CreateInstance( pAttributeDefinitions );
				case 16: return VertexFormatSignature<tpAttributeArraySize, 16>::CreateInstance( pAttributeDefinitions );
			}

			return nullptr;
		}

		CPPX_ATTR_NO_DISCARD inline TSharedHandle<VertexFormatSignatureBase> CreateVertexFormatSignature(
				native_uint pAttributeArraySize,
				native_uint pStreamArraySize,
				const cppx::array_view<VertexInputAttributeDefinition> & pAttributeDefinitions )
		{
			switch( pAttributeArraySize )
			{
				case 0:  return CreateVertexFormatSignature< 0>( pStreamArraySize, pAttributeDefinitions );
				case 1:  return CreateVertexFormatSignature< 1>( pStreamArraySize, pAttributeDefinitions );
				case 2:  return CreateVertexFormatSignature< 2>( pStreamArraySize, pAttributeDefinitions );
				case 3:  return CreateVertexFormatSignature< 3>( pStreamArraySize, pAttributeDefinitions );
				case 4:  return CreateVertexFormatSignature< 4>( pStreamArraySize, pAttributeDefinitions );
				case 5:  return CreateVertexFormatSignature< 5>( pStreamArraySize, pAttributeDefinitions );
				case 6:  return CreateVertexFormatSignature< 6>( pStreamArraySize, pAttributeDefinitions );
				case 7:  return CreateVertexFormatSignature< 7>( pStreamArraySize, pAttributeDefinitions );
				case 8:  return CreateVertexFormatSignature< 8>( pStreamArraySize, pAttributeDefinitions );
				case 9:  return CreateVertexFormatSignature< 9>( pStreamArraySize, pAttributeDefinitions );
				case 10: return CreateVertexFormatSignature<10>( pStreamArraySize, pAttributeDefinitions );
				case 11: return CreateVertexFormatSignature<11>( pStreamArraySize, pAttributeDefinitions );
				case 12: return CreateVertexFormatSignature<12>( pStreamArraySize, pAttributeDefinitions );
				case 13: return CreateVertexFormatSignature<13>( pStreamArraySize, pAttributeDefinitions );
				case 14: return CreateVertexFormatSignature<14>( pStreamArraySize, pAttributeDefinitions );
				case 15: return CreateVertexFormatSignature<15>( pStreamArraySize, pAttributeDefinitions );
				case 16: return CreateVertexFormatSignature<16>( pStreamArraySize, pAttributeDefinitions );
			}

			return nullptr;
		}

		CPPX_ATTR_NO_DISCARD TSharedHandle<VertexFormatSignatureBase> CreateVertexFormatSignature(
				const cppx::array_view<VertexInputAttributeDefinition> & pAttributeDefinitions );


	}

} // namespace Ic3

#include "VertexFormatSignature.inl"

#endif // __IC3_NXMAIN_VERTEX_FORMAT_SIGNATURE_H__
