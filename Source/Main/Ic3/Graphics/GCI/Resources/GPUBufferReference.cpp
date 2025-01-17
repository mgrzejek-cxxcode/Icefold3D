
#include "GpuBufferReference.h"
#include "GpuBuffer.h"

namespace Ic3::Graphics::GCI
{

	GpuBufferReference::GpuBufferReference( GpuBufferHandle pGpuBuffer )
	: _refBufferObject( pGpuBuffer )
	, _refSubRegion{ pGpuBuffer->GetWholeBufferRegion() }
	{}

	GpuBufferReference::GpuBufferReference( GpuBufferHandle pGpuBuffer, const GpuMemoryRegion & pReferencedSubRegion )
	{
		_SetRefBufferInternal( pGpuBuffer, RCU::ValidateGpuBufferRegion( pGpuBuffer, pReferencedSubRegion ) );
	}

	GpuBufferReference::GpuBufferReference( GpuBufferHandle pGpuBuffer, gpu_memory_size_t pOffset, gpu_memory_size_t pSize )
	{
		_SetRefBufferInternal( pGpuBuffer, RCU::ValidateGpuBufferRegion( pGpuBuffer, pOffset, pSize ) );
	}

	bool GpuBufferReference::SetRefBuffer( GpuBufferHandle pGpuBuffer )
	{
		return _SetRefBufferInternal( pGpuBuffer, pGpuBuffer->GetWholeBufferRegion() );
	}

	bool GpuBufferReference::SetRefBuffer( GpuBufferHandle pGpuBuffer, const GpuMemoryRegion & pReferencedSubRegion )
	{
		return _SetRefBufferInternal( pGpuBuffer, RCU::ValidateGpuBufferRegion( pGpuBuffer, pReferencedSubRegion ) );
	}

	bool GpuBufferReference::SetRefBuffer( GpuBufferHandle pGpuBuffer, gpu_memory_size_t pOffset, gpu_memory_size_t pSize )
	{
		return _SetRefBufferInternal( pGpuBuffer, RCU::ValidateGpuBufferRegion( pGpuBuffer, pOffset, pSize ) );
	}

	bool GpuBufferReference::Reset()
	{
		return _SetRefBufferInternal( nullptr );
	}

	bool GpuBufferReference::_SetRefBufferInternal( GpuBufferHandle pGpuBuffer, const GpuMemoryRegion & pReferencedSubRegion )
	{
		bool referenceUpdated = false;

		// If an empty buffer handle is specified, reset this reference object: the buffer gets cleared (and its
		// active use counter decremented by one), referenced region is reset to default.
		if( !pGpuBuffer )
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
		else if( ( pGpuBuffer != _refBufferObject ) || ( pReferencedSubRegion != _refSubRegion ) )
		{
			// Validate if the specified subregion is valid for the given buffer object.
			if( RCU::CheckGpuBufferRegion( pGpuBuffer, pReferencedSubRegion ) )
			{
				// Update the buffer only if necessary.
				if( pGpuBuffer != _refBufferObject )
				{
					// This can be null (empty reference).
					if( _refBufferObject )
					{
						_refBufferObject->ReleaseActiveRef();
						_refBufferObject = nullptr;
					}

					_refBufferObject = pGpuBuffer;
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
