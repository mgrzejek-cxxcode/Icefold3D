
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_VKCOMMON_PRESENTATION_LAYER_H__
#define __IC3_GRAPHICS_HW3D_VKCOMMON_PRESENTATION_LAYER_H__

#include "VKPrerequisites.h"
#include <Ic3/Graphics/GCI/PresentationLayer.h>

namespace Ic3::Graphics::GCI
{

	Ic3DeclareClassHandle( VKPresentationLayer );

	class IC3_GX_VKCOMMON_CLASS VKPresentationLayer : public PresentationLayer
	{
	public:
		VKPresentationLayer( GPUDevice & pDevice );
		virtual ~VKPresentationLayer();
	};

	class IC3_GX_VKCOMMON_CLASS VKScreenPresentationLayer : public VKPresentationLayer
	{
	public:
		VKScreenPresentationLayer( GPUDevice & pDevice );
		virtual ~VKScreenPresentationLayer();

		virtual void BindRenderTarget( CommandContext & pCommandContext ) override;

		virtual void InvalidateRenderTarget( CommandContext & pCommandContext ) override;

		virtual void Present() override;

		virtual void resize( uint32 pWidth, uint32 pHeight ) override;

		virtual void SetFullscreenMode( bool pEnable ) override;

		virtual cxm::vec2u32 QueryRenderTargetSize() const override;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_VKCOMMON_PRESENTATION_LAYER_H__
