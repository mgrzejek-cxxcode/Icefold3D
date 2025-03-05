
#pragma once

#ifndef __IC3_GRAPHICS_GCI_DISPLAY_MANAGER_H__
#define __IC3_GRAPHICS_GCI_DISPLAY_MANAGER_H__

#include "DisplayCommon.h"

namespace Ic3::Graphics::GCI
{

	Ic3GCIDeclareClassHandle( DisplayManager );

	using AdapterList = std::vector<AdapterDesc>;
	using OutputList = std::vector<OutputDesc>;
	using VideoModeList = std::vector<VideoModeDesc>;

	/// @brief
	class IC3_GRAPHICS_GCI_API DisplayManager : public IDynamicObject
	{
	public:
		DisplayManager();
		virtual ~DisplayManager();

		/// @brief
		virtual void Reset() = 0;

		/// @brief
		virtual AdapterList EnumAdapterList() const = 0;

		/// @brief
		virtual AdapterDesc GetDefaultAdapter() const = 0;

		/// @brief
		virtual OutputList EnumOutputList( display_system_id_t pAdapterID ) const = 0;

		/// @brief
		virtual OutputDesc GetDefaultOutput( display_system_id_t pAdapterID ) const = 0;

		/// @brief
		virtual VideoModeList EnumVideoModeList( display_system_id_t pOutputID, System::EColorFormat pFormat ) const = 0;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_DISPLAY_MANAGER_H__
