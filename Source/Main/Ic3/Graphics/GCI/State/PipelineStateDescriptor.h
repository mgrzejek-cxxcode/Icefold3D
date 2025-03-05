
#pragma once

#ifndef __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_H__
#define __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_H__

#include "PipelineStateCommon.h"

namespace Ic3::Graphics::GCI
{

	class DynamicDescriptorDriverState;

	template <typename TPDriverDataType>
	class TDynamicDescriptorDriverState;

	/*
	 *
	 */
	class IC3_GRAPHICS_GCI_CLASS PipelineStateDescriptor : public GPUDeviceChildObject
	{
		friend class PipelineStateController;

	public:
		pipeline_state_descriptor_id_t const mDescriptorID;

	public:
		PipelineStateDescriptor( GPUDevice & pGPUDevice, pipeline_state_descriptor_id_t pDescriptorID );
		virtual ~PipelineStateDescriptor();

		/**
		 * Queries the type of the descriptor, expressed as one of the possible values defined in
		 * EPipelineStateDescriptorType enumeration. Implemented by the child descriptor classes.
		 * @return The dynamic type of this descriptor instance.
		 * @see EPipelineStateDescriptorType
		 */
		CPPX_ATTR_NO_DISCARD virtual EPipelineStateDescriptorType GetDescriptorType() const noexcept = 0;

		/**
		 * Returns true if this descriptor is a dynamic descriptor. Dynamic descriptor is a descriptor class
		 * that does not contain any API-specific properties, i.e. it is defined fully at the GCI abstraction level
		 * and their configuration can be updated on the fly, without re-creating the descriptor via GPUDevice.
		 * The only exception to these descriptors being API-agnostic is that they contain an additional,
		 * descriptor- and API-specific data that stores the cached state managed and updated by the driver.
		 * This reduces the overhead of translating a generic specification from dynamic descriptors each time
		 * they are bound to the pipeline.
		 *
		 * How this works:
		 * - Each dynamic descriptor class adds an extra RAII pointer that holds the additional driver data.
		 *   This pointer stores the data as the base DynamicDescriptorDriverState class.
		 * - When a dynamic descriptor is bound to the pipeline, driver initializes that data, translates
		 *   the descriptor's config to its native format and stores the translated config inside it.
		 * - Each time the descriptor is updated (its config changes), driver updates the cached state accordingly.
		 * There is also an additional helper class provided: PipelineStateDescriptorDynamic which implements
		 * the functionality (see below) required for dynamic descriptors to work properly.
		 *
		 * @note The following classes are implementations of dynamic descriptors:
		 * - VertexSourceBindingDescriptorDynamic
		 * - RenderPassDescriptorDynamic
		 * - RenderTargetBindingStateDescriptorDynamic
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD virtual bool IsDynamicDescriptor() const noexcept;

		/**
		 * Returns true if the dynamic driver data has been initialized or false otherwise.
		 * For non-dynamic, static descriptors (IsDynamicDescriptor() == false), this always returns false.
		 * @return True if the dynamic driver data has been initialized or false otherwise.
		 */
		CPPX_ATTR_NO_DISCARD virtual bool IsDynamicDriverStateInitialized() const noexcept;

		/**
		 * Returns the pointer to the descriptor's dynamic driver data or null if it has not been initialized.
		 * @return The pointer to the descriptor's dynamic driver data or null if it has not been initialized.
		 *
		 * @note
		 * This function can be only called on dynamic descriptors (IsDynamicDescriptor() == true). In case of
		 * 'this' is an ordinary, static descriptor, an exception is thrown.
		 */
		CPPX_ATTR_NO_DISCARD virtual DynamicDescriptorDriverState * GetDynamicDriverState() const;

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
		CPPX_ATTR_NO_DISCARD TDynamicDescriptorDriverState<TPDriverDataType> * GetDynamicDriverStateAs() const;

	protected:
		using descriptor_internal_flags_value_t = uint32;

		void SetInternalFlags( cppx::bitmask<descriptor_internal_flags_value_t> pInternalFlags ) noexcept;

		void ResetInternalFlags( cppx::bitmask<descriptor_internal_flags_value_t> pInternalFlags ) noexcept;

		bool CheckInternalFlags( cppx::bitmask<descriptor_internal_flags_value_t> pInternalFlags ) const noexcept;

		cppx::bitmask<descriptor_internal_flags_value_t> GetInternalFlags() const noexcept;

	private:
		/**
		 *
		 * @note
		 * Similar to InitializeDynamicDriverState(), this function is const and requires the member that stores
		 * the driver data to be mutable - this is because driver data is totally independent of the rest of
		 * the descriptor's state and created also for const-qualified objects.
		 */
		virtual void SetDynamicDriverState( DynamicDescriptorDriverState * pDynamicDriverState ) noexcept;

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
		CPPX_ATTR_NO_DISCARD TDynamicDescriptorDriverState<TPDriverDataType> * InitializeDynamicDriverState( TPArgs && ...pArgs );

	private:
		cppx::bitmask<descriptor_internal_flags_value_t> _internalFlags;
	};

	template <typename TPDriverDataType>
	inline TDynamicDescriptorDriverState<TPDriverDataType> * PipelineStateDescriptor::GetDynamicDriverStateAs() const
	{
		if( !IsDynamicDescriptor() )
		{
			Ic3Throw( 0 );
		}

		auto * dynamicStatePtr = GetDynamicDriverState();

		return dynamic_cast_dbg<TDynamicDescriptorDriverState<TPDriverDataType> *>( dynamicStatePtr );
	}

	template <typename TPDriverDataType, typename... TPArgs>
	inline TDynamicDescriptorDriverState<TPDriverDataType> * PipelineStateDescriptor::InitializeDynamicDriverState( TPArgs && ...pArgs )
	{
		if( !IsDynamicDescriptor() )
		{
			Ic3Throw( 0 );
		}

		auto dynamicStateUPtr = std::make_unique<TDynamicDescriptorDriverState<TPDriverDataType>>( std::forward<TPArgs>( pArgs )... );

		auto * dynamicStatePtr = dynamicStateUPtr.release();

		SetDynamicDriverState( dynamicStatePtr );

		return dynamicStatePtr;
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_H__
