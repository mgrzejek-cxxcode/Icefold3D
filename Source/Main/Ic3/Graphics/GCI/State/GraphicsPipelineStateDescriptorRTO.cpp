
#include "GraphicsPipelineStateDescriptorRTO.h"

namespace Ic3::Graphics::GCI
{

	//!! RenderPassConfigurationStateDescriptor

	RenderPassConfigurationStateDescriptor::RenderPassConfigurationStateDescriptor(
			GPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID )
	: PipelineStateDescriptor( pGPUDevice, pDescriptorID )
	{}

	RenderPassConfigurationStateDescriptor::~RenderPassConfigurationStateDescriptor() = default;

	EPipelineStateDescriptorType RenderPassConfigurationStateDescriptor::GetDescriptorType() const noexcept
	{
		return EPipelineStateDescriptorType::DTRenderPassConfig;
	}


	//!! RenderPassConfigurationStateDescriptorDynamic

	RenderPassConfigurationStateDescriptorDynamic::RenderPassConfigurationStateDescriptorDynamic(
			GPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID )
	: RenderPassConfigurationStateDescriptor( pGPUDevice, pDescriptorID )
	{}

	RenderPassConfigurationStateDescriptorDynamic::~RenderPassConfigurationStateDescriptorDynamic() = default;

	bool RenderPassConfigurationStateDescriptorDynamic::IsDynamicDescriptor() const noexcept
	{
		return true;
	}

	bool RenderPassConfigurationStateDescriptorDynamic::IsEmpty() const noexcept
	{
		return _renderPassConfiguration.IsEmpty();
	}

	bool RenderPassConfigurationStateDescriptorDynamic::IsDepthStencilAttachmentActive() const noexcept
	{
		return _renderPassConfiguration.activeAttachmentsMask.is_set( eRTAttachmentFlagDepthStencilBit );
	}

	native_uint RenderPassConfigurationStateDescriptorDynamic::CountActiveColorAttachments() const noexcept
	{
		return ( _renderPassConfiguration.activeAttachmentsMask & eRTAttachmentMaskColorAll ).count_bits();
	}

	const RenderPassConfiguration & RenderPassConfigurationStateDescriptorDynamic::GetRenderPassConfiguration() const noexcept
	{
		return _renderPassConfiguration;
	}

	RenderPassAttachmentReference * RenderPassConfigurationStateDescriptorDynamic::UpdateActiveColorAttachment( native_uint pColorAttachmentIndex )
	{
		return _SetColorAttachmentActive( pColorAttachmentIndex );
	}

	RenderPassAttachmentReference * RenderPassConfigurationStateDescriptorDynamic::UpdateActiveDepthStencilAttachment()
	{
		return _SetDepthStencilAttachmentActive();
	}

	void RenderPassConfigurationStateDescriptorDynamic::SetRenderPassConfiguration( const RenderPassConfiguration & pRenderPassConfiguration ) noexcept
	{
		_SetColorAttachmentRefs( 0, GCM::kRTOMaxColorAttachmentsNum, pRenderPassConfiguration.colorAttachmentConfigArray.data() );
		_SetDepthStencilAttachmentRef( pRenderPassConfiguration.depthStencilAttachmentConfig );
	}

	void RenderPassConfigurationStateDescriptorDynamic::SetColorAttachmentRef(
			native_uint pColorAttachmentIndex,
			const RenderPassAttachmentReference & pAttachmentRef )
	{
		_SetColorAttachmentRefs( pColorAttachmentIndex, 1, &pAttachmentRef );
	}

	void RenderPassConfigurationStateDescriptorDynamic::SetColorAttachmentRefs(
			const cppx::array_view<const RenderPassAttachmentReference> & pAttachmentRefs,
			native_uint pColorAttachmentFirstIndex,
			native_uint pColorAttachmentCount )
	{
		_SetColorAttachmentRefs( pColorAttachmentFirstIndex, pColorAttachmentCount, pAttachmentRefs.data() );
	}

	void RenderPassConfigurationStateDescriptorDynamic::SetDepthStencilAttachmentRef( const RenderPassAttachmentReference & pAttachmentRef )
	{
		_SetDepthStencilAttachmentRef( pAttachmentRef );
	}

	void RenderPassConfigurationStateDescriptorDynamic::ResetAll()
	{
		_ResetColorAttachmentRefs( 0, GCM::kRTOMaxColorAttachmentsNum );
		_ResetDepthStencilAttachmentRef();
	}

	void RenderPassConfigurationStateDescriptorDynamic::ResetAllFlags()
	{
		_renderPassConfiguration.activeAttachmentsMask.clear();
		_renderPassConfiguration.activeAttachmentsNum = 0;
	}

	void RenderPassConfigurationStateDescriptorDynamic::ResetColorAttachmentRef( native_uint pColorAttachmentIndex )
	{
		_ResetColorAttachmentRefs( pColorAttachmentIndex, 1 );
	}

	void RenderPassConfigurationStateDescriptorDynamic::ResetColorAttachmentRefs(
			native_uint pColorAttachmentFirstIndex,
			native_uint pColorAttachmentCount )
	{
		_ResetColorAttachmentRefs( pColorAttachmentFirstIndex, pColorAttachmentCount );
	}

	void RenderPassConfigurationStateDescriptorDynamic::ResetDepthStencilAttachmentRef()
	{
		_ResetDepthStencilAttachmentRef();
	}

	RenderPassAttachmentReference * RenderPassConfigurationStateDescriptorDynamic::_SetColorAttachmentActive( native_uint pColorAttachmentIndex )
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

	void RenderPassConfigurationStateDescriptorDynamic::_SetColorAttachmentRefs(
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

	RenderPassAttachmentReference * RenderPassConfigurationStateDescriptorDynamic::_SetDepthStencilAttachmentActive()
	{
		if( !_renderPassConfiguration.activeAttachmentsMask.is_set( eRTAttachmentFlagDepthStencilBit ) )
		{
			_renderPassConfiguration.activeAttachmentsMask.set( eRTAttachmentFlagDepthStencilBit );
			_renderPassConfiguration.activeAttachmentsNum += 1;
		}

		return &( _renderPassConfiguration.depthStencilAttachmentConfig );
	}

	void RenderPassConfigurationStateDescriptorDynamic::_SetDepthStencilAttachmentRef( const RenderPassAttachmentReference & pAttachmentRef )
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

	void RenderPassConfigurationStateDescriptorDynamic::_ResetColorAttachmentRefs(
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

	void RenderPassConfigurationStateDescriptorDynamic::_ResetDepthStencilAttachmentRef()
	{
		if( _renderPassConfiguration.activeAttachmentsMask.is_set( eRTAttachmentFlagDepthStencilBit ) )
		{
			_renderPassConfiguration.activeAttachmentsMask.unset( eRTAttachmentFlagDepthStencilBit );
			_renderPassConfiguration.activeAttachmentsNum -= 1;
		}

		_renderPassConfiguration.depthStencilAttachmentConfig.Reset();
	}


	namespace PIM
	{

		//!! RenderPassConfigurationStateDescriptorGeneric

		RenderPassConfigurationStateDescriptorGeneric::RenderPassConfigurationStateDescriptorGeneric(
				GPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID,
				const RenderPassConfiguration & pPassConfiguration )
				: RenderPassConfigurationStateDescriptor( pGPUDevice, pDescriptorID )
		{}

		RenderPassConfigurationStateDescriptorGeneric::~RenderPassConfigurationStateDescriptorGeneric() = default;

		TGfxHandle<RenderPassConfigurationStateDescriptorGeneric> RenderPassConfigurationStateDescriptorGeneric::CreateFromPassConfiguration(
				GPUDevice & pGPUDevice,
				const RenderPassConfiguration & pPassConfiguration,
				pipeline_state_descriptor_id_t pDescriptorID )
		{
			if( !GCU::RTOValidateRenderPassConfiguration( pPassConfiguration ) )
			{
				return nullptr;
			}

			return CreateDynamicObject<RenderPassConfigurationStateDescriptorGeneric>( pGPUDevice, pDescriptorID, pPassConfiguration );
		}


		//!! RenderPassConfigurationStateDescriptor

		RenderPassConfigurationStateDescriptorNative::RenderPassConfigurationStateDescriptorNative(
				GPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID )
		: RenderPassConfigurationStateDescriptor( pGPUDevice, pDescriptorID )
		{}

		RenderPassConfigurationStateDescriptorNative::~RenderPassConfigurationStateDescriptorNative() = default;

	}

}
