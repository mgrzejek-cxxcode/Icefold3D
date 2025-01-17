
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_SAMPLER_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_SAMPLER_H__

#include "../Objects/GLSamplerObject.h"
#include <Ic3/Graphics/GCI/State/Sampler.h>

namespace Ic3::Graphics::GCI
{

	ic3DeclareClassHandle( GLSampler );

	class  GLSampler : public Sampler
	{
	public:
		GLSamplerObjectHandle const mGLSamplerObject;

	public:
		GLSampler( GLGpuDevice & pGpuDevice, GLSamplerObjectHandle pGLSamplerObject );
		virtual ~GLSampler();

		static GLSamplerHandle CreateSampler( GLGpuDevice & pGpuDevice, const SamplerCreateInfo & pCreateInfo );

	private:
		static bool TranslateSamplerConfig( const SamplerConfig & pSamplerConfig, GLSamplerState & pOutSamplerState );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_SAMPLER_H__
