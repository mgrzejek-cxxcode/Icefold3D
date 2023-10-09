
#pragma once

#ifndef __IC3_GRAPHICS_GCI_SYSTEM_DISPLAY_MANAGER_H__
#define __IC3_GRAPHICS_GCI_SYSTEM_DISPLAY_MANAGER_H__

#include "DisplayManager.h"

namespace Ic3::Graphics::GCI
{

	ic3DeclareClassHandle( SystemDisplayManager );

	struct SystemDisplayManagerCreateInfo
	{
		System::temContext * exfSystemContext = nullptr;
		Ic3::ESysDsmDisplayDriverType exfDisplayDriverType = Ic3::ESysDsmDisplayDriverType::Default;
	};

	/// @brief
	class IC3_GRAPHICS_GCI_CLASS SystemDisplayManager final : public DisplayManager
	{
		friend class GPUDriver;

	public:
		System::DisplayDriver * const mExfDisplayDriver = nullptr;

		explicit SystemDisplayManager( System::DisplayDriver * pExfDisplayDriver );
		virtual ~SystemDisplayManager();

		/// @brief
		virtual void reset() override final;

		/// @brief
		virtual AdapterList enumAdapterList() const override;

		/// @brief
		virtual AdapterDesc getDefaultAdapter() const override;

		/// @brief
		virtual OutputList enumOutputList( display_system_id_t pAdapterID ) const override;

		/// @brief
		virtual OutputDesc getDefaultOutput( display_system_id_t pAdapterID ) const override;

		/// @brief
		virtual VideoModeList enumVideoModeList( display_system_id_t pOutputID, System::EColorFormat pFormat ) const override;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_SYSTEM_DISPLAY_MANAGER_H__
