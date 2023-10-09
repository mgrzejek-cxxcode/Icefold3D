
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_MTLCOMMON_GPU_COMMAND_LIST_H__
#define __IC3_GRAPHICS_HW3D_MTLCOMMON_GPU_COMMAND_LIST_H__

#include "MTLPrerequisites.h"
#include <Ic3/Graphics/GCI/CommandList.h>

#include <Metal/MTLCommandBuffer.h>
#include <Metal/MTLCommandEncoder.h>

namespace Ic3::Graphics::GCI
{

	/// @brief
	class MetalCommandList : public CommandList
	{
	public:
		///
		id<MTLDevice> mMTLDevice;
		///
		id<MTLCommandQueue> const mMTLCommandQueue;
		///
		id<MTLCommandBuffer> const mMTLCommandBuffer;

	public:
		MetalCommandList( MetalCommandSystem & pMTLCommandSystem, ECommandListType pListType, id<MTLCommandQueue> pMTLCommandQueue );
		virtual ~MetalCommandList();

		virtual bool beginRenderPass(
				const RenderPassConfigurationImmutableState & pRenderPassState,
				Bitmask<ECommandListActionFlags> pFlags ) override;

		virtual bool beginRenderPass(
				const RenderPassConfigurationDynamicState & pRenderPassState,
				Bitmask<ECommandListActionFlags> pFlags ) override;

		virtual void endRenderPass() override;

		virtual void beginCommandSequence() override;
		virtual void endCommandSequence() override;

		virtual void cmdDrawDirectIndexed( native_uint pIndicesNum, native_uint pIndicesOffset, native_uint pBaseVertexIndex ) override;
		virtual void cmdDrawDirectIndexedInstanced( native_uint pIndicesNumPerInstance, native_uint pInstancesNum, native_uint pIndicesOffset ) override;
		virtual void cmdDrawDirectNonIndexed( native_uint pVerticesNum, native_uint pVerticesOffset ) override;
		virtual void cmdDrawDirectNonIndexedInstanced( native_uint pVerticesNumPerInstance, native_uint pInstancesNum, native_uint pVerticesOffset ) override;

		virtual void cmdExecuteDeferredContext( CommandContextDeferred & pDeferredContext ) override;

	private:
		id<MTLRenderCommandEncoder> _currentPassCommandEncoder;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_MTLCOMMON_GPU_COMMAND_LIST_H__
