
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
	class PipelineStateDescriptorDynamic : public GCIPipelineStateDescriptor<TPBaseDescriptor>
	{
		friend class PipelineStateController;

	public:
		Ic3DeclareNonCopyable( PipelineStateDescriptorDynamic );

		template <typename... TPArgs>
		PipelineStateDescriptorDynamic( TPArgs && ...pArgs )
		: GCIPipelineStateDescriptor<TPBaseDescriptor>( std::forward<TPArgs>( pArgs )... )
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

		/**
		 * Returns true if the dynamic driver data has been initialized or false otherwise.
		 * For non-dynamic, static descriptors (IsDynamicDescriptor() == false), this always returns false.
		 * @return True if the dynamic driver data has been initialized or false otherwise.
		 */
		CPPX_ATTR_NO_DISCARD bool IsDynamicDriverStateInitialized() const noexcept
		{
			return static_cast<bool>( _dynamicDriverState );
		}
		/**
		 * Returns the pointer to the descriptor's dynamic driver data or null if it has not been initialized.
		 * @return The pointer to the descriptor's dynamic driver data or null if it has not been initialized.
		 */

		CPPX_ATTR_NO_DISCARD DynamicDescriptorDriverState * GetDynamicDriverState() const noexcept
		{
			return _dynamicDriverState.get();
		}

		/**
		 * Returns the pointer to the descriptor's dynamic driver data or null if it has not been initialized.
		 * The return type is TDynamicDescriptorDriverState<TPDriverDataType> (a base pointer returned by the
		 * subclass is cast) and should match the actual data type used by the calling driver.
		 * @return The pointer to the descriptor's dynamic driver data or null if it has not been initialized.
		 *
		 * @note
		 * This function can be only called on dynamic descriptors (IsDynamicDescriptor() == true). In case of
		 * 'this' is an ordinary, static descriptor, an exception is thrown.
		 */
		template <typename TPDriverDataType>
		CPPX_ATTR_NO_DISCARD TDynamicDescriptorDriverState<TPDriverDataType> * GetDynamicDriverStateAs() const
		{
			auto * dynamicStatePtr = GetDynamicDriverState();
			return dynamic_cast_dbg<TDynamicDescriptorDriverState<TPDriverDataType> *>( dynamicStatePtr );
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
		 *
		 * @return
		 *
		 * @note
		 * Similar to SetDynamicDriverState(), this function is const and requires the member that stores
		 * the driver data to be mutable - this is because driver data is totally independent of the rest of
		 * the descriptor's state and created also for const-qualified objects.
		 */
		template <typename TPDriverDataType, typename... TPArgs>
		CPPX_ATTR_NO_DISCARD TDynamicDescriptorDriverState<TPDriverDataType> * InitializeDynamicDriverState( TPArgs && ...pArgs )
		{
			auto dynamicDriverState = std::make_unique<TDynamicDescriptorDriverState<TPDriverDataType>>( std::forward<TPArgs>( pArgs )... );
			auto * dynamicDriverStatePtr = dynamicDriverState.get();
			_dynamicDriverState = std::move( dynamicDriverState );
			return dynamicDriverStatePtr;
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
