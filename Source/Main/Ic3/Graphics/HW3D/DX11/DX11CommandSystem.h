
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX11_COMMAND_SYSTEM_H__
#define __IC3_GRAPHICS_HW3D_DX11_COMMAND_SYSTEM_H__

#include "DX11Prerequisites.h"
#include <Ic3/Graphics/GCI/CommandSystem.h>
#include <mutex>

namespace Ic3::Graphics::GCI
{

	struct DX11CommandSyncData
	{
		ID3D11DeviceContext1 * d3d11DeviceContext1 = nullptr;
		ID3D11Query * d3d11SyncQuery = nullptr;
	};

	IC3_GX_DX11_API void releaseDX11CommandSyncData( void * pSyncData );

	/// @brief
	class IC3_GX_DX11_CLASS DX11CommandSystem final : public CommandSystem
	{
	public:
		ComPtr<ID3D11Device1> const mD3D11Device1 = nullptr;

		explicit DX11CommandSystem( DX11GpuDevice & pDX11GpuDevice );
		virtual ~DX11CommandSystem();

		virtual std::unique_ptr<CommandContext> AcquireCommandContext( ECommandContextType pContextType ) override;

		virtual CommandSync SubmitContext( CommandContextDirect & pContext, const CommandContextSubmitInfo & pSubmitInfo ) override;

	private:
		DX11CommandList * AcquireCommandList( ECommandExecutionMode pCommandExecutionMode );
		bool InitializeDirectCommandList();
		bool CreateDeferredCommandList();

	private:
		DX11CommandListHandle _directCommandList;
		using DX11CommandListStorage = std::unordered_map<DX11CommandList *, DX11CommandListHandle>;
		DX11CommandListStorage _deferredCommandListStorage;
		using DX11CommandListList = std::list<DX11CommandList *>;
		DX11CommandListList _availableList;
		std::mutex _availableListLock;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_COMMAND_SYSTEM_H__
