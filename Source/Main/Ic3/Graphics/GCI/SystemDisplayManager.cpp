
#include "SystemDisplayManager.h"
#include <Ic3/System/DisplayDriver.h>
#include <Ic3/System/DisplaySystem.h>

namespace Ic3::Graphics::GCI
{

	SystemDisplayManager::SystemDisplayManager( System::DisplayDriver * pExfDisplayDriver )
	: DisplayManager()
	, mExfDisplayDriver( pExfDisplayDriver )
	{}

	SystemDisplayManager::~SystemDisplayManager() = default;

	void SystemDisplayManager::Reset()
	{
	}

	AdapterList SystemDisplayManager::EnumAdapterList() const
	{
		AdapterList adapterList;

		try
		{
			auto sysAdapterList = mExfDisplayDriver->EnumAdapterList();
			for( auto * sysAdapterDesc : sysAdapterList )
			{
				auto & adapterDesc = adapterList.emplace_back();
				adapterDesc.id = sysAdapterDesc->index;
				adapterDesc.name = sysAdapterDesc->name;
				adapterDesc.flags = translateSysAdapterFlags( sysAdapterDesc->flags );
			}
		}
		catch ( ... )
		{
			Ic3DebugInterrupt();
		}

		return adapterList;
	}

	AdapterDesc SystemDisplayManager::GetDefaultAdapter() const
	{
		AdapterDesc adapterDesc;

		try
		{
			auto * sysDefaultAdapterDesc = mExfDisplayDriver->GetDefaultAdapter();
			if( sysDefaultAdapterDesc != nullptr )
			{
				adapterDesc.id = sysDefaultAdapterDesc->index;
				adapterDesc.name = sysDefaultAdapterDesc->name;
				adapterDesc.flags = translateSysAdapterFlags( sysDefaultAdapterDesc->flags );
			}
		}
		catch ( ... )
		{
			Ic3DebugInterrupt();
		}

		return adapterDesc;
	}

	OutputList SystemDisplayManager::EnumOutputList( display_system_id_t pAdapterID ) const
	{
		OutputList outputList;

		try
		{
			auto sysAdapterIndex = static_cast<Ic3::sys_dsm_index_t>( pAdapterID );
			auto sysOutputList = mExfDisplayDriver->EnumOutputList( sysAdapterIndex );
			for( auto * sysOutputDesc : sysOutputList )
			{
				auto & outputDesc = outputList.emplace_back();
				outputDesc.id = sysOutputDesc->id;
				outputDesc.name = sysOutputDesc->name;
				outputDesc.screenRect = sysOutputDesc->screenRect;
				outputDesc.flags = translateSysOutputFlags( sysOutputDesc->flags );
			}
		}
		catch ( ... )
		{
			Ic3DebugInterrupt();
		}

		return outputList;
	}

	OutputDesc SystemDisplayManager::GetDefaultOutput( display_system_id_t pAdapterID ) const
	{
		OutputDesc outputDesc;

		try
		{
			auto sysAdapterIndex = static_cast<Ic3::sys_dsm_index_t>( pAdapterID );
			auto * sysDefaultOutputDesc = mExfDisplayDriver->GetDefaultOutput( sysAdapterIndex );
			if( sysDefaultOutputDesc != nullptr )
			{
				outputDesc.id = sysDefaultOutputDesc->id;
				outputDesc.name = sysDefaultOutputDesc->name;
				outputDesc.screenRect = sysDefaultOutputDesc->screenRect;
				outputDesc.flags = translateSysOutputFlags( sysDefaultOutputDesc->flags );
			}
		}
		catch ( ... )
		{
			Ic3DebugInterrupt();
		}

		return outputDesc;
	}

	VideoModeList SystemDisplayManager::EnumVideoModeList( display_system_id_t pOutputID, System::EColorFormat pFormat ) const
	{
		VideoModeList videoModeList;

		try
		{
			auto sysOutputID = static_cast<Ic3::sys_dsm_output_id_t>( pOutputID );
			auto sysVideoModeList = mExfDisplayDriver->EnumVideoModeList( sysOutputID, pFormat );
			for( auto * sysVideoModeDesc : sysVideoModeList )
			{
				auto & outputDesc = videoModeList.emplace_back();
				outputDesc.id = sysVideoModeDesc->id;
				outputDesc.settings.resolution = sysVideoModeDesc->settings.resolution;
				outputDesc.settings.refreshRate = sysVideoModeDesc->settings.refreshRate;
				outputDesc.settings.flags = translateSysVideoSettingsFlags( sysVideoModeDesc->settings.flags );
				outputDesc.format = pFormat;
			}
		}
		catch ( ... )
		{
			Ic3DebugInterrupt();
		}

		return videoModeList;
	}

} // namespace Ic3::Graphics::GCI
