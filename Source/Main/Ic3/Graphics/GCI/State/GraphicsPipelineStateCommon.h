
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_COMMON_H__
#define __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_COMMON_H__

#include "CommonGraphicsConfig.h"
#include "GraphicsShaderDefs.h"
#include "InputAssemblerCommon.h"
#include "PipelineStateCommon.h"
#include "RenderTargetCommon.h"
#include "RootSignature.h"

namespace Ic3::Graphics::GCI
{

	struct GraphicsPipelineStateObjectCreateInfo
	{
		pipeline_state_object_id_t baseObjectID = kPipelineStateObjectIDAuto;

		pipeline_state_descriptor_id_t blendStateDescriptorID;
		pipeline_state_descriptor_id_t depthStencilStateDescriptorID;
		pipeline_state_descriptor_id_t rasterizerStateDescriptorID;
		pipeline_state_descriptor_id_t rootSignatureDescriptorID;
		pipeline_state_descriptor_id_t shaderLinkageStateDescriptorID;
		pipeline_state_descriptor_id_t vertexAttributeLayoutDescriptorID;

		mutable BlendStateDescriptorHandle blendStateDescriptor;
		mutable DepthStencilStateDescriptorHandle depthStencilStateDescriptor;
		mutable RasterizerStateDescriptorHandle rasterizerStateDescriptor;
		mutable RootSignatureDescriptorHandle rootSignatureDescriptor;
		mutable GraphicsShaderLinkageDescriptorHandle shaderLinkageStateDescriptor;
		mutable VertexAttributeLayoutDescriptorHandle vertexAttributeLayoutDescriptor;

		RenderTargetLayout renderTargetLayout;
	};

	struct GraphicsPipelineStateObjectDescriptorIDSet
	{
		pipeline_state_descriptor_id_t descIDVertexAttributeLayout = kPipelineStateDescriptorIDInvalid;
		pipeline_state_descriptor_id_t descIDShaderLinkage = kPipelineStateDescriptorIDInvalid;
		pipeline_state_descriptor_id_t descIDRootSignature = kPipelineStateDescriptorIDInvalid;
		pipeline_state_descriptor_id_t descIDBlend = kPipelineStateDescriptorIDInvalid;
		pipeline_state_descriptor_id_t descIDDepthStencil = kPipelineStateDescriptorIDInvalid;
		pipeline_state_descriptor_id_t descIDRasterizer = kPipelineStateDescriptorIDInvalid;
		pipeline_state_descriptor_id_t _padding[2]{};
	};

	/**
	 *
	 */
	class IC3_GRAPHICS_GCI_CLASS GraphicsPipelineStateObject : public PipelineStateObject
	{
	public:
		RenderTargetLayout const mRenderTargetLayout;

		GraphicsPipelineStateObjectDescriptorIDSet const mDescriptorsIDSet;

	public:
		GraphicsPipelineStateObject(
				GPUDevice & pGPUDevice,
				pipeline_state_object_id_t pStateObjectID,
				const RenderTargetLayout & pRenderTargetLayout,
				const GraphicsPipelineStateObjectDescriptorIDSet & pDescriptorsIDSet );

		virtual ~GraphicsPipelineStateObject();

		/**
		 *
		 */
		CPPX_ATTR_NO_DISCARD virtual EPipelineType GetPipelineType() const noexcept override final;

		/**
		 *
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD const GraphicsPipelineStateObjectDescriptorIDSet & GetDescriptorIDSet() const noexcept;

	private:
		GraphicsPipelineStateObjectDescriptorIDSet _descriptorIDSet;
	};

	inline const GraphicsPipelineStateObjectDescriptorIDSet & GraphicsPipelineStateObject::GetDescriptorIDSet() const noexcept
	{
		return  _descriptorIDSet;
	}


	inline bool operator==( const GraphicsPipelineStateObjectDescriptorIDSet & pLhs, const GraphicsPipelineStateObjectDescriptorIDSet & pRhs ) noexcept
	{
		return cppx::mem_cmp_equal( pLhs, pRhs );
	}

	inline bool operator!=( const GraphicsPipelineStateObjectDescriptorIDSet & pLhs, const GraphicsPipelineStateObjectDescriptorIDSet & pRhs ) noexcept
	{
		return cppx::mem_cmp_not_equal( pLhs, pRhs );
	}

	inline bool operator<( const GraphicsPipelineStateObjectDescriptorIDSet & pLhs, const GraphicsPipelineStateObjectDescriptorIDSet & pRhs ) noexcept
	{
		return cppx::mem_cmp_less( pLhs, pRhs );
	}

	inline bool operator<=( const GraphicsPipelineStateObjectDescriptorIDSet & pLhs, const GraphicsPipelineStateObjectDescriptorIDSet & pRhs ) noexcept
	{
		return cppx::mem_cmp_less_equal( pLhs, pRhs );
	}

	inline bool operator>( const GraphicsPipelineStateObjectDescriptorIDSet & pLhs, const GraphicsPipelineStateObjectDescriptorIDSet & pRhs ) noexcept
	{
		return cppx::mem_cmp_greater( pLhs, pRhs );
	}

	inline bool operator>=( const GraphicsPipelineStateObjectDescriptorIDSet & pLhs, const GraphicsPipelineStateObjectDescriptorIDSet & pRhs ) noexcept
	{
		return cppx::mem_cmp_greater_equal( pLhs, pRhs );
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_COMMON_H__
