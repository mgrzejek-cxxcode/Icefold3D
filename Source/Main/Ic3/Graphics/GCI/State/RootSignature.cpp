
#include "RootSignature.h"
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

	static bool CreateRootSignatureConstantLayout(
			const RootSignatureDesc & pRootSignatureDesc,
			RootSignature & pOutSignature );
	
	static bool CreateRootSignatureDescriptorLayout(
			const RootSignatureDesc & pRootSignatureDesc,
			RootSignature & pOutSignature );
	
	static bool ValidateRootSignatureConstantLayoutDesc(
			const RootSignatureDesc & pRootSignatureDesc,
			InputConstantLayoutInfo & pOutConstantLayoutInfo );
	
	static bool ValidateRootSignatureDescriptorLayoutDesc(
			const RootSignatureDesc & pRootSignatureDesc,
			InputDescriptorLayoutInfo & pOutDescriptorLayoutInfo );
	
	static uint32 ComputeConstantDwordSize( size_t pByteSize );

	namespace Utilities
	{

		RootSignature CreateRootSignature( const RootSignatureDesc & pRootSignatureDesc )
		{
			RootSignature rootSignature;
			rootSignature.dwordSize = 0;
			rootSignature.constantsNum = 0;
			rootSignature.descriptorsNum = 0;
			rootSignature.descriptorSetsNum = 0;
			rootSignature.parametersNum = 0;

			if( !CreateRootSignatureConstantLayout( pRootSignatureDesc, rootSignature ) )
			{
				return kInitEmpty;
			}

			if( !CreateRootSignatureDescriptorLayout( pRootSignatureDesc, rootSignature ) )
			{
				return kInitEmpty;
			}

			rootSignature.constantsNum = rootSignature.constantLayout.constantsNum;
			rootSignature.descriptorsNum = rootSignature.descriptorLayout.totalDescriptorsNum;
			rootSignature.descriptorSetsNum = rootSignature.descriptorLayout.descriptorSetsNum;
			rootSignature.parametersNum = rootSignature.constantsNum + rootSignature.descriptorsNum;
			rootSignature.dwordSize = rootSignature.constantLayout.dwordSize + rootSignature.descriptorSetsNum;

			uint32 globalParamIndex = 0;

			for( auto & constant : rootSignature.constantLayout.commonConstantArray )
			{
				constant.cParamIndex = globalParamIndex++;
				rootSignature.commonParameterList.push_back( &constant );
				rootSignature.constantMap[constant.cRefID] = &constant;
			}

			for( auto & resourceDescriptor : rootSignature.descriptorLayout.commonDescriptorArray )
			{
				resourceDescriptor.cParamIndex = globalParamIndex++;
				rootSignature.commonParameterList.push_back( &resourceDescriptor );
				rootSignature.descriptorMap[resourceDescriptor.cRefID] = &resourceDescriptor;
			}

			return rootSignature;
		}

	}

	bool CreateRootSignatureConstantLayout(
			const RootSignatureDesc & pRootSignatureDesc,
			RootSignature & pOutSignature )
	{
		InputConstantLayoutInfo constantLayoutInfo;
		if( !ValidateRootSignatureConstantLayoutDesc( pRootSignatureDesc, constantLayoutInfo ) )
		{
			return false;
		}

		RootSignature::ConstantLayout constantLayout;
		constantLayout.constantsNum = 0;
		constantLayout.dwordSize = 0;
		constantLayout.commonConstantArray.reserve( constantLayoutInfo.constantsNum );

		for( uint32 inputConstantGroupIndex = 0; inputConstantGroupIndex < pRootSignatureDesc.constantGroupsNum; ++inputConstantGroupIndex )
		{
			const auto & constantGroupDesc = pRootSignatureDesc.constantGroupArray[inputConstantGroupIndex];

			for( uint32 inputConstantIndex = 0; inputConstantIndex < constantGroupDesc.constantsNum; ++inputConstantIndex )
			{
				const auto & constantDesc = constantGroupDesc.constantList[inputConstantIndex];

				auto constantParameter = constantLayout.commonConstantArray.emplace_back();
				constantParameter.cRefID = constantDesc.refID;
				constantParameter.cParamType = EShaderInputParameterType::Constant;
				constantParameter.iFormat = constantDesc.format;
				constantParameter.iStageIndex = constantDesc.bindingIndex;
				constantParameter.iByteSize = CXU::GetVertexAttribFormatByteSize( constantDesc.format );
				constantParameter.iDwordSize = ComputeConstantDwordSize( constantParameter.iByteSize );
				constantParameter.iAccessClass = constantGroupDesc.accessClass;

				constantLayout.constantsNum += 1;
				constantLayout.dwordSize += constantParameter.iDwordSize;
			}
		}

		std::sort( constantLayout.commonConstantArray.begin(),
		           constantLayout.commonConstantArray.end(),
		           []( const ShaderInputParameterConstant & pLeft, const ShaderInputParameterConstant & pRight ) -> bool {
			           return ( pLeft.iAccessClass < pRight.iAccessClass ) ||
			                  ( ( pLeft.iAccessClass == pRight.iAccessClass ) && ( pLeft.iStageIndex < pRight.iStageIndex ) );
		           } );

		for( uint32 globalConstantIndex = 0; globalConstantIndex < constantLayout.constantsNum; ++globalConstantIndex )
		{
			auto & constant = constantLayout.commonConstantArray.at( globalConstantIndex );
			constant.iGlobalIndex = globalConstantIndex;

			if( constant.iAccessClass == EShaderConstantAccessClass::ACAllActive )
			{
				constant.iVisibilityMask = pRootSignatureDesc.activeShaderStagesMask;
			}
			else
			{
				constant.iVisibilityMask = CXU::GetShaderConstantVisibilityStageMask( constant.iAccessClass );
			}
		}

		pOutSignature.constantLayout = std::move( constantLayout );

		return true;
	}

	bool CreateRootSignatureDescriptorLayout(
			const RootSignatureDesc & pRootSignatureDesc,
			RootSignature & pOutSignature )
	{
		InputDescriptorLayoutInfo descriptorLayoutInfo;
		if( !ValidateRootSignatureDescriptorLayoutDesc( pRootSignatureDesc, descriptorLayoutInfo ) )
		{
			return false;
		}

		RootSignature::DescriptorLayout descriptorLayout;
		descriptorLayout.descriptorSetsNum = descriptorLayoutInfo.descriptorSetsNum;
		descriptorLayout.totalDescriptorsNum = descriptorLayoutInfo.descriptorsNum;
		descriptorLayout.commonDescriptorArray.reserve( descriptorLayout.totalDescriptorsNum );
		descriptorLayout.descriptorSets.reserve( descriptorLayout.descriptorSetsNum );

		for( uint32 inputDescriptorSetIndex = 0; inputDescriptorSetIndex < pRootSignatureDesc.descriptorSetsNum; ++inputDescriptorSetIndex )
		{
			const auto & descriptorSetDesc = pRootSignatureDesc.descriptorSetArray[inputDescriptorSetIndex];

			auto descriptorSetArrayOffset = descriptorLayout.commonDescriptorArray.size();
			auto descriptorSetIndex = descriptorLayout.descriptorSets.size();

			uint32 setDescriptorsNum = 0;
			cppx::bitmask<EShaderStageFlags> descriptorSetVisibilityMask = 0;

			for( uint32 inputDescriptorIndex = 0; inputDescriptorIndex < descriptorSetDesc.descriptorsNum; ++inputDescriptorIndex )
			{
				const auto & descriptorDesc = descriptorSetDesc.descriptorList[inputDescriptorIndex];

				auto & descriptor = descriptorLayout.commonDescriptorArray.emplace_back();
				descriptor.cRefID = descriptorDesc.refID;
				descriptor.dDescriptorType = descriptorSetDesc.descriptorType;
				descriptor.dSetIndex = cppx::numeric_cast<shader_input_index_t>( descriptorSetIndex );
				descriptor.dShaderVisibilityMask = descriptorDesc.shaderVisibilityMask & pRootSignatureDesc.activeShaderStagesMask;

				if( descriptorSetDesc.descriptorType == EShaderInputDescriptorType::Resource )
				{
					descriptor.cParamType = EShaderInputParameterType::Resource;
					descriptor.uResourceInfo.resourceArraySize = descriptorDesc.uResourceDesc.resourceArraySize;
					descriptor.uResourceInfo.resourceType = descriptorDesc.uResourceDesc.resourceType;
					descriptor.uResourceInfo.resourceClass = CXU::GetShaderInputResourceResourceClass( descriptorDesc.uResourceDesc.resourceType );
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
			descriptorSet.arrayOffset = cppx::numeric_cast<uint32>( descriptorSetArrayOffset );
			descriptorSet.basePtr = descriptorLayout.commonDescriptorArray.data() + descriptorSetArrayOffset;
			descriptorSet.setDescriptorsNum = setDescriptorsNum;
			descriptorSet.setIndex = cppx::numeric_cast<uint32>( descriptorSetIndex );
			descriptorSet.shaderVisibilityMask = descriptorSetVisibilityMask;
		}

		pOutSignature.descriptorLayout = std::move( descriptorLayout );

		return true;
	}

	bool ValidateRootSignatureConstantLayoutDesc(
			const RootSignatureDesc & pRootSignatureDesc,
			InputConstantLayoutInfo & pOutConstantLayoutInfo )
	{
		size_t constantsNum = 0;
		size_t totalDwordSize = 0;

		if( pRootSignatureDesc.constantGroupsNum > GCM::kShaderCombinedStagesNum + 1 )
		{
			Ic3DebugInterrupt();
			return false;
		}

		for( uint32 inputConstantGroupIndex = 0; inputConstantGroupIndex < pRootSignatureDesc.constantGroupsNum; ++inputConstantGroupIndex )
		{
			const auto & constantGroupDesc = pRootSignatureDesc.constantGroupArray[inputConstantGroupIndex];
			auto groupShaderStageMask = CXU::GetShaderConstantVisibilityStageMask( constantGroupDesc.accessClass );

			constantsNum += constantGroupDesc.constantsNum;

			if( !pRootSignatureDesc.activeShaderStagesMask.is_set( groupShaderStageMask ) )
			{
				Ic3DebugInterrupt();
				return false;
			}

			for( uint32 inputConstantIndex = 0; inputConstantIndex < constantGroupDesc.constantsNum; ++inputConstantIndex )
			{
				const auto & constantDesc = constantGroupDesc.constantList[inputConstantIndex];

				auto constantByteSize = CXU::GetVertexAttribFormatByteSize( constantDesc.format );
				auto constantDwordSize = ComputeConstantDwordSize( constantByteSize );
				totalDwordSize += constantDwordSize;

				if( totalDwordSize > GCM::kSRSMaxDwordSize )
				{
					Ic3DebugInterrupt();
					return false;
				}
			}
		}

		pOutConstantLayoutInfo.constantsNum = cppx::numeric_cast<uint32>( constantsNum );
		pOutConstantLayoutInfo.totalDwordSize = cppx::numeric_cast<uint32>( totalDwordSize );

		return true;
	}

	bool ValidateRootSignatureDescriptorLayoutDesc(
			const RootSignatureDesc & pRootSignatureDesc,
			InputDescriptorLayoutInfo & pOutDescriptorLayoutInfo )
	{
		size_t descriptorsNum = 0;
		size_t descriptorSetsNum = pRootSignatureDesc.descriptorSetsNum;

		if( descriptorSetsNum > GCM::kSRSMaxDescriptorSetsNum )
		{
			Ic3DebugInterrupt();
			return false;
		}

		for( uint32 inputDescriptorSetIndex = 0; inputDescriptorSetIndex < pRootSignatureDesc.descriptorSetsNum; ++inputDescriptorSetIndex )
		{
			const auto & descriptorSetDesc = pRootSignatureDesc.descriptorSetArray[inputDescriptorSetIndex];
			descriptorsNum += descriptorSetDesc.descriptorsNum;

			for( uint32 inputDescriptorIndex = 0; inputDescriptorIndex < descriptorSetDesc.descriptorsNum; ++inputDescriptorIndex )
			{
				const auto & descriptorDesc = descriptorSetDesc.descriptorList[inputDescriptorIndex];
				if( descriptorDesc.descriptorType != descriptorSetDesc.descriptorType )
				{
					Ic3DebugInterrupt();
					return false;
				}
			}
		}

		pOutDescriptorLayoutInfo.descriptorsNum = cppx::numeric_cast<uint32>( descriptorsNum );
		pOutDescriptorLayoutInfo.descriptorSetsNum = cppx::numeric_cast<uint32>( descriptorSetsNum );

		return true;
	}

	uint32 ComputeConstantDwordSize( size_t pByteSize )
	{
		auto dwordSize = pByteSize / sizeof( uint32 );
		auto dwordSizeMod = pByteSize % sizeof( uint32 );
		return ( dwordSizeMod == 0 ) ? cppx::numeric_cast<uint32>( dwordSize ) : cppx::numeric_cast<uint32>( dwordSize + 1 );
	}

} // namespace Ic3::Graphics::GCI
