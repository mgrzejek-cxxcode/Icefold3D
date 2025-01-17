
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_BUFFER_COMMON_H__
#define __IC3_GRAPHICS_GCI_GPU_BUFFER_COMMON_H__

#include "CommonGpuResourceDefs.h"

namespace Ic3::Graphics::GCI
{

	Ic3DeclareClassHandle( GpuBuffer );

	enum EGpuBufferDataCopyFlags : uint32
	{
		eGpuBufferDataCopyFlagModeAppendBit     = eGpuMemoryMapFlagWriteAppendBit,
		eGpuBufferDataCopyFlagModeInvalidateBit = eGpuMemoryMapFlagWriteInvalidateBit,
		eGpuBufferDataCopyFlagsDefault = 0,
	};

	/// @brief Bind flags for buffer resources. Used to specify how buffers are bound to the pipeline.
	///
	/// Note:
	/// Certain bind targets may also imply something about how the resource will be used in general. For example,
	/// vertex and index buffers should naturally have E_GPU_RESOURCE_USAGE_FLAG_VERTEX_STREAM_BIT set as well as a
	/// part of their properties. To ensure that is the case (and to reduce the amount of flags specified at creation
	/// time), these bind flags also have all required USAGE bits set as well.
	enum EGpuBufferBindFlags : resource_flags_value_t
	{
		// Bind flag for using a buffer as a constant buffer in one or more shader stages.
		// Such buffer is accessible via one of the valid CB input registers.
		// Implies SHADER_INPUT usage bit.
		eGpuBufferBindFlagConstantBufferBit          = 0x010000 | eGpuResourceUsageFlagShaderInputBit,

		// Bind flag for using a buffer as a source for vertex data fetched during execution of all rendering commands.
		// Such buffer is bound to the pipeline via TODO.
		// Implies VERTEX_STREAM usage bit.
		eGpuBufferBindFlagVertexBufferBit            = 0x020000 | eGpuResourceUsageFlagVertexStreamBit,

		// Bind flag for using a buffer as a source for index data fetched during execution of indexed rendering commands.
		// Such buffer is bound to the pipeline via TODO.
		// Implies VERTEX_STREAM usage bit.
		eGpuBufferBindFlagIndexBufferBit             = 0x040000 | eGpuResourceUsageFlagVertexStreamBit,

		// Bind flag for using a buffer as a target storage for stream output functionality.
		eGpuBufferBindFlagStreamOutputBufferBit     = 0x080000,

		// Bind flag for using a buffer as a general read-only resource.
		eGpuBufferBindFlagShaderInputBufferBit      = 0x100000 | eGpuResourceUsageFlagShaderInputBit,
		// Implies SHADER_INPUT usage bit.

		// Bind flag for using a buffer as a general read/write resource supporting unordered access.
		eGpuBufferBindFlagShaderUAVBufferBit        = 0x200000 | eGpuResourceUsageFlagShaderUavBit,
		// Implies SHADER_UAV usage bit.

		// Bind flag for using a buffer as a source for arguments to indirect dispatch commands.
		eGpuBufferBindFlagIndirectDispatchBufferBit = 0x400000,

		// Bind flag for using a buffer as a source for arguments to indirect draw commands.
		eGpuBufferBindFlagIndirectDrawBufferBit     = 0x800000,

		// Bind flag for using a buffer as a source in transfer operations. @See E_GPU_RESOURCE_USAGE_FLAG_TRANSFER_SOURCE_BIT.
		eGpuBufferBindFlagTransferSourceBufferBit   = eGpuResourceUsageFlagTransferSourceBit,

		// Bind flag for using a buffer as a target in transfer operations. @See E_GPU_RESOURCE_USAGE_FLAG_TRANSFER_TARGET_BIT.
		eGpuBufferBindFlagTransferTargetBufferBit   = eGpuResourceUsageFlagTransferTargetBit,

		// Mask with all valid BIND_FLAG bits set.
		eGpuBufferBindMaskAll = 0xFF0000 | eGpuResourceUsageMaskAll,
	};

	/// @brief Represents all valid targets for buffer resources. Corresponding E_GPU_BUFFER_BIND_FLAGs are used as values.
	enum class EGpuBufferTarget : uint32
	{
		Unknown                = 0,
		ConstantBuffer         = eGpuBufferBindFlagConstantBufferBit,
		VertexBuffer           = eGpuBufferBindFlagVertexBufferBit,
		IndexBuffer            = eGpuBufferBindFlagIndexBufferBit,
		StreamOutputBuffer     = eGpuBufferBindFlagStreamOutputBufferBit,
		ShaderInputBuffer      = eGpuBufferBindFlagShaderInputBufferBit,
		ShaderUAVBuffer        = eGpuBufferBindFlagShaderUAVBufferBit,
		IndirectDispatchBuffer = eGpuBufferBindFlagIndirectDispatchBufferBit,
		IndirectDrawBuffer     = eGpuBufferBindFlagIndirectDrawBufferBit,
		TransferSourceBuffer   = eGpuBufferBindFlagTransferSourceBufferBit,
		TransferTargetBuffer   = eGpuBufferBindFlagTransferTargetBufferBit,
	};

	struct GpuBufferInitDataDesc : public ResourceInputDataDesc
	{
	};

	struct GpuBufferCreateInfo : public ResourceCreateInfo
	{
		// Total size of the buffer, in bytes. The buffer, if created, will have at least the specified size.
		// Additional alignment rules and memory requirements may cause the buffer to be larger than requested.
		gpu_memory_size_t bufferSize = 0;

		// Initial data for the buffer. If the buffer is created with E_GPU_RESOURCE_CONTENT_FLAG_IMMUTABLE_BIT,
		// this has to contain a valid data specification.
		GpuBufferInitDataDesc initDataDesc;
	};

	struct GpuBufferDataCopyDesc
	{
		cppx::bitmask<EGpuBufferDataCopyFlags> flags = eGpuBufferDataCopyFlagsDefault;
	};

	struct GpuBufferSubDataCopyDesc
	{
		GpuMemoryRegion sourceBufferRegion;
		gpu_memory_size_t targetBufferOffset = cxGpuMemoryOffsetInvalid;
		cppx::bitmask<EGpuBufferDataCopyFlags> flags = eGpuBufferDataCopyFlagsDefault;
	};

	struct GpuBufferDataUploadDesc
	{
		ResourceInputDataDesc inputDataDesc;
		cppx::bitmask<EGpuBufferDataCopyFlags> flags = eGpuBufferDataCopyFlagsDefault;
	};

	struct GpuBufferSubDataUploadDesc
	{
		GpuMemoryRegion bufferRegion;
		ResourceInputDataDesc inputDataDesc;
		cppx::bitmask<EGpuBufferDataCopyFlags> flags = eGpuBufferDataCopyFlagsDefault;
	};

	namespace RCU
	{

		IC3_GRAPHICS_GCI_API_NO_DISCARD EGpuBufferTarget GetGpuBufferDefaultTargetFromBindFlags( cppx::bitmask<resource_flags_value_t> pBindFlags );

		IC3_GRAPHICS_GCI_API_NO_DISCARD gpu_memory_size_t QueryGpuBufferByteSize( GpuBufferHandle pGpuBuffer );

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CheckGpuBufferRegion(
				GpuBufferHandle pGpuBuffer,
				gpu_memory_size_t pOffset,
				gpu_memory_size_t pSize );

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CheckGpuBufferRegion(
				GpuBufferHandle pGpuBuffer,
				const GpuMemoryRegion & pRegion );

		IC3_GRAPHICS_GCI_API_NO_DISCARD GpuMemoryRegion ValidateGpuBufferRegion(
				GpuBufferHandle pGpuBuffer,
				gpu_memory_size_t pOffset,
				gpu_memory_size_t pSize );

		IC3_GRAPHICS_GCI_API_NO_DISCARD GpuMemoryRegion ValidateGpuBufferRegion(
				GpuBufferHandle pGpuBuffer,
				const GpuMemoryRegion & pRegion );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_BUFFER_COMMON_H__
