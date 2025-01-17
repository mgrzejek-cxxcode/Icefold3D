
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_BUFFER_REFERENCE_H__
#define __IC3_GRAPHICS_GCI_GPU_BUFFER_REFERENCE_H__

#include "GPUBufferCommon.h"

namespace Ic3::Graphics::GCI
{

	/// @brief A GPU buffer reference. Represents a valid sub-region of a buffer which can be used for resource access.
	class IC3_GRAPHICS_GCI_CLASS GPUBufferReference
	{
	public:
		GPUBufferReference( const GPUBufferReference & ) = default;
		GPUBufferReference & operator=( const GPUBufferReference & ) = default;

		GPUBufferReference() = default;
		~GPUBufferReference() = default;

		GPUBufferReference( GPUBufferHandle pGPUBuffer );
		GPUBufferReference( GPUBufferHandle pGPUBuffer, const GPUMemoryRegion & pReferencedSubRegion );
		GPUBufferReference( GPUBufferHandle pGPUBuffer, gpu_memory_size_t pOffset, gpu_memory_size_t pSize = cxGPUMemorySizeMax );

		GPUBuffer * operator->() const noexcept;

		/// @brief Returns a pointer to the referenced GPU buffer. This can be null if the reference is empty().
		CPPX_ATTR_NO_DISCARD GPUBufferHandle GetRefBuffer() const noexcept;

		/// @brief
		CPPX_ATTR_NO_DISCARD const GPUMemoryRegion & GetRefSubRegion() const noexcept;

		/// @brief Returns true if this instance represents a valid binding.
		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept;

		/// @brief
		CPPX_ATTR_NO_DISCARD bool IsValid() const noexcept;

		/// @brief
		bool SetRefBuffer( GPUBufferHandle pGPUBuffer );

		/// @brief
		bool SetRefBuffer( GPUBufferHandle pGPUBuffer, const GPUMemoryRegion & pReferencedSubRegion );

		/// @brief
		bool SetRefBuffer( GPUBufferHandle pGPUBuffer, gpu_memory_size_t pOffset, gpu_memory_size_t pSize = cxGPUMemorySizeMax );

		/// @brief
		bool Reset();

	private:
		bool _SetRefBufferInternal( GPUBufferHandle pGPUBuffer, const GPUMemoryRegion & pReferencedSubRegion = {} );

	private:
		/// A handle to the referenced buffer object. If the reference is empty, the handle is null.
		GPUBufferHandle _refBufferObject = nullptr;

		/// A region of the referenced buffer which is usable via this reference object.
		/// The offset and size are expressed in bytes.
		/// Size can be GPU_MEMORY_SIZE_MAX - it means the whole remaining region (starting at offset) is referenced.
		GPUMemoryRegion _refSubRegion;
	};

	inline GPUBuffer * GPUBufferReference::operator->() const noexcept
	{
		return _refBufferObject.get();
	}

	inline GPUBufferHandle GPUBufferReference::GetRefBuffer() const noexcept
	{
		return _refBufferObject;
	}

	inline const GPUMemoryRegion & GPUBufferReference::GetRefSubRegion() const noexcept
	{
		return _refSubRegion;
	}

	inline bool GPUBufferReference::IsEmpty() const noexcept
	{
		return !_refBufferObject || !_refSubRegion;
	}

	inline bool GPUBufferReference::IsValid() const noexcept
	{
		return _refBufferObject && RCU::CheckGPUBufferRegion( _refBufferObject, _refSubRegion );
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_BUFFER_REFERENCE_H__
