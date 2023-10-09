
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

	void GLCommandList::beginCommandSequence()
	{
		CommandList::beginCommandSequence();
	}

	void GLCommandList::endCommandSequence()
	{
		CommandList::endCommandSequence();
	}

	void GLCommandList::cmdDrawDirectIndexed( native_uint pIndicesNum, native_uint pIndicesOffset, native_uint pBaseVertexIndex )
	{
		_graphicsPipelineStateControllerGL->applyStateChanges();

		const auto & drawTopologyProperties = _graphicsPipelineStateControllerGL->getGLDrawTopologyProperties();
		const auto relativeIndexDataOffset = pIndicesOffset * drawTopologyProperties.indexBufferElementByteSize;
		auto * baseIndexDataOffset = reinterpret_cast<void *>( drawTopologyProperties.indexBufferBaseOffset + relativeIndexDataOffset );

		glDrawElementsBaseVertex(
			drawTopologyProperties.primitiveTopology,
			static_cast<GLsizei>( pIndicesNum ),
			drawTopologyProperties.indexBufferDataType,
			baseIndexDataOffset,
			static_cast<GLint>( pBaseVertexIndex ) );
		ic3OpenGLHandleLastError();
	}

	void GLCommandList::cmdDrawDirectIndexedInstanced( native_uint pIndicesNumPerInstance, native_uint pInstancesNum, native_uint pIndicesOffset )
	{
	}

	void GLCommandList::cmdDrawDirectNonIndexed( native_uint pVerticesNum, native_uint pVerticesOffset )
	{
		_graphicsPipelineStateControllerGL->applyStateChanges();

		const auto & drawTopologyProperties = _graphicsPipelineStateControllerGL->getGLDrawTopologyProperties();

		glDrawArrays(
			drawTopologyProperties.primitiveTopology,
			static_cast< GLint >( pVerticesOffset ),
			static_cast< GLsizei >( pVerticesNum ) );
		ic3OpenGLHandleLastError();
	}

	void GLCommandList::cmdDrawDirectNonIndexedInstanced( native_uint pVerticesNumPerInstance, native_uint pInstancesNum, native_uint pVerticesOffset )
	{
	}

	void GLCommandList::cmdExecuteDeferredContext( CommandContextDeferred & pDeferredContext )
	{
		ic3DebugInterrupt();
	}

	void GLCommandList::executeRenderPassLoadActions(
			const RenderPassConfiguration & pRenderPassConfiguration,
			const GraphicsPipelineDynamicState & pDynamicState )
	{
		if( pRenderPassConfiguration.attachmentsActionClearMask != 0 )
		{
			smutil::clearRenderPassFramebuffer( pRenderPassConfiguration, pDynamicState );
		}
	}

	void GLCommandList::executeRenderPassStoreActions(
			const RenderPassConfiguration & pRenderPassConfiguration,
			const GraphicsPipelineDynamicState & pDynamicState )
	{
		if( pRenderPassConfiguration.attachmentsActionResolveMask != 0 )
		{
			smutil::resolveRenderPassFramebuffer(
				_graphicsPipelineStateControllerGL->getCurrentRenderTargetBindingInfo(),
				pRenderPassConfiguration );
		}
	}

} // namespace Ic3::Graphics::GCI
