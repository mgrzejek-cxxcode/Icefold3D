
#include "GLCommandList.h"
#include "GLCommandSystem.h"
#include "GLGPUDevice.h"
#include "Objects/GLFramebufferObject.h"
#include "Objects/GLShaderProgramObject.h"
#include "Objects/GLVertexArrayObject.h"
#include "State/GLPipelineStateObject.h"
#include "State/GLGraphicsShaderState.h"
#include "State/GLRenderTarget.h"

#include <Ic3/Graphics/GCI/State/RenderTargetDynamicStates.h>

namespace Ic3::Graphics::GCI
{

	GLCommandList::GLCommandList(
			GLCommandSystem & pGLCommandSystem,
			ECommandListType pListType,
			System::OpenGLRenderContextHandle pSysGLRenderContext,
			GLGraphicsPipelineStateController & pStateController )
	: CommandListRenderPassDefault( pGLCommandSystem, pListType, pStateController )
	, mSysGLRenderContext( pSysGLRenderContext )
	, _graphicsPipelineStateControllerGL( &pStateController )
	{}

	GLCommandList::~GLCommandList() = default;

	void GLCommandList::BeginCommandSequence()
	{
		CommandList::BeginCommandSequence();
	}

	void GLCommandList::EndCommandSequence()
	{
		CommandList::EndCommandSequence();
	}

	void GLCommandList::CmdDrawDirectIndexed( native_uint pIndicesNum, native_uint pIndicesOffset, native_uint pBaseVertexIndex )
	{
		_graphicsPipelineStateControllerGL->ApplyStateChanges();

		const auto & drawTopologyProperties = _graphicsPipelineStateControllerGL->GetGLDrawTopologyProperties();
		const auto relativeIndexDataOffset = pIndicesOffset * drawTopologyProperties.indexBufferElementByteSize;
		auto * baseIndexDataOffset = reinterpret_cast<void *>( drawTopologyProperties.indexBufferBaseOffset + relativeIndexDataOffset );

		glDrawElementsBaseVertex(
			drawTopologyProperties.primitiveTopology,
			static_cast<GLsizei>( pIndicesNum ),
			drawTopologyProperties.indexBufferDataType,
			baseIndexDataOffset,
			static_cast<GLint>( pBaseVertexIndex ) );
		Ic3OpenGLHandleLastError();
	}

	void GLCommandList::CmdDrawDirectIndexedInstanced( native_uint pIndicesNumPerInstance, native_uint pInstancesNum, native_uint pIndicesOffset )
	{
	}

	void GLCommandList::CmdDrawDirectNonIndexed( native_uint pVerticesNum, native_uint pVerticesOffset )
	{
		_graphicsPipelineStateControllerGL->ApplyStateChanges();

		const auto & drawTopologyProperties = _graphicsPipelineStateControllerGL->GetGLDrawTopologyProperties();

		glDrawArrays(
			drawTopologyProperties.primitiveTopology,
			static_cast< GLint >( pVerticesOffset ),
			static_cast< GLsizei >( pVerticesNum ) );
		Ic3OpenGLHandleLastError();
	}

	void GLCommandList::CmdDrawDirectNonIndexedInstanced( native_uint pVerticesNumPerInstance, native_uint pInstancesNum, native_uint pVerticesOffset )
	{
	}

	void GLCommandList::CmdExecuteDeferredContext( CommandContextDeferred & pDeferredContext )
	{
		Ic3DebugInterrupt();
	}

	void GLCommandList::ExecuteRenderPassLoadActions(
			const RenderPassConfiguration & pRenderPassConfiguration,
			const GraphicsPipelineDynamicState & pDynamicState )
	{
		if( pRenderPassConfiguration.attachmentsActionClearMask != 0 )
		{
			SMU::ClearRenderPassFramebuffer( pRenderPassConfiguration, pDynamicState );
		}
	}

	void GLCommandList::ExecuteRenderPassStoreActions(
			const RenderPassConfiguration & pRenderPassConfiguration,
			const GraphicsPipelineDynamicState & pDynamicState )
	{
		if( pRenderPassConfiguration.attachmentsActionResolveMask != 0 )
		{
			SMU::ResolveRenderPassFramebuffer(
				_graphicsPipelineStateControllerGL->GetCurrentRenderTargetBindingInfo(),
				pRenderPassConfiguration );
		}
	}

} // namespace Ic3::Graphics::GCI
