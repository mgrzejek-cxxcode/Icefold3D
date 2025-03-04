
#pragma once

#ifndef __IC3_GRAPHICS_GCI_SHADER_INPUT_SIGNATURE_H__
#define __IC3_GRAPHICS_GCI_SHADER_INPUT_SIGNATURE_H__

#include "../Resources/ShaderCommon.h"
#include "InputAssemblerCommon.h"

namespace Ic3::Graphics::GCI
{

	struct ShaderAttributeFormatDesc
	{
		EBaseDataType baseType = EBaseDataType::Undefined;
		uint8 baseComponentsNum;
		uint8 semanticGroupSize;

		explicit operator bool() const noexcept
		{
			return baseType != EBaseDataType::Undefined;
		}
	};

	struct ShaderAttributeSignature
	{
		EVertexAttribFormat format = EVertexAttribFormat::Undefined;

		uint16 semanticIndex = 0;

		cppx::immutable_string semanticName;

		CPPX_ATTR_NO_DISCARD explicit operator bool() const
		{
			return IsActive();
		}

		CPPX_ATTR_NO_DISCARD bool IsActive() const
		{
			return ( format != EVertexAttribFormat::Undefined ) && !semanticName.empty();
		}
	};

	struct ShaderInputSignature
	{
		using AttributeSignatureArray = std::array<ShaderAttributeSignature, GCM::kIAMaxVertexAttributesNum>;

		AttributeSignatureArray attributeSignatures;

		cppx::bitmask<EIAVertexAttributeFlags> activeAttributesMask;

		CPPX_ATTR_NO_DISCARD explicit operator bool() const
		{
			return !IsEmpty();
		}

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const
		{
			return activeAttributesMask.empty();
		}
	};

	namespace GCU
	{

		/// @brief
		IC3_GRAPHICS_GCI_API_NO_DISCARD bool ValidateVertexAttributeLayoutDefinitionWithShaderInputSignature(
				const IAVertexAttributeLayoutDefinition & pLayoutDefinition,
				const ShaderInputSignature & pShaderInputSignature );

		/// @brief
		IC3_GRAPHICS_GCI_API_NO_DISCARD cppx::immutable_string GenerateShaderInputSignatureSerialString(
				const ShaderInputSignature & pShaderInputSignature );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_SHADER_INPUT_SIGNATURE_H__
