
#if !defined( __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_IMPL_SEPARABLE_STATE_H__ )
#  error ".inl files cannot be included on their own - you need to include the related header file!"
#endif

namespace Ic3::Graphics::GCI
{

	inline const SeparableGraphicsPSDCache & GraphicsPipelineStateControllerSeparable::GetSeparableDescriptorsCache() const noexcept
	{
		return _separableDescriptorsCache;
	}

	template<typename TPBlendStateDescriptor>
	inline const TPBlendStateDescriptor & GraphicsPipelineStateControllerSeparable::GetCurrentPSOBlendStateDescriptor() const noexcept
	{
		Ic3DebugAssert( _separableDescriptorsCache.blendStateDescriptor );
		const auto * baseDynamicObject = dynamic_cast_dbg<IDynamicObject *>( _separableDescriptorsCache.blendStateDescriptor );
		return *( baseDynamicObject->QueryInterface<TPBlendStateDescriptor>() );
	}

	template<typename TPDepthStencilStateDescriptor>
	inline const TPDepthStencilStateDescriptor & GraphicsPipelineStateControllerSeparable::GetCurrentPSODepthStencilStateDescriptor() const noexcept
	{
		Ic3DebugAssert( _separableDescriptorsCache.depthStencilStateDescriptor );
		const auto * baseDynamicObject = dynamic_cast_dbg<IDynamicObject *>( _separableDescriptorsCache.depthStencilStateDescriptor );
		return *( baseDynamicObject->QueryInterface<TPDepthStencilStateDescriptor>() );
	}

	template<typename TPRasterizerStateDescriptor>
	inline const TPRasterizerStateDescriptor & GraphicsPipelineStateControllerSeparable::GetCurrentPSORasterizerStateDescriptor() const noexcept
	{
		Ic3DebugAssert( _separableDescriptorsCache.rasterizerStateDescriptor );
		const auto * baseDynamicObject = dynamic_cast_dbg<IDynamicObject *>( _separableDescriptorsCache.rasterizerStateDescriptor );
		return *( baseDynamicObject->QueryInterface<TPRasterizerStateDescriptor>() );
	}

	template<typename TPRootSignatureDescriptor>
	inline const TPRootSignatureDescriptor & GraphicsPipelineStateControllerSeparable::GetCurrentPSORootSignatureDescriptor() const noexcept
	{
		Ic3DebugAssert( _separableDescriptorsCache.rootSignatureDescriptor );
		const auto * baseDynamicObject = dynamic_cast_dbg<IDynamicObject *>( _separableDescriptorsCache.rootSignatureDescriptor );
		return *( baseDynamicObject->QueryInterface<TPRootSignatureDescriptor>() );
	}

	template<typename TPShaderLinkageDescriptor>
	inline const TPShaderLinkageDescriptor & GraphicsPipelineStateControllerSeparable::GetCurrentPSOShaderLinkageDescriptor() const noexcept
	{
		Ic3DebugAssert( _separableDescriptorsCache.shaderLinkageStateDescriptor );
		const auto * baseDynamicObject = dynamic_cast_dbg<IDynamicObject *>( _separableDescriptorsCache.shaderLinkageStateDescriptor );
		return *( baseDynamicObject->QueryInterface<TPShaderLinkageDescriptor>() );
	}

	template<typename TPVertexAttributeLayoutDescriptor>
	inline const TPVertexAttributeLayoutDescriptor & GraphicsPipelineStateControllerSeparable::GetCurrentPSOVertexAttributeLayoutDescriptor() const noexcept
	{
		Ic3DebugAssert( _separableDescriptorsCache.vertexAttributeLayoutDescriptor );
		const auto * baseDynamicObject = dynamic_cast_dbg<IDynamicObject *>( _separableDescriptorsCache.vertexAttributeLayoutDescriptor );
		return *( baseDynamicObject->QueryInterface<TPVertexAttributeLayoutDescriptor>() );
	}

} // namespace Ic3::Graphics::GCI
