
#pragma once

#ifndef __IC3_GRAPHICS_GCI_RENDER_TARGET_DYNAMIC_STATES_H__
#define __IC3_GRAPHICS_GCI_RENDER_TARGET_DYNAMIC_STATES_H__

#include "RenderPassCommon.h"

namespace Ic3::Graphics::GCI
{

	class RenderTargetBindingDynamicState
	{
	public:
		RenderTargetBindingDynamicState( const RenderTargetBindingDynamicState & ) = default;
		RenderTargetBindingDynamicState & operator=( const RenderTargetBindingDynamicState & ) = default;

		RenderTargetBindingDynamicState();
		explicit RenderTargetBindingDynamicState( const RenderTargetBindingDefinition & pDefinition );

		~RenderTargetBindingDynamicState();

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept;

		CPPX_ATTR_NO_DISCARD native_uint CountActiveColorAttachments() const noexcept;

		CPPX_ATTR_NO_DISCARD const RenderTargetBindingDefinition & GetBindingDefinition() const noexcept;

		void assign( const RenderTargetBindingDefinition & pDefinition );

		RenderTargetAttachmentBinding & SetColorAttachmentBinding( native_uint pIndex );

		void SetColorAttachmentBinding( native_uint pIndex, const RenderTargetAttachmentBinding & pRPCAttachmentBinding );
		void SetColorAttachmentBindings( const RenderTargetColorAttachmentBindingArray & pRPCAttachmentBindings );
		void SetColorAttachmentBindings( native_uint pFirstIndex, native_uint pCount, const RenderTargetAttachmentBinding * pRPCAttachmentBindings );

		RenderTargetAttachmentBinding & SetDepthStencilAttachmentBinding();

		void SetDepthStencilAttachmentBinding( const RenderTargetAttachmentBinding & pRPDSAttachmentBinding );

		void ResetColorAttachmentBinding( native_uint pIndex );
		void ResetColorAttachmentBindings( native_uint pFirstIndex, native_uint pCount );
		void ResetColorAttachmentBindings();

		void ResetDepthStencilAttachmentBinding();

		void ResetAllAttachmentBindings();

	private:
		void _SetColorAttachmentBindings( native_uint pFirstIndex, native_uint pCount, const RenderTargetAttachmentBinding * pRPCAttachmentBindings );
		void _ResetColorAttachmentBindings( native_uint pFirstIndex, native_uint pCount );

	private:
		RenderTargetBindingDefinition _renderTargetBindingDefinition;
	};

	class RenderPassConfigurationDynamicState
	{
	public:
		RenderPassConfigurationDynamicState( const RenderPassConfigurationDynamicState & ) = default;
		RenderPassConfigurationDynamicState & operator=( const RenderPassConfigurationDynamicState & ) = default;

		RenderPassConfigurationDynamicState();
		explicit RenderPassConfigurationDynamicState( const RenderPassConfiguration & pConfiguration );

		~RenderPassConfigurationDynamicState();

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept;

		CPPX_ATTR_NO_DISCARD native_uint CountActiveColorAttachments() const noexcept;

		CPPX_ATTR_NO_DISCARD const RenderPassConfiguration & GetRenderPassConfiguration() const noexcept;

		void Assign( const RenderPassConfiguration & pConfiguration );

		RenderPassAttachmentConfig & SetColorAttachmentUsage( native_uint pIndex );

		void SetColorAttachmentUsage( native_uint pIndex, const RenderPassAttachmentConfig & pRPCAttachmentUsage );
		void SetColorAttachmentUsages( const RenderPassColorAttachmentConfigArray & pRPCAttachmentUsages );
		void SetColorAttachmentBindings( native_uint pFirstIndex, native_uint pCount, const RenderPassAttachmentConfig * pRPCAttachmentUsages );

		RenderPassAttachmentConfig & SetDepthStencilAttachmentUsage();

		void SetDepthStencilAttachmentUsage( const RenderPassAttachmentConfig & pRPDSAttachmentUsage );

		void ResetColorAttachmentUsage( native_uint pIndex );
		void ResetColorAttachmentUsages( native_uint pFirstIndex, native_uint pCount );
		void ResetColorAttachmentUsages();

		void ResetDepthStencilAttachmentUsage();

		void ResetAllAttachmentUsages();

	private:
		void _SetColorAttachmentUsages( native_uint pFirstIndex, native_uint pCount, const RenderPassAttachmentConfig * pRPCAttachmentUsages );
		void _ResetColorAttachmentUsages( native_uint pFirstIndex, native_uint pCount );

	private:
		RenderPassConfiguration _renderPassConfiguration;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_RENDER_TARGET_DYNAMIC_STATES_H__
