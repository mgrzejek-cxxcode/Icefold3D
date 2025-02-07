
#pragma once

#ifndef __IC3_GRAPHICS_GCI_INPUT_ASSEMBLER_IMMUTABLE_STATES_H__
#define __IC3_GRAPHICS_GCI_INPUT_ASSEMBLER_IMMUTABLE_STATES_H__

#include "PipelineStateCommon.h"
#include "InputAssemblerCommon.h"

namespace Ic3::Graphics::GCI
{

	/**
	 * Represents an IA input layout state (vertex attribute layout) for the graphics pipeline.
	 * This descriptor is always an API-specific one (compiled), so there are no Compiled/Dynamic sub-classes.
	 */
	class IAInputLayoutStateDescriptor : public PipelineStateDescriptor
	{
	public:
		const IAInputLayoutCommonConfig & mCommonInputLayoutConfig;

	public:
		Ic3DeclareNonCopyable( IAInputLayoutStateDescriptor );

		IAInputLayoutStateDescriptor(
				GPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID,
				const IAInputLayoutCommonConfig & pCommonInputLayoutConfig );

		virtual ~IAInputLayoutStateDescriptor();

		CPPX_ATTR_NO_DISCARD virtual EPipelineStateDescriptorType GetDescriptorType() const noexcept override final;
	};

	/**
	 * Represents an IA input data stream state (vertex/index buffer binding) for the graphics pipeline.
	 * This descriptor can be either optimized at the API level or specified as dynamic state.
	 * @see IAVertexStreamStateDescriptorNative
	 * @see IAVertexStreamStateDescriptorDynamic
	 */
	class IAVertexStreamStateDescriptor : public PipelineStateDescriptor
	{
	public:
		const IAVertexStreamCommonConfig & mCommonVertexStreamConfig;

	public:
		Ic3DeclareNonCopyable( IAVertexStreamStateDescriptor );

		IAVertexStreamStateDescriptor(
				GPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID,
				const IAVertexStreamCommonConfig & pCommonVertexStreamConfig );

		virtual ~IAVertexStreamStateDescriptor();

		CPPX_ATTR_NO_DISCARD virtual EPipelineStateDescriptorType GetDescriptorType() const noexcept override final;
	};

	/**
	 * Represents an IA input data stream state which is fully API-agnostic (plain GCI representation) and can be changed
	 * during the runtime directly on the client side (no GPU translation is involved). Dynamic descriptors are translated
	 * on-the-fly during command execution which gives additional flexibility for the minor cost of performance.
	 * @see IAVertexStreamStateDescriptor
	 */
	class IC3_GRAPHICS_GCI_CLASS IAVertexStreamStateDescriptorDynamic : public IAVertexStreamStateDescriptor
	{
	public:
		Ic3DeclareNonCopyable( IAVertexStreamStateDescriptorDynamic );

		IAVertexStreamStateDescriptorDynamic(
				GPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID = kPipelineStateDescriptorIDAuto );

		~IAVertexStreamStateDescriptorDynamic();

		/**
		 * @see PipelineStateDescriptor::IsDynamicDescriptor()
		 */
		CPPX_ATTR_NO_DISCARD virtual bool IsDynamicDescriptor() const noexcept override final;

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsIndexBufferActive() const noexcept;

		CPPX_ATTR_NO_DISCARD native_uint CountActiveVertexBuffers() const noexcept;

		CPPX_ATTR_NO_DISCARD const IAVertexStreamConfiguration & GetStreamArrayConfiguration() const noexcept;

		IAVertexBufferReference * UpdateActiveVertexBuffer( native_uint pVertexBufferIndex );

		IAIndexBufferReference * UpdateActiveIndexBuffer();

		void SetStreamArrayConfiguration( const IAVertexStreamConfiguration & pStreamArrayConfiguration );

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

		TGfxHandle<IAVertexStreamStateDescriptorDynamic> CreateFromStreamArrayDefinition(
				const IAVertexStreamConfiguration & pStreamArrayDefinition );

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
		IAVertexStreamConfiguration _vertexStreamConfiguration;
	};

	/// namespace PIM: Private Implementation
	namespace PIM
	{

		/**
		 * Represents an IA input data stream state which has been pre-processed and compiled into the API-specific representation.
		 * @see IAVertexStreamStateDescriptor
		 */
		class IC3_GRAPHICS_GCI_CLASS IAInputLayoutStateDescriptorNative : public IAInputLayoutStateDescriptor
		{
			Ic3DeclareNonCopyable( IAInputLayoutStateDescriptorNative );

		public:
			IAInputLayoutStateDescriptorNative(
					GPUDevice & pGPUDevice,
					pipeline_state_descriptor_id_t pDescriptorID,
					const IAInputLayoutCommonConfig & pCommonInputLayoutConfig );

			virtual ~IAInputLayoutStateDescriptorNative();
		};

		/**
		 * Represents an IA input data stream state which has been pre-processed and compiled into the API-specific representation.
		 * @see IAVertexStreamStateDescriptor
		 */
		class IC3_GRAPHICS_GCI_CLASS IAVertexStreamStateDescriptorNative : public IAVertexStreamStateDescriptor
		{
			Ic3DeclareNonCopyable( IAVertexStreamStateDescriptorNative );

		public:
			IAVertexStreamStateDescriptorNative(
					GPUDevice & pGPUDevice,
					pipeline_state_descriptor_id_t pDescriptorID,
					const IAVertexStreamCommonConfig & pCommonVertexStreamConfig );

			virtual ~IAVertexStreamStateDescriptorNative();
		};

	} // namespace PIM

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_INPUT_ASSEMBLER_IMMUTABLE_STATES_H__
