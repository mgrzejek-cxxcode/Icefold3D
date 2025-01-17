
#pragma once

#ifndef __IC3_NXMAIN_GPA_SHARED_STATE_LIBRARY_H__
#define __IC3_NXMAIN_GPA_SHARED_STATE_LIBRARY_H__

#include "CommonRendererDefs.h"
#include <Ic3/Graphics/GCI/State/commonGPUStateDefs.h>
#include <Ic3/Graphics/GCI/Resources/shaderCommon.h>
#include <unordered_map>

namespace Ic3
{

	namespace CxDef
	{

		constexpr GfxObjectID GPA_RESOURCE_ID_SHADER_PASSTHROUGH_IA_DEFAULT_VS { 0xFF007001 };
		constexpr GfxObjectID GPA_RESOURCE_ID_SHADER_PASSTHROUGH_IA_DEFAULT_PS { 0xFF007002 };

		constexpr GfxObjectID GPA_STATE_ID_BLEND_DEFAULT { 0xFF00BB01 };

		constexpr GfxObjectID GPA_STATE_ID_DEPTH_STENCIL_DEFAULT { 0xFF00DD01 };
		constexpr GfxObjectID GPA_STATE_ID_DEPTH_STENCIL_DEPTH_TEST_ENABLE { 0xFF00DD02 };

		constexpr GfxObjectID GPA_STATE_ID_RASTERIZER_DEFAULT_CULL_BACK_CCW { 0xFF00CC01 };
		constexpr GfxObjectID GPA_STATE_ID_RASTERIZER_DEFAULT_CULL_BACK_CW { 0xFF00CC02 };
		constexpr GfxObjectID GPA_STATE_ID_RASTERIZER_DEFAULT_CULL_FRONT_CCW { 0xFF00CC03 };
		constexpr GfxObjectID GPA_STATE_ID_RASTERIZER_DEFAULT_CULL_FRONT_CW { 0xFF00CC04 };

		constexpr GfxObjectID GPA_STATE_ID_IA_INPUT_LAYOUT_DEFAULT { 0xFF00AA01 };
		constexpr GfxObjectID GPA_STATE_ID_IA_INPUT_LAYOUT_DEFAULT_16B { 0xFF00AA02 };

		constexpr GfxObjectID GPA_STATE_ID_GRAPHICS_SHADER_LINKAGE_PASSTHROUGH_IA_DEFAULT { 0xFF00EE01 };

	}

	class GpaSharedStateLibrary : public CoreEngineObject
	{
	public:
		ShaderLibraryHandle const mShaderLibrary;

	public:
		explicit GpaSharedStateLibrary( const CoreEngineState & pCES, ShaderLibraryHandle pShaderLibrary = nullptr );
		~GpaSharedStateLibrary();

		CPPX_ATTR_NO_DISCARD GCI::ShaderHandle getShader( GfxObjectID pShaderID ) const noexcept;
		CPPX_ATTR_NO_DISCARD GCI::ShaderHandle getShader( const GpaUniqueObjectName & pShaderName ) const noexcept;

		void initialize();

	private:
		void CreateDefaultShaders();
		void initializeDefaultImmutableBlendStates();
		void initializeDefaultImmutableDepthStencilStates();
		void initializeDefaultImmutableRasterizerStates();
		void initializeDefaultImmutableInputLayoutStates();
		void initializeDefaultPipelineStateObjects();
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_GPA_SHARED_STATE_LIBRARY_H__
