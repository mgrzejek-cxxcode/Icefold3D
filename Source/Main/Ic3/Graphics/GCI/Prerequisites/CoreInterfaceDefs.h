
#pragma once

#ifndef __IC3_GRAPHICS_GCI_CORE_INTERFACE_DEFS_H__
#define __IC3_GRAPHICS_GCI_CORE_INTERFACE_DEFS_H__

namespace Ic3::Graphics::GCI
{

	class GpuDriver;
	class GpuDevice;

	/// @brief
    class GfxObject : public IDynamicObject
    {
    };

	/// @brief
    class IC3_GRAPHICS_GCI_API GpuDriverChildObject : public GfxObject
    {
    public:
	    GpuDriver & mGpuDriver;

	    explicit GpuDriverChildObject( GpuDriver & pGpuDriver );
	    virtual ~GpuDriverChildObject();
    };

	/// @brief
	class IC3_GRAPHICS_GCI_API GpuDeviceChildObject : public GfxObject
	{
	public:
        GpuDriver & mGpuDriver;
		GpuDevice & mGpuDevice;

	public:
		explicit GpuDeviceChildObject( GpuDevice & pGpuDevice );
		virtual ~GpuDeviceChildObject();

		virtual GfxObjectID QueryObjectID() const;
	};

	template <typename TClass, typename... TArgs>
	inline TGfxHandle<TClass> CreateGfxObject( TArgs && ...pArgs )
	{
		return CreateDynamicObject<TClass>( std::forward<TArgs>( pArgs )... );
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_CORE_INTERFACE_DEFS_H__
