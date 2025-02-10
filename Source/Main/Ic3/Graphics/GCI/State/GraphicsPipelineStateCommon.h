
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_COMMON_H__
#define __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_COMMON_H__

#include "CommonGraphicsConfig.h"
#include "GraphicsShaderDefs.h"
#include "InputAssemblerCommon.h"
#include "PipelineStateCommon.h"
#include "RenderTargetCommon.h"
#include "ShaderRootSignature.h"

namespace Ic3::Graphics::GCI
{

	struct GraphicsPipelineStateObjectCreateInfo
	{
		pipeline_state_descriptor_id_t blendStateDescriptorID;
		pipeline_state_descriptor_id_t depthStencilStateDescriptorID;
		pipeline_state_descriptor_id_t rasterizerStateDescriptorID;
		pipeline_state_descriptor_id_t shaderLinkageStateDescriptorID;
		pipeline_state_descriptor_id_t vertexAttributeLayoutStateDescriptorID;
		pipeline_state_descriptor_id_t shaderRootSignatureStateDescriptorID;

		BlendStateDescriptorHandle blendStateDescriptor;
		DepthStencilStateDescriptorHandle depthStencilStateDescriptor;
		RasterizerStateDescriptorHandle rasterizerStateDescriptor;
		GraphicsShaderLinkageStateDescriptorHandle shaderLinkageStateDescriptor;
		IAVertexAttributeLayoutStateDescriptorHandle vertexAttributeLayoutStateDescriptor;
		ShaderRootSignatureStateDescriptorHandle shaderRootSignatureStateDescriptor;

		RenderTargetArrayLayout renderTargetLayout;
	};

	struct GraphicsPipelineStateObjectDescriptorIDSet
	{
		pipeline_state_descriptor_id_t descIDIAVertexAttributeLayout = kPipelineStateDescriptorIDInvalid;
		pipeline_state_descriptor_id_t descIDGraphicsShaderLinkage = kPipelineStateDescriptorIDInvalid;
		pipeline_state_descriptor_id_t descIDShaderRootSignature = kPipelineStateDescriptorIDInvalid;
		pipeline_state_descriptor_id_t descIDBlend = kPipelineStateDescriptorIDInvalid;
		pipeline_state_descriptor_id_t descIDDepthStencil = kPipelineStateDescriptorIDInvalid;
		pipeline_state_descriptor_id_t descIDRasterizer = kPipelineStateDescriptorIDInvalid;
		pipeline_state_descriptor_id_t _padding[2]{};
	};

	/// @brief PSO for the graphics pipeline.
	class IC3_GRAPHICS_GCI_CLASS GraphicsPipelineStateObject : public PipelineStateObject
	{
	public:
		GraphicsPipelineStateObject(
				GPUDevice & pGPUDevice,
				pipeline_state_object_id_t pStateObjectID );

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
