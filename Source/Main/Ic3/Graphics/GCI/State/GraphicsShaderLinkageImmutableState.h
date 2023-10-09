
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GRAPHICS_SHADER_BINDING_IMMUTABLE_STATE_H__
#define __IC3_GRAPHICS_GCI_GRAPHICS_SHADER_BINDING_IMMUTABLE_STATE_H__

#include "GraphicsPipelineImmutableState.h"
#include "GraphicsShaderState.h"

namespace Ic3::Graphics::GCI
{

	struct GraphicsShaderLinkageCommonProperties
	{
		Bitmask<EShaderStageFlags> activeStagesMask;
		uint32 activeStagesNum;

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

		IC3_ATTR_NO_DISCARD bool isStageActive( uint32 pStageIndex ) const noexcept
		{
			return mCommonProperties.activeStagesMask.isSet( CxDefs::makeShaderStageBit( pStageIndex ) );
		}

		IC3_ATTR_NO_DISCARD bool isStageActive( EShaderType pShaderType ) const noexcept
		{
			return mCommonProperties.activeStagesMask.isSet( CxDefs::getShaderStageBit( pShaderType ) );
		}

		IC3_ATTR_NO_DISCARD uint32 getActiveStagesNum() const noexcept
		{
			return mCommonProperties.activeStagesNum;
		}

		IC3_ATTR_NO_DISCARD virtual Shader * getShader( size_t pIndex ) const noexcept;

		IC3_ATTR_NO_DISCARD virtual Shader * getShader( EShaderType pShaderType ) const noexcept;
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

		IC3_ATTR_NO_DISCARD virtual Shader * getShader( size_t pIndex ) const noexcept override final;

		IC3_ATTR_NO_DISCARD virtual Shader * getShader( EShaderType pShaderType ) const noexcept override final;
	};

	namespace smutil
	{

		IC3_GRAPHICS_GCI_API_NO_DISCARD GraphicsShaderLinkageCommonProperties getGraphicsShaderLinkageCommonPropertiesForShaderSet(
				const GraphicsShaderSet & pShaderSet ) noexcept;

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GRAPHICS_SHADER_BINDING_IMMUTABLE_STATE_H__
