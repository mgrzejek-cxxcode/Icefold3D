
#pragma once

#ifndef __IC3_NXMAIN_GPA_SHARED_STATE_LIBRARY_H__
#define __IC3_NXMAIN_GPA_SHARED_STATE_LIBRARY_H__

#include "CommonRendererDefs.h"
#include <Ic3/Graphics/GCI/State/CommonGPUStateDefs.h>
#include <Ic3/Graphics/GCI/Resources/ShaderCommon.h>
#include <unordered_map>

namespace Ic3
{

	namespace GID
	{

		inline constexpr cppx::string_view kShaderNamePassthroughIADefaultVS = "SH_PASSTHROUGH_IA_DEFAULT_VS";
		inline constexpr cppx::string_view kShaderNamePassthroughIADefaultPS = "SH_PASSTHROUGH_IA_DEFAULT_PS";

		inline constexpr GfxObjectID kShaderIDPassthroughIADefaultVS = 0xFF770001;
		inline constexpr GfxObjectID kShaderIDPassthroughIADefaultPS = 0xFF770002;

		inline constexpr auto kGfxIDStateDescriptorBlendDefault =
				GCI::CXU::DeclarePipelineStateDescriptorIDBlendState( 0x001 );

		inline constexpr auto kGfxIDStateDescriptorDepthStencilDefault =
				GCI::CXU::DeclarePipelineStateDescriptorIDDepthStencilState( 0x001 );

		inline constexpr auto kGfxIDStateDescriptorDepthStencilDepthTestEnable =
				GCI::CXU::DeclarePipelineStateDescriptorIDDepthStencilState( 0x002 );

		inline constexpr auto kGfxIDStateDescriptorRasterizerSolidCullBackCCW =
				GCI::CXU::DeclarePipelineStateDescriptorIDRasterizerState( 0x001 );

		inline constexpr auto kGfxIDStateDescriptorRasterizerSolidCullBackCW =
				GCI::CXU::DeclarePipelineStateDescriptorIDRasterizerState( 0x002 );

		inline constexpr auto kGfxIDStateDescriptorRasterizerSolidCullNoneCCW =
				GCI::CXU::DeclarePipelineStateDescriptorIDRasterizerState( 0x003 );

		inline constexpr auto kGfxIDStateDescriptorRasterizerSolidCullNoneCW =
				GCI::CXU::DeclarePipelineStateDescriptorIDRasterizerState( 0x004 );

		inline constexpr auto kGfxIDStateDescriptorRasterizerWireframeCCW =
				GCI::CXU::DeclarePipelineStateDescriptorIDRasterizerState( 0x005 );

		inline constexpr auto kGfxIDStateDescriptorRasterizerWireframeCW =
				GCI::CXU::DeclarePipelineStateDescriptorIDRasterizerState( 0x006 );

		inline constexpr auto kGfxIDStateDescriptorIAVertexAttributeLayoutDefault =
				GCI::CXU::DeclarePipelineStateDescriptorIDVertexAttributeLayout( 0x001 );

		inline constexpr auto kGfxIDStateDescriptorIAVertexAttributeLayoutDefault16B =
				GCI::CXU::DeclarePipelineStateDescriptorIDVertexAttributeLayout( 0x001 );

		inline constexpr auto kGfxIDStateDescriptorGraphicsShaderLinkagePassthroughIADefault =
				GCI::CXU::DeclarePipelineStateDescriptorIDGraphicsShaderLinkage( 0x001 );

	}


	class GCISharedStateLibrary : public CoreEngineObject
	{
	public:
		ShaderLibraryHandle const mShaderLibrary;

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
