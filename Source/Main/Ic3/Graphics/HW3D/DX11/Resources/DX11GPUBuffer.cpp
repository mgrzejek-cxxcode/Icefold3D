
#include "DX11GPUBuffer.h"
#include <Ic3/Graphics/HW3D/DX11/DX11APITranslationLayer.h>
#include <Ic3/Graphics/HW3D/DX11/DX11GPUDevice.h>
#include <Ic3/Graphics/HW3D/DX11/DX11CommandList.h>

namespace Ic3::Graphics::GCI
{

	DX11GPUBuffer::DX11GPUBuffer(
		DX11GPUDevice & pDX11GPUDevice,
		const ResourceMemoryInfo & pResourceMemory,
		const GPUBufferProperties & pBufferProperties,
		ComPtr<ID3D11Buffer> pD3D11Buffer )
    : GPUBuffer( pDX11GPUDevice, pResourceMemory, pBufferProperties )
    , mD3D11Buffer( std::move( pD3D11Buffer ) )
	{ }

	DX11GPUBuffer::~DX11GPUBuffer() = default;

	DX11GPUBufferHandle DX11GPUBuffer::Create( DX11GPUDevice & pDX11GPUDevice, const GPUBufferCreateInfo & pCreateInfo )
	{
		auto createInfo = pCreateInfo;
		if( !ValidateBufferCreateInfo( createInfo ) )
		{
			return nullptr;
		}

		auto dx11GPUBufferDesc = TranslateBufferDesc( createInfo );

		D3D11_BUFFER_DESC d3d11BufferDesc;
		d3d11BufferDesc.ByteWidth = cppx::numeric_cast<UINT>( createInfo.bufferSize );
		d3d11BufferDesc.BindFlags = dx11GPUBufferDesc.bindFlags;
		d3d11BufferDesc.CpuAccessFlags = dx11GPUBufferDesc.cpuAccessFlags;
		d3d11BufferDesc.MiscFlags = dx11GPUBufferDesc.resourceMiscFlags;
		d3d11BufferDesc.Usage = dx11GPUBufferDesc.usage;
		d3d11BufferDesc.StructureByteStride = 0;

		const void * initDataPtr = nullptr;

		if( createInfo.initDataDesc )
		{
			if( createInfo.initDataDesc.size < createInfo.bufferSize )
			{
				Ic3DebugInterrupt();
				Ic3DebugOutput( "Warn: init data must fill the whole resource. Buffer will be created with default content." );
			}
			else
			{
				initDataPtr = createInfo.initDataDesc.pointer;
			}
		}

		ComPtr<ID3D11Buffer> d3d11Buffer;
		HRESULT hResult = S_OK;

		if( !initDataPtr )
		{
			hResult = pDX11GPUDevice.mD3D11Device1->CreateBuffer( &d3d11BufferDesc, nullptr, d3d11Buffer.GetAddressOf() );
		}
		else
		{
			D3D11_SUBRESOURCE_DATA d3d11BufferInitData;
			d3d11BufferInitData.pSysMem = createInfo.initDataDesc.pointer;
			d3d11BufferInitData.SysMemPitch = 0;
			d3d11BufferInitData.SysMemSlicePitch = 0;

			hResult = pDX11GPUDevice.mD3D11Device1->CreateBuffer( &d3d11BufferDesc, &d3d11BufferInitData, d3d11Buffer.GetAddressOf() );
		}

		if( FAILED( hResult ) )
		{
			return nullptr;
		}

		d3d11Buffer->GetDesc( &d3d11BufferDesc );

		GPUBufferProperties bufferProperties;
		bufferProperties.byteSize = d3d11BufferDesc.ByteWidth;
		bufferProperties.resourceFlags = createInfo.resourceFlags;

		ResourceMemoryInfo bufferMemoryInfo;
		bufferMemoryInfo.sourceHeapRegion.offset = 0;
		bufferMemoryInfo.sourceHeapRegion.size = d3d11BufferDesc.ByteWidth;
		bufferMemoryInfo.baseAlignment = createInfo.memoryBaseAlignment;
		bufferMemoryInfo.memoryFlags = createInfo.memoryFlags;

		auto dx11Buffer = CreateDynamicObject<DX11GPUBuffer>( pDX11GPUDevice, bufferMemoryInfo, bufferProperties, d3d11Buffer );

		return dx11Buffer;
	}

	bool DX11GPUBuffer::ValidateBufferCreateInfo( GPUBufferCreateInfo & pCreateInfo )
	{
		if( !GPUBuffer::ValidateBufferCreateInfo( pCreateInfo ) )
		{
			return false;
		}

		constexpr uint32 cxDX11GPUBufferMemorySupportedFlags = E_GPU_MEMORY_MAP_FLAG_ACCESS_READ_WRITE_BIT;
		pCreateInfo.memoryFlags = pCreateInfo.memoryFlags & cxDX11GPUBufferMemorySupportedFlags;

		return true;
	}

	DX11GPUBufferDesc DX11GPUBuffer::TranslateBufferDesc( const GPUBufferCreateInfo & pCreateInfo )
	{
		DX11GPUBufferDesc dx11GPUBufferDesc;
		dx11GPUBufferDesc.bindFlags = ATL::TranslateDX11BufferBindFlags( pCreateInfo.resourceFlags );
		dx11GPUBufferDesc.cpuAccessFlags = 0;
		dx11GPUBufferDesc.resourceMiscFlags = 0;
		dx11GPUBufferDesc.usage = D3D11_USAGE_DEFAULT;

		if( pCreateInfo.resourceFlags.is_set( E_GPU_RESOURCE_CONTENT_FLAG_IMMUTABLE_BIT ) )
		{
			dx11GPUBufferDesc.usage = D3D11_USAGE_IMMUTABLE;
		}
		if( pCreateInfo.resourceFlags.is_set_any_of( E_GPU_RESOURCE_USAGE_FLAG_TRANSFER_SOURCE_BIT ) )
		{
			dx11GPUBufferDesc.usage = D3D11_USAGE_STAGING;
		}
		if( pCreateInfo.resourceFlags.is_set( E_GPU_RESOURCE_CONTENT_FLAG_DYNAMIC_BIT ) && ( dx11GPUBufferDesc.usage != D3D11_USAGE_DYNAMIC ) )
		{
			dx11GPUBufferDesc.usage = D3D11_USAGE_DYNAMIC;
			dx11GPUBufferDesc.cpuAccessFlags.set( D3D11_CPU_ACCESS_WRITE );
		}

		if( pCreateInfo.memoryFlags.is_set( E_GPU_MEMORY_MAP_FLAG_ACCESS_READ_BIT ) )
		{
			dx11GPUBufferDesc.cpuAccessFlags.set( D3D11_CPU_ACCESS_READ );
		}
		if( pCreateInfo.memoryFlags.is_set( E_GPU_MEMORY_MAP_FLAG_ACCESS_WRITE_BIT ) )
		{
			dx11GPUBufferDesc.cpuAccessFlags.set( D3D11_CPU_ACCESS_WRITE );
		}

		auto indirectBufferFlags = E_GPU_BUFFER_BIND_FLAG_INDIRECT_DISPATCH_BUFFER_BIT | E_GPU_BUFFER_BIND_FLAG_INDIRECT_DRAW_BUFFER_BIT;
		if( pCreateInfo.resourceFlags.is_set_any_of( indirectBufferFlags ) )
		{
			dx11GPUBufferDesc.resourceMiscFlags.set( D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS );
		}

		if( pCreateInfo.resourceFlags.is_set_any_of( E_GPU_BUFFER_BIND_FLAG_SHADER_UAV_BUFFER_BIT ) )
		{
			dx11GPUBufferDesc.resourceMiscFlags.set( D3D11_RESOURCE_MISC_BUFFER_STRUCTURED );
		}

		return dx11GPUBufferDesc;
	}

	bool DX11GPUBuffer::MapRegion( void * pCommandObject, const GPUMemoryRegion & pRegion, EGPUMemoryMapMode pMapMode )
	{
		auto * d3d11DeviceContext1 = GCU::GetD3D11DeviceContextFromCommandList( pCommandObject );

		auto d3d11MapMode = ATL::TranslateDX11BufferMapFlags( pMapMode, mResourceMemory.memoryFlags );
		Ic3DebugAssert( d3d11MapMode != 0 );

		D3D11_MAPPED_SUBRESOURCE mappedBufferInfo;
		auto hResult = d3d11DeviceContext1->Map( mD3D11Buffer.Get(), 0, d3d11MapMode, 0, &mappedBufferInfo );
		if( FAILED( hResult ) )
		{
			return false;
		}

		ResourceMappedMemory mappedMemoryInfo;
		mappedMemoryInfo.pointer = mappedBufferInfo.pData;
		mappedMemoryInfo.mappedRegion = pRegion;
		mappedMemoryInfo.memoryMapFlags = static_cast<EGPUMemoryMapFlags>( pMapMode );

		_dx11MapInfo.mappedSubresource = mappedBufferInfo;
		_dx11MapInfo.requestedMapMode = d3d11MapMode;

		SetMappedMemory( mappedMemoryInfo );

		return true;
	}

	void DX11GPUBuffer::Unmap( void * pCommandObject )
	{
		auto * d3d11DeviceContext1 = GCU::GetD3D11DeviceContextFromCommandList( pCommandObject );

		d3d11DeviceContext1->Unmap( mD3D11Buffer.Get(), 0 );

		ResetMappedMemory();
	}

	void DX11GPUBuffer::FlushMappedRegion( void * pCommandObject, const GPUMemoryRegion & pRegion )
	{
		auto * d3d11DeviceContext1 = GCU::GetD3D11DeviceContextFromCommandList( pCommandObject );

		d3d11DeviceContext1->Unmap( mD3D11Buffer.Get(), 0 );

		auto hResult = d3d11DeviceContext1->Map( mD3D11Buffer.Get(), 0, _dx11MapInfo.requestedMapMode, 0, &( _dx11MapInfo.mappedSubresource ) );
		if( FAILED( hResult ) )
		{
			Ic3DebugInterrupt();
		}
	}

	void DX11GPUBuffer::InvalidateRegion( void * pCommandObject, const GPUMemoryRegion & pRegion )
	{
		if( pRegion.size == mBufferProperties.byteSize )
		{
			auto * d3d11DeviceContext1 = GCU::GetD3D11DeviceContextFromCommandList( pCommandObject );
			d3d11DeviceContext1->DiscardResource( mD3D11Buffer.Get() );
		}
	}

	void DX11GPUBuffer::UpdateSubDataCopy( void * pCommandObject, GPUBuffer & pSource, const GPUBufferSubDataCopyDesc & pCopyDesc )
	{
		auto * dx11CommandList = reinterpret_cast<DX11CommandList *>( pCommandObject );
		if( !dx11CommandList->CheckCommandClassSupport( ECommandQueueClass::Transfer ) )
		{
			Ic3Throw( 0 );
		}

		auto * d3d11DeviceContext1 = dx11CommandList->mD3D11DeviceContext1.Get();

		cppx::bitmask<UINT> copyFlags = 0;
		if( ( pCopyDesc.sourceBufferRegion.size == mBufferProperties.byteSize ) || pCopyDesc.flags.is_set( E_GPU_BUFFER_DATA_COPY_FLAG_MODE_INVALIDATE_BIT ) )
		{
			copyFlags.set( D3D11_COPY_DISCARD );
		}

		D3D11_BOX sourceBufferBox;
		sourceBufferBox.top = 0;
		sourceBufferBox.bottom = 1;
		sourceBufferBox.front = 0;
		sourceBufferBox.back = 1;
		sourceBufferBox.left = cppx::numeric_cast<UINT>( pCopyDesc.sourceBufferRegion.offset );
		sourceBufferBox.right = cppx::numeric_cast<UINT>( pCopyDesc.sourceBufferRegion.offset + pCopyDesc.sourceBufferRegion.size );

		auto * targetBuffer = mD3D11Buffer.Get();
		auto targetBufferOffset = cppx::numeric_cast<UINT>( pCopyDesc.targetBufferOffset );
		auto * sourceBuffer = pSource.QueryInterface<DX11GPUBuffer>()->mD3D11Buffer.Get();

		d3d11DeviceContext1->CopySubresourceRegion1( targetBuffer, 0, targetBufferOffset, 0, 0, sourceBuffer, 0, &sourceBufferBox, copyFlags );
	}

	void DX11GPUBuffer::UpdateSubDataUpload( void * pCommandObject, const GPUBufferSubDataUploadDesc & pUploadDesc )
	{
		auto * dx11CommandList = reinterpret_cast<DX11CommandList *>( pCommandObject );
		if( !dx11CommandList->CheckCommandClassSupport( ECommandQueueClass::Transfer ) )
		{
			Ic3Throw( 0 );
		}

		auto * d3d11DeviceContext1 = dx11CommandList->mD3D11DeviceContext1.Get();

		cppx::bitmask<UINT> updateFlags = 0;
		if( ( pUploadDesc.bufferRegion.size == mBufferProperties.byteSize ) || pUploadDesc.flags.is_set( E_GPU_BUFFER_DATA_COPY_FLAG_MODE_INVALIDATE_BIT ) )
		{
			updateFlags.set( D3D11_COPY_DISCARD );
		}

        if( pUploadDesc.bufferRegion.size == mBufferProperties.byteSize )
        {
            d3d11DeviceContext1->UpdateSubresource1( mD3D11Buffer.Get(), 0, nullptr, pUploadDesc.inputDataDesc.pointer, 0, 0, updateFlags );
        }
        else
        {
            D3D11_BOX targetBufferBox;
            targetBufferBox.top = 0;
            targetBufferBox.bottom = 1;
            targetBufferBox.front = 0;
            targetBufferBox.back = 1;
            targetBufferBox.left = cppx::numeric_cast<UINT>( pUploadDesc.bufferRegion.offset );
            targetBufferBox.right = cppx::numeric_cast<UINT>( pUploadDesc.bufferRegion.offset + pUploadDesc.bufferRegion.size );

            d3d11DeviceContext1->UpdateSubresource1( mD3D11Buffer.Get(), 0, &targetBufferBox, pUploadDesc.inputDataDesc.pointer, 0, 0, updateFlags );
        }
	}

	bool DX11GPUBuffer::ValidateMapRequest( const GPUMemoryRegion & pRegion, const EGPUMemoryMapMode & pMapMode )
	{
		return GPUBuffer::ValidateMapRequest( pRegion, pMapMode );
	}

} // namespace Ic3::Graphics::GCI
