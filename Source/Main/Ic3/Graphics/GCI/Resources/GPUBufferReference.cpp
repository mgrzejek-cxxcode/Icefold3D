
#include "GPUBufferReference.h"
#include "GPUBuffer.h"

namespace Ic3::Graphics::GCI
{

	GPUBufferReference::GPUBufferReference( GPUBufferHandle pGPUBuffer )
	: _refBufferObject( pGPUBuffer )
	, _refSubRegion{ pGPUBuffer->GetWholeBufferRegion() }
	{}

	GPUBufferReference::GPUBufferReference( GPUBufferHandle pGPUBuffer, const GPUMemoryRegion & pReferencedSubRegion )
	{
		_SetRefBufferInternal( pGPUBuffer, RCU::ValidateGPUBufferRegion( pGPUBuffer, pReferencedSubRegion ) );
	}

	GPUBufferReference::GPUBufferReference( GPUBufferHandle pGPUBuffer, gpu_memory_size_t pOffset, gpu_memory_size_t pSize )
	{
		_SetRefBufferInternal( pGPUBuffer, RCU::ValidateGPUBufferRegion( pGPUBuffer, pOffset, pSize ) );
	}

	bool GPUBufferReference::SetRefBuffer( GPUBufferHandle pGPUBuffer )
	{
		return _SetRefBufferInternal( pGPUBuffer, pGPUBuffer->GetWholeBufferRegion() );
	}

	bool GPUBufferReference::SetRefBuffer( GPUBufferHandle pGPUBuffer, const GPUMemoryRegion & pReferencedSubRegion )
	{
		return _SetRefBufferInternal( pGPUBuffer, RCU::ValidateGPUBufferRegion( pGPUBuffer, pReferencedSubRegion ) );
	}

	bool GPUBufferReference::SetRefBuffer( GPUBufferHandle pGPUBuffer, gpu_memory_size_t pOffset, gpu_memory_size_t pSize )
	{
		return _SetRefBufferInternal( pGPUBuffer, RCU::ValidateGPUBufferRegion( pGPUBuffer, pOffset, pSize ) );
	}

	bool GPUBufferReference::Reset()
	{
		return _SetRefBufferInternal( nullptr );
	}

	bool GPUBufferReference::_SetRefBufferInternal( GPUBufferHandle pGPUBuffer, const GPUMemoryRegion & pReferencedSubRegion )
	{
		bool referenceUpdated = false;

		// If an empty buffer handle is specified, reset this reference object: the buffer gets cleared (and its
		// active use counter decremented by one), referenced region is reset to default.
		if( !pGPUBuffer )
		{
			// Of course, assuming there is something to reset in the first place.
			if( _refBufferObject )
			{
				_refBufferObject->ReleaseActiveRef();
				_refBufferObject = nullptr;
				_refSubRegion.set_empty();

				referenceUpdated = true;
			}
		}
		// Otherwise, check if there is anything to update: either the buffer is a
		// different buffer or the referenced subregion is different from the current one.
		else if( ( pGPUBuffer != _refBufferObject ) || ( pReferencedSubRegion != _refSubRegion ) )
		{
			// Validate if the specified subregion is valid for the given buffer object.
			if( RCU::CheckGPUBufferRegion( pGPUBuffer, pReferencedSubRegion ) )
			{
				// Update the buffer only if necessary.
				if( pGPUBuffer != _refBufferObject )
				{
					// This can be null (empty reference).
					if( _refBufferObject )
					{
						_refBufferObject->ReleaseActiveRef();
						_refBufferObject = nullptr;
					}

					_refBufferObject = pGPUBuffer;
					_refBufferObject->AddActiveRef();
				}

				// Update the referenced subregion.
				_refSubRegion = pReferencedSubRegion;

				referenceUpdated = true;
			}
		}

		return referenceUpdated;
	}

}
