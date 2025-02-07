
#include "GraphicsPipelineStateDescriptorRTA.h"

namespace Ic3::Graphics::GCI
{

	RenderPassConfigStateDescriptorDynamic::RenderPassConfigStateDescriptorDynamic(
			GPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID )
	: RenderPassConfigStateDescriptor( pGPUDevice, pDescriptorID )
	{}

	RenderPassConfigStateDescriptorDynamic::~RenderPassConfigStateDescriptorDynamic() = default;

	bool RenderPassConfigStateDescriptorDynamic::IsDynamicDescriptor() const noexcept
	{
		return true;
	}

	bool RenderPassConfigStateDescriptorDynamic::IsEmpty() const noexcept
	{
		return _renderPassConfiguration.IsEmpty();
	}

	bool RenderPassConfigStateDescriptorDynamic::IsDepthStencilAttachmentActive() const noexcept
	{
		return _renderPassConfiguration.activeAttachmentsMask.is_set( eRTAttachmentFlagDepthStencilBit );
	}

	native_uint RenderPassConfigStateDescriptorDynamic::CountActiveColorAttachments() const noexcept
	{
		return ( _renderPassConfiguration.activeAttachmentsMask & eRTAttachmentMaskColorAll ).count_bits();
	}

	const RenderPassConfiguration & RenderPassConfigStateDescriptorDynamic::GetRenderPassConfiguration() const noexcept
	{
		return _renderPassConfiguration;
	}

	RenderPassAttachmentReference * RenderPassConfigStateDescriptorDynamic::UpdateActiveColorAttachment( native_uint pColorAttachmentIndex )
	{
		return _SetColorAttachmentActive( pColorAttachmentIndex );
	}

	RenderPassAttachmentReference * RenderPassConfigStateDescriptorDynamic::UpdateActiveDepthStencilAttachment()
	{
		return _SetDepthStencilAttachmentActive();
	}

	void RenderPassConfigStateDescriptorDynamic::SetRenderPassConfiguration( const RenderPassConfiguration & pRenderPassConfiguration ) noexcept
	{
		_SetColorAttachmentRefs( 0, GCM::kRTOMaxColorAttachmentsNum, pRenderPassConfiguration.colorAttachmentConfigArray.data() );
		_SetDepthStencilAttachmentRef( pRenderPassConfiguration.depthStencilAttachmentConfig );
	}

	void RenderPassConfigStateDescriptorDynamic::SetColorAttachmentRef(
			native_uint pColorAttachmentIndex,
			const RenderPassAttachmentReference & pAttachmentRef )
	{
		_SetColorAttachmentRefs( pColorAttachmentIndex, 1, &pAttachmentRef );
	}

	void RenderPassConfigStateDescriptorDynamic::SetColorAttachmentRefs(
			const cppx::array_view<const RenderPassAttachmentReference> & pAttachmentRefs,
			native_uint pColorAttachmentFirstIndex,
			native_uint pColorAttachmentCount )
	{
		_SetColorAttachmentRefs( pColorAttachmentFirstIndex, pColorAttachmentCount, pAttachmentRefs.data() );
	}

	void RenderPassConfigStateDescriptorDynamic::SetDepthStencilAttachmentRef( const RenderPassAttachmentReference & pAttachmentRef )
	{
		_SetDepthStencilAttachmentRef( pAttachmentRef );
	}

	void RenderPassConfigStateDescriptorDynamic::ResetAll()
	{
		_ResetColorAttachmentRefs( 0, GCM::kRTOMaxColorAttachmentsNum );
		_ResetDepthStencilAttachmentRef();
	}

	void RenderPassConfigStateDescriptorDynamic::ResetAllFlags()
	{
		_renderPassConfiguration.activeAttachmentsMask.clear();
		_renderPassConfiguration.activeAttachmentsNum = 0;
	}

	void RenderPassConfigStateDescriptorDynamic::ResetColorAttachmentRef( native_uint pColorAttachmentIndex )
	{
		_ResetColorAttachmentRefs( pColorAttachmentIndex, 1 );
	}

	void RenderPassConfigStateDescriptorDynamic::ResetColorAttachmentRefs(
			native_uint pColorAttachmentFirstIndex,
			native_uint pColorAttachmentCount )
	{
		_ResetColorAttachmentRefs( pColorAttachmentFirstIndex, pColorAttachmentCount );
	}

	void RenderPassConfigStateDescriptorDynamic::ResetDepthStencilAttachmentRef()
	{
		_ResetDepthStencilAttachmentRef();
	}

	RenderPassAttachmentReference * RenderPassConfigStateDescriptorDynamic::_SetColorAttachmentActive( native_uint pColorAttachmentIndex )
	{
		if( CXU::RTOIsAttachmentIndexValid( pColorAttachmentIndex ) )
		{
			const auto colorAttachmentBit = CXU::RTOMakeAttachmentFlag( pColorAttachmentIndex );

			auto & colorAttachmentRef = _renderPassConfiguration.colorAttachmentConfigArray[pColorAttachmentIndex];

			if( !_renderPassConfiguration.activeAttachmentsMask.is_set( colorAttachmentBit ) )
			{
				_renderPassConfiguration.activeAttachmentsMask.set( colorAttachmentBit );
				_renderPassConfiguration.activeAttachmentsNum += 1;
			}

			return &colorAttachmentRef;
		}

		return nullptr;
	}

	void RenderPassConfigStateDescriptorDynamic::_SetColorAttachmentRefs(
			native_uint pColorAttachmentFirstIndex,
			native_uint pColorAttachmentCount,
			const RenderPassAttachmentReference * pAttachmentRefs )
	{
		if( ( pColorAttachmentCount > 0 ) && CXU::RTOIsAttachmentIndexValid( pColorAttachmentFirstIndex ) )
		{
			for( uint32 nColorAttachment = 0; nColorAttachment < pColorAttachmentCount; ++nColorAttachment )
			{
				const auto colorAttachmentIndex = pColorAttachmentFirstIndex + nColorAttachment;
				if( !CXU::RTOIsAttachmentIndexValid( colorAttachmentIndex ) )
				{
					break;
				}

				const auto colorAttachmentBit = CXU::RTOMakeAttachmentFlag( colorAttachmentIndex );

				auto & colorAttachmentRef = _renderPassConfiguration.colorAttachmentConfigArray[colorAttachmentIndex];

				if( const auto & inputColorAttachmentRef = pAttachmentRefs[nColorAttachment] )
				{
					if( !_renderPassConfiguration.activeAttachmentsMask.is_set( colorAttachmentBit ) )
					{
						_renderPassConfiguration.activeAttachmentsMask.set( colorAttachmentBit );
						_renderPassConfiguration.activeAttachmentsNum += 1;
					}

					colorAttachmentRef = inputColorAttachmentRef;
				}
				else
				{
					if( _renderPassConfiguration.activeAttachmentsMask.is_set( colorAttachmentBit ) )
					{
						_renderPassConfiguration.activeAttachmentsMask.unset( colorAttachmentBit );
						_renderPassConfiguration.activeAttachmentsNum -= 1;
					}

					colorAttachmentRef.Reset();
				}
			}
		}
	}

	RenderPassAttachmentReference * RenderPassConfigStateDescriptorDynamic::_SetDepthStencilAttachmentActive()
	{
		if( !_renderPassConfiguration.activeAttachmentsMask.is_set( eRTAttachmentFlagDepthStencilBit ) )
		{
			_renderPassConfiguration.activeAttachmentsMask.set( eRTAttachmentFlagDepthStencilBit );
			_renderPassConfiguration.activeAttachmentsNum += 1;
		}

		return &( _renderPassConfiguration.depthStencilAttachmentConfig );
	}

	void RenderPassConfigStateDescriptorDynamic::_SetDepthStencilAttachmentRef( const RenderPassAttachmentReference & pAttachmentRef )
	{
		if( pAttachmentRef )
		{
			if( !_renderPassConfiguration.activeAttachmentsMask.is_set( eRTAttachmentFlagDepthStencilBit ) )
			{
				_renderPassConfiguration.activeAttachmentsMask.set( eRTAttachmentFlagDepthStencilBit );
				_renderPassConfiguration.activeAttachmentsNum += 1;
			}

			_renderPassConfiguration.depthStencilAttachmentConfig = pAttachmentRef;
		}
		else
		{
			if( _renderPassConfiguration.activeAttachmentsMask.is_set( eRTAttachmentFlagDepthStencilBit ) )
			{
				_renderPassConfiguration.activeAttachmentsMask.unset( eRTAttachmentFlagDepthStencilBit );
				_renderPassConfiguration.activeAttachmentsNum -= 1;
			}

			_renderPassConfiguration.depthStencilAttachmentConfig.Reset();
		}
	}

	void RenderPassConfigStateDescriptorDynamic::_ResetColorAttachmentRefs(
			native_uint pColorAttachmentFirstIndex,
			native_uint pColorAttachmentCount )
	{
		if( ( pColorAttachmentCount > 0 ) && CXU::RTOIsAttachmentIndexValid( pColorAttachmentFirstIndex ) )
		{
			for( uint32 nColorAttachment = 0; nColorAttachment < pColorAttachmentCount; ++nColorAttachment )
			{
				const auto colorAttachmentIndex = pColorAttachmentFirstIndex + nColorAttachment;
				if( !CXU::RTOIsAttachmentIndexValid( colorAttachmentIndex ) )
				{
					break;
				}

				const auto colorAttachmentBit = CXU::RTOMakeAttachmentFlag( colorAttachmentIndex );

				auto & colorAttachmentRef = _renderPassConfiguration.colorAttachmentConfigArray[colorAttachmentIndex];

				if( _renderPassConfiguration.activeAttachmentsMask.is_set( colorAttachmentBit ) )
				{
					_renderPassConfiguration.activeAttachmentsMask.unset( colorAttachmentBit );
					_renderPassConfiguration.activeAttachmentsNum -= 1;
				}

				colorAttachmentRef.Reset();
			}
		}
	}

	void RenderPassConfigStateDescriptorDynamic::_ResetDepthStencilAttachmentRef()
	{
		if( _renderPassConfiguration.activeAttachmentsMask.is_set( eRTAttachmentFlagDepthStencilBit ) )
		{
			_renderPassConfiguration.activeAttachmentsMask.unset( eRTAttachmentFlagDepthStencilBit );
			_renderPassConfiguration.activeAttachmentsNum -= 1;
		}

		_renderPassConfiguration.depthStencilAttachmentConfig.Reset();
	}

}
