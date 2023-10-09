
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DXCOMMON_COMMON_SHADER_DEFS_H__
#define __IC3_GRAPHICS_HW3D_DXCOMMON_COMMON_SHADER_DEFS_H__

namespace Ic3::Graphics::GCI
{

	enum class DXShaderTarget : uint64
	{
		SM_4_1_VS,
		SM_4_1_GS,
		SM_4_1_PS,
		SM_4_1_CS,
		SM_5_0_VS,
		SM_5_0_HS,
		SM_5_0_DS,
		SM_5_0_GS,
		SM_5_0_PS,
		SM_5_0_CS,
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DXCOMMON_COMMON_SHADER_DEFS_H__
