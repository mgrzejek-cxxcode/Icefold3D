
#include "VK_commandList.h"
#include "VK_commandSystem.h"
#include <Ic3/Graphics/GCI/CommandContext.h>
#include <Ic3/Graphics/GCI/Resources/GPUBuffer.h>
#include <Ic3/Graphics/GCI/Resources/Shader.h>
#include <Ic3/Graphics/GCI/Resources/Texture.h>

namespace Ic3::Graphics::GCI
{

	VKCommandList::VKCommandList( VKCommandSystem & pVKCommandSystem, ECommandListType pListType )
	: CommandList( pVKCommandSystem, pListType )
	, _stateController( *this, mGPUDevice.QueryInterface<VKGPUDevice>()->GetDescriptorCache() )
	{ }

	VKCommandList::~VKCommandList() = default;

	void VKCommandList::BeginCommandSequence()
	{
		CommandList::BeginCommandSequence();
	}

	void VKCommandList::EndCommandSequence()
	{
		CommandList::EndCommandSequence();
	}

	void VKCommandList::executeDeferredContext( CommandContextDeferred & pDeferredContext )
	{
	}

	bool VKCommandList::SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPipelineSO )
	{
		return true;
	}

	bool VKCommandList::SetVertexStreamStateObject( const VertexStreamStateObject & pVertexStreamSO )
	{
		return true;
	}

	void VKCommandList::clearRenderTarget( cppx::bitmask<ERTAttachmentFlags> pAttachmentMask )
	{
	}

	void VKCommandList::SetViewport( const ViewportDesc & pViewportDesc )
	{
	}

	bool VKCommandList::SetShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData )
	{
		return true;
	}

	bool VKCommandList::SetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GPUBuffer & pConstantBuffer )
	{
		return true;
	}

	bool VKCommandList::SetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture )
	{
		return true;
	}

	bool VKCommandList::SetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler )
	{
		return true;
	}

	void VKCommandList::drawDirectIndexed( native_uint pIndicesNum, native_uint pIndicesOffset )
	{
	}

	void VKCommandList::drawDirectIndexedInstanced( native_uint pIndicesNumPerInstance, native_uint pInstancesNum, native_uint pIndicesOffset )
	{
	}

	void VKCommandList::drawDirectNonIndexed( native_uint pVerticesNum, native_uint pVerticesOffset )
	{
	}

	void VKCommandList::drawDirectNonIndexedInstanced( native_uint pVerticesNumPerInstance, native_uint pInstancesNum, native_uint pVerticesOffset )
	{
	}

} // namespace Ic3::Graphics::GCI
