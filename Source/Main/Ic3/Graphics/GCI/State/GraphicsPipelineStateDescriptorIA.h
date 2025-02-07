
#pragma once

#ifndef __IC3_GRAPHICS_GCI_INPUT_ASSEMBLER_IMMUTABLE_STATES_H__
#define __IC3_GRAPHICS_GCI_INPUT_ASSEMBLER_IMMUTABLE_STATES_H__

#include "GraphicsPipelineStateDescriptor.h"
#include "InputAssemblerCommon.h"

namespace Ic3::Graphics::GCI
{

	struct IAInputLayoutStateCommonProperties
	{
		/// Active attributes mask. It contains all bits corresponding to attributes active as part of this descriptor.
		/// @see EIAVertexAttributeFlags
		cppx::bitmask<EIAVertexAttributeFlags> activeAttributesMask;

		/// Number of active attributes enabled for the IA stage.
		native_uint activeAttributesNum;

		/// Primitive topology used to render vertices.
		EPrimitiveTopology primitiveTopology;
	};

	struct IAVertexStreamStateCommonProperties
	{
	};

	/// @brief
	class IAInputLayoutStateDescriptor : public GraphicsPipelineStateDescriptor
	{
	public:
		IAInputLayoutStateCommonProperties const mCommonProperties;

	public:
		IAInputLayoutStateDescriptor( const IAInputLayoutStateDescriptor & ) = delete;
		IAInputLayoutStateDescriptor & operator=( const IAInputLayoutStateDescriptor & ) = delete;

		IAInputLayoutStateDescriptor( GPUDevice & pGPUDevice, const IAInputLayoutStateCommonProperties & pCommonProperties );
		virtual ~IAInputLayoutStateDescriptor();

		CPPX_ATTR_NO_DISCARD virtual EGraphicsPipelineStateDescriptorType QueryStateType() const noexcept override final;
	};

	/// @brief
	class IAVertexStreamStateDescriptor : public GraphicsPipelineStateDescriptor
	{
	public:
		IAVertexStreamStateCommonProperties const mCommonProperties;

	public:
		IAVertexStreamStateDescriptor( const IAVertexStreamStateDescriptor & ) = delete;
		IAVertexStreamStateDescriptor & operator=( const IAVertexStreamStateDescriptor & ) = delete;

		IAVertexStreamStateDescriptor( GPUDevice & pGPUDevice, const IAVertexStreamStateCommonProperties & pCommonProperties );
		virtual ~IAVertexStreamStateDescriptor();

		CPPX_ATTR_NO_DISCARD virtual EGraphicsPipelineStateDescriptorType QueryStateType() const noexcept override final;

		CPPX_ATTR_NO_DISCARD static const IAVertexStreamStateDescriptor & GetDynamicOverrideState();
	};

	namespace SMU
	{

		IC3_GRAPHICS_GCI_API_NO_DISCARD IAInputLayoutStateCommonProperties GetIAInputLayoutStateCommonProperties(
				const IAInputLayoutDefinition & pInputLayoutDefinition );

		IC3_GRAPHICS_GCI_API_NO_DISCARD IAVertexStreamStateCommonProperties GetIAVertexStreamStateCommonProperties(
				const IAVertexStreamDefinition & pVertexStreamDefinition );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_INPUT_ASSEMBLER_IMMUTABLE_STATES_H__
