
#pragma once

#ifndef __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_DYNAMIC_H__
#define __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_DYNAMIC_H__

#include "PipelineStateDescriptor.h"

namespace Ic3::Graphics::GCI
{

	/**
	 * Base class used to store the driver data for dynamic descriptors. The data is stored as a pointer-to-base,
	 * because the actual type of the data used by drivers for descriptors is unknown to the GCI layer.
	 */
	class DynamicDescriptorDriverState : public IDynamicObject
	{
	public:
		virtual ~DynamicDescriptorDriverState() = default;
	};

	/**
	 *
	 * @tparam TPDriverDataType
	 */
	template <typename TPDriverDataType>
	class TDynamicDescriptorDriverState : public DynamicDescriptorDriverState
	{
	public:
		using DriverReleaseFunc = std::function<void( TPDriverDataType * )>;

		TPDriverDataType mDriverData;

		TDynamicDescriptorDriverState() = default;

		TDynamicDescriptorDriverState( DriverReleaseFunc pDriverReleaseFunc )
		: _driverReleaseFunc( std::move( pDriverReleaseFunc ) )
		{}

		virtual ~TDynamicDescriptorDriverState()
		{
			if( _driverReleaseFunc )
			{
				_driverReleaseFunc( &mDriverData );
				_driverReleaseFunc = {};
			}
		}

	private:
		DriverReleaseFunc _driverReleaseFunc;
	};

	/**
	 * 
	 * @tparam TPBaseDescriptor
	 */
	template <typename TPBaseDescriptor>
	class PipelineStateDescriptorDynamic : public TPBaseDescriptor
	{
		friend class PipelineStateController;

	public:
		Ic3DeclareNonCopyable( PipelineStateDescriptorDynamic );

		template <typename... TPArgs>
		PipelineStateDescriptorDynamic( TPArgs && ...pArgs )
		: TPBaseDescriptor( std::forward<TPArgs>( pArgs )... )
		{
			SetConfigChangedFlag();
		}

		virtual ~PipelineStateDescriptorDynamic() = default;

		/**
		 * @see GraphicsPipelineStateDescriptor::IsDynamicDescriptor()
		 */
		CPPX_ATTR_NO_DISCARD virtual bool IsDynamicDescriptor() const noexcept override final
		{
			return true;
		}

		CPPX_ATTR_NO_DISCARD virtual bool IsDynamicDriverStateInitialized() const noexcept override final
		{
			return static_cast<bool>( _dynamicDriverState );
		}

		CPPX_ATTR_NO_DISCARD virtual DynamicDescriptorDriverState * GetDynamicDriverState() const noexcept override final
		{
			return _dynamicDriverState.get();
		}

	protected:
		/**
		 * Sets an internal flag (eDescriptorStateFlagDynamicConfigChangedBit) which signals that the internal
		 * configuration managed by the descriptor has been updated.
		 */
		void SetConfigChangedFlag()
		{
			this->SetInternalFlags( eDescriptorStateFlagDynamicConfigChangedBit );
		}

	private:
		/**
		 * Implementation of #PipelineStateDescriptorDynamic::SetDynamicDriverState.
		 * @see PipelineStateDescriptorDynamic::SetDynamicDriverState
		 */
		virtual void SetDynamicDriverState( DynamicDescriptorDriverState * pDynamicDriverState ) noexcept override final
		{
			_dynamicDriverState.reset( pDynamicDriverState );
		}

		/**
		 * Returns true if the internal descriptor configuration has changed or false otherwise.
		 * Called by the PipelineStateController which exposes it to its subclasses.
		 * @return True if the internal descriptor configuration has changed or false otherwise.
		 */
		CPPX_ATTR_NO_DISCARD bool IsConfigurationModified() const noexcept
		{
			return this->CheckInternalFlags( eDescriptorStateFlagDynamicConfigChangedBit );
		}

		/**
		 * Resets (clears) the flag that indicates configuration change.
		 * Called by the PipelineStateController which exposes it to its subclasses.
		 */
		void ResetConfigurationModifiedState() noexcept
		{
			return this->ResetInternalFlags( eDescriptorStateFlagDynamicConfigChangedBit );
		}

	protected:
		enum : typename TPBaseDescriptor::descriptor_internal_flags_value_t
		{
			eDescriptorStateFlagDynamicConfigChangedBit = 0x0100,
		};

	private:
		std::unique_ptr<DynamicDescriptorDriverState> _dynamicDriverState;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_DYNAMIC_H__
