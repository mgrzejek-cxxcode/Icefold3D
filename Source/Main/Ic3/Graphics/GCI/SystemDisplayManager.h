
#pragma once

#ifndef __IC3_GRAPHICS_GCI_SYSTEM_DISPLAY_MANAGER_H__
#define __IC3_GRAPHICS_GCI_SYSTEM_DISPLAY_MANAGER_H__

#include "DisplayManager.h"

namespace Ic3::Graphics::GCI
{

	ic3DeclareClassHandle( SystemDisplayManager );

	struct SystemDisplayManagerCreateInfo
	{
		System::SysContext * exfSystemContext = nullptr;
		System::EDisplayDriverType exfDisplayDriverType = System::EDisplayDriverType::Default;
	};

	/// @brief
	class IC3_GRAPHICS_GCI_CLASS SystemDisplayManager final : public DisplayManager
	{
		friend class GpuDriver;

	public:
		System::DisplayDriver * const mExfDisplayDriver = nullptr;

		explicit SystemDisplayManager( System::DisplayDriver * pExfDisplayDriver );
		virtual ~SystemDisplayManager();

		/// @brief
		virtual void Reset() override final;

		/// @brief
		virtual AdapterList EnumAdapterList() const override;

		/// @brief
		virtual AdapterDesc GetDefaultAdapter() const override;

		/// @brief
		virtual OutputList EnumOutputList( display_system_id_t pAdapterID ) const override;

		/// @brief
		virtual OutputDesc GetDefaultOutput( display_system_id_t pAdapterID ) const override;

		/// @brief
		virtual VideoModeList EnumVideoModeList( display_system_id_t pOutputID, System::EColorFormat pFormat ) const override;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_SYSTEM_DISPLAY_MANAGER_H__
