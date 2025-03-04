
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX12_CORE_LAYER_H__
#define __IC3_GRAPHICS_HW3D_DX12_CORE_LAYER_H__

#include "DX12Prerequisites.h"
#include <Ic3/Graphics/HW3D/DX/DXAPITranslationLayer.h>
#include <Ic3/Graphics/GCI/cmds/CommonCommandDefs.h>

namespace Ic3::Graphics::GCI
{

	struct D3D12CommandListData
	{
		ComPtr<ID3D12CommandAllocator> commandAllocator;
		ComPtr<ID3D12GraphicsCommandList> graphicsCommandList;

		explicit operator bool() const
		{
			return commandAllocator && graphicsCommandList;
		}
	};

	namespace ATL
	{

		inline constexpr bool CheckDescriptorEmpty( const D3D12_CPU_DESCRIPTOR_HANDLE & pDescriptor )
		{
			return pDescriptor.ptr == cvD3D12CPUDescriptorPtrInvalid;
		}

		ComPtr<ID3D12Debug> InitializeD3D12DebugInterface( cppx::bitmask<EGPUDriverConfigFlags> pDriverConfigFlags );

		ComPtr<ID3D12Device> CreateD3D12Device();

		ComPtr<IDXGIFactory3> createDXGIFactoryForD3D12Device(
				const ComPtr<ID3D12Device> & pD3D12Device,
				const ComPtr<ID3D12Debug> & pD3D12DebugInterface );

		ComPtr<ID3D12CommandQueue> CreateD3D12CommandQueue(
				const ComPtr<ID3D12Device> & pD3D12Device,
				D3D12_COMMAND_LIST_TYPE pD3D12CommandListType );

		ComPtr<IDXGISwapChain3> CreateD3D12SwapChainForExfWindow(
				const ComPtr<ID3D12CommandQueue> & pD3D12PresentCommandQueue,
				void * pExfSysWindow, /* It must be a System::Window */
				cppx::bitmask<UINT> pDXGIFlags );

		D3D12CommandListData CreateD3D12CommandList(
				const ComPtr<ID3D12Device> & pD3D12Device,
				D3D12_COMMAND_LIST_TYPE pD3D12CommandListType );

		D3D12_COMMAND_LIST_TYPE TranslateD3D12CommandListType(
				ECommandContextType pContextType,
				cppx::bitmask<GPUCmdCommandClassFlags> pCommandClassFlags );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX12_CORE_LAYER_H__
