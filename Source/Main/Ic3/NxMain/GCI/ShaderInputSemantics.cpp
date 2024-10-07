
#include "ShaderInputSemantics.h"
#include "VertexAttributeCommonDefs.h"

namespace Ic3
{

	bool ShaderSemantics::resolve() noexcept
	{
		if( smtID == EShaderInputSemanticID::Custom )
		{
			// Custom semantics, so a non-empty semantic name is required.
			if( smtName.empty() )
			{
				return false;
			}
		}
		else if( CxDef::isStandardShaderInputSemanticID( smtID )  )
		{
			// One of the standard semantic IDs, semantic name resolved from the ID.
			smtName = GCU::getShaderInputSemanticNameFromID( smtID );
		}
		else
		{
			if( smtName.empty() )
			{
				return false;
			}

			smtID = GCU::getShaderInputSemanticIDFromName( smtName );
		}

		return true;
	}

	ShaderSemantics ShaderSemantics::resolveSemantics( const ShaderSemantics & pSemantics )
	{
		ShaderSemantics resolvedSemantics{ pSemantics };
		if( !resolvedSemantics.resolve() )
		{
			resolvedSemantics.clear();
		}
		return resolvedSemantics;
	}

	ShaderSemantics ShaderSemantics::fromVertexAttributeKey( VertexAttributeKey pAttributeKey, uint32 pSmtIndex )
	{
		const auto smtID= CxDef::getVertexAttributeKeySemanticID( pAttributeKey );
		auto smtName = GCU::getShaderInputSemanticNameFromID( smtID );
		return ShaderSemantics{ smtID, std::move( smtName ), pSmtIndex };
	}

	namespace GCU
	{

		EShaderInputSemanticID getShaderInputSemanticIDFromName( const std::string_view & pSemanticName )
		{
			if( pSemanticName == "POSITION"  ) return EShaderInputSemanticID::Position  ;
			if( pSemanticName == "NORMAL"    ) return EShaderInputSemanticID::Normal    ;
			if( pSemanticName == "TANGENT"   ) return EShaderInputSemanticID::Tangent   ;
			if( pSemanticName == "BITANGENT" ) return EShaderInputSemanticID::BiTangent ;
			if( pSemanticName == "COLOR"     ) return EShaderInputSemanticID::FixedColor;

			if( pSemanticName == "TEXCOORD0" ) return EShaderInputSemanticID::TexCoord0 ;
			if( pSemanticName == "TEXCOORD1" ) return EShaderInputSemanticID::TexCoord1 ;
			if( pSemanticName == "TEXCOORD2" ) return EShaderInputSemanticID::TexCoord2 ;

			if( pSemanticName == "BLEND_INDICES"   ) return EShaderInputSemanticID::BlendIndices    ;
			if( pSemanticName == "BLEND_WEIGHTS"   ) return EShaderInputSemanticID::BlendWeights    ;
			if( pSemanticName == "INSTANCE_MATRIX" ) return EShaderInputSemanticID::InstanceMatrix  ;
			if( pSemanticName == "INSTANCE_UDATA"  ) return EShaderInputSemanticID::InstanceUserData;

			if( pSemanticName == "TEXCOORD3" ) return EShaderInputSemanticID::TexCoord3 ;
			if( pSemanticName == "TEXCOORD4" ) return EShaderInputSemanticID::TexCoord4 ;
			if( pSemanticName == "TEXCOORD5" ) return EShaderInputSemanticID::TexCoord5 ;
			if( pSemanticName == "TEXCOORD6" ) return EShaderInputSemanticID::TexCoord6 ;
			if( pSemanticName == "TEXCOORD7" ) return EShaderInputSemanticID::TexCoord7 ;

			return EShaderInputSemanticID::Custom;
		}

		std::string_view getShaderInputSemanticNameFromID( EShaderInputSemanticID pSemanticID )
		{
			switch( pSemanticID )
			{
				ic3CaseReturn( EShaderInputSemanticID::Position   , "POSITION"  );
				ic3CaseReturn( EShaderInputSemanticID::Normal     , "NORMAL"    );
				ic3CaseReturn( EShaderInputSemanticID::Tangent    , "TANGENT"   );
				ic3CaseReturn( EShaderInputSemanticID::BiTangent  , "BITANGENT" );
				ic3CaseReturn( EShaderInputSemanticID::FixedColor , "COLOR"     );

				ic3CaseReturn( EShaderInputSemanticID::BlendIndices     , "BLEND_INDICES"   );
				ic3CaseReturn( EShaderInputSemanticID::BlendWeights     , "BLEND_WEIGHTS"   );
				ic3CaseReturn( EShaderInputSemanticID::InstanceMatrix   , "INSTANCE_MATRIX" );
				ic3CaseReturn( EShaderInputSemanticID::InstanceUserData , "INSTANCE_UDATA"  );

				ic3CaseReturn( EShaderInputSemanticID::TexCoord0 , "TEXCOORD0" );
				ic3CaseReturn( EShaderInputSemanticID::TexCoord1 , "TEXCOORD1" );
				ic3CaseReturn( EShaderInputSemanticID::TexCoord2 , "TEXCOORD2" );
				ic3CaseReturn( EShaderInputSemanticID::TexCoord3 , "TEXCOORD3" );
				ic3CaseReturn( EShaderInputSemanticID::TexCoord4 , "TEXCOORD4" );
				ic3CaseReturn( EShaderInputSemanticID::TexCoord5 , "TEXCOORD5" );
				ic3CaseReturn( EShaderInputSemanticID::TexCoord6 , "TEXCOORD6" );
				ic3CaseReturn( EShaderInputSemanticID::TexCoord7 , "TEXCOORD7" );
				ic3CaseDefaultBreak();
			}

			return "";
		}

	}

}
