
#include "constantBufferProxy.h"
#include <Ic3/Graphics/GCI/Resources/GPUBuffer.h>

namespace Ic3
{

	ConstantBufferProxy::ConstantBufferProxy( GCI::GPUBufferHandle pBuffer )
	: mGPUDevice( pBuffer->mGPUDevice )
	{}

	ConstantBufferProxy::~ConstantBufferProxy() = default;

}
