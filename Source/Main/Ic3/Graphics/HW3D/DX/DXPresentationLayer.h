
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DXCOMMON_SWAP_CHAIN_H__
#define __IC3_GRAPHICS_HW3D_DXCOMMON_SWAP_CHAIN_H__

#include "DXPrerequisites.h"
#include <Ic3/Graphics/GCI/PresentationLayer.h>
#include <Ic3/System/Core/WindowSystem.h>

namespace Ic3::Graphics::GCI
{

	Ic3DeclareClassHandle( DXPresentationLayer );

	class IC3_GX_DXCOMMON_CLASS DXPresentationLayer : public PresentationLayer
	{
	public:
		DXPresentationLayer( GPUDevice & pDevice );
		virtual ~DXPresentationLayer();
	};

	class IC3_GX_DXCOMMON_CLASS DXScreenPresentationLayer : public DXPresentationLayer
	{
	public:
		System::WindowHandle const mSysWindow;
		ComPtr<IDXGISwapChain1> const mDXGISwapChain1;

		DXScreenPresentationLayer( GPUDevice & pDevice, System::WindowHandle pSysWindow, ComPtr<IDXGISwapChain1> pDXGISwapChain1 ) noexcept;
		virtual ~DXScreenPresentationLayer() noexcept;

		virtual System::EventSource * GetInternalSystemEventSource() const noexcept override;

		virtual void Resize( uint32 pWidth, uint32 pHeight ) override final;

		virtual void SetFullscreenMode( bool pEnable ) override final;

		virtual cxm::vec2u32 QueryRenderTargetSize() const override;

	protected:
	    static System::WindowHandle createSysWindow( DXGPUDevice & pGPUDevice, const PresentationLayerCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DXCOMMON_SWAP_CHAIN_H__
