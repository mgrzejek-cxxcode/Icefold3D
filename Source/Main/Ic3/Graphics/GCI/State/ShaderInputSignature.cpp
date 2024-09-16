
#include "ShaderInputSignature.h"
#include "../Resources/Shader.h"

namespace Ic3::Graphics::GCI
{

	struct InputConstantLayoutInfo
	{
		uint32 constantsNum = 0;
		uint32 totalDwordSize = 0;
	};

	struct InputDescriptorLayoutInfo
	{
		uint32 descriptorsNum = 0;
		uint32 descriptorSetsNum = 0;
	};

	static bool createInputSignatureConstantLayout( const ShaderInputSignatureDesc & pInputSignatureDesc,
	                                                ShaderInputSignature & pOutSignature );
	static bool createInputSignatureDescriptorLayout( const ShaderInputSignatureDesc & pInputSignatureDesc,
	                                                  ShaderInputSignature & pOutSignature );
	static bool validateInputSignatureConstantLayoutDesc( const ShaderInputSignatureDesc & pInputSignatureDesc,
	                                                      InputConstantLayoutInfo & pOutConstantLayoutInfo );
	static bool validateInputSignatureDescriptorLayoutDesc( const ShaderInputSignatureDesc & pInputSignatureDesc,
	                                                        InputDescriptorLayoutInfo & pOutDescriptorLayoutInfo );
	static uint32 computeConstantDwordSize( size_t pByteSize );

	namespace SMU
	{

		ShaderInputSignature createShaderInputSignature( const ShaderInputSignatureDesc & pInputSignatureDesc )
		{
			ShaderInputSignature inputSignature;
			inputSignature.dwordSize = 0;
			inputSignature.constantsNum = 0;
			inputSignature.descriptorsNum = 0;
			inputSignature.descriptorSetsNum = 0;
			inputSignature.parametersNum = 0;

			if( !createInputSignatureConstantLayout( pInputSignatureDesc, inputSignature ) )
			{
				return CX_INIT_EMPTY;
			}

			if( !createInputSignatureDescriptorLayout( pInputSignatureDesc, inputSignature ) )
			{
				return CX_INIT_EMPTY;
			}

			inputSignature.constantsNum = inputSignature.constantLayout.constantsNum;
			inputSignature.descriptorsNum = inputSignature.descriptorLayout.totalDescriptorsNum;
			inputSignature.descriptorSetsNum = inputSignature.descriptorLayout.descriptorSetsNum;
			inputSignature.parametersNum = inputSignature.constantsNum + inputSignature.descriptorsNum;
			inputSignature.dwordSize = inputSignature.constantLayout.dwordSize + inputSignature.descriptorSetsNum;

			uint32 globalParamIndex = 0;

			for( auto & constant : inputSignature.constantLayout.commonConstantArray )
			{
				constant.cParamIndex = globalParamIndex++;
				inputSignature.commonParameterList.push_back( &constant );
				inputSignature.constantMap[constant.cRefID] = &constant;
			}
			for( auto & resourceDescriptor : inputSignature.descriptorLayout.commonDescriptorArray )
			{
				resourceDescriptor.cParamIndex = globalParamIndex++;
				inputSignature.commonParameterList.push_back( &resourceDescriptor );
				inputSignature.descriptorMap[resourceDescriptor.cRefID] = &resourceDescriptor;
			}

			return inputSignature;
		}

	}

	bool createInputSignatureConstantLayout( const ShaderInputSignatureDesc & pInputSignatureDesc, ShaderInputSignature & pOutSignature )
	{
		InputConstantLayoutInfo constantLayoutInfo;
		if( !validateInputSignatureConstantLayoutDesc( pInputSignatureDesc, constantLayoutInfo ) )
		{
			return false;
		}

		ShaderInputSignature::ConstantLayout constantLayout;
		constantLayout.constantsNum = 0;
		constantLayout.dwordSize = 0;
		constantLayout.commonConstantArray.reserve( constantLayoutInfo.constantsNum );

		for( uint32 inputConstantGroupIndex = 0; inputConstantGroupIndex < pInputSignatureDesc.constantGroupsNum; ++inputConstantGroupIndex )
		{
			const auto & constantGroupDesc = pInputSignatureDesc.constantGroupArray[inputConstantGroupIndex];

			for( uint32 inputConstantIndex = 0; inputConstantIndex < constantGroupDesc.constantsNum; ++inputConstantIndex )
			{
				const auto & constantDesc = constantGroupDesc.constantList[inputConstantIndex];

				auto constantParameter = constantLayout.commonConstantArray.emplace_back();
				constantParameter.cRefID = constantDesc.refID;
				constantParameter.cParamType = EShaderInputParameterType::Constant;
				constantParameter.iFormat = constantDesc.format;
				constantParameter.iStageIndex = constantDesc.bindingIndex;
				constantParameter.iByteSize = CxDef::getVertexAttribFormatByteSize( constantDesc.format );
				constantParameter.iDwordSize = computeConstantDwordSize( constantParameter.iByteSize );
				constantParameter.iAccessClass = constantGroupDesc.accessClass;

				constantLayout.constantsNum += 1;
				constantLayout.dwordSize += constantParameter.iDwordSize;
			}
		}

		std::sort( constantLayout.commonConstantArray.begin(),
		           constantLayout.commonConstantArray.end(),
		           []( const ShaderInputParameterConstant & pLeft, const ShaderInputParameterConstant & pRight ) -> bool {
			           return ( pLeft.iAccessClass < pRight.iAccessClass ) ||
			                  ( ( pLeft.iAccessClass < pRight.iAccessClass ) && ( pLeft.iStageIndex < pRight.iStageIndex ) );
		           } );

		for( uint32 globalConstantIndex = 0; globalConstantIndex < constantLayout.constantsNum; ++globalConstantIndex )
		{
			auto & constant = constantLayout.commonConstantArray.at( globalConstantIndex );
			constant.iGlobalIndex = globalConstantIndex;

			if( constant.iAccessClass == EShaderConstantAccessClass::ACAllActive )
			{
				constant.iVisibilityMask = pInputSignatureDesc.activeShaderStagesMask;
			}
			else
			{
				constant.iVisibilityMask = CxDef::getShaderConstantVisibilityStageMask( constant.iAccessClass );
			}
		}

		pOutSignature.constantLayout = std::move( constantLayout );

		return true;
	}

	bool createInputSignatureDescriptorLayout( const ShaderInputSignatureDesc & pInputSignatureDesc, ShaderInputSignature & pOutSignature )
	{
		InputDescriptorLayoutInfo descriptorLayoutInfo;
		if( !validateInputSignatureDescriptorLayoutDesc( pInputSignatureDesc, descriptorLayoutInfo ) )
		{
			return false;
		}

		ShaderInputSignature::DescriptorLayout descriptorLayout;
		descriptorLayout.descriptorSetsNum = descriptorLayoutInfo.descriptorSetsNum;
		descriptorLayout.totalDescriptorsNum = descriptorLayoutInfo.descriptorsNum;
		descriptorLayout.commonDescriptorArray.reserve( descriptorLayout.totalDescriptorsNum );
		descriptorLayout.descriptorSets.reserve( descriptorLayout.descriptorSetsNum );

		for( uint32 inputDescriptorSetIndex = 0; inputDescriptorSetIndex < pInputSignatureDesc.descriptorSetsNum; ++inputDescriptorSetIndex )
		{
			const auto & descriptorSetDesc = pInputSignatureDesc.descriptorSetArray[inputDescriptorSetIndex];

			auto descriptorSetArrayOffset = descriptorLayout.commonDescriptorArray.size();
			auto descriptorSetIndex = descriptorLayout.descriptorSets.size();

			uint32 setDescriptorsNum = 0;
			TBitmask<EShaderStageFlags> descriptorSetVisibilityMask = 0;

			for( uint32 inputDescriptorIndex = 0; inputDescriptorIndex < descriptorSetDesc.descriptorsNum; ++inputDescriptorIndex )
			{
				const auto & descriptorDesc = descriptorSetDesc.descriptorList[inputDescriptorIndex];

				auto & descriptor = descriptorLayout.commonDescriptorArray.emplace_back();
				descriptor.cRefID = descriptorDesc.refID;
				descriptor.dDescriptorType = descriptorSetDesc.descriptorType;
				descriptor.dSetIndex = numeric_cast<shader_input_index_t>( descriptorSetIndex );
				descriptor.dShaderVisibilityMask = descriptorDesc.shaderVisibilityMask & pInputSignatureDesc.activeShaderStagesMask;

				if( descriptorSetDesc.descriptorType == EShaderInputDescriptorType::Resource )
				{
					descriptor.cParamType = EShaderInputParameterType::Resource;
					descriptor.uResourceInfo.resourceArraySize = descriptorDesc.uResourceDesc.resourceArraySize;
					descriptor.uResourceInfo.resourceType = descriptorDesc.uResourceDesc.resourceType;
					descriptor.uResourceInfo.resourceClass = CxDef::getShaderInputResourceResourceClass( descriptorDesc.uResourceDesc.resourceType );
					descriptor.uResourceInfo.resourceBaseRegisterIndex = descriptorDesc.uResourceDesc.resourceBaseRegisterIndex;
				}
				else if( descriptorSetDesc.descriptorType == EShaderInputDescriptorType::Sampler )
				{
					descriptor.cParamType = EShaderInputParameterType::Sampler;
					descriptor.uSamplerInfo.samplerBindingIndex = descriptorDesc.uSamplerConfig.samplerBindingIndex;
				}

				setDescriptorsNum += 1;
				descriptorSetVisibilityMask.set( descriptor.dShaderVisibilityMask );
			}

			auto & descriptorSet = descriptorLayout.descriptorSets.emplace_back();
			descriptorSet.descriptorType = descriptorSetDesc.descriptorType;
			descriptorSet.arrayOffset = numeric_cast<uint32>( descriptorSetArrayOffset );
			descriptorSet.basePtr = descriptorLayout.commonDescriptorArray.data() + descriptorSetArrayOffset;
			descriptorSet.setDescriptorsNum = setDescriptorsNum;
			descriptorSet.setIndex = numeric_cast<uint32>( descriptorSetIndex );
			descriptorSet.shaderVisibilityMask = descriptorSetVisibilityMask;
		}

		pOutSignature.descriptorLayout = std::move( descriptorLayout );

		return true;
	}

	bool validateInputSignatureConstantLayoutDesc( const ShaderInputSignatureDesc & pInputSignatureDesc,
	                                               InputConstantLayoutInfo & pOutConstantLayoutInfo )
	{
		size_t constantsNum = 0;
		size_t totalDwordSize = 0;

		if( pInputSignatureDesc.constantGroupsNum > GCM::cxShaderCombinedStagesNum + 1 )
		{
			ic3DebugInterrupt();
			return false;
		}

		for( uint32 inputConstantGroupIndex = 0; inputConstantGroupIndex < pInputSignatureDesc.constantGroupsNum; ++inputConstantGroupIndex )
		{
			const auto & constantGroupDesc = pInputSignatureDesc.constantGroupArray[inputConstantGroupIndex];
			auto groupShaderStageMask = CxDef::getShaderConstantVisibilityStageMask( constantGroupDesc.accessClass );

			constantsNum += constantGroupDesc.constantsNum;

			if( !pInputSignatureDesc.activeShaderStagesMask.isSet( groupShaderStageMask ) )
			{
				ic3DebugInterrupt();
				return false;
			}

			for( uint32 inputConstantIndex = 0; inputConstantIndex < constantGroupDesc.constantsNum; ++inputConstantIndex )
			{
				const auto & constantDesc = constantGroupDesc.constantList[inputConstantIndex];

				auto constantByteSize = CxDef::getVertexAttribFormatByteSize( constantDesc.format );
				auto constantDwordSize = computeConstantDwordSize( constantByteSize );
				totalDwordSize += constantDwordSize;

				if( totalDwordSize > GCM::cxISMaxDwordSize )
				{
					ic3DebugInterrupt();
					return false;
				}
			}
		}

		pOutConstantLayoutInfo.constantsNum = numeric_cast<uint32>( constantsNum );
		pOutConstantLayoutInfo.totalDwordSize = numeric_cast<uint32>( totalDwordSize );

		return true;
	}

	bool validateInputSignatureDescriptorLayoutDesc( const ShaderInputSignatureDesc & pInputSignatureDesc,
	                                                 InputDescriptorLayoutInfo & pOutDescriptorLayoutInfo )
	{
		size_t descriptorsNum = 0;
		size_t descriptorSetsNum = pInputSignatureDesc.descriptorSetsNum;

		if( descriptorSetsNum > GCM::cxISMaxDescriptorSetsNum )
		{
			ic3DebugInterrupt();
			return false;
		}

		for( uint32 inputDescriptorSetIndex = 0; inputDescriptorSetIndex < pInputSignatureDesc.descriptorSetsNum; ++inputDescriptorSetIndex )
		{
			const auto & descriptorSetDesc = pInputSignatureDesc.descriptorSetArray[inputDescriptorSetIndex];
			descriptorsNum += descriptorSetDesc.descriptorsNum;

			for( uint32 inputDescriptorIndex = 0; inputDescriptorIndex < descriptorSetDesc.descriptorsNum; ++inputDescriptorIndex )
			{
				const auto & descriptorDesc = descriptorSetDesc.descriptorList[inputDescriptorIndex];
				if( descriptorDesc.descriptorType != descriptorSetDesc.descriptorType )
				{
					ic3DebugInterrupt();
					return false;
				}
			}
		}

		pOutDescriptorLayoutInfo.descriptorsNum = numeric_cast<uint32>( descriptorsNum );
		pOutDescriptorLayoutInfo.descriptorSetsNum = numeric_cast<uint32>( descriptorSetsNum );

		return true;
	}

	uint32 computeConstantDwordSize( size_t pByteSize )
	{
		auto dwordSize = pByteSize / sizeof( uint32 );
		auto dwordSizeMod = pByteSize % sizeof( uint32 );
		return ( dwordSizeMod == 0 ) ? numeric_cast<uint32>( dwordSize ) : numeric_cast<uint32>( dwordSize + 1 );
	}

} // namespace Ic3::Graphics::GCI
