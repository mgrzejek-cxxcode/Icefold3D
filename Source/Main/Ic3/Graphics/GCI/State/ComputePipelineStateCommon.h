
#pragma once

#ifndef __IC3_GRAPHICS_GCI_COMPUTE_PIPELINE_STATE_OBJECT_H__
#define __IC3_GRAPHICS_GCI_COMPUTE_PIPELINE_STATE_OBJECT_H__

#include "PipelineStateCommon.h"

namespace Ic3::Graphics::GCI
{

	struct ComputePipelineStateObjectCreateInfo
	{
	};

	/// @brief PSO for the compute pipeline.
	class IC3_GRAPHICS_GCI_CLASS ComputePipelineStateObject : public PipelineStateObject
	{
	public:
		ComputePipelineStateObject( GPUDevice & pGPUDevice, pipeline_state_object_id_t pStateObjectID );
		virtual ~ComputePipelineStateObject();

		/**
		 *
		 */
		CPPX_ATTR_NO_DISCARD virtual EPipelineType GetPipelineType() const noexcept override final;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_COMPUTE_PIPELINE_STATE_OBJECT_H__
