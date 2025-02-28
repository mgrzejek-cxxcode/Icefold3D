
#pragma once

#ifndef __IC3_NXMAIN_COMMON_RENDERER_DEFS_H__
#define __IC3_NXMAIN_COMMON_RENDERER_DEFS_H__

#include "../Prerequisites.h"
#include <Ic3/Graphics/GCI/Resources/TextureCommon.h>
#include <Ic3/Graphics/GCI/State/CommonGPUStateDefs.h>

namespace Ic3
{

	Ic3DeclareClassHandle( GpaSharedStateLibrary );
	Ic3DeclareClassHandle( ShaderLibrary );
	Ic3DeclareClassHandle( ShaderLoader );

	using GfxObjectID = Graphics::GfxObjectID;
	using GfxObjectName = Graphics::GfxObjectName;

	/// @brief
	constexpr uint32 cxGCIShaderBindingMetricCBMaxNum = GCM::kResMaxConstantBuffersNum;

	/// @brief
	constexpr uint32 cxGCIShaderBindingMetricTexMaxNum = GCM::kResMaxTextureUnitsNum;

	struct TextureUnitBinding
	{
		GCI::TextureHandle texture;
		GCI::SamplerHandle sampler;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_COMMON_RENDERER_DEFS_H__
