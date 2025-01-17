
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX12_PRESENTATION_LAYER_H__
#define __IC3_GRAPHICS_HW3D_DX12_PRESENTATION_LAYER_H__

#include "../DX12Prerequisites.h"
#include <Ic3/Graphics/HW3D/DX/system/DX_presentationLayer.h>

namespace Ic3::Graphics::GCI
{

	ic3DeclareClassHandle( DX12ScreenPresentationLayer );

	struct DX12PresentationLayerCreateInfo : public PresentationLayerCreateInfo
	{
		cppx::bitmask<UINT> dxgiFlags = 0;
	};

	class IC3_GX_DX12_CLASS DX12ScreenPresentationLayer final : public DXScreenPresentationLayer
	{
	public:
		static constexpr uint32 sMinFrameQueueSize = 2;

		ComPtr<IDXGISwapChain3> const mDXGISwapChain3 = nullptr;
		ComPtr<ID3D12Device> const mD3D12Device = nullptr;
		ComPtr<ID3D12CommandQueue> const mD3D12PresentQueue = nullptr;

		DX12ScreenPresentationLayer( DX12GpuDevice & pDevice, ComPtr<IDXGISwapChain3> pDXGISwapChain3, uint32 pFrameQueueSize = sMinFrameQueueSize );
		virtual ~DX12ScreenPresentationLayer();

		virtual void BindRenderTarget( CommandContext * pCmdContext ) override;

		virtual void InvalidateRenderTarget( CommandContext * pCmdContext ) override;

		virtual void Present() override;

		static DX12ScreenPresentationLayerHandle Create( DX12GpuDevice & pDX12Device, const DX12PresentationLayerCreateInfo & pCreateInfo );

	private:
		bool _InitializeRTVDescriptorHeap();
		bool _InitializeRTVResources();

	private:
		struct FrameResource
		{
			ComPtr<ID3D12Resource> backBufferTexture;
			D3D12_CPU_DESCRIPTOR_HANDLE backBufferTextureRTVDescriptor;
		};

		uint32 _frameQueueSize;
		uint32 _currentFrameIndex;
		std::vector<FrameResource> _frameResourceArray;
		HANDLE _frameSyncFenceEvent;
		ComPtr<ID3D12Fence> _frameSyncFence;
		UINT64 _frameSyncFenceValue = 0;
		ComPtr<ID3D12DescriptorHeap> _rtvDescriptorHeap;
		UINT _rtvDescriptorSize;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX12_PRESENTATION_LAYER_H__
