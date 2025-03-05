
#ifndef __IC3_SYSTEM_DISPLAY_CONFIGURATION_H__
#define __IC3_SYSTEM_DISPLAY_CONFIGURATION_H__

#include "DisplayCommon.h"
#include "SysContext.h"
#include "SysObject.h"

namespace Ic3::System
{

	Ic3SysDeclareHandle( DisplayAdapter );
	Ic3SysDeclareHandle( DisplayOutput );
	Ic3SysDeclareHandle( DisplayVideoMode );

	/// @brief
	class IC3_SYSTEM_CLASS DisplayAdapter : public SysObject
	{
		friend class DisplayDriver;

	public:
		EDisplayDriverType const mDriverType;
		DisplayDriver * const mDisplayDriver;

	public:
		explicit DisplayAdapter( DisplayDriver & pDisplayDriver );

		virtual ~DisplayAdapter() noexcept;

		CPPX_ATTR_NO_DISCARD DisplayOutput * FindOutput( DisplayOutputPredicate pPredicate ) const;
		CPPX_ATTR_NO_DISCARD DisplayOutputList FindOutputs( DisplayOutputPredicate pPredicate ) const;

		CPPX_ATTR_NO_DISCARD DisplayOutput * GetOutput( dsm_index_t pOutputIndex ) const;
		CPPX_ATTR_NO_DISCARD DisplayOutput * GetDefaultOutput() const;

		CPPX_ATTR_NO_DISCARD const DisplayAdapterDesc & GetAdapterDesc() const;
		CPPX_ATTR_NO_DISCARD const DisplayOutputList & GetOutputList() const;

		CPPX_ATTR_NO_DISCARD bool IsActiveAdapter() const;
		CPPX_ATTR_NO_DISCARD bool IsPrimaryAdapter() const;
		CPPX_ATTR_NO_DISCARD bool HasActiveOutputs() const;
		CPPX_ATTR_NO_DISCARD bool HasAnyOutputs() const;

	private:
		// Registers an output of this adapter. Adds it to the list and fills internal info.
		// Called by the display driver when the display configuration is initialized.
		void RegisterOutput( DisplayOutputHandle pOutput );

		// Validates the list of outputs, checks and updates flags, fills the additional pointer-based list.
		// Internally it also calls ValidateVideoModesConfiguration() for each output and supported color format.
		uint32 ValidateOutputsConfiguration();

		// Returns a writable adapter desc. Used by the driver to fill the info about this adapter.
		DisplayAdapterDesc & GetAdapterDescInternal();

	protected:
		struct DisplayAdapterPrivateData;
		std::unique_ptr<DisplayAdapterPrivateData> _privateData;
	};

	/// @brief
	class IC3_SYSTEM_CLASS DisplayOutput : public SysObject
	{
		friend class DisplayDriver;
		friend class DisplayAdapter;

	public:
		EDisplayDriverType const mDriverType;
		DisplayDriver * const mDisplayDriver;
		DisplayAdapter * const mParentAdapter;

	public:
		explicit DisplayOutput( DisplayAdapter & pDisplayAdapter );
		virtual ~DisplayOutput() noexcept;

		CPPX_ATTR_NO_DISCARD cppx::array_view<const EColorFormat> GetSupportedColorFormatList() const;

		CPPX_ATTR_NO_DISCARD bool CheckVideoSettingsSupport(
				const DisplayVideoSettings & pVideoSettings,
				EColorFormat pColorFormat = EColorFormat::Unknown ) const;

		CPPX_ATTR_NO_DISCARD DisplayVideoMode * FindVideoMode( EColorFormat pColorFormat, DisplayVideoModePredicate pPredicate ) const;
		CPPX_ATTR_NO_DISCARD DisplayVideoModeList FindVideoModes( EColorFormat pColorFormat, DisplayVideoModePredicate pPredicate ) const;

		CPPX_ATTR_NO_DISCARD const DisplayOutputDesc & GetOutputDesc() const;
		CPPX_ATTR_NO_DISCARD const DisplayVideoModeList & GetVideoModeList() const;
		CPPX_ATTR_NO_DISCARD const DisplayVideoModeList & GetVideoModeList( EColorFormat pColorFormat ) const;

		CPPX_ATTR_NO_DISCARD bool IsActiveOutput() const;
		CPPX_ATTR_NO_DISCARD bool IsPrimaryOutput() const;

		CPPX_ATTR_NO_DISCARD bool IsColorFormatSupported( EColorFormat pColorFormat ) const;

	private:
		// Registers a supported video mode for this output. Adds it to the list and fills internal info.
		// Called by the display driver when the display configuration is initialized.
		void RegisterVideoMode( EColorFormat pColorFormat, DisplayVideoModeHandle pVideoMode );

		uint32 ValidateVideoModesConfiguration( EColorFormat pColorFormat );

		// Returns a writable output desc. Used by the driver and parent adapter to fill the info about this output.
		DisplayOutputDesc & GetOutputDescInternal();

	protected:
		struct DisplayOutputPrivateData;
		std::unique_ptr<DisplayOutputPrivateData> _privateData;
	};

	/// @brief
	class IC3_SYSTEM_CLASS DisplayVideoMode : public SysObject
	{
		friend class DisplayDriver;
		friend class DisplayOutput;

	public:
		EDisplayDriverType const mDriverType;
		DisplayDriver * const mDisplayDriver;
		DisplayOutput * const mParentOutput;

	public:
		explicit DisplayVideoMode( DisplayOutput & pDisplayOutput );
		virtual ~DisplayVideoMode() noexcept;

		CPPX_ATTR_NO_DISCARD const DisplayVideoModeDesc & GetModeDesc() const;

	private:
		// Returns a writable video mode desc. Used by the driver and parent output to fill the info about this video mode.
		DisplayVideoModeDesc & GetModeDescInternal();

	protected:
		struct DisplayVideoModePrivateData;
		std::unique_ptr<DisplayVideoModePrivateData> _privateData;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_DISPLAY_CONFIGURATION_H__
