
#include "RenderTargetDynamicStates.h"

namespace Ic3::Graphics::GCI
{

	RenderTargetBindingDynamicState::RenderTargetBindingDynamicState()
	{}

	RenderTargetBindingDynamicState::RenderTargetBindingDynamicState( const RenderTargetBindingDefinition & pDefinition )
	: _renderTargetBindingDefinition( pDefinition )
	{}

	RenderTargetBindingDynamicState::~RenderTargetBindingDynamicState() = default;

	bool RenderTargetBindingDynamicState::empty() const noexcept
	{
		return _renderTargetBindingDefinition.activeAttachmentsMask.empty();
	}

	native_uint RenderTargetBindingDynamicState::countActiveColorAttachments() const noexcept
	{
		return popCount( _renderTargetBindingDefinition.activeAttachmentsMask & eRTAttachmentMaskColorAll );
	}

	const RenderTargetBindingDefinition & RenderTargetBindingDynamicState::getBindingDefinition() const noexcept
	{
		return _renderTargetBindingDefinition;
	}

	void RenderTargetBindingDynamicState::assign( const RenderTargetBindingDefinition & pDefinition )
	{
		_renderTargetBindingDefinition = pDefinition;
	}

	RenderTargetAttachmentBinding & RenderTargetBindingDynamicState::setColorAttachmentBinding( native_uint pIndex )
	{
		ic3DebugAssert( CxDef::isRTAttachmentIndexValid( pIndex ) );
		_renderTargetBindingDefinition.activeAttachmentsMask.set( CxDef::makeRTAttachmentFlag( pIndex ) );
		return _renderTargetBindingDefinition.colorAttachments[pIndex];
	}

	void RenderTargetBindingDynamicState::setColorAttachmentBinding(
			native_uint pIndex,
			const RenderTargetAttachmentBinding & pRPCAttachmentBinding )
	{
		_setColorAttachmentBindings( pIndex, 1, &pRPCAttachmentBinding );
	}

	void RenderTargetBindingDynamicState::setColorAttachmentBindings(
			const RenderTargetColorAttachmentBindingArray & pRPCAttachmentBindings )
	{
		_setColorAttachmentBindings( 0, pRPCAttachmentBindings.size(), pRPCAttachmentBindings.data() );
	}

	void RenderTargetBindingDynamicState::setColorAttachmentBindings(
			native_uint pFirstIndex,
			native_uint pCount,
			const RenderTargetAttachmentBinding * pRPCAttachmentBindings )
	{
		_setColorAttachmentBindings( pFirstIndex, pCount, pRPCAttachmentBindings );
	}

	RenderTargetAttachmentBinding & RenderTargetBindingDynamicState::setDepthStencilAttachmentBinding()
	{
		_renderTargetBindingDefinition.activeAttachmentsMask.set( eRtAttachmentFlagDepthStencilBit );
		return _renderTargetBindingDefinition.depthStencilAttachment;
	}

	void RenderTargetBindingDynamicState::setDepthStencilAttachmentBinding(
			const RenderTargetAttachmentBinding & pRPDSAttachmentBinding )
	{
		_renderTargetBindingDefinition.activeAttachmentsMask.setOrUnset( eRtAttachmentFlagDepthStencilBit, !pRPDSAttachmentBinding.empty() );
		_renderTargetBindingDefinition.depthStencilAttachment = pRPDSAttachmentBinding;
	}

	void RenderTargetBindingDynamicState::resetColorAttachmentBinding( native_uint pIndex )
	{
		_resetColorAttachmentBindings( pIndex, 1 );
	}

	void RenderTargetBindingDynamicState::resetColorAttachmentBindings( native_uint pFirstIndex, native_uint pCount )
	{
		_resetColorAttachmentBindings( pFirstIndex, pCount );
	}

	void RenderTargetBindingDynamicState::resetColorAttachmentBindings()
	{
		_resetColorAttachmentBindings( 0, GCM::cxRTMaxColorAttachmentsNum );
	}

	void RenderTargetBindingDynamicState::resetDepthStencilAttachmentBinding()
	{
		_renderTargetBindingDefinition.activeAttachmentsMask.unset( eRtAttachmentFlagDepthStencilBit );
		_renderTargetBindingDefinition.depthStencilAttachment.reset();
	}

	void RenderTargetBindingDynamicState::resetAllAttachmentBindings()
	{
		resetColorAttachmentBindings();
		resetDepthStencilAttachmentBinding();
	}

	void RenderTargetBindingDynamicState::_setColorAttachmentBindings(
			native_uint pFirstIndex,
			native_uint pCount,
			const RenderTargetAttachmentBinding * pRPCAttachmentBindings )
	{
		for( native_uint caIndex = pFirstIndex; CxDef::isRTColorAttachmentIndexValid( caIndex ) && ( pCount != 0 ); ++caIndex, --pCount )
		{
			const auto & sourceCABinding = pRPCAttachmentBindings[caIndex - pFirstIndex];

			const auto colorAttachmentBit = CxDef::makeRTAttachmentFlag( caIndex );

			_renderTargetBindingDefinition.colorAttachments[caIndex] = sourceCABinding;
			_renderTargetBindingDefinition.activeAttachmentsMask.setOrUnset( colorAttachmentBit, !sourceCABinding.empty() );
		}
	}

	void RenderTargetBindingDynamicState::_resetColorAttachmentBindings( native_uint pFirstIndex, native_uint pCount )
	{
		for( native_uint caIndex = pFirstIndex; CxDef::isRTColorAttachmentIndexValid( caIndex ) && ( pCount != 0 ); ++caIndex, --pCount )
		{
			const auto colorAttachmentBit = CxDef::makeRTAttachmentFlag( caIndex );

			_renderTargetBindingDefinition.colorAttachments[caIndex].reset();
			_renderTargetBindingDefinition.activeAttachmentsMask.unset( colorAttachmentBit );
		}
	}


	RenderPassConfigurationDynamicState::RenderPassConfigurationDynamicState()
	{}

	RenderPassConfigurationDynamicState::RenderPassConfigurationDynamicState( const RenderPassConfiguration & pConfiguration )
	: _renderPassConfiguration( pConfiguration )
	{}

	RenderPassConfigurationDynamicState::~RenderPassConfigurationDynamicState() = default;

	bool RenderPassConfigurationDynamicState::empty() const noexcept
	{
		return _renderPassConfiguration.activeAttachmentsMask.empty();
	}

	native_uint RenderPassConfigurationDynamicState::countActiveColorAttachments() const noexcept
	{
		return popCount( _renderPassConfiguration.activeAttachmentsMask & eRTAttachmentMaskColorAll );
	}

	const RenderPassConfiguration & RenderPassConfigurationDynamicState::getRenderPassConfiguration() const noexcept
	{
		return _renderPassConfiguration;
	}

	void RenderPassConfigurationDynamicState::assign( const RenderPassConfiguration & pConfiguration )
	{
		_renderPassConfiguration = pConfiguration;
	}

	RenderPassAttachmentConfig & RenderPassConfigurationDynamicState::setColorAttachmentUsage( native_uint pIndex )
	{
		ic3DebugAssert( CxDef::isRTAttachmentIndexValid( pIndex ) );
		_renderPassConfiguration.activeAttachmentsMask.set( CxDef::makeRTAttachmentFlag( pIndex ) );
		return _renderPassConfiguration.colorAttachments[pIndex];
	}

	void RenderPassConfigurationDynamicState::setColorAttachmentUsage(
			native_uint pIndex,
			const RenderPassAttachmentConfig & pRPCAttachmentUsage )
	{
		_setColorAttachmentUsages( pIndex, 1, &pRPCAttachmentUsage );
	}

	void RenderPassConfigurationDynamicState::setColorAttachmentUsages(
			const RenderPassColorAttachmentConfigArray & pRPCAttachmentUsages )
	{
		_setColorAttachmentUsages( 0, pRPCAttachmentUsages.size(), pRPCAttachmentUsages.data() );
	}

	void RenderPassConfigurationDynamicState::setColorAttachmentBindings(
			native_uint pFirstIndex,
			native_uint pCount,
			const RenderPassAttachmentConfig * pRPCAttachmentUsages )
	{
		_setColorAttachmentUsages( pFirstIndex, pCount, pRPCAttachmentUsages );
	}

	RenderPassAttachmentConfig & RenderPassConfigurationDynamicState::setDepthStencilAttachmentUsage()
	{
		_renderPassConfiguration.activeAttachmentsMask.set( eRtAttachmentFlagDepthStencilBit );
		return _renderPassConfiguration.depthStencilAttachment;
	}

	void RenderPassConfigurationDynamicState::setDepthStencilAttachmentUsage(
			const RenderPassAttachmentConfig & pRPDSAttachmentUsage )
	{
		_renderPassConfiguration.activeAttachmentsMask.setOrUnset( eRtAttachmentFlagDepthStencilBit, !pRPDSAttachmentUsage.empty() );
		_renderPassConfiguration.depthStencilAttachment = pRPDSAttachmentUsage;
	}

	void RenderPassConfigurationDynamicState::resetColorAttachmentUsage( native_uint pIndex )
	{
		_resetColorAttachmentUsages( pIndex, 1 );
	}

	void RenderPassConfigurationDynamicState::resetColorAttachmentUsages( native_uint pFirstIndex, native_uint pCount )
	{
		_resetColorAttachmentUsages( pFirstIndex, pCount );
	}

	void RenderPassConfigurationDynamicState::resetColorAttachmentUsages()
	{
		_resetColorAttachmentUsages( 0, GCM::cxRTMaxColorAttachmentsNum );
	}

	void RenderPassConfigurationDynamicState::resetDepthStencilAttachmentUsage()
	{
		_renderPassConfiguration.activeAttachmentsMask.unset( eRtAttachmentFlagDepthStencilBit );
		_renderPassConfiguration.depthStencilAttachment.reset();
	}

	void RenderPassConfigurationDynamicState::resetAllAttachmentUsages()
	{
		resetColorAttachmentUsages();
		resetDepthStencilAttachmentUsage();
	}

	void RenderPassConfigurationDynamicState::_setColorAttachmentUsages(
			native_uint pFirstIndex,
			native_uint pCount,
			const RenderPassAttachmentConfig * pRPCAttachmentUsages )
	{
		for( native_uint caIndex = pFirstIndex; CxDef::isRTColorAttachmentIndexValid( caIndex ) && ( pCount != 0 ); ++caIndex, --pCount )
		{
			const auto & sourceCAUsage = pRPCAttachmentUsages[caIndex - pFirstIndex];

			const auto colorAttachmentBit = CxDef::makeRTAttachmentFlag( caIndex );

			_renderPassConfiguration.colorAttachments[caIndex] = sourceCAUsage;
			_renderPassConfiguration.activeAttachmentsMask.setOrUnset( colorAttachmentBit, !sourceCAUsage.empty() );
		}
	}

	void RenderPassConfigurationDynamicState::_resetColorAttachmentUsages( native_uint pFirstIndex, native_uint pCount )
	{
		for( native_uint caIndex = pFirstIndex; CxDef::isRTColorAttachmentIndexValid( caIndex ) && ( pCount != 0 ); ++caIndex, --pCount )
		{
			const auto colorAttachmentBit = CxDef::makeRTAttachmentFlag( caIndex );

			_renderPassConfiguration.colorAttachments[caIndex].reset();
			_renderPassConfiguration.activeAttachmentsMask.unset( colorAttachmentBit );
		}
	}

}
