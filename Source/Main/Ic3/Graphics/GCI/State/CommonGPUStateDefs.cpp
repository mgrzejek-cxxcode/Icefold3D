
#include "CommonGPUStateDefs.h"

namespace Ic3::Graphics::GCI
{

	bool PipelineStateDescriptorCreateInfoBase::Validate() const noexcept
	{
		return true;
	}


	Ic3TypeInfoEnumDefine( EPipelineStateDescriptorType )
	{
		Ic3TypeInfoEnumBegin( EPipelineStateDescriptorType );
		Ic3TypeInfoEnumRegisterClassConstant( EPipelineStateDescriptorType::Invalid );
		Ic3TypeInfoEnumRegisterClassConstant( EPipelineStateDescriptorType::Unknown );
		Ic3TypeInfoEnumRegisterClassConstant( EPipelineStateDescriptorType::DTBlendState );
		Ic3TypeInfoEnumRegisterClassConstant( EPipelineStateDescriptorType::DTDepthStencilState );
		Ic3TypeInfoEnumRegisterClassConstant( EPipelineStateDescriptorType::DTRasterizerState );
		Ic3TypeInfoEnumRegisterClassConstant( EPipelineStateDescriptorType::DTGraphicsShaderLinkage );
		Ic3TypeInfoEnumRegisterClassConstant( EPipelineStateDescriptorType::DTVertexAttributeLayout );
		Ic3TypeInfoEnumRegisterClassConstant( EPipelineStateDescriptorType::DTRootSignature );
		Ic3TypeInfoEnumRegisterClassConstant( EPipelineStateDescriptorType::DTRenderPass );
		Ic3TypeInfoEnumRegisterClassConstant( EPipelineStateDescriptorType::DTRenderTarget );
		Ic3TypeInfoEnumRegisterClassConstant( EPipelineStateDescriptorType::DTVertexSourceBinding );
	}

} // namespace Ic3::Graphics::GCI
