
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GRAPHICS_SHADER_DEFS_H__
#define __IC3_GRAPHICS_GCI_GRAPHICS_SHADER_DEFS_H__

#include "CommonGPUStateDefs.h"
#include "../Resources/ShaderCommon.h"

namespace Ic3::Graphics::GCI
{

	struct GraphicsShaderBindingCommonConfig
	{
		///
		cppx::bitmask<EShaderStageFlags> activeStagesMask;

		CPPX_ATTR_NO_DISCARD explicit operator bool() const noexcept
		{
			return !IsEmpty();
		}

		CPPX_ATTR_NO_DISCARD uint32 GetActiveStagesNum() const noexcept
		{
			return ( activeStagesMask & eShaderStageMaskGraphicsAll ).count_bits();
		}

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept
		{
			return activeStagesMask.empty();
		}

		void ResetActiveStagesInfo() noexcept
		{
			activeStagesMask.clear();
		}
	};

	struct GraphicsShaderBinding : public GraphicsShaderBindingCommonConfig
	{
	public:
		using ShaderRefType = GraphicsShaderArray::reference;

		/// All shaders in a continuous array, indexed according to order defined by EShaderStageIndex enumeration.
		/// @see EShaderStageIndex
		GraphicsShaderArray commonShaderArray;

		/// A reference to a shader object (handle) used for the vertex stage of the shader pipeline.
		ShaderRefType vertexShader;

		/// A reference to a shader object (handle) used for the hull stage of the shader pipeline.
		ShaderRefType hullShader;

		/// A reference to a shader object (handle) used for the domain stage of the shader pipeline.
		ShaderRefType domainShader;

		/// A reference to a shader object (handle) used for the geometry stage of the shader pipeline.
		ShaderRefType geometryShader;

		/// A reference to a shader object (handle) used for the pixel stage of the shader pipeline.
		ShaderRefType pixelShader;

	public:
		GraphicsShaderBinding()
		: vertexShader( commonShaderArray[kShaderStageIndexGraphicsVertex] )
		, hullShader( commonShaderArray[kShaderStageIndexGraphicsTessHull] )
		, domainShader( commonShaderArray[kShaderStageIndexGraphicsTessDomain] )
		, geometryShader( commonShaderArray[kShaderStageIndexGraphicsGeometry] )
		, pixelShader( commonShaderArray[kShaderStageIndexGraphicsPixel] )
		{}

		GraphicsShaderBinding( const GraphicsShaderBinding & pSource )
		: commonShaderArray( pSource.commonShaderArray )
		, vertexShader( commonShaderArray[kShaderStageIndexGraphicsVertex] )
		, hullShader( commonShaderArray[kShaderStageIndexGraphicsTessHull] )
		, domainShader( commonShaderArray[kShaderStageIndexGraphicsTessDomain] )
		, geometryShader( commonShaderArray[kShaderStageIndexGraphicsGeometry] )
		, pixelShader( commonShaderArray[kShaderStageIndexGraphicsPixel] )
		{}

		GraphicsShaderBinding & operator=( const GraphicsShaderBinding & pRhs )
		{
			if( &pRhs != this )
			{
				commonShaderArray = pRhs.commonShaderArray;
				activeStagesMask = pRhs.activeStagesMask;
			}
			return *this;
		}

		CPPX_ATTR_NO_DISCARD Shader * operator[]( size_t pStageIndex ) const noexcept
		{
			Ic3DebugAssert( CXU::SHIsShaderStageIndexValid( pStageIndex ) );
			return commonShaderArray[pStageIndex].get();
		}

		CPPX_ATTR_NO_DISCARD Shader * operator[]( EShaderType pShaderType ) const noexcept
		{
			Ic3DebugAssert( CXU::SHIsShaderTypeGraphics( pShaderType ) );
			const auto stageIndex = CXU::SHGetShaderStageIndex( pShaderType );
			return commonShaderArray[stageIndex].get();
		}

		IC3_GRAPHICS_GCI_API void AddShader( Shader & pShader );

		IC3_GRAPHICS_GCI_API void AddShader( ShaderHandle pShader );

		IC3_GRAPHICS_GCI_API void SetShaders( const GraphicsShaderArray & pShaderArray );

		IC3_GRAPHICS_GCI_API void Reset() noexcept;

		IC3_GRAPHICS_GCI_API void ResetStage( uint32 pStageIndex ) noexcept;

		IC3_GRAPHICS_GCI_API void ResetStage( EShaderType pShaderType ) noexcept;

		IC3_GRAPHICS_GCI_API void UpdateActiveStagesInfo() noexcept;
	};

	struct GraphicsShaderLinkageDescriptorCreateInfo : public PipelineStateDescriptorCreateInfoBase
	{
		GraphicsShaderBinding shaderBinding;

		CPPX_ATTR_NO_DISCARD pipeline_config_hash_t GetConfigHash() const noexcept
		{
			return cppx::hash_compute<pipeline_config_hash_t::hash_algo>(
					shaderBinding.commonShaderArray,
					shaderBinding.activeStagesMask );
		}
	};

	namespace Utilities
	{

		/// @brief
		IC3_GRAPHICS_GCI_API_NO_DISCARD bool SHValidateGraphicsShaderArray( const GraphicsShaderArray & pShaderArray ) noexcept;

		/// @brief
		IC3_GRAPHICS_GCI_API_NO_DISCARD bool SHValidateGraphicsShaderBinding(
				const GraphicsShaderBinding & pBindingConfiguration ) noexcept;

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GRAPHICS_SHADER_DEFS_H__
