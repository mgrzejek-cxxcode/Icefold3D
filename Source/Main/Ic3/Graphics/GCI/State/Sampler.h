
#pragma once

#ifndef __IC3_GRAPHICS_GCI_SAMPLER_H__
#define __IC3_GRAPHICS_GCI_SAMPLER_H__

#include "SamplerCommon.h"

namespace Ic3::Graphics::GCI
{

	class IC3_GRAPHICS_GCI_CLASS Sampler : public GPUDeviceChildObject
	{
	public:
		Sampler( GPUDevice & pGPUDevice );
		virtual ~Sampler();
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_SAMPLER_H__
