
#pragma once

#ifndef __IC3_GRAPHICS_GCI_CORE_INTERFACE_DEFS_H__
#define __IC3_GRAPHICS_GCI_CORE_INTERFACE_DEFS_H__

namespace Ic3::Graphics::GCI
{

	class GPUDriver;
	class GPUDevice;

	struct GfxObjectCreateInfo
	{
		GfxObjectUID gfxObjectUID = kGfxObjectIDEmpty;
	};

	/**
	 * Base type for all resource/state types in the GCI module. It provides basic functionality
	 * like support for ID and debug name (debug builds only).
	 */
	class IC3_GRAPHICS_GCI_API GfxObject : public IDynamicObject
    {
	public:
		GfxObject();
		virtual ~GfxObject();

		/**
		 * Returns the ID of this object. If the ID has not been set, kGfxObjectIDEmpty is returned instead.
		 * @return ID of this object or kGfxObjectIDEmpty.
		 */
		CPPX_ATTR_NO_DISCARD GfxObjectID GetObjectID() const noexcept;

		/**
		 * Returns the debug name of this object, used to additionally identify objects in debug builds.
		 * @return Debug name of this object or an empty string.
		 *
		 * @note
		 * This function will return an empty string if either:
		 * - debug name for the object has not been set, or
		 * - module was not compiled in debug mode.
		 */
		CPPX_ATTR_NO_DISCARD cppx::string_view GetObjectDebugName() const noexcept;

		/**
		 *
		 * @param pGfxObjectUID
		 */
		void SetObjectUID( GfxObjectUID pGfxObjectUID );

		/**
		 * Sets the ID for this object. IDs are associated only at the object level, i.e. association
		 * between an object and its name is not propagated into any kind of global registry.
		 * ID can be retrieved later on using GetObjectID().
		 * @param pGfxObjectID An ID to set.
		 */
		void SetObjectID( GfxObjectID pGfxObjectID );

		/**
		 * Sets the debug name for this object. In debug builds this name serves as an additional way
		 * to easily identify objects created by the engine.
		 * Debug name can be retrieved later on using GetObjectDebugName().
		 * @param pObjectDebugName A debug name to set.
		 *
		 * @note
		 * Debug names are only present in debug builds. In case debug information is disabled
		 * (e.g. in a release/optimized build), this function is a no-op.
		 */
		void SetObjectDebugName( std::string pObjectDebugName );

	private:
		GfxObjectID _gfxObjectID;

	#if( IC3_DEBUG )
		cppx::immutable_string _debugName;
	#endif
    };

	inline GfxObjectID GfxObject::GetObjectID() const noexcept
	{
		return _gfxObjectID;;
	}

	inline cppx::string_view GfxObject::GetObjectDebugName() const noexcept
	{
	#if( IC3_DEBUG )
		return _debugName.str_view();
	#else
		return {};
	#endif
	}

	inline void GfxObject::SetObjectID( GfxObjectID pGfxObjectID )
	{
		_gfxObjectID = pGfxObjectID;
	}

	inline void GfxObject::SetObjectDebugName( std::string pObjectDebugName )
	{
	#if( IC3_DEBUG )
		_debugName = std::move( pObjectDebugName );
	#else
		Ic3UnusedParam( pObjectDebugName );
	#endif
	}

	/**
	 *
	 */
    class IC3_GRAPHICS_GCI_API GPUDriverChildObject : public GfxObject
    {
    public:
	    GPUDriver & mGPUDriver;

    public:
	    explicit GPUDriverChildObject( GPUDriver & pGPUDriver );
	    virtual ~GPUDriverChildObject();
    };

    /**
     *
     */
	class IC3_GRAPHICS_GCI_API GPUDeviceChildObject : public GfxObject
	{
	public:
		GPUDevice & mGPUDevice;

	public:
		explicit GPUDeviceChildObject( GPUDevice & pGPUDevice );
		virtual ~GPUDeviceChildObject();
	};

	/**
	 *
	 * @tparam TPClass
	 * @tparam TPArgs
	 * @param pArgs
	 * @return
	 */
	template <typename TPClass, typename... TPArgs>
	inline TGfxHandle<TPClass> CreateGfxObject( TPArgs && ...pArgs )
	{
		return CreateDynamicObject<TPClass>( std::forward<TPArgs>( pArgs )... );
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_CORE_INTERFACE_DEFS_H__
