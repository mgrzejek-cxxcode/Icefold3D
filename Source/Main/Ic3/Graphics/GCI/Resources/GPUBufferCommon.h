
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_BUFFER_COMMON_H__
#define __IC3_GRAPHICS_GCI_GPU_BUFFER_COMMON_H__

#include "CommonGPUResourceDefs.h"

namespace Ic3::Graphics::GCI
{

	Ic3GCIDeclareClassHandle( GPUBuffer );

	enum EGPUBufferDataCopyFlags : uint32
	{
		eGPUBufferDataCopyFlagModeAppendBit     = eGPUMemoryMapFlagWriteAppendBit,
		eGPUBufferDataCopyFlagModeInvalidateBit = eGPUMemoryMapFlagWriteInvalidateBit,
		eGPUBufferDataCopyFlagsDefault = 0,
	};

	/// @brief Bind flags for buffer resources. Used to specify how buffers are bound to the pipeline.
	///
	/// Note:
	/// Certain bind targets may also imply something about how the resource will be used in general. For example,
	/// vertex and index buffers should naturally have E_GPU_RESOURCE_USAGE_FLAG_VERTEX_STREAM_BIT set as well as a
	/// part of their properties. To ensure that is the case (and to reduce the amount of flags specified at creation
	/// time), these bind flags also have all required USAGE bits set as well.
	enum EGPUBufferBindFlags : resource_flags_value_t
	{
		// Bind flag for using a buffer as a constant buffer in one or more shader stages.
		// Such buffer is accessible via one of the valid CB input registers.
		// Implies SHADER_INPUT usage bit.
		eGPUBufferBindFlagConstantBufferBit          = 0x010000 | eGPUResourceUsageFlagShaderInputBit,

		// Bind flag for using a buffer as a source for vertex data fetched during execution of all rendering commands.
		// Such buffer is bound to the pipeline via TODO.
		// Implies VERTEX_STREAM usage bit.
		eGPUBufferBindFlagVertexBufferBit            = 0x020000 | eGPUResourceUsageFlagVertexStreamBit,

		// Bind flag for using a buffer as a source for index data fetched during execution of indexed rendering commands.
		// Such buffer is bound to the pipeline via TODO.
		// Implies VERTEX_STREAM usage bit.
		eGPUBufferBindFlagIndexBufferBit             = 0x040000 | eGPUResourceUsageFlagVertexStreamBit,

		// Bind flag for using a buffer as a target storage for stream output functionality.
		eGPUBufferBindFlagStreamOutputBufferBit     = 0x080000,

		// Bind flag for using a buffer as a general read-only resource.
		eGPUBufferBindFlagShaderInputBufferBit      = 0x100000 | eGPUResourceUsageFlagShaderInputBit,
		// Implies SHADER_INPUT usage bit.

		// Bind flag for using a buffer as a general read/write resource supporting unordered access.
		eGPUBufferBindFlagShaderUAVBufferBit        = 0x200000 | eGPUResourceUsageFlagShaderUavBit,
		// Implies SHADER_UAV usage bit.

		// Bind flag for using a buffer as a source for arguments to indirect dispatch commands.
		eGPUBufferBindFlagIndirectDispatchBufferBit = 0x400000,

		// Bind flag for using a buffer as a source for arguments to indirect draw commands.
		eGPUBufferBindFlagIndirectDrawBufferBit     = 0x800000,

		// Bind flag for using a buffer as a source in transfer operations. @See E_GPU_RESOURCE_USAGE_FLAG_TRANSFER_SOURCE_BIT.
		eGPUBufferBindFlagTransferSourceBufferBit   = eGPUResourceUsageFlagTransferSourceBit,

		// Bind flag for using a buffer as a target in transfer operations. @See E_GPU_RESOURCE_USAGE_FLAG_TRANSFER_TARGET_BIT.
		eGPUBufferBindFlagTransferTargetBufferBit   = eGPUResourceUsageFlagTransferTargetBit,

		// Mask with all valid BIND_FLAG bits set.
		eGPUBufferBindMaskAll = 0xFF0000 | eGPUResourceUsageMaskAll,
	};

	/// @brief Represents all valid targets for buffer resources. Corresponding E_GPU_BUFFER_BIND_FLAGs are used as values.
	enum class EGPUBufferTarget : uint32
	{
		Unknown                = 0,
		ConstantBuffer         = eGPUBufferBindFlagConstantBufferBit,
		VertexBuffer           = eGPUBufferBindFlagVertexBufferBit,
		IndexBuffer            = eGPUBufferBindFlagIndexBufferBit,
		StreamOutputBuffer     = eGPUBufferBindFlagStreamOutputBufferBit,
		ShaderInputBuffer      = eGPUBufferBindFlagShaderInputBufferBit,
		ShaderUAVBuffer        = eGPUBufferBindFlagShaderUAVBufferBit,
		IndirectDispatchBuffer = eGPUBufferBindFlagIndirectDispatchBufferBit,
		IndirectDrawBuffer     = eGPUBufferBindFlagIndirectDrawBufferBit,
		TransferSourceBuffer   = eGPUBufferBindFlagTransferSourceBufferBit,
		TransferTargetBuffer   = eGPUBufferBindFlagTransferTargetBufferBit,
	};

	struct GPUBufferInitDataDesc : public ResourceInputDataDesc
	{
	};

	struct GPUBufferCreateInfo : public ResourceCreateInfo
	{
		// Total size of the buffer, in bytes. The buffer, if created, will have at least the specified size.
		// Additional alignment rules and memory requirements may cause the buffer to be larger than requested.
		gpu_memory_size_t bufferSize = 0;

		// Initial data for the buffer. If the buffer is created with E_GPU_RESOURCE_CONTENT_FLAG_IMMUTABLE_BIT,
		// this has to contain a valid data specification.
		GPUBufferInitDataDesc initDataDesc;
	};

	struct GPUBufferDataCopyDesc
	{
		cppx::bitmask<EGPUBufferDataCopyFlags> flags = eGPUBufferDataCopyFlagsDefault;
	};

	struct GPUBufferSubDataCopyDesc
	{
		GPUMemoryRegion sourceBufferRegion;
		gpu_memory_size_t targetBufferOffset = cxGPUMemoryOffsetInvalid;
		cppx::bitmask<EGPUBufferDataCopyFlags> flags = eGPUBufferDataCopyFlagsDefault;
	};

	struct GPUBufferDataUploadDesc
	{
		ResourceInputDataDesc inputDataDesc;
		cppx::bitmask<EGPUBufferDataCopyFlags> flags = eGPUBufferDataCopyFlagsDefault;
	};

	struct GPUBufferSubDataUploadDesc
	{
		GPUMemoryRegion bufferRegion;
		ResourceInputDataDesc inputDataDesc;
		cppx::bitmask<EGPUBufferDataCopyFlags> flags = eGPUBufferDataCopyFlagsDefault;
	};

	namespace RCU
	{

		IC3_GRAPHICS_GCI_API_NO_DISCARD EGPUBufferTarget GetGPUBufferDefaultTargetFromBindFlags( cppx::bitmask<resource_flags_value_t> pBindFlags );

		IC3_GRAPHICS_GCI_API_NO_DISCARD gpu_memory_size_t QueryGPUBufferByteSize( GPUBufferHandle pGPUBuffer );

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CheckGPUBufferRegion(
				GPUBufferHandle pGPUBuffer,
				gpu_memory_size_t pOffset,
				gpu_memory_size_t pSize );

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CheckGPUBufferRegion(
				GPUBufferHandle pGPUBuffer,
				const GPUMemoryRegion & pRegion );

		IC3_GRAPHICS_GCI_API_NO_DISCARD GPUMemoryRegion ValidateGPUBufferRegion(
				GPUBufferHandle pGPUBuffer,
				gpu_memory_size_t pOffset,
				gpu_memory_size_t pSize );

		IC3_GRAPHICS_GCI_API_NO_DISCARD GPUMemoryRegion ValidateGPUBufferRegion(
				GPUBufferHandle pGPUBuffer,
				const GPUMemoryRegion & pRegion );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_BUFFER_COMMON_H__
