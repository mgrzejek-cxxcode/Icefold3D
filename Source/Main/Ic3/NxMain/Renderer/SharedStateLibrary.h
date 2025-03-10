
#pragma once

#ifndef __IC3_NXMAIN_GPA_SHARED_STATE_LIBRARY_H__
#define __IC3_NXMAIN_GPA_SHARED_STATE_LIBRARY_H__

#include "CommonRendererDefs.h"
#include <Ic3/Graphics/GCI/State/CommonGPUStateDefs.h>
#include <Ic3/Graphics/GCI/Resources/ShaderCommon.h>

namespace Ic3
{

	namespace GID
	{

		inline constexpr cppx::string_view kShaderNamePassthroughIADefaultVS = "SH_PASSTHROUGH_IA_DEFAULT_VS";
		inline constexpr cppx::string_view kShaderNamePassthroughIADefaultPS = "SH_PASSTHROUGH_IA_DEFAULT_PS";

		inline constexpr GfxObjectID kShaderIDPassthroughIADefaultVS = 0xFF770001;
		inline constexpr GfxObjectID kShaderIDPassthroughIADefaultPS = 0xFF770002;


	}


	struct GCISharedDescriptorsIDSet
	{
		GCI::pipeline_state_descriptor_id_t descriptorIDBlendDefault;
		GCI::pipeline_state_descriptor_id_t descriptorIDDepthStencilDefault;
		GCI::pipeline_state_descriptor_id_t descriptorIDDepthStencilDepthTestEnable;
		GCI::pipeline_state_descriptor_id_t descriptorIDRasterizerSolidCullBackCCW;
		GCI::pipeline_state_descriptor_id_t descriptorIDRasterizerSolidCullBackCW;
		GCI::pipeline_state_descriptor_id_t descriptorIDRasterizerSolidCullNoneCCW;
		GCI::pipeline_state_descriptor_id_t descriptorIDRasterizerSolidCullNoneCW;
		GCI::pipeline_state_descriptor_id_t descriptorIDRasterizerWireframeCCW;
		GCI::pipeline_state_descriptor_id_t descriptorIDRasterizerWireframeCW;
		GCI::pipeline_state_descriptor_id_t descriptorIDIAVertexAttributeLayoutDefault;
		GCI::pipeline_state_descriptor_id_t descriptorIDIAVertexAttributeLayoutDefault16B;
		GCI::pipeline_state_descriptor_id_t descriptorIDGraphicsShaderLinkagePassthroughIADefault;
	};

	class IC3_NXMAIN_CLASS GCISharedStateLibrary : public CoreEngineObject
	{
	public:
		ShaderLibraryHandle const mShaderLibrary;

		static const GCISharedDescriptorsIDSet & sSharedDescriptors;

	public:
		explicit GCISharedStateLibrary( ShaderLibrary & pShaderLibrary );
		~GCISharedStateLibrary();

		CPPX_ATTR_NO_DISCARD GCI::ShaderHandle GetShader( GfxObjectID pShaderID ) const noexcept;

		CPPX_ATTR_NO_DISCARD GCI::ShaderHandle GetShader( const GfxObjectName & pShaderName ) const noexcept;

		void Initialize();

	private:
		void CreateDefaultShaders();
		void InitializeDefaultBlendStateDescriptors();
		void InitializeDefaultDepthStencilStateDescriptors();
		void InitializeDefaultRasterizerStateDescriptors();
		void InitializeDefaultInputLayoutStateDescriptors();
		void InitializeDefaultPipelineStateObjects();
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_GPA_SHARED_STATE_LIBRARY_H__
