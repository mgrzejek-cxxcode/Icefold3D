
#pragma once

#ifndef __IC3_NXMAIN_COMMON_RENDERER_DEFS_H__
#define __IC3_NXMAIN_COMMON_RENDERER_DEFS_H__

#include "../Prerequisites.h"

#include <Ic3/Graphics/GCI/Resources/textureCommon.h>
#include <Ic3/Graphics/GCI/State/commonGPUStateDefs.h>

namespace Ic3
{

	ic3DeclareClassHandle( GpaSharedStateLibrary );
	ic3DeclareClassHandle( ShaderLibrary );
	ic3DeclareClassHandle( ShaderLoader );

	using GpaUniqueObjectID = GCI::UniqueGPUObjectID;
	using GpaUniqueObjectName = GCI::UniqueGPUObjectName;

	namespace CxDefs
	{

		constexpr uint32 GPA_SHADER_BINDING_METRIC_CB_MAX_NUM = GCI::gpm::RES_MAX_CONSTANT_BUFFERS_NUM;

		constexpr uint32 GPA_SHADER_BINDING_METRIC_TEX_MAX_NUM = GCI::gpm::RES_MAX_TEXTURE_UNITS_NUM;

	}

	struct TextureUnitBinding
	{
		GCI::TextureHandle texture;
		GCI::SamplerHandle sampler;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_COMMON_RENDERER_DEFS_H__
