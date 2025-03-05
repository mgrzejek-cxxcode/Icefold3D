
#pragma once

#ifndef __IC3_GRAPHICS_GCI_COMMON_RESOURCE_DEFS_H__
#define __IC3_GRAPHICS_GCI_COMMON_RESOURCE_DEFS_H__

#include "../Memory/CommonGPUMemoryDefs.h"

namespace Ic3::Graphics::GCI
{

	Ic3GCIDeclareClassHandle( GPUResource );
	Ic3GCIDeclareClassHandle( GPUBuffer );
	Ic3GCIDeclareClassHandle( Sampler );
	Ic3GCIDeclareClassHandle( Shader );
	Ic3GCIDeclareClassHandle( Texture );

	struct GPUBufferCreateInfo;
	struct SamplerCreateInfo;
	struct ShaderCreateInfo;
	struct TextureCreateInfo;

	struct GPUBufferDataCopyDesc;
	struct GPUBufferSubDataCopyDesc;
	struct GPUBufferDataUploadDesc;
	struct GPUBufferSubDataUploadDesc;

	using resource_flags_value_t = uint32;
	using resource_id_t = uint64;

	namespace CXU
	{

		/// A special constant which can be used for resources IDs to indicate that ID should be assigned automatically.
		/// In most cases it is safe to assume that object address will be used as the ID (unless stated otherwise).
		inline constexpr resource_id_t RESOURCE_ID_AUTO = cppx::meta::limits<uint64>::max_value - 1;

		/// An invalid resource ID. Such IDs may refer to resources which are either uninitialised, marked for deletion,
		/// or do not yet exist in the resource management system. This ID also means "not found" in case of queries.
		inline constexpr resource_id_t RESOURCE_ID_INVALID = cppx::meta::limits<uint64>::max_value;

	}

	/// @brief
	enum class EGPUResourceBaseType : enum_default_value_t
	{
		Buffer,
		Texture,
		Unknown
	};

	/// @brief
	enum EGPUResourceContentFlags : resource_flags_value_t
	{
		// Specifies dynamic content, i.e. content expected to be updated frequently, possibly multiple
		// times per single frame. This flag should be combined with CPU_WRITE access to enable resource
		// mapping and effective write modes like Append or NoOverwrite. Cannot be combined with neither
		// IMMUTABLE nor STATIC usages.
		eGPUResourceContentFlagDynamicBit    = 0x1,

		// Specifies immutable content which must me specified at the creation time and cannot be altered
		// afterwards. No CPU access is allowed for resources created with this flag. Cannot be combined
		// with DYNAMIC usage. Although IMMUTABLE|STATIC is a valid combination, this effectively causes
		// the resource to be created as IMMUTABLE.
		eGPUResourceContentFlagImmutableBit  = 0x2,

		// Specifies static content, specified either at the creation time or afterwards via direct upload
		// API (DMA transfer or upload heap, depending on the driver). Resources with static content are
		// generally advised to have no CPU access specified, as this can prevent certain optimisations
		// and reduce the performance. Cannot be combined with DYNAMIC usage. Although STATIC|IMMUTABLE
		// is a valid combination, this effectively causes the resource to be created as IMMUTABLE.
		eGPUResourceContentFlagStaticBit     = 0x4,

		// Additional flag which can be combined with all three basic usage modes. It describes a resource
		// which will be used only temporarily (at most few frames), which can help the runtime to select
		// the best memory type and allocation strategy. Final effect depends on the resource type and
		// memory availability:
		// - DYNAMIC resources are most likely to end up in the host memory (RAM)
		// - for IMMUTABLE/STATIC resources, the heap type used for allocation will depend on additional
		//   flags specified for the resource (like required CPU/GPU access and explicit memory preferences).
		//   For small temporary resources, host memory can be selected due to more sophisticated allocation
		//   strategies for avoiding the fragmentation.
		eGPUResourceContentFlagTemporaryBit  = 0x8,

		// Mask with all valid CONTENT_FLAG bits set.
		eGPUResourceContentMaskAll           = 0xF,
	};

	/// @brief
	enum EGPUResourceUsageFlags : resource_flags_value_t
	{
		eGPUResourceUsageFlagVertexStreamBit   = 0x0010,

		// Resource can be bound to one or more shader stages as an input (read-only) resource.
		// Can be used for both buffers and textures.
		eGPUResourceUsageFlagShaderInputBit    = 0x0020,

		// Resource can be bound to one or more shader stages as a read/write resource supporting
		// unordered access. Can be used for both buffers and textures.
		eGPUResourceUsageFlagShaderUavBit      = 0x0040,

		// Resource can be used as a color attachment in the render target state. This enables writing
		// to such resource in the pixel shader stage. RT resources require a color-compatible format.
		eGPUResourceUsageFlagRenderTargetColorBit = 0x0100,

		//
		eGPUResourceUsageFlagRenderTargetDepthBit = 0x0200,

		//
		eGPUResourceUsageFlagRenderTargetStencilBit = 0x0400,

		// Resource can be used as a depth/stencil attachment in the render target state. This enables
		// writing to such resource in the pixel shader stage and using it as depth/stencil buffer in
		// the depth and/or stencil tests. DS resources require a depth/stencil-compatible format.
		eGPUResourceUsageMaskRenderTargetDepthStencil =
			eGPUResourceUsageFlagRenderTargetStencilBit |
			eGPUResourceUsageFlagRenderTargetDepthBit,

		// Resource can be used as a source in transfer operations. Typical usage will be an upload
		// resource with CPU_WRITE access, used to write the data and copy it to the target resource.
		eGPUResourceUsageFlagTransferSourceBit = 0x1000,

		// Resource can be used as a target in transfer operations. This flag will be typically set
		// for all resources which can be updated via a dedicated upload resource (e.g. those without
		// CPU_WRITE access).
		eGPUResourceUsageFlagTransferTargetBit = 0x2000,

		//
		eGPUResourceUsageFlagsValidBuffers =
			eGPUResourceUsageFlagVertexStreamBit |
			eGPUResourceUsageFlagShaderInputBit |
			eGPUResourceUsageFlagShaderUavBit |
			eGPUResourceUsageFlagTransferSourceBit |
			eGPUResourceUsageFlagTransferTargetBit,

		//
		eGPUResourceUsageFlagsValidTextures =
			eGPUResourceUsageFlagShaderInputBit |
			eGPUResourceUsageFlagShaderUavBit |
			eGPUResourceUsageFlagRenderTargetColorBit |
			eGPUResourceUsageFlagRenderTargetDepthBit |
			eGPUResourceUsageMaskRenderTargetDepthStencil |
			eGPUResourceUsageFlagTransferSourceBit |
			eGPUResourceUsageFlagTransferTargetBit,

		// Mask with all valid USAGE_FLAG bits set.
		eGPUResourceUsageMaskAll      = 0xFFF0,
	};

	/// @brief A set of pre-defined memory bit masks for most common scenarios.
	enum EGPUResourceMemoryFlags : resource_flags_value_t
	{
		eGPUResourceMemoryMaskDefault =
			eGPUMemoryAccessFlagGPUReadBit |
			eGPUMemoryHeapPropertyFlagCPUCachedBit,

		eGPUResourceMemoryMaskConstantBuffer =
			eGPUMemoryAccessFlagCPUWriteBit |
			eGPUMemoryAccessFlagGPUReadBit |
			eGPUMemoryHeapPropertyFlagCPUCoherentBit,

		eGPUResourceMemoryMaskVertexStreamBufferDynamic =
			eGPUMemoryAccessFlagCPUWriteBit |
			eGPUMemoryAccessFlagGPUReadBit |
			eGPUMemoryHeapPropertyFlagCPUCachedBit,

		eGPUResourceMemoryMaskVertexStreamBufferStatic =
			eGPUMemoryAccessFlagGPUReadBit |
			eGPUMemoryHeapPropertyFlagCPUCachedBit,

		eGPUResourceMemoryMaskTransferSourceBuffer =
			eGPUMemoryAccessFlagCPUWriteBit |
			eGPUMemoryHeapPropertyFlagCPUCachedBit,

		eGPUResourceMemoryMaskTransferTargetBuffer =
			eGPUMemoryAccessFlagGPUReadBit,
	};

	enum class EResourceUsageState : enum_default_value_t
	{
		Undefined,
		CommonCopySource,
		CommonCopyTarget,
		RTColorAttachment,
		RTDepthOnlyAttachment,
		RTDepthStencilAttachment,
		RTDepth
	};

	struct ResourceInputDataDesc
	{
		const void * pointer = nullptr;

		gpu_memory_size_t size = 0;

		constexpr explicit operator bool() const
		{
			return pointer && ( size > 0 );
		}
	};

	struct ResourceCreateInfo
	{
		cppx::bitmask<resource_flags_value_t> resourceFlags = 0;

		memory_align_t memoryBaseAlignment = kMemoryCPUDefaultAlignment;

		cppx::bitmask<EGPUMemoryFlags> memoryFlags = eGPUResourceMemoryMaskDefault;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_COMMON_RESOURCE_DEFS_H__
