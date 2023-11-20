
#include "ShaderInputSemantics.h"

namespace Ic3
{

	bool SShaderSemantics::resolveSemantics( SShaderSemantics & pSemantics )
	{
		if( pSemantics.smtID == EShaderInputSemanticID::Custom )
		{
			// Custom semantics, so a non-empty semantic name is required.
			if( pSemantics.smtName.empty() )
			{
				return false;
			}
		}
		else if( CxDef::isStandardShaderInputSemanticID( pSemantics.smtID )  )
		{
			// One of the standard semantic IDs, semantic name resolved from the ID.
			pSemantics.smtName = GCIUtils::getShaderInputSemanticNameFromID( pSemantics.smtID );
		}
		else
		{
			if( pSemantics.smtName.empty() )
			{
				return false;
			}

			pSemantics.smtID = GCIUtils::getShaderInputSemanticIDFromName( pSemantics.smtName );
		}

		return true;
	}

	SShaderSemantics SShaderSemantics::resolveSemantics( const SShaderSemantics & pSemantics )
	{
	}

	namespace GCIUtils
	{

		EShaderInputSemanticID getShaderInputSemanticIDFromName( const std::string_view & pSemanticName )
		{
			if( pSemanticName == "POSITION"  ) return EShaderInputSemanticID::Position  ;
			if( pSemanticName == "NORMAL"    ) return EShaderInputSemanticID::Normal    ;
			if( pSemanticName == "TANGENT"   ) return EShaderInputSemanticID::Tangent   ;
			if( pSemanticName == "COLOR"     ) return EShaderInputSemanticID::FixedColor;
			if( pSemanticName == "TEXCOORD0" ) return EShaderInputSemanticID::TexCoord0 ;
			if( pSemanticName == "TEXCOORD1" ) return EShaderInputSemanticID::TexCoord1 ;
			if( pSemanticName == "TEXCOORD2" ) return EShaderInputSemanticID::TexCoord2 ;
			if( pSemanticName == "TEXCOORD3" ) return EShaderInputSemanticID::TexCoord3 ;
			if( pSemanticName == "TEXCOORD4" ) return EShaderInputSemanticID::TexCoord4 ;
			if( pSemanticName == "TEXCOORD5" ) return EShaderInputSemanticID::TexCoord5 ;
			if( pSemanticName == "TEXCOORD6" ) return EShaderInputSemanticID::TexCoord6 ;
			if( pSemanticName == "TEXCOORD7" ) return EShaderInputSemanticID::TexCoord7 ;
			if( pSemanticName == "INSTANCE0" ) return EShaderInputSemanticID::Instance0 ;
			if( pSemanticName == "INSTANCE1" ) return EShaderInputSemanticID::Instance1 ;

			return EShaderInputSemanticID::Custom;
		}

		const char * getShaderInputSemanticNameFromID( EShaderInputSemanticID pSemanticID )
		{
			switch( pSemanticID )
			{
				ic3CaseReturn( EShaderInputSemanticID::Position   , "POSITION"  );
				ic3CaseReturn( EShaderInputSemanticID::Normal     , "NORMAL"    );
				ic3CaseReturn( EShaderInputSemanticID::Tangent    , "TANGENT"   );
				ic3CaseReturn( EShaderInputSemanticID::FixedColor , "COLOR"     );
				ic3CaseReturn( EShaderInputSemanticID::TexCoord0  , "TEXCOORD0" );
				ic3CaseReturn( EShaderInputSemanticID::TexCoord1  , "TEXCOORD1" );
				ic3CaseReturn( EShaderInputSemanticID::TexCoord2  , "TEXCOORD2" );
				ic3CaseReturn( EShaderInputSemanticID::TexCoord3  , "TEXCOORD3" );
				ic3CaseReturn( EShaderInputSemanticID::TexCoord4  , "TEXCOORD4" );
				ic3CaseReturn( EShaderInputSemanticID::TexCoord5  , "TEXCOORD5" );
				ic3CaseReturn( EShaderInputSemanticID::TexCoord6  , "TEXCOORD6" );
				ic3CaseReturn( EShaderInputSemanticID::TexCoord7  , "TEXCOORD7" );
				ic3CaseReturn( EShaderInputSemanticID::Instance0  , "INSTANCE0" );
				ic3CaseReturn( EShaderInputSemanticID::Instance1  , "INSTANCE1" );
				ic3CaseDefaultBreak();
			}

			return "";
		}

	}

}
