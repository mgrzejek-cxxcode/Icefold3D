
#if !defined( __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_CONTROLLER_H__ )
#  error ".inl files cannot be included on their own - you need to include the related header file!"
#endif

namespace Ic3::Graphics::GCI
{

	inline const GraphicsPipelineDynamicConfig & GraphicsPipelineStateController::GetPipelineDynamicConfig() const noexcept
	{
		return _currentPipelineDynamicConfig;
	}

	inline bool GraphicsPipelineStateController::IsDynamicPipelineConfigEnabled(
			cppx::bitmask<EGraphicsPipelineDynamicConfigFlags> pConfigMask ) const noexcept
	{
		return _currentPipelineDynamicConfig.activeStateMask.is_set( pConfigMask & eGraphicsPipelineDynamicConfigMaskAll );
	}

	inline bool GraphicsPipelineStateController::HasPendingStateChanges() const noexcept
	{
		return !_stateUpdateMask.empty();
	}

	inline bool GraphicsPipelineStateController::IsRenderPassDescriptorSet() const noexcept
	{
		return _currentPipelineBindings.renderPassDescriptor != nullptr;
	}

	template <typename TGraphicsPipelineStateObject>
	inline const TGraphicsPipelineStateObject * GraphicsPipelineStateController::GetCurrentGraphicsPipelineStateObject() const noexcept
	{
		if( _currentPipelineBindings.pipelineStateObject )
		{
			const auto * baseInterface = static_cast<const IDynamicObject *>( _currentPipelineBindings.pipelineStateObject );
			return baseInterface->QueryInterface<TGraphicsPipelineStateObject>();
		}
		return nullptr;
	}

	template <typename TGraphicsPipelineStateObject>
	inline const TGraphicsPipelineStateObject & GraphicsPipelineStateController::GetCurrentGraphicsPipelineStateObjectRef() const
	{
		if( const auto * pipelineStateObjectPtrMaybeNull = GetCurrentGraphicsPipelineStateObject<TGraphicsPipelineStateObject>() )
		{
			return *pipelineStateObjectPtrMaybeNull;
		}
		Ic3Throw( 0 );
	}

	template <typename TRenderTargetDescriptor>
	inline const TRenderTargetDescriptor * GraphicsPipelineStateController::GetCurrentRenderTargetDescriptor() const noexcept
	{
		if( _currentPipelineBindings.renderTargetDescriptor )
		{
			const auto * baseInterface = static_cast<const IDynamicObject *>( _currentPipelineBindings.renderTargetDescriptor );
			return baseInterface->QueryInterface<TRenderTargetDescriptor>();
		}
		return nullptr;
	}

	template <typename TRenderTargetDescriptor>
	inline const TRenderTargetDescriptor & GraphicsPipelineStateController::GetCurrentRenderTargetDescriptorRef() const
	{
		if( const auto * descriptorPtrMaybeNull = GetCurrentRenderTargetDescriptor<TRenderTargetDescriptor>() )
		{
			return *descriptorPtrMaybeNull;
		}
		Ic3Throw( 0 );
	}

	template <typename TRenderPassDescriptor>
	inline const TRenderPassDescriptor * GraphicsPipelineStateController::GetCurrentRenderPassDescriptor() const noexcept
	{
		if( _currentPipelineBindings.renderPassDescriptor )
		{
			const auto * baseInterface = static_cast<const IDynamicObject *>( _currentPipelineBindings.renderPassDescriptor );
			return baseInterface->QueryInterface<TRenderPassDescriptor>();
		}
		return nullptr;
	}

	template <typename TRenderPassDescriptor>
	inline const TRenderPassDescriptor & GraphicsPipelineStateController::GetCurrentRenderPassDescriptorRef() const
	{
		if( const auto * descriptorPtrMaybeNull = GetCurrentRenderPassDescriptor<TRenderPassDescriptor>() )
		{
			return *descriptorPtrMaybeNull;
		}
		Ic3Throw( 0 );
	}

	template <typename TVertexSourceBindingDescriptor>
	inline const TVertexSourceBindingDescriptor * GraphicsPipelineStateController::GetCurrentVertexSourceBindingDescriptor() const noexcept
	{
		if( _currentPipelineBindings.vertexSourceBindingDescriptor )
		{
			const auto * baseInterface = static_cast<const IDynamicObject *>( _currentPipelineBindings.vertexSourceBindingDescriptor );
			return baseInterface->QueryInterface<TVertexSourceBindingDescriptor>();
		}
		return nullptr;
	}

	template <typename TVertexSourceBindingDescriptor>
	inline const TVertexSourceBindingDescriptor & GraphicsPipelineStateController::GetCurrentVertexSourceBindingDescriptorRef() const
	{
		if( const auto * descriptorPtrMaybeNull = GetCurrentVertexSourceBindingDescriptor<TVertexSourceBindingDescriptor>() )
		{
			return *descriptorPtrMaybeNull;
		}
		Ic3Throw( 0 );
	}

} // namespace Ic3::Graphics::GCI
