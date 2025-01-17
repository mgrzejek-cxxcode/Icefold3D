
#include "DX11GpuBuffer.h"
#include <Ic3/Graphics/HW3D/DX11/DX11ApiTranslationLayer.h>
#include <Ic3/Graphics/HW3D/DX11/DX11GpuDevice.h>
#include <Ic3/Graphics/HW3D/DX11/DX11CommandList.h>

namespace Ic3::Graphics::GCI
{

	DX11GpuBuffer::DX11GpuBuffer(
		DX11GpuDevice & pDX11GpuDevice,
		const ResourceMemoryInfo & pResourceMemory,
		const GpuBufferProperties & pBufferProperties,
		ComPtr<ID3D11Buffer> pD3D11Buffer )
    : GpuBuffer( pDX11GpuDevice, pResourceMemory, pBufferProperties )
    , mD3D11Buffer( std::move( pD3D11Buffer ) )
	{ }

	DX11GpuBuffer::~DX11GpuBuffer() = default;

	DX11GpuBufferHandle DX11GpuBuffer::Create( DX11GpuDevice & pDX11GpuDevice, const GpuBufferCreateInfo & pCreateInfo )
	{
		auto createInfo = pCreateInfo;
		if( !ValidateBufferCreateInfo( createInfo ) )
		{
			return nullptr;
		}

		auto dx11GpuBufferDesc = TranslateBufferDesc( createInfo );

		D3D11_BUFFER_DESC d3d11BufferDesc;
		d3d11BufferDesc.ByteWidth = cppx::numeric_cast<UINT>( createInfo.bufferSize );
		d3d11BufferDesc.BindFlags = dx11GpuBufferDesc.bindFlags;
		d3d11BufferDesc.CpuAccessFlags = dx11GpuBufferDesc.cpuAccessFlags;
		d3d11BufferDesc.MiscFlags = dx11GpuBufferDesc.resourceMiscFlags;
		d3d11BufferDesc.Usage = dx11GpuBufferDesc.usage;
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
			hResult = pDX11GpuDevice.mD3D11Device1->CreateBuffer( &d3d11BufferDesc, nullptr, d3d11Buffer.GetAddressOf() );
		}
		else
		{
			D3D11_SUBRESOURCE_DATA d3d11BufferInitData;
			d3d11BufferInitData.pSysMem = createInfo.initDataDesc.pointer;
			d3d11BufferInitData.SysMemPitch = 0;
			d3d11BufferInitData.SysMemSlicePitch = 0;

			hResult = pDX11GpuDevice.mD3D11Device1->CreateBuffer( &d3d11BufferDesc, &d3d11BufferInitData, d3d11Buffer.GetAddressOf() );
		}

		if( FAILED( hResult ) )
		{
			return nullptr;
		}

		d3d11Buffer->GetDesc( &d3d11BufferDesc );

		GpuBufferProperties bufferProperties;
		bufferProperties.byteSize = d3d11BufferDesc.ByteWidth;
		bufferProperties.resourceFlags = createInfo.resourceFlags;

		ResourceMemoryInfo bufferMemoryInfo;
		bufferMemoryInfo.sourceHeapRegion.offset = 0;
		bufferMemoryInfo.sourceHeapRegion.size = d3d11BufferDesc.ByteWidth;
		bufferMemoryInfo.baseAlignment = createInfo.memoryBaseAlignment;
		bufferMemoryInfo.memoryFlags = createInfo.memoryFlags;

		auto dx11Buffer = CreateDynamicObject<DX11GpuBuffer>( pDX11GpuDevice, bufferMemoryInfo, bufferProperties, d3d11Buffer );

		return dx11Buffer;
	}

	bool DX11GpuBuffer::ValidateBufferCreateInfo( GpuBufferCreateInfo & pCreateInfo )
	{
		if( !GpuBuffer::ValidateBufferCreateInfo( pCreateInfo ) )
		{
			return false;
		}

		constexpr uint32 cxDX11GpuBufferMemorySupportedFlags = E_GPU_MEMORY_MAP_FLAG_ACCESS_READ_WRITE_BIT;
		pCreateInfo.memoryFlags = pCreateInfo.memoryFlags & cxDX11GpuBufferMemorySupportedFlags;

		return true;
	}

	DX11GpuBufferDesc DX11GpuBuffer::TranslateBufferDesc( const GpuBufferCreateInfo & pCreateInfo )
	{
		DX11GpuBufferDesc dx11GpuBufferDesc;
		dx11GpuBufferDesc.bindFlags = ATL::TranslateDX11BufferBindFlags( pCreateInfo.resourceFlags );
		dx11GpuBufferDesc.cpuAccessFlags = 0;
		dx11GpuBufferDesc.resourceMiscFlags = 0;
		dx11GpuBufferDesc.usage = D3D11_USAGE_DEFAULT;

		if( pCreateInfo.resourceFlags.is_set( E_GPU_RESOURCE_CONTENT_FLAG_IMMUTABLE_BIT ) )
		{
			dx11GpuBufferDesc.usage = D3D11_USAGE_IMMUTABLE;
		}
		if( pCreateInfo.resourceFlags.is_set_any_of( E_GPU_RESOURCE_USAGE_FLAG_TRANSFER_SOURCE_BIT ) )
		{
			dx11GpuBufferDesc.usage = D3D11_USAGE_STAGING;
		}
		if( pCreateInfo.resourceFlags.is_set( E_GPU_RESOURCE_CONTENT_FLAG_DYNAMIC_BIT ) && ( dx11GpuBufferDesc.usage != D3D11_USAGE_DYNAMIC ) )
		{
			dx11GpuBufferDesc.usage = D3D11_USAGE_DYNAMIC;
			dx11GpuBufferDesc.cpuAccessFlags.set( D3D11_CPU_ACCESS_WRITE );
		}

		if( pCreateInfo.memoryFlags.is_set( E_GPU_MEMORY_MAP_FLAG_ACCESS_READ_BIT ) )
		{
			dx11GpuBufferDesc.cpuAccessFlags.set( D3D11_CPU_ACCESS_READ );
		}
		if( pCreateInfo.memoryFlags.is_set( E_GPU_MEMORY_MAP_FLAG_ACCESS_WRITE_BIT ) )
		{
			dx11GpuBufferDesc.cpuAccessFlags.set( D3D11_CPU_ACCESS_WRITE );
		}

		auto indirectBufferFlags = E_GPU_BUFFER_BIND_FLAG_INDIRECT_DISPATCH_BUFFER_BIT | E_GPU_BUFFER_BIND_FLAG_INDIRECT_DRAW_BUFFER_BIT;
		if( pCreateInfo.resourceFlags.is_set_any_of( indirectBufferFlags ) )
		{
			dx11GpuBufferDesc.resourceMiscFlags.set( D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS );
		}

		if( pCreateInfo.resourceFlags.is_set_any_of( E_GPU_BUFFER_BIND_FLAG_SHADER_UAV_BUFFER_BIT ) )
		{
			dx11GpuBufferDesc.resourceMiscFlags.set( D3D11_RESOURCE_MISC_BUFFER_STRUCTURED );
		}

		return dx11GpuBufferDesc;
	}

	bool DX11GpuBuffer::MapRegion( void * pCommandObject, const GpuMemoryRegion & pRegion, EGpuMemoryMapMode pMapMode )
	{
		auto * d3d11DeviceContext1 = SMU::GetD3D11DeviceContextFromCommandList( pCommandObject );

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
		mappedMemoryInfo.memoryMapFlags = static_cast<EGpuMemoryMapFlags>( pMapMode );

		_dx11MapInfo.mappedSubresource = mappedBufferInfo;
		_dx11MapInfo.requestedMapMode = d3d11MapMode;

		SetMappedMemory( mappedMemoryInfo );

		return true;
	}

	void DX11GpuBuffer::Unmap( void * pCommandObject )
	{
		auto * d3d11DeviceContext1 = SMU::GetD3D11DeviceContextFromCommandList( pCommandObject );

		d3d11DeviceContext1->Unmap( mD3D11Buffer.Get(), 0 );

		ResetMappedMemory();
	}

	void DX11GpuBuffer::FlushMappedRegion( void * pCommandObject, const GpuMemoryRegion & pRegion )
	{
		auto * d3d11DeviceContext1 = SMU::GetD3D11DeviceContextFromCommandList( pCommandObject );

		d3d11DeviceContext1->Unmap( mD3D11Buffer.Get(), 0 );

		auto hResult = d3d11DeviceContext1->Map( mD3D11Buffer.Get(), 0, _dx11MapInfo.requestedMapMode, 0, &( _dx11MapInfo.mappedSubresource ) );
		if( FAILED( hResult ) )
		{
			Ic3DebugInterrupt();
		}
	}

	void DX11GpuBuffer::InvalidateRegion( void * pCommandObject, const GpuMemoryRegion & pRegion )
	{
		if( pRegion.size == mBufferProperties.byteSize )
		{
			auto * d3d11DeviceContext1 = SMU::GetD3D11DeviceContextFromCommandList( pCommandObject );
			d3d11DeviceContext1->DiscardResource( mD3D11Buffer.Get() );
		}
	}

	void DX11GpuBuffer::UpdateSubDataCopy( void * pCommandObject, GpuBuffer & pSource, const GpuBufferSubDataCopyDesc & pCopyDesc )
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
		auto * sourceBuffer = pSource.QueryInterface<DX11GpuBuffer>()->mD3D11Buffer.Get();

		d3d11DeviceContext1->CopySubresourceRegion1( targetBuffer, 0, targetBufferOffset, 0, 0, sourceBuffer, 0, &sourceBufferBox, copyFlags );
	}

	void DX11GpuBuffer::UpdateSubDataUpload( void * pCommandObject, const GpuBufferSubDataUploadDesc & pUploadDesc )
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

	bool DX11GpuBuffer::ValidateMapRequest( const GpuMemoryRegion & pRegion, const EGpuMemoryMapMode & pMapMode )
	{
		return GpuBuffer::ValidateMapRequest( pRegion, pMapMode );
	}

} // namespace Ic3::Graphics::GCI
