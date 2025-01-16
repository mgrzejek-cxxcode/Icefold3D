
#include "OSXSysContext.h"
#include "NSIApplicationProxy.h"
#import <AppKit/NSNibLoading.h>

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_OSX )
namespace Ic3::System
{

	namespace Platform
	{

		OSXSharedData & OSXGetOSXSharedData( SysContext & pSysContext )
		{
			auto * osxSysContext = pSysContext.QueryInterface<OSXSysContext>();
			return osxSysContext->mNativeData.mOSXSharedData;
		}

		OSXSharedData & OSXGetOSXSharedData( OSXSysContext & pSysContext )
		{
			return pSysContext.mNativeData.mOSXSharedData;
		}

		bool OSXNibLoadMenuNibFile()
		{
		@autoreleasepool
		{
			const auto cvAppKitVersion = floor( NSAppKitVersionNumber );

			if( cvAppKitVersion >= NSAppKitVersionNumber10_8 )
			{
				NSBundle * mainBundle = [NSBundle mainBundle];
				NSDictionary * bundleInfoDict = [mainBundle infoDictionary];
				if( bundleInfoDict != nil )
				{
					NSString * mainNibFleNameStr = [bundleInfoDict valueForKey:@"NSMainNibFile"];

					if( mainNibFleNameStr != nil )
					{
						if( [mainBundle loadNibNamed:mainNibFleNameStr owner:[NSApplication sharedApplication] topLevelObjects:nil] )
						{
							return true;
						}
					}
					else
					{
						NSString * mainMenuNibResource = [mainBundle pathForResource:@"MainMenu" ofType:@"nib"];
						if( mainMenuNibResource != nil )
						{
							if( [mainBundle loadNibNamed:@"MainMenu" owner:[NSApplication sharedApplication] topLevelObjects:nil] )
							{
								return true;
							}
						}

					}
				}
			}

			return false;
		}
		}

		void OSXNibCreateDefaultApplicationMenu()
		{
		@autoreleasepool
		{
			if( NSApp == nil )
			{
				return;
			}

			NSMenu * mainAppMenuBar = [[NSMenu alloc] init];
			[NSApp setMainMenu:mainAppMenuBar];

			auto * appMenuItem = [mainAppMenuBar addItemWithTitle:@"" action:nullptr keyEquivalent:@""];
			auto * appMenu = [[NSMenu alloc] init];
			[appMenuItem setSubmenu:appMenu];

			auto * servicesMenuItem = [appMenu addItemWithTitle:@"Services" action:nullptr keyEquivalent:@""];
			auto * servicesMenu = [[NSMenu alloc] init];
			[NSApp setServicesMenu:servicesMenu];
			[servicesMenuItem setSubmenu:servicesMenu];

			auto * windowMenuItem = [mainAppMenuBar addItemWithTitle:@"" action:nullptr keyEquivalent:@""];
			auto * windowMenu = [[NSMenu alloc] initWithTitle:@"Window"];
			[NSApp setWindowsMenu:windowMenu];
			[windowMenuItem setSubmenu:windowMenu];

			SEL setAppleMenuSelector = NSSelectorFromString(@"setAppleMenu:");
			[NSApp performSelector:setAppleMenuSelector withObject:appMenu];

			[mainAppMenuBar release];
		}
		}

		bool OSXCheckAppKitFrameworkVersion( NSAppKitVersion pRequiredVersion )
		{
			const auto cvAppKitVersion = floor( NSAppKitVersionNumber );

			if( cvAppKitVersion >= pRequiredVersion )
			{
				return true;
			}

			return false;
		}

		const char * OSXQueryCGErrorMessage( CGError pCGError )
		{
			switch( pCGError )
			{
			case kCGErrorSuccess           : return "kCGErrorSuccess"           ;
			case kCGErrorFailure           : return "kCGErrorFailure"           ;
			case kCGErrorIllegalArgument   : return "kCGErrorIllegalArgument"   ;
			case kCGErrorInvalidConnection : return "kCGErrorInvalidConnection" ;
			case kCGErrorInvalidContext    : return "kCGErrorInvalidContext"    ;
			case kCGErrorCannotComplete    : return "kCGErrorCannotComplete"    ;
			case kCGErrorNotImplemented    : return "kCGErrorNotImplemented"    ;
			case kCGErrorRangeCheck        : return "kCGErrorRangeCheck"        ;
			case kCGErrorTypeCheck         : return "kCGErrorTypeCheck"         ;
			case kCGErrorInvalidOperation  : return "kCGErrorInvalidOperation"  ;
			case kCGErrorNoneAvailable     : return "kCGErrorNoneAvailable"     ;
			}
			return "<UNKNOWN>";
		}

	}

} // namespace Ic3::System
#endif // PCL_TARGET_SYSAPI_OSX
