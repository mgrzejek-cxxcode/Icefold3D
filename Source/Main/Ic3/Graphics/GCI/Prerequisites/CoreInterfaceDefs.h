
#pragma once

#ifndef __IC3_GRAPHICS_GCI_CORE_INTERFACE_DEFS_H__
#define __IC3_GRAPHICS_GCI_CORE_INTERFACE_DEFS_H__

namespace Ic3::Graphics::GCI
{

	class GPUDriver;
	class GPUDevice;

	/// @brief
    class GfxObject : public IDynamicObject
    {
    };

	/// @brief
    class IC3_GRAPHICS_GCI_API GPUDriverChildObject : public GfxObject
    {
    public:
	    GPUDriver & mGPUDriver;

	    explicit GPUDriverChildObject( GPUDriver & pGPUDriver );
	    virtual ~GPUDriverChildObject();
    };

	/// @brief
	class IC3_GRAPHICS_GCI_API GPUDeviceChildObject : public GfxObject
	{
	public:
        GPUDriver & mGPUDriver;
		GPUDevice & mGPUDevice;

	public:
		explicit GPUDeviceChildObject( GPUDevice & pGPUDevice );
		virtual ~GPUDeviceChildObject();

		virtual GfxObjectID QueryObjectID() const;
	};

	template <typename TClass, typename... TArgs>
	inline TGfxHandle<TClass> CreateGfxObject( TArgs && ...pArgs )
	{
		return CreateDynamicObject<TClass>( std::forward<TArgs>( pArgs )... );
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_CORE_INTERFACE_DEFS_H__
