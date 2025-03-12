
#pragma once

#ifndef __IC3_GRAPHICS_GCI_PIPELINE_STATE_CONTROLLER_H__
#define __IC3_GRAPHICS_GCI_PIPELINE_STATE_CONTROLLER_H__

#include "PipelineStateDescriptorDynamic.h"

namespace Ic3::Graphics::GCI
{

	/*
	 *
	 */
	class IC3_GRAPHICS_GCI_CLASS PipelineStateController : public GfxObject
	{
	public:
		PipelineStateController() = default;
		virtual ~PipelineStateController() = default;

	protected:
		/**
		 * Initializes the specified dynamic descriptor's driver data.
		 * This function is a proxy function that enables subclasses of the state controller to the private API of PipelineStateDescriptorDynamic.
		 *
		 * @tparam TPDriverDataType Type of the driver data that will be allocated and stored within the descriptor object.
		 * @tparam TPBaseDescriptor Type of the descriptor, deduced automatically from the parameter 'pPipelineStateDescriptorDynamic'.
		 * @tparam TPArgs Types tuple for 'pArgs'.
		 * @param pPipelineStateDescriptorDynamic The descriptor for which the driver data should be allocated. Must be a dynamic descriptor.
		 * @param pArgs Additional, optional arguments passed to the TPDriverDataType's constructor.
		 * @return Pointer to the allocated object which holds the driver-specific data.
		 */
		template <typename TPDriverDataType, typename TPBaseDescriptor, typename... TPArgs>
		static TDynamicDescriptorDriverState<TPDriverDataType> * InitializeDynamicDriverStateForDescriptor(
			PipelineStateDescriptorDynamic<TPBaseDescriptor> & pPipelineStateDescriptorDynamic,
			TPArgs && ...pArgs )
		{
			return pPipelineStateDescriptorDynamic.InitializeDynamicDriverState<TPDriverDataType>( std::forward<TPArgs>( pArgs )... );
		}

		/**
		 * Proxy function for #PipelineStateDescriptorDynamic::IsConfigurationModified.
		 * Returns true if the internal descriptor configuration has changed or false otherwise.
		 * @return True if the internal descriptor configuration has changed or false otherwise.
		 */
		template<typename TPBaseDescriptor>
		CPPX_ATTR_NO_DISCARD bool IsDynamicDescriptorConfigurationModified(
				const PipelineStateDescriptorDynamic<TPBaseDescriptor> & pPipelineStateDescriptorDynamic ) const noexcept
		{
			return pPipelineStateDescriptorDynamic.IsConfigurationModified();
		}

		/**
		 * Proxy function for #PipelineStateDescriptorDynamic::ResetConfigurationModifiedState.
		 * Resets (clears) the flag that indicates configuration change.
		 * Called by the PipelineStateController which exposes it to its subclasses.
		 */
		template<typename TPBaseDescriptor>
		void ResetDynamicDescriptorConfigurationModifiedState(
				PipelineStateDescriptorDynamic<TPBaseDescriptor> & pPipelineStateDescriptorDynamic ) noexcept
		{
			pPipelineStateDescriptorDynamic.ResetConfigurationModifiedState();
		}
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_PIPELINE_STATE_CONTROLLER_H__
