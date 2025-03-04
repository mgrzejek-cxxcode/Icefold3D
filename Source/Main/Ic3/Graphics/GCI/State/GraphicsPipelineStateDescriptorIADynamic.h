
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_DESCRIPTOR_IA_DYNAMIC_H__
#define __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_DESCRIPTOR_IA_DYNAMIC_H__

#include "GraphicsPipelineStateDescriptorIA.h"
#include "PipelineStateDescriptorDynamic.h"

namespace Ic3::Graphics::GCI
{

	/**
	 * Represents an IA input data stream state which is fully API-agnostic (plain GCI representation) and can be changed
	 * during the runtime directly on the client side (no GPU translation is involved). Dynamic descriptors are translated
	 * on-the-fly during command execution which gives additional flexibility for the minor cost of performance.
	 * @see VertexSourceBindingDescriptor
	 */
	class IC3_GRAPHICS_GCI_CLASS VertexSourceBindingDescriptorDynamic : public PipelineStateDescriptorDynamic<VertexSourceBindingDescriptor>
	{
	public:
		Ic3DeclareNonCopyable( VertexSourceBindingDescriptorDynamic );

		VertexSourceBindingDescriptorDynamic( GPUDevice & pGPUDevice );

		~VertexSourceBindingDescriptorDynamic();

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsIndexBufferActive() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsVertexBufferActive( native_uint pVertexBufferIndex ) const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsBufferActive( native_uint pBufferIndex ) const noexcept;

		CPPX_ATTR_NO_DISCARD native_uint CountActiveVertexBuffers() const noexcept;

		CPPX_ATTR_NO_DISCARD const IAVertexSourceBindingDefinition & GetVertexSourceBindingDefinition() const noexcept;

		IAVertexBufferReference * UpdateActiveVertexBuffer( native_uint pVertexBufferIndex );

		IAIndexBufferReference * UpdateActiveIndexBuffer();

		void SetStreamArrayConfiguration( const IAVertexSourceBindingDefinition & pStreamArrayConfiguration );

		void SetVertexBufferReference(
				native_uint pVertexBufferIndex,
				const IAVertexBufferReference & pBufferReference );

		void SetVertexBufferReferences(
				const cppx::array_view<const IAVertexBufferReference> & pBufferReferences,
				native_uint pVertexBufferFirstIndex = 0,
				native_uint pVertexBufferCount = cppx::cve::native_uint_max );

		void SetIndexBufferReference( const IAIndexBufferReference & pBufferReference );

		void ResetAll();

		void ResetAllFlags();

		void ResetVertexBufferReference( native_uint pVertexBufferIndex );

		void ResetVertexBufferReferences(
				native_uint pVertexBufferFirstIndex = 0,
				native_uint pVertexBufferCount = cppx::cve::native_uint_max );

		void ResetIndexBufferReference();

		static TGfxHandle<VertexSourceBindingDescriptorDynamic> CreateNew( GPUDevice & pGPUDevice );

	private:
		IAVertexBufferReference * _SetVertexBufferActive( native_uint pVertexBufferIndex );

		void _SetVertexBufferReferences(
				native_uint pVertexBufferFirstIndex,
				native_uint pVertexBufferCount,
				const IAVertexBufferReference * pBufferReferences );

		IAIndexBufferReference * _SetIndexBufferActive();

		void _SetIndexBufferReference( const IAIndexBufferReference & pBufferReference );

		void _ResetVertexBufferReferences( native_uint pVertexBufferFirstIndex, native_uint pVertexBufferCount );

		void _ResetIndexBufferReference();

	private:
		IAVertexSourceBindingDefinition _vertexSourceBinding;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_DESCRIPTOR_IA_DYNAMIC_H__
