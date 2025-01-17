
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_BUFFER_REFERENCE_H__
#define __IC3_GRAPHICS_GCI_GPU_BUFFER_REFERENCE_H__

#include "GpuBufferCommon.h"

namespace Ic3::Graphics::GCI
{

	/// @brief A GPU buffer reference. Represents a valid sub-region of a buffer which can be used for resource access.
	class IC3_GRAPHICS_GCI_CLASS GpuBufferReference
	{
	public:
		GpuBufferReference( const GpuBufferReference & ) = default;
		GpuBufferReference & operator=( const GpuBufferReference & ) = default;

		GpuBufferReference() = default;
		~GpuBufferReference() = default;

		GpuBufferReference( GpuBufferHandle pGpuBuffer );
		GpuBufferReference( GpuBufferHandle pGpuBuffer, const GpuMemoryRegion & pReferencedSubRegion );
		GpuBufferReference( GpuBufferHandle pGpuBuffer, gpu_memory_size_t pOffset, gpu_memory_size_t pSize = cxGpuMemorySizeMax );

		GpuBuffer * operator->() const noexcept;

		/// @brief Returns a pointer to the referenced GPU buffer. This can be null if the reference is empty().
		CPPX_ATTR_NO_DISCARD GpuBufferHandle GetRefBuffer() const noexcept;

		/// @brief
		CPPX_ATTR_NO_DISCARD const GpuMemoryRegion & GetRefSubRegion() const noexcept;

		/// @brief Returns true if this instance represents a valid binding.
		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept;

		/// @brief
		CPPX_ATTR_NO_DISCARD bool IsValid() const noexcept;

		/// @brief
		bool SetRefBuffer( GpuBufferHandle pGpuBuffer );

		/// @brief
		bool SetRefBuffer( GpuBufferHandle pGpuBuffer, const GpuMemoryRegion & pReferencedSubRegion );

		/// @brief
		bool SetRefBuffer( GpuBufferHandle pGpuBuffer, gpu_memory_size_t pOffset, gpu_memory_size_t pSize = cxGpuMemorySizeMax );

		/// @brief
		bool Reset();

	private:
		bool _SetRefBufferInternal( GpuBufferHandle pGpuBuffer, const GpuMemoryRegion & pReferencedSubRegion = {} );

	private:
		/// A handle to the referenced buffer object. If the reference is empty, the handle is null.
		GpuBufferHandle _refBufferObject = nullptr;

		/// A region of the referenced buffer which is usable via this reference object.
		/// The offset and size are expressed in bytes.
		/// Size can be GPU_MEMORY_SIZE_MAX - it means the whole remaining region (starting at offset) is referenced.
		GpuMemoryRegion _refSubRegion;
	};

	inline GpuBuffer * GpuBufferReference::operator->() const noexcept
	{
		return _refBufferObject.get();
	}

	inline GpuBufferHandle GpuBufferReference::GetRefBuffer() const noexcept
	{
		return _refBufferObject;
	}

	inline const GpuMemoryRegion & GpuBufferReference::GetRefSubRegion() const noexcept
	{
		return _refSubRegion;
	}

	inline bool GpuBufferReference::IsEmpty() const noexcept
	{
		return !_refBufferObject || !_refSubRegion;
	}

	inline bool GpuBufferReference::IsValid() const noexcept
	{
		return _refBufferObject && RCU::CheckGpuBufferRegion( _refBufferObject, _refSubRegion );
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_BUFFER_REFERENCE_H__
