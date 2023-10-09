
#ifndef __IC3_GRAPHICS_HW3D_MTLCOMMON_FEATURE_LEVEL_H__
#define __IC3_GRAPHICS_HW3D_MTLCOMMON_FEATURE_LEVEL_H__

namespace Ic3::Graphics::GCI
{

	enum class MTLFeatureLevel : uint32
	{
		/// Common2 feature set for Metal (generic). This FL guarantees at least:
		/// - Texture arrays  (read and write)
		/// - Cube map texture arrays
		/// - Compressed volume textures
		/// - Sampler arrays
		/// - MTLSamplerState: support for comparison functions
		/// - Base vertex/instance rendering
		/// - Tesselation
		/// - Indirect draw/dispatch arguments
		/// - Indirect command buffers (rendering)
		/// - Indirect command buffers (compute)
		MFL1CommonSet2 = 0x20,

		/// Tier1 feature set for Apple GPUs. Requires at least Apple4 GPU family.
		MFL1GpuAppleTier1 = 0x41,

		/// Tier2 feature set for Apple GPUs. Requires at least Apple6 GPU family.
		MFL2GpuAppleTier2 = 0x42,

		///
		MFL2GpuMac2 = 0x52
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_MTLCOMMON_FEATURE_LEVEL_H__
