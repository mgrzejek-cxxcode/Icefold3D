
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GRAPHICS_SHADER_BINDING_IMMUTABLE_STATE_H__
#define __IC3_GRAPHICS_GCI_GRAPHICS_SHADER_BINDING_IMMUTABLE_STATE_H__

#include "GraphicsPipelineImmutableState.h"
#include "GraphicsShaderState.h"

namespace Ic3::Graphics::GCI
{

	struct GraphicsShaderLinkageCommonProperties
	{
		///
		cppx::bitmask<EShaderStageFlags> mActiveStagesMask;
		///
		uint32 mActiveStagesNum;
	};

	/// @brief
	class GraphicsShaderLinkageImmutableState : public GraphicsPipelineImmutableState
	{
	public:
		GraphicsShaderLinkageCommonProperties const mCommonProperties;

	public:
		GraphicsShaderLinkageImmutableState( const GraphicsShaderLinkageImmutableState & ) = delete;
		GraphicsShaderLinkageImmutableState & operator=( const GraphicsShaderLinkageImmutableState & ) = delete;

		GraphicsShaderLinkageImmutableState(
				GPUDevice & pGPUDevice,
				const GraphicsShaderLinkageCommonProperties & pCommonProperties );

		virtual ~GraphicsShaderLinkageImmutableState();

		CPPX_ATTR_NO_DISCARD bool IsStageActive( uint32 pStageIndex ) const noexcept
		{
			return mCommonProperties.mActiveStagesMask.is_set( CxDef::makeShaderStageBit( pStageIndex ));
		}

		CPPX_ATTR_NO_DISCARD bool IsStageActive( EShaderType pShaderType ) const noexcept
		{
			return mCommonProperties.mActiveStagesMask.is_set( CxDef::GetShaderStageBit( pShaderType ));
		}

		CPPX_ATTR_NO_DISCARD uint32 GetActiveStagesNum() const noexcept
		{
			return mCommonProperties.mActiveStagesNum;
		}

		CPPX_ATTR_NO_DISCARD virtual Shader * GetShader( size_t pIndex ) const noexcept;

		CPPX_ATTR_NO_DISCARD virtual Shader * GetShader( EShaderType pShaderType ) const noexcept;
	};

	/// @brief
	class GraphicsShaderLinkageImmutableStateSeparable : public GraphicsShaderLinkageImmutableState
	{
	public:
		GraphicsShaderSet const mShaderSet;

	public:
		GraphicsShaderLinkageImmutableStateSeparable( const GraphicsShaderLinkageImmutableStateSeparable & ) = delete;
		GraphicsShaderLinkageImmutableStateSeparable & operator=( const GraphicsShaderLinkageImmutableStateSeparable & ) = delete;

		GraphicsShaderLinkageImmutableStateSeparable(
				GPUDevice & pGPUDevice,
				const GraphicsShaderLinkageCommonProperties & pCommonProperties,
				const GraphicsShaderSet & pShaderSet );

		virtual ~GraphicsShaderLinkageImmutableStateSeparable();

		CPPX_ATTR_NO_DISCARD virtual Shader * GetShader( size_t pIndex ) const noexcept override final;

		CPPX_ATTR_NO_DISCARD virtual Shader * GetShader( EShaderType pShaderType ) const noexcept override final;
	};

	namespace SMU
	{

		IC3_GRAPHICS_GCI_API_NO_DISCARD GraphicsShaderLinkageCommonProperties GetGraphicsShaderLinkageCommonPropertiesForShaderSet(
				const GraphicsShaderSet & pShaderSet ) noexcept;

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GRAPHICS_SHADER_BINDING_IMMUTABLE_STATE_H__
