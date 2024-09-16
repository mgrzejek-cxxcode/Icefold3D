
#pragma once

#ifndef __IC3_GRAPHICS_GCI_INPUT_ASSEMBLER_IMMUTABLE_STATES_H__
#define __IC3_GRAPHICS_GCI_INPUT_ASSEMBLER_IMMUTABLE_STATES_H__

#include "GraphicsPipelineImmutableState.h"
#include "InputAssemblerCommon.h"

namespace Ic3::Graphics::GCI
{

	struct IAInputLayoutStateCommonProperties
	{
		/// Active attributes mask. It contains all bits corresponding to attributes active as part of this descriptor.
		/// @see EIAVertexAttributeFlags
		TBitmask<EIAVertexAttributeFlags> activeAttributesMask;

		/// Number of active attributes enabled for the IA stage.
		native_uint activeAttributesNum;

		/// Primitive topology used to render vertices.
		EPrimitiveTopology primitiveTopology;
	};

	struct IAVertexStreamStateCommonProperties
	{
	};

	/// @brief
	class IAInputLayoutImmutableState : public GraphicsPipelineImmutableState
	{
	public:
		IAInputLayoutStateCommonProperties const mCommonProperties;

	public:
		IAInputLayoutImmutableState( const IAInputLayoutImmutableState & ) = delete;
		IAInputLayoutImmutableState & operator=( const IAInputLayoutImmutableState & ) = delete;

		IAInputLayoutImmutableState( GPUDevice & pGPUDevice, const IAInputLayoutStateCommonProperties & pCommonProperties );
		virtual ~IAInputLayoutImmutableState();

		IC3_ATTR_NO_DISCARD virtual EGraphicsPipelineImmutableStateType queryStateType() const noexcept override final;
	};

	/// @brief
	class IAVertexStreamImmutableState : public GraphicsPipelineImmutableState
	{
	public:
		IAVertexStreamStateCommonProperties const mCommonProperties;

	public:
		IAVertexStreamImmutableState( const IAVertexStreamImmutableState & ) = delete;
		IAVertexStreamImmutableState & operator=( const IAVertexStreamImmutableState & ) = delete;

		IAVertexStreamImmutableState( GPUDevice & pGPUDevice, const IAVertexStreamStateCommonProperties & pCommonProperties );
		virtual ~IAVertexStreamImmutableState();

		IC3_ATTR_NO_DISCARD virtual EGraphicsPipelineImmutableStateType queryStateType() const noexcept override final;

		IC3_ATTR_NO_DISCARD static const IAVertexStreamImmutableState & getDynamicOverrideState();
	};

	namespace SMU
	{

		IC3_GRAPHICS_GCI_API_NO_DISCARD IAInputLayoutStateCommonProperties getIAInputLayoutStateCommonProperties(
				const IAInputLayoutDefinition & pInputLayoutDefinition );

		IC3_GRAPHICS_GCI_API_NO_DISCARD IAVertexStreamStateCommonProperties getIAVertexStreamStateCommonProperties(
				const IAVertexStreamDefinition & pVertexStreamDefinition );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_INPUT_ASSEMBLER_IMMUTABLE_STATES_H__
