
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLC_SHADER_COMMON_H__
#define __IC3_GRAPHICS_HW3D_GLC_SHADER_COMMON_H__

#include "../GLPrerequisites.h"
#include <Ic3/Graphics/GCI/State/ShaderInputSignature.h>
#include <unordered_map>

namespace Ic3::Graphics::GCI
{

	enum EGLShaderProcessingFlags : enum_default_value_t
	{
		eGLShaderProcessingFlagGenerateBindingLayoutBit = 0x0001,
		eGLShaderProcessingFlagGenerateInputSignatureBit = 0x0002,
		eGLShaderProcessingMaskGenerateAll =
			eGLShaderProcessingFlagGenerateBindingLayoutBit |
			eGLShaderProcessingFlagGenerateInputSignatureBit,
	};

	struct GLShaderBindingLayout
	{
		using ElementBindingMap = std::unordered_map<std::string, GLuint>;
		ElementBindingMap attributeLocations;
		ElementBindingMap fragDataLocations;
		ElementBindingMap samplerBindings;
		ElementBindingMap uniformBlockBindings;

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept
		{
			return attributeLocations.empty() && fragDataLocations.empty() && samplerBindings.empty() && uniformBlockBindings.empty();
		}
	};

	namespace RCU
	{

		bool ProcessGLShaderSource(
				const cppx::version & pSupportedAPIVersion,
				std::string & pShaderSource,
				ShaderInputSignature & pInputSignature,
				GLShaderBindingLayout * pBindingLayout = nullptr );

		void GenerateShaderInputSignatureAndRemoveSemanticsInfo(
				const cppx::array_view<std::string> & pShaderSourceLines,
				ShaderInputSignature & pInputSignature );

		void GenerateGLShaderBindingLayoutAndRemoveExplicitLayoutSpecifiers(
				const cppx::array_view<std::string> & pShaderSourceLines,
				GLShaderBindingLayout & pBindingLayout );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLC_SHADER_COMMON_H__
