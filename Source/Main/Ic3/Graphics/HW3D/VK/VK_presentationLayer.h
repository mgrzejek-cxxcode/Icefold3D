
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_VKCOMMON_PRESENTATION_LAYER_H__
#define __IC3_GRAPHICS_HW3D_VKCOMMON_PRESENTATION_LAYER_H__

#include "VKPrerequisites.h"
#include <Ic3/Graphics/GCI/PresentationLayer.h>

namespace Ic3::Graphics::GCI
{

	ic3DeclareClassHandle( VKPresentationLayer );

	class ICFGX_VKCOMMON_CLASS VKPresentationLayer : public PresentationLayer
	{
	public:
		VKPresentationLayer( GPUDevice & pDevice );
		virtual ~VKPresentationLayer();
	};

	class ICFGX_VKCOMMON_CLASS VKScreenPresentationLayer : public VKPresentationLayer
	{
	public:
		VKScreenPresentationLayer( GPUDevice & pDevice );
		virtual ~VKScreenPresentationLayer();

		virtual void bindRenderTarget( CommandContext * pCmdContext ) override;

		virtual void invalidateRenderTarget( CommandContext * pCmdContext ) override;

		virtual void present() override;

		virtual void resize( uint32 pWidth, uint32 pHeight ) override;

		virtual void setFullscreenMode( bool pEnable ) override;

		virtual Ic3::Math::Vec2u32 queryRenderTargetSize() const override;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_VKCOMMON_PRESENTATION_LAYER_H__
