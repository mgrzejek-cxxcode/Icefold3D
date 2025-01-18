
#pragma once

#ifndef __IC3_NXMAIN_SHADOW_RENDERER_H__
#define __IC3_NXMAIN_SHADOW_RENDERER_H__

#include "CommonRendererDefs.h"
#include <Ic3/Graphics/GCI/State/RenderTargetDynamicStates.h>

namespace Ic3
{

	struct RLConstantBufferDataCommonModel
	{
		Math::Mat4f m4fModelMatrix;
	};

	struct RLConstantBufferDataCommonScene
	{
		Math::Mat4f m4fViewMatrix;
		Math::Mat4f m4fProjectionMatrix;
	};

	class Renderer
	{
	public:
		GCI::GPUDeviceHandle const mGPUDevice;
		ShaderLibraryHandle const mShaderLibrary;

	public:
		Renderer( ShaderLibraryHandle pShaderLibrary );
		virtual ~Renderer();

		virtual void createRendererResources();

		void beginRendering( GCI::CommandContext & pCommandContext );
		void endRendering();

		void assignConstantBuffer( native_uint pCBIndex, GCI::GPUBufferHandle pConstantBuffer );

		template <typename TCBData>
		void createConstantBuffer();

		template <typename TCBData>
		bool setConstantBufferData( native_uint pCBIndex, const TCBData & pCBData );

		template <typename TCBData>
		TCBData * updateConstantBufferData( native_uint pCBIndex );

	protected:
		struct GPUAPIStateCommon
		{
		};

		struct ConstantBufferLocalDataRef
		{
			uint32 bufferSize = 0;
			void * dataPtr = nullptr;
			uint32 dataSize = 0;
			uint32 modified = 0;
		};

		struct LocalDataCache
		{
			RLConstantBufferDataCommonModel cbDataCommonModel;
			RLConstantBufferDataCommonScene cbDataCommonScene;
		};

	protected:
		ShaderLibraryHandle _shaderLibrary;
		GPUAPIStateCommon _gpaStateCommon;
		GCI::CommandContextHandle _currentCmdContext;
	};


} // namespace Ic3

#endif // __IC3_NXMAIN_SHADOW_RENDERER_H__
