
#ifndef __IC3_SYSTEM_DISPLAY_CONFIGURATION_H__
#define __IC3_SYSTEM_DISPLAY_CONFIGURATION_H__

#include "DisplayCommon.h"
#include "SysContext.h"
#include "SysObject.h"

namespace Ic3::System
{

	ic3SysDeclareHandle( DisplayAdapter );
	ic3SysDeclareHandle( DisplayOutput );
	ic3SysDeclareHandle( DisplayVideoMode );

	/// @brief
	class DisplayAdapter : public SysObject
	{
		friend class DisplayDriver;

	public:
		EDisplayDriverType const mDriverType;
		DisplayDriver * const mDisplayDriver;

	public:
		explicit DisplayAdapter( DisplayDriver & pDisplayDriver );

		virtual ~DisplayAdapter() noexcept;

		IC3_ATTR_NO_DISCARD DisplayOutput * findOutput( DisplayOutputPredicate pPredicate ) const;
		IC3_ATTR_NO_DISCARD DisplayOutputList findOutputs( DisplayOutputPredicate pPredicate ) const;

		IC3_ATTR_NO_DISCARD DisplayOutput * getOutput( dsm_index_t pOutputIndex ) const;
		IC3_ATTR_NO_DISCARD DisplayOutput * getDefaultOutput() const;

		IC3_ATTR_NO_DISCARD const DisplayAdapterDesc & getAdapterDesc() const;
		IC3_ATTR_NO_DISCARD const DisplayOutputList & getOutputList() const;

		IC3_ATTR_NO_DISCARD bool isActiveAdapter() const;
		IC3_ATTR_NO_DISCARD bool isPrimaryAdapter() const;
		IC3_ATTR_NO_DISCARD bool hasActiveOutputs() const;
		IC3_ATTR_NO_DISCARD bool hasAnyOutputs() const;

	private:
		// Registers an output of this adapter. Adds it to the list and fills internal info.
		// Called by the display driver when the display configuration is initialized.
		void registerOutput( DisplayOutputHandle pOutput );

		// Validates the list of outputs, checks and updates flags, fills the additional pointer-based list.
		// Internally it also calls validateVideoModesConfiguration() for each output and supported color format.
		uint32 validateOutputsConfiguration();

		// Returns a writable adapter desc. Used by the driver to fill the info about this adapter.
		DisplayAdapterDesc & getAdapterDescInternal();

	protected:
		struct DisplayAdapterPrivateData;
		std::unique_ptr<DisplayAdapterPrivateData> _privateData;
	};

	/// @brief
	class DisplayOutput : public SysObject
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

		IC3_ATTR_NO_DISCARD ArrayView<const EColorFormat> getSupportedColorFormatList() const;

		IC3_ATTR_NO_DISCARD bool checkVideoSettingsSupport( const DisplayVideoSettings & pVideoSettings,
		                                                    EColorFormat pColorFormat = EColorFormat::Unknown ) const;

		IC3_ATTR_NO_DISCARD DisplayVideoMode * findVideoMode( EColorFormat pColorFormat, DisplayVideoModePredicate pPredicate ) const;
		IC3_ATTR_NO_DISCARD DisplayVideoModeList findVideoModes( EColorFormat pColorFormat, DisplayVideoModePredicate pPredicate ) const;

		IC3_ATTR_NO_DISCARD const DisplayOutputDesc & getOutputDesc() const;
		IC3_ATTR_NO_DISCARD const DisplayVideoModeList & getVideoModeList() const;
		IC3_ATTR_NO_DISCARD const DisplayVideoModeList & getVideoModeList( EColorFormat pColorFormat ) const;

		IC3_ATTR_NO_DISCARD bool isActiveOutput() const;
		IC3_ATTR_NO_DISCARD bool isPrimaryOutput() const;

		IC3_ATTR_NO_DISCARD bool isColorFormatSupported( EColorFormat pColorFormat ) const;

	private:
		// Registers a supported video mode for this output. Adds it to the list and fills internal info.
		// Called by the display driver when the display configuration is initialized.
		void registerVideoMode( EColorFormat pColorFormat, DisplayVideoModeHandle pVideoMode );

		uint32 validateVideoModesConfiguration( EColorFormat pColorFormat );

		// Returns a writable output desc. Used by the driver and parent adapter to fill the info about this output.
		DisplayOutputDesc & getOutputDescInternal();

	protected:
		struct DisplayOutputPrivateData;
		std::unique_ptr<DisplayOutputPrivateData> _privateData;
	};

	/// @brief
	class DisplayVideoMode : public SysObject
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

		IC3_ATTR_NO_DISCARD const DisplayVideoModeDesc & getModeDesc() const;

	private:
		// Returns a writable video mode desc. Used by the driver and parent output to fill the info about this video mode.
		DisplayVideoModeDesc & getModeDescInternal();

	protected:
		struct DisplayVideoModePrivateData;
		std::unique_ptr<DisplayVideoModePrivateData> _privateData;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_DISPLAY_CONFIGURATION_H__
