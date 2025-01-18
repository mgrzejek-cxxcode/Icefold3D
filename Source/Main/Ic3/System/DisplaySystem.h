
#ifndef __IC3_SYSTEM_DISPLAY_SYSTEM_H__
#define __IC3_SYSTEM_DISPLAY_SYSTEM_H__

#include "SysContext.h"
#include "SysObject.h"
#include "DisplayCommon.h"
#include "WindowCommon.h"

namespace Ic3::System
{

	Ic3SysDeclareHandle( DisplayAdapter );
	Ic3SysDeclareHandle( DisplayOutput );
	Ic3SysDeclareHandle( DisplayVideoMode );

	/// @brief
	class DisplayManager : public SysObject
	{
	public:
		explicit DisplayManager( SysContextHandle pSysContext );
		virtual ~DisplayManager() noexcept;

		CPPX_ATTR_NO_DISCARD DisplayDriverHandle CreateDisplayDriver( EDisplayDriverType pDriverID );

		CPPX_ATTR_NO_DISCARD DisplayDriverHandle CreateDisplayDriver();

		CPPX_ATTR_NO_DISCARD bool CheckDriverSupport( EDisplayDriverType pDriverID ) const;

		CPPX_ATTR_NO_DISCARD EDisplayDriverType ResolveDisplayDriverID( EDisplayDriverType pDriverID ) const;

		CPPX_ATTR_NO_DISCARD DisplayOffset QueryDefaultDisplayOffset() const;

		CPPX_ATTR_NO_DISCARD DisplaySize QueryDefaultDisplaySize() const;

		CPPX_ATTR_NO_DISCARD DisplaySize QueryMinWindowSize() const;

		CPPX_ATTR_NO_DISCARD bool CheckFrameGeometry( const FrameGeometry & pFrameGeometry ) const;

		CPPX_ATTR_NO_DISCARD FrameGeometry ValidateFrameGeometry( const FrameGeometry & pFrameGeometry ) const;

	private:
		virtual DisplayDriverHandle _NativeCreateDisplayDriver() = 0;

		virtual void _NativeQueryDefaultDisplayOffset( DisplayOffset & pOutOffset ) const = 0;

		virtual void _NativeQueryDefaultDisplaySize( DisplaySize & pOutSize ) const = 0;

		virtual void _NativeQueryMinWindowSize( DisplaySize & pOutSize ) const = 0;

	protected:
		struct DisplayManagerPrivateData;
		std::unique_ptr<DisplayManagerPrivateData> _privateData;
	};

	/// @brief
	class DisplayDriver : public SysObject
	{
		friend class DisplayManager;

	public:
		DisplayManagerHandle const mDisplayManager;
		EDisplayDriverType const mDriverType;

	public:
		virtual ~DisplayDriver();

		explicit operator bool() const;

		void SyncDisplayConfiguration();

		void Reset();

		EColorFormat QueryDefaultSystemColorFormat() const;

		CPPX_ATTR_NO_DISCARD DisplayAdapter * FindAdapter( DisplayAdapterPredicate pPredicate ) const;
		CPPX_ATTR_NO_DISCARD DisplayAdapterList FindAdapters( DisplayAdapterPredicate pPredicate ) const;

		CPPX_ATTR_NO_DISCARD const DisplayAdapterList & GetAdapterList() const;
		CPPX_ATTR_NO_DISCARD const DisplayOutputList & GetOutputList( dsm_index_t pAdapterIndex ) const;

		CPPX_ATTR_NO_DISCARD DisplayAdapter * GetAdapter( dsm_index_t pAdapterIndex ) const;
		CPPX_ATTR_NO_DISCARD DisplayAdapter * GetDefaultAdapter() const;

		CPPX_ATTR_NO_DISCARD DisplayOutput * GetDefaultOutput( dsm_index_t pAdapterIndex = CX_DSM_INDEX_DEFAULT ) const;
		CPPX_ATTR_NO_DISCARD DisplayOutput * GetOutput( dsm_output_id_t pOutputID ) const;

		CPPX_ATTR_NO_DISCARD bool HasActiveAdapters() const;
		CPPX_ATTR_NO_DISCARD bool HasAnyAdapters() const;

		CPPX_ATTR_NO_DISCARD bool HasValidConfiguration() const;

		CPPX_ATTR_NO_DISCARD std::string GenerateConfigurationDump( const std::string & pLinePrefix = {} ) const;

	protected:
		DisplayDriver( DisplayManagerHandle pDisplayManager, EDisplayDriverType pDriverType );

		static DisplayAdapterDesc & GetAdapterDescInternal( DisplayAdapter & pAdapter );

		static DisplayOutputDesc & GetOutputDescInternal( DisplayOutput & pOutput );

		static DisplayVideoModeDesc & GetVideoModeDescInternal( DisplayVideoMode & pVideoMode );

		template <typename TPAdapter, typename TDriver>
		TSysHandle<TPAdapter> CreateAdapter( TDriver & pDriver)
		{
			auto adapterHandle = CreateSysObject<TPAdapter>( pDriver );
			_registerAdapter( adapterHandle );
			return adapterHandle;
		}

		template <typename TPOutput, typename TPAdapter>
		TSysHandle<TPOutput> CreateOutput( TPAdapter & pAdapter )
		{
			auto outputHandle = CreateSysObject<TPOutput>( pAdapter );
			_RegisterOutput( pAdapter, outputHandle );
			return outputHandle;
		}

		template <typename TVideoMode, typename TPOutput>
		TSysHandle<TVideoMode> CreateVideoMode( TPOutput & pOutput, EColorFormat pColorFormat )
		{
			auto videoModeHandle = CreateSysObject<TVideoMode>( pOutput );
			_RegisterVideoMode( pOutput, pColorFormat, videoModeHandle );
			return videoModeHandle;
		}

	private:
		/// Platform-level function: enumerates display devices in the system (adapters and their outputs).
		virtual void _NativeEnumDisplayDevices() = 0;

		/// Platform-level function: enumerates supported video modes for a given output and color format.
		virtual void _NativeEnumVideoModes( DisplayOutput & pOutput, EColorFormat pColorFormat ) = 0;

		/// Platform-level function: returns the default color format (OS-/driver-specific).
		virtual EColorFormat _NativeQueryDefaultSystemColorFormat() const = 0;

		void _InitializeDisplayConfiguration();
		void _ResetDisplayConfiguration();
		void _enumDisplayDevices();
		void _enumVideoModes();
		void _registerAdapter( DisplayAdapterHandle pAdapter );
		void _RegisterOutput( DisplayAdapter & pAdapter, DisplayOutputHandle pOutput );
		void _RegisterVideoMode( DisplayOutput & pOutput, EColorFormat pColorFormat, DisplayVideoModeHandle pVideoMode );
		void _validateAdaptersConfiguration();

	protected:
		struct DisplayDriverPrivateData;
		std::unique_ptr<DisplayDriverPrivateData> _privateData;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_DISPLAY_SYSTEM_H__
