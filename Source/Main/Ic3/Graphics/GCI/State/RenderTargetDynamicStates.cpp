
#include "RenderTargetDynamicStates.h"

namespace Ic3::Graphics::GCI
{

	RenderTargetBindingDynamicState::RenderTargetBindingDynamicState()
	{}

	RenderTargetBindingDynamicState::RenderTargetBindingDynamicState( const RenderTargetBindingDefinition & pDefinition )
	: _renderTargetBindingDefinition( pDefinition )
	{}

	RenderTargetBindingDynamicState::~RenderTargetBindingDynamicState() = default;

	bool RenderTargetBindingDynamicState::IsEmpty() const noexcept
	{
		return _renderTargetBindingDefinition.activeAttachmentsMask.empty();
	}

	native_uint RenderTargetBindingDynamicState::CountActiveColorAttachments() const noexcept
	{
		return pop_count( _renderTargetBindingDefinition.activeAttachmentsMask & eRTAttachmentMaskColorAll );
	}

	const RenderTargetBindingDefinition & RenderTargetBindingDynamicState::GetBindingDefinition() const noexcept
	{
		return _renderTargetBindingDefinition;
	}

	void RenderTargetBindingDynamicState::assign( const RenderTargetBindingDefinition & pDefinition )
	{
		_renderTargetBindingDefinition = pDefinition;
	}

	RenderTargetAttachmentBinding & RenderTargetBindingDynamicState::SetColorAttachmentBinding( native_uint pIndex )
	{
		Ic3DebugAssert( CxDef::IsRTAttachmentIndexValid( pIndex ) );
		_renderTargetBindingDefinition.activeAttachmentsMask.set( CxDef::makeRTAttachmentFlag( pIndex ) );
		return _renderTargetBindingDefinition.colorAttachments[pIndex];
	}

	void RenderTargetBindingDynamicState::SetColorAttachmentBinding(
			native_uint pIndex,
			const RenderTargetAttachmentBinding & pRPCAttachmentBinding )
	{
		_SetColorAttachmentBindings( pIndex, 1, &pRPCAttachmentBinding );
	}

	void RenderTargetBindingDynamicState::SetColorAttachmentBindings(
			const RenderTargetColorAttachmentBindingArray & pRPCAttachmentBindings )
	{
		_SetColorAttachmentBindings( 0, pRPCAttachmentBindings.size(), pRPCAttachmentBindings.data() );
	}

	void RenderTargetBindingDynamicState::SetColorAttachmentBindings(
			native_uint pFirstIndex,
			native_uint pCount,
			const RenderTargetAttachmentBinding * pRPCAttachmentBindings )
	{
		_SetColorAttachmentBindings( pFirstIndex, pCount, pRPCAttachmentBindings );
	}

	RenderTargetAttachmentBinding & RenderTargetBindingDynamicState::SetDepthStencilAttachmentBinding()
	{
		_renderTargetBindingDefinition.activeAttachmentsMask.set( eRtAttachmentFlagDepthStencilBit );
		return _renderTargetBindingDefinition.depthStencilAttachment;
	}

	void RenderTargetBindingDynamicState::SetDepthStencilAttachmentBinding(
			const RenderTargetAttachmentBinding & pRPDSAttachmentBinding )
	{
		_renderTargetBindingDefinition.activeAttachmentsMask.set_or_unset( eRtAttachmentFlagDepthStencilBit,
		                                                                   !pRPDSAttachmentBinding.IsEmpty());
		_renderTargetBindingDefinition.depthStencilAttachment = pRPDSAttachmentBinding;
	}

	void RenderTargetBindingDynamicState::ResetColorAttachmentBinding( native_uint pIndex )
	{
		_ResetColorAttachmentBindings( pIndex, 1 );
	}

	void RenderTargetBindingDynamicState::ResetColorAttachmentBindings( native_uint pFirstIndex, native_uint pCount )
	{
		_ResetColorAttachmentBindings( pFirstIndex, pCount );
	}

	void RenderTargetBindingDynamicState::ResetColorAttachmentBindings()
	{
		_ResetColorAttachmentBindings( 0, GCM::cxRTMaxColorAttachmentsNum );
	}

	void RenderTargetBindingDynamicState::ResetDepthStencilAttachmentBinding()
	{
		_renderTargetBindingDefinition.activeAttachmentsMask.unset( eRtAttachmentFlagDepthStencilBit );
		_renderTargetBindingDefinition.depthStencilAttachment.Reset();
	}

	void RenderTargetBindingDynamicState::ResetAllAttachmentBindings()
	{
		ResetColorAttachmentBindings();
		ResetDepthStencilAttachmentBinding();
	}

	void RenderTargetBindingDynamicState::_SetColorAttachmentBindings(
			native_uint pFirstIndex,
			native_uint pCount,
			const RenderTargetAttachmentBinding * pRPCAttachmentBindings )
	{
		for( native_uint caIndex = pFirstIndex; CxDef::IsRTColorAttachmentIndexValid( caIndex ) && ( pCount != 0 ); ++caIndex, --pCount )
		{
			const auto & sourceCABinding = pRPCAttachmentBindings[caIndex - pFirstIndex];

			const auto colorAttachmentBit = CxDef::makeRTAttachmentFlag( caIndex );

			_renderTargetBindingDefinition.colorAttachments[caIndex] = sourceCABinding;
			_renderTargetBindingDefinition.activeAttachmentsMask.set_or_unset( colorAttachmentBit, !sourceCABinding.IsEmpty() );
		}
	}

	void RenderTargetBindingDynamicState::_ResetColorAttachmentBindings( native_uint pFirstIndex, native_uint pCount )
	{
		for( native_uint caIndex = pFirstIndex; CxDef::IsRTColorAttachmentIndexValid( caIndex ) && ( pCount != 0 ); ++caIndex, --pCount )
		{
			const auto colorAttachmentBit = CxDef::makeRTAttachmentFlag( caIndex );

			_renderTargetBindingDefinition.colorAttachments[caIndex].Reset();
			_renderTargetBindingDefinition.activeAttachmentsMask.unset( colorAttachmentBit );
		}
	}


	RenderPassConfigurationDynamicState::RenderPassConfigurationDynamicState()
	{}

	RenderPassConfigurationDynamicState::RenderPassConfigurationDynamicState( const RenderPassConfiguration & pConfiguration )
	: _renderPassConfiguration( pConfiguration )
	{}

	RenderPassConfigurationDynamicState::~RenderPassConfigurationDynamicState() = default;

	bool RenderPassConfigurationDynamicState::IsEmpty() const noexcept
	{
		return _renderPassConfiguration.activeAttachmentsMask.empty();
	}

	native_uint RenderPassConfigurationDynamicState::CountActiveColorAttachments() const noexcept
	{
		return pop_count( _renderPassConfiguration.activeAttachmentsMask & eRTAttachmentMaskColorAll );
	}

	const RenderPassConfiguration & RenderPassConfigurationDynamicState::GetRenderPassConfiguration() const noexcept
	{
		return _renderPassConfiguration;
	}

	void RenderPassConfigurationDynamicState::Assign( const RenderPassConfiguration & pConfiguration )
	{
		_renderPassConfiguration = pConfiguration;
	}

	RenderPassAttachmentConfig & RenderPassConfigurationDynamicState::SetColorAttachmentUsage( native_uint pIndex )
	{
		Ic3DebugAssert( CxDef::IsRTAttachmentIndexValid( pIndex ) );
		_renderPassConfiguration.activeAttachmentsMask.set( CxDef::makeRTAttachmentFlag( pIndex ) );
		return _renderPassConfiguration.colorAttachments[pIndex];
	}

	void RenderPassConfigurationDynamicState::SetColorAttachmentUsage(
			native_uint pIndex,
			const RenderPassAttachmentConfig & pRPCAttachmentUsage )
	{
		_SetColorAttachmentUsages( pIndex, 1, &pRPCAttachmentUsage );
	}

	void RenderPassConfigurationDynamicState::SetColorAttachmentUsages(
			const RenderPassColorAttachmentConfigArray & pRPCAttachmentUsages )
	{
		_SetColorAttachmentUsages( 0, pRPCAttachmentUsages.size(), pRPCAttachmentUsages.data() );
	}

	void RenderPassConfigurationDynamicState::SetColorAttachmentBindings(
			native_uint pFirstIndex,
			native_uint pCount,
			const RenderPassAttachmentConfig * pRPCAttachmentUsages )
	{
		_SetColorAttachmentUsages( pFirstIndex, pCount, pRPCAttachmentUsages );
	}

	RenderPassAttachmentConfig & RenderPassConfigurationDynamicState::SetDepthStencilAttachmentUsage()
	{
		_renderPassConfiguration.activeAttachmentsMask.set( eRtAttachmentFlagDepthStencilBit );
		return _renderPassConfiguration.depthStencilAttachment;
	}

	void RenderPassConfigurationDynamicState::SetDepthStencilAttachmentUsage(
			const RenderPassAttachmentConfig & pRPDSAttachmentUsage )
	{
		_renderPassConfiguration.activeAttachmentsMask.set_or_unset(
				eRtAttachmentFlagDepthStencilBit,
				!pRPDSAttachmentUsage.empty());
		_renderPassConfiguration.depthStencilAttachment = pRPDSAttachmentUsage;
	}

	void RenderPassConfigurationDynamicState::ResetColorAttachmentUsage( native_uint pIndex )
	{
		_ResetColorAttachmentUsages( pIndex, 1 );
	}

	void RenderPassConfigurationDynamicState::ResetColorAttachmentUsages( native_uint pFirstIndex, native_uint pCount )
	{
		_ResetColorAttachmentUsages( pFirstIndex, pCount );
	}

	void RenderPassConfigurationDynamicState::ResetColorAttachmentUsages()
	{
		_ResetColorAttachmentUsages( 0, GCM::cxRTMaxColorAttachmentsNum );
	}

	void RenderPassConfigurationDynamicState::ResetDepthStencilAttachmentUsage()
	{
		_renderPassConfiguration.activeAttachmentsMask.unset( eRtAttachmentFlagDepthStencilBit );
		_renderPassConfiguration.depthStencilAttachment.reset();
	}

	void RenderPassConfigurationDynamicState::ResetAllAttachmentUsages()
	{
		ResetColorAttachmentUsages();
		ResetDepthStencilAttachmentUsage();
	}

	void RenderPassConfigurationDynamicState::_SetColorAttachmentUsages(
			native_uint pFirstIndex,
			native_uint pCount,
			const RenderPassAttachmentConfig * pRPCAttachmentUsages )
	{
		for( native_uint caIndex = pFirstIndex; CxDef::IsRTColorAttachmentIndexValid( caIndex ) && ( pCount != 0 ); ++caIndex, --pCount )
		{
			const auto & sourceCAUsage = pRPCAttachmentUsages[caIndex - pFirstIndex];

			const auto colorAttachmentBit = CxDef::makeRTAttachmentFlag( caIndex );

			_renderPassConfiguration.colorAttachments[caIndex] = sourceCAUsage;
			_renderPassConfiguration.activeAttachmentsMask.set_or_unset( colorAttachmentBit, !sourceCAUsage.empty() );
		}
	}

	void RenderPassConfigurationDynamicState::_ResetColorAttachmentUsages( native_uint pFirstIndex, native_uint pCount )
	{
		for( native_uint caIndex = pFirstIndex; CxDef::IsRTColorAttachmentIndexValid( caIndex ) && ( pCount != 0 ); ++caIndex, --pCount )
		{
			const auto colorAttachmentBit = CxDef::makeRTAttachmentFlag( caIndex );

			_renderPassConfiguration.colorAttachments[caIndex].reset();
			_renderPassConfiguration.activeAttachmentsMask.unset( colorAttachmentBit );
		}
	}

}
