
#pragma once

#ifndef __IC3_GRAPHICS_GCI_CORE_INTERFACE_DEFS_H__
#define __IC3_GRAPHICS_GCI_CORE_INTERFACE_DEFS_H__

namespace Ic3::Graphics::GCI
{

	class GPUDriver;
	class GPUDevice;

	/// @brief
    class GPUAPIObject : public IDynamicObject
    {
    };

	/// @brief
    class IC3_GRAPHICS_GCI_API GPUDriverChildObject : public GPUAPIObject
    {
    public:
	    GPUDriver & mGPUDriver;

	    explicit GPUDriverChildObject( GPUDriver & pGPUDriver );
	    virtual ~GPUDriverChildObject();
    };

	/// @brief
	class IC3_GRAPHICS_GCI_API GPUDeviceChildObject : public GPUAPIObject
	{
	public:
        GPUDriver & mGPUDriver;
		GPUDevice & mGPUDevice;

	public:
		explicit GPUDeviceChildObject( GPUDevice & pGPUDevice );
		virtual ~GPUDeviceChildObject();

		virtual UniqueGPUObjectID queryObjectID() const;
	};

	template <typename TClass, typename... TArgs>
	inline TGPAHandle<TClass> createGPUAPIObject( TArgs && ...pArgs )
	{
		return createDynamicObject<TClass>( std::forward<TArgs>( pArgs )... );
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_CORE_INTERFACE_DEFS_H__
