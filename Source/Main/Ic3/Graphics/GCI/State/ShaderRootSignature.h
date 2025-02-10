
#pragma once

#ifndef __IC3_GRAPHICS_GCI_SHADER_ROOT_SIGNATURE_H__
#define __IC3_GRAPHICS_GCI_SHADER_ROOT_SIGNATURE_H__

#include "CommonGPUStateDefs.h"
#include "../Resources/ShaderCommon.h"
#include <unordered_map>

namespace Ic3::Graphics::GCI
{

	enum EShaderConstantAccessClass : uint32
	{
		ACAllActive  = eShaderStageMaskAll,
		ACGSVertex   = eShaderStageFlagGraphicsVertexBit,
		ACGSHull     = eShaderStageFlagGraphicsHullBit,
		ACGSDomain   = eShaderStageFlagGraphicsDomainBit,
		ACGSGeometry = eShaderStageFlagGraphicsGeometryBit,
		ACGSPixel    = eShaderStageFlagGraphicsPixelBit,
		ACCSCompute  = eShaderStageFlagComputeBit,
	};

	enum class EShaderInputParameterType : uint16
	{
		// D3D12: root constants
		// Vulkan: push constants
		// Others: API-specific (explicit direct constants or implicit constant buffer)
		Constant,

		// CBV/SRV/UAV
		Resource,

		//
		Sampler
	};

	enum class EShaderInputDescriptorType : uint16
	{
		// CBV/SRV/UAV
		Resource = 1,
		//
		Sampler,
	};

	enum class EShaderInputResourceClass : uint16
	{
		//
		CBV = 1,
		//
		SRV,
		//
		UAV,
		//
		Unknown = 0,
	};

	namespace CxDef
	{

		inline constexpr uint16 declareShaderInputResourceType( EShaderInputResourceClass pResourceClass, uint8 pIndex )
		{
			return ( ( static_cast<uint16>( pResourceClass ) << 8u ) | pIndex );
		}

	}

	enum class EShaderInputResourceType : uint16
	{
		CBVConstantBuffer = CxDef::declareShaderInputResourceType( EShaderInputResourceClass::CBV, 0 ),
		SRVTextureBuffer  = CxDef::declareShaderInputResourceType( EShaderInputResourceClass::SRV, 1 ),
		SRVTextureImage   = CxDef::declareShaderInputResourceType( EShaderInputResourceClass::SRV, 2 ),
		UAVStorageBuffer  = CxDef::declareShaderInputResourceType( EShaderInputResourceClass::UAV, 3 ),
		UAVStorageImage   = CxDef::declareShaderInputResourceType( EShaderInputResourceClass::UAV, 4 ),
		Unknown           = 0
	};

	namespace CxDef
	{

		inline constexpr EShaderInputResourceClass GetShaderInputResourceResourceClass( EShaderInputResourceType pResourceType )
		{
			return static_cast<EShaderInputResourceClass>( ( static_cast<uint32>( pResourceType ) >> 8u ) & 0xFFu );
		}

		inline constexpr cppx::bitmask<EShaderStageFlags> GetShaderConstantVisibilityStageMask( EShaderConstantAccessClass pAccessClass )
		{
			return static_cast<uint32>( pAccessClass );
		}

	}

	struct ShaderRootSignatureDesc
	{
		struct ConstantDesc
		{
			shader_input_index_t bindingIndex;
			shader_input_ref_id_t refID;
			EShaderInputConstantFormat format;
		};

		struct ConstantGroup
		{
			using ConstantList = std::array<ConstantDesc, GCM::kSRSMaxConstantGroupSize>;
			EShaderConstantAccessClass accessClass;
			ConstantList constantList;
			uint32 constantsNum = 0;
		};

		struct DescriptorDesc
		{
			struct ResourceDesc
			{
				EShaderInputResourceType resourceType;
				shader_input_index_t resourceBaseRegisterIndex;
				shader_input_index_t resourceArraySize;
			};

			struct SamplerConfig
			{
				shader_input_index_t samplerBindingIndex;
			};

			shader_input_ref_id_t refID;
			EShaderInputDescriptorType descriptorType;
			cppx::bitmask<EShaderStageFlags> shaderVisibilityMask;

			union
			{
				ResourceDesc uResourceDesc;
				SamplerConfig uSamplerConfig;
			};
		};

		struct DescriptorSet
		{
			using DescriptorList = std::array<DescriptorDesc, GCM::kSRSMaxDescriptorSetSize>;
			EShaderInputDescriptorType descriptorType;
			DescriptorList descriptorList;
			uint32 descriptorsNum = 0;
		};

		using ConstantGroupArray = std::array<ConstantGroup, GCM::kShaderCombinedStagesNum>;
		using DescriptorSetArray = std::array<DescriptorSet, GCM::kSRSMaxDescriptorSetsNum>;

		cppx::bitmask<EShaderStageFlags> activeShaderStagesMask;
		ConstantGroupArray constantGroupArray;
		uint32 constantGroupsNum = 0;
		DescriptorSetArray descriptorSetArray;
		uint32 descriptorSetsNum = 0;
	};

	struct ShaderInputParameterBase
	{
		shader_input_ref_id_t cRefID;
		shader_input_index_t cParamIndex;
		EShaderInputParameterType cParamType;
	};

	struct ShaderInputParameterConstant : public ShaderInputParameterBase
	{
		uint32 iByteSize;
		uint32 iDwordSize;
		EShaderInputConstantFormat iFormat;
		shader_input_index_t iGlobalIndex;
		shader_input_index_t iStageIndex;
		EShaderConstantAccessClass iAccessClass;
		cppx::bitmask<EShaderStageFlags> iVisibilityMask;
	};

	struct ShaderInputParameterDescriptor : public ShaderInputParameterBase
	{
		struct ResourceInfo
		{
			EShaderInputResourceType resourceType;
			EShaderInputResourceClass resourceClass;
			shader_input_index_t resourceArraySize;
			shader_input_index_t resourceBaseRegisterIndex;
		};

		struct SamplerInfo
		{
			shader_input_index_t samplerBindingIndex;
		};

		EShaderInputDescriptorType dDescriptorType;
		shader_input_index_t dSetIndex;
		cppx::bitmask<EShaderStageFlags> dShaderVisibilityMask;

		union
		{
			ResourceInfo uResourceInfo;
			SamplerInfo uSamplerInfo;
		};
	};

	struct ShaderRootSignature
	{
		struct ConstantLayout
		{
			using CommonConstantArray = std::vector<ShaderInputParameterConstant>;

			cppx::bitmask<EShaderStageFlags> activeShaderStageMask;
			CommonConstantArray commonConstantArray;
			uint32 constantsNum;
			uint32 dwordSize;

			explicit operator bool() const
			{
				return !commonConstantArray.empty();
			}
		};

		struct DescriptorLayout
		{
			struct DescriptorSet
			{
				EShaderInputDescriptorType descriptorType;
				ShaderInputParameterDescriptor * basePtr;
				uint32 arrayOffset;
				uint32 setDescriptorsNum;
				uint32 setIndex;
				cppx::bitmask<EShaderStageFlags> shaderVisibilityMask;
			};

			using CommonDescriptorArray = std::vector<ShaderInputParameterDescriptor>;
			using DescriptorSetList = std::vector<DescriptorSet>;

			CommonDescriptorArray commonDescriptorArray;
			DescriptorSetList descriptorSets;
			uint32 descriptorSetsNum;
			uint32 totalDescriptorsNum;

			explicit operator bool() const
			{
				return !commonDescriptorArray.empty();
			}
		};

		using CommonParameterList = std::vector<ShaderInputParameterBase *>;
		using ConstantMap = std::unordered_map<shader_input_ref_id_t, ShaderInputParameterConstant *>;
		using DescriptorMap = std::unordered_map<shader_input_ref_id_t, ShaderInputParameterDescriptor *>;

		cppx::bitmask<EShaderStageFlags> activeShaderStagesMask;

		//
		CommonParameterList commonParameterList;

		//
		ConstantLayout constantLayout;

		//
		ConstantMap constantMap;

		//
		DescriptorLayout descriptorLayout;

		//
		DescriptorMap descriptorMap;

		uint32 dwordSize;
		uint32 constantsNum;
		uint32 descriptorsNum;
		uint32 descriptorSetsNum;
		uint32 parametersNum;

		ShaderRootSignature() = default;

		ShaderRootSignature( const InitEmptyTag & )
        : dwordSize( 0 )
		{}

		explicit operator bool() const
		{
			return ( dwordSize > 0 ) && !commonParameterList.empty();
		}

		const ShaderInputParameterConstant & GetConstantInfo( shader_input_ref_id_t pParamRefID ) const
		{
			const auto * constantInfoPtr = constantMap.at( pParamRefID );
			return *constantInfoPtr;
		}

		const ShaderInputParameterDescriptor & GetDescriptorInfo( shader_input_ref_id_t pParamRefID ) const
		{
			const auto * descriptorInfoPtr = descriptorMap.at( pParamRefID );
			return *descriptorInfoPtr;
		}
	};

	struct ShaderRootSignatureStateDescriptorCreateInfo : public PipelineStateDescriptorCreateInfoBase
	{
		ShaderRootSignatureDesc rootSignatureDesc;

		CPPX_ATTR_NO_DISCARD pipeline_config_hash_t GetConfigHash() const noexcept
		{
			return cppx::hash_compute<pipeline_config_hash_t::hash_algo>( rootSignatureDesc );
		}
	};

	namespace GCU
	{

		IC3_GRAPHICS_GCI_API ShaderRootSignature CreateShaderRootSignature( const ShaderRootSignatureDesc & pSignatureDesc );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_SHADER_ROOT_SIGNATURE_H__
