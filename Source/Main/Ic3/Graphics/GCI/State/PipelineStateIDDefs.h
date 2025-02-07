
#pragma once

#ifndef __IC3_GRAPHICS_GCI_PIPELINE_STATE_ID_DEFS_H__
#define __IC3_GRAPHICS_GCI_PIPELINE_STATE_ID_DEFS_H__

#include "CommonGPUStateDefs.h"
#include <cppx/hash.h>

namespace Ic3::Graphics::GCI
{

	using pipeline_state_descriptor_id_t = uint16;
	using pipeline_state_object_id_t = uint64;

	inline constexpr auto kPipelineStateDescriptorIDAuto = static_cast<pipeline_state_descriptor_id_t>( 0u );

	inline constexpr auto kPipelineStateDescriptorIDInvalid = cppx::meta::limits<pipeline_state_descriptor_id_t>::max_value;

	struct GraphicsPipelineStateObjectDescriptorIDSet
	{
		pipeline_state_descriptor_id_t descIDIAInputLayout = kPipelineStateDescriptorIDInvalid;
		pipeline_state_descriptor_id_t descIDGraphicsShaderLinkage = kPipelineStateDescriptorIDInvalid;
		pipeline_state_descriptor_id_t descIDShaderRootSignature = kPipelineStateDescriptorIDInvalid;
		pipeline_state_descriptor_id_t descIDBlend = kPipelineStateDescriptorIDInvalid;
		pipeline_state_descriptor_id_t descIDDepthStencil = kPipelineStateDescriptorIDInvalid;
		pipeline_state_descriptor_id_t descIDRasterizer = kPipelineStateDescriptorIDInvalid;
		pipeline_state_descriptor_id_t descIDMultiSampling = kPipelineStateDescriptorIDInvalid;
		pipeline_state_descriptor_id_t _padding = 0;
	};

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

#endif // __IC3_GRAPHICS_GCI_PIPELINE_STATE_ID_DEFS_H__
