
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
		GPUBufferReference( GPUBufferHandle pGPUBuffer, gpu_memory_size_t pOffset, gpu_memory_size_t pSize = CxDef::GPU_MEMORY_SIZE_MAX );

		GPUBuffer * operator->() const noexcept;

		/// @brief Returns a pointer to the referenced GPU buffer. This can be null if the reference is empty().
		IC3_ATTR_NO_DISCARD GPUBufferHandle getRefBuffer() const noexcept;

		/// @brief
		IC3_ATTR_NO_DISCARD const GPUMemoryRegion & getRefSubRegion() const noexcept;

		/// @brief Returns true if this instance represents a valid binding.
		IC3_ATTR_NO_DISCARD bool empty() const noexcept;

		/// @brief
		IC3_ATTR_NO_DISCARD bool valid() const noexcept;

		/// @brief
		bool setRefBuffer( GPUBufferHandle pGPUBuffer );

		/// @brief
		bool setRefBuffer( GPUBufferHandle pGPUBuffer, const GPUMemoryRegion & pReferencedSubRegion );

		/// @brief
		bool setRefBuffer( GPUBufferHandle pGPUBuffer, gpu_memory_size_t pOffset, gpu_memory_size_t pSize = CxDef::GPU_MEMORY_SIZE_MAX );

		/// @brief
		bool reset();

	private:
		bool _setRefBufferInternal( GPUBufferHandle pGPUBuffer, const GPUMemoryRegion & pReferencedSubRegion = {} );

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

	inline GPUBufferHandle GPUBufferReference::getRefBuffer() const noexcept
	{
		return _refBufferObject;
	}

	inline const GPUMemoryRegion & GPUBufferReference::getRefSubRegion() const noexcept
	{
		return _refSubRegion;
	}

	inline bool GPUBufferReference::empty() const noexcept
	{
		return !_refBufferObject || !_refSubRegion;
	}

	inline bool GPUBufferReference::valid() const noexcept
	{
		return _refBufferObject && RCU::checkGPUBufferRegion( _refBufferObject, _refSubRegion );
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_BUFFER_REFERENCE_H__
