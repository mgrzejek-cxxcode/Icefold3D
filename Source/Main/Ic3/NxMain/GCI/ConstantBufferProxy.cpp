
#include "constantBufferProxy.h"
#include <Ic3/Graphics/GCI/Resources/GpuBuffer.h>

namespace Ic3
{

	ConstantBufferProxy::ConstantBufferProxy( GCI::GpuBufferHandle pBuffer )
	: mGpuDevice( pBuffer->mGpuDevice )
	{}

	ConstantBufferProxy::~ConstantBufferProxy() = default;

}
