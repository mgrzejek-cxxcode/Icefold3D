
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DXCOMMON_SWAP_CHAIN_H__
#define __IC3_GRAPHICS_HW3D_DXCOMMON_SWAP_CHAIN_H__

#include "DXPrerequisites.h"
#include <Ic3/Graphics/GCI/PresentationLayer.h>
#include <Ic3/System/WindowSystem.h>

namespace Ic3::Graphics::GCI
{

	ic3DeclareClassHandle( DXPresentationLayer );

	class ICFGX_DXCOMMON_CLASS DXPresentationLayer : public PresentationLayer
	{
	public:
		DXPresentationLayer( GPUDevice & pDevice );
		virtual ~DXPresentationLayer();
	};

	class ICFGX_DXCOMMON_CLASS DXScreenPresentationLayer : public DXPresentationLayer
	{
	public:
		System::WindowHandle const mSysWindow;
		ComPtr<IDXGISwapChain1> const mDXGISwapChain1;

		DXScreenPresentationLayer( GPUDevice & pDevice, System::WindowHandle pSysWindow, ComPtr<IDXGISwapChain1> pDXGISwapChain1 ) noexcept;
		virtual ~DXScreenPresentationLayer() noexcept;

		virtual System::EventSource * getInternalSystemEventSource() const noexcept override;

		virtual void resize( uint32 pWidth, uint32 pHeight ) override final;

		virtual void setFullscreenMode( bool pEnable ) override final;

		virtual Ic3::Math::Vec2u32 queryRenderTargetSize() const override;

	protected:
	    static System::WindowHandle createSysWindow( DXGPUDevice & pGPUDevice, const PresentationLayerCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DXCOMMON_SWAP_CHAIN_H__
