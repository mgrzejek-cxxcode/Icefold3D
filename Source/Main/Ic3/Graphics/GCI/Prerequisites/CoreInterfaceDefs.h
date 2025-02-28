
#pragma once

#ifndef __IC3_GRAPHICS_GCI_CORE_INTERFACE_DEFS_H__
#define __IC3_GRAPHICS_GCI_CORE_INTERFACE_DEFS_H__

namespace Ic3::Graphics::GCI
{

	class GPUDriver;
	class GPUDevice;

	/**
	 *
	 */
	class IC3_GRAPHICS_GCI_API GfxObject : public IDynamicObject
    {
	public:
		GfxObject();
		virtual ~GfxObject();

		/**
		 * XX
		 * @return 
		 */
		CPPX_ATTR_NO_DISCARD virtual GfxObjectID GetObjectID() const noexcept;

		/**
		 * XX
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD virtual GfxObjectName GetObjectName() const noexcept;
    };

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
	 */
	template <typename TPBase, bool tpEnableNameStorage = true>
	class TGfxObjectNameProxy;

	/**
	 *
	 */
	template <typename TPBase>
	class TGfxObjectNameProxy<TPBase, false> : public TPBase
	{
	public:
		template <typename... TPArgs>
		TGfxObjectNameProxy( TPArgs &&... pArgs )
		: TPBase( std::forward<TPArgs>( pArgs )... )
		{}
	};

	/**
	 *
	 */
	template <typename TPBase>
	class TGfxObjectNameProxy<TPBase, true> : public TPBase
	{
	public:
		template <typename... TPArgs>
		TGfxObjectNameProxy( TPArgs &&... pArgs )
		: TPBase( std::forward<TPArgs>( pArgs )... )
		{}

		/**
		 * @copydoc GfxObject::GetObjectName
		 */
		CPPX_ATTR_NO_DISCARD virtual GfxObjectName GetObjectName() const noexcept override
		{
			return _gfxObjectName;
		}

		/**
		 *
		 * @param pObjectName
		 */
		void SetObjectName( GfxObjectName pObjectName ) noexcept
		{
			_gfxObjectName = pObjectName;
		}

	private:
		GfxObjectName _gfxObjectName;
	};

	/**
	 *
	 * @tparam TPBase
	 */
	template<typename TPBase, bool tpEnableNameStorage = false>
	class TGfxObjectIDProxy : public TGfxObjectNameProxy<TPBase, tpEnableNameStorage>
	{
	public:
		template<typename... TPArgs>
		TGfxObjectIDProxy( const InitEmptyTag &, TPArgs && ...pArgs )
		: TGfxObjectNameProxy<TPBase, tpEnableNameStorage>( std::forward<TPArgs>( pArgs )... )
		, _gfxObjectID( kGfxObjectIDEmpty )
		{}

		template <typename... TPArgs>
		TGfxObjectIDProxy( GfxObjectID pGfxObjectID, TPArgs && ...pArgs )
		: TGfxObjectNameProxy<TPBase, tpEnableNameStorage>( std::forward<TPArgs>( pArgs )... )
		, _gfxObjectID( pGfxObjectID )
		{}

		/**
		 * @copydoc GfxObject::GetObjectID
		 */
		CPPX_ATTR_NO_DISCARD virtual GfxObjectID GetObjectID() const noexcept override final
		{
			return _gfxObjectID;
		}

		/**
		 * 
		 * @param pObjectID
		 */
		void SetObjectID( GfxObjectID pObjectID ) noexcept
		{
			_gfxObjectID = pObjectID;
		}

	private:
		GfxObjectID _gfxObjectID;
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
